/*************************************************************************************
@brief Definition of compiler dependend struct packing macros for cx_base library
@file : cx_struct_packing.h
@package : cx_base library, AT CX-libraries

@copyright (c) 2017, Automation Technology GmbH.

@version 01.12.2017, initial version cx_types

*************************************************************************************/
#pragma once
#ifndef CX_STRUCT_PACKING_H_INCLUDED
#define CX_STRUCT_PACKING_H_INCLUDED

#ifdef SWIG
#	define CX_STRUCT_PACKED_START
#	define CX_STRUCT_PACKED_END
#	define CX_STRUCT_ALIGNED
#	define CX_STRUCT_PACKED
#else
/* compiler dependend packet alignement */
#	if defined(__MINGW32__) || defined(__MINGW64__)
#		define CX_STRUCT_ALIGNED __attribute__((aligned (1)))
#		define CX_STRUCT_PACKED	__attribute__((packed))
#		define CX_STRUCT_PACKED_START
#		define CX_STRUCT_PACKED_END
#	elif defined(__GNUC__)
#		define CX_STRUCT_ALIGNED __attribute__((aligned (1)))
#		define CX_STRUCT_PACKED	__attribute__((packed))
#		define CX_STRUCT_PACKED_START
#		define CX_STRUCT_PACKED_END
#	elif defined(_MSC_VER)
#		define CX_STRUCT_ALIGNED
#		define CX_STRUCT_PACKED
#		define CX_STRUCT_PACKED_START __pragma(pack(push))\
									  __pragma(pack(1))
#		define CX_STRUCT_PACKED_END	__pragma(pack(pop))
#	else
#		warning "Unknown platform, struct packing not defined!"
#		define CX_STRUCT_ALIGNED
#		define CX_STRUCT_PACKED
#		define CX_STRUCT_PACKED_START
#		define CX_STRUCT_PACKED_END
#   endif
#endif /* SWIG */

#endif /* CX_STRUCT_PACKING_H_INCLUDED */
