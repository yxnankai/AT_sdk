/**
@file : Point3.h
@package : cx_3d library
@brief C++ Point3 class
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
#ifndef CX_C3D_POINT3_H_INCLUDED
#define CX_C3D_POINT3_H_INCLUDED

#include "AT/cx/base.h"
#include <cmath>

namespace AT {
	namespace cx {
		//! @addtogroup cx_wrapper_cpp
		//! @{
				
		/** @brief Template class for 3D points specified by its coordinates `x`, `y` and `z`.

		An instance of the class is interchangeable with the C structure CvPoint2D32f and cv::Point3_X. Similarly to
		Point_ , the coordinates of 3D points can be converted to another type. The vector arithmetic and
		comparison operations are also supported.

		The following Point3_\<\> aliases are available:
		
		\code{.cpp}
			typedef Point3_<int> Point3i;
			typedef Point3_<float> Point3f;
			typedef Point3_<double> Point3d;
		\endcode
		
		\see cx::Point3i, cx::Point3f and cx::Point3d
		*/
		template<typename _Tp> class Point3_
		{
		public:
			typedef _Tp value_type;

			//! default constructor
			Point3_();
			Point3_(_Tp _x, _Tp _y, _Tp _z);
			Point3_(const Point3_& pt);

			Point3_& operator = (const Point3_& pt);

			//! dot product
			_Tp dot(const Point3_& pt) const;
			//! dot product computed in double-precision arithmetics
			double ddot(const Point3_& pt) const;
			//! cross product of the 2 3D points
			Point3_ cross(const Point3_& pt) const;

			_Tp x; //!< x coordinate of the 3D point
			_Tp y; //!< y coordinate of the 3D point
			_Tp z; //!< z coordinate of the 3D point
		};

		typedef Point3_<int> Point3i;
		typedef Point3_<float> Point3f;
		typedef Point3_<double> Point3d;


		//////////////////////////////// 3D Point ///////////////////////////////

		template<typename _Tp> inline
		Point3_<_Tp>::Point3_()
			: x(0), y(0), z(0) {}

		template<typename _Tp> inline
		Point3_<_Tp>::Point3_(_Tp _x, _Tp _y, _Tp _z)
			: x(_x), y(_y), z(_z) {}

		template<typename _Tp> inline
		Point3_<_Tp>::Point3_(const Point3_& pt)
			: x(pt.x), y(pt.y), z(pt.z) {}

		template<typename _Tp> inline
		Point3_<_Tp>& Point3_<_Tp>::operator = (const Point3_& pt)
		{
			x = pt.x; y = pt.y; z = pt.z;
			return *this;
		}

		template<typename _Tp> inline
		_Tp Point3_<_Tp>::dot(const Point3_& pt) const
		{
			return (x*pt.x + y*pt.y + z*pt.z);
		}

		template<typename _Tp> inline
		double Point3_<_Tp>::ddot(const Point3_& pt) const
		{
			return (double)x*pt.x + (double)y*pt.y + (double)z*pt.z;
		}

		template<typename _Tp> inline
		Point3_<_Tp> Point3_<_Tp>::cross(const Point3_<_Tp>& pt) const
		{
			return Point3_<_Tp>(y*pt.z - z*pt.y, z*pt.x - x*pt.z, x*pt.y - y*pt.x);
		}

		template<typename _Tp> static inline
		Point3_<_Tp>& operator += (Point3_<_Tp>& a, const Point3_<_Tp>& b)
		{
			a.x += b.x;
			a.y += b.y;
			a.z += b.z;
			return a;
		}

		template<typename _Tp> static inline
		Point3_<_Tp>& operator -= (Point3_<_Tp>& a, const Point3_<_Tp>& b)
		{
			a.x -= b.x;
			a.y -= b.y;
			a.z -= b.z;
			return a;
		}

		template<typename _Tp> static inline
		Point3_<_Tp>& operator *= (Point3_<_Tp>& a, int b)
		{
			a.x = (a.x * b);
			a.y = (a.y * b);
			a.z = (a.z * b);
			return a;
		}

		template<typename _Tp> static inline
		Point3_<_Tp>& operator *= (Point3_<_Tp>& a, float b)
		{
			a.x = (a.x * b);
			a.y = (a.y * b);
			a.z = (a.z * b);
			return a;
		}

		template<typename _Tp> static inline
		Point3_<_Tp>& operator *= (Point3_<_Tp>& a, double b)
		{
			a.x = (a.x * b);
			a.y = (a.y * b);
			a.z = (a.z * b);
			return a;
		}

		template<typename _Tp> static inline
		Point3_<_Tp>& operator /= (Point3_<_Tp>& a, int b)
		{
			a.x = (a.x / b);
			a.y = (a.y / b);
			a.z = (a.z / b);
			return a;
		}

		template<typename _Tp> static inline
		Point3_<_Tp>& operator /= (Point3_<_Tp>& a, float b)
		{
			a.x = (a.x / b);
			a.y = (a.y / b);
			a.z = (a.z / b);
			return a;
		}

		template<typename _Tp> static inline
		Point3_<_Tp>& operator /= (Point3_<_Tp>& a, double b)
		{
			a.x = (a.x / b);
			a.y = (a.y / b);
			a.z = (a.z / b);
			return a;
		}

		template<typename _Tp> static inline
		double norm(const Point3_<_Tp>& pt)
		{
			return sqrt((double)pt.x*pt.x + (double)pt.y*pt.y + (double)pt.z*pt.z);
		}

		template<typename _Tp> inline
			Point3_<_Tp> normalize(const Point3_<_Tp>& v)
		{
			double nv = norm(v);
			return v * (nv ? 1.0 / nv : 0.0);
		}

		template<typename _Tp> inline
			Point3_<_Tp> min3(const Point3_<_Tp>& p1, const Point3_<_Tp>& p2)
		{
			return Point3_<_Tp>(
				p1.x < p2.x ? p1.x : p2.x,
				p1.y < p2.y ? p1.y : p2.y,
				p1.z < p2.z ? p1.z : p2.z);
		}

		template<typename _Tp> inline
			cx::Point3_<_Tp> max3(const Point3_<_Tp>& p1, const Point3_<_Tp>& p2)
		{
			return Point3_<_Tp>(
				p1.x > p2.x ? p1.x : p2.x,
				p1.y > p2.y ? p1.y : p2.y,
				p1.z > p2.z ? p1.z : p2.z);
		}

		template<typename _Tp> static inline
		bool operator == (const Point3_<_Tp>& a, const Point3_<_Tp>& b)
		{
			return a.x == b.x && a.y == b.y && a.z == b.z;
		}

		template<typename _Tp> static inline
		bool operator != (const Point3_<_Tp>& a, const Point3_<_Tp>& b)
		{
			return a.x != b.x || a.y != b.y || a.z != b.z;
		}

		template<typename _Tp> static inline
		Point3_<_Tp> operator + (const Point3_<_Tp>& a, const Point3_<_Tp>& b)
		{
			return Point3_<_Tp>( (a.x + b.x), (a.y + b.y), (a.z + b.z));
		}

		template<typename _Tp> static inline
		Point3_<_Tp> operator - (const Point3_<_Tp>& a, const Point3_<_Tp>& b)
		{
			return Point3_<_Tp>( (a.x - b.x), (a.y - b.y), (a.z - b.z));
		}

		template<typename _Tp> static inline
		Point3_<_Tp> operator - (const Point3_<_Tp>& a)
		{
			return Point3_<_Tp>( (-a.x), (-a.y), (-a.z) );
		}

		template<typename _Tp> static inline
		Point3_<_Tp> operator * (const Point3_<_Tp>& a, int b)
		{
			return Point3_<_Tp>( (a.x*b), (a.y*b), (a.z*b) );
		}

		template<typename _Tp> static inline
		Point3_<_Tp> operator * (int a, const Point3_<_Tp>& b)
		{
			return Point3_<_Tp>( (b.x * a), (b.y * a), (b.z * a) );
		}

		template<typename _Tp> static inline
		Point3_<_Tp> operator * (const Point3_<_Tp>& a, float b)
		{
			return Point3_<_Tp>( (a.x * b), (a.y * b), (a.z * b) );
		}

		template<typename _Tp> static inline
		Point3_<_Tp> operator * (float a, const Point3_<_Tp>& b)
		{
			return Point3_<_Tp>( (b.x * a), (b.y * a), (b.z * a) );
		}

		template<typename _Tp> static inline
		Point3_<_Tp> operator * (const Point3_<_Tp>& a, double b)
		{
			return Point3_<_Tp>( (_Tp)(a.x * b), (_Tp)(a.y * b), (_Tp)(a.z * b) );
		}

		template<typename _Tp> static inline
		Point3_<_Tp> operator * (double a, const Point3_<_Tp>& b)
		{
			return Point3_<_Tp>((_Tp)(b.x * a), (_Tp)(b.y * a), (_Tp)(b.z * a) );
		}

		template<typename _Tp> static inline
		Point3_<_Tp> operator / (const Point3_<_Tp>& a, int b)
		{
			Point3_<_Tp> tmp(a);
			tmp /= b;
			return tmp;
		}

		template<typename _Tp> static inline
		Point3_<_Tp> operator / (const Point3_<_Tp>& a, float b)
		{
			Point3_<_Tp> tmp(a);
			tmp /= b;
			return tmp;
		}

		template<typename _Tp> static inline
		Point3_<_Tp> operator / (const Point3_<_Tp>& a, double b)
		{
			Point3_<_Tp> tmp(a);
			tmp /= b;
			return tmp;
		}

		//! @} cx_wrapper_cpp
	}
}

#endif // CX_C3D_POINT3_H_INCLUDED
