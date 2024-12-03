/**
@file : Target.h
@package : cx_3d library
@brief C++ Target wrapper class
@copyright (c) 2017-2021, Automation Technology GmbH.
@version 02.12.2021, AT: initial version
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
#ifndef CX_C3D_TARGET_H_INCLUDED
#define CX_C3D_TARGET_H_INCLUDED

#include "cx_3d_metric.h"
#include "cx_3d_calib.h"
#include "cx_3d_calib_int.h"
#include "AT/cx/base.h"
#include "AT/cx/Point3.h"

namespace AT {
	namespace cx {
		namespace c3d {
			//! @addtogroup cx_wrapper_cpp
			//! @{

			/** Target class is wrapping the CX_TARGET_HANDLE for easy usage of the cx_3dLib target functions in C++.
			*/
			class Target
			{
			public:
				typedef std::shared_ptr<Target> Ptr;

				Target()
				{
					m_hTarget = CX_INVALID_HANDLE;
					cx_3d_calib_new(&m_hTarget);
				}
				~Target()
				{
					cx_3d_calib_release(m_hTarget);
					m_hTarget = CX_INVALID_HANDLE;
				}

				bool isValid() const
				{
					return (m_hTarget != CX_INVALID_HANDLE);
				}

				/**	Load calibration target from file.
					@param[in] fileName		name of file
					@param[in] targetId		id-attribute to identify the target, if empty the first Target object is loaded
				 */
				void load(const std::string& fileName, const std::string& targetId);

				/**	Save calibration target to a file. It saves all target points (extraced and given points).
					If the file already exist it is openend in append mode and the new target is added to the existing content.
					@param[in] fileName		name of file
					@param[in] targetId		id-attribute to identify the target. If a target with the id already exists it will be overwritten.
				*/
				void save(const std::string& fileName, const std::string& targetId);

				/**	get parameter of the calibration target
					@param param		parameters supported, see \ref cx_3d_target_param
					@param[out] val		value of the parameter as variant
				*/
				void getParam(cx_3d_target_param_t param, cx::Variant& val) const;

				/**	set parameter of the calibration target
					@param param		parameter to set
					@param[in] val		value to set as a variant object
				*/
				void setParam(cx_3d_target_param_t param, const cx::Variant& val);

				/** Extract target points from image
					For supported targets see \ref cx_3d_calib_target.
					- chessbord, the given image is a greyscale image of a chessboard
					- Facet12 (aka static traget) given by range image
					- Profile Tragets given by one to N profiles. If multiples profiles are given they are combined to a mean profile before starting extraction.
						- trapez (aka linear target)
						- sawtooth target
						- multi-trapez target
						- step height target, mainly used for extrinsic calibration

					Multiple target scans, e.g. sawttoth target at different height levels, can be combined by calling the function with flag CX_3D_TARGET_ADD_POINTS.
					You can translate the Target geometry points (e.g. setting different height levels of the target) before calling this function by setting parameter

					@param[in] img		input range image
					@param flags		see \ref cx_3d_target_flags
				*/
				void findPoints(const cx::Image& img, int flags);

				CX_TARGET_HANDLE getHandle() const { return m_hTarget; }
				operator CX_TARGET_HANDLE() const { return m_hTarget; }
				operator CX_TARGET_HANDLE&() { return m_hTarget; }

				/** Creates a shared_ptr Calib object.
				*/
				static Target::Ptr createShared() { return std::make_shared<Target>(); }

			private:
				CX_TARGET_HANDLE m_hTarget;
			};

			typedef Target::Ptr TargetPtr;

			inline void Target::load(const std::string& fileName, const std::string& targetId)
			{
				cx::checkOk(cx_3d_target_load(fileName.c_str(), targetId.c_str(), &m_hTarget));
			}

			inline void Target::save(const std::string& fileName, const std::string& targetId)
			{
				cx::checkOk(cx_3d_target_save(m_hTarget, fileName.c_str(), targetId.c_str()));
			}

			inline void Target::getParam(cx_3d_target_param_t param, cx::Variant& val) const
			{
				cx::checkOk(cx_3d_target_get(m_hTarget, param, (cx_variant_t*)&val));
			}

			inline void Target::setParam(cx_3d_target_param_t param, const cx::Variant& val)
			{
				cx::checkOk(cx_3d_target_set(m_hTarget, param, (cx_variant_t*)&val));
			}

			inline void Target::findPoints(const cx::Image& img, int flags)
			{
				cx::checkOk(cx_3d_target_findPoints(m_hTarget, (cx_img_t*)&img, flags));
			}
			
			//! @} cx_wrapper_cpp
		}
	}
}

#endif // CX_C3D_TARGET_H_INCLUDED