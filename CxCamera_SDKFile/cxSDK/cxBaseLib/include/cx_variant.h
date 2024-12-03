/*************************************************************************************
@brief Definition of Variant object for cx - API libs
@file : cx_variant.h
@package : cx_base library, AT CX-libraries

@copyright (c) 2017, Automation Technology GmbH.

@version 27.11.2016, initial version cx_types
@version 01.08.2017, revised interface as separate library

*************************************************************************************/
#pragma once
#ifndef CX_VARIANT_H_INCLUDED
#define CX_VARIANT_H_INCLUDED

#include "cx_base_lib.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CX_VT_TYPE_MASK 0xFF00
#define CX_VT_TYPE_ARRAY 0x8000
#define CX_VT_TYPE_STRING 0x4000
#define CX_VT_TYPE_BYTE 0x0100
#define CX_VT_TYPE_INT	0x0200
#define CX_VT_TYPE_REAL 0x0300
#define CX_VT_SIZE_MASK	0x00FF
#define CX_VT_SIZE_B		sizeof(unsigned char)
#define CX_VT_SIZE_I		sizeof(int64_t)
#define CX_VT_SIZE_R		sizeof(double)

//! @addtogroup cx_base
//! @{
typedef enum cx_vt_type {
	CX_VT_EMPTY = 0, 
	CX_VT_INT = (CX_VT_TYPE_INT+CX_VT_SIZE_I),
	CX_VT_REAL = (CX_VT_TYPE_REAL+CX_VT_SIZE_R),
	CX_VT_STRING = (CX_VT_TYPE_BYTE+CX_VT_TYPE_ARRAY+CX_VT_TYPE_STRING+CX_VT_SIZE_B),
	CX_VT_BYTE_ARRAY = (CX_VT_TYPE_BYTE+CX_VT_TYPE_ARRAY+CX_VT_SIZE_B),
	CX_VT_INT_ARRAY = (CX_VT_TYPE_INT+CX_VT_TYPE_ARRAY+CX_VT_SIZE_I),
	CX_VT_REAL_ARRAY = (CX_VT_TYPE_REAL+CX_VT_TYPE_ARRAY+CX_VT_SIZE_R)
	} cx_vt_type_t;

#ifndef SWIG

/** Structure for holding a union of data types. It is used as generic get/set data object in cx library interfaces.
The struct will be automatically converted to native types by language wrappers, eg. for Python and Matlab.
There is a wrapper class cx::Variant for usage in C++.
*/
typedef struct cx_variant
{
	union
	{
		int64_t i;		//!< integer
		double r;		//!< real
		struct {
			void* buf;	//!< ptr to buffer
			size_t len;	//!< size of buffer in elements (not in bytes)
		} a;			//!< array of char, integer or real
	} data;
	cx_vt_type_t type;	//!< \ref cx_vt_type
} cx_variant_t;

/** Initializes the variant
   @param[in] v		pointer to variant structure.
   @return CX_STATUS_OK on success, error value otherwise.
 */
CX_BASE_API cx_status_t __cdecl cx_variant_init(cx_variant_t* v);

/** Initializes the variant and allocates array of given type and size
   An array with length zero is a valid array, in this case we allocate a dummy buffer of length one.
   @param[in] v		pointer to variant structure.
   @param[in] type	type of variant array
   @param[in] num	number of elements to allocate
   @return CX_STATUS_OK on success, error value otherwise.
 */
CX_BASE_API cx_status_t __cdecl cx_variant_alloc(cx_variant_t* v, cx_vt_type_t type, size_t num);

/** Frees and initializes the variant
   @param[in] v		pointer to variant structure.
   @return CX_STATUS_OK on success, error value otherwise.
 */
CX_BASE_API cx_status_t __cdecl cx_variant_free(cx_variant_t* v);

#endif /* SWIG */
//! @} cx_base

#ifdef __cplusplus
}
#endif
#endif /* CX_VARIANT_H_INCLUDED */
