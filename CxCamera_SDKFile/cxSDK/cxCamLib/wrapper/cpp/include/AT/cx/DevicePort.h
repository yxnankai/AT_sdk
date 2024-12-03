/**
@file : DevicePort.h
@package : cx_cam library
@brief C++ DevicePort class can be used for linking cx-device with GenICam Nodemap
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
#ifndef AT_CX_DEVICEPORT_H_INCLUDED
#define AT_CX_DEVICEPORT_H_INCLUDED

#include "AT/cx/Device.h"
#include <GenICam.h>

namespace AT {
	namespace cx {
	
		/** DevicePort class can be used for linking cx-device with GenICam Nodemap.
			It implements necessary functions from IPort and IBase.

		\code{.cpp}
			auto camera = cx::DeviceFactory::openDevice(uri);
			cx::DevicePort devicePort(camera);
			CNodeMapFactory cameraNodeMapFactory(ContentType_Xml, xml.c_str(), m_xml.size());
			CNodeMapRef cameraNodeMap = cameraNodeMapFactory.CreateNodeMap();
			cameraNodeMap._Connect( &devicePort, "Device" );
		\endcode
		*/
		class DevicePort : public GENAPI_NAMESPACE::IPort
		{
		public:
			DevicePort(cx::DevicePtr dev)	{ m_dev = dev; }
			
			/** If the driver is open, return RW (= read/write), therwise NA (= not available)
				Function is defined in IBase.
			*/
			virtual GENAPI_NAMESPACE::EAccessMode GetAccessMode() const
			{
				return (m_dev->isOpen())? GENAPI_NAMESPACE::RW : GENAPI_NAMESPACE::NA;
			}
			
		    //! Reads a chunk of bytes from the port
			virtual void Read(void *pBuffer, int64_t address, int64_t length)
			{
				m_dev->getMemory((uint32_t)address, (uint8_t*)pBuffer, (uint32_t)length);
			}

			//! Writes a chunk of bytes to the port
			virtual void Write(const void *pBuffer, int64_t address, int64_t length)
			{
				m_dev->setMemory((uint32_t)address, (uint8_t*)pBuffer, (uint32_t)length);
			}
		
		protected:
			cx::DevicePtr m_dev;
		};

	}	// namespace cx
}		// namespace AT
#endif	// AT_CX_DEVICEPORT_H_INCLUDED
	