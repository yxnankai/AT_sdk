/**
@file : DeviceFactory.h
@package : cx_cam library
@brief C++ DeviceFactory class
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
#ifndef AT_CX_DEVICEFACTORY_H_INCLUDED
#define AT_CX_DEVICEFACTORY_H_INCLUDED

#include <string>

#include "AT/cx/base.h"
#include "cx_cam.h"
#include "cx_cam_param.h"
#include "AT/cx/DeviceInfo.h"
#include "AT/cx/Device.h"

namespace AT {
	namespace cx {
		//! @addtogroup cx_wrapper_cpp
		//! @{
		class DeviceFactory
		{
		public:
			static DeviceInfoList findDevices(const std::string& uriFilter = std::string(""), unsigned int waitTime = 200, unsigned int flags = CX_DD_USE_GEV | CX_DD_USE_GEV_BROADCAST | CX_DD_USE_MDNS)
			{
				cx::Variant val;
				cx::checkOk("cx_dd_findDevices", cx_dd_findDevices(uriFilter.c_str(), waitTime, flags));
				unsigned int numDevices = getNumFoundDevices();
				DeviceInfoList devInfos;
				for (unsigned i = 0; i < numDevices; i++)
				{
					DeviceInfoPtr devInfo = std::make_shared<DeviceInfo>();
					if (cx_dd_getParam(i, CX_CAM_DD_DEVICE_STATUS, val) == CX_STATUS_OK) devInfo->deviceStatus = (int)val;
					if (cx_dd_getParam(i, CX_CAM_DD_DEVICE_URI, val) == CX_STATUS_OK) devInfo->deviceURI = (std::string)val;
					if (cx_dd_getParam(i, CX_CAM_DD_DEVICE_SUPPORTED_IPCFG, val) == CX_STATUS_OK) devInfo->deviceIpConfigSupported = (unsigned)val;
					if (cx_dd_getParam(i, CX_CAM_DD_DEVICE_CURRENT_IPCFG, val) == CX_STATUS_OK) devInfo->deviceIpConfig = (unsigned)val;
					if (cx_dd_getParam(i, CX_CAM_DD_DEVICE_MODEL, val) == CX_STATUS_OK) devInfo->deviceModel = (std::string)val;
					if (cx_dd_getParam(i, CX_CAM_DD_DEVICE_SERIAL, val) == CX_STATUS_OK) devInfo->deviceSerialNumber = (std::string)val;
					if (cx_dd_getParam(i, CX_CAM_DD_DEVICE_MAC, val) == CX_STATUS_OK) devInfo->deviceMAC = (std::string)val;
					if (cx_dd_getParam(i, CX_CAM_DD_DEVICE_IP, val) == CX_STATUS_OK) devInfo->deviceIP = (std::string)val;
					if (cx_dd_getParam(i, CX_CAM_DD_DEVICE_NM, val) == CX_STATUS_OK) devInfo->deviceNM = (std::string)val;
					if (cx_dd_getParam(i, CX_CAM_DD_DEVICE_GW, val) == CX_STATUS_OK) devInfo->deviceGW = (std::string)val;
					if (cx_dd_getParam(i, CX_CAM_DD_ADAPTER_ID, val) == CX_STATUS_OK) devInfo->adapterID = (std::string)val;
					if (cx_dd_getParam(i, CX_CAM_DD_PROTOCOLS, val) == CX_STATUS_OK) devInfo->protocols = (std::string)val;
					if (cx_dd_getParam(i, CX_CAM_DD_DEVICE_USER_ID, val) == CX_STATUS_OK) devInfo->deviceUserID = (std::string)val;
					if (cx_dd_getParam(i, CX_CAM_DD_DEVICE_VERSION, val) == CX_STATUS_OK) devInfo->deviceVersion = (std::string)val;
					if (cx_dd_getParam(i, CX_CAM_DD_DEVICE_FIRMWARE_VERSION, val) == CX_STATUS_OK) devInfo->deviceFirmwareVersion = (std::string)val;
					devInfos.push_back(devInfo);
				}
				return devInfos;
			}

			static unsigned int getNumFoundDevices()
			{
				unsigned int numDevices = 0;
				cx::checkOk("cx_dd_getNumFoundDevices", cx_dd_getNumFoundDevices(&numDevices));
				return numDevices;
			}

			static void setParam(unsigned int devIdx, const std::string& prm, const cx::Variant& val)
			{
				cx::checkOk("cx_dd_setParam", cx_dd_setParam(devIdx, prm.c_str(), val));
			}

			static void getParam(unsigned int devIdx, const std::string& prm, cx::Variant& val)
			{
				cx::checkOk("cx_dd_getParam", cx_dd_getParam(devIdx, prm.c_str(), val));
			}

			/** Get the connection status of a device
				see \ref cx_dd_device_status for status flags
			*/
			static unsigned int getDeviceStatus(unsigned int devIdx)
			{
				cx::Variant val;
				cx::DeviceFactory::getParam(devIdx, CX_CAM_DD_DEVICE_STATUS, val);
				return (unsigned int)val;
			}

			/** Force an ip reconfiguration cycle on the device.
			*/
			static void forceIpReconfigure(unsigned int devIdx)
			{
				cx::DeviceFactory::setParam(devIdx, CX_CAM_DD_FORCE_IP, cx::Variant("0.0.0.0 0.0.0.0 0.0.0.0"));
			}

			/** Set new IP via ForceIP 
			*/
			static void forceIp(unsigned int devIdx, const std::string& ip, const std::string& nm, const std::string& gw)
			{
				std::string param = ip + ' ' + nm + ' ' + gw;
				cx::DeviceFactory::setParam(devIdx, CX_CAM_DD_FORCE_IP, cx::Variant(param));
			}

			static DevicePtr createDevice()
			{
				return std::make_shared<Device>();
			}

			static DevicePtr openDevice(const std::string& uri, Device::open_mode openMode=Device::OPEN_EXCLUSIVE)
			{
				DevicePtr dev = createDevice();
				dev->open(uri, openMode);
				return dev;
			}

			static void closeAllDevices()
			{
				cx::checkOk("closeAllDevices", cx_closeAllDevices());
			}
		};

		//! @} cx_wrapper_cpp
	}	// namespace cx
}	// namespace AT
#endif	// AT_CX_DEVICEFACTORY_H_INCLUDED
