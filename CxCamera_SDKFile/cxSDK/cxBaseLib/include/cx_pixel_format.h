/*************************************************************************************
@brief Pixel Format Types for cx - API libs
@file : cx_pixel_format.h
@package : cx_base library, AT CX-libraries

@copyright (c) 2017, Automation Technology GmbH.

@version 27.11.2016, initial version cx_types
@version 01.08.2017, revised interface as separate library

*************************************************************************************/
#pragma once
#ifndef CX_PIXEL_FORMAT_H_INCLUDED
#define CX_PIXEL_FORMAT_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

//! @addtogroup cx_base
//! @{

/** Definition of supported pixel formats for cx_image functions.
	Pixelformat definitions according to GenICam SFNC, values according to GenICam PFNC see PFNC.h.
	XXX_PLANAR means the components are in separate consecutive parts of the images, e.g. AAAAABBBBBCCCCC
	whereas non planar means, the compnents are stored per pixel, e.g. ABCABCABCABCABC
	CX_PF_COORD3D_XXX according to Scan3D category. 
*/
enum cx_pixel_format {
	CX_PF_MONO_8				= 0x01080001,	//!< Monochrome 8-bit
	CX_PF_MONO_10				= 0x01100003,	//!< Monochrome 10-bit unpacked
	CX_PF_MONO_10p				= 0x010A0046,	//!< Monochrome 10-bit packed, two 10 bit pixels packed into 24 bits
	CX_PF_MONO_12				= 0x01100005,	//!< Monochrome 12-bit unpacked
	CX_PF_MONO_12p				= 0x010C0047,	//!< Monochrome 12-bit packed, two 12 bit pixels packed into 24 bits
	CX_PF_MONO_14				= 0x01100025,	//!< Monochrome 14-bit unpacked
	CX_PF_MONO_16				= 0x01100007,	//!< Monochrome 16-bit
	CX_PF_MONO_32				= 0x01200200,	//!< Monochrome 32-bit, AT Extension
	CX_PF_MONO_64				= 0x01400300,	//!< Monochrome 64-bit, AT Extension
	CX_PF_COORD3D_C8			= 0x010800B1,	//!< C-Coordinate (if cartesian coordinate system Z-value) 8-bit
	CX_PF_COORD3D_C12p			= 0x010C00DA,	//!< C-Coordinate (if cartesian coordinate system Z-value) two 12-bit packed into 24 bits
	CX_PF_COORD3D_C16			= 0x011000B8,	//!< C-Coordinate (if cartesian coordinate system Z-value) 16-bit
	CX_PF_COORD3D_C32f			= 0x012000BF,	//!< C-Coordinate (if cartesian coordinate system Z-value) 32-bit float
	CX_PF_COORD3D_ABC8			= 0x021800B2,	//!< ABC-Coordinate pixels (if cartesian coordinate system XYZ tuple) 8-bit per ccordinate value
	CX_PF_COORD3D_ABC8_PLANAR	= 0x021800B3,	//!< ABC-Coordinate in three planes (if cartesian coordinate system XYZ) 8-bit per ccordinate value
	CX_PF_COORD3D_ABC16			= 0x023000B9,	//!< ABC-Coordinate pixels (if cartesian coordinate system XYZ tuple) 16-bit per ccordinate value
	CX_PF_COORD3D_ABC16_PLANAR	= 0x023000BA,	//!< ABC-Coordinate in three planes (if cartesian coordinate system XYZ) 16-bit per ccordinate value
	CX_PF_COORD3D_ABC32f		= 0x026000C0,	//!< ABC-Coordinate pixels (if cartesian coordinate system XYZ tuple) 32-bit float per ccordinate value
	CX_PF_COORD3D_ABC32f_PLANAR	= 0x026000C1	//!< ABC-Coordinate in three planes (if cartesian coordinate system XYZ) 32-bit float per ccordinate value
}; // cx_pixel_format

typedef enum cx_pixel_format cx_pixel_format_t;

//! @addtogroup cx_pixel_format_strings Pixel Format Strings
//! @{
/// Definition of pixel fromat names according to GenICam PFNC, see \ref cx_pixel_format for explaination.
#define CX_PF_MONO_8_STR				"Mono8"
#define CX_PF_MONO_10_STR				"Mono10"
#define CX_PF_MONO_10p_STR				"Mono10p"
#define CX_PF_MONO_12_STR				"Mono12"
#define CX_PF_MONO_12p_STR				"Mono12p"
#define CX_PF_MONO_14_STR				"Mono14"
#define CX_PF_MONO_16_STR				"Mono16"
#define CX_PF_MONO_32_STR				"Mono32"
#define CX_PF_MONO_64_STR				"Mono64"
#define CX_PF_COORD3D_C8_STR			"Coord3D_C8"
#define CX_PF_COORD3D_C12p_STR			"Coord3D_12p"
#define CX_PF_COORD3D_C16_STR			"Coord3D_C16"
#define CX_PF_COORD3D_C32f_STR			"Coord3D_C32f"
#define CX_PF_COORD3D_ABC8_STR			"Coord3D_ABC8"
#define CX_PF_COORD3D_ABC8_PLANAR_STR	"Coord3D_ABC8_Planar"
#define CX_PF_COORD3D_ABC16_STR			"Coord3D_ABC16"
#define CX_PF_COORD3D_ABC16_PLANAR_STR	"Coord3D_ABC16_Planar"
#define CX_PF_COORD3D_ABC32f_STR		"Coord3D_32f"
#define CX_PF_COORD3D_ABC32f_PLANAR_STR	"Coord3D_32f_Planar"
//! @} cx_pixel_format_strings

//! @} cx_base

#ifdef __cplusplus
}
#endif
#endif /* CX_PIXEL_FORMAT_H_INCLUDED */
