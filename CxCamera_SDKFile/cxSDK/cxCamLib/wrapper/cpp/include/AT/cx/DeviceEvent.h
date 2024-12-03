/**
@file : DeviceEvent.h
@package : cx_cam library
@brief C++ DeviceEvent wrapper class
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
#ifndef AT_CX_DEVICEEVENT_H_INCLUDED
#define AT_CX_DEVICEEVENT_H_INCLUDED

#include <string>
#include "AT/cx/base.h"
#include "cx_cam.h"
#include "cx_cam_param.h"
#include "AT/cx/Device.h"

namespace AT {
	namespace cx {

		//! @addtogroup cx_wrapper_cpp
		//! @{

		/** DeviceEvent class wrapping the CX_EVENT_HANDLE.
			Registers the event on creation.
			\note Class will automatically unregister the event in its destructor.
		*/
		class DeviceEvent
		{
		public:
			typedef std::shared_ptr<DeviceEvent> Ptr;

			DeviceEvent(const DevicePtr& dev, const std::string& name, cx_event_cb cb, void* userParam = NULL) : m_dev(dev)
			{
				m_hEvent = m_dev->registerEvent(name, cb, userParam);
			}

			~DeviceEvent()
			{
				unregister();
			}

			void getData(CX_EVENT_DATA_HANDLE e, const std::string& name, cx::Variant& val)
			{
				if (m_dev && m_hEvent != CX_INVALID_HANDLE && e != CX_INVALID_HANDLE)
					m_dev->getEventData(e, name, val);
			}

			void unregister()
			{
				if (m_dev && m_hEvent != CX_INVALID_HANDLE)
					m_dev->unregisterEvent(m_hEvent);
			}

			static DeviceEvent::Ptr registerEvent(const DevicePtr& dev, const std::string& name, cx_event_cb cb, void* userParam = NULL)
			{
				return std::make_shared<DeviceEvent>(dev, name, cb, userParam);
			}

			CX_DEVICE_HANDLE getHandle() const { return m_hEvent; }

		private:
			CX_EVENT_HANDLE m_hEvent;
			DevicePtr m_dev;
		};

		typedef DeviceEvent::Ptr DeviceEventPtr;

		//! @} cx_wrapper_cpp
	}	// namespace cx
}	// namespace AT
#endif	// AT_CX_DEVICEEVENT_H_INCLUDED
