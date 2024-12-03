/**
@package : cx_3d library
@file : cx_3d_calib_TargetFacets12.cpp
@brief CPP example of using AT 3D library for calibration from a rangemap of a Facets12 calibration target.

This example shows how to do an intrinsic calibration based on the Facet12 calibration target.
The following steps are demonstrated:
-# Load range map, target description and calibration default values (start values) from file
-# Optionally select ROI in range map
-# Load Target description from xml
-# Do Target point extraction
-# Set calibration parameters
-# Do instrinsic calibration
-# Visualize results
-# Cleanup

@copyright (c) 2017, Automation Technology GmbH.
@version 04.09.2017, AT: initial version
@version 06.02.2019, AT: cleanup
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

#include <assert.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include "opencv2/opencv.hpp"

#include "cx_3d_common.h"
#include "cx_3d_calib.h"
#include "cx_3d_calib_int.h"
#include "AT/cx/CVUtils.h"

using namespace std;
using namespace AT;

int main(int argc, char* argv[])
{
	//CX_TARGET_HANDLE hTarget = CX_INVALID_HANDLE;
	//CX_CALIB_HANDLE hCalib = CX_INVALID_HANDLE;
	cx::Variant val;
	std::string basePath = "../../../cx3dLib/data/";

	// example data at0506142
	std::string target_fname = basePath + "xml/targets.xml";				// file with target descriptions
	std::string targetId = "at0506142";										// id of target in description file
	std::string img_fname = basePath + "img/AT-050614-2_Linear.tif";		// range map of target
	std::string calib_fname = basePath + "img/AT-050614-2_Linear.xml";		// default calibration values and description (start values)
	std::string result_fname = basePath + "temp/calib_from_facets12.xml";	// result file


	// load rangemap
	cv::Mat rimgFull = cv::imread(img_fname, cv::IMREAD_ANYDEPTH);
	if (rimgFull.empty())
	{
		cerr << "empty rangemap" << endl;
		exit(-2);
	}
	// optional flip image in Y- direction in case the target moving direction does not match target point definition
	//cv::flip(rimgFull, rimgFull, 0);

	// add a ramp image for testing the detection algorithm
	//cx::addRamp(rimgFull, 10.0, 15.0);

	// inverse Z-range for testing
#if 0
	cv::Mat zeroMask = rimgFull == 0;
	rimgFull = 60000 - rimgFull;
	rimgFull.setTo(0, zeroMask);
#endif

	// select ROI in loaded image
	cv::namedWindow("Range Image", cv::WINDOW_NORMAL);
	cv::resizeWindow("Range Image", 600, 600);
	cv::Rect roi = cv::selectROI("Range Image", rimgFull, true, false);	// selectROI available since OpenCV 3.2
	cv::destroyWindow("Range Image");
	cv::Mat rangeImgMat;
	if (roi.empty())
		rangeImgMat = rimgFull;		// if cancel 'c' was pressed use full window
	else
		rangeImgMat = rimgFull(roi);	// crop image to selected area with reference, for real copy use rimgFull(roi).copyTo(rimg)
	cx::Image rangeImg = cx::cvUtils::imageCreate(rangeImgMat);	// create cx_3d image from OpenCV image

	// load target
	cx::c3d::Target target;
	target.load(target_fname.c_str(), targetId.c_str());

	// extract points from target
	target.findPoints(rangeImg, (cx_3d_target_flags)0);

	// show info about target
	cx::printTargetInfo(target.getHandle());
	cx::showTargetResults(target.getHandle(), rangeImgMat.size(), 0.7);

	// get points from target
	std::vector<cx_point3r_t> ip, tp;
	target.getParam(CX_3D_PARAM_TARGET_TP, val);
	val.get(tp);
	target.getParam(CX_3D_PARAM_TARGET_IP, val);
	val.get(ip);
	assert(tp.size() == ip.size());

	// load "start values" from calib file and set some more parameters manually
	cx::c3d::Calib calib;
	calib.load(calib_fname.c_str(), "");
	//cx::checkOk( cx_3d_calib_load(calib_fname.c_str(), "", CX_3D_CALIB_FORMAT_XML, &hCalib) );

	// set model and start parameters
	//cx::checkOk(cx_3d_calib_set(hCalib, CX_3D_PARAM_MODEL, cx::Variant(CX_3D_CALIB_MODEL_P_HOM | CX_3D_CALIB_MODEL_L_DIV)));
	calib.setParam(CX_3D_PARAM_MODEL, cx::Variant(CX_3D_CALIB_MODEL_P_HOM));
	// set start parameter for principal point
	//calib.setParam(CX_3D_PARAM_CP, cx::Variant(double(rimgFull.cols) / 2.0, 1024.0 / 2.0));
	// in order to compensate for croping set X-offset in metric calibration
	if (!roi.empty())
		calib.setParam(CX_3D_PARAM_S_ROI_X, cx::Variant(roi.x));
	// range scaling of ip points
	//cx::checkOk(cx_3d_calib_set(hCalib, CX_3D_PARAM_RANGE_SCALE, cx::Variant(1.0/64.0)));
	// estimate profile step width from last-first point along y-axis, this is valid for facet12 target
	double dy = (tp[11].b - tp[0].b) / (ip[11].b - ip[0].b);
	calib.setParam(CX_3D_PARAM_SY, cx::Variant(dy));

	cout << "Start values:" << endl;
	cx::printInfo(calib.getHandle());

	int ext_calib_flags = CX_3D_CALIB_SOLVE_TX | CX_3D_CALIB_SOLVE_TY | CX_3D_CALIB_SOLVE_TZ;	// object translation 
	ext_calib_flags |= CX_3D_CALIB_SOLVE_RX | CX_3D_CALIB_SOLVE_RY | CX_3D_CALIB_SOLVE_RZ;		// object rotation
	ext_calib_flags |= CX_3D_CALIB_SOLVE_SXY | CX_3D_CALIB_SOLVE_SZY;	// shear
	//ext_calib_flags |= CX_3D_CALIB_SOLVE_SY;							// scaling in Y direction

	for (int j = 0; j < 2; j++)
	{
		// estimate intrinsic model, this does not include any extrinsic parameters, only mapping from sensor-plane to laser-plane
		calib.calibrateIntrinsicFromPoints(ip.data(), tp.data(), (unsigned)ip.size(), (cx_3d_calib_int_flags)0);	// CX_3D_CALIB_SOLVE_L_K2, CX_3D_CALIB_SOLVE_C_P3, CX_3D_CALIB_FIX_CP | CX_3D_CALIB_FIX_L_K1
		// estimate extrinsic model
		calib.calibrateExtrinsicFromPoints(ip.data(), tp.data(), (unsigned)ip.size(), (cx_3d_calib_ext_flags)ext_calib_flags);
	}

	// project image points to target space ip->ip2tp
	std::vector<cx_point3r_t> ip2tp(ip.size());
	calib.sensor2world((const cx_point3r_t*)ip.data(), ip2tp.data(), (unsigned)ip.size());
	
	// visualize results in laser plane with exaggerated residuals and trsh take from accuracy statistics
	calib.getParam(CX_3D_PARAM_A_STAT, val);
	cv::Mat stat; cx::cvUtils::variantGet(val, stat, 4, 3);
	double trsh = stat.at<double>(3, 0) + stat.at<double>(3, 1);	// trsh = mean(d) + std(d) with d=distance

	cx::plotPointDistances("Laser Plane [mm]", tp, ip2tp, trsh, 3.0);
	
	// print info about calibration and save it
	cx::printInfo(calib.getHandle());
	try
	{
		calib.save(result_fname.c_str(), "", CX_3D_CALIB_FORMAT_XML);
	}
	catch (...)
	{
		cerr << "can't save calibration to file: " << result_fname << endl;
	}

	// add target with extracted points to xml
	try 
	{
		target.save(result_fname.c_str(), targetId.c_str());
	}
	catch(...)
	{
		cerr << "can't save target to file: " << result_fname << endl;
	}

	cv::waitKey(0);	// Wait for a keystroke in the OpenCV window
    return 0;
}
