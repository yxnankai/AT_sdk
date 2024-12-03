/*************************************************************************************
@brief Definition of chunk data structs and IDs for cx - API libs
@file : cx_chunk_data.h
@package : cx_base library, AT CX-libraries

@copyright (c) 2017, Automation Technology GmbH.

@version 27.11.2016, initial version cx_types
@version 01.08.2017, revised interface as separate library

*************************************************************************************/
#pragma once
#ifndef CX_CHUNK_DATA_H_INCLUDED
#define CX_CHUNK_DATA_H_INCLUDED

#include "cx_struct_packing.h"

#ifdef __cplusplus
extern "C" {
#endif

/* @addtogroup cx_base
   @{
*/

CX_STRUCT_PACKED_START
#if defined(__MINGW32__) || defined(__MINGW64__)
#pragma pack(1)
#endif

struct CX_CHUNK_DESCRIPTOR
{
	uint32_t descriptor;
	uint32_t length;
} CX_STRUCT_PACKED;

#define CX_CHUNK_DESCRIPTOR_SIZE 8


#if defined(__MINGW32__) || defined(__MINGW64__)
#pragma pack()
#endif
CX_STRUCT_PACKED_END


#ifndef SWIG
typedef struct CX_CHUNK_DESCRIPTOR cx_chunk_descriptor_t;

static_assert(sizeof(cx_chunk_descriptor_t) == CX_CHUNK_DESCRIPTOR_SIZE, "wrong struct size, check struct packing and alignment");
#endif	/* SWIG */

/// payload part description = chunk data struct
typedef struct cx_chunk
{
	uint32_t descriptor;		//!< \ref cx_chunk_id
#ifndef SWIG
	size_t length;				//!< length of this part in bytes
	void* data;					//!< pointer to continous memory with chunk data
#endif // SWIG
} cx_chunk_t;

//! @} cx_base

#ifdef __cplusplus
}
#endif
#endif /* CX_CHUNK_DATA_H_INCLUDED */
