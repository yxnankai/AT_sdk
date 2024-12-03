/*************************************************************************************
@brief Definition of image object for cx - API libs
@file : cx_image.h
@package : cx_base library, AT CX-libraries

@copyright (c) 2017, Automation Technology GmbH.

@version 27.11.2016, initial version cx_types
@version 01.08.2017, revised interface as separate library

*************************************************************************************/
#pragma once
#ifndef CX_IMAGE_H_INCLUDED
#define CX_IMAGE_H_INCLUDED

#include "cx_base_lib.h"
#include "cx_status.h"
#include "cx_pixel_format.h"

#ifdef __cplusplus
extern "C" {
#endif

//! @addtogroup cx_base
//! @{

/** Image flags
*/
enum cx_img_flags {
	CX_IMG_BUFFER_OWNER	= 0x01	//!< Indicating that the image buffer is owned by structure and was allocated by cx_image_alloc
};

/** Image object
	For supported pixel formats see \ref cx_pixel_format.
	it support pixels with multiple components in planar and pixel wise memory organization.
*/
typedef struct cx_img
{
	unsigned int pixelFormat;	//!< \ref cx_pixel_format pixel format definitions
	unsigned int height;		//!< height of image in pixels
	unsigned int width;			//!< width of image in pixels
	unsigned int flag;			//!< \ref cx_img_flags
	size_t linePitch;			//!< Total number of bytes between the starts of 2 consecutive rows.
	size_t planePitch;			//!< Total number of bytes between the starts of 2 consecutive planes. Zero for single plane image.
	size_t dataSz;				//!< Total data size of data buffer in bytes
#ifndef SWIG
	void* data;					//!< pointer to continous memory with payload data, can be NULL if buffer was not yet allocated
#endif
} cx_img_t;


#ifndef SWIG
/** Initializes the image
	@param[in] img		pointer to image structure.
	@return CX_STATUS_OK on success, error value otherwise.
*/
CX_BASE_API cx_status_t __cdecl cx_image_init(cx_img_t* img);

/** Calculates the line, plane pitch and total size.
	The struc member width and heigth must be set before calling this function. Function does not allocate any memory, data pointer will be not modified.
	@param[inout] img		pointer to image structure.
	@return CX_STATUS_OK on success, error value otherwise.
*/
CX_BASE_API cx_status_t __cdecl cx_image_setPitchAndSize(cx_img_t* img);

/** Initializes the image from given information
	@param[in] img			pointer to image structure.
	@param[in] height		height of image in pixels
	@param[in] width		width of image in pixels
	@param[in] pixelFormat	pixel format
	@param[in] ptr			pointer to image data
	@param[in] sz			data size
	@param[in] linePitch	line pitch, if 0 line pitch will be calculated
	@param[in] pagePitch	page pitch, if 0 page pitch will be calculated
	@return CX_STATUS_OK on success, error value otherwise.
*/
CX_BASE_API cx_status_t __cdecl cx_image_create(cx_img_t* img, unsigned int height, unsigned int width, unsigned int pixelFormat, void* ptr, size_t sz, size_t linePitch, size_t pagePitch);

/** Initializes the image and allocates array of given pixel format and size
	If the already allocated image has CX_IMG_BUFFER_OWNER set and matches width, height and pixelFormat the old pixel buffer is used.
	@param[in] img			pointer to image structure.
	@param[in] height		height of image in pixels
	@param[in] width		width of image in pixels
	@param[in] pixelFormat	pixel format
	@return CX_STATUS_OK on success, error value otherwise.
*/
CX_BASE_API cx_status_t __cdecl cx_image_alloc(cx_img_t* img, unsigned int height, unsigned int width, unsigned int pixelFormat);

/** Frees and initializes the image
	@param[in] img		pointer to image structure.
	@return CX_STATUS_OK on success, error value otherwise.
*/
CX_BASE_API cx_status_t __cdecl cx_image_free(cx_img_t* img);

/** Copies data from one image to another
	@param[in] src		pointer to source image
	@param[inout] dst	pointer to destination image
	@param[in] deep		If deep is not zero all data are copied and dst becomes owner of the newly allocated data. If deep is zero only a reference to the src.data is copied and dst is not owner of the data.
	@return CX_STATUS_OK on success, error value otherwise.
*/
CX_BASE_API cx_status_t __cdecl cx_image_copy(const cx_img_t* src, cx_img_t* dst, int deep);

/** Decode image with CX_PF_MONO_12p/CX_PF_COORD3D_C12p pixel format to CX_PF_MONO_16/CX_PF_COORD3D_C16 pixel format.
	@param[in] src		pointer to source image.
	@param[inout] dst	pointer to destination image
	@return CX_STATUS_OK on success, error value otherwise.
*/
CX_BASE_API cx_status_t __cdecl cx_image_decode_mono12p(const cx_img_t* src, cx_img_t* dst);

/** Load image from file
	@param[in] img		pointer to image structure.
	@param[in] fileName	filename of the image
	@return CX_STATUS_OK on success, error value otherwise.
*/
CX_BASE_API cx_status_t __cdecl cx_image_load(cx_img_t* img, const char* fileName);

/** Save image to file
	@param[in] img		pointer to image structure.
	@param[in] fileName	filename of the image
	@return CX_STATUS_OK on success, error value otherwise.
*/
CX_BASE_API cx_status_t __cdecl cx_image_save(const cx_img_t* img, const char* fileName);

#endif	/* SWIG */

//! @} cx_base

#ifdef __cplusplus
}
#endif
#endif /* CX_IMAGE_H_INCLUDED */
