/** 
@package cx_base library
@file   cx_endianness.h
@brief  platform independend conversion of Endianness of shorts, longs, long longs
*
* Defines bswap16, bswap32, bswap64
*	ntoh16, hton16, ntoh32 hton32, ntoh64, hton64
*	cxtoh16, htocx16, cxtoh32 htocx32, cxtoh64, htocx64
*
* Supports GCC, MSVC, clang on linux and windows
* Note Windows x86/x64 is always little endian.
*
@copyright (c) 2019, Automation Technology GmbH.
@version 04.03.2019, AT: initial version
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
#ifndef CX_ENDIANNESS_H_INCLUDED
#define CX_ENDIANNESS_H_INCLUDED

#include <stdlib.h>
#include <stdint.h>

/* Detect platform endianness at compile time */
#ifdef __has_include // C++17, supported as extension to C++11 in clang, GCC 5+, vs2015
#  if __has_include(<endian.h>)
#    include <endian.h> // gnu libc normally provides, linux
#  elif __has_include(<machine/endian.h>)
#    include <machine/endian.h> //open bsd, macos
#  elif __has_include(<sys/param.h>)
#    include <sys/param.h> // mingw, some bsd (not open/macos)
#  elif __has_include(<sys/isadefs.h>)
#    include <sys/isadefs.h> // solaris
#  endif
#endif

#if !defined(__LITTLE_ENDIAN__) && !defined(__BIG_ENDIAN__)
#if ( defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__ ) || /* gcc */\
    ( defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN )  || /* linux header */\
	( defined(_BYTE_ORDER) && _BYTE_ORDER == _BIG_ENDIAN ) || ( defined(BYTE_ORDER) && BYTE_ORDER == BIG_ENDIAN ) || /* mingw header */\
    defined(_BIG_ENDIAN) || /* solaris */ \
	defined(__ARMEB__) || defined(__THUMBEB__) || defined(__AARCH64EB__) || \
    defined(_MIBSEB) || defined(__MIBSEB) || defined(__MIBSEB__) || \
    defined(_M_PPC)
#	define __BIG_ENDIAN__
#elif ( defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ ) || \
    ( defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN )  || \
	( defined(_BYTE_ORDER) && _BYTE_ORDER == _LITTLE_ENDIAN ) || ( defined(BYTE_ORDER) && BYTE_ORDER == LITTLE_ENDIAN ) || \
    defined(_LITTLE_ENDIAN) || \
    defined(__ARMEL__) || defined(__THUMBEL__) || defined(__AARCH64EL__) || \
    defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__) || \
    defined(_M_IX86) || defined(_M_X64) || defined(_M_IA64) || /* msvc for intel processors */ \
    defined(_M_ARM) /* msvc code on arm executes in little endian mode */
#	define __LITTLE_ENDIAN__
#endif
#endif

#if !defined(__LITTLE_ENDIAN__) && !defined(__BIG_ENDIAN__)
#  warning "Unknown platform, endianness not defined!"
#endif

/* Define byte-swap functions, using fast processor-native built-ins where possible */
#if defined(_MSC_VER)
#  define bswap16(x)     _byteswap_ushort((x))
#  define bswap32(x)     _byteswap_ulong((x))
#  define bswap64(x)     _byteswap_uint64((x))
//#elif ( defined(__has_builtin) && __has_builtin (bswap16) ) || /* for clang or future gcc, does not work with ocatve mingw compiler*/
#elif (__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8)
#  define bswap16(x)     __builtin_bswap16((x))
#  define bswap32(x)     __builtin_bswap32((x))
#  define bswap64(x)     __builtin_bswap64((x))
#else
    /* even in this case, compilers often optimize by using native instructions */
    static inline uint16_t bswap16(uint16_t x) {
           return (( x  >> 8 ) & 0xffu ) | (( x  & 0xffu ) << 8 );
       }
    static inline uint32_t bswap32(uint32_t x) {
        return ((( x & 0xff000000u ) >> 24 ) |
                (( x & 0x00ff0000u ) >> 8  ) |
                (( x & 0x0000ff00u ) << 8  ) |
                (( x & 0x000000ffu ) << 24 ));
    }
    static inline uint64_t bswap64(uint64_t x) {
        return ((( x & 0xff00000000000000ull ) >> 56 ) |
                (( x & 0x00ff000000000000ull ) >> 40 ) |
                (( x & 0x0000ff0000000000ull ) >> 24 ) |
                (( x & 0x000000ff00000000ull ) >> 8  ) |
                (( x & 0x00000000ff000000ull ) << 8  ) |
                (( x & 0x0000000000ff0000ull ) << 24 ) |
                (( x & 0x000000000000ff00ull ) << 40 ) |
                (( x & 0x00000000000000ffull ) << 56 ));
    }
#endif

/* define conversion functions for network byte order which is big endian */
#if defined(__LITTLE_ENDIAN__)
#  define ntoh16(x)     bswap16((x))
#  define hton16(x)     bswap16((x))
#  define ntoh32(x)     bswap32((x))
#  define hton32(x)     bswap32((x))
#  define ntoh64(x)     bswap64((x))
#  define hton64(x)     bswap64((x))
#elif defined(__BIG_ENDIAN__)
#  define ntoh16(x)     (x)
#  define hton16(x)     (x)
#  define ntoh32(x)     (x)
#  define hton32(x)     (x)
#  define ntoh64(x)     (x)
#  define hton64(x)     (x)
#endif

/* define conversion functions for cx-cameras chunk data which is little endian byte order */
#if defined(__LITTLE_ENDIAN__)
#  define cxtoh16(x)    (x)
#  define htocx16(x)    (x)
#  define cxtoh32(x)    (x)
#  define htocx32(x)    (x)
#  define cxtoh64(x)    (x)
#  define htocx64(x)    (x)
#elif defined(__BIG_ENDIAN__)
#  define cxtoh16(x)     bswap16((x))
#  define htocx16(x)     bswap16((x))
#  define cxtoh32(x)     bswap32((x))
#  define htocx32(x)     bswap32((x))
#  define cxtoh64(x)     bswap64((x))
#  define htocx64(x)     bswap64((x))
#endif

#endif // CX_ENDIANNESS_H_INCLUDED
