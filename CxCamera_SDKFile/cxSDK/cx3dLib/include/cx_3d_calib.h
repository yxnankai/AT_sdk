/**
@file : cx_3d_calib.h
@package : cx_3d library
@brief C-interface for AT 3D library calibration functions.
@copyright (c) 2017, Automation Technology GmbH.
@version 18.10.2016, initial version
@version 01.08.2017, revised interface
@version 08.06.2022, added CX_3D_CALIB_MODEL_N_POLY correction
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
#ifndef CX_3D_CALIB_H_INCLUDED
#define CX_3D_CALIB_H_INCLUDED

/**
@defgroup cx_3d_target		Calibration Target functions
@defgroup cx_3d_calibrate	Calibration functions
*/

#include "cx_3d_lib.h"
#include "cx_3d_metric.h"

#ifdef __cplusplus
extern "C" {
#endif

//! @addtogroup cx_3d_target
//! @{

/** Create a new calibration target
	@param[out] phTarget	handle to the new created calibration target, set to CX_INVALID_HANDLE if failed
*/
CX_3D_API cx_status_t __cdecl cx_3d_target_new(CX_TARGET_HANDLE* phTarget);

/**	Release calibration target, frees all resources internally used.
	@param hTarget		handle of the calibration target to release
	@return CX_STATUS_OK on success, error value otherwise.
*/
CX_3D_API cx_status_t __cdecl cx_3d_target_release(CX_TARGET_HANDLE hTarget);

/**	Load calibration target from file.
	@param[in] fileName		name of file
	@param[in] targetId		id-attribute to identify the target, if empty the first Target object is loaded
	@param[out] phTarget	returns handle of the loaded calibration, set to CX_INVALID_HANDLE if failed
	@return CX_STATUS_OK	on success, error value otherwise.
			CX_STATUS_INVALID_PARAMETER		if targetId was not found.
			CX_STATUS_BAD_FORMAT			if format or version does not fit.
 */
CX_3D_API cx_status_t __cdecl cx_3d_target_load(const char* fileName, const char* targetId, CX_TARGET_HANDLE* phTarget);

/**	Save calibration target to a file. It saves all target points (extraced and given points).
	If the file already exist it is openend in append mode and the new target is added to the existing content.
	@param hTarget			handle of the target to store
	@param[in] fileName		name of file
	@param[in] targetId		id-attribute to identify the target. If a target with the id already exists it will be overwritten.
	@return CX_STATUS_OK on success, error value otherwise.
*/
CX_3D_API cx_status_t __cdecl cx_3d_target_save(CX_TARGET_HANDLE hTarget, const char* fileName, const char* targetId);

/**	Parameter for cx_3d_target_get/set
*/
typedef enum cx_3d_target_param {
	CX_3D_PARAM_TARGET_ID,			//!< target ID, RO property, internally set when reading or writing. The id is used to identify multiple targets in one file.
	CX_3D_PARAM_TARGET_TYPE,		//!< target type as integer, see \ref cx_3d_calib_target
	CX_3D_PARAM_TARGET_NAME,		//!< target name as string
	CX_3D_PARAM_TARGET_DESCRIPTION,	//!< target description as string
	CX_3D_PARAM_TARGET_SN,			//!< target serial number as string
	CX_3D_PARAM_TARGET_GP,			//!< array of target geometry points in target CS
	CX_3D_PARAM_TARGET_IP,			//!< array of detected target points in image CS
	CX_3D_PARAM_TARGET_TP,			//!< array of target points in target CS corresponding to the detected image points
	CX_3D_PARAM_TARGET_RES,			//!< get matrix of residuen, typically it is an image of same size as input range map
	CX_3D_PARAM_TARGET_LABEL_MASK,	//!< get segmentation mask of identified primitives, typically it is an image of same size as input range map
	CX_3D_PARAM_TARGET_GP_T,		//!< additional translation parameter (x,y,z) for the transformation of target geometry points to target points. E.g. can be used for setting different height levels of the same target geometry.
	CX_3D_PARAM_TARGET_GP_R,		//!< additional rotation parameter (x,y,z) for the transformation of target geometry points to target points.
	CX_3D_PARAM_TARGET_FP,			//!< fit parameters for line and plane fits
	CX_3D_PARAM_TARGET_SP,			//!< segmentation parameters for detection of the primitives e.g. lines and planes
	CX_3D_PARAM_TARGET_TGP,			//!< target geometry parameters for analytical description of the target geometry
	CX_3D_PARAM_TARGET_STAT			//!< get statistic values of target point extraction process
} cx_3d_target_param_t;

/**	get parameter of the calibration target
	@param hTarget		handle of the target
	@param param		parameters supported, see \ref cx_3d_target_param
	@param[out] val		pointer to variable for return.
	@return CX_STATUS_OK on success
			CX_STATUS_INVALID_PARAMETER if parameter is not available.
*/
CX_3D_API cx_status_t __cdecl cx_3d_target_get(CX_TARGET_HANDLE hTarget, cx_3d_target_param_t param, cx_variant_t* val);

/**	set parameter of the calibration target
	@param hTarget		handle of the target
	@param param		parameter to set
	@param[in] val		pointer to value. If variant data type does not match expected type for parameter CX_STATUS_INVALID_PARAMETER is returned.
	@return CX_STATUS_OK on success
			CX_STATUS_INVALID_PARAMETER if parameter is not available.
*/
CX_3D_API cx_status_t __cdecl cx_3d_target_set(CX_TARGET_HANDLE hTarget, cx_3d_target_param_t param, const cx_variant_t* val);

//! Definition of target flags, the chunk related bits are compatible with \ref cx_3d_metric_flags
enum cx_3d_target_flags
{
	CX_3D_TARGET_USE_CHUNK_Y = 0x0100,		//!< chunkVec contains Y coordinate per profile that should be used for transformation. E.g. encoder position from chunk data. 
	CX_3D_TARGET_USE_CHUNK_Z0 = 0x0200,		//!< chunkVec contains Z0 coordinate per profile that should be used for transformation. E.g. aoi-ys position from chunk data when using AOI-tracking and absolut value is false.
	CX_3D_TARGET_ADD_POINTS = 0x0800		//!< add the newly detected points to the existing target points.
};

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
	@param hTarget		handle of the target
	@param[in] img		input range image
	@param flags		see \ref cx_3d_target_flags
	@return CX_STATUS_OK on success
*/
CX_3D_API cx_status_t __cdecl cx_3d_target_findPoints(CX_TARGET_HANDLE hTarget, const cx_img_t* img, int flags);

//! @} cx_3d_target

/*************************************************************************************/
//! @addtogroup cx_3d_calibrate
//! @{

//! Flags that modify the default behaviour of the calibration solver
enum cx_3d_calib_ext_flags
{
	CX_3D_CALIB_FIX_FLAG		= 0x80000000,	//!< flag indicates that the given parameters are fixed and wil be excluded from optimization
	CX_3D_CALIB_TARGET_DEFAULT	= 0x40000000,	//!< use the targets default solve flags, can be combined with other SOLVE flags
	CX_3D_CALIB_SOLVE_TX		= 0x00000100,	//!< extrinsic translation along X axis
	CX_3D_CALIB_SOLVE_TY		= 0x00000200,	//!< extrinsic translation along Y axis
	CX_3D_CALIB_SOLVE_TZ		= 0x00000400,	//!< extrinsic translation along Z axis
	CX_3D_CALIB_SOLVE_RX		= 0x00000800,	//!< extrinsic rotation around X axis
	CX_3D_CALIB_SOLVE_RY		= 0x00001000,	//!< extrinsic rotation around Y axis
	CX_3D_CALIB_SOLVE_RZ		= 0x00002000,	//!< extrinsic rotation around Z axis
	CX_3D_CALIB_SOLVE_SY		= 0x00004000,	//!< solve for scaling along Y axis, extrinsic parameter
	CX_3D_CALIB_SOLVE_SXY		= 0x00008000,	//!< solve for X axis skew depending on Y, extrinsic parameter
	CX_3D_CALIB_SOLVE_SZY		= 0x00010000,	//!< solve for Z axis skew depending on Y, extrinsic parameter
	CX_3D_CALIB_TARGET_INIT_SY	= 0x04000000	//!< initialize SY with a target specific estimation
};

/** Calculate transformation (3D-Rotation and 3D-Translation) from Sensor-CS to World-CS from given target data set. It will set the calibrations parameter CX_3D_PARAM_R and CX_3D_PARAM_T.
	The current calibration given by hCalib is used to convert from image coordinates to sensor coordinates before estimating pose and location.
	@param hCalib	handle to the calibration, can be created before with \ref cx_3d_calib_new or \ref cx_3d_calib_load
	@param hTarget	handle to calibration target
	@param calib_flags	flags that modify the default behaviour of the calibration solver, see \ref cx_3d_calib_ext_flags. Use CX_3D_CALIB_TARGET_DEFAULT for default solving algorithm which depends on the actual target.
 */
CX_3D_API cx_status_t __cdecl cx_3d_calibrateExtrinsic(CX_CALIB_HANDLE hCalib, CX_TARGET_HANDLE hTarget, int calib_flags);

/** Calculate transformation (3D-Rotation and 3D-Translation) from Sensor-CS to World-CS from given point pairs. It will set the calibrations parameter CX_3D_PARAM_R and CX_3D_PARAM_T.
	If more than two pairs are give a least square estimation is used.
	The given points pairs are relative image coordinates vs. world coordinates.
	The current calibration given by hCalib is used to convert from image coordinates to sensor coordinates before estimating pose and location.
	@param hCalib	handle to the calibration, can be created before with \ref cx_3d_calib_new or \ref cx_3d_calib_load
	@param ip		array of points in sensor plane, y-component is moving direction
	@param tp		array of points in laser (target) plane, y-component is moving direction
	@param numPoints	number of points in point array ip and tp
	@param calib_flags	flags that modify the default behaviour of the calibration solver, see \ref cx_3d_calib_ext_flags. Use zero for default solving RX,RY,RZ, TX,TY,TZ.
*/
CX_3D_API cx_status_t __cdecl cx_3d_calibrateExtrinsicFromPoints(CX_CALIB_HANDLE hCalib, const cx_point3r_t* ip, const cx_point3r_t* tp, unsigned numPoints, int calib_flags);

//! @} cx_3d_calibrate

#ifdef __cplusplus
}
#endif

#endif /* CX_3D_CALIB_H_INCLUDED */
