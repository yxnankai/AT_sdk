/**
@file : exception.h
@package : cx_base library
@brief Definition of exceptions used in C++ wrapper classes
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
#ifndef AT_CX_EXCEPTIONS_H_INCLUDED
#define AT_CX_EXCEPTIONS_H_INCLUDED

#include <string>
#include <stdexcept>
#include "AT/cx/base.h"

namespace AT {
	namespace cx {
		//! @addtogroup cx_wrapper_cpp
		//! @{
		/** Runtime exception of cx Libs
		*/
		class RuntimeError : public std::runtime_error
		{
		public:
			RuntimeError(cx_status_t err) : std::runtime_error(std::string("cx runtime error: ") + cx_status_getText(err)) {}
			RuntimeError(const std::string& func, cx_status_t err) : std::runtime_error(func + " :" + cx_status_getText(err)) {}
		};

		/** exception that is thrown wenn conversion to or from variant fails within Variant assigment or cast operators.
		*/
		class BadVariantAccess : public std::runtime_error
		{
		public:
			BadVariantAccess() : std::runtime_error("bad variant access") {}
			BadVariantAccess(const std::string& arg) : std::runtime_error("bad variant access: " + arg) {}
		};

		/** Checks the status and throws cx::RuntimeError if the status is not CX_STATUS_OK.
			@param s		status, see \ref CX_STATUS_ENUM for possible values.
		*/
		inline cx_status_t checkOk(cx_status_t s)
		{
			if (s != CX_STATUS_OK)
			{
				throw(RuntimeError(s));
			}
			return s;
		}
		/** @overload
			@param func 	name of the function that returned the status s.
			@param s		status, see \ref CX_STATUS_ENUM for possible values.
		*/
		inline cx_status_t checkOk(const char* func, cx_status_t s)
		{
			if (s != CX_STATUS_OK)
			{
				throw(RuntimeError(func, s));
			}
			return s;
		}

		//! @} cx_wrapper_cpp
	}	// namespace cx
}	// namespace AT
#endif	// AT_CX_EXCEPTIONS_H_INCLUDED
