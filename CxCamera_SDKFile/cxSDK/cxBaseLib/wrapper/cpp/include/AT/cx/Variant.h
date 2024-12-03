/**
@file : Variant.h
@package : cx_base library
@brief C++ Variant wrapper class
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
#ifndef AT_CX_VARIANT_H_INCLUDED
#define AT_CX_VARIANT_H_INCLUDED

#include <cstring>
#include <string>
#include <vector>
#include "AT/cx/base.h"
#include "AT/cx/Point3.h"

/*
- QString getter und setter entfernen
- OpenCV-bezogene getter und setter entfernen -> ggf. Überadung in extra Header zur Verfügung stellen
*/

namespace AT {
	namespace cx {
		//! @addtogroup cx_wrapper_cpp
		//! @{

		/** Class Variant wraps struct cx_variant for easy usage in C++
			It adds the necessary conversion functions to C++ language types.
			Class does not add any member data or virtual functions. Therefore if needed struct cx_variant_t can be casted into cx::Variant.
			Include OpenCV header "opencv2/opencv.hpp" before this file in order to get the OpenCV types support included.
		*/
		class Variant : public cx_variant_t
		{
		public:
			Variant();
			// Move constructor.
#if(defined _MSC_VER && _MSC_VER <= 1800)
			Variant(Variant&& other);
#else
			Variant(Variant&& other) noexcept;
#endif
			Variant(const Variant&);
			Variant(bool b);
			Variant(int32_t);
			Variant(uint32_t);
			Variant(int64_t);
			Variant(uint64_t);
			Variant(int32_t, int32_t);
			Variant(uint32_t, uint32_t);
			Variant(int64_t, int64_t);
			Variant(uint64_t, uint64_t);
			Variant(double);
			Variant(double, double);
			Variant(double, double, double);
			Variant(const char*);
			Variant(const std::string&);
			Variant(const std::vector<double>&);
			Variant(const cx::Point3f&);
			Variant(const cx::Point3d&);
			~Variant();

			cx_status_t create(cx_vt_type_t type, size_t num);
			void free();
			bool isArray() const;
			size_t arrayLength() const;
			operator cx_variant_t*() { return this; }
			operator const cx_variant_t*() const { return this; }

			// Move assignment operator.
#if(defined _MSC_VER && _MSC_VER <= 1800)
			Variant& operator=(Variant&& other);
#else
			Variant& operator=(Variant&& other) noexcept;
#endif
			Variant& operator=(const Variant&);
			Variant& operator=(bool);
			Variant& operator=(int32_t a);
			Variant& operator=(uint32_t);
			Variant& operator=(int64_t);
			Variant& operator=(uint64_t);
			Variant& operator=(double);
			Variant& operator=(const char*);
			Variant& operator=(const std::string&);
			Variant& operator=(const std::vector<std::string>&);
			Variant& operator=(const std::vector<unsigned char>&);
			Variant& operator=(const std::vector<int64_t>&);
			Variant& operator=(const std::vector<double>&);

			cx_status_t set(bool);
			cx_status_t set(int32_t);
			cx_status_t set(uint32_t);
			cx_status_t set(int64_t);
			cx_status_t set(uint64_t);
			cx_status_t set(int32_t, int32_t);
			cx_status_t set(uint32_t, uint32_t);
			cx_status_t set(int64_t, int64_t);
			cx_status_t set(uint64_t, uint64_t);
			cx_status_t set(double);
			cx_status_t set(double, double);
			cx_status_t set(double, double, double);
			cx_status_t set(const cx::Point3f&);
			cx_status_t set(const cx::Point3d&);
			cx_status_t set(const char*);
			cx_status_t set(const std::string&);
			cx_status_t set(const char*, size_t);
			cx_status_t set(const int32_t*, size_t);
			cx_status_t set(const uint32_t*, size_t);
			cx_status_t set(const int64_t*, size_t);
			cx_status_t set(const uint64_t*, size_t);
			cx_status_t set(const double*, size_t);
			cx_status_t set(const std::vector<std::string>&);
			cx_status_t set(const std::vector<unsigned char>&);
			cx_status_t set(const std::vector<int64_t>&);
			cx_status_t set(const std::vector<double>&);
			cx_status_t set(const std::vector<cx_point3r_t>&);
			cx_status_t set(const std::vector<cx::Point3d>&);

			cx_status_t get(bool& b) const;
			cx_status_t get(int32_t&) const;
#if ( !defined __GNUC__ || (!defined __x86_64__ && !defined __ppc64__))
			cx_status_t get(long& l) const;
#endif
			cx_status_t get(uint32_t&) const;
			cx_status_t get(int64_t&) const;
			cx_status_t get(uint64_t&) const;
			cx_status_t get(int32_t& i0, int32_t& i1) const;
			cx_status_t get(uint32_t& i0, uint32_t& i1) const;
			cx_status_t get(int64_t& i0, int64_t& i1) const;
			cx_status_t get(uint64_t& i0, uint64_t& i1) const;
			cx_status_t get(double&) const;
			cx_status_t get(double& r0, double& r1) const;
			cx_status_t get(double& r0, double& r1, double& r2) const;
			cx_status_t get(float& flt) const;
			cx_status_t get(float& r0, float& r1, float& r2) const;
			cx_status_t get(cx::Point3f&) const;
			cx_status_t get(cx::Point3d&) const;
			cx_status_t get(int32_t*, size_t) const;
			cx_status_t get(uint32_t*, size_t) const;
			cx_status_t get(int64_t*, size_t) const;
			cx_status_t get(uint64_t*, size_t) const;
			cx_status_t get(double*, size_t) const;
			cx_status_t get(std::string&) const;
			cx_status_t get(std::vector<std::string>& vec) const;
			cx_status_t get(std::vector<unsigned char>&) const;
			cx_status_t get(std::vector<int64_t>&) const;
			cx_status_t get(std::vector<double>&) const;
			cx_status_t get(std::vector<cx_point3r_t>&) const;
			cx_status_t get(std::vector<cx::Point3d>&) const;

			operator bool();
			operator bool() const;
			operator int32_t();
			operator int32_t() const;
			operator uint32_t();
			operator uint32_t() const;
			operator int64_t();
			operator int64_t() const;
			operator uint64_t();
			operator uint64_t() const;
			operator float();
			operator float() const;
			operator double();
			operator double() const;
			operator std::string();
			operator std::string() const;
			operator cx::Point3f();
			operator cx::Point3f() const;
		};

		//! @} cx_wrapper_cpp

		// Inlines

		inline Variant::Variant()
		{
			cx_variant_init(this);
		}	
#if(defined _MSC_VER && _MSC_VER <= 1800)
		inline Variant::Variant(Variant&& other)
#else
		inline Variant::Variant(Variant&& other) noexcept
#endif
		{
			cx_variant_init(this);
			std::swap(cx_variant_t::type, ((cx_variant_t*)&other)->type);
			std::swap(cx_variant_t::data, ((cx_variant_t*)&other)->data);
		}
		inline Variant::Variant(Variant const& other)
		{
			cx_variant_init(this);
			*this = other;		// redirect to the copy assignment
		}
		inline Variant::Variant(bool b)
		{
			cx_variant_init(this);
			set(b);
		}
		inline Variant::Variant(int32_t i32)
		{
			cx_variant_init(this);
			set(i32);
		}
		inline Variant::Variant(uint32_t u32)
		{
			cx_variant_init(this);
			set(u32);
		}
		inline Variant::Variant(int64_t i64)
		{
			cx_variant_init(this);
			set(i64);
		}
		inline Variant::Variant(uint64_t u64)
		{
			cx_variant_init(this);
			set(u64);
		}
		inline Variant::Variant(int32_t i0, int32_t i1)
		{
			cx_variant_init(this);
			set(i0, i1);
		}
		inline Variant::Variant(uint32_t i0, uint32_t i1)
		{
			cx_variant_init(this);
			set(i0, i1);
		}
		inline Variant::Variant(int64_t i0, int64_t i1)
		{
			cx_variant_init(this);
			set(i0, i1);
		}
		inline Variant::Variant(uint64_t i0, uint64_t i1)
		{
			cx_variant_init(this);
			set(i0, i1);
		}
		inline Variant::Variant(double dbl)
		{
			cx_variant_init(this);
			set(dbl);
		}
		inline Variant::Variant(double r0, double r1)
		{
			cx_variant_init(this);
			set(r0, r1);
		}
		inline Variant::Variant(double r0, double r1, double r2)
		{
			cx_variant_init(this);
			set(r0, r1, r2);
		}
		inline Variant::Variant(const char* s)
		{
			cx_variant_init(this);
			set(std::string(s));
		}
		inline Variant::Variant(const std::string& s)
		{
			cx_variant_init(this);
			set(s);
		}
		inline Variant::Variant(const std::vector<double>& v)
		{
			cx_variant_init(this);
			set(v);
		}
		inline Variant::Variant(const AT::cx::Point3f& v)
		{
			cx_variant_init(this);
			set(v);
		}
		inline Variant::Variant(const AT::cx::Point3d& v)
		{
			cx_variant_init(this);
			set(v);
		}
		inline Variant::~Variant()
		{
			cx_variant_free(this);
		}

		inline cx_status_t Variant::create(cx_vt_type_t type, size_t num)
		{
			cx_variant_free(this);
			return cx_variant_alloc(this, type, num);
		}
		inline void Variant::free()
		{
			cx_variant_free(this);
		}
		inline bool Variant::isArray() const
		{
			return (type & CX_VT_TYPE_ARRAY) ? true : false;
		}
		inline size_t Variant::arrayLength() const
		{
			return (type & CX_VT_TYPE_ARRAY) ? data.a.len : 0; 
		}

#if(defined _MSC_VER && _MSC_VER <= 1800)
		inline Variant& Variant::operator=(Variant&& other)
#else
		inline Variant& Variant::operator=(Variant&& other) noexcept
#endif
		{
			std::swap(cx_variant_t::type, ((cx_variant_t*)&other)->type);
			std::swap(cx_variant_t::data, ((cx_variant_t*)&other)->data);
			return *this;
		}
		inline Variant& Variant::operator = (const Variant& other)
		{
			if (this != &other)
			{
				cx_variant_free(this);
				if (other.isArray())
				{
					cx_variant_alloc(this, other.type, other.data.a.len);	// call even if other.data.a.len==0, in case of string a dummy array with one byte is allocated
					if (other.data.a.len > 0)
						memcpy(this->data.a.buf, other.data.a.buf, other.data.a.len * (type & CX_VT_SIZE_MASK));
				}
				else
				{
					this->data = other.data;
				}
				this->type = other.type;
			}
			return *this;
		}
		inline Variant& Variant::operator= (bool b)
		{
			set(b);
			return *this;
		}
		inline Variant& Variant::operator= (int32_t i32)
		{
			set(i32);
			return *this;
		}
#if ( !defined __GNUC__ || (!defined __x86_64__ && !defined __ppc64__))
		inline cx_status_t Variant::get(long& l) const
		{
			if (type != CX_VT_INT)
				return CX_STATUS_INVALID_PARAMETER;

			l = (long)data.i;
			return CX_STATUS_OK;
		}
#endif
		inline Variant& Variant::operator= (uint32_t u32)
		{
			set(u32);
			return *this;
		}
		inline Variant& Variant::operator= (int64_t i64)
		{
			set(i64);
			return *this;
		}
		inline Variant& Variant::operator= (uint64_t ui64)
		{
			set(ui64);
			return *this;
		}
		inline Variant& Variant::operator= (double dbl)
		{
			set(dbl);
			return *this;
		}
		inline Variant& Variant::operator= (const char* s)
		{
			set(std::string(s));
			return *this;
		}
		inline Variant& Variant::operator= (const std::string& s)
		{
			set(s);
			return *this;
		}
		inline Variant& Variant::operator= (const std::vector<std::string>& vec)
		{
			set(vec);
			return *this;
		}
		inline Variant& Variant::operator= (const std::vector<unsigned char>& vc)
		{
			set(vc);
			return *this;
		}
		inline Variant& Variant::operator= (const std::vector<int64_t>& vll)
		{
			set(vll);
			return *this;
		}
		inline Variant& Variant::operator= (const std::vector<double>& vdbl)
		{
			set(vdbl);
			return *this;
		}

		inline cx_status_t Variant::set(bool b)
		{
			cx_variant_free(this);
			data.i = (b == true) ? 1 : 0;
			type = CX_VT_INT;
			return CX_STATUS_OK;
		}
		inline cx_status_t Variant::set(int32_t i32)
		{
			cx_variant_free(this);
			data.i = i32;
			type = CX_VT_INT;
			return CX_STATUS_OK;
		}
		inline cx_status_t Variant::set(uint32_t u32)
		{
			cx_variant_free(this);
			data.i = u32;
			type = CX_VT_INT;
			return CX_STATUS_OK;
		}
		inline cx_status_t Variant::set(int64_t i64)
		{
			cx_variant_free(this);
			data.i = i64;
			type = CX_VT_INT;
			return CX_STATUS_OK;
		}
		inline cx_status_t Variant::set(uint64_t u64)
		{
			cx_variant_free(this);
			data.i = u64;
			type = CX_VT_INT;
			return CX_STATUS_OK;
		}
		inline cx_status_t Variant::set(int32_t i0, int32_t i1)
		{
			cx_status_t res = create(CX_VT_INT_ARRAY, 2);
			if (res == CX_STATUS_OK)
			{
				int64_t* buf = (int64_t*)data.a.buf;
				buf[0] = i0;
				buf[1] = i1;
			}
			return res;
		}
		inline cx_status_t Variant::set(uint32_t i0, uint32_t i1)
		{
			cx_status_t res = create(CX_VT_INT_ARRAY, 2);
			if (res == CX_STATUS_OK)
			{
				int64_t* buf = (int64_t*)data.a.buf;
				buf[0] = i0;
				buf[1] = i1;
			}
			return res;
		}
		inline cx_status_t Variant::set(int64_t i0, int64_t i1)
		{
			cx_status_t res = create(CX_VT_INT_ARRAY, 2);
			if (res == CX_STATUS_OK)
			{
				int64_t* buf = (int64_t*)data.a.buf;
				buf[0] = i0;
				buf[1] = i1;
			}
			return res;
		}
		inline cx_status_t Variant::set(uint64_t i0, uint64_t i1)
		{
			cx_status_t res = create(CX_VT_INT_ARRAY, 2);
			if (res == CX_STATUS_OK)
			{
				int64_t* buf = (int64_t*)data.a.buf;
				buf[0] = i0;
				buf[1] = i1;
			}
			return res;
		}
		inline cx_status_t Variant::set(double dbl)
		{
			cx_variant_free(this);
			data.r = dbl;
			type = CX_VT_REAL;
			return CX_STATUS_OK;
		}
		inline cx_status_t Variant::set(double r0, double r1)
		{
			cx_status_t res = create(CX_VT_REAL_ARRAY, 2);
			if (res == CX_STATUS_OK)
			{
				double* buf = (double*)data.a.buf;
				buf[0] = r0;
				buf[1] = r1;
			}
			return res;
		}
		inline cx_status_t Variant::set(double r0, double r1, double r2)
		{
			cx_status_t res = create(CX_VT_REAL_ARRAY, 3);
			if (res == CX_STATUS_OK)
			{
				double* buf = (double*)data.a.buf;
				buf[0] = r0;
				buf[1] = r1;
				buf[2] = r2;
			}
			return res;
		}
		inline cx_status_t Variant::set(const cx::Point3f& v)
		{
			cx_status_t res = create(CX_VT_REAL_ARRAY, 3);
			if (res == CX_STATUS_OK)
			{
				double* buf = (double*)data.a.buf;
				buf[0] = v.x;
				buf[1] = v.y;
				buf[2] = v.z;
			}
			return res;
		}
		inline cx_status_t Variant::set(const cx::Point3d& v)
		{
			cx_status_t res = create(CX_VT_REAL_ARRAY, 3);
			if (res == CX_STATUS_OK)
			{
				double* buf = (double*)data.a.buf;
				buf[0] = v.x;
				buf[1] = v.y;
				buf[2] = v.z;
			}
			return res;
		}
		inline cx_status_t Variant::set(const char* s)
		{
			return set(std::string(s));
		}
		inline cx_status_t Variant::set(const std::string& str)
		{
			size_t len = str.length();
			cx_status_t res = create(CX_VT_STRING, len);
			if (res == CX_STATUS_OK)
				memcpy(data.a.buf, str.c_str(), len);
			return res;
		}
		inline cx_status_t Variant::set(const char* str, size_t len)
		{
			if (str == NULL)
				return CX_STATUS_INVALID_PARAMETER;

			cx_status_t res = create(CX_VT_STRING, len);
			if (res == CX_STATUS_OK)
				memcpy(data.a.buf, str, len);
			return res;
		}
		inline cx_status_t Variant::set(const int32_t* a, size_t len)
		{
			if (a == NULL)
				return CX_STATUS_INVALID_PARAMETER;

			cx_status_t res = create(CX_VT_INT_ARRAY, len);
			if (res == CX_STATUS_OK)
			{
				int64_t* dptr = (int64_t*)data.a.buf;
				for (size_t i=0; i < len; i++)
					*dptr++ = *a++;
			}
			return res;
		}
		inline cx_status_t Variant::set(const uint32_t* a, size_t len)
		{
			if (a == NULL)
				return CX_STATUS_INVALID_PARAMETER;

			cx_status_t res = create(CX_VT_INT_ARRAY, len);
			if (res == CX_STATUS_OK)
			{
				int64_t* dptr = (int64_t*)data.a.buf;
				for (size_t i=0; i < len; i++)
					*dptr++ = *a++;
			}
			return res;
		}
		inline cx_status_t Variant::set(const int64_t* a, size_t len)
		{
			if (a == NULL)
				return CX_STATUS_INVALID_PARAMETER;

			cx_status_t res = create(CX_VT_INT_ARRAY, len);
			if (res == CX_STATUS_OK)
				memcpy(data.a.buf, a, len * sizeof(int64_t));
			return res;
		}
		inline cx_status_t Variant::set(const uint64_t* a, size_t len)
		{
			if (a == NULL)
				return CX_STATUS_INVALID_PARAMETER;

			cx_status_t res = create(CX_VT_INT_ARRAY, len);
			if (res == CX_STATUS_OK)
				memcpy(data.a.buf, a, len * sizeof(uint64_t));
			return res;
		}
		inline cx_status_t Variant::set(const double* a, size_t len)
		{
			if (a == NULL)
				return CX_STATUS_INVALID_PARAMETER;

			cx_status_t res = create(CX_VT_REAL_ARRAY, len);
			if (res == CX_STATUS_OK)
				memcpy(data.a.buf, a, len * sizeof(double));
			return res;
		}
		inline cx_status_t Variant::set(const std::vector<std::string>& vec)
		{
			size_t len = 0;
			for (std::vector<std::string>::const_iterator iter = vec.begin(); iter != vec.end(); iter++)
				len += (*iter).length() + 1;

			cx_status_t res = create(CX_VT_STRING, len);
			if (res == CX_STATUS_OK)
			{
				char* dst = (char*)data.a.buf;
				for (std::vector<std::string>::const_iterator iter = vec.begin(); iter != vec.end(); iter++)
				{
					size_t sz = (*iter).length();
					memcpy(dst, (*iter).c_str(), sz);
					dst += sz;
					*dst = '\0';
					dst++;
				}
			}
			return res;
		}
		inline cx_status_t Variant::set(const std::vector<unsigned char>& vec)
		{
			cx_status_t res = create(CX_VT_BYTE_ARRAY, vec.size());
			if (res == CX_STATUS_OK)
				memcpy(data.a.buf, vec.data(), vec.size());
			return res;
		}
		inline cx_status_t Variant::set(const std::vector<int64_t>& vec)
		{
			cx_status_t res = create(CX_VT_INT_ARRAY, vec.size());
			if (res == CX_STATUS_OK)
				memcpy(data.a.buf, vec.data(), vec.size() * sizeof(int64_t));
			return res;
		}
		inline cx_status_t Variant::set(const std::vector<double>& vec)
		{
			cx_status_t res = create(CX_VT_REAL_ARRAY, vec.size());
			if (res == CX_STATUS_OK)
				memcpy(data.a.buf, vec.data(), vec.size() * sizeof(double));
			return res;
		}
		inline cx_status_t Variant::set(const std::vector<cx_point3r_t>& v)
		{
			cx_status_t res = create(CX_VT_REAL_ARRAY, v.size() * 3);
			if (res == CX_STATUS_OK && data.a.len > 0)
				memcpy(data.a.buf, v.data(), data.a.len * sizeof(double));
			return res;
		}
		inline cx_status_t Variant::set(const std::vector<cx::Point3d>& v)
		{
			cx_status_t res = create(CX_VT_REAL_ARRAY, v.size() * 3);
			if (res == CX_STATUS_OK && data.a.len > 0)
				memcpy(data.a.buf, v.data(), data.a.len * sizeof(double));
			return res;
		}

		inline cx_status_t Variant::get(bool& b) const
		{
			if (type != CX_VT_INT)
				return CX_STATUS_INVALID_PARAMETER;

			b = (data.i == 0) ? false : true;
			return CX_STATUS_OK;
		}
		inline cx_status_t Variant::get(int32_t& i32) const
		{
			if (type != CX_VT_INT)
				return CX_STATUS_INVALID_PARAMETER;

			i32 = (int32_t)data.i;
			return CX_STATUS_OK;
		}
		inline cx_status_t Variant::get(uint32_t& u32) const
		{
			if (type != CX_VT_INT)
				return CX_STATUS_INVALID_PARAMETER;

			u32 = (uint32_t)data.i;
			return CX_STATUS_OK;
		}
		inline cx_status_t Variant::get(int64_t& i64) const
		{
			if (type != CX_VT_INT)
				return CX_STATUS_INVALID_PARAMETER;

			i64 = (int64_t)data.i;
			return CX_STATUS_OK;
		}
		inline cx_status_t Variant::get(uint64_t& u64) const
		{
			if (type != CX_VT_INT)
				return CX_STATUS_INVALID_PARAMETER;

			u64 = (uint64_t)data.i;
			return CX_STATUS_OK;
		}
		inline cx_status_t Variant::get(int32_t& i0, int32_t& i1) const
		{
			if (type != CX_VT_INT_ARRAY || data.a.len != 2)
				return CX_STATUS_INVALID_PARAMETER;

			int64_t* buf = (int64_t*)data.a.buf;
			i0 = (int32_t)buf[0];
			i1 = (int32_t)buf[1];
			return CX_STATUS_OK;
		}
		inline cx_status_t Variant::get(uint32_t& i0, uint32_t& i1) const
		{
			if (type != CX_VT_INT_ARRAY || data.a.len != 2)
				return CX_STATUS_INVALID_PARAMETER;

			int64_t* buf = (int64_t*)data.a.buf;
			i0 = (uint32_t)buf[0];
			i1 = (uint32_t)buf[1];
			return CX_STATUS_OK;
		}
		inline cx_status_t Variant::get(int64_t& i0, int64_t& i1) const
		{
			if (type != CX_VT_INT_ARRAY || data.a.len != 2)
				return CX_STATUS_INVALID_PARAMETER;

			int64_t* buf = (int64_t*)data.a.buf;
			i0 = (int64_t)buf[0];
			i1 = (int64_t)buf[1];
			return CX_STATUS_OK;
		}
		inline cx_status_t Variant::get(uint64_t& i0, uint64_t& i1) const
		{
			if (type != CX_VT_INT_ARRAY || data.a.len != 2)
				return CX_STATUS_INVALID_PARAMETER;

			int64_t* buf = (int64_t*)data.a.buf;
			i0 = (uint64_t)buf[0];
			i1 = (uint64_t)buf[1];
			return CX_STATUS_OK;
		}
		inline cx_status_t Variant::get(double& dbl) const
		{
			if (type == CX_VT_REAL)
				dbl = data.r;
			else if (type == CX_VT_INT)
				dbl = (double)data.i;
			else
				return CX_STATUS_INVALID_PARAMETER;

			return CX_STATUS_OK;
		}
		inline cx_status_t Variant::get(double& r0, double& r1) const
		{
			if (type != CX_VT_REAL_ARRAY || data.a.len != 2)
				return CX_STATUS_INVALID_PARAMETER;

			double* buf = (double*)data.a.buf;
			r0 = buf[0];
			r1 = buf[1];
			return CX_STATUS_OK;
		}
		inline cx_status_t Variant::get(double& r0, double& r1, double& r2) const
		{
			if (type != CX_VT_REAL_ARRAY || data.a.len != 3)
				return CX_STATUS_INVALID_PARAMETER;

			double* buf = (double*)data.a.buf;
			r0 = buf[0];
			r1 = buf[1];
			r2 = buf[2];
			return CX_STATUS_OK;
		}
		inline cx_status_t Variant::get(float& flt) const
		{
			if (type == CX_VT_REAL)
				flt = (float)data.r;
			else if (type == CX_VT_INT)
				flt = (float)data.i;
			else
				return CX_STATUS_INVALID_PARAMETER;

			return CX_STATUS_OK;
		}
		inline cx_status_t Variant::get(float& r0, float& r1, float& r2) const
		{
			if (type != CX_VT_REAL_ARRAY || data.a.len != 3)
				return CX_STATUS_INVALID_PARAMETER;

			double* buf = (double*)data.a.buf;
			r0 = (float)buf[0];
			r1 = (float)buf[1];
			r2 = (float)buf[2];
			return CX_STATUS_OK;
		}
		inline cx_status_t Variant::get(cx::Point3f& v) const
		{
			if (type != CX_VT_REAL_ARRAY || data.a.len != 3)
				return CX_STATUS_INVALID_PARAMETER;

			double* buf = (double*)data.a.buf;
			v.x = (float)buf[0];
			v.y = (float)buf[1];
			v.z = (float)buf[2];
			return CX_STATUS_OK;
		}
		inline cx_status_t Variant::get(cx::Point3d& v) const
		{
			if (type != CX_VT_REAL_ARRAY || data.a.len != 3)
				return CX_STATUS_INVALID_PARAMETER;

			double* buf = (double*)data.a.buf;
			v.x = buf[0];
			v.y = buf[1];
			v.z = buf[2];
			return CX_STATUS_OK;
		}
		inline cx_status_t Variant::get(int32_t* dst, size_t len) const
		{
			if (type != CX_VT_INT_ARRAY || len > data.a.len)
				return CX_STATUS_INVALID_PARAMETER;

			int64_t* src = (int64_t*)data.a.buf;
			for (size_t i = 0; i < len; i++)
				*dst++ = (int32_t)*src++;

			return CX_STATUS_OK;
		}
		inline cx_status_t Variant::get(uint32_t* dst, size_t len) const
		{
			if (type != CX_VT_INT_ARRAY || len > data.a.len)
				return CX_STATUS_INVALID_PARAMETER;

			int64_t* src = (int64_t*)data.a.buf;
			for (size_t i = 0; i < len; i++)
				*dst++ = (uint32_t)*src++;

			return CX_STATUS_OK;
		}
		inline cx_status_t Variant::get(int64_t* arr, size_t len) const
		{
			if (type != CX_VT_INT_ARRAY || len > data.a.len)
				return CX_STATUS_INVALID_PARAMETER;

			size_t cpysz = len * sizeof(int64_t);
			memcpy(arr, data.a.buf, cpysz);
			return CX_STATUS_OK;
		}
		inline cx_status_t Variant::get(uint64_t* arr, size_t len) const
		{
			if (type != CX_VT_INT_ARRAY || len > data.a.len)
				return CX_STATUS_INVALID_PARAMETER;

			size_t cpysz = len * sizeof(int64_t);
			memcpy(arr, data.a.buf, cpysz);
			return CX_STATUS_OK;
		}
		inline cx_status_t Variant::get(double* arr, size_t len) const
		{
			if (type != CX_VT_REAL_ARRAY || len > data.a.len)
				return CX_STATUS_INVALID_PARAMETER;

			size_t cpysz = len * sizeof(double);
			memcpy(arr, data.a.buf, cpysz);
			return CX_STATUS_OK;
		}
		inline cx_status_t Variant::get(std::string& str) const
		{
			if (type != CX_VT_STRING)
				return CX_STATUS_INVALID_PARAMETER;

			size_t len = data.a.len;
			if (len > 0 && ((char*)data.a.buf)[len - 1] == '\0')
				len--;

			str.resize(len);
			memcpy((void*)str.c_str(), data.a.buf, len);

			return CX_STATUS_OK;
		}
		inline cx_status_t Variant::get(std::vector<std::string>& vec) const
		{
			if (type != CX_VT_STRING)
				return CX_STATUS_INVALID_PARAMETER;

			vec.clear();
			char* src = (char*)data.a.buf;
			while (src < (char*)data.a.buf + data.a.len)
			{
				vec.push_back(src);
				src += strlen(src) + 1;
			}
			return CX_STATUS_OK;
		}
		inline cx_status_t Variant::get(std::vector<unsigned char>& vec) const
		{
			if (type != CX_VT_BYTE_ARRAY)
				return CX_STATUS_INVALID_PARAMETER;

			size_t sz = data.a.len;
			vec.resize(data.a.len);
			memcpy(vec.data(), data.a.buf, sz);
			return CX_STATUS_OK;
		}
		inline cx_status_t Variant::get(std::vector<int64_t>& vec) const
		{
			if (type != CX_VT_INT_ARRAY)
				return CX_STATUS_INVALID_PARAMETER;

			size_t sz = data.a.len * sizeof(int64_t);
			vec.resize(data.a.len);
			memcpy(vec.data(), data.a.buf, sz);
			return CX_STATUS_OK;
		}
		inline cx_status_t Variant::get(std::vector<double>& vec) const
		{
			if (type != CX_VT_REAL_ARRAY)
				return CX_STATUS_INVALID_PARAMETER;

			size_t sz = data.a.len * sizeof(double);
			vec.resize(data.a.len);
			memcpy(vec.data(), data.a.buf, sz);
			return CX_STATUS_OK;
		}
		inline cx_status_t Variant::get(std::vector<cx_point3r_t>& v) const
		{
			if (type != CX_VT_REAL_ARRAY)
				return CX_STATUS_INVALID_PARAMETER;
			v.resize(data.a.len / 3);
			memcpy(v.data(), data.a.buf, data.a.len * sizeof(double));
			return CX_STATUS_OK;
		}
		inline cx_status_t Variant::get(std::vector<cx::Point3d>& v) const
		{
			if (type != CX_VT_REAL_ARRAY)
				return CX_STATUS_INVALID_PARAMETER;
			v.resize(data.a.len / 3);
			memcpy(v.data(), data.a.buf, data.a.len * sizeof(double));
			return CX_STATUS_OK;
		}

		inline Variant::operator bool()
		{
			if (type != CX_VT_INT)
				throw(AT::cx::BadVariantAccess("bool"));
			return (data.i == 0) ? false : true;
		}
		inline Variant::operator bool() const
		{
			if (type != CX_VT_INT)
				throw(AT::cx::BadVariantAccess("bool const"));
			return (data.i == 0) ? false : true;
		}
		inline Variant::operator int32_t()
		{
			if (type != CX_VT_INT)
				throw(AT::cx::BadVariantAccess("int32_t"));
			return (int32_t)data.i;
		}
		inline Variant::operator int32_t() const
		{
			if (type != CX_VT_INT)
				throw(AT::cx::BadVariantAccess("int32_t const"));
			return (int32_t)data.i;
		}
		inline Variant::operator uint32_t()
		{
			if (type != CX_VT_INT)
				throw(AT::cx::BadVariantAccess("uint32_t"));
			return (uint32_t)data.i;
		}
		inline Variant::operator uint32_t() const
		{
			if (type != CX_VT_INT)
				throw(AT::cx::BadVariantAccess("uint32_t const"));
			return (uint32_t)data.i;
		}
		inline Variant::operator int64_t()
		{
			if (type != CX_VT_INT)
				throw(AT::cx::BadVariantAccess("int64_t"));
			return data.i;
		}
		inline Variant::operator int64_t() const
		{
			if (type != CX_VT_INT)
				throw(AT::cx::BadVariantAccess("int64_t const"));
			return data.i;
		}
		inline Variant::operator uint64_t()
		{
			if (type != CX_VT_INT)
				throw(AT::cx::BadVariantAccess("uint64_t"));
			return (uint64_t)data.i;
		}
		inline Variant::operator uint64_t() const
		{
			if (type != CX_VT_INT)
				throw(AT::cx::BadVariantAccess("uint64_t const"));
			return (uint64_t)data.i;
		}
		inline Variant::operator float()
		{
			float d = 0.0f;
			if (type == CX_VT_REAL)
				d = (float)data.r;
			else if (type == CX_VT_INT)
				d = (float)data.i;
			else
				throw(AT::cx::BadVariantAccess("float"));
			return d;
		}
		inline Variant::operator float() const
		{
			float d = 0.0f;
			if (type == CX_VT_REAL)
				d = (float)data.r;
			else if (type == CX_VT_INT)
				d = (float)data.i;
			else
				throw(AT::cx::BadVariantAccess("float const"));
			return d;
		}
		inline Variant::operator double()
		{
			double d = 0.0;
			if (type == CX_VT_REAL)
				d = data.r;
			else if (type == CX_VT_INT)
				d = (double)data.i;
			else
				throw(AT::cx::BadVariantAccess("double"));
			return d;
		}
		inline Variant::operator double() const
		{
			double d = 0.0;
			if (type == CX_VT_REAL)
				d = data.r;
			else if (type == CX_VT_INT)
				d = (double)data.i;
			else
				throw(AT::cx::BadVariantAccess("double const"));
			return d;
		}
		inline Variant::operator std::string()
		{
			std::string str;
			if (get(str) != CX_STATUS_OK)
				throw(AT::cx::BadVariantAccess("string"));
			return str;
		}
		inline Variant::operator std::string() const
		{
			std::string str;
			if (get(str) != CX_STATUS_OK)
				throw(AT::cx::BadVariantAccess("string const"));
			return str;
		}
		inline Variant::operator cx::Point3f()
		{
			cx::Point3f p;
			get(p);
			return p;
		}
		inline Variant::operator cx::Point3f() const
		{
			cx::Point3f p;
			get(p);
			return p;
		}
	}	// namespace cx
}	// namespace AT

#endif /* AT_CX_VARIANT_H_INCLUDED */
