/**
@file : PointCloud.h
@package : cx_3d library
@brief C++ PointCloud class representing an organized point cloud.
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
#ifndef CX_C3D_POINTCLOUD_H_INCLUDED
#define CX_C3D_POINTCLOUD_H_INCLUDED

#include "cx_3d_metric.h"
#include "cx_3d_calib.h"
#include "cx_3d_pointcloud.h"
#include "AT/cx/base.h"
#include "AT/cx/c3d/Calib.h"

namespace AT {
	namespace cx {
		namespace c3d {
			//! @addtogroup cx_wrapper_cpp
			//! @{

			/** Point Cloud class represents an organized point cloud based on cx::Image object. 
			*/
			class PointCloud
			{
			public:
				typedef std::shared_ptr<PointCloud> Ptr;

				PointCloud() : points(), normals(), colors(), scale(1.0f, 1.0f, 1.0f), offset(0.0f, 0.0f, 0.0f) {}
				PointCloud(unsigned int h, unsigned int w, cx_pixel_format pf = CX_PF_COORD3D_ABC32f, const cx::Point3f s = cx::Point3f(1.0f, 1.0f, 1.0f), const cx::Point3f o = cx::Point3f(0.0f, 0.0f, 0.0f)) : points(h, w, pf), normals(), colors(), scale(s), offset(o) {}
				~PointCloud() {}

				bool hasPoints() const {
					return points.size() > 0;
				}

				bool hasNormals() const {
					return points.size() > 0 && normals.size() == points.size();
				}

				bool hasColors() const {
					return points.size() > 0 && colors.size() == points.size();
				}

				/** write point cloud, supported formats: ply, xyz, obj
					Supported depths: CV_32F and CV_64F. Supported channels: 3 and 4.
					colors	Used by PLY format only. Supported depth: CV_8U. Supported channels: 1, 3 and 4.
					normals	Used by PLY and OBJ format only. Supported depths: CV_32F and CV_64F. Supported channels: 3 and 4.
					binary	Used only for PLY format.

					\note
					The current implementation does only support saving point clouds of format CX_PF_COORD3D_ABC32f, it does not support saving of scale and offset.
				*/
				void save(const std::string& fileName, bool binary = true)
				{
					checkOk(cx_3d_pointcloud_save(fileName.c_str(), points, hasColors() ? (cx_img_t*)colors : nullptr, hasNormals() ? (cx_img_t*)normals : nullptr, binary));
				}


				/** load point cloud, supported formats: ply, xyz, obj, stl
				*/
				void load(const std::string& fileName)
				{
					checkOk(cx_3d_pointcloud_load(fileName.c_str(), points, colors, normals));
				}
			

				/** Compute normals from points.
					The calculated Normal vectors are normalized.
					Function only works for organized point clouds. 
					
					\note 
					After importing point cloud from eg. ply-files the organization is unordered and the calculation will fail.
				*/
				void computeNormals()
				{
					normals.create(points.height(), points.width(), CX_PF_COORD3D_ABC32f);
					assert(points.pixelFormat() == CX_PF_COORD3D_ABC32f);
					float sdzdx = scale.z / scale.x;
					float sdzdy = scale.z / scale.y;
					for (unsigned y = 1; y < points.height(); y++)
					{
						for (unsigned x = 1; x < points.width(); x++)
						{
#if 0
							cx::Point3f t = points.at<cx::Point3f>(y - 1, x);
							cx::Point3f l = points.at<cx::Point3f>(y, x - 1);
							cx::Point3f c = points.at<cx::Point3f>(y, x);
							cx::Point3f d = (c - t).cross(c - l);
							normals.at<cx::Point3f>(y, x) = normalize(d);
#else			
							cx::Point3f dx = points.at<cx::Point3f>(y, x) - points.at<cx::Point3f>(y, x - 1);
							float dzdx = dx.z / dx.x * sdzdx;
							cx::Point3f dy = points.at<cx::Point3f>(y, x) - points.at<cx::Point3f>(y - 1, x);
							float dzdy = dy.z / dy.y * sdzdy;
							normals.at<cx::Point3f>(y, x) = normalize(cx::Point3f(-dzdx, -dzdy, 1.0f));
#endif
						}
					}
				}

				/** Compute bounding box coordinates.
					@param[out] min3	point with minimum coordinates fo x, y, z.
					@param[out] max3	point with maximum coordinates fo x, y, z.
					@param[in] idv		invalid data value used in z to indicate an invalid data point
				*/
				void computeBoundingBox(cx::Point3f& min3, cx::Point3f& max3, float idv = NAN)
				{
					if (points.pixelFormat() != CX_PF_COORD3D_ABC32f)
						throw std::runtime_error("pixelFormat not supported");

					min3 = cx::Point3f(FLT_MAX, FLT_MAX, FLT_MAX);
					max3 = cx::Point3f(FLT_MIN, FLT_MIN, FLT_MIN);
					for (unsigned y = 0; y < points.height(); y++)
					{
						cx::Point3f* row = points.row<cx::Point3f>(y);
						for (unsigned x = 0; x < points.width(); x++)
						{
							const cx::Point3f p = row[x];
							if (!isnan(p.z) && p.z != idv)
							{
								if (p.x < min3.x)	min3.x = p.x;
								if (p.y < min3.y)	min3.y = p.y;
								if (p.z < min3.z)	min3.z = p.z;
								if (p.x > max3.x)	max3.x = p.x;
								if (p.y > max3.y)	max3.y = p.y;
								if (p.z > max3.z)	max3.z = p.z;
							}
						}
					}
					min3.x = min3.x * scale.x + offset.x;
					min3.y = min3.y * scale.y + offset.y;
					min3.z = min3.z * scale.z + offset.z;
					max3.x = max3.x * scale.x + offset.x;
					max3.y = max3.y * scale.y + offset.y;
					max3.z = max3.z * scale.z + offset.z;
				}
											
				/** Create a shared_ptr PointCloud object with empty size.
					Points, normals and colors are empty.
				*/
				static PointCloud::Ptr createShared()
				{
					return std::make_shared<PointCloud>();
				}
				
				/** Create a shared_ptr PointCloud object with specific size and type.
					Only points are initialized with the specified size. Normals and colors are empty.
				*/
				static PointCloud::Ptr createShared(unsigned int h, unsigned int w, cx_pixel_format pf = CX_PF_COORD3D_ABC32f)
				{
					return std::make_shared<PointCloud>(h, w, pf);
				}

				cx::Image points;		//!< Only valid after calculation or loading from file. Note: must be of type CX_PF_COORD3D_ABC32f (CV_32FC3) in order to use vtk(WCloud) visualization.
				cx::Image normals;		//!< point cloud normals calculated from points or loaded from file.
				cx::Image colors;		//!< point cloud colors, eg. intensity or scatter component of multip-part acquisition. Supported types: CX_PF_MONO_8, CX_PF_MONO_16.
				cx::Point3f scale;	//!< scaling factor for conversion points into real world coordinates, for CX_PF_COORD3D_ABC32f typically (1, 1, 1)
				cx::Point3f offset;	//!< offset for conversion points into real world coordinates, for CX_PF_COORD3D_ABC32f typically (0, 0, 0)
			};

			typedef PointCloud::Ptr PointCloudPtr;


			/** Calculate point cloud from RangeMap
			*/
			inline void calculatePointCloud(cx::c3d::Calib& cal, const cx::Image& rangeMap, PointCloud& pc, int flags=CX_3D_METRIC_MARK_Z_INVALID_DATA)
			{
				cal.setParam(CX_3D_PARAM_METRIC_O, pc.offset);
				cal.setParam(CX_3D_PARAM_METRIC_S, pc.scale);
				cx::checkOk(cx_3d_range2calibratedABC(cal, rangeMap, pc.points, flags));
			}

			/** @overload
			*/
			inline void calculatePointCloud(cx::c3d::Calib& cal, const cx::Image& rangeMap, const uint16_t* xs, const uint16_t* ys, const int32_t* encoderValue, PointCloud& pc, int flags = CX_3D_METRIC_MARK_Z_INVALID_DATA)
			{
				cal.setParam(CX_3D_PARAM_METRIC_O, pc.offset);
				cal.setParam(CX_3D_PARAM_METRIC_S, pc.scale);
				cx::checkOk(cx_3d_rangeWithChunk2calibratedABC(cal, rangeMap, xs, ys, encoderValue, pc.points, flags));
			}

			//! @} cx_wrapper_cpp
		}
	}
}

#endif // CX_3D_POINTCLOUD_H_INCLUDED
