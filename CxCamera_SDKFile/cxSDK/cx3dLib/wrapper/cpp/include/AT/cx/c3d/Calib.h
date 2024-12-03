/**
@file : Calib.h
@package : cx_3d library
@brief C++ Calib wrapper class
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
#ifndef CX_C3D_CALIB_H_INCLUDED
#define CX_C3D_CALIB_H_INCLUDED

#include "cx_3d_metric.h"
#include "cx_3d_calib.h"
#include "cx_3d_calib_int.h"
#include "AT/cx/base.h"
#include "AT/cx/Point3.h"
#include "AT/cx/c3d/Target.h"

namespace AT {
	namespace cx {
		namespace c3d {
			//! @addtogroup cx_wrapper_cpp
			//! @{

			/** Calib class is wrapping the CX_CALIB_HANDLE for easy usage of the cx_3dLib calibration functions in C++.
			*/
			class Calib
			{
			public:
				typedef std::shared_ptr<Calib> Ptr;
			
				Calib()
				{
					m_hCalib = CX_INVALID_HANDLE;
					cx_3d_calib_new(&m_hCalib);
				}
				~Calib()
				{
					cx_3d_calib_release(m_hCalib);
					m_hCalib = CX_INVALID_HANDLE;
				}

				bool isValid() const
				{
					return (m_hCalib != CX_INVALID_HANDLE);
				}

				/** Set parameter
					\sa cx_3d_calib_set
				*/
				void setParam(cx_3d_calib_param_t param, const cx::Variant& val)
				{
					cx::checkOk("cx_3d_calib_set", cx_3d_calib_set(m_hCalib, param, &val));
				}

				/** Return parameter value by reference, optimized version without any additional return copies.
					\sa cx_3d_calib_get
				*/
				void getParam(cx_3d_calib_param_t param, cx::Variant& val) const
				{
					cx::checkOk("cx_3d_calib_get", cx_3d_calib_get(m_hCalib, param, &val));
				}

				/** Load calibration from file.
					throws cx::RuntimeError if it fails
					\sa cx_3d_calib_load
				*/
				void load(const std::string& fileName, const std::string& id = std::string("factory"))
				{
					cx_3d_calib_release(m_hCalib);
					m_hCalib = CX_INVALID_HANDLE;
					cx::checkOk("cx_3d_calib_load", cx_3d_calib_load(fileName.c_str(), id.c_str(), CX_3D_CALIB_FORMAT_AUTO, &m_hCalib));
				}
	
				/** Load calibration from buffer.
					throws cx::RuntimeError if it fails
					\sa cx_3d_calib_loadFromBuffer
				*/
				void loadFromBuffer(const std::string& buffer, const std::string& id = std::string("factory"))
				{
					cx_3d_calib_release(m_hCalib);
					m_hCalib = CX_INVALID_HANDLE;
					cx::checkOk("cx_3d_calib_loadFromBuffer", cx_3d_calib_loadFromBuffer((const uint8_t*)buffer.data(), buffer.length(), id.c_str(), CX_3D_CALIB_FORMAT_AUTO, &m_hCalib));
				}

				/** Save calibration to file
					throws cx::RuntimeError if it fails
					\sa cx_3d_calib_save
				*/
				void save(const std::string& fileName, const std::string& id = std::string("cal0"), int formatFlags = CX_3D_CALIB_FORMAT_XML)
				{
					cx::checkOk("cx_3d_calib_save", cx_3d_calib_save(m_hCalib, fileName.c_str(), id.c_str(), formatFlags));
				}

				/** Save calibration to buffer
					throws cx::RuntimeError if it fails
					\sa cx_3d_calib_saveToBuffer
				*/
				std::string saveToBuffer(const std::string& id = std::string("cal0"), int formatFlags = CX_3D_CALIB_FORMAT_XML)
				{
					size_t bufferSz = 262144;		// 256kByte buffer size
					std::string buffer(bufferSz, 0);
					cx::checkOk("cx_3d_calib_saveToBuffer", cx_3d_calib_saveToBuffer(m_hCalib, (uint8_t*)buffer.data(), &bufferSz, id.c_str(), formatFlags));
					buffer.resize(bufferSz);
					return buffer;
				}

				/** Convert points from sensor to world coordinates
					See also \ref cx_3d_sensor2world
					@param[in] pi  Input points in sensor coordinates. x=sensor-x-pixel coordinate, y=moving direction coordinate, z=sensor-y-pixelcoordinate.
					@param[out] po  Output points in world coordinates. The given buffer must provide enough space for numPoints points.
					@param numPoints  Number of points to convert
				*/
				void sensor2world(const cx_point3r_t* pi, cx_point3r_t* po, unsigned numPoints=1)
				{
					cx::checkOk("cx_3d_sensor2world", cx_3d_sensor2world(m_hCalib, pi, po, numPoints));
				}
				/** @overload
					@param pi Point to convert to world coordinates
				*/
				cx::Point3d sensor2world(const cx::Point3d& pi)
				{
					cx::Point3d po;
					sensor2world((const cx_point3r_t*)&pi, (cx_point3r_t*)&po);
					return po;
				}
				/** @overload
					@param pi 	Array pf points to convert to world coordinates
				*/
				std::vector<cx::Point3d> sensor2world(const std::vector<cx::Point3d>& pi)
				{
					std::vector<cx::Point3d> po(pi.size());
					sensor2world((const cx_point3r_t*)pi.data(), (cx_point3r_t*)po.data(), (unsigned)pi.size());
					return po;
				}
				
				/** Convert points from sensor to world coordinates
					See also \ref cx_3d_world2sensor
					@param[in] pi  Input points in world coordinates.
					@param[out] po  Output points in points in sensor coordinates. x=sensor-x-pixel coordinate, y=moving direction coordinate, z=sensor-y-pixelcoordinate. The given buffer must provide enough space for numPoints points.
					@param numPoints  Number of points to convert
					
					\note
					not all parts of the calibration model are invertable. Depending on the calibration there might be some numerical errors in the conversion.
				*/
				void world2sensor(const cx_point3r_t* pi, cx_point3r_t* po, unsigned numPoints=1)
				{
					cx::checkOk("cx_3d_world2sensor", cx_3d_world2sensor(m_hCalib, pi, po, numPoints));
				}
				/** @overload
					@param pi Point to convert to sensor coordinates
				*/
				cx::Point3d world2sensor(const cx::Point3d& pi)
				{
					cx::Point3d po;
					world2sensor((const cx_point3r_t*)&pi, (cx_point3r_t*)&po);
					return po;
				}
				/** @overload
					@param pi 	Array pf points to convert to sensor coordinates
				*/
				std::vector<cx::Point3d> world2sensor(const std::vector<cx::Point3d>& pi)
				{
					std::vector<cx::Point3d> po(pi.size());
					world2sensor((const cx_point3r_t*)pi.data(), (cx_point3r_t*)po.data(), (unsigned)pi.size());
					return po;
				}

				/** Calculate transformation (3D-Rotation and 3D-Translation) from Sensor-CS to World-CS from given target data set. It will set the calibrations parameter CX_3D_PARAM_R and CX_3D_PARAM_T.
					The current calibration given by hCalib is used to convert from image coordinates to sensor coordinates before estimating pose and location.
					@param target	reference to calibration target
					@param calib_flags	flags that modify the default behaviour of the calibration solver, see \ref cx_3d_calib_ext_flags. Use CX_3D_CALIB_TARGET_DEFAULT for default solving algorithm which depends on the actual target.
				 */
				void calibrateExtrinsic(cx::c3d::Target& target, int calib_flags)
				{
					cx::checkOk(cx_3d_calibrateExtrinsic(getHandle(), target.getHandle(), calib_flags));
				}

				/** Calculate transformation (3D-Rotation and 3D-Translation) from Sensor-CS to World-CS from given point pairs. It will set the calibrations parameter CX_3D_PARAM_R and CX_3D_PARAM_T.
					If more than two pairs are give a least square estimation is used.
					The given points pairs are relative image coordinates vs. world coordinates.
					The current calibration given by hCalib is used to convert from image coordinates to sensor coordinates before estimating pose and location.
					@param ip		array of points in sensor plane, y-component is moving direction
					@param tp		array of points in laser (target) plane, y-component is moving direction
					@param numPoints number of points in point array ip and tp
					@param calib_flags	flags that modify the default behaviour of the calibration solver, see \ref cx_3d_calib_ext_flags. Use zero for default solving RX,RY,RZ, TX,TY,TZ.
				*/
				void calibrateExtrinsicFromPoints(const cx_point3r_t* ip, const cx_point3r_t* tp, unsigned numPoints, int calib_flags)
				{
					cx::checkOk(cx_3d_calibrateExtrinsicFromPoints(getHandle(), ip, tp, numPoints, calib_flags));
				}

				/** Calculate transformation (3D-Rotation and 3D-Translation) from Sensor-CS to World-CS from given point pairs. It will set the calibrations parameter CX_3D_PARAM_R and CX_3D_PARAM_T.
					If more than two pairs are give a least square estimation is used.
					The given points pairs are relative image coordinates vs. world coordinates.
					The current calibration given by hCalib is used to convert from image coordinates to sensor coordinates before estimating pose and location.
					@param ip		vector of points in sensor plane, y-component is moving direction
					@param tp		vector of points in laser (target) plane, y-component is moving direction
					@param calib_flags	flags that modify the default behaviour of the calibration solver, see \ref cx_3d_calib_ext_flags. Use zero for default solving RX,RY,RZ, TX,TY,TZ.
				*/
				void calibrateExtrinsicFromPoints(const std::vector<cx::Point3d>& ip, const std::vector<cx::Point3d>& tp, int calib_flags)
				{
					cx::checkOk(cx_3d_calibrateExtrinsicFromPoints(getHandle(), (const cx_point3r_t*)ip.data(), (const cx_point3r_t*)tp.data(), (unsigned int)ip.size(), calib_flags));
				}

				/** Calculate sensor calibration model from given target data set.
					A least square estimation is used.
					@param hTarget	handle to calibration target
					@param calib_flags	flags that modify the default behaviour of the calibration solver, see \ref cx_3d_calib_int_flags and \ref cx_3d_calib_ext_flags. Use zero for default solving algorithm.
				 */
				void calibrateIntrinsic(cx::c3d::Target& target, int calib_flags)
				{
					cx::checkOk(cx_3d_calibrateIntrinsic(getHandle(), target.getHandle(), calib_flags));
				}

				/** Calculate sensor calibration model from given point pairs.
					A least square estimation is used.
					@param ip		array of points in sensor plane, y-component is moving direction
					@param tp		array of points in laser (target) plane, y-component is moving direction
					@param numPoints	number of points in point array ip and tp
					@param calib_flags	flags that modify the default behaviour of the calibration solver, see \ref cx_3d_calib_int_flags and \ref cx_3d_calib_ext_flags. Use zero for default solving algorithm.
				*/
				void calibrateIntrinsicFromPoints(const cx_point3r_t* ip, const cx_point3r_t* tp, unsigned numPoints, int calib_flags)
				{
					cx::checkOk(cx_3d_calibrateIntrinsicFromPoints(getHandle(), ip, tp, numPoints, calib_flags));
				}

				/** Calculate sensor calibration model from given point pairs.
					A least square estimation is used.
					@param ip		vector of points in sensor plane, y-component is moving direction
					@param tp		vector of points in laser (target) plane, y-component is moving direction
					@param calib_flags	flags that modify the default behaviour of the calibration solver, see \ref cx_3d_calib_int_flags and \ref cx_3d_calib_ext_flags. Use zero for default solving algorithm.
				*/
				void calibrateIntrinsicFromPoints(const std::vector<cx::Point3d>& ip, const std::vector<cx::Point3d>& tp, int calib_flags)
				{
					cx::checkOk(cx_3d_calibrateIntrinsicFromPoints(getHandle(), (const cx_point3r_t*)ip.data(), (const cx_point3r_t*)tp.data(), (unsigned int)ip.size(), calib_flags));
				}

				CX_CALIB_HANDLE getHandle() const { return m_hCalib; }
				operator CX_CALIB_HANDLE() const { return m_hCalib; }
				operator CX_CALIB_HANDLE&() { return m_hCalib; }
				
				/** Creates a shared_ptr Calib object.
				*/
				static Calib::Ptr createShared() { return std::make_shared<Calib>(); }

			private:
				CX_CALIB_HANDLE m_hCalib;
			};

			typedef Calib::Ptr CalibPtr;
			//! @} cx_wrapper_cpp
		}
	}
}

#endif // CX_C3D_CALIB_H_INCLUDED