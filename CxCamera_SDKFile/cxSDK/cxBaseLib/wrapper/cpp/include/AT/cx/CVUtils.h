/**
@file : CVUtils.h
@package : cx_base library
@brief Helper functions for data exchange with OpenCV Mat
@copyright (c) 2021, Automation Technology GmbH.
@version 09.12.2021, AT: initial version
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
#ifndef AT_CX_CVUTILS_H_INCLUDED
#define AT_CX_CVUTILS_H_INCLUDED

#include <memory>
#include <assert.h>
#include "AT/cx/base.h"
#include "AT/cx/Image.h"
#include "AT/cx/Variant.h"

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

namespace AT {
	namespace cx {
		namespace cvUtils
		{
			static cx_pixel_format cv2pf(int cvType);
			static int pfstr2cv(const char* pixelFormat);

			static Image imageCreate(const cv::Mat& m, bool deep = false);
			static Image* imageCreatePtr(const cv::Mat& m, bool deep = false);
			static Image::Ptr imageCreateShared(const cv::Mat& m, bool deep = false);
			static cv::Mat imageCopyToMat(const Image& img, bool deepCopy = true);				//!< Creates a cv::Mat object from image. If deepCopy is true a full copy of the image and the underlying data is made.
			static void imageCopyToMat(const Image& img, cv::Mat& mat, bool deepCopy = true);	//!< Creates a cv::Mat object from image. If deepCopy is true a full copy of the image and the underlying data is made.
			static void imageCopyFromMat(const cv::Mat& m, Image& img, bool deepCopy = true);	//!< Copies from cv::Mat image. If deepCopy is true a full copy of the image and the underlying data is made.

			static Variant variantCreate(const cv::Vec2i& p);
			static Variant variantCreate(const cv::Point3d& p);
			static Variant* variantCreatePtr(const cv::Vec2i& p);
			static Variant* variantCreatePtr(const cv::Point3d& p);
			static cx_status_t variantGet(const Variant& var, cv::Vec2i& p);
			static cx_status_t variantSet(Variant& var, const cv::Vec2i& p);
			static cx_status_t variantGet(const Variant& var, cv::Vec4i& p);
			static cx_status_t variantSet(Variant& var, const cv::Vec4i& p);
			static cx_status_t variantGet(const Variant& var, cv::Vec2d& p);
			static cx_status_t variantSet(Variant& var, const cv::Vec2d& p);
			static cx_status_t variantGet(const Variant& var, cv::Vec3f& p);
			static cx_status_t variantSet(Variant& var, const cv::Vec3f& p);
			static cx_status_t variantGet(const Variant& var, cv::Vec3d& p);
			static cx_status_t variantSet(Variant& var, const cv::Vec3d& p);
			static cx_status_t variantGet(const Variant& var, cv::Vec6d& p);
			static cx_status_t variantSet(Variant& var, const cv::Vec6d& p);
			static cx_status_t variantGet(const Variant& var, cv::Point2d& p);
			static cx_status_t variantSet(Variant& var, const cv::Point2d& p);
			static cx_status_t variantGet(const Variant& var, cv::Point3d& p);
			static cx_status_t variantSet(Variant& var, const cv::Point3d& p);
			static cx_status_t variantGet(const Variant& var, cv::Mat& m, int r, int c);
			static cx_status_t variantGet(const Variant& var, cv::Mat& m);
			static cx_status_t variantSet(Variant& var, const cv::Mat& m);
			static cx_status_t variantGet(const Variant& var, std::vector<cv::Point3d>&);
			static cx_status_t variantSet(Variant& var, const std::vector<cv::Point3d>&);

			template<typename _Tp>
			static Point3_<_Tp> point3Create(const cv::Point3_<_Tp>& pt);
			template<typename _Tp>
			static Point3_<_Tp> point3Create(const cv::Vec<_Tp, 3>& v);
			template<typename _Tp>
			static cv::Point3_<_Tp> point3ToCvPoint3(const Point3_<_Tp> pt);
			template<typename _Tp>
			static cv::Vec<_Tp, 3> point3ToCvVec3(const Point3_<_Tp> pt);


			/** get corresponding cx_img_t pixel format id for given opencv mat type
				@param cvType	OpenCV data type, one of CV_8UC1, CV_8UC3, CV_16UC1, CV_16UC3, CV_32SC1, CV_32F, CV_32FC3
				@return			pixel format \ref cx_pixel_format
			*/
			inline cx_pixel_format cv2pf(int cvType)
			{
				cx_pixel_format pf = CX_PF_MONO_8;
				switch (cvType)
				{
				case CV_8UC1:	pf = CX_PF_MONO_8; break;
				case CV_8UC3:	pf = CX_PF_COORD3D_ABC8; break;
				case CV_16UC1:	pf = CX_PF_MONO_16; break;
				case CV_16UC3:	pf = CX_PF_COORD3D_ABC16; break;
				case CV_32SC1:	pf = CX_PF_MONO_32; break;
				case CV_32F:	pf = CX_PF_COORD3D_C32f; break;
				case CV_32FC3:	pf = CX_PF_COORD3D_ABC32f; break;
				default:
					throw(RuntimeError(CX_STATUS_INVALID_PARAMETER));
				}
				return pf;
			}

			/** get corresponding opencv mat type for cx_img_t pixel format id
			@param pixelFormat	pixel format \ref cx_pixel_format
			@return				OpenCV data type, one of CV_8UC1, CV_8UC3, CV_16UC1, CV_16UC3, CV_32SC1, CV_32F, CV_32FC3
			*/
			inline int pf2cv(cx_pixel_format pixelFormat)
			{
				int cvType;
				switch (pixelFormat)
				{
				case CX_PF_MONO_8:
				case CX_PF_COORD3D_C8:				cvType = CV_8UC1; break;

				case CX_PF_COORD3D_ABC8:
				case CX_PF_COORD3D_ABC8_PLANAR:		cvType = CV_8UC3; break;

				case CX_PF_MONO_10:
				case CX_PF_MONO_10p:
				case CX_PF_MONO_12:
				case CX_PF_MONO_12p:
				case CX_PF_MONO_14:
				case CX_PF_MONO_16:
				case CX_PF_COORD3D_C12p:
				case CX_PF_COORD3D_C16:				cvType = CV_16UC1; break;

				case CX_PF_COORD3D_ABC16:
				case CX_PF_COORD3D_ABC16_PLANAR:	cvType = CV_16UC3; break;

				case CX_PF_MONO_32:					cvType = CV_32SC1; break;

				case CX_PF_COORD3D_C32f: 			cvType = CV_32F; break;

				case CX_PF_COORD3D_ABC32f:
				case CX_PF_COORD3D_ABC32f_PLANAR:	cvType = CV_32FC3; 	break;
				default:
					throw(RuntimeError(CX_STATUS_INVALID_PARAMETER));
				}
				return cvType;
			}

			/** get corresponding opencv mat type for cx_img_t pixel format string
			@param pixelFormat	pixel format string \ref cx_pixel_format_strings
			@return				OpenCV data type, one of CV_8UC1, CV_8UC3, CV_16UC1, CV_16UC3, CV_32SC1, CV_32F, CV_32FC3
			*/
			inline int pfstr2cv(const char* pixelFormat)
			{
				int cvType;
				if (strcmp(pixelFormat, CX_PF_MONO_8_STR) == 0)						cvType = CV_8UC1;
				else if (strcmp(pixelFormat, CX_PF_MONO_10_STR) == 0)				cvType = CV_16UC1;
				else if (strcmp(pixelFormat, CX_PF_MONO_10p_STR) == 0)				cvType = CV_16UC1;
				else if (strcmp(pixelFormat, CX_PF_MONO_12_STR) == 0)				cvType = CV_16UC1;
				else if (strcmp(pixelFormat, CX_PF_MONO_12p_STR) == 0)				cvType = CV_16UC1;
				else if (strcmp(pixelFormat, CX_PF_MONO_14_STR) == 0)				cvType = CV_16UC1;
				else if (strcmp(pixelFormat, CX_PF_MONO_16_STR) == 0)				cvType = CV_16UC1;
				else if (strcmp(pixelFormat, CX_PF_MONO_32_STR) == 0)				cvType = CV_32SC1;
				else if (strcmp(pixelFormat, CX_PF_COORD3D_C8_STR) == 0)			cvType = CV_8UC1;
				else if (strcmp(pixelFormat, CX_PF_COORD3D_C12p_STR) == 0)			cvType = CV_16UC1;
				else if (strcmp(pixelFormat, CX_PF_COORD3D_C16_STR) == 0)			cvType = CV_16UC1;
				else if (strcmp(pixelFormat, CX_PF_COORD3D_C32f_STR) == 0)			cvType = CV_32F;
				else if (strcmp(pixelFormat, CX_PF_COORD3D_ABC8_STR) == 0)			cvType = CV_8UC3;
				else if (strcmp(pixelFormat, CX_PF_COORD3D_ABC8_PLANAR_STR) == 0)	cvType = CV_8UC3;
				else if (strcmp(pixelFormat, CX_PF_COORD3D_ABC16_STR) == 0)			cvType = CV_16UC3;
				else if (strcmp(pixelFormat, CX_PF_COORD3D_ABC16_PLANAR_STR) == 0)	cvType = CV_16UC3;
				else if (strcmp(pixelFormat, CX_PF_COORD3D_ABC32f_STR) == 0)		cvType = CV_32FC3;
				else if (strcmp(pixelFormat, CX_PF_COORD3D_ABC32f_PLANAR_STR) == 0)	cvType = CV_32FC3;
				else
				{
					throw(RuntimeError(CX_STATUS_INVALID_PARAMETER));
				}
				return cvType;
			}


			inline Image imageCreate(const cv::Mat& m, bool deep)
			{
				Image img;
				imageCopyFromMat(m, img, deep);

				return img;
			}

			inline Image* imageCreatePtr(const cv::Mat& m, bool deep)
			{
				Image *pImg = new Image();
				imageCopyFromMat(m, *pImg, deep);

				return pImg;
			}

			inline ImagePtr imageCreateShared(const cv::Mat& m, bool deep)
			{
				ImagePtr pImg = std::make_shared<Image>();
				imageCopyFromMat(m, *pImg, deep);

				return pImg;
			}

			inline cv::Mat imageCopyToMat(const Image& img, bool deepCopy)
			{

				if (deepCopy == false)
					return cv::Mat(img.height(), img.width(), pf2cv(img.pixelFormat()), img.data(), img.linePitch());
				else
				{
					cv::Mat mat;
					mat.create(img.height(), img.width(), pf2cv(img.pixelFormat()));
					memcpy(mat.data, img.data(), img.dataSz());
					return mat;
				}
			}

			inline void imageCopyToMat(const Image& img, cv::Mat& mat, bool deepCopy)
			{
				if (deepCopy == false)
					mat = cv::Mat(img.height(), img.width(), pf2cv(img.pixelFormat()), img.data(), img.linePitch());
				else
				{
					mat.create(img.height(), img.width(), pf2cv(img.pixelFormat()));
					memcpy(mat.data, img.data(), img.dataSz());
				}
			}

			inline void imageCopyFromMat(const cv::Mat& m, Image& img, bool deepCopy)
			{
				if (deepCopy == false)
				{
					img.create(m.rows, m.cols, cv2pf(m.type()), (void*)m.ptr<uint8_t>(), m.total()*m.elemSize(), m.step, 0);
				}
				else
				{
					img.create(m.rows, m.cols, cv2pf(m.type()));
					memcpy(img.data(), m.data, img.dataSz());
				}
			}

			inline Variant variantCreate(const cv::Vec2i& p)
			{
				Variant var;
				variantSet(var, p);
				return var;
			}
			inline Variant variantCreate(const cv::Point3d& p)
			{
				Variant var;
				var.set(p.x, p.y, p.z);
				return var;
			}
			inline Variant* variantCreatePtr(const cv::Vec2i& p)
			{
				Variant* var = new Variant();
				variantSet(*var, p);
				return var;
			}
			inline Variant* variantCreatePtr(const cv::Point3d& p)
			{
				Variant* var = new Variant();
				var->set(p.x, p.y, p.z);
				return var;
			}
			inline cx_status_t variantGet(const Variant& var, cv::Vec2i& p)
			{
				return var.get(p(0), p(1));
			}
			inline cx_status_t variantSet(Variant& var, const cv::Vec2i& p)
			{
				return var.set(p(0), p(1));
			}
			inline cx_status_t variantGet(const Variant& var, cv::Vec4i& p)
			{
				if (var.type != CX_VT_INT_ARRAY || var.data.a.len != 4)
					return CX_STATUS_INVALID_PARAMETER;

				int64_t* buf = (int64_t*)var.data.a.buf;
				p(0) = (int)buf[0];
				p(1) = (int)buf[1];
				p(2) = (int)buf[2];
				p(3) = (int)buf[3];
				return CX_STATUS_OK;
			}
			inline cx_status_t variantSet(Variant& var, const cv::Vec4i& p)
			{
				cx_status_t res = var.create(CX_VT_INT_ARRAY, 4);
				if (res == CX_STATUS_OK)
				{
					int64_t* buf = (int64_t*)var.data.a.buf;
					buf[0] = p(0);
					buf[1] = p(1);
					buf[2] = p(2);
					buf[3] = p(3);
				}
				return res;
			}
			inline cx_status_t variantGet(const Variant& var, cv::Vec2d& p)
			{
				return var.get(p(0), p(1));
			}
			inline cx_status_t variantSet(Variant& var, const cv::Vec2d& p)
			{
				return var.set(p(0), p(1));
			}
			inline cx_status_t variantGet(const Variant& var, cv::Vec3f& p)
			{
				return var.get(p(0), p(1), p(2));
			}
			inline cx_status_t variantSet(Variant& var, const cv::Vec3f& p)
			{
				return var.set(p(0), p(1), p(2));
			}
			inline cx_status_t variantGet(const Variant& var, cv::Vec3d& p)
			{
				return var.get(p(0), p(1), p(2));
			}
			inline cx_status_t variantSet(Variant& var, const cv::Vec3d& p)
			{
				return var.set(p(0), p(1), p(2));
			}
			inline cx_status_t variantGet(const Variant& var, cv::Vec6d& p)
			{
				if (var.type != CX_VT_REAL_ARRAY || var.data.a.len != 6)
					return CX_STATUS_INVALID_PARAMETER;

				double* buf = (double*)var.data.a.buf;
				p(0) = (int)buf[0];
				p(1) = (int)buf[1];
				p(2) = (int)buf[2];
				p(3) = (int)buf[3];
				p(4) = (int)buf[4];
				p(5) = (int)buf[5];
				return CX_STATUS_OK;
			}
			inline cx_status_t variantSet(Variant& var, const cv::Vec6d& p)
			{
				cx_status_t res = var.create(CX_VT_REAL_ARRAY, 6);
				if (res == CX_STATUS_OK)
				{
					double* buf = (double*)var.data.a.buf;
					buf[0] = p(0);
					buf[1] = p(1);
					buf[2] = p(2);
					buf[3] = p(3);
					buf[4] = p(4);
					buf[5] = p(5);
				}
				return res;
			}
			inline cx_status_t variantGet(const Variant& var, cv::Point2d& p)
			{
				return var.get(p.x, p.y);
			}
			inline cx_status_t variantSet(Variant& var, const cv::Point2d& p)
			{
				return var.set(p.x, p.y);
			}
			inline cx_status_t variantGet(const Variant& var, cv::Point3d& p)
			{
				return var.get(p.x, p.y, p.z);
			}
			inline cx_status_t variantSet(Variant& var, const cv::Point3d& p)
			{
				return var.set(p.x, p.y, p.z);
			}
			inline cx_status_t variantGet(const Variant& var, cv::Mat& m, int r, int c)
			{
				if (var.data.a.len != r * c)
					return CX_STATUS_INVALID_PARAMETER;

				if (var.type == CX_VT_BYTE_ARRAY)
				{
					m.create(r, c, CV_8U);
					assert(m.isContinuous());
					memcpy(m.ptr<uint8_t>(), var.data.a.buf, var.data.a.len * sizeof(uint8_t));
				}
				else if (var.type == CX_VT_REAL_ARRAY)
				{
					m.create(r, c, CV_64F);
					assert(m.isContinuous());
					memcpy(m.ptr<double>(), var.data.a.buf, var.data.a.len * sizeof(double));
				}
				else
					return CX_STATUS_NOT_IMPLEMENTED;

				return CX_STATUS_OK;
			}
			inline cx_status_t variantGet(const Variant& var, cv::Mat& m)
			{
				return variantGet(var, m, 1, (int)var.data.a.len);
			}
			inline cx_status_t variantSet(Variant& var, const cv::Mat& m)
			{
				cx_status_t res = CX_STATUS_NOT_IMPLEMENTED;

				if (m.isContinuous() == false)
					return CX_STATUS_INVALID_PARAMETER;

				if (m.type() == CV_8U || m.type() == CV_8S)
				{
					res = var.create(CX_VT_BYTE_ARRAY, m.total());
					if (res == CX_STATUS_OK && var.data.a.len > 0)
						memcpy(var.data.a.buf, m.ptr<uint8_t>(), var.data.a.len * sizeof(uint8_t));
				}
				else if (m.type() == CV_64F)
				{
					res = var.create(CX_VT_REAL_ARRAY, m.total());
					if (res == CX_STATUS_OK && var.data.a.len > 0)
						memcpy(var.data.a.buf, m.ptr<double>(), var.data.a.len * sizeof(double));
				}
				else if (m.type() == CV_32F)
				{
					res = var.create(CX_VT_REAL_ARRAY, m.total());
					if (res == CX_STATUS_OK && var.data.a.len > 0)
					{
						const float* srcp = m.ptr<float>();
						double* dstp = (double*)var.data.a.buf;
						for (size_t i = 0; i < var.data.a.len; i++, srcp++, dstp++)
							*dstp = *srcp;
					}
				}
				return res;
			}
			inline cx_status_t variantGet(const Variant& var, std::vector<cv::Point3d>& v)
			{
				if (var.type != CX_VT_REAL_ARRAY)
					return CX_STATUS_INVALID_PARAMETER;
				v.resize(var.data.a.len / 3);
				memcpy(v.data(), var.data.a.buf, var.data.a.len * sizeof(double));
				return CX_STATUS_OK;
			}
			inline cx_status_t variantSet(Variant& var, const std::vector<cv::Point3d>& v)
			{
				cx_status_t res = var.create(CX_VT_REAL_ARRAY, v.size() * 3);
				if (res == CX_STATUS_OK && var.data.a.len > 0)
					memcpy(var.data.a.buf, v.data(), var.data.a.len * sizeof(double));
				return res;
			}

			template<typename _Tp>
			inline Point3_<_Tp> point3Create(const cv::Point3_<_Tp>& pt)
			{
				return Point3_<_Tp>(pt.x, pt.y, pt.z);
			}
			template<typename _Tp>
			inline Point3_<_Tp> point3Create(const cv::Vec<_Tp, 3>& v)
			{
				return Point3_<_Tp>(x(v[0]), y(v[1]), z(v[2]));
			}
			template<typename _Tp>
			inline cv::Point3_<_Tp> point3ToCvPoint3(const Point3_<_Tp> pt)
			{
				return Point3_<_Tp>(pt.x, pt.y, pt.z);
			}
			template<typename _Tp>
			inline cv::Vec<_Tp,3> point3ToCvVec3(const Point3_<_Tp> pt)
			{
				return cv::Vec<_Tp, 3>(pt.x, pt.y, pt.z);
			}

		}
	}
}	// namespace AT::CX

#endif /* AT_CX_CVUTILS_H_INCLUDED */
