/*! Helper functions for cx_CamLib C++ example

@file cx_cam_common.cpp
@copyright (c) 2018, Automation Technology GmbH.
@version 12.10.2018, AT: initial version
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
#ifndef CX_CAM_COMMON_H_INCLUDED
#define CX_CAM_COMMON_H_INCLUDED

#include "cx_cam.h"
#include "cx_cam_param.h"
#include "AT/cx/base.h"
#include "AT/cx/Device.h"
#include "AT/cx/DeviceFactory.h"
#include "AT/cx/DeviceBuffer.h"

namespace AT {
	namespace cx {

		//! @addtogroup cx_wrapper_cpp
		//! @{

		/** Discover and prints the available devices. Ask the user to choose the device to use for the demo.
			See \ref cx_dd_findDevices for details about device discovery.
			Funtion calls exit(-1) if user presses cancel and exit(-2) if no devices are found.
		*/
		DeviceInfoPtr discoverAndChooseDevice(bool useFilterDriver = false);

		/** Prints all available information about parameter to os.
		*/
		void printParamInfo(cx::DevicePtr dev, const std::string& param, std::ostream& os);

		/** Get symbolic name string for given TypeID
		*/
		std::string partTypeIDToStr(cx_buffer_part_type typeID);

		/** Get symbolic name string for given PurposeID
		*/
		std::string partPurposeIDToStr(cx_buffer_part_purpose purposeID);

		/** Get min and max value of given image
		*/
		void imageMinMax(double& minVal, double& maxVal, const cx::ImagePtr &image);

		//! @} cx_wrapper_cpp
	}	// namespace cx
}	// namespace AT
#endif // CX_CAM_COMMON_H_INCLUDED
