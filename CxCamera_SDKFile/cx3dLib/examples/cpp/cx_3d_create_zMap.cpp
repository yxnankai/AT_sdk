/**
@package : cx_3d library
@file : cx_3d_create_zMap.cpp
@brief C++ example of using cx_3d library for converting range image to a rectified image in x-y Plane (Z-Map).

This example shows how to read a range map from file and convert it to a zMap.
The following steps are demonstrated:
	1. Load 3d calibration from file
	2. Load a range image from file
	3. optionally modify some calibration parameters
	4. Calculate rectified image from range image. 
	5. Calculate point cloud from rectified image and visualize point cloud.
	6. Cleanup.

\note In order to enable OpenCV support in AT's C++ wrapper classes include OpenCV header before AT headers or manually define CX_SUPPORT_OPENCV.

@copyright (c) 2018, Automation Technology GmbH.
@version 26.11.2018, AT: initial version
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
#include <numeric>
#include <chrono>

// In order to enable OpenCV support in AT's C++ wrapper classes include OpenCV header before AT headers or manually define CX_SUPPORT_OPENCV
#include "opencv2/opencv.hpp"
#include "opencv2/viz.hpp"

// C++ Wrapper
#include "cx_3d_common.h"
#include "AT/cx/CVUtils.h"

using namespace std;
using namespace AT;


int main(int argc, char* argv[])
{
	// 1. load calibration from file
	std::string basePath	= "../../../cx3dLib/data/";
	std::string rangeImg_fname	= basePath + "img/AT-050614-2_Linear.tif";
	std::string calib_fname = basePath + "img/AT-050614-2_Linear_Full.xml";		// with H, K1, Skew, T, R
	std::string calib_id = "factory";
	std::string zMap_fname = basePath + "temp/AT-050614-2_Linear_zMap.tif";

	if (argc > 1)
		rangeImg_fname = argv[1];
	if (argc > 2)
		calib_fname = argv[2];
	if (argc > 3)
		zMap_fname = argv[3];

	int major, minor, build;
	cx_3d_lib_version(&major, &minor, &build);
	printf("cx_3d_lib version %d.%2d.%d", major, minor, build);

	try
	{
		// 1. load calibration from file
		cx::c3d::Calib calib;
		calib.load(calib_fname, calib_id);

		// print info about loaded calibration
		cx::printInfo(calib);

		// 2. load range image
		cx::Image rangeImg;
		rangeImg.load(rangeImg_fname);
	
		// 3. optional modify calibration parameter, some examples:
		// Range scaling of rangeImg pixel values, this is determined by the number of subpixels used. 
		// Changing this parameter after loading the calibration will invalidate the internal cache.
		//calib.setParam(CX_3D_PARAM_RANGE_SCALE, cx::Variant(1.0 / 64.0));

		// Optional set transformation parameter (rotation) from Anchor (Sensor-CS) to Transformed (World-CS) (defines Pose) = RotationX,Y,Z
		// calib.setParam(CX_3D_PARAM_R, cx::Variant(0.0*M_PI / 180.0, 0.0*M_PI / 180.0, 0.0*M_PI / 180.0));

		// Set invalid data value (IDV). That value is used to mark invalid values in rectified image.
		// Zero range values are considered invalid.
		float ivd = NAN;	// could be any number e.g. -10.0f, if NAN is used the points are automatically suppressed in showPointCloud 
		calib.setParam(CX_3D_PARAM_METRIC_IDV, ivd);

		// Trigger update of internal cache. When calling with argument > 0, the internal cache gets updated.
		// This can be used to prevent a longer exectution time due to cache update at first call to function cx_3d_range2calibratedABC.
		calib.setParam(CX_3D_PARAM_METRIC_CACHE_MODE, cx::Variant(1));
		
		// 4. calculate rectified image
		// 4.1 set scaling for rectified image: x = -30...+30, y = 0...100, z = -10...30, dx=600, dy=1000, dz=4000
		unsigned zMap_dx = 600;		// with 60mm object-dx  we get 0.01 mm X-resolution
		unsigned zMap_dy = 1000;	// with 100mm object-dy we get 0.01 mm Y-resolution
		unsigned zMap_dz = 4000;	// with 40mm object-dz  we get 0.001 mm Z-resolution
		float obj_dx = 60.0f;
		float obj_dy = 100.0f;
		float obj_dz = 40.0f;
		cx::Point3f offset(-obj_dx / 2.0f, 0.0f, -10.0f);
		cx::Point3f scale(obj_dx / float(zMap_dx), obj_dy / float(zMap_dy), obj_dz / float(zMap_dz));

		// 4.2 calculate the rectified image (Z-Map) 
		// possible pixel formats: CX_PF_COORD3D_C32f, CX_PF_COORD3D_C16
		// Note: We always ignore range values of zero, also when flag CX_3D_METRIC_MARK_Z_INVALID_DATA is not set
		cx::c3d::ZMap zMap(zMap_dy, zMap_dx, CX_PF_COORD3D_C32f, scale, offset);
		auto t1 = std::chrono::high_resolution_clock::now();
		cx::c3d::calculateZMap(calib, rangeImg, zMap, CX_3D_METRIC_MARK_Z_INVALID_DATA | CX_3D_METRIC_INTERP_IDW);
		auto t2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = t2 - t1;	// seconds
		std::cout << "time elapsed for transformation to rectified image: " << elapsed.count() << endl;

		// 5. convert rectified image to point cloud
		// Invalid points in point cloud are set to NAN in order to supress the point in showPointCloud
		cx::c3d::PointCloud pc;
		cx::c3d::convertToPointCloud(zMap, pc, ivd);

		// 5.1 show rectified image as point cloud
		cv::viz::Viz3d viz("Rectified Point Cloud");
		pc.computeNormals();											// compute normals from point cloud points
		cx::normalizeMinMax8U(zMap.img, pc.colors);						// compute colors from height values of range map. Function defined in cx_3d_common.
		cx::showPointCloud(viz, pc, "pc1", 2, cv::COLORMAP_JET + 1);	// 0=only show points without normals and static color, 1=use colors, 2=use colors and normals. Function from cx_3d_common.

		// 5.2 show rectified image as image 
		cv::imshow("Rectified image (zMap)", cx::cvUtils::imageCopyToMat(pc.colors,false));

		// 5.3 save rectified image
		zMap.save(zMap_fname);

		// 6. release loaded calibration, free resources
		// cx::Calib automatically calls cx_3d_calib_release(hCalib) in Destructor
		
		cout << "CS: red=x, green=y, blue=z" << endl;
		cout << "3D-View: press 'q' for quit or 'h' for help" << endl;
		viz.spin();	// Wait for a keystroke in the OpenCV::Viz window
	}
	catch (std::exception& e)
	{
		cout << "exception caught, msg:" << e.what();
		exit(-3);
	}
    return 0;
}
