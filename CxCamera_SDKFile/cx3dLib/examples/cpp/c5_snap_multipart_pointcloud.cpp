/**
@package : cx_3d library
@file : c5_snap_multipart_pointcloud.cpp
@brief C++ example using cx_3d library, shows howto to snap a single range map from CX camera and convert it to a point cloud.

This example shows how to snap a single range map from CX camera and convert it to a point cloud. 
The following steps are demonstrated:
	1. Discover, select and connect a camera device, see \ref AT::cx::discoverAndChooseDevice
	2. Load 3d calibration from camera or file, see \ref AT::cx::downloadCalib
	3. Set relevant acquisition parameters in calibration, see \ref AT::cx::updateCalib
	4. Allocate and queue internal buffers.
	5. Start acquisition.
	6. Grab an image buffer.
	7. Get range image from buffer
	8. Convert range image to point cloud
	9. Save range image and point cloud to file
	10. Visualize point cloud
	11. Queue back the image buffer.
	12. Stop acquisition.
	13. Cleanup.

\note In order to enable OpenCV support in AT's C++ wrapper classes include OpenCV header before AT headers or manually define CX_SUPPORT_OPENCV.

@copyright (c) 2020, Automation Technology GmbH.
@version 14.02.2020, AT: initial version
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

// In order to enable OpenCV support in AT's C++ wrapper classes include OpenCV header before AT headers or manually define CX_SUPPORT_OPENCV
#include "opencv2/opencv.hpp"
#include "opencv2/viz.hpp"

// C++ Wrapper
#include "cx_cam_common.h"
#include "cx_3d_common.h"
#include "AT/cx/CVUtils.h"

using namespace std;
using namespace AT;


int main(int argc, char* argv[])
{
	try
	{
		// 1a. discover devices and choose the one to work with
		std::string uri = cx::discoverAndChooseDevice(true)->deviceURI;
		std::cout << "Open Device: " << uri.c_str() << endl;

		// 1b. connect camera
		auto cam = cx::DeviceFactory::openDevice(uri);

		// visualization frame
		cv::viz::Viz3d viz("Point Cloud");


		// Probe and set device settings:
		cx::Variant val;

		uint32_t numAOIs = 1;		// the number of AOIs, default is 1

		// a) Scan type of camera
		cam->getParam("DeviceScanType", val);
		if (std::string(val) != "Linescan3D")
		{
			cerr << "Current sensor devicemode is not 'Linescan3D', we activate it..." << endl;
			cam->setParam("DeviceScanType", "Linescan3D");
		}

		// b) Number of AOIs
		cam->getParam("NumAois", val);
		numAOIs = uint32_t(val);


		// c) Data Output Channels
		int numDC = 0;		// number of enabled data channels
		bool enableDC[3];

		// data channel 0 enabled flag
		cam->getParam("EnableDC0", val);
		enableDC[0] = (int)val >= 1;
		if (enableDC[0]) numDC++;

		// data channel 1 enabled flag
		cam->getParam("EnableDC1", val);
		enableDC[1] = (int)val >= 1;
		if (enableDC[1]) numDC++;

		// data channel 2 enabled flag
		cam->getParam("EnableDC2", val);
		enableDC[2] = (int)val >= 1;
		if (enableDC[2]) numDC++;

		// 2. load 3d-calibration from camera or file

		std::vector<cx::c3d::CalibPtr> calibs;
		double dy = std::numeric_limits<double>::quiet_NaN();

		for (unsigned int aoi = 0; aoi < numAOIs; aoi++)
		{
			calibs.push_back(cx::c3d::Calib::createShared());

			if (argc > 1)
			{
				// load the calibration from the given file
				calibs[aoi]->load(argv[1]);
			}
			else
			{	// load calibration from sensor, possible sources are "UserData", "Calibration", "CalibrationUser"
				cx::downloadCalib(cam->getHandle(), *calibs[aoi]);
			}

			// 3. set relevant acquisition parameters in calibration, we assume we have only one aoi active      
			cx::updateCalibC5(cam->getHandle(), *calibs[aoi], aoi+1);

			// set the profile-step-width scaling
			calibs[aoi]->getParam(CX_3D_PARAM_SY, val);

			if (aoi == 0)	// query profile distance only once
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
				calibs[aoi]->setParam(CX_3D_PARAM_SY, dy);
			}

			// print updated calibration that is used to compute the point cloud
			cx::printInfo(calibs[aoi]->getHandle());

			char buffer[100];

			sprintf(buffer, "%s_%d", "CalibAOI", aoi);

			calibs[aoi]->save(std::string(buffer),"");

			// set the invalid value that will bes used in point cloud for all rangemap values that are zero 
			float ivd = NAN;	// could be any number e.g. -10.0f, if NAN is used the points are automatically suppressed in showPointCloud 
			calibs[aoi]->setParam(CX_3D_PARAM_METRIC_IDV, ivd);

			// Trigger cache update. When calling with argument > 0, the internal cache gets updated. 
			// This prevents a cache update at first call to metric functions cx_3d_range2calibratedABC.
			calibs[aoi]->setParam(CX_3D_PARAM_METRIC_CACHE_MODE, int(1));
		}

		// 4. Allocate and queue internal acquisition buffers
		cam->allocAndQueueBuffers();

		// 5. Start image acquisition
		cam->startAcquisition();

		// 6. Grab acquisition buffer, wait for valid buffer with optional timeout. Timeout is given in ms.
		cx::DeviceBuffer buffer = cam->waitForBuffer(10000);

		// 7. get image data from buffer and do some processing on the image data (or get a copy for later use)
		// \note img holds a reference to the image data in the DeviceBuffer, if you need the image data after cx_queueBuffer you need to clone the image!
		{
			auto frameImg = buffer.getImage();

			// optionally save range image to file
			if (argc > 2)
				frameImg->save(argv[2]);


			// E.g. for two AOIS and DC1 and DC2 active, frameImg->data is aligned as follows: 
			//		(AOI1,DC1,profile#1) 
			//		(AOI1,DC2,profile#1)  
			//		(AOI2,DC1,profile#1) 
			//		(AOI2,DC2,profile#1)  
			//		... 
			//		(AOI1,DC1,profile#n) 
			//		(AOI1,DC2,profile#n)  
			//		(AOI2,DC1,profile#n) 
			//		(AOI2,DC2,profile#n)
			// Reinterprete frameImg->data as if data channels were aligned side by side:
			//		(AOI1,DC1,profile#1),(AOI1,DC2,profile#1),(AOI2,DC1,profile#1),(AOI2,DC2,profile#1)
			//		... 
			//		(AOI1,DC1,profile#n),(AOI1,DC2,profile#n),(AOI2,DC1,profile#n),(AOI2,DC2,profile#n)

			cv::Mat imgmat(frameImg->height() / (numDC * numAOIs), frameImg->width() * numDC * numAOIs, cx::cvUtils::pf2cv(frameImg->pixelFormat()), frameImg->data());

			std::vector<std::vector<cv::Mat>> images;
			images.resize(numAOIs);

			cx::Image rangeImg;

			// split the multichannel/multiaoi image into separate images for each available data channel
			for (unsigned int aoi = 0; aoi < numAOIs; aoi++)
			{
				images[aoi].resize(numDC);

				int imgCh = 0;
				for (int dc = 0; dc < 3; dc++)
				{
					if (enableDC[dc])
					{
						cv::Rect roi((int)frameImg->width() * numDC * aoi + (int)frameImg->width() * imgCh, 0, (int)frameImg->width(), (int)frameImg->height() / (numAOIs * numDC));
						images[aoi][imgCh] = cv::Mat(imgmat, roi);			// get DC as submatrix from imgmat

						if (dc == 2)
						{
							rangeImg = cx::cvUtils::imageCreate(images[aoi][imgCh]);

							// 8. calculate point cloud
							cx::c3d::PointCloud pc(rangeImg.height(), rangeImg.width());
							cx::c3d::calculatePointCloud(*calibs[aoi], rangeImg, pc);

							// 9. show point cloud using OpenCV Viz3d module
							cv::viz::Viz3d viz("Point Cloud");
							pc.computeNormals();											// compute normals from point cloud points
							cx::normalizeMinMax8U(rangeImg, pc.colors);						// compute colors from height values of range map. Function defined in cx_3d_common.
							//pc.save("pc.ply");
							cx::showPointCloud(viz, pc, "pc1", 2, cv::COLORMAP_JET + 1);	// 0=only show points without normals and static color, 1=use colors, 2=use colors and normals. Function from cx_3d_common.

																							// show range image in OpenCV window
							cv::imshow("Range Image", cx::cvUtils::imageCopyToMat(pc.colors, false));

							// 10. optionally save PointCloud to file
							if (argc > 3)
								pc.save(argv[3]);

							std::cout << "CS: red=x, green=y, blue=z" << endl;
							std::cout << "3D-View: press 'q' for quit or 'h' for help" << endl;
							viz.spin();	// Wait for a keystroke in the OpenCV window

						}

						imgCh++;
					}
				}
			}
		}

		// 11. Queue back the buffer to the devices acquisition engine.
		// \note From now on any references to the buffer images are not valid anymore and might be overwritten with new image data at any time!
		buffer.queueBuffer();

		// 12. Stop acquisition
		cam->stopAcquisition();

		// 13. Cleanup
		cam->freeBuffers();
		cam->close();

		//std::cout << "CS: red=x, green=y, blue=z" << endl;
		//std::cout << "3D-View: press 'q' for quit or 'h' for help" << endl;
		//viz.spin();	// Wait for a keystroke in the OpenCV window
	}
	catch (std::exception& e)
	{
		std::cout << "exception caught, msg:" << e.what();
 		exit(-3);
	}
	return 0;
}
