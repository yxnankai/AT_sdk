/**
@file : Chunk.h
@package : cx_base library
@brief C++ Chunk data wrapper class
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
#ifndef AT_CX_CHUNK_H_INCLUDED
#define AT_CX_CHUNK_H_INCLUDED

#include "AT/cx/base.h"
#include "cx_endianness.h"

namespace AT {
	namespace cx {
		//! @addtogroup cx_wrapper_cpp
		//! @{

		/** Class Chunk wraps struct cx_chunk_t for easy usage in C++
			Class does not add any member data nor virtual functions. Therefore struct cx_chunk_t can be casted into cx::Chunk if needed.
		*/
		class Chunk : public cx_chunk_t
		{
		public:
			typedef std::shared_ptr<Chunk> Ptr;

			Chunk()
			{
				memset(this, 0, sizeof(cx_chunk_t));
			}

			operator cx_chunk_t*()				{ return this; }
			operator const cx_chunk_t*() const	{ return this; }
			
			/** Create a shared_ptr chunk object.
			*/
			static Chunk::Ptr createShared()
			{
				return std::make_shared<cx::Chunk>();
			}
		};

		typedef Chunk::Ptr ChunkPtr;

		//! @} cx_wrapper_cpp
	}	// namespace cx
}	// namespace AT
#endif	// AT_CX_CHUNK_H_INCLUDED
