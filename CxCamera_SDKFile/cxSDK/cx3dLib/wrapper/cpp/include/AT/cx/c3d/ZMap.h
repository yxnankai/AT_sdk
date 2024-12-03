/**
@file : ZMap.h
@package : cx_3d library
@brief C++ class representing a ZMap and its scaling parameters
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
#ifndef CX_C3D_ZMAP_H_INCLUDED
#define CX_C3D_ZMAP_H_INCLUDED

#include "cx_3d_metric.h"
#include "cx_3d_calib.h"
#include "AT/cx/base.h"
#include "AT/cx/c3d/Calib.h"
#include "AT/cx/c3d/PointCloud.h"

namespace AT {
	namespace cx {
		namespace c3d {
			//! @addtogroup cx_wrapper_cpp
			//! @{

			/** ZMap class represents a telecentric projection of 3D Data onto XY plane. 
				It contains the image and the scaling parameters needed to convert a pixel of the ZMap into real coordinates.
				It supports two types of ZMap pixel formats: CX_PF_COORD3D_C32f and CX_PF_COORD3D_C16. The ZMap is based on cx::Image class.
			*/
			class ZMap
			{
			public:
				typedef std::shared_ptr<ZMap> Ptr;

				ZMap() : img(0, 0, CX_PF_COORD3D_C32f), scale(1.0f, 1.0f, 1.0f), offset(0.0f, 0.0f, 0.0f) {}
				ZMap(unsigned int h, unsigned int w, cx_pixel_format pf = CX_PF_COORD3D_C32f, const cx::Point3f s = cx::Point3f(1.0f, 1.0f, 1.0f), const cx::Point3f o = cx::Point3f(0.0f, 0.0f, 0.0f)) : img(h, w, pf), scale(s), offset(o) {}

				void create(unsigned int h, unsigned int w, cx_pixel_format pf = CX_PF_COORD3D_C32f, const cx::Point3f s = cx::Point3f(1.0f, 1.0f, 1.0f), const cx::Point3f o = cx::Point3f(0.0f, 0.0f, 0.0f))
				{
					img.create(h, w, pf);
					scale = s;
					offset = o;
				}

				/** Save ZMap image
					
					\note
					The current implementation does not support saving of scale and offset.
				*/
				void save(const std::string& fileName) const
				{
					img.save(fileName);
				}

				/** Load ZMap image
					
					\note
					The current implementation does not support loading of scale and offset.
				*/
				void load(const std::string& fileName)
				{
					img.load(fileName);
				}

				static ZMap::Ptr makeShared(unsigned int h, unsigned int w, cx_pixel_format pf = CX_PF_COORD3D_C32f, const cx::Point3f s = cx::Point3f(1.0f, 1.0f, 1.0f), const cx::Point3f o = cx::Point3f(0.0f, 0.0f, 0.0f))
				{
					return std::make_shared<ZMap>(h, w, pf, s, o);
				}

				cx::Image img;			//!< ZMap image, we support two types of ZMaps, CX_PF_COORD3D_C32f and CX_PF_COORD3D_C16.
				cx::Point3f scale;		//!< scaling factor for conversion into real world coordinates
				cx::Point3f offset;		//!< offset for conversion into real world coordinates
			};

			typedef ZMap::Ptr ZMapPtr;

			/** Calculate Z-Map (rectified image in XY plane) from RangeMap image
				@param cal		the calibration to use for the conversion of sensor range image to ZMap.
				@param rangeMap	the sensor range image
				@param zMap		the ZMap to calculate. The image must be initialized with the desired resolution and pixelformat. The scale and offset must be set according to the desired sampling, see also \ref cx_3d_range2rectifiedC.
				@param flags	use CX_3D_METRIC_NEAREST_POINT | CX_3D_METRIC_FILL_HOLES or CX_3D_METRIC_INTERP_IDW, optional you can also add CX_3D_METRIC_MARK_Z_INVALID_DATA
			*/
			inline void calculateZMap(cx::c3d::Calib& cal, const cx::Image& rangeMap, cx::c3d::ZMap& zMap, int flags = CX_3D_METRIC_MARK_Z_INVALID_DATA | CX_3D_METRIC_INTERP_IDW)
			{
				cal.setParam(CX_3D_PARAM_METRIC_O, zMap.offset);
				cal.setParam(CX_3D_PARAM_METRIC_S, zMap.scale);
				cx::checkOk(cx_3d_range2rectifiedC(cal, rangeMap, zMap.img, flags));
			}

			/** @overload
			*/
			inline void calculateZMap(cx::c3d::Calib& cal, const cx::Image& rangeMap, const uint16_t* xs, const uint16_t* ys, const int32_t* encoderValue, cx::c3d::ZMap& zMap, int flags = CX_3D_METRIC_MARK_Z_INVALID_DATA | CX_3D_METRIC_INTERP_IDW)
			{
				cal.setParam(CX_3D_PARAM_METRIC_O, zMap.offset);
				cal.setParam(CX_3D_PARAM_METRIC_S, zMap.scale);
				cx::checkOk(cx_3d_rangeWithChunk2rectifiedC(cal, rangeMap, xs, ys, encoderValue, zMap.img, flags));
			}

			/** Calculate PointCloud points from ZMap.
				Function does not alter pc.normals nor pc.colors.
				PointCloud will be created with type CX_PF_COORD3D_ABC32f.
			*/
			inline void convertToPointCloud(const cx::c3d::ZMap& zmap, cx::c3d::PointCloud& pc, float ivd = 0.0f)
			{	
				pc.points.create(zmap.img.height(), zmap.img.width(), CX_PF_COORD3D_ABC32f);
				if (zmap.img.pixelFormat() == CX_PF_COORD3D_C32f)
				{
					for (unsigned r = 0; r < zmap.img.height(); r++)
					{
						const float* zMapRowPtr = zmap.img.row<float>(r);
						cx::Point3f* pcRowPtr = pc.points.row<cx::Point3f>(r);
						for (unsigned c = 0; c < zmap.img.width(); c++)
						{
							float z = zMapRowPtr[c];
							cx::Point3f p;
							p.x = float(c) * zmap.scale.x + zmap.offset.x;
							p.y = float(r) * zmap.scale.y + zmap.offset.y;
							p.z = (z == ivd) ? z : z * zmap.scale.z + zmap.offset.z;
							pcRowPtr[c] = p;
						}
					}
				}
				else if (zmap.img.pixelFormat() == CX_PF_COORD3D_C16)
				{
					for (unsigned r = 0; r < zmap.img.height(); r++)
					{
						const uint16_t* zMapRowPtr = zmap.img.row<uint16_t>(r);
						cx::Point3f* pcRowPtr = pc.points.row<cx::Point3f>(r);
						for (unsigned c = 0; c < zmap.img.width(); c++)
						{
							float z = zMapRowPtr[c];
							cx::Point3f p;
							p.x = float(c) * zmap.scale.x + zmap.offset.x;
							p.y = float(r) * zmap.scale.y + zmap.offset.y;
							p.z = (z == ivd) ? z : z * zmap.scale.z + zmap.offset.z;
							pcRowPtr[c] = p;
						}
					}
				}
				else
					throw std::runtime_error("unsupported pixel format");
			}

			//! @} cx_wrapper_cpp
		}
	}
}

#endif // CX_C3D_ZMAP_H_INCLUDED