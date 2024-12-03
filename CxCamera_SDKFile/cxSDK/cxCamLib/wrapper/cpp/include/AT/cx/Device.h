/**
@file : Device.h
@package : cx_cam library
@brief C++ Device wrapper class
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
#ifndef AT_CX_DEVICE_H_INCLUDED
#define AT_CX_DEVICE_H_INCLUDED

#include <string>
#include <memory>

#include "AT/cx/base.h"
#include "cx_cam.h"
#include "cx_cam_param.h"
#include "AT/cx/DeviceInfo.h"
#include "AT/cx/DeviceBuffer.h"

namespace AT {
	namespace cx {
		class DeviceFactory;

		//! @addtogroup cx_wrapper_cpp
		//! @{

		/** Class Device
		*/
		class Device
		{
		public:
			typedef std::shared_ptr<Device> Ptr;

			Device() : m_hDevice(CX_INVALID_HANDLE) {}
			Device(CX_DEVICE_HANDLE h) : m_hDevice(h) {}	//!< don't instanciate Device directly, use DeviceFactory. Contructor must be public for std::make_shared<Device>

			~Device()
			{
				close();
			}

			bool isOpen() const
			{
				return (m_hDevice != CX_INVALID_HANDLE);
			}

			/** Device open mode.
				If the device does not support different access/open modes, OPEN_EXCLUSIVE is assumed.
			*/
			enum open_mode {
				OPEN_READONLY = 0, 			//!< allows read only access to the device when no application with exclusive access is linked with the device
				OPEN_EXCLUSIVE = 1, 		//!< Exclusive access to the device. No other application can read or write to the device.
				OPEN_CONTROL = 2,			//!< Application can read and write to the device. But other applications are allowed to read from the device.
			};

			void open(const std::string& uri, open_mode openMode = OPEN_EXCLUSIVE)
			{
				close();

				// OPEN_EXCLUSIVE is default when no access mode is given in uri
				std::string uri_access = uri;
				if (openMode == OPEN_READONLY)
					uri_access += "&access=readonly";
				else if (openMode == OPEN_CONTROL)
					uri_access += "&access=control";
				cx::checkOk("cx_openDevice", cx_openDevice(uri_access.c_str(), &m_hDevice));
			}

			void close()
			{
				if (isOpen())
				{
					cx_closeDevice(m_hDevice);
					m_hDevice = CX_INVALID_HANDLE;
				}
			}

			void setParam(const std::string& prm, const cx::Variant& val)
			{
				cx::checkOk("cx_setParam", cx_setParam(m_hDevice, prm.c_str(), val));
			}

			void getParam(const std::string& prm, cx::Variant& val)
			{
				cx::checkOk("cx_getParam", cx_getParam(m_hDevice, prm.c_str(), val));
			}

			void getParamInfo(cx_param_info infoType, const std::string& prm, cx::Variant& val)
			{
				cx::checkOk("cx_getParamInfo", cx_getParamInfo(m_hDevice, infoType, prm.c_str(), val));
			}

			void setRegister(uint32_t regAddress, uint32_t regValue)
			{
				cx::checkOk("cx_setRegister", cx_setRegister(m_hDevice, regAddress, regValue));
			}

			void getRegister(uint32_t regAddress, uint32_t& regValueOut)
			{
				cx::checkOk("cx_getRegister", cx_getRegister(m_hDevice, regAddress, &regValueOut));
			}

			void getMemory(uint32_t startRegAddress, uint8_t* dstBuf, uint32_t length)
			{
				cx::checkOk("cx_getMemory", cx_getMemory(m_hDevice, startRegAddress, dstBuf, length));
			}

			void setMemory(uint32_t startRegAddress, const uint8_t* srcBuf, uint32_t length)
			{
				cx::checkOk("cx_setMemory", cx_setMemory(m_hDevice, startRegAddress, srcBuf, length));
			}

			void uploadFile(const std::string& srcFilePath, const std::string& deviceDst)
			{
				cx::checkOk("cx_uploadFile", cx_uploadFile(m_hDevice, srcFilePath.c_str(), deviceDst.c_str()));
			}

			void uploadFileFromBuffer(const std::string& buffer, const std::string& deviceDst)
			{
				cx::checkOk("cx_uploadFileFromBuffer", cx_uploadFileFromBuffer(m_hDevice, (const uint8_t*)buffer.c_str(), buffer.size(), deviceDst.c_str()));
			}

			void downloadFile(const std::string& deviceSrc, const std::string& dstfilePath)
			{
				cx::checkOk("cx_downloadFile", cx_downloadFile(m_hDevice, deviceSrc.c_str(), dstfilePath.c_str()));
			}

			std::string downloadFileToBuffer(const std::string& deviceSrc)
			{
				cx::Variant val;
				getFileInfo(CX_FILE_INFO_SIZE, deviceSrc, val);
				size_t len = (unsigned)val;
				std::string buffer(len, 0);
				cx::checkOk("cx_downloadFileToBuffer", cx_downloadFileToBuffer(m_hDevice, deviceSrc.c_str(), (uint8_t*)buffer.data(), &len));
				buffer.resize(len);
				return buffer;
			}

			void getFileInfo(cx_file_info infoType, const std::string& deviceFile, cx::Variant& val)
			{
				cx::checkOk("cx_getFileInfo", cx_getFileInfo(m_hDevice, infoType, deviceFile.c_str(), val));
			}

			void allocAndQueueBuffers(int numBuffers=3)
			{
				cx::checkOk("cx_allocAndQueueBuffers", cx_allocAndQueueBuffers(m_hDevice, numBuffers));
			}

			void freeBuffers()
			{
				cx::checkOk("cx_freeBuffers", cx_freeBuffers(m_hDevice));
			}

			DeviceBuffer waitForBuffer(unsigned int timeout, bool noThrow=false)
			{
				CX_BUFFER_HANDLE hBuffer = CX_INVALID_HANDLE;
				cx_status_t status = cx_waitForBuffer(m_hDevice, &hBuffer, timeout);
				if(noThrow==false)
					cx::checkOk("cx_waitForBuffer", status);
				return DeviceBuffer(hBuffer);
			}

			void startAcquisition()
			{
				cx::checkOk("cx_startAcquisition", cx_startAcquisition(m_hDevice));
			}

			void stopAcquisition()
			{
				cx::checkOk("cx_stopAcquisition", cx_stopAcquisition(m_hDevice));
			}

			CX_EVENT_HANDLE registerEvent(const std::string& name, cx_event_cb cb, void* userParam)
			{
				CX_EVENT_HANDLE hEvent;
				cx::checkOk("cx_registerEvent", cx_registerEvent(m_hDevice, name.c_str(), cb, userParam, &hEvent));
				return hEvent;
			}

			void unregisterEvent(CX_EVENT_HANDLE e)
			{
				cx::checkOk("cx_unregisterEvent", cx_unregisterEvent(m_hDevice, e));
			}

			void getEventData(CX_EVENT_DATA_HANDLE e, const std::string& name, cx::Variant& val)
			{
				cx::checkOk("cx_getEventData", cx_getEventData(m_hDevice, e, name.c_str(), val));
			}

			CX_DEVICE_HANDLE getHandle() const { return m_hDevice; }

		private:
			CX_DEVICE_HANDLE m_hDevice;
		};

		typedef Device::Ptr DevicePtr;

		//! @} cx_wrapper_cpp
	}	// namespace cx
}	// namespace AT
#endif	// AT_CX_DEVICE_H_INCLUDED
