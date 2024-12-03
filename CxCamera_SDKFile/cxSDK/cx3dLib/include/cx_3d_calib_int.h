/**
@file : cx_3d_calib_int.h
@package : cx_3d library
@brief C-interface for AT 3D library for intrinsic calibration.
@copyright (c) 2017, Automation Technology GmbH.
@version 18.10.2016, AT: initial version
@version 01.08.2017, AT: revised interface
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
#ifndef CX_3D_CALIB_INT_H_INCLUDED
#define CX_3D_CALIB_INT_H_INCLUDED

/**
@defgroup cx_3d_calibrate	Calibration functions
*/

#include "cx_3d_lib.h"

#ifdef __cplusplus
extern "C" {
#endif


/*************************************************************************************/
//! @addtogroup cx_3d_calibrate
//! @{

//! Flags that modify the default behaviour of the calibration solver, see also \ref cx_3d_calib_ext_flags
enum cx_3d_calib_int_flags
{
	CX_3D_CALIB_FIX_CP		= 0x80000001,	//!< do not vary principal point
	CX_3D_CALIB_FIX_L_K1	= 0x80000002,	//!< do not vary radial lens distortion parameter k1
	CX_3D_CALIB_SOLVE_L_K2	= 0x00000004,	//!< include additional parameter k2 for radial lens distortion
	CX_3D_CALIB_SOLVE_L_K3	= 0x00000008,	//!< include additional parameter k3 for radial lens distortion, only Brown–Conrady lens correction model
	CX_3D_CALIB_SOLVE_L_P12 = 0x00000010,	//!< include additional tangential distortion parameter p1, p2, only Brown–Conrady lens correction model
	CX_3D_CALIB_FIX_H		= 0x80000020,	//!< do not vary homography
	CX_3D_CALIB_SOLVE_F		= 0x00000040,	//!< solve for focal length
	CX_3D_CALIB_SOLVE_SRX	= 0x00000080,	//!< solve for sensor rotation around X=Scheimpflug
	CX_3D_CALIB_SOLVE_C_P2	= 0x00020000,	//!< solve for correction polynom 2nd order, 4th order is default
	CX_3D_CALIB_SOLVE_C_P3	= 0x00040000	//!< solve for correction polynom 3rd order, 4th order is default
};

/** Calculate sensor calibration model from given target data set. 
	A least square estimation is used.
	@param hCalib	handle to the calibration, can be created before with \ref cx_3d_calib_new or \ref cx_3d_calib_load
	@param hTarget	handle to calibration target
	@param calib_flags	flags that modify the default behaviour of the calibration solver, see \ref cx_3d_calib_int_flags and \ref cx_3d_calib_ext_flags. Use zero for default solving algorithm.
 */
CX_3D_API cx_status_t __cdecl cx_3d_calibrateIntrinsic(CX_CALIB_HANDLE hCalib, CX_TARGET_HANDLE hTarget, int calib_flags);

/** Calculate sensor calibration model from given point pairs.
	A least square estimation is used.
	@param hCalib	handle to the calibration, can be created before with \ref cx_3d_calib_new or \ref cx_3d_calib_load
	@param ip		array of points in sensor plane, y-component is moving direction
	@param tp		array of points in laser (target) plane, y-component is moving direction
	@param numPoints	number of points in point array ip and tp
	@param calib_flags	flags that modify the default behaviour of the calibration solver, see \ref cx_3d_calib_int_flags and \ref cx_3d_calib_ext_flags. Use zero for default solving algorithm.
*/
CX_3D_API cx_status_t __cdecl cx_3d_calibrateIntrinsicFromPoints(CX_CALIB_HANDLE hCalib, const cx_point3r_t* ip, const cx_point3r_t* tp, unsigned numPoints, int calib_flags);

//! @} cx_3d_calibrate

#ifdef __cplusplus
}
#endif

#endif /* CX_3D_CALIB_INT_H_INCLUDED */
