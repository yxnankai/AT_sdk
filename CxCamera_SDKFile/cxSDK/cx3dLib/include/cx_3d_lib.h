/**
@file : cx_3d_lib.h
@package : cx_3d library
@brief C-interface for AT 3D library.
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
#ifndef CX_3D_LIB_H_INCLUDED
#define CX_3D_LIB_H_INCLUDED

/**
@defgroup cx_base			Basic types
@defgroup cx_3d_lib			Common library functions
@defgroup cx_wrapper_cpp	Helper classes and functions for C++
*/

#include "cx_base.h"

#ifndef CX_3D_API
	#if (defined WIN32 || defined _WIN32 || defined WINCE || defined __CYGWIN__)
		#ifdef CX_3D_EXPORTS
			#define CX_3D_API __declspec(dllexport)
		#else
			#define CX_3D_API __declspec(dllimport)
		#endif
	#elif defined __GNUC__ && __GNUC__ >= 4
		#define CX_3D_API __attribute__ ((visibility ("default")))
		#if defined(__i386__)
		    #ifndef __cdecl
			    #define __cdecl __attribute__((cdecl))
		    #endif
		#else
		    #define __cdecl
		#endif
	#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

//! @addtogroup cx_3d_lib
//! @{
/// Library functions

/** Returns library version
@return CX_STATUS_OK on success, error value otherwise.
*/
CX_3D_API cx_status_t __cdecl cx_3d_lib_version(int* major, int* minor, int* build);

//! @} cx_3d_lib

#ifdef __cplusplus
}
#endif

#endif /* CX_3D_LIB_H_INCLUDED */
