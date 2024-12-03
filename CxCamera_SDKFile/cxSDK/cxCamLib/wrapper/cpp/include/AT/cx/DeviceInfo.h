/**
@file : DeviceInfo.h
@package : cx_cam library
@brief C++ DeviceInfo class
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
#ifndef AT_CX_DEVICEINFO_H_INCLUDED
#define AT_CX_DEVICEINFO_H_INCLUDED

#include <string>
#include <regex>
#include <vector>
#include <memory>

namespace AT {
	namespace cx {
		//! @addtogroup cx_wrapper_cpp
		//! @{
		/** see cx_cam_param.h \ref cx_cam_dd for definition of available discovery parameters
		*/
		class DeviceInfo
		{
		public:
			DeviceInfo() : deviceStatus(0), deviceIpConfigSupported(0), deviceIpConfig(0) {}

			bool isCxModel() { return std::regex_search(deviceModel, std::regex("[cC][2456][-_]|[cC][xX][0-9]{4}")); }
			bool isIrsxModel() { return std::regex_search(deviceModel, std::regex("IRSX-[ITBX]")); }
			bool isIrsModel() { return std::regex_search(deviceModel, std::regex("IRS-[36]")); }

			unsigned deviceStatus;				//!< see status flags \ref cx_dd_device_status
			std::string deviceURI;				//!< uniqe device identifier, can be used to open the device, see also \ref cx_cam_uri
			uint32_t deviceIpConfigSupported;	//!< Supported IP Configuration and PAUSE schemes
			uint32_t deviceIpConfig;			//!< Activated (current) IP Configuration and PAUSE schemes
			std::string deviceModel;			//!< device model
			std::string deviceSerialNumber;		//!< device serial number
			std::string deviceUserID;			//!< a descriptive name that can be set by the user
			std::string deviceVersion;			//!< device version string
			std::string deviceFirmwareVersion;	//!< device firmware version string
			std::string deviceMAC;				//!< device mac address
			std::string deviceIP;				//!< current device ip address, can be changed with forceIP or persistentIP and reboot
			std::string deviceNM;				//!< current device network mask, can be changed with forceIP or persistentNM and reboot
			std::string deviceGW;				//!< current device gateway address, can be changed with forceIP or persistentGW and reboot
			std::string adapterID;				//!< adapter id the device is connected to
			std::string protocols;				//!< comma seperated list of supported protocols (uri list)
		};

		/** Compare two DeviceInfo's.
			returns true if both DeviceInfo's are equal.
		*/
		inline bool operator == (const AT::cx::DeviceInfo& a, const AT::cx::DeviceInfo& b)
		{
			return (a.deviceStatus == b.deviceStatus &&
				a.deviceURI == b.deviceURI &&
				a.deviceIpConfigSupported == b.deviceIpConfigSupported &&
				a.deviceIpConfig == b.deviceIpConfig &&
				a.deviceModel == b.deviceModel &&
				a.deviceSerialNumber == b.deviceSerialNumber &&
				a.deviceUserID == b.deviceUserID &&
				a.deviceVersion == b.deviceVersion &&
				a.deviceFirmwareVersion == b.deviceFirmwareVersion &&
				a.deviceMAC == b.deviceMAC &&
				a.deviceIP == b.deviceIP &&
				a.deviceNM == b.deviceNM &&
				a.deviceGW == b.deviceGW &&
				a.adapterID == b.adapterID &&
				a.protocols == b.protocols) ? true : false;
		}
		inline bool operator != (const AT::cx::DeviceInfo& a, const AT::cx::DeviceInfo& b)
		{
			return !(a == b);
		}

		typedef std::shared_ptr<DeviceInfo> DeviceInfoPtr;
		typedef std::vector<DeviceInfoPtr> DeviceInfoList;

		//! @} cx_wrapper_cpp
	}	// namespace cx
}	// namespace AT
#endif	// AT_CX_DEVICEINFO_H_INCLUDED
