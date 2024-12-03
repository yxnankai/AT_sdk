/**
@file : cx_3d_common.cpp
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

#include <assert.h>
#include <stdio.h>
#include <string>
#include <iostream>

#include "opencv2/opencv.hpp"	// include this first to make sure OpenCVsupport is enabled in cx C++ Wrapper classes
#include "AT/cx/base.h"
#include "AT/CX/PlotCanvas.h"
#include "cx_3d_common.h"
#include "AT/cx/CVUtils.h"

using namespace std;
using namespace cv;
#ifdef HAVE_OPENCV_VIZ
using namespace cv::viz;
#endif

namespace AT {
	namespace cx {
		void loadPointsXZ(const char* fname, std::vector<cx_point3r_t>& v)
		{
			ifstream is(fname);
			while (is)
			{
				cx_point3r_t p;
				p.b = 0.0;			// y = 0
				is >> p.a >> p.c;	// x, z
				if (is)
					v.push_back(p);
			}
		}

		void getMinMax(const std::vector<cx_point3r_t> v, cx_point3r_t& min3, cx_point3r_t& max3)
		{
			if (v.size() > 0)
				min3 = max3 = v[0];
			for (unsigned i = 0; i < v.size(); i++)
			{
				const cx_point3r_t p = v[i];
				if (p.a < min3.a)	min3.a = p.a;
				if (p.b < min3.b)	min3.b = p.b;
				if (p.c < min3.c)	min3.c = p.c;
				if (p.a > max3.a)	max3.a = p.a;
				if (p.b > max3.b)	max3.b = p.b;
				if (p.c > max3.c)	max3.c = p.c;
			}
		}

		void normalizeMinMax8U(const cx::Image& in, cx::Image& out, bool excludeZeros)
		{
			// use OpenCV functions for implementation
			double minR = 0.0;
			double maxR = 0.0;
			cv::Mat in_cv = cx::cvUtils::imageCopyToMat(in, false);
			if (excludeZeros)
			{
				cv::Mat validMask = (in_cv != 0);
				cv::minMaxIdx(in_cv, &minR, &maxR, nullptr, nullptr, validMask);
			}
			else
			{
				cv::minMaxIdx(in_cv, &minR, &maxR);
			}
			double s = (maxR == minR) ? 1.0 : 255.0 / (maxR - minR);
			cv::Mat out_cv;
			in_cv.convertTo(out_cv, CV_8U, s, -minR * s);
			cx::cvUtils::imageCopyFromMat(out_cv, out, true);
		}

		void normalizeMinMax8U(const cv::Mat& in, cv::Mat& out, bool excludeZeros)
		{
			double minR = 0.0;
			double maxR = 0.0;
			if (excludeZeros)
			{
				cv::Mat validMask = (in != 0);
				cv::minMaxIdx(in, &minR, &maxR, nullptr, nullptr, validMask);
			}
			else
			{
				cv::minMaxIdx(in, &minR, &maxR);
			}
			double s = (maxR == minR) ? 1.0 : 255.0 / (maxR - minR);
			in.convertTo(out, CV_8U, s, -minR * s);
		}

#ifdef HAVE_OPENCV_VIZ
		void showPointCloud(cv::viz::Viz3d& viz, const cx::c3d::PointCloud& pc, const std::string& id, int flags, int colormap)
		{
			// create OpenCV object from point cloud, just reference to buffer
			cv::Mat cloud = cx::cvUtils::imageCopyToMat(pc.points, false);
			// cloud.convertTo(cloud, CV_32FC3);
			cv::Mat normals = cx::cvUtils::imageCopyToMat(pc.normals, false);

			cv::Mat colors = cx::cvUtils::imageCopyToMat(pc.colors, false);
			if (colormap > 0)
			{
				cv::Mat colors_map;
				cv::applyColorMap(colors, colors_map, colormap - 1);	// cv colormap definition starts from 0, we use 0 for grayscale
				cv::swap(colors, colors_map);
			}

			viz.setBackgroundMeshLab();
			viz.showWidget(id+"_cs", cv::viz::WCoordinateSystem(10.0));
			if (flags == 2)
				viz.showWidget(id, WCloud(cloud, colors, normals));	// show with colors and normals
			else if (flags == 1)
				viz.showWidget(id, cv::viz::WCloud(cloud, colors));	// show without normals
			else
				viz.showWidget(id, cv::viz::WCloud(cloud));	//  show without colors and normals
			viz.setRenderingProperty(id, cv::viz::SHADING, cv::viz::SHADING_PHONG);
		}
#endif

		void plotPoints(const std::string title, std::vector<cx_point3r_t>& p)
		{
			cx::PlotCanvas figure;
			cx_point3r_t min3, max3;
			cx::getMinMax(p, min3, max3);
			cv::Rect2f bbXZ((float)min3.a, (float)min3.c, (float)(max3.a - min3.a), (float)(max3.c - min3.c));
			float inflate_x = bbXZ.size().width * 0.1f;
			float inflate_y = bbXZ.size().height * 0.1f;
			bbXZ.x -= inflate_x;
			bbXZ.y -= inflate_y;
			bbXZ.width += 2.0f*inflate_x;
			bbXZ.height += 2.0f*inflate_y;
			figure.setArea(bbXZ);
			double totalErr = 0.0;
			for (unsigned i = 0; i < p.size(); i++)
			{
				cv::Point2f t((float)p[i].a, (float)p[i].c);
				figure.plotMarker(t, cv::Scalar(255, 0, 0), cv::MARKER_CROSS);	// blue +
			}
			figure.show(title);
		}

		void plotPointDistances(const std::string title, std::vector<cx_point3r_t>& tp, std::vector<cx_point3r_t>& ipt, double err_trsh, float exag)
		{
			cx::PlotCanvas figure;
			cx_point3r_t min3, max3;
			cx::getMinMax(tp, min3, max3);
			cv::Rect2f bbXZ((float)min3.a, (float)min3.c, (float)(max3.a - min3.a), (float)(max3.c - min3.c));
			float inflate_x = bbXZ.size().width * 0.1f;
			float inflate_y = bbXZ.size().height * 0.1f;
			bbXZ.x -= inflate_x;
			bbXZ.y -= inflate_y;
			bbXZ.width += 2.0f*inflate_x;
			bbXZ.height += 2.0f*inflate_y;
			figure.setArea(bbXZ);
			double totalErr = 0.0;
			for (unsigned i = 0; i < tp.size(); i++)
			{
				cv::Point3d pd = (cv::Point3d&)tp[i] - (cv::Point3d&)ipt[i];
				double err = norm(pd);
				totalErr += err;
				cv::Scalar color = (err > err_trsh) ? cv::Scalar(0, 0, 255) : cv::Scalar(0, 255, 0);	// red = outlier, green = inlier
				cv::Point2f t((float)tp[i].a, (float)tp[i].c);
				cv::Point2f l((float)ipt[i].a, (float)ipt[i].c);
				figure.plotMarker(l, color, cv::MARKER_TILTED_CROSS);	// red = outlier, green = inlier x
				figure.plotMarker(t, cv::Scalar(255, 0, 0), cv::MARKER_CROSS);	// blue +
				figure.plotVector(t, (l - t) * exag, cv::Scalar(0, 0, 255));	// exaggeration of direction vector
			}
			double meanErr = totalErr / double(tp.size());
			cv::Point2f pt(bbXZ.x, bbXZ.br().y - bbXZ.height / 100);
			std::ostringstream ss;
			ss.precision(3);
			ss << "Mean error: " << meanErr << " trsh: " << err_trsh;
			figure.text(pt, ss.str(), cv::Scalar(0, 0, 255));
			figure.show(title);
		}

		void showTargetResults(CX_TARGET_HANDLE hTarget, cv::Size imgSz, double resScale)
		{
			cx::Variant var;
			cx::checkOk(cx_3d_target_get(hTarget, CX_3D_PARAM_TARGET_LABEL_MASK, var));
			cv::Mat labelMask, labelMaskColor;
			cx::cvUtils::variantGet(var, labelMask, imgSz.height, imgSz.width);
			cv::normalize(labelMask, labelMask, 0, 255, cv::NORM_MINMAX);
			cv::applyColorMap(labelMask, labelMaskColor, cv::COLORMAP_JET);
			cv::namedWindow("Label Mask", cv::WINDOW_NORMAL);
			cv::resizeWindow("Label Mask", 600, 600);
			cv::imshow("Label Mask", labelMaskColor);

			cx::checkOk(cx_3d_target_get(hTarget, CX_3D_PARAM_TARGET_RES, var));
			cv::Mat residuen, residuenColor;
			cx::cvUtils::variantGet(var, residuen, imgSz.height, imgSz.width);
			residuen.convertTo(residuen, CV_8U, resScale, 128.0);
			cv::applyColorMap(residuen, residuenColor, COLORMAP_JET);
			cv::namedWindow("Residuen", cv::WINDOW_NORMAL);
			cv::resizeWindow("Residuen", 600, 600);
			cv::imshow("Residuen", residuenColor);
		}
		
		void printInfo(CX_CALIB_HANDLE hCalib, std::ostream& os)
		{
			cx::Variant val;

			os << "-------------------------------------------------------------------------------" << endl;
			cx::checkOk(cx_3d_calib_get(hCalib, CX_3D_PARAM_NAME, val));
			std::string name = val;
			os << "Name:                       " << name << endl;

			cx::checkOk(cx_3d_calib_get(hCalib, CX_3D_PARAM_SENSOR_SN, val));
			std::string sensorSN = val;
			os << "Sensor SN:                  " << sensorSN << endl;

			cx::checkOk(cx_3d_calib_get(hCalib, CX_3D_PARAM_DATE, val));
			std::string date = val;
			os << "Date:                       " << date << endl;

			cx::checkOk(cx_3d_calib_get(hCalib, CX_3D_PARAM_CREATOR, val));
			std::string creator = val;
			os << "Creator:                    " << creator << endl;

			cx::checkOk(cx_3d_calib_get(hCalib, CX_3D_PARAM_DESCRIPTION, val));
			std::string descr = val;
			os << "Description:                " << descr << endl;

			cx::checkOk(cx_3d_calib_get(hCalib, CX_3D_PARAM_MODEL, val));
			int model = val;
			os << "Model:                      " << model << endl;

			cx::checkOk(cx_3d_calib_get(hCalib, CX_3D_PARAM_TARGET, val));
			int target = val;
			os << "Target:                     " << target << endl;

			cx::checkOk(cx_3d_calib_get(hCalib, CX_3D_PARAM_RANGE_SCALE, val));
			double rangeScale = val;
			os << "RangeScale:                 " << rangeScale << endl;

			cx::checkOk(cx_3d_calib_get(hCalib, CX_3D_PARAM_SY, val));
			double profileStepWidth = val;
			os << "ProfileStepWidth Sy:        " << profileStepWidth << endl;

			cx::checkOk(cx_3d_calib_get(hCalib, CX_3D_PARAM_SXY, val));
			double Sxy = val;
			os << "Skew Sxy:                   " << Sxy*(180/CV_PI) << char(248) << endl;

			cx::checkOk(cx_3d_calib_get(hCalib, CX_3D_PARAM_SZY, val));
			double Szy = val;
			os << "Skew Szy:                   " << Szy*(180/CV_PI) << char(248) << endl;

			cx::checkOk(cx_3d_calib_get(hCalib, CX_3D_PARAM_F, val));
			double lensF = val;
			os << "Lens focal length:          " << lensF << endl;

			cx::checkOk(cx_3d_calib_get(hCalib, CX_3D_PARAM_S_SZ, val));
			cv::Size sensorSz;
			val.get(sensorSz.width, sensorSz.height);
			os << "Sensor size:                " << sensorSz << endl;

			cx::checkOk(cx_3d_calib_get(hCalib, CX_3D_PARAM_S_PSZ, val));
			cv::Point2d pixelSz; cx::cvUtils::variantGet(val, pixelSz);
			os << "Pixel size:                 " << pixelSz << endl;

			cx::checkOk(cx_3d_calib_get(hCalib, CX_3D_PARAM_CP, val));
			cv::Point2d cp; cx::cvUtils::variantGet(val,cp);
			os << "Principal Point:            " << cp << endl;

			cx::checkOk(cx_3d_calib_get(hCalib, CX_3D_PARAM_S_R, val));
			cv::Point3d sensorR; cx::cvUtils::variantGet(val, sensorR);
			os << "Rotation of sensor plane:   " << sensorR*(180/CV_PI) << endl;

			cx::checkOk(cx_3d_calib_get(hCalib, CX_3D_PARAM_FOV, val));
			cv::Mat fov;
			cx::cvUtils::variantGet(val, fov);
			os << "FOV (Field of View):        " << fov << endl;

			cx::checkOk(cx_3d_calib_get(hCalib, CX_3D_PARAM_P, val));
			cv::Mat P;
			cx::cvUtils::variantGet(val, P, 3, (int)val.data.a.len/3);
			cv::Mat Temp_Mat;
			P.convertTo(Temp_Mat, CV_32F, 1, 0);
			os << "Homography matrix (image plane to laser plane transformation):" << endl << Temp_Mat.row(0) << endl << Temp_Mat.row(1) << endl << Temp_Mat.row(2) << endl;
			//cv::Mat P;
			//val.get(P, 3, (int)val.data.a.len / 3);
			//os << "Transformation matrix:" << P << endl;

			cx::checkOk(cx_3d_calib_get(hCalib, CX_3D_PARAM_L, val));
			cv::Mat L;
			cx::cvUtils::variantGet(val, L);
			os << "Lens correction matrix:     " << L << endl;

			cx::checkOk(cx_3d_calib_get(hCalib, CX_3D_PARAM_C, val));
			cv::Mat C;
			cx::cvUtils::variantGet(val, C);
			os << "Correction matrix:          " << C << endl;

			cx::checkOk(cx_3d_calib_get(hCalib, CX_3D_PARAM_R, val));
			cv::Point3d R; cx::cvUtils::variantGet(val, R);
			os << "Sensor to world rotation:" << endl;
			os << "\tRx(alpha): " << R.x*(180 / CV_PI) << char(248) << endl;
			os << "\tRy(beta):  " << R.y*(180 / CV_PI) << char(248) << endl;
			os << "\tRz(gamma): " << R.z*(180 / CV_PI) << char(248) << endl;
			  
			cx::checkOk(cx_3d_calib_get(hCalib, CX_3D_PARAM_T, val));
			cv::Point3d T;  cx::cvUtils::variantGet(val, T);
			os << "Sensor to world translation:" << endl;
			os << "\ttx: " << T.x << endl;
			os << "\tty: " << T.y << endl;
			os << "\ttz: " << T.z << endl;

			cx::checkOk(cx_3d_calib_get(hCalib, CX_3D_PARAM_A_STAT, val));
			cv::Mat A_stat;
			cx::cvUtils::variantGet(val, A_stat, 4, 3);

			A_stat.convertTo(Temp_Mat, CV_32F, 1, 0);
			os << "X Accuracy statistic:" << endl;
			os << "\tmean: " << Temp_Mat.row(0).col(0) << endl << "\tstd:  " << Temp_Mat.row(0).col(1) << endl << "\tmax:  " << Temp_Mat.row(0).col(2) << endl;
			os << "Y Accuracy statistic:" << endl;
			os << "\tmean: " << Temp_Mat.row(1).col(0) << endl << "\tstd:  " << Temp_Mat.row(1).col(1) << endl << "\tmax:  " << Temp_Mat.row(1).col(2) << endl;
			os << "Z Accuracy statistic:" << endl;
			os << "\tmean: " << Temp_Mat.row(2).col(0) << endl << "\tstd:  " << Temp_Mat.row(2).col(1) << endl << "\tmax:  " << Temp_Mat.row(2).col(2) << endl;
			os << "Residuals Accuracy statistic:" << endl;
			os << "\tmean: " << Temp_Mat.row(3).col(0) << endl << "\tstd:  " << Temp_Mat.row(3).col(1) << endl << "\tmax:  " << Temp_Mat.row(3).col(2) << endl;

			cx::checkOk(cx_3d_calib_get(hCalib, CX_3D_PARAM_L_STAT, val));
			cv::Mat L_stat;
			cx::cvUtils::variantGet(val, L_stat);
			os << "Lens correction error statistics:" << std::setprecision(5) << L_stat << endl;
			os << "-------------------------------------------------------------------------------" << endl;
		}

		void printTargetInfo(CX_TARGET_HANDLE hTarget, std::ostream& os)
		{
			cx::Variant val;

			os << "-------------------------------------------------------------------------------" << endl;
			cx::checkOk(cx_3d_target_get(hTarget, CX_3D_PARAM_TARGET_NAME, val));
			std::string name = val;
			os << "Name: " << name << endl;

			cx::checkOk(cx_3d_target_get(hTarget, CX_3D_PARAM_TARGET_SN, val));
			std::string sensorSN = val;
			os << "Serial Number: " << sensorSN << endl;

			cx::checkOk(cx_3d_target_get(hTarget, CX_3D_PARAM_TARGET_DESCRIPTION, val));
			std::string descr = val;
			os << "Description: " << descr << endl;

			cx::checkOk(cx_3d_target_get(hTarget, CX_3D_PARAM_TARGET_TYPE, val));
			int target = val;
			os << "Target: " << target << endl;

			cx::checkOk(cx_3d_target_get(hTarget, CX_3D_PARAM_TARGET_STAT, val));
			cv::Mat A_stat;
			cx::cvUtils::variantGet(val, A_stat);
			os << "Statistics:" << A_stat << endl;
			os << "-------------------------------------------------------------------------------" << endl;
		}
	}
}
