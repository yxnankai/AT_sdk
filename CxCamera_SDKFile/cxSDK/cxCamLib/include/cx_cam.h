/*************************************************************************************
@brief cx - camera API for device discovery, image acquisition and parameter handling
@file : cx_cam.h
@package : cx_cam library, AT CX-libraries

@copyright (c) 2017, Automation Technology GmbH.

@version 27.11.2016, initial version 
@version 01.08.2017, revised interface, cx_base as separate library, new buffer functions, waitForImage, releaseImg removed

*************************************************************************************/
#pragma once

#ifndef CX_CAM_H_INCLUDED
#define CX_CAM_H_INCLUDED

/**
@defgroup cx_base Basic structures and types
@defgroup cx_cam_uri Device URI
@defgroup cx_cam_dd Device Discovery
@defgroup cx_cam_device Device access and image aquisition
@defgroup cx_cam_device_param Device parameter access
@defgroup cx_wrapper_cpp	Helper classes for C++ binding
@defgroup cx_wrapper_Python	Helper classes for Python binding
@defgroup cx_cam_checklicenses License functions
@defgroup cx_cam_lib Library utillity functions
*/

#include "cx_base.h"

#ifndef CX_CAM_API
	#if (defined WIN32 || defined _WIN32 || defined WINCE || defined __CYGWIN__)
		#ifdef CX_CAM_EXPORTS
			#define CX_CAM_API __declspec(dllexport)
		#else
			#define CX_CAM_API __declspec(dllimport)
		#endif
	#elif defined __GNUC__ && __GNUC__ >= 4
		#define CX_CAM_API __attribute__ ((visibility ("default")))
		#if defined(__i386__)
		    #ifndef __cdecl
			    #define __cdecl __attribute__((cdecl))
		    #endif
		#else
		    #define __cdecl
		#endif

	#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

//! @addtogroup cx_cam_lib
//! @{
/// Library functions

/** Returns library version
@return CX_STATUS_OK on success, error value otherwise.
*/
CX_CAM_API cx_status_t __cdecl cx_lib_version(int* major, int* minor, int* build);

/** Initialize library resources
@return CX_STATUS_OK on success, error value otherwise.
*/
CX_CAM_API cx_status_t __cdecl cx_lib_init();


/** Frees all internal library resources
@return CX_STATUS_OK on success, error value otherwise.
*/
CX_CAM_API cx_status_t __cdecl cx_lib_free();

//! @} cx_cam_lib


//! @addtogroup cx_cam_device_param
//! @{

/** Parameter type
*/
typedef enum cx_param_type
{
	CX_PARAM_INTEGER	= 2,		//!< integer parameter type
	CX_PARAM_BOOLEAN	= 3,		//!< boolean parameter type
	CX_PARAM_COMMAND	= 4,		//!< command parameter type
	CX_PARAM_FLOAT		= 5,		//!< float parameter type
	CX_PARAM_STRING		= 6,		//!< string parameter type
	CX_PARAM_CATEGORY	= 8,		//!< category
	CX_PARAM_ENUM		= 9,		//!< enumerator parameter type
	CX_PARAM_UNDEFINED	= 0xFFFF,	//!< unknown parameter type
} cx_param_type_t;
//! @} cx_cam_device_param

/*************************************************************************************/
//! @addtogroup cx_cam_checklicenses
//! @{

// possible license discover flags
enum cx_lic_flags
{
	CX_LIC_ALL = 1,			//!< discover all known license types
	CX_LIC_CVB = 2			//!< discover Stemmer Imaging AG TL and CVB licenses
};

/**	Find licenses.
@param	maxWaitTime		maximum time in milliseconds to wait for licenses to discover.
@param	flags 			flags for controlling license discovery process, \ref cx_lic_flags
 */
CX_CAM_API cx_status_t __cdecl cx_lic_findLicenses(unsigned int maxWaitTime, cx_lic_flags flags);

/** Returns the number of all discovered licenses.
   @param[out] pNumberLicenses number of discovered licenses.
   @return CX_STATUS_OK on success, error value otherwise.
 */
CX_CAM_API cx_status_t __cdecl cx_lic_getNumFoundLicenses(unsigned int* pNumberLicenses);

/** Get information about a discovered license.
   @param[in] licIdx				index of the license
   @param[in] prm					parameter name, see also \ref cam_lic_param for common standard parameter names.
   @param[out] val					pointer to variable for return.
   @return CX_STATUS_OK on success, error value otherwise.
 */
CX_CAM_API cx_status_t __cdecl cx_lic_getParam(unsigned int licIdx, const char* prm, cx_variant_t* val);


//! @} cx_cam_checklicenses

/*************************************************************************************/
//! @addtogroup cx_cam_dd
//! @{

// possible discover flags
enum cx_dd_flags
{
	CX_DD_USE_GEV					= 1,	//!< use discovery according to GEV standard
	CX_DD_USE_GEV_BROADCAST			= 2,	//!< use discovery according to GEV standard with broadcast inorder to discover device in other subnets
	CX_DD_USE_MDNS					= 4,	//!< use mDNS/DNS-SD (aka Bonjour) discovery
	CX_DD_STOP_AFTER_FIRST_DEVICE	= 8		//!< stop discovery after the first device was found
};

/**	Discover devices using the given discovery services, see \ref cx_dd_flags.
Filter URI Examples:

URI-Filter								|				Remark
----------------------------------------|----------------------------------
"" (empty string)						|	Searches all network interfaces for all available devices.
filter://192.168.1.2					|	Searches all network interfaces for a device with the unicast ip 192.168.1.2
filter://192.168.1.2?mask=255.255.0.0	|	Searches for cameras within the given network address range.

@param	uriFilter		URI-Filter, e.g. could be the network adapter. If NULL no filter is applied for device discovery. Parameter not yet supported.
@param	maxWaitTime		maximum time in milliseconds to wait for devices to discover.
@param	flags 			flags for controlling discovery process, \ref cx_dd_flags
 */
CX_CAM_API cx_status_t __cdecl cx_dd_findDevices(const char* uriFilter, unsigned int maxWaitTime, unsigned int flags);
	
/** Returns the number of all discovered devices.
   @param[out] pNumberDevices number of discovered devices
   @return CX_STATUS_OK on success, error value otherwise.
 */
CX_CAM_API cx_status_t __cdecl cx_dd_getNumFoundDevices(unsigned int* pNumberDevices);

/** Set parameter of discovered device before open the device.
	Allows the user to change parameters of a device befor actually open it. Typically connection parameters are set with this function.
	@param[in] devIdx				index of discovered device
	@param[in] prm					parameter name string, currently only CX_CAM_DD_FORCE_IP supported.
	@param[in] val					pointer to value. Function automatically converts given variant data type into target parameter data type. If no conversion is possible CX_STATUS_INVALID_PARAMETER is returned.
	@return CX_STATUS_OK on success, error value otherwise.
*/
CX_CAM_API cx_status_t __cdecl cx_dd_setParam(unsigned int devIdx, const char* prm, const cx_variant_t* val);

/** Get information about a discovered device.
   @param[in] devIdx				index of the device
   @param[in] prm					parameter name, see also \ref cam_dd_param for common standard parameter names. 
   @param[out] val					pointer to variable for return.
   @return CX_STATUS_OK on success, error value otherwise.
 */
CX_CAM_API cx_status_t __cdecl cx_dd_getParam(unsigned int devIdx, const char* prm, cx_variant_t* val);

/** Device status flags, can be querried with CX_CAM_DD_DEVICE_STATUS.
*/
enum cx_dd_device_status {
	CX_DD_STATUS_MISCONFIGURED = 1,	//!< the device is in state when it cannot be opened due to its current configuration. Examples are wrong driver(USB) or wrong subnet(Ethernet).
	CX_DD_STATUS_CONTROL_READY = 2,	//!< the device is ready for opening in control(RW) access.
	CX_DD_STATUS_MONITOR_READY = 4	//!< the device is ready for opening in monitor(RO) access
};

//! @} cx_cam_dd

/*************************************************************************************/
//! @addtogroup cx_cam_device
//! @{

/** Open camera given bei URI, see \ref cx_cam_uri.
    It populates structures, runs initializing procedures, allocates resources - prepares the camera for work.
		
	URI-Examples																			|				Remark
	----------------------------------------------------------------------------------------|----------------------------------
	gev://192.168.1.2/?nif=00-1B-21-D4-91-90												|	Connects the device with ip 192.168.1.2 connected to nic with mac 00-1B-21-D4-91-90 using the default library (Stemmer Imaging GEVTL.cti).
	gev://192.168.1.2/?nif=00-1B-21-D4-91-90&xml=C:\\Temp\\CX-1280_GigE_1.2.0.xml			|	Like the first example but loading a custom camera XML file from "C:\Temp\CX-1280_GigE_1.2.0.xml".
	gev://192.168.1.2/?mac=00-50-C2-8E-DD-EE&nif=00-1B-21-D4-91-90&lib=mvGenTLProducer.cti	|	Like the first example but using the MatrixVision library instead the default library. For MV you must specify the nif and the device's mac!
	
	The device access mode can be given with option "access". The following modes are supported:
	- "readonly" : allows read only access to the device when no application with exclusive access is linked with the device
	- "exclusive" : default mode, exclusive access to the device. No other application can read or write to the device.
	- "control" : Application can read and write to the device. But other applications are allowed to read from the device.
	
	\note 
	Function creates and returns handle of the specified device. To de-initialize the camera and destroy the handler cx_closeDevice should be called.
	
	@param[in] uri			uri of the device, see \ref cx_cam_uri
	@param[out] phDevice	handle to device created
	@return CX_STATUS_OK on success, error value otherwise.
 */
CX_CAM_API cx_status_t __cdecl cx_openDevice(const char* uri, CX_DEVICE_HANDLE* phDevice);

/** Uninitialize device   
   Closes camera handle and releases allocated resources.
   @param[in] hDevice					handle to device
   @return CX_STATUS_OK on success, error value otherwise.
 */
CX_CAM_API cx_status_t __cdecl cx_closeDevice(CX_DEVICE_HANDLE hDevice);

/** Uninitialize all previously opened device
Closes camera handles and releases allocated resources.
@return CX_STATUS_OK on success, error value otherwise.
*/
CX_CAM_API cx_status_t __cdecl cx_closeAllDevices();

/** Start image acquisition   
   Begins the work cycle and starts data acquisition from the camera.
   @param[in] hDevice					handle to device
   @return CX_STATUS_OK on success, error value otherwise.
 */
CX_CAM_API cx_status_t __cdecl cx_startAcquisition(CX_DEVICE_HANDLE hDevice);

/** Stop image acquisition
   Ends the work cycle of the camera, stops data acquisition and deallocates internal image buffers.
   @param[in] hDevice					handle to device
   @return CX_STATUS_OK on success, error value otherwise.
 */
CX_CAM_API cx_status_t __cdecl cx_stopAcquisition(CX_DEVICE_HANDLE hDevice);

/** allocate and queue buffers for acquisition engine
   @param[in] hDevice		handle to device
   @param[in] num_buffers	
   @return CX_STATUS_OK on success, error value otherwise.
 */
CX_CAM_API cx_status_t __cdecl cx_allocAndQueueBuffers(CX_DEVICE_HANDLE hDevice, int num_buffers);

/** queue buffer again to acquisition engine
	buffer returned by waitForBuffer
   @param[in] hBuffer		handle to buffer
   @return CX_STATUS_OK on success, error value otherwise.
 */
CX_CAM_API cx_status_t __cdecl cx_queueBuffer(CX_BUFFER_HANDLE hBuffer);

/** free all acquisition buffers
   @param[in] hDevice			handle to device
   @return CX_STATUS_OK on success, error value otherwise.
 */
CX_CAM_API cx_status_t __cdecl cx_freeBuffers(CX_DEVICE_HANDLE hDevice);

/** \brief Return new grabbed buffer. Automatically dequeues the buffer for user processing. Buffer must be queued again after processing.
   @param[in] hDevice					handle to device
   @param[out] phBuffer					pointer to buffer handle
   @param[in] timeout					time interval required to wait for the buffer (in milliseconds).
   @return CX_STATUS_OK on success, error value otherwise.
 */
CX_CAM_API cx_status_t __cdecl cx_waitForBuffer(CX_DEVICE_HANDLE hDevice, CX_BUFFER_HANDLE* phBuffer, unsigned int timeout);

/** \brief Return Image from grabbed buffer.
   @param[in] hBuffer					handle to buffer
   @param[in] partIdx					index of image part if multipart payload, otherwise ignored, \ref CX_BUFFER_INFO_NUM_PARTS
   @param[out] img						pointer to image
   @return CX_STATUS_OK on success, error value otherwise.

   \note
   The image holds a reference to the image data in the DeviceBuffer, if you need the image data after \ref cx_queueBuffer you need to clone (deep copy) the image!
 */
CX_CAM_API cx_status_t __cdecl cx_getBufferImage(CX_BUFFER_HANDLE hBuffer, int partIdx, cx_img_t* img);

/** \brief Return chunk data buffer from grabbed buffer.
	@param[in] hBuffer					handle to buffer
	@param[in] chunkIdx					index of chunk part \ref CX_BUFFER_INFO_NUM_CHUNK
	@param[out] chunk					pointer to chunk struct \ref cx_chunk
	@return CX_STATUS_OK on success, error value otherwise.

	\note
	The chunk object holds a reference to the data in the DeviceBuffer, if you need the data after \ref cx_queueBuffer you need to clone (deep copy) the data!
*/
CX_CAM_API cx_status_t __cdecl cx_getBufferChunk(CX_BUFFER_HANDLE hBuffer, int chunkIdx, cx_chunk_t* chunk);

//! possible parameters for cx_getBufferInfo
enum cx_buffer_info {
	CX_BUFFER_INFO_TIMESTAMP		= 1,	//!< timestamp of payload buffer
	CX_BUFFER_INFO_NUM_PARTS		= 2,	//!< number of image parts within payload buffer
	CX_BUFFER_INFO_NUM_CHUNK		= 3,	//!< number of chunk parts within payload buffer
	CX_BUFFER_INFO_IS_INCOMPLETE	= 4,	//!< buffer was filled but an error occurred during that process
	CX_BUFFER_INFO_IS_MULTIPART		= 5		//!< buffer contains multipart data
};

/** Query information of acquisition buffer.
	@param[in] hBuffer		handle to buffer
	@param[in] param		Parameter info Id, \ref cx_buffer_info
	@param[out] val			pointer to parameter value
	@return CX_STATUS_OK on success, error value otherwise.
*/
CX_CAM_API cx_status_t __cdecl cx_getBufferInfo(CX_BUFFER_HANDLE hBuffer, int param, cx_variant_t* val);

//! multipart buffer part types
enum cx_buffer_part_type {
	CX_BUFFER_PART_TYPE_ID_UNDEFINED = 0,
	CX_BUFFER_PART_TYPE_ID_IMAGE2D = 1,	//!< image type
	CX_BUFFER_PART_TYPE_ID_TOKEN = 2,	//!< at token image type
	CX_BUFFER_PART_TYPE_ID_CHUNK = 3,	//!< chunk data
	CX_BUFFER_PART_TYPE_ID_CALIBIR = 4,	//!< IR calibration data
	CX_BUFFER_PART_TYPE_ID_CALIB3D = 5,	//!< 3D calibration data
	CX_BUFFER_PART_TYPE_ID_CONFIG = 6		//!< Camera configuration (e.g. UserSet file in JSON format, GenICam featurebag, ...)
};

//! multipart buffer part purpose ids
enum cx_buffer_part_purpose {
	CX_BUFFER_PART_PURPOSE_ID_UNDEFINED = 0x00,
	CX_BUFFER_PART_PURPOSE_ID_INTENSITY = 0x01,
	CX_BUFFER_PART_PURPOSE_ID_INFRARED = 0x02,
	CX_BUFFER_PART_PURPOSE_ID_RANGE = 0x04,
	CX_BUFFER_PART_PURPOSE_ID_REFLECTANCE = 0x05,
	CX_BUFFER_PART_PURPOSE_ID_CONFIDENCE = 0x06,
	CX_BUFFER_PART_PURPOSE_ID_SCATTER = 0x07,
	CX_BUFFER_PART_PURPOSE_ID_RANGE_AT_TOKEN = 0x2A,
	CX_BUFFER_PART_PURPOSE_ID_EXTENDED_COMPONENT = 0x8000,
	CX_BUFFER_PART_PURPOSE_ID_METADATA = 0x8001,
	CX_BUFFER_PART_PURPOSE_ID_CUSTOM_COMPONENT = 0xFF00,   // + 0x0-0xFE valid
	CX_BUFFER_PART_PURPOSE_ID_RESERVED_COMPONENT = 0xFFFF
};

//! possible parameters for cx_getBufferPartInfo
enum cx_buffer_part_info {
	CX_BUFFER_PART_INFO_DATA_SIZE		= 1,	//!< buffer part size in bytes
	CX_BUFFER_PART_INFO_TYPE_ID			= 2,	//!< buffer part type id \ref cx_buffer_part_type
	CX_BUFFER_PART_INFO_DATA_FORMAT		= 3,	//!< buffer part data format (e.g. pixelformat)
	CX_BUFFER_PART_INFO_WIDTH			= 5,	//!< buffer part data width in px
	CX_BUFFER_PART_INFO_HEIGHT			= 6,	//!< buffer part data height in px
	CX_BUFFER_PART_INFO_XOFFSET			= 7,	//!< horizontal offset of buffer part data in px
	CX_BUFFER_PART_INFO_YOFFSET			= 8,	//!< vertical offset of buffer part data in px
	CX_BUFFER_PART_INFO_SOURCE_ID		= 10,	//!< buffer part source id
	CX_BUFFER_PART_INFO_REGION_ID		= 12,	//!< buffer part region id
	CX_BUFFER_PART_INFO_DATA_PURPOSE_ID = 13,	//!< buffer part purpose id \ref cx_buffer_part_purpose
	CX_BUFFER_PART_INFO_DELIVERED_HEIGHT = 14	//!< delivered buffer part data height (e.g. number of profiles captured)
};

/** Query information of acquisition buffer part.
	@param[in] hBuffer		handle to buffer
	@param[in] partIdx		index of image part if multipart payload, otherwise ignored, \ref CX_BUFFER_INFO_NUM_PARTS
	@param[in] param		Parameter info Id, \ref cx_buffer_part_info
	@param[out] val			pointer to parameter value
	@return CX_STATUS_OK on success, error value otherwise.
*/
CX_CAM_API cx_status_t __cdecl cx_getBufferPartInfo(CX_BUFFER_HANDLE hBuffer, int partIdx, int param, cx_variant_t* val);

//! @} cx_cam_device

//! @addtogroup cx_cam_device_param
//! @{

/** Set device parameter
   Allows the user to set camera and TL parameters. See also \ref cx_cam_device_param for common standard parameter names. 
   Please see GenICam SFNC and camera users manual for specific parameter names. The default nodelist is "Device". 
   To access a parameter in another nodelist, the parameter name must be prefixed with the name of the nodelist: 
   <CxCamLibParameterName> ::= [ <Nodelist>, "::",] <ParameterName> where 
   <Nodelist> ::= "DataStream" |"Device" | "DeviceTL" | "Interface"
   @param[in] hDevice	handle to device
   @param[in] prm		parameter name
   @param[in] val		pointer to value. Function automatically converts given variant data type into target parameter data type. If conversion is not possible CX_STATUS_INVALID_PARAMETER is returned.
   @return CX_STATUS_OK on success, error value otherwise.
 */
CX_CAM_API cx_status_t __cdecl cx_setParam(CX_DEVICE_HANDLE hDevice, const char* prm, const cx_variant_t* val);

/** Get device parameter
   Allows the user to query current camera and TL parameters. See also \ref cx_cam_device_param for common standard parameter names. 
   Please see GenICam SFNC and camera users manual for specific parameter names. The default nodelist is "Device". 
   To access a parameter in another nodelist, the parameter name must be prefixed with the name of the nodelist: 
   <CxCamLibParameterName> ::= [ <Nodelist>, "::",] <ParameterName> where 
   <Nodelist> ::= "DataStream" |"Device" | "DeviceTL" | "Interface"
   @param[in] hDevice			handle to device
   @param[in] prm				parameter name string.
   @param[out] val				pointer to variable for return.
   @return CX_STATUS_OK on success, error value otherwise.
*/
CX_CAM_API cx_status_t __cdecl cx_getParam(CX_DEVICE_HANDLE hDevice, const char* prm, cx_variant_t* val);

enum cx_param_info {
	CX_PARAM_INFO_TYPE				= 1,	//!< type of parameter, see \ref cx_param_type
	/** get array with range information, depending on parameter type the following range information are returned
		CX_PARAM_INTEGER 	returns min, max, increment in integer array
		CX_PARAM_FLOAT		returns min, max, increment or for multiplexer float the possible values as array
		CX_PARAM_STRING		returns maximum string length as single integer
		CX_PARAM_ENUM		returns integer array of possible enumeration values
	*/
	CX_PARAM_INFO_RANGE					= 2,	
	CX_PARAM_INFO_DESCRIPTION			= 3,	//!< string with description
	CX_PARAM_INFO_TOOLTIP				= 4,	//!< string with tooltip
	CX_PARAM_INFO_ACCESSS_MODE			= 5,	//!< returns integer with access mode of parameter, \ref cx_param_access_mode
	CX_PARAM_INFO_ENUM_SYMBOLS			= 6,	//!< returns null seperated string of possible enumeration strings
	CX_PARAM_INFO_REPRESENTATION		= 7,	//!< info about representation of parameter, \ref cx_param_representation
	CX_PARAM_INFO_UNIT					= 8,	//!< unit string
	CX_PARAM_INFO_FLOAT_NOTATION		= 9,	//!< returns integer with display notation for float parameter, \ref cx_param_float_notations
	CX_PARAM_INFO_FLOAT_PRECISION		= 10,	//!< return integer with display precision, only available for float parameters
	CX_PARAM_INFO_VISIBILITY			= 11,	//!< return recommended visibility of parameter, \ref cx_param_visibility
	CX_PARAM_INFO_CATEGORY_CHILDS		= 12,	//!< returns string array of child parameters
	CX_PARAM_INFO_DISPLAYNAME			= 13,	//!< string with display name
	CX_PARAM_INFO_ENUM_INT_VALUE		= 14	//!< returns enum selection as integer
};

/** Access mode of a parameter or file
	Defintion was adopted from GenICam Standard, see GenApi / Types.h.
*/
enum cx_param_access_mode {
	CX_PARAM_ACCESS_NOT_IMPLEMENTED	= 0,	//!< parameter node or file does not exist, is not implemented
	CX_PARAM_ACCESS_NOT_AVAILABLE	= 1,	//!< parameter node or file is not available (might be just temporary)
	CX_PARAM_ACCESS_WO				= 2,	//!< parameter node or file with write only access
	CX_PARAM_ACCESS_RO				= 3,	//!< parameter node or file with read access
	CX_PARAM_ACCESS_RW				= 4		//!< parameter node or file with write access
};

/** Recommended visibility of a parameter or file
	Defintion was adopted from GenICam Standard, see GenApi / Types.h.
*/
enum cx_param_visibility
{
	CX_PARAM_VISIBILITY_BEGINNER	= 0,	//!< parameter node or file is always visible
	CX_PARAM_VISIBILITY_EXPERT		= 1,    //!< Visible for experts or Gurus
	CX_PARAM_VISIBILITY_GURU		= 2,    //!< Visible for Gurus
	CX_PARAM_VISIBILITY_INVISIBLE	= 3,	//!< Not Visible
	CX_PARAM_VISIBILITY_UNDEFINED	= 99	//!< Object is not yet initialized
};

/** Gives a hint about how to display the parameter, only used for float and integer values. 
	Defintion was adopted from GenICam Standard, see GenApi/Types.h.
*/
enum cx_param_representation {
	CX_PARAM_REPRESENTATION_LINEAR		= 0,	//!< float, integer: Slider with linear behavior
	CX_PARAM_REPRESENTATION_LOGARITHMIC = 1,	//!< float, integer:Slider with logarithmic behaviour
	CX_PARAM_REPRESENTATION_BOOLEAN		= 2,	//!< integer: show as boolean Check box
	CX_PARAM_REPRESENTATION_PURENUMBER	= 3,	//!< float, integer: use an edit box with decimal display
	CX_PARAM_REPRESENTATION_HEXNUMBER	= 4,	//!< integer: use an edit box with hexadecimal display
	CX_PARAM_REPRESENTATION_IPV4ADDRESS = 5,	//!< integer: show as IP4 address
	CX_PARAM_REPRESENTATION_MACADDRESS	= 6,	//!< integer: show as MAC address
	CX_PARAM_REPRESENTATION_UNDEFINED	= 7,	//!< no specific representation was defined for this parameter
};

/** Gives a hint about how to display a float parameter. 
	Defintion was adopted from GenICam Standard, see GenApi / Types.h.
*/
enum cx_param_float_notations {
	CX_PARAM_FLOAT_NOTATION_AUTOMATIC	= 0,	//!< scientific or fixed notation is automatically choosen
	CX_PARAM_FLOAT_NOTATION_FIXED		= 1,	//!< use fixed floating point notation for display
	CX_PARAM_FLOAT_NOTATION_SCIENTIFIC	= 2		//!< use scientific floating point notation for display
};

/** Get device parameter information
	Allows the user to query additional parameter information, see \ref cx_param_info.
	See also \ref cx_cam_device_param for common standard parameter names. Please see GenICam SFNC and camera users manual for specific parameter names.
	The default nodelist is "Device". To access a parameter in another nodelist, the parameter name must be prefixed with the name of the nodelist:
    <CxCamLibParameterName> ::= [ <Nodelist>, "::",] <ParameterName> where
    <Nodelist> ::= "DataStream" |"Device" | "DeviceTL" | "Interface"
   @param[in] hDevice	handle to device
   @param[in] infoType	type of information to query, \ref cx_param_info
   @param[in] prm		parameter name
   @param[out] val		pointer to variable for return
   @return CX_STATUS_OK on success, error value otherwise.
*/
CX_CAM_API cx_status_t __cdecl cx_getParamInfo(CX_DEVICE_HANDLE hDevice, int infoType, const char* prm, cx_variant_t* val);

/**	Upload local file to camera. 
	For GenICam compatible devices GenICam FileAccess is used.
	See \ref cx_cam_device_files for standard device file names. Please see camera users manual for supported file names.
   @param[in] hDevice		handle to device
   @param[in] srcFilePath	file path of local file to upload.
   @param[in] deviceDst		describes destination and usage in camera, xpath style is allowed. E.g. UserSet/0, UserSet/Factory
   @return CX_STATUS_OK on success, error value otherwise.
*/
CX_CAM_API cx_status_t __cdecl cx_uploadFile(CX_DEVICE_HANDLE hDevice, const char* srcFilePath, const char* deviceDst);

/**	Upload buffer to camera file. 
	For GenICam compatible devices GenICam FileAccess is used. 
	See \ref cx_cam_device_files for standard device file names. Please see camera users manual for supported file names.
	@param[in] hDevice		handle to device
	@param[in] srcBuf		data buffer to upload.
	@param[in] length		length of data to upload.
	@param[in] deviceDst	describes destination and usage in camera, xpath style is allowed. E.g. UserSet/0, UserSet/Factory
	@return CX_STATUS_OK on success, error value otherwise.
*/
CX_CAM_API cx_status_t __cdecl cx_uploadFileFromBuffer(CX_DEVICE_HANDLE hDevice, const uint8_t* srcBuf, size_t length, const char* deviceDst);

/**	Download file from camera to local file. 
	For GenICam compatible devices GenICam FileAccess is used.
	See \ref cx_cam_device_files for standard device file names. Please see camera users manual for supported file names.
	@param[in] hDevice		handle to device
	@param[in] deviceSrc	describes camera source to download, xpath style is allowed, e.g. UserSet/0, UserSet/Factory.
	@param[in] dstfilePath	local file path.
	@return CX_STATUS_OK on success, error value otherwise.
*/
CX_CAM_API cx_status_t __cdecl cx_downloadFile(CX_DEVICE_HANDLE hDevice, const char* deviceSrc, const char* dstfilePath);

/**	Download file from camera to buffer. 
	For GenICam compatible devices GenICam FileAccess is used.
	See \ref cx_cam_device_files for standard device file names. Please see camera users manual for supported file names.
	@param[in] hDevice		handle to device
	@param[in] deviceSrc	file name on camera, xpath style is allowed, e.g. UserSet/0, UserSet/Factory.
	@param[inout] dstBuf	buffer to fill with downloaded file data.
	@param[inout] length	on input length of buffer, on output length of bytes copied.
	@return CX_STATUS_OK on success, error value otherwise.
*/
CX_CAM_API cx_status_t __cdecl cx_downloadFileToBuffer(CX_DEVICE_HANDLE hDevice, const char* deviceSrc, uint8_t* dstBuf, size_t* length);

enum cx_file_info
{
	CX_FILE_INFO_ACCESS_MODE	= 1,	//!< returns integer with access mode flags of file, \ref cx_param_access_mode
	CX_FILE_INFO_VISIBILITY		= 2,	//!< return recommended visibility of file, \ref cx_param_visibility
	CX_FILE_INFO_SIZE			= 3,	//!< returns file size if file exists otherwise zero
	CX_FILE_INFO_MAX_SIZE		= 4		//!< returns the maximum possible file size for the given deviceFile
};

/** Query information about device file
	@param[in] hDevice		handle to device
	@param[in] infoType		type of file information to query, \ref cx_file_info
	@param[in] deviceFile	file name on camera, xpath style is allowed, e.g. UserSet/0, UserSet/Factory.
	@param[out] val			pointer to variable for return
	@return CX_STATUS_OK on success, error value otherwise.
*/
CX_CAM_API cx_status_t __cdecl cx_getFileInfo(CX_DEVICE_HANDLE hDevice, int infoType, const char* deviceFile, cx_variant_t* val);

/**	GigEVision set register.
   Use CVP protocol WRITE_REGISTER command.
   Caller must take care about the byte order of data.
   @param[in] hDevice		handle to device
   @param[in] regAddress	
   @param[in] regValue
   @return CX_STATUS_OK on success, error value otherwise.
 */
CX_CAM_API cx_status_t __cdecl cx_setRegister(CX_DEVICE_HANDLE hDevice, uint32_t regAddress, uint32_t regValue);

/** GigEVision set multiple registers at once.
   Use CVP protocol WRITE_REGISTERS command.
   Caller must take care about the byte order of data.
   @param[in] hDevice		handle to device
   @param[in] count			register count
   @param[in] regAddresses	register address array
   @param[in] regValues		register value array
   @param[in] errIndex		errIndex array
   @return CX_STATUS_OK on success, error value otherwise.
 */
CX_CAM_API cx_status_t __cdecl cx_setRegisterMulti(CX_DEVICE_HANDLE hDevice, uint16_t count, uint32_t *regAddresses, uint32_t *regValues, uint16_t *errIndex);

/**	GigEVision get register.
   Use CVP protocol READ_REGISTER command.
   Caller must take care about the byte order of data.
   @param[in] hDevice					handle to device
   @param[in] regAddress
   @param[out] regValueOut
   @return CX_STATUS_OK on success, error value otherwise.
 */
CX_CAM_API cx_status_t __cdecl cx_getRegister(CX_DEVICE_HANDLE hDevice, uint32_t regAddress, uint32_t *regValueOut);

/**	 GigEVision get multiple registers at once.
   Use CVP protocol READ_REGISTERS command.
   Caller must take care about the byte order of data.
   @param[in] hDevice					handle to device
   @param[in] count						register count
   @param[out] regAddressesOut			register address array
   @param[out] regValuesOut				register value array
   @param[out] errIndex					errIndex array
   @return CX_STATUS_OK on success, error value otherwise.
 */
CX_CAM_API cx_status_t __cdecl cx_getRegisterMulti(CX_DEVICE_HANDLE hDevice, uint16_t count, uint32_t *regAddressesOut, uint32_t *regValuesOut, uint16_t *errIndex);

/**	GigEVision get memory.
   Use CVP protocol READ_MEMORY command.
   If the bufferlength is 4 Byte then the READ_REGISTER command is used instead.
   Caller must take care about the byte order of data.
   @param[in] hDevice					handle to device
   @param[in] startRegAddress
   @param[in] length
   @param[out] dstBuf
   @return CX_STATUS_OK on success, error value otherwise.
 */
CX_CAM_API cx_status_t __cdecl cx_getMemory(CX_DEVICE_HANDLE hDevice, uint32_t startRegAddress, uint8_t* dstBuf, uint32_t length);

/**	GigEVision set memory.
   Use CVP protocol WRITE_MEMORY command.
   If the bufferlength is 4 Byte then the WRITE_REGISTER command is used instead.
   Caller must take care about the byte order of data.
   @param[in] hDevice					handle to device
   @param[in] startRegAddress
   @param[in] length
   @param[in] srcBuf
   @return CX_STATUS_OK on success, error value otherwise.
 */
CX_CAM_API cx_status_t __cdecl cx_setMemory(CX_DEVICE_HANDLE hDevice, uint32_t startRegAddress, const uint8_t* srcBuf, uint32_t length);

/** Register a camera event callback
	The following event types are supported:
	- Device status change events, see \ref cx_cam_device_events.
	- GenICam Parameter change event. To register on parameter change events, call registerEvent with the parameter name
	- GenICam events. GenICam event name following the naming convention published in GenICam SFNC 2.3 Event Control.
	- To register on GEV Events without using GenICam events it is possible to call registerEvent with the event id (hexadecimal eventID as string without "0x").
	
	\note
	Function creates and returns handle to registered event. To free resources cx_unregisterEvent must be called. 

	@param[in] hDevice		handle to device
	@param[in] name			event name, see \ref cx_cam_device_events, 
	@param[in] cb			pointer to callback function
	@param[in] userParam	pointer to user data
	@param[out] phEvent		pointer to event handle 
	@return CX_STATUS_OK on success, error value otherwise.
*/
CX_CAM_API cx_status_t __cdecl cx_registerEvent(CX_DEVICE_HANDLE hDevice, const char* name, cx_event_cb cb, void* userParam, CX_EVENT_HANDLE* phEvent);

/** Unregister a camera event callback

	@param[in] hDevice	handle to device
	@param[in] hEvent	handle to event
	@return CX_STATUS_OK on success, error value otherwise.
*/
CX_CAM_API cx_status_t __cdecl cx_unregisterEvent(CX_DEVICE_HANDLE hDevice, CX_EVENT_HANDLE hEvent);

/** Get additional event data.
	See \ref cx_cam_event_data for standard event data names. Please also see camera users manual for specific event data names.
	
	\note
	The hEventData is only valid inside event handler. Call this function inside your event handler only!
	
	@param[in] hDevice		handle to device
	@param[in] hEventData	handle to event data
	@param[in] name			event data name to query, see \ref cx_cam_event_data
	@param[out] val			event data
	@return CX_STATUS_OK on success, error value otherwise.
*/
CX_CAM_API cx_status_t __cdecl cx_getEventData(CX_DEVICE_HANDLE hDevice, CX_EVENT_DATA_HANDLE hEventData, const char* name, cx_variant_t* val);

//! @} cx_cam_device_param

/*************************************************************************************/
//! @addtogroup cx_cam_uri
//! @{
//!		Uniform Resource Identifier URI
//!		In information technology, a Uniform Resource Identifier (URI) is a string of characters used to identify a resource. Such identification enables 
//!		interaction with representations of the resource over a network, typically the World Wide Web, using specific protocols. Schemes specifying a concrete
//!		syntax and associated protocols define each URI. The most common form of URI is the Uniform Resource Locator (URL), frequently referred to informally 
//!		as a web address. More rarely seen in usage is the Uniform Resource Name (URN), which was designed to complement URLs by providing a mechanism for the 
//!		identification of resources in particular namespaces. 
//!
//!		All URLs follow a common scheme using the format 
//!		Scheme : // Authority [/] [? query-string ] [ #Fragment]
//!		
//!		\see https://en.wikipedia.org/wiki/Uniform_Resource_Identifier
//!		\see https://en.wikipedia.org/wiki/Uniform_Resource_Locator
//!		\see https://pocoproject.org/slides/160-URIandUUID.pdf
//!
//!		Scheme is a abbreviation used to identify the protocol to use. 
//!		Authority consist of user information, host name and port number and defines the unique location identifier compatible with the protocol domain. 
//!		query-string is an optional part of the URL which allows the user to pass some extra configuration options in special cases. The format is key=value. Multiple queries can be concatenated using the & char.
//!		Fragment defines the fragment part.
//!
//!		Parts inside the square braces [ ] are optional and can be omitted while still holding a valid URL.
//!	
//!		**************************************************************************************************
//!		Interface URI
//!		**************************************************************************************************
//!
//!		Type	   	|	Scheme		|			Authority			|				Remark
//!		------------|---------------|-------------------------------|---------------------------------------------
//!		Ethernet   	|	eth			|	Ethernet address			|	e.g. eth://AB-12-E2-B1-AA-FF
//!		CamLink		|	cl			|	CL0...CL255					|				-
//!		Serial		|	uart		|	COM1...COM255				|				-
//!		USB			|   usb			|			-					|				-
//!		Firewire	|	ieee1394	|			-					|				-
//!		FlashLink	|	flashlink	|	ID0...ID8					|				-
//!	
//!		Query string
//!
//!		Key				|	Scheme				|			Value						|			Remark
//!		----------------|-----------------------|---------------------------------------|----------------------------
//!		forceip			|	gev, flir			|	(ip mask gateway)					|	Force a Device IP e.g. ?forceip=(192.168.1.1 255.255.0.0 192.168.2.100)
//!     nif				|	gev					|	adapter ethernet address			|	Specify the adapter the camera is connected to e.g. ?nif=AA-12-DE-22-BA-EF
//!		nif				|	clser				|	CL0...CL255							|	Select a Camera Link interface e.g. ?nif=CL0
//!		lib				|	gev					|	library path						|	Specify the library to use e.g. ?lib=GEVTL.cti
//!		xml				|	gev					|	camera xml file path				|	Specify the camera xml file to use e.g. ?xml=irsx_i_1435.xml
//!		fd				|	gev					|	true or false						|	Enables or disables the use of an available filter driver. If not specified the default behavior is using the filter driver
//!		
//!		**************************************************************************************************
//!		Device URI
//!		**************************************************************************************************
//!
//!		Type	   				|	Scheme		|			Authority			|				Remark
//!		------------------------|---------------|-------------------------------|----------------------------------
//!		GEV						|	gev			|	Ethernet address			|	e.g. gev://00-A1-34-23-1B-FF
//!		GEV						|	gev			|	Ipv4 ethernet address		|	e.g. gev://192.168.1.1
//!		GEV						|	gev			|	Device Enumeration Index	|	e.g. gev://0
//!		CamLink Serial			|	clser		|	CL0...CL255					|				-
//!		FLIR Resource Protocol	|	rtree		|	Ipv4 ethernet address		|	e.g. rtree://192.168.1.1
//!		Modbus TCP				|	modbus_tcp	|	Ipv4 ethernet address		|	e.g. modbus_tcp://192.168.1.1
//!		RTSP					|	rtsp		|	Ipv4 ethernet address		|	e.g. rtsp://192.168.1.1
//!	
//!		Query string
//!
//!		Key				|	Scheme				|			Value						|			Remark
//!		----------------|-----------------------|---------------------------------------|----------------------------
//!		portsettings	|	uart				|	(Speed DataBits Parity Stop bits)	|	?portsettings=(38400 8N1)
//!
//!		Multiple Keys can be concatenated using '&'	e.g. ?nif=AA-12-DE-22-BA-EF&lib=GEVTL.cti
//!	
//!		Examples
//!
//!		URI																		|				Remark
//!		------------------------------------------------------------------------|----------------------------------
//!		gev://0?nif=00-1B-21-D4-91-90&fd=false&lib=mvGenTLProducer.cti			|	Connects the first (index 0) device connected to nic with mac 00-1B-21-D4-91-90 over stack driver using the transport layer library mvGenTLProducer.cti
//!		gev://192.168.1.2?mac=10-1B-22-D4-91-96&nif=00-1B-21-D4-91-90			|	Connects the device with ip 192.168.1.3 connected to nic with mac 00-1B-21-D4-91-90 using the default library (Stemmer Imaging GEVTL.cti). Some Libraries require additional mac address of the device
//!		gev://10-1B-22-D4-91-96?nif=00-1B-21-D4-91-90							|	Connects the device with mac 10-1B-22-D4-91-96 connected to nic with mac 00-1B-21-D4-91-90 using the default library (Stemmer Imaging GEVTL.cti).
//!
//!		Please note that the corresponding enviroment variables holding path information to the Tranport Layer libraries must be set. x86 systems require the GENICAM_GENTL32_PATH enviroment variable and x64 systems using GENICAM_GENTL64_PATH enviroment.
//!
//!		**************************************************************************************************
//!		Service URI
//!		**************************************************************************************************
//!
//!		Type	   				|	Scheme		|			Authority			|				Remark
//!		------------------------|---------------|-------------------------------|----------------------------------
//!		Generic					|	generic		|				-				|				-		
//!		GEV						|	gev			|	Ethernet address			|	e.g. gev://00-A1-34-23-1B-FF
//!		ONWIF					|	onvif		|	Ipv4 ethernet address		|	e.g. onvif://192.168.1.1
//!		FLIR A Series			|	flir_ax		|	Ipv4 ethernet address		|	e.g. flir_ax://192.168.1.1
//!		MODBUS TCP				|	modbus_tcp	|	Ipv4 ethernet address		|	e.g. modbus_tcp://192.168.1.1
//!
//!		**************************************************************************************************
//!		Filter URI
//!		**************************************************************************************************
//!
//!		Type	   				|	Scheme		|			Authority			|				Remark
//!		------------------------|---------------|-------------------------------|----------------------------------
//!		Generic					|	filter		|	Ipv4 ethernet address		|	e.g. filter://192.168.1.2<sup>1</sup>
//!
//!		<sup>1</sup> If the ip address is an unicast address it searches for a camera with given ip using unicast discovery
//!		There is no need to provide a mask=255.255.255.255 query parameter
//!
//!		Query string
//!
//!		Key		|	Scheme	|			Value		|			Remark
//!		--------|-----------|-----------------------|------------------------------------------------------------------------------------
//!		mask	|	filter	|	Ipv4 Networkmask	|	?mask=255.255.0.0 searches for cameras within the given network address range
//!		man		|	filter	|	Manufactorer		|	?man=FLIR SYSTEMS searches all devices where the manufactorer is equal the given one
//!		model	|	filter	|	Model				|	?model=A645 searches all devices where the model is equal the given one
//!		sn		|	filter	|	Serial Number		|	?sn=12345 searches all devices where the serial number is equal the given one
//!		
//! @} cx_cam_uri

#ifdef __cplusplus
}
#endif

#endif /* CX_CAM_H_INCLUDED */
