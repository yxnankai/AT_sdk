/**
@file : Image.h
@package : cx_base library
@brief C++ Image wrapper class
@copyright (c) 2017, Automation Technology GmbH.
@version 04.09.2017, AT: initial version
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
#ifndef AT_CX_IMAGE_H_INCLUDED
#define AT_CX_IMAGE_H_INCLUDED

#include <memory>
#include <assert.h>
#include "AT/cx/base.h"

namespace AT {
	namespace cx {
		//! @addtogroup cx_wrapper_cpp
		//! @{

		/** Class Image wraps struct cx_img for easy usage in C++.
			Class does not add any member data nor virtual functions. Therefore struct cx_img_t can be casted into cx::Image if needed.
			\note 
			Include OpenCV header "opencv2/opencv.hpp" before this file in order to get the OpenCV cv::Mat support included.
		*/
		class Image : private cx_img_t
		{
		public:
			typedef std::shared_ptr<Image> Ptr;

			Image();													//!< Creates empty image
#if(defined _MSC_VER && _MSC_VER <= 1800)
			Image(Image&& other);								//!< Move constructor.
#else
			Image(Image&& other) noexcept;								//!< Move constructor.
#endif
			Image(const Image&) = default;
			Image(const cx_img_t&, bool deep);							//!< Copy constructor, if deep=true copies also data and becomes owner
			Image(const Image&, bool deep);								//!< Copy constructor, if deep=true copies also data and becomes owner
			Image(unsigned int h, unsigned int w, cx_pixel_format pf);		//!< Constructor allocates image with given size and pixel format
			Image(unsigned int h, unsigned int w, cx_pixel_format pf, void* ptr, size_t sz, size_t lineStep = 0, size_t pageStep = 0);		//!< Constructor creates image with reference to the given buffer. Image is not owner of the buffer.
			~Image();

			// Move assignment operator.
#if(defined _MSC_VER && _MSC_VER <= 1800)
			Image& operator=(Image&& other);
#else
			Image& operator=(Image&& other) noexcept;
#endif	

			Image& operator=(const Image& other) = default;

			/** Creates image of given pixel format and size
				If the already allocated image isOwner and matches width, height and pixelFormat the old pixel buffer is used.
				@param[in] h	height of image in pixels
				@param[in] w	width of image in pixels
				@param[in] pf	pixel format
			*/
			void create(unsigned int h, unsigned int w, cx_pixel_format pf);

			/** Creates image with reference to the given buffer.
				Image is not owner of the buffer.
			*/
			void create(unsigned int h, unsigned int w, cx_pixel_format pf, void* ptr, size_t sz, size_t lineStep = 0, size_t pageStep = 0);
			void free();
			void clear() { memset(cx_img_t::data, 0, cx_img_t::dataSz); }

			unsigned flag() const { return cx_img_t::flag; }			//!< \ref cx_img_flags
			bool isOwner() const { return (flag()&CX_IMG_BUFFER_OWNER) ? true : false; }
			bool isSubImage() const { return !isOwner(); }				//!< returns true if the image is a subimage of another image
			bool isEmpty() const { return (cx_img_t::height == 0 || cx_img_t::width == 0) ? true : false; }
			size_t size() const { return cx_img_t::height * cx_img_t::width; }

			cx_pixel_format pixelFormat() const { return (cx_pixel_format)cx_img_t::pixelFormat; }
			unsigned height() const { return cx_img_t::height; }
			unsigned width() const { return cx_img_t::width; }
			size_t linePitch() const { return cx_img_t::linePitch; }		//!< Total number of bytes between the starts of 2 consecutive rows.
			size_t planePitch() const { return cx_img_t::planePitch; }	//!< Total number of bytes between the starts of 2 consecutive planes. Zero for single plane image.
			size_t dataSz() const { return cx_img_t::dataSz; }		//!< Total data size of data buffer in bytes
			void* data() const { return cx_img_t::data; }
			void* data() { return cx_img_t::data; }
			unsigned planes() const;

			/** Returns a reference to the specified array element.
				@param row Index along the dimension 0
				@param col Index along the dimension 1
			*/
			template<typename _Tp> _Tp& at(unsigned row, unsigned col);
			/** @overload
				@param row Index along the dimension 0
				@param col Index along the dimension 1
			*/
			template<typename _Tp> const _Tp& at(unsigned row, unsigned col) const;

			/** Returns a typed pointer to the specified image row.
				@param row Index along the dimension 0
			*/
			template<typename _Tp> _Tp* row(unsigned row);
			/** @overload
				@param row Index along the dimension 0
			*/
			template<typename _Tp> const _Tp* row(unsigned row) const;

			/** Returns a pointer to the cx_img struct.
				Cast operator is used for accessing cx-API functions that expect parameter of type cx_img_t.
			*/
			operator cx_img_t*() { return this; }
			/** @overload
			*/
			operator const cx_img_t*() const { return this; }

			/** Create a shared_ptr image object with the specified parameters.
			*/
			static Image::Ptr createShared(unsigned int h, unsigned int w, cx_pixel_format pf) { return std::make_shared<cx::Image>(h, w, pf); }

			/** Create a shared_ptr image object with the specified parameters.
				A reference to the supplied buffer is used, no data is copied. The ownership of the buffer remains to the caller.
			*/
			static Image::Ptr createShared(unsigned int h, unsigned int w, cx_pixel_format pf, void* ptr, size_t sz, size_t lineStep = 0, size_t pageStep = 0) { return std::make_shared<cx::Image>(h, w, pf, ptr, sz, lineStep, pageStep); }

			/** Creates a copy of the image an returns the new image as shared_ptr.
			*/
			static Image::Ptr createShared(const Image& src, bool deep = true) { return std::make_shared<cx::Image>(src, deep); }

			/** save image
			*/
			void save(const std::string& fileName) const;

			/** load image
			*/
			void load(const std::string& fileName);

			/** Decode image with CX_PF_MONO_12p/CX_PF_COORD3D_C12p pixel format to CX_PF_MONO_16/CX_PF_COORD3D_C16 pixel format.
				@param[in] src		source image.
				@param[inout] dst	destination image
			*/
			static void decodeMono12p(const cx::Image& src, cx::Image& dst)
			{
				cx::checkOk(cx_image_decode_mono12p(src, dst));
			}
		};

		typedef Image::Ptr ImagePtr;


		//! @} cx_wrapper_cpp

		// Inlines
		inline Image::Image()
		{
			cx_image_init(this);
		}
#if(defined _MSC_VER && _MSC_VER <= 1800)
		inline Image::Image(Image&& other)
#else
		inline Image::Image(Image&& other) noexcept
#endif
		{
			cx_image_init(this);
			std::swap(cx_img_t::data, ((cx_img_t*)&other)->data);
			std::swap(cx_img_t::dataSz, ((cx_img_t*)&other)->dataSz);
			std::swap(cx_img_t::flag, ((cx_img_t*)&other)->flag);
			std::swap(cx_img_t::height, ((cx_img_t*)&other)->height);
			std::swap(cx_img_t::linePitch, ((cx_img_t*)&other)->linePitch);
			std::swap(cx_img_t::pixelFormat, ((cx_img_t*)&other)->pixelFormat);
			std::swap(cx_img_t::planePitch, ((cx_img_t*)&other)->planePitch);
			std::swap(cx_img_t::width, ((cx_img_t*)&other)->width);
		}
		inline Image::Image(unsigned int h, unsigned int w, cx_pixel_format pf)
		{
			cx_image_init(this);
			cx_image_alloc(this, h, w, pf);
		}
		inline Image::Image(unsigned int h, unsigned int w, cx_pixel_format pf, void* ptr, size_t sz, size_t lineStep, size_t pageStep)
		{
			cx_image_init(this);
			cx_image_create(this, h, w, pf, ptr, sz, lineStep, pageStep);
		}
		inline Image::~Image()
		{
			free();
		}
		//inline Image::Image(const Image& src)
		//{
		//	Image(src, false);
		//}
		inline Image::Image(const cx_img_t& src, bool deep)
		{
			cx_image_init(this);
			checkOk(cx_image_copy(&src, this, deep));
		}
		inline Image::Image(const Image& src, bool deep)
		{
			cx_image_init(this);
			checkOk(cx_image_copy((cx_img_t*)&src, this, deep));
		}
#if(defined _MSC_VER && _MSC_VER <= 1800)
		inline Image& Image::operator=(Image&& other)
#else
		inline Image& Image::operator=(Image&& other) noexcept
#endif
		{
			std::swap(cx_img_t::data, ((cx_img_t*)&other)->data);
			std::swap(cx_img_t::dataSz, ((cx_img_t*)&other)->dataSz);
			std::swap(cx_img_t::flag, ((cx_img_t*)&other)->flag);
			std::swap(cx_img_t::height, ((cx_img_t*)&other)->height);
			std::swap(cx_img_t::linePitch, ((cx_img_t*)&other)->linePitch);
			std::swap(cx_img_t::pixelFormat, ((cx_img_t*)&other)->pixelFormat);
			std::swap(cx_img_t::planePitch, ((cx_img_t*)&other)->planePitch);
			std::swap(cx_img_t::width, ((cx_img_t*)&other)->width);
			return *this;
		}
		inline void Image::create(unsigned int h, unsigned int w, cx_pixel_format pf)
		{
			checkOk(cx_image_alloc(this, h, w, pf));
		}
		inline void Image::create(unsigned int h, unsigned int w, cx_pixel_format pf, void* ptr, size_t sz, size_t lineStep, size_t pageStep)
		{
			checkOk(cx_image_create(this, h, w, pf, ptr, sz, lineStep, pageStep));
		}
		inline unsigned Image::planes() const
		{
			return (unsigned)((planePitch() > 0) ? dataSz() / planePitch() : 1);
		}
		inline void Image::free()
		{
			cx_image_free(this);
		}

		template<typename _Tp> inline
			_Tp* Image::row(unsigned r)
		{
			assert(r < cx_img_t::height);
			return (_Tp*)((uint8_t*)cx_img_t::data + r * cx_img_t::linePitch);
		}

		template<typename _Tp> inline
			const _Tp* Image::row(unsigned r) const
		{
			assert(r < cx_img_t::height);
			return (const _Tp*)((uint8_t*)cx_img_t::data + r * cx_img_t::linePitch);
		}

		template<typename _Tp> inline
			_Tp& Image::at(unsigned r, unsigned c)
		{
			assert(c < cx_img_t::width);
			return row<_Tp>(r)[c];
		}

		template<typename _Tp> inline
			const _Tp& Image::at(unsigned r, unsigned c) const
		{
			assert(c < cx_img_t::width);
			return row<_Tp>(r)[c];
		}

		inline void Image::save(const std::string& fileName) const
		{
			checkOk(cx_image_save(this, fileName.c_str()));
		}

		inline void Image::load(const std::string& fileName)
		{
			checkOk(cx_image_load(this, fileName.c_str()));
		}

	}
}	// namespace AT::CX

#endif /* AT_CX_IMAGE_H_INCLUDED */
