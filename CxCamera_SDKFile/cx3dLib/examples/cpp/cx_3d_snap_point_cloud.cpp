/**
@package : cx_3d library
@file : cx_3d_snap_point_cloud.cpp
@brief C++ example using cx_3d library, shows howto to snap a single range map from CX camera and convert it to a point cloud.

This example shows how to snap a single range map from CX camera and convert it to a point cloud. 
The following steps are demonstrated:
	1. Discover, select and connect a camera device, see \ref AT::cx::discoverAndChooseDevice
	2. Load 3d calibration from camera or file, see \ref AT::cx::downloadCalib
	3. Set relevant acquisition parameters in calibration, see \ref AT::cx::updateCalibC5
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

@copyright (c) 2017, Automation Technology GmbH.
@version 04.09.2017, AT: initial version
@version 06.11.2017, AT: added functionality of saving range image and point cloud
@version 08.12.2018, AT: make use of latest wrapper classes
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

		cx::Variant val;
		cam->getParam("DeviceScanType", val);
		if (std::string(val) != "Linescan3D")
		{
			cerr << "Current sensor devicemode is not 'Linescan3D', we activate it..." << endl;
			cam->setParam("DeviceScanType", "Linescan3D");
		}

		// 2. load 3d-calibration from camera or file
		cx::c3d::Calib calib;
		if (argc > 1)
		{
			// load the calibration from the given file
			calib.load(argv[1]);
		}
		else
		{	// load calibration from sensor, possible sources are "UserData", "Calibration", "CalibrationUser"
			cx::downloadCalib(cam->getHandle(), calib);
		}

		// 3. set relevant acquisition parameters in calibration, we assume we have only one aoi active
		cx::updateCalibC6(cam->getHandle(), calib, 1);

		// set the profile-step-width scaling
		calib.getParam(CX_3D_PARAM_SY, val);
		std::cout << "enter profile distance, current value=" << (double)val << ", new value: ";
		std::string userInput;
		cin >> userInput;
		if (!userInput.empty())
		{
			double dy = std::stod(userInput);
			calib.setParam(CX_3D_PARAM_SY, dy);
		}

		// print updated calibration that is used to compute the point cloud
		cx::printInfo(calib.getHandle());

		// set the invalid value that will bes used in point cloud for all rangemap values that are zero 
		float ivd = NAN;	// could be any number e.g. -10.0f, if NAN is used the points are automatically suppressed in showPointCloud 
		calib.setParam(CX_3D_PARAM_METRIC_IDV, ivd);

		// Trigger cache update. When calling with argument > 0, the internal cache gets updated. 
		// This prevents a cache update at first call to metric functions cx_3d_range2calibratedABC.
		calib.setParam(CX_3D_PARAM_METRIC_CACHE_MODE, int(1));

		// 4. Allocate and queue internal acquisition buffers
		cam->allocAndQueueBuffers();

		// 5. Start image acquisition
		cam->startAcquisition();

		// 6. Grab acquisition buffer, wait for valid buffer with optional timeout. Timeout is given in ms.
		cx::DeviceBuffer buffer = cam->waitForBuffer(10000);

		// 7. get image data from buffer and do some processing on the image data (or get a copy for later use)
		// \note img holds a reference to the image data in the DeviceBuffer, if you need the image data after cx_queueBuffer you need to clone the image!
		{
			auto rangeImg = buffer.getImage();

			// optionally save range image to file
			if (argc > 2)
				rangeImg->save(argv[2]);

			// 8. calculate point cloud
			cx::c3d::PointCloud pc(rangeImg->height(),rangeImg->width());
			cx::c3d::calculatePointCloud(calib, *rangeImg, pc);

			// 9. show point cloud using OpenCV Viz3d module
			cv::viz::Viz3d viz("Point Cloud");
			pc.computeNormals();											// compute normals from point cloud points
			cx::normalizeMinMax8U(*rangeImg, pc.colors);					// compute colors from height values of range map. Function defined in cx_3d_common.
			cx::showPointCloud(viz, pc, "pc1", 2, cv::COLORMAP_JET + 1);	// 0=only show points without normals and static color, 1=use colors, 2=use colors and normals. Function from cx_3d_common.

			// show range image in OpenCV window
			cv::imshow("Range Image", cx::cvUtils::imageCopyToMat(pc.colors, false));

			// 10. optionally save PointCloud to file
			if (argc > 3)
				pc.save(argv[3]);
		}

		// 11. Queue back the buffer to the devices acquisition engine.
		// \note From now on any references to the buffer images are not valid anymore and might be overwritten with new image data at any time!
		buffer.queueBuffer();

		// 12. Stop acquisition
		cam->stopAcquisition();

		// 13. Cleanup
		cam->freeBuffers();
		cam->close();

		cout << "CS: red=x, green=y, blue=z" << endl;
		cout << "3D-View: press 'q' for quit or 'h' for help" << endl;
		viz.spin();	// Wait for a keystroke in the OpenCV window
	}
	catch (std::exception& e)
	{
		cout << "exception caught, msg:" << e.what();
		exit(-3);
	}
	return 0;
}
