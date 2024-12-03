/**
@file : cx_3d_common.h
@package : cx_3d library
@brief CPP common example functionality.
@copyright (c) 2017, Automation Technology GmbH.
@version 04.09.2017, AT: initial version
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
#pragma once
#ifndef CX_3D_COMMON_H_INCLUDED
#define CX_3D_COMMON_H_INCLUDED

#define _USE_MATH_DEFINES
#include <math.h> 

#include "opencv2/opencv.hpp"
#include "cx_3d_metric.h"
#include "cx_3d_calib.h"
#include "AT/cx/base.h"
#include "AT/cx/c3d/Calib.h"
#include "AT/cx/c3d/PointCloud.h"
#include "AT/cx/c3d/ZMap.h"

namespace AT {
	namespace cx {

		//! @addtogroup cx_wrapper_cpp
		//! @{

		/** Load XZ-point file into Point3d vector
			The text file format is x followed by z. The y coordinate is set to zero.
		*/
		void loadPointsXZ(const char* fname, std::vector<cx_point3r_t>& v);

		/** compute bounding coordinates for a given vector of XYZ points.
			@param v			vector of points
			@param[out] min3	point with minimum coordinates fo x, y, z.
			@param[out] max3	point with maximum coordinates fo x, y, z.
		*/
		void getMinMax(const std::vector<cx_point3r_t> v, cx_point3r_t& min3, cx_point3r_t& max3);

		/** Queries all parameters of calibration object and dumps it to the given stream.
			@param hCalib	handle to calibration.
			@param os		output stream
		*/
		void printInfo(CX_CALIB_HANDLE hCalib, std::ostream& os=std::cout);

		void printTargetInfo(CX_TARGET_HANDLE hTarget, std::ostream& os = std::cout);
		
		void normalizeMinMax8U(const cx::Image& in, cx::Image& out, bool excludeZeros = true);

#ifdef HAVE_OPENCV_VIZ
		/** Show Point Cloud optional with colors and normals
			@param viz			viewer object
			@param pc			input point cloud image
			@param id			id of cloud-widget, \note you must use different ids if you want to show multiple point clouds.
			@param flags		0=only show points without normals and static color, 1=use colors, 2=use colors and normals
			@param colormap		If the color image is valid an additional colormap can be applied. 0=grayscale, see cv::COLORMAP_XXX for non grayscale colormaps, pass cv::COLORMAP_XXX+1 as parameter.
		*/
		void showPointCloud(cv::viz::Viz3d& viz, const cx::c3d::PointCloud& pc, const std::string& id = "pc", int flags=0, int colormap=0);
#endif

		/** Plot point list.
		*/
		void plotPoints(const std::string title, std::vector<cx_point3r_t>& p);

		/** Plot two point lists and visualize the distance between, distance can be optionally exaggerated.
		*/
		void plotPointDistances(const std::string title, std::vector<cx_point3r_t>& tp, std::vector<cx_point3r_t>& ipt, double err_trsh, float exag = 10.0);
		
		/** Show the results of target point extraction
		label mask = shows the result of the facets segmentation, the invalid data points and the detected outliers
		residuen image = deviation of points from fitted planes
		*/
		void showTargetResults(CX_TARGET_HANDLE hTarget, cv::Size imgSz, double resScale=1.0);

		/** Load calibration from camera using a temporary file on host side.
			We first try to load the new XML calibration format from "Calibration".
			If this Filetype is not available we try to load the old binary format from "UserData"
		*/
		cx_status_t downloadCalib(CX_DEVICE_HANDLE hDevice, CX_CALIB_HANDLE& hCalib, const char* tempFileName);

		/** Load calibration from camera using internal buffer.
			We first try to load the new XML calibration format from "Calibration".
			If this Filetype is not available we try to load the old binary format from "UserData"
		*/
		cx_status_t downloadCalib(CX_DEVICE_HANDLE hDevice, CX_CALIB_HANDLE& hCalib);

		/** Update calibration settings with current acquisition settings from camera
			The following calibration parameters need to be updated with device settings: CX_3D_PARAM_O_X, CX_3D_PARAM_RANGE_SCALE, CX_3D_PARAM_O_Y.

			@param hDevice	handle of the camera device
			@param hCalib	handle of the calibration
			@param aoiSel	takes the device settings of aoiSel. Number between 1 and number of aois. 
		*/
		void updateCalibC5(CX_DEVICE_HANDLE hDevice, CX_CALIB_HANDLE hCalib, int aoiSel=1);

		/** Update calibration settings with current acquisition settings from camera
			The following calibration parameters need to be updated with device settings: CX_3D_PARAM_O_X, CX_3D_PARAM_RANGE_SCALE, CX_3D_PARAM_O_Y.

			@param hDevice	handle of the camera device
			@param hCalib	handle of the calibration
			@param aoiSel	takes the device settings of aoiSel. Number between 1 and number of aois.
		*/
		void updateCalibC6(CX_DEVICE_HANDLE hDevice, CX_CALIB_HANDLE hCalib, int regionId = 0);

		//! @} cx_wrapper_cpp
	}
}

#endif // CX_3D_COMMON_H_INCLUDED
