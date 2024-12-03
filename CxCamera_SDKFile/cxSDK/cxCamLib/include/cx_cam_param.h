//
// FileName : cx_cam_param.h
//
// Package : AT CX-Camera library
// Definition of common discovery and device parameters
// Names are compatible with GenICam SFNC
//
// Revisionhistory
//	27.11.2016, AT: initial version
//
// Copyright (c) 2016, Automation Technology GmbH.
//
#pragma once

#ifndef CX_CAM_PARAM_H_INCLUDED
#define CX_CAM_PARAM_H_INCLUDED


/* @addtogroup cx_cam_lic_std_params	Standard Parameter for license discovery
   @{
*/
#define CX_CAM_LIC_PUBLISHER				"Publisher"				//!< The publisher of the license (e.g. Stemmer Imaging AG,...)
#define CX_CAM_LIC_SERIAL					"Serial"				//!< Serial number or id of the license
#define CX_CAM_LIC_SPECIFIC					"Specific"				//!< String containing license specific info.

//! @} cx_cam_lic_std_params


/* @addtogroup cx_cam_dd_std_params	Standard Parameter for device discovery 
   @{
*/
#define CX_CAM_DD_DEVICE_URI				"uri"				//!< uniqe device identifier, can be used to open the device, see also \ref cx_cam_uri
#define CX_CAM_DD_DEVICE_SUPPORTED_IPCFG	"GevSupportedIPConfiguration"	//!< Supported IP Configuration and PAUSE schemes, see \ref cx_cam_ipcfg_flags
#define CX_CAM_DD_DEVICE_CURRENT_IPCFG		"GevCurrentIPConfiguration"		//!< Activated IP Configuration and PAUSE schemes, see \ref cx_cam_ipcfg_flags
#define CX_CAM_DD_DEVICE_MODEL				"Model"				//!< device model
#define CX_CAM_DD_DEVICE_VENDOR_NAME		"Manufacturer"		//!< device manufacturer name
#define CX_CAM_DD_DEVICE_MANUFACTURER_INFO	"ManufacturerInfo"	//!< manufacturer information about the device
#define CX_CAM_DD_DEVICE_SERIAL				"SN"				//!< device serial number
#define CX_CAM_DD_DEVICE_MAC				"MAC"				//!< device mac address, returned as string
#define CX_CAM_DD_DEVICE_IP					"IP"				//!< device ip address
#define CX_CAM_DD_DEVICE_NM					"Mask"				//!< device network mask
#define CX_CAM_DD_DEVICE_GW					"GW"				//!< device gateway address
#define CX_CAM_DD_ADAPTER_ID				"Adapter"			//!< adapter id the device is connected to. For network devices the MAC of the interface is returned.
#define CX_CAM_DD_PROTOCOLS					"Protocols"			//!< comma seperated list of supported protocols (uri list)
#define CX_CAM_DD_DEVICE_USER_ID			"UserID"			//!< a descriptive name that can be set by the user
#define CX_CAM_DD_DEVICE_VERSION			"Version"			//!< device version string
#define CX_CAM_DD_DEVICE_GEV_VERSION		"GevVersion"		//!< RO device gev version major.minor as uint32_t 
#define CX_CAM_DD_DEVICE_STATUS				"Status"			//!< RO returns device connection status, see status flags \ref cx_dd_device_status
#define CX_CAM_DD_FORCE_IP					"ForceIP"			//!< WO, sends a force ip message to a device. Parameter is a string with ip mask gateway.
#define CX_CAM_DD_DEVICE_FIRMWARE_VERSION	"FirmwareVersion"	//!< RO string max length 32, only available when the device allows RO access. This is a CX specific feature and is not available on other cameras.

/** GEV ipcfg flags
	All values are given in host order.
	
	\note 
	Use htonl before sending these flags via GEV function cx_setRegister.
*/
enum cx_cam_ipcfg_flags {
	CX_CAM_IPCFG_PERSISTENT_IP = 1,	//!< Persistent IP is supported / activated
	CX_CAM_IPCFG_DHCP = 2,			//!< DHCP is supported / activated
	CX_CAM_IPCFG_LLA = 4,			//!< Link-local address is supported /activated. Always rnabled for GEV device.
	CX_CAM_IPCFG_PG = 0x40000000,	//!< PAUSE_reception (PR) Set to 1 if the network interface can handle PAUSE frames.
	CX_CAM_IPCFG_PR = 0x80000000	//!< PAUSE_generation (PG) Set to 1 if the network interface can generate PAUSE frames.
};
//! @} cx_cam_dd_std_params

/** @addtogroup cx_cam_device_std_params Standard Camera Parameter
    @{
*/
#define CX_CAM_DEVICE_SERIAL			"DeviceSerialNumber"					//!< Parameter type: String. Device's serial number (former "DeviceID"). Adapted from GenICam SFNC (since V2.0).
#define CX_CAM_DEVICE_VENDOR			"DeviceVendorName"						//!< Parameter type: String. Name of the manufacturer of the device. Adapted from GenICam SFNC.
#define CX_CAM_DEVICE_MANUFACTURER_INFO	"DeviceManufacturerInfo"				//!< RO String. Manufacturer information about the device.
#define CX_CAM_DEVICE_MODEL				"DeviceModelName"						//!< Parameter type: String. Model of the device. Adapted from GenICam SFNC.
#define CX_CAM_DEVICE_CAPABILITY		"DeviceCapability"						//!< Parameter type: Integer. Device capability flags. AT-proprietary extension to GenICam SFNC.
#define CX_CAM_DEVICE_VERSION			"DeviceVersion"							//!< Parameter type: String. Device hardware version.
#define CX_CAM_DEVICE_FIRMWARE_VERSION	"DeviceFirmwareVersion"					//!< Parameter type: String. Device firmware version.
#define CX_CAM_DEVICE_USER_ID			"DeviceUserID"							//!< Parameter type: String. A descriptive name that can be set by the user
#define CX_CAM_DEVICE_SUPPORTED_CFG_LLA	"GevSupportedIPConfigurationLLA"		//!< RO Boolean, device supports LLA
#define CX_CAM_DEVICE_SUPPORTED_CFG_DHCP "GevSupportedIPConfigurationDHCP"		//!< RO Boolean, device support DHCP
#define CX_CAM_DEVICE_SUPPORTED_CFG_PERSISTENT	"GevSupportedIPConfigurationPersistentIP"	//!< RO Boolean, device support persistent IP
#define CX_CAM_DEVICE_IP_CFG_STATUS		"GevIPConfigurationStatus"				//!< RO enum, current IP configuration status: None, PersistentIP, DHCP, LLA or ForceIP
#define CX_CAM_DEVICE_PERSISTENT_IP		"GevPersistentIPAddress"				//!< RW Integer, Controls the Persistent IP address. It is only used when the device boots with the Persistent IP configuration scheme.
#define CX_CAM_DEVICE_PERSISTENT_NM		"GevPersistentSubnetMask"				//!< RW Integer, Controls the Persistent IP configuration. It is only used when the device boots with the Persistent IP configuration scheme.
#define CX_CAM_DEVICE_PERSISTENT_GW		"GevPersistentDefaultGateway"			//!< RW Integer, Controls the Persistent IP configuration. It is only used when the device boots with the Persistent IP configuration scheme.
#define CX_CAM_DEVICE_CURRENT_IP		"GevCurrentIPAddress"					//!< RO Integer current IP address
#define CX_CAM_DEVICE_CURRENT_NM		"GevCurrentSubnetMask"					//!< RO Integer current subnet mask
#define CX_CAM_DEVICE_CURRENT_GW		"GevCurrentDefaultGateway"				//!< RO Integer current default gateway
#define CX_CAM_DEVICE_CFG_LLA			"GevCurrentIPConfigurationLLA"			//!< RW Boolean, Controls whether the LLA IP configuration scheme is activated.
#define CX_CAM_DEVICE_CFG_DHCP			"GevCurrentIPConfigurationDHCP"			//!< RW Boolean, Controls whether the DHCP IP configuration scheme is activated.
#define CX_CAM_DEVICE_CFG_PERSISTENT	"GevCurrentIPConfigurationPersistentIP"	//!< RW Boolean, Controls whether the PersistentIP configuration scheme is activated.
#define CX_CAM_DEVICE_RESET				"DeviceReset"							//!< Parameter type: Command. Resets the device to its power up state. Adapted from GenICam SFNC.
#define CX_CAM_IMAGE_WIDTH				"Width"									//!< Parameter type: Integer. Width of the image provided by the device (in pixels). Adapted from GenICam SFNC.
#define CX_CAM_IMAGE_HEIGHT				"Height"								//!< Parameter type: Integer. Height of the image provided by the device (in pixels). Adapted from GenICam SFNC.
#define CX_CAM_IMAGE_OFFSET_X			"OffsetX"								//!< Parameter type: Integer. Horizontal offset from the origin to the region of interest (in pixels).
#define CX_CAM_IMAGE_REVERSE_X			"ReverseX"								//!< Parameter type: Integer/Boolean. TRUE = YES = 1, FALSE = NO = 0. Flip horizontally the image sent by the device.
#define CX_CAM_IMAGE_REVERSE_Y			"ReverseY"								//!< Parameter type: Integer/Boolean. TRUE = YES = 1, FALSE = NO = 0. Flip vertically the image sent by the device.
#define CX_CAM_PIXEL_FORMAT				"PixelFormat"							//!< Parameter type: Enumeration. Format of the pixels provided by the device. Adapted from GenICam SFNC.
#define CX_CAM_PAYLOAD_SIZE				"PayloadSize"							//!< Parameter type: Integer. Provides the number of bytes transferred for each image or chunk on the stream channel. Adapted from GenICam SFNC.
//! @} cx_cam_device_std_params

//! @addtogroup cx_cam_device_gev_register Standard GEV-Camera Register
//! @{
enum cx_cam_gev_bootstrap_register
{
	CX_CAM_DEVICE_BS_NET_IF_CAP_0 = 0x0010,
	CX_CAM_DEVICE_BS_NET_IF_CFG_0 = 0x0014,
	CX_CAM_DEVICE_BS_CURRENT_IP_0 = 0x0024,
	CX_CAM_DEVICE_BS_CURRENT_SM_0 = 0x0034,
	CX_CAM_DEVICE_BS_CURRENT_GW_0 = 0x0044
};
//! @} cx_cam_device_gev_register

//! @addtogroup cx_cam_device_events	Standard Camera Events
//! @{
// internal
#define CX_CAM_EVENT_CONNECTION_LOST			"EventConnectionLost"				//!< connection lost
#define CX_CAM_EVENT_CONNECTION_RESTORED		"EventConnectionRestored"			//!< connection restored
// SFNC
#define CX_CAM_EVENT_ACQUISITION_START			"EventAcquisitionStart"				//!< SFNC acquisition start event
#define CX_CAM_EVENT_ACQUISITION_END			"EventAcquisitionEnd"				//!< SFNC acquisition end event
#define CX_CAM_EVENT_ACQUISITION_TRANSFER_START	"EventAcquisitionTransferStart"		//!< SFNC acquisition transfer start event
#define CX_CAM_EVENT_ACQUISITION_TRANSFER_END	"EventAcquisitionTransferEnd"		//!< SFNC acquisition transfer end event
// Custom Log Event
#define CX_CAM_EVENT_LOG_MESSAGE				"EventLogMessageGenTL"				//!< AT custom log message (0x9300)
//! @} cx_cam_device_events

//! @addtogroup cx_cam_event_data	Standard Camera Event Data
//! @{
#define CX_CAM_EVENT_DATA_EVENTID		"EventId"		//!< event id
#define CX_CAM_EVENT_DATA_TIMESTAMP		"Timestamp"		//!< event timestamp
//! @} cx_cam_event_data

//! @addtogroup cx_cam_log_event_data	Log Event Data
//! @{
#define CX_CAM_LOG_EVENT_DATA_LEVEL		"MessageLevel"		//!< The log level of the message. 0-Fine, 1-Info, 2-Warning, 3-Severe.
#define CX_CAM_LOG_EVENT_DATA_TEXT		"MessageText"		//!< The actual text of the message.
//! @} cx_cam_log_event_data

//! @addtogroup cx_cam_device_files	Standard Camera Files
//! @{
#define CX_CAM_FILE_FEATURE_BAG		"FeatureBag"	//!< special deviceSrc used to store the current configuration in a GenICam FeatureBag file format
#define CX_CAM_FILE_BPR_DATA		"BPRData"		//!< special deviceSrc used to store BadPixel Table.
//! @} cx_cam_device_files

#endif /* CX_CAM_PARAM_H_INCLUDED */
