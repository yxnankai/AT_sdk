/*************************************************************************************
@brief Basic Types and API definitions for cx_bae library
@file : cx_base_lib.h
@package : cx_base library, AT CX-libraries

@copyright (c) 2017, Automation Technology GmbH.

@version 27.11.2016, initial version cx_types
@version 01.08.2017, revised interface as separate library

*************************************************************************************/
#pragma once
#ifndef CX_BASE_LIB_H_INCLUDED
#define CX_BASE_LIB_H_INCLUDED

#include <stdint.h> 
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CX_BASE_API
	#if (defined WIN32 || defined _WIN32 || defined WINCE || defined __CYGWIN__)
		#ifdef CX_BASE_EXPORTS
			#define CX_BASE_API __declspec(dllexport)
		#else
			#define CX_BASE_API __declspec(dllimport)
		#endif
	#elif defined __GNUC__ && __GNUC__ >= 4
		#define CX_BASE_API __attribute__ ((visibility ("default")))
		#if defined(__i386__)
		    #ifndef __cdecl
			    #define __cdecl __attribute__((cdecl))
		    #endif
		#else
		    #define __cdecl
		#endif
	#endif
#endif


//! @addtogroup cx_base
//! @{

typedef void* cx_handle_t;					//!< generic handle used in cx-libraries
#define CX_INVALID_HANDLE	0				//!< invalid handle value for cx_handle_t

// specific handles
typedef cx_handle_t	CX_DEVICE_HANDLE;		//!< Device handle, obtained through cx_openDevice
typedef cx_handle_t	CX_BUFFER_HANDLE;		//!< Buffer handle, obtained through cx_waitForBuffer 
typedef cx_handle_t	CX_CALIB_HANDLE;		//!< Calibration handle, obtained through the cx_3d_calib_new/load
typedef cx_handle_t	CX_TARGET_HANDLE;		//!< Calibration target handle, obtained through the cx_3d_target_new/load
typedef cx_handle_t	CX_EVENT_HANDLE;		//!< Event handle, obtained through the cx_registerEventCallback
typedef cx_handle_t	CX_EVENT_DATA_HANDLE;	//!< Event data handle, obtained through the cx_EventCallback

// callback function prototype
typedef void(*cx_event_cb)  (CX_DEVICE_HANDLE hDevice, const char* name, void* userParam, CX_EVENT_DATA_HANDLE hEventData);

/**	Point in 3D space with three real coordinates.
Can be used for cartesian, spherical and cylindrical coordiantes
can be converted to OpenCV CvPoint3D64f, see OpenCV header "types_c.h"
*/
typedef struct cx_point3r
{
	double a, b, c;	//!< cartesian (x,y,z); sperical(theta,phi,rho); cylindrical(theta,y,rho).
} cx_point3r_t;


//! @} cx_base

#ifdef __cplusplus
}
#endif
#endif /* CX_BASE_LIB_H_INCLUDED */
