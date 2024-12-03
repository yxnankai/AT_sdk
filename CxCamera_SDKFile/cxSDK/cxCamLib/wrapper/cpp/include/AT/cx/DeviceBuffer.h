/**
@file : DeviceBuffer.h
@package : cx_cam library
@brief C++ DeviceBuffer wrapper class
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
#ifndef AT_CX_DEVICEBUFFER_H_INCLUDED
#define AT_CX_DEVICEBUFFER_H_INCLUDED

#include <string>
#include "AT/cx/base.h"
#include "cx_cam.h"
#include "cx_cam_param.h"
#include "AT/cx/Device.h"

namespace AT {
	namespace cx {

		//! @addtogroup cx_wrapper_cpp
		//! @{

		/** DeviceBuffer class is wrapping the CX_BUFFER_HANDLE.
		*/
		class DeviceBuffer
		{
		public:
			DeviceBuffer(CX_BUFFER_HANDLE hBuffer) : m_hBuffer(hBuffer) {}
			~DeviceBuffer() {}

			bool isValid() const
			{
				return (m_hBuffer != CX_INVALID_HANDLE);
			}

			/** Get image from acquisition buffer. The function allocates an image object and returns a shared pointer to it. The image holds a reference to the buffer data, no copy happens.
								
				\code{.cpp}
					cx::DeviceBuffer buffer = cam->waitForBuffer(1000);
					{
						auto img = buffer.getImage();
						// do processing of img ...
					}
					buffer.queueBuffer();
				\endcode
				
				\note 
				The returned image holds a reference to the image data in the DeviceBuffer, if you need the image data after queueBuffer (\ref cx_queueBuffer) you need to clone (deep copy) the image!
			*/
			cx::ImagePtr getImage(int partIdx=0)
			{
				cx::ImagePtr img = std::make_shared<cx::Image>();
				cx::checkOk("cx_getBufferImage", cx_getBufferImage(m_hBuffer, partIdx, *img));
				return img;
			}

			/** Get image from acquisition buffer. The function uses the provide image object, no allocation or copy happens.
				
				\code{.cpp}
					cx::DeviceBuffer buffer = cam->waitForBuffer(1000);
					{
						cx::Image img;
						buffer.getImage(img);
						// do processing of img ...
					}
					buffer.queueBuffer();
				\endcode
				
				\note 
				The image holds a reference to the image data in the DeviceBuffer, if you need the image data after queueBuffer (\ref cx_queueBuffer) you need to clone (deep copy) the image!
			*/
			void getImage(cx::Image& img, int partIdx=0)
			{
				cx::checkOk("cx_getBufferImage", cx_getBufferImage(m_hBuffer, partIdx, img));
			}

			cx::ChunkPtr getChunk(int chunkIdx=0)
			{
				cx::ChunkPtr chunk = std::make_shared<cx::Chunk>();
				cx::checkOk("cx_getBufferChunk", cx_getBufferChunk(m_hBuffer, chunkIdx, *chunk));
				return chunk;
			}

			void getChunk(cx::Chunk& chunk, int chunkIdx=0)
			{
				cx::checkOk("cx_getBufferChunk", cx_getBufferChunk(m_hBuffer, chunkIdx, chunk));
			}

			void getInfo(cx_buffer_info param, cx::Variant& val)
			{
				cx::checkOk("cx_getBufferInfo", cx_getBufferInfo(m_hBuffer, param, val));
			}

			void getPartInfo(int partIdx, cx_buffer_part_info param, cx::Variant& val)
			{
				cx::checkOk("cx_getBufferPartInfo", cx_getBufferPartInfo(m_hBuffer, partIdx, param, val));
			}

			void queueBuffer()
			{
				cx::checkOk("cx_queueBuffer", cx_queueBuffer(m_hBuffer));
			}

		protected:
			operator CX_BUFFER_HANDLE() const { return m_hBuffer; }

		private:
			CX_BUFFER_HANDLE m_hBuffer;
		};

		//! @} cx_wrapper_cpp
	}	// namespace cx
}	// namespace AT
#endif	// AT_CX_DEVICEBUFFER_H_INCLUDED
