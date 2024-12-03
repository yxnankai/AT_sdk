/**
@file : base.h
@package : cx_base library
@brief C++ wrapper include file for cx_base lib

Include OpenCV header "opencv2/opencv.hpp" before this file in order to get the OpenCV support included.

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
#ifndef AT_CX_BASE_H_INCLUDED
#define AT_CX_BASE_H_INCLUDED

#include "cx_base.h"

#ifdef OPENCV_CORE_HPP
#define CX_SUPPORT_OPENCV 1
#endif

#ifdef QSTRING_H
#define CX_SUPPORT_QT 1
#endif

#include "AT/cx/exception.h"
#include "AT/cx/Variant.h"
#include "AT/cx/Image.h"
#include "AT/cx/Chunk.h"
#include "AT/cx/Point3.h"

namespace AT {
	namespace cx {
		//! @addtogroup cx_wrapper_cpp
		//! @{
		//! @} cx_wrapper_cpp
	}	// namespace cx
} // namespace AT
#endif  // AT_CX_BASE_H_INCLUDED
