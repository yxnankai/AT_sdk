/**
@package : cx_3d library
@file : cx_3d_show_point_cloud.cpp
@brief C++ example of using cx_3d library for converting range image to point cloud.

This example shows how to read a range map from file and convert it to a point cloud.
The following steps are demonstrated:
	1. Load 3d calibration from file
	2. Load a range map image from file
	3. Optionally modify some calibration parameters
	4. Convert range image to point cloud and visualize point cloud.
	5. Save point cloud file, supported formats: ply, xyz, obj. Only saved if argv[4] was given on command line.
	6. Cleanup.

\note In order to enable OpenCV support in AT's C++ wrapper classes include OpenCV header before AT headers or manually define CX_SUPPORT_OPENCV.

@copyright (c) 2018, Automation Technology GmbH.
@version 04.09.2017, AT: initial version
@version 06.11.2017, AT: added functionality of saving range image and point cloud
@version 09.02.2018, AT: added plots for x, y, and z plane
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

// in order to enable OpenCV support in AT's C++ wrapper classes include OpenCV header before AT headers or manually define CX_SUPPORT_OPENCV
#include "opencv2/opencv.hpp"
#include "opencv2/viz.hpp"

// C++ Wrapper
#include "cx_3d_common.h"

using namespace std;
using namespace AT;

int main(int argc, char* argv[])
{
	// 1. load calibration from file
	std::string basePath	= "../../../cx3dLib/data/";
	std::string rangeImg_fname	= basePath + "img/AT-050614-2_Linear.tif";
	std::string calib_fname = basePath + "img/AT-050614-2_Linear_Full.xml";		// with H, K1, Skew, T, R
	std::string calib_id = "factory";
	std::string cloud_fname = basePath + "img/AT-050614-2_Linear.ply";						// supported formats: ply, xyz, obj

	if (argc > 1)
		rangeImg_fname = argv[1];
	if (argc > 2)
		calib_fname = argv[2];
	if (argc > 3)
		calib_id = argv[3];
	if (argc > 4)
		cloud_fname = argv[4];

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
		// In order to compensate for image croping or AOI-x-Windowing set X-offset in calibration.
		// calib.setParam(CX_3D_PARAM_S_ROI_X, cx::Variant(roi.x));

		// We can also compensate for x-reverse / flipped range image by setting a negative sensor ROI width.
		// calib.setParam(CX_3D_PARAM_S_ROI_W, cx::Variant(-roi.width));

		// We can also compensate for sensor y-reverse (Z-coordinate) by setting a negative sensor ROI height.
		// calib.setParam(CX_3D_PARAM_S_ROI_H, cx::Variant(-1024));

		// Range scaling of rangeImg pixel values, this is determined by the number of subpixels used. 
		// Changing this parameter after loading the calibration will invalidate the internal cache.
		//calib.setParam(CX_3D_PARAM_RANGE_SCALE, cx::Variant(1.0 / 64.0));

		// Optional set transformation parameter (rotation) from Anchor (Sensor-CS) to Transformed (World-CS) (defines Pose) = RotationX,Y,Z
		// calib.setParam(CX_3D_PARAM_R, cx::Variant(0.0*M_PI / 180.0, 0.0*M_PI / 180.0, 0.0*M_PI / 180.0));

		// Set invalid data value (IDV) that is used in the Point Cloud to indicate invalid range map pixel. Invalid range map pixels are pixel with value zero.
		// In this example we use the value NAN. This prevents the visualization of invalid points when using the cv::viz::Viz3d module.
		float ivd = NAN;	// could be any number e.g. -10.0f, if NAN is used the points are automatically suppressed in showPointCloud 
		calib.setParam(CX_3D_PARAM_METRIC_IDV, ivd);

		// Trigger update of internal cache. When calling with argument > 0, the internal cache gets updated.
		// This can be used in order to prevent a cache update at first call to function cx_3d_range2calibratedABC.
		calib.setParam(CX_3D_PARAM_METRIC_CACHE_MODE, cx::Variant(1));
		
		// 4. calculate point cloud
		cx::c3d::PointCloud pc(rangeImg.height(), rangeImg.width(), CX_PF_COORD3D_ABC32f, cx::Point3f(1.0f, 1.0f, 1.0f), cx::Point3f(0.0f, 0.0f, 0.0f));
		cx::c3d::calculatePointCloud(calib, rangeImg, pc);
		
		// 4.1 show point cloud using OpenCV Viz3d module
		cv::viz::Viz3d viz("Point Cloud");
		pc.computeNormals();										// compute normals from point cloud points
		cx::normalizeMinMax8U(rangeImg, pc.colors);					// compute colors from height values of range map. Function defined in cx_3d_common.
		cx::showPointCloud(viz, pc, "pc1", 2, cv::COLORMAP_JET+1);	// 0=only show points without normals and static color, 1=use colors, 2=use colors and normals. Function from cx_3d_common.

		// 5. optionally save PointCloud to file

		if (cloud_fname.length())
			pc.save(cloud_fname,true);

		// 6. release loaded calibration, free resources
		// cx::Calib automatically calls cx_3d_calib_release(hCalib) in Destructor

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
