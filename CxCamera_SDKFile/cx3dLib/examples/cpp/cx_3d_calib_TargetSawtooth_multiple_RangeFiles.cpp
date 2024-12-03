/**
@package : cx_3d library
@file : cx_3d_calib_TargetSawtooth_multiple_RangeFile.cpp
@brief CPP example of using AT 3D library for calibration from a set of rangemaps of sawtooth calibration target.

This example shows how to do an intrinsic calibration based on the sawtooth calibration target.
The following steps are demonstrated:
-# Load Target description from xml
-# Load range map per height step and do Target point extraction
-# Load calibration default values (start values) from file
-# Set calibration parameters
-# Do instrinsic calibration
-# Visualize results
-# Cleanup.

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
	cx::Variant val;
	std::string basePath	= "../../../cx3dLib/data/";
	std::string targetFname	= basePath + "xml/targets.xml";				// target calibration description  
	std::string imgPath		= basePath + "img/";						// directory for calibration images
	std::string resultFname	= basePath + "temp/calib_from_sawtooth";	// calibration result file

	// example with multiple range files, one file per height step
	std::string files[] = {	"C5-4090CS39-145_zickzack_5_2_001.tif", "C5-4090CS39-145_zickzack_5_2_002.tif", "C5-4090CS39-145_zickzack_5_2_003.tif", "C5-4090CS39-145_zickzack_5_2_004.tif", "C5-4090CS39-145_zickzack_5_2_005.tif", "C5-4090CS39-145_zickzack_5_2_006.tif", "C5-4090CS39-145_zickzack_5_2_007.tif", "C5-4090CS39-145_zickzack_5_2_008.tif", "C5-4090CS39-145_zickzack_5_2_009.tif", "C5-4090CS39-145_zickzack_5_2_010.tif", "C5-4090CS39-145_zickzack_5_2_011.tif" };
	double targetOffsetZ[] = {-7.5000000e+00, -6.0000000e+00, -4.5000000e+00, -3.0000000e+00, -1.5000000e+00, 0.0000000e+00, 1.5000000e+00, 3.0000000e+00, 4.5000000e+00, 6.0000000e+00, 7.5000000e+00};
	std::string calib_fname = basePath + "img/C5-4090CS39-145.xml";		// initial calibration file with start values
	
	int numHeightSteps = sizeof(targetOffsetZ) / sizeof(targetOffsetZ[0]);

	// load target from xml-file
	cx::c3d::Target target;
	target.load(targetFname.c_str(), "st5x2");

	// extract points from each file and add to targets points list
	cv::Size resSz;

	for (int i = 0; i < numHeightSteps; i++)
	{
		std::string img_fname = imgPath + files[i];
		cv::Mat img = cv::imread(img_fname, cv::IMREAD_ANYDEPTH);
		cx::Image rangeImg = cx::cvUtils::imageCreate(img);	// rangeImg is just a reference to img, img must stay alive as long as rangeImg is used!
		resSz = cv::Size(img.cols, numHeightSteps);
		target.setParam(CX_3D_PARAM_TARGET_GP_T, cx::Variant(0.0, 0.0, targetOffsetZ[i]));
		target.findPoints(rangeImg, CX_3D_TARGET_ADD_POINTS);
	}

	// show target info and display residuen
	cx::printTargetInfo(target.getHandle());
	cx::showTargetResults(target.getHandle(), resSz, 10.0);

	cx::c3d::Calib calib;
	// load "start values" from calib file and set some more parameters manually
	calib.load(calib_fname.c_str(), "");
	// set model and start parameters -> otherwise use default parameters from XML 
	calib.setParam(CX_3D_PARAM_MODEL, cx::Variant(CX_3D_CALIB_MODEL_P_HOM | CX_3D_CALIB_MODEL_C_POLY));

	// range scaling of ip points -> optional: default value from XML
	//cx::checkOk(cx_3d_calib_set(hCalib, CX_3D_PARAM_RANGE_SCALE, cx::Variant(1.0/64.0)));
	cout << "Start values:" << endl;
	cx::printInfo(calib.getHandle());

	// estimate intrinsic model, this does not include any extrinsic parameters, only mapping from sensor-plane to laser-plane
	// get points from target
	std::vector<cx_point3r_t> ip, tp;
	target.getParam(CX_3D_PARAM_TARGET_TP, val);
	val.get(tp);
	target.getParam(CX_3D_PARAM_TARGET_IP, val);
	val.get(ip);

	cx::plotPoints("IP [pix]", ip);
	cx::plotPoints("TP [mm]", tp);

	assert(tp.size() == ip.size());
	calib.calibrateIntrinsicFromPoints(ip.data(), tp.data(), (unsigned)ip.size(), (cx_3d_calib_int_flags)0); // CX_3D_CALIB_SOLVE_L_K2, CX_3D_CALIB_SOLVE_C_P3, CX_3D_CALIB_FIX_CP | CX_3D_CALIB_FIX_L_K1

	// project image points to target space ip->ip2tp
	std::vector<cx_point3r_t> ip2tp(ip.size());
	calib.sensor2world(ip.data(), ip2tp.data(), (unsigned)ip.size());

	// visualize results in laser plane with exaggerated residuals and trsh take from accuracy statistics
	calib.getParam(CX_3D_PARAM_A_STAT, val);
	cv::Mat stat; cx::cvUtils::variantGet(val, stat, 4, 3);
	double trsh = stat.at<double>(3, 0) + stat.at<double>(3, 1);	// trsh = mean(d) + std(d) with d=distance
	cx::plotPointDistances("Laser Plane [mm]", tp, ip2tp, trsh, 5.0);

	// print info about calibration and save it
	cx::printInfo(calib.getHandle());

	// store as xml and binary
	std::string resultFnameXml = resultFname + ".xml";		// add file extension corresponding to calibFormat
	calib.save(resultFnameXml.c_str(), "", CX_3D_CALIB_FORMAT_XML);
	target.save(resultFnameXml.c_str(), "st60x20_calib");	// add target with extracted points to xml

	calib.save((resultFname + ".dat").c_str(), "", CX_3D_CALIB_FORMAT_BINARY);
	cout << "stored calibration to " << resultFname << ".dat/.xml" << endl;

	cv::waitKey(0);	// Wait for a keystroke in the OpenCV window
	return 0;
}
