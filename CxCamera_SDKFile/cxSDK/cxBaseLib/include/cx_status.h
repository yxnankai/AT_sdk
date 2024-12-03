/*************************************************************************************
@brief Status Type for cx - API libs
@file : cx_status.h
@package : cx_base library, AT CX-libraries

@copyright (c) 2017, Automation Technology GmbH.

@version 27.11.2016, initial version cx_types
@version 01.08.2017, revised interface as separate library

*************************************************************************************/
#pragma once
#ifndef CX_STATUS_H_INCLUDED
#define CX_STATUS_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

//! @addtogroup cx_base
//! @{

/** Status codes resturned by cx-library functions
all failed codes are negative. Positive integer range can be used to return function specific integer values.
values are compatible with AT/FndStatus definition
*/
	typedef enum CX_STATUS_ENUM
	{
		CX_STATUS_OK = 0,						//!< OK, Success
		CX_STATUS_FAILED = -1,					//!< Generic error
		CX_STATUS_NOT_IMPLEMENTED = -2,			//!< Feature not implemented
		CX_STATUS_OPEN_FAILED = -3,				//!< Device or File open failed
		CX_STATUS_DEVICE_NOT_OPEN = -4,			//!< Device not open or connection lost
		CX_STATUS_OUT_OF_MEMORY = -5,			//!< Out of memory error
		CX_STATUS_TIMEOUT = -6,					//!< Timed out waiting for response
		CX_STATUS_PROTOCOL_ERROR = -7,			//!< Error in protocol
		CX_STATUS_DATA_ERROR = -8,				///< Data / Message was corrupted during transmission / data extraction.
		CX_STATUS_BAD_DATA_LENGTH = -9,			///< Data / Message size is not what was expected or allowed.
		CX_STATUS_WRONG_DATA_TYPE = -10,		///< unsupported data type
		CX_STATUS_INVALID_PARAMETER = -11,		//!< Invalid parameter
		CX_STATUS_CHECKSUM_ERROR = -12,			///< error in checksum, typically causes a protocol error
		CX_STATUS_WRONG_BUFFER_SIZE = -13,		//!< Given buffer size is not supported
		CX_STATUS_DEVICE_ALREADY_OPEN = -14,	//!< Device open failed because it has been previously opened before
		CX_STATUS_ACCESS_DENIED = -15,			//!< Access to device, file or parameter denied
		CX_STATUS_DEVICE_BUSY = -16,			//!< Request failed 
		CX_STATUS_INVALID_ADDRESS = -17,		//!< invalid address
		CX_STATUS_NO_DATA = -18,				//!< No data available
		CX_STATUS_INVALID_HANDLE = -19,			//!< Invalid type of handle (FdnStatus_NotFound)
		CX_STATUS_UNKNOWN_PARAMETER = -20,		//!< Use of unknown parameter (FdnStatus_FreeBufferError)
		// extension zu FdnStatus
		CX_STATUS_BAD_FORMAT = -21,				//!< Unknown file or data format
		CX_STATUS_NOT_SUPPORTED = -22,			//!< Unsupported request
		CX_STATUS_ALREADY_REGISTERED = -23,		//!< Registration failed because of previous registration
		CX_STATUS_LOADING_DEVICE_DRIVER_FAILED = -24	//!< failed loading a required device driver, e.g transportlayer lib was not found
	} cx_status_t;


	/** Frees and initializes the variant
	@param s	Status value.
	@return		Returns a string with a message that describes the status condition. The returned pointer points to a statically allocated string, which shall not be modified by the program.
	*/
	CX_BASE_API const char* __cdecl cx_status_getText(cx_status_t s);

//! @} cx_base

#ifdef __cplusplus
}
#endif
#endif /* CX_STATUS_H_INCLUDED */
