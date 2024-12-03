/**
@package CxCamLib library
@file cx_cam_grab_gev_multipart.cpp
@brief CPP example of using AT Camera library.
@copyright (c) 2017, Automation Technology GmbH.
@version 04.09.2017, AT: initial version
@version 02.08.2018, AT: auto-scale image view to Mono8
@version 12.10.2018, AT: rewritten example using cpp-wrapper classes

This example shows how to snap a single frame with gev multipart payload. We use the C++ wrapper classes for ease of use, see \ref AT::cx::DeviceFactory, \ref AT::cx::Device, \ref AT::cx::DeviceBuffer.
1. Discover and connect a device, see \ref cx_dd_findDevices.
2. Detect if the device supports GeV multipart and activate it
3. Probe and set device settings:
	Switch DeviceScanType to "Linescan3D" and activate components Range, Reflectance and Scatter
4. Allocate and queue internal buffers, see \ref cx_allocAndQueueBuffers.
5. Start acquisition.
6. Grab acquisition buffer, wait for valid buffer with optional timeout. Timeout is given in ms.
7. Iterate the buffer parts and do some processing on the image data or get a copy for later use.
	\note img holds a reference to the image data in the DeviceBuffer, if you need the image data after cx_queueBuffer you need to copy it!
8. Queue back the buffer to the devices acquisition engine.
	\note From now on img is not valid anymore and might be overwritten with new image data at any time!
9. Stop acquisition.
10. Cleanup.
*/
/*************************************************************************************
THIS SOFTWARE IS PROVIDED BY AUTOMATION TECHNOLOGY GMBH "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL AUTOMATION TECHNOLOGY GMBH BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*************************************************************************************/
#include <string>
#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "AT/cx/CVUtils.h"
using namespace cv;

#include "cx_cam_common.h"
#include "AT/cx/c3d/Calib.h"
#include "AT/cx/c3d/PointCloud.h"
#include "cx_3d_common.h"
using namespace AT;

int main(int argc, char* argv[])
{
	try
	{
		cx::Variant val;

		// 1. discover and connect a device.
		std::string uri= gev://192.168.110.39/?mac=8c-1f-64-69-e2-e3&nif=C4-00-AD-8B-F1-F9;
		/*
		if (argc > 1)
			uri = argv[1];
		else
			uri = cx::discoverAndChooseDevice(true)->deviceURI;
		*/

		auto cam = cx::DeviceFactory::openDevice(uri);

		// 2. Detect if the device supports GeV multipart and activate it
		try
		{
			cam->getParam("GevSCCFGMultiPart", val);
			val.set((bool)true);
			cam->setParam("GevSCCFGMultiPart", val);
		}
		catch (...)
		{
			std::cerr << "camera doesn't support GeV multipart" << endl;
			exit(-3);
		}

		// 3. Probe and adjust device settings:
		try
		{
			cam->getParam("DeviceScanType", val);
		}
		catch (...)
		{
			std::cerr << "device doesn't support 3D" << endl;
			exit(-3);
		}

		//	- switch DeviceScanType to "Areascan"
		try
		{
			val.set("Areascan");
			cam->setParam("DeviceScanType", val);
		}
		catch (...)
		{
			std::cerr << "cannot switch device into Areascan mode" << endl;
			exit(-3);
		}

		//	- define and activate 2 regions
		unsigned int numRegions = 2;
		try
		{
			val.set("Region0");
			cam->setParam("RegionSelector", val);
			val.set(108);
			cam->setParam("Height", val);
			val.set(0);
			cam->setParam("OffsetY", val);
			val.set("On");
			cam->setParam("RegionMode", val);

			val.set("Region1");
			cam->setParam("RegionSelector", val);
			val.set(108);
			cam->setParam("Height", val);
			val.set(108);
			cam->setParam("OffsetY", val);
			val.set("On");
			cam->setParam("RegionMode", val);
		}
		catch (...)
		{
			std::cerr << "cannot set regions" << endl;
			exit(-3);
		}

		//	- switch DeviceScanType to "Linescan3D" and activate Range components
		try
		{
			val.set("Linescan3D");
			cam->setParam("DeviceScanType", val);
		}
		catch (...)
		{
			std::cerr << "cannot switch device into Linescan3D mode" << endl;
			exit(-3);
		}

		try
		{
			val.set("Scan3dExtraction0");
			cam->setParam("RegionSelector", val);
			val.set("Range");
			cam->setParam("ComponentSelector", val);
			val.set(true);
			cam->setParam("ComponentEnable", val);
			val.set("On");
			cam->setParam("RegionMode", val);

			val.set("Scan3dExtraction1");
			cam->setParam("RegionSelector", val);
			val.set("Range");
			cam->setParam("ComponentSelector", val);
			val.set(true);
			cam->setParam("ComponentEnable", val);
			val.set("On");
			cam->setParam("RegionMode", val);
		}
		catch (...)
		{
			std::cerr << "cannot set components" << endl;
			exit(-3);
		}

		// 3. load 3d-calibration from camera or file
		std::vector<cx::c3d::CalibPtr> calibs;
		double dy = std::numeric_limits<double>::quiet_NaN();

		for (unsigned int region = 0; region < numRegions; region++)
		{
			calibs.push_back(cx::c3d::Calib::createShared());     //这里应该就是创造某个calib，

			if (argc > 1)
			{
				// load the calibration from the given file
				calibs[region]->load(argv[1]);
			}
			else
			{	// load calibration from sensor, possible sources are "UserData", "Calibration", "CalibrationUser"
				cx::downloadCalib(cam->getHandle(), *calibs[region]);
			}

			// 3. set relevant acquisition parameters in calibration, we assume we have only one aoi active
			cx::updateCalibC6(cam->getHandle(), *calibs[region], region);       //应该是开启相机获得了对应权限

			// set the profile-step-width scaling
			calibs[region]->getParam(CX_3D_PARAM_SY, val);

			if (region == 0)	// query profile distance only once
			{
				std::cout << "enter profile distance, current value=" << (double)val << ", new value: ";
				std::string userInput;
				cin >> userInput;
				if (!userInput.empty())
				{
					dy = std::stod(userInput);
				}
			}

			if (!isnan(dy))
			{
				calibs[region]->setParam(CX_3D_PARAM_SY, dy);
			}

			// print updated calibration that is used to compute the point cloud
			cx::printInfo(calibs[region]->getHandle());

			char buffer[100];

			sprintf(buffer, "%s_%d", "CalibAOI", region);

			calibs[region]->save(std::string(buffer), "");

			// set the invalid value that will bes used in point cloud for all rangemap values that are zero 
			float ivd = NAN;	// could be any number e.g. -10.0f, if NAN is used the points are automatically suppressed in showPointCloud 
			calibs[region]->setParam(CX_3D_PARAM_METRIC_IDV, ivd);

			// Trigger cache update. When calling with argument > 0, the internal cache gets updated. 
			// This prevents a cache update at first call to metric functions cx_3d_range2calibratedABC.
			calibs[region]->setParam(CX_3D_PARAM_METRIC_CACHE_MODE, int(1));
		}

		// 4. Allocate and queue internal acquisition buffers
		cam->allocAndQueueBuffers();

		// 5. Start image acquisition
		cam->startAcquisition();

		// 6. Grab acquisition buffer, wait for valid buffer with optional timeout. Timeout is given in ms.
		cx::DeviceBuffer buffer = cam->waitForBuffer(3000);

		buffer.getInfo(CX_BUFFER_INFO_IS_MULTIPART, val);
		if (!(bool)val)
		{
			std::cerr << "buffer is not multipart" << endl;
			exit(-3);
		}

		buffer.getInfo(CX_BUFFER_INFO_NUM_PARTS, val);
		int numParts = val;
		std::cout << "Number of Parts: " << numParts << "\n";

		buffer.getInfo(CX_BUFFER_INFO_NUM_CHUNK, val);
		int numChunks = val;
		std::cout << "Number of Chunks: " << numChunks << "\n\n";

		// 7. Iterate the buffer parts and do some processing on the image data or get a copy for later use.
		// \note img holds a reference to the image data in the DeviceBuffer, if you need the image data after cx_queueBuffer you need to clone the image!

		std::string title = "";
		unsigned int region = 0;
		bool isImagePart = false;
		bool isRangeImage = false;

		for (int partIdx = 0; partIdx < numParts; partIdx++)
		{
			title = "";
			isImagePart = false;

			title += "Part: " + std::to_string(partIdx) + ", ";
			std::cout << "Part " << partIdx << ":\n";

			buffer.getPartInfo(partIdx, CX_BUFFER_PART_INFO_REGION_ID, val);
			title += "RegionID:" + std::to_string((int)val) + ", ";
			std::cout << " RegionID: " << (int)val << "\n";
			region = (unsigned int)val & 0xf;

			buffer.getPartInfo(partIdx, CX_BUFFER_PART_INFO_TYPE_ID, val);
			if ((int)val == CX_BUFFER_PART_TYPE_ID_IMAGE2D)
			{
				isImagePart = true;
			}
			title += "TypeID: " + cx::partTypeIDToStr((cx_buffer_part_type)((int32_t)val)) + ", ";
			std::cout << " TypeID: " << cx::partTypeIDToStr((cx_buffer_part_type)((int32_t)val)) << "\n";

			buffer.getPartInfo(partIdx, CX_BUFFER_PART_INFO_DATA_PURPOSE_ID, val);
			title += "PurposeID: " + cx::partPurposeIDToStr((cx_buffer_part_purpose)((int32_t)val));
			std::cout << " PurposeID: " << cx::partPurposeIDToStr((cx_buffer_part_purpose)((int32_t)val)) << "\n\n";
			if ((int)val == CX_BUFFER_PART_PURPOSE_ID_RANGE)
			{
				isRangeImage = true;
			}

			if (isImagePart && isRangeImage)
			{
				auto rangeImg = buffer.getImage(partIdx);

				// 8. calculate point cloud
				cx::c3d::PointCloud pc(rangeImg->height(), rangeImg->width());
				cx::c3d::calculatePointCloud(*calibs[region], *rangeImg, pc);

				// 9. show point cloud using OpenCV Viz3d module
				cv::viz::Viz3d viz("Point Cloud");
				pc.computeNormals();											// compute normals from point cloud points
				cx::normalizeMinMax8U(*rangeImg, pc.colors);					// compute colors from height values of range map. Function defined in cx_3d_common.
				//pc.save("pc.ply");
				cx::showPointCloud(viz, pc, "pc1", 2, cv::COLORMAP_JET + 1);	// 0=only show points without normals and static color, 1=use colors, 2=use colors and normals. Function from cx_3d_common.

				cv::imshow(title, cx::cvUtils::imageCopyToMat(pc.colors, false));	// show range image in OpenCV window

				// 10. optionally save PointCloud to file
				if (argc > 3)
					pc.save(argv[3]);

				std::cout << "CS: red=x, green=y, blue=z" << endl;
				std::cout << "3D-View: press 'q' for quit or 'h' for help" << endl << endl;
				viz.spin();	// Wait for a keystroke in the OpenCV window
			}

		}

		// 8. Queue back the buffer to the devices acquisition engine.
		// \note From now on any references to the buffer images are not valid anymore and might be overwritten with new image data at any time!
		buffer.queueBuffer();

		// 9. Stop acquisition
		cam->stopAcquisition();

		// 10. Cleanup
		cam->freeBuffers();
		cam->close();
	}
	catch (const cx::RuntimeError& err)
	{
		std::cerr << "cx runtime exception: " << err.what() << endl;
		exit(-3);
	}
	return 0;
}
