/*************************************************************************************
@brief collection of math functions
@file : math.h
@package : cx_3d library

@copyright (c) 2017, Automation Technology GmbH.

@version 04.09.2017, AT: initial version

*************************************************************************************/
#pragma once
#ifndef CX_MATH_INCLUDED
#define CX_MATH_INCLUDED

#define _USE_MATH_DEFINES
#include <math.h> 
#include "opencv2/core/core.hpp"
#include "cx_base_lib.h"


namespace AT {
	namespace cx {

		/** calculate rotation matrix around 1. X, 2. Y, 3. Z
			Euler angles of the rotation around the three axes.
		*/
		inline void eulerMatrix(const cv::Vec3d& r, cv::Matx33d& Rm)
		{
			double sinRx = sin(r(0));
			double sinRy = sin(r(1));
			double sinRz = sin(r(2));
			double cosRx = cos(r(0));
			double cosRy = cos(r(1));
			double cosRz = cos(r(2));

			// set the individual rotation matrices and multiply them in reverse order
			cv::Matx<double, 3, 3> Rx(
				1.0,   0.0,    0.0,
				0.0, cosRx, -sinRx,
				0.0, sinRx, cosRx);

			cv::Matx<double, 3, 3> Ry(
				cosRy,  0.0, sinRy,
				  0.0,  1.0,   0.0,
				-sinRy, 0.0, cosRy);

			cv::Matx<double, 3, 3> Rz(
				cosRz, -sinRz, 0.0,
				sinRz,  cosRz, 0.0,
				  0.0,    0.0, 1.0);

			Rm = Rz*Ry*Rx;
		}

		inline void eulerMatrix(const cv::Vec3f& r, cv::Matx33f& Rm)
		{
			float sinRx = sin(r(0));
			float sinRy = sin(r(1));
			float sinRz = sin(r(2));
			float cosRx = cos(r(0));
			float cosRy = cos(r(1));
			float cosRz = cos(r(2));

			// set the individual rotation matrices and multiply them in reverse order
			cv::Matx<float, 3, 3> Rx(
				1.0,   0.0,    0.0,
				0.0, cosRx, -sinRx,
				0.0, sinRx, cosRx);

			cv::Matx<float, 3, 3> Ry(
				cosRy,  0.0, sinRy,
				  0.0,  1.0, 0.0,
				-sinRy, 0.0, cosRy);

			cv::Matx<float, 3, 3> Rz(
				cosRz, -sinRz, 0.0,
				sinRz,  cosRz, 0.0,
				  0.0,    0.0, 1.0);

			Rm = Rz*Ry*Rx;
		}

		inline cv::Point2d mult(const cv::Matx<double, 3, 3>& mx, const cv::Point2d& b)
		{
			const double eps = DBL_EPSILON;
			const double* m = mx.val;
			cv::Point2d p(0.0, 0.0);
			double w = b.x*m[6] + b.y*m[7] + m[8];
			if (fabs(w) > eps)
			{
				w = 1.0 / w;
				p.x = (b.x*m[0] + b.y*m[1] + m[2])*w;
				p.y = (b.x*m[3] + b.y*m[4] + m[5])*w;
			}
			return p;
		}

		inline cv::Point2d mult(const cv::Mat& mx, const cv::Point2d& b)
		{
			assert(mx.total() == 9 && mx.type() == CV_64F);
			const double eps = FLT_EPSILON;
			const double* m = mx.ptr<double>();
			cv::Point2d p(0.0f, 0.0f);
			double w = b.x*m[6] + b.y*m[7] + m[8];
			if (fabs(w) > eps)
			{
				w = 1.0 / w;
				p.x = (b.x*m[0] + b.y*m[1] + m[2])*w;
				p.y = (b.x*m[3] + b.y*m[4] + m[5])*w;
			}
			return p;
		}

		inline double meanNorm(const cv::Mat& p1, const cv::Mat& p2)
		{
			assert(p1.type() == CV_64FC2 && p2.type() == CV_64FC2);
			assert(p1.checkVector(2) == p2.checkVector(2));
			int npoints = p1.checkVector(2);

			const cv::Point2d* p1p = p1.ptr<cv::Point2d>();
			const cv::Point2d* p2p = p2.ptr<cv::Point2d>();
			double sum = 0.0;
			for (int i = 0; i < npoints; i++)
			{
				cv::Point2d d = p1p[i] - p2p[i];
				sum += norm(d);
			}
			return sum / double(npoints);
		}


		inline double meanNorm(const cx_point3r_t* p1, const cx_point3r_t* p2, unsigned numPoints)
		{
			double sum = 0.0;
			for (unsigned i = 0; i < numPoints; i++)
			{
				cv::Point3d d = (cv::Point3d&)p1[i] - (cv::Point3d&)p2[i];
				sum += norm(d);
			}
			return sum / double(numPoints);
		}

		inline double sumNorm(const cx_point3r_t* p1, const cx_point3r_t* p2, unsigned numPoints)
		{
			double sum = 0.0;
			for (unsigned i = 0; i < numPoints; i++)
			{
				cv::Point3d d = (cv::Point3d&)p1[i] - (cv::Point3d&)p2[i];
				sum += norm(d);
			}
			return sum;
		}

	}
}

#endif // CX_MATH_INCLUDED
