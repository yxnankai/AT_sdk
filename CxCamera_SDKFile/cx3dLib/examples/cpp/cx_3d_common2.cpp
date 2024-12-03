/**
@file : cx_3d_common2.cpp
@package : cx_3d library
@brief CPP common example functionality. This file adds functions with dependency on cx_cam library.
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

#include <assert.h>
#include <stdio.h>
#include <string>
#include <iostream>

#include "cx_cam.h"
#include "cx_cam_param.h"
#include "cx_3d_common.h"

namespace AT {
	namespace cx {

		/** Load calibration from camera using a temporary file on host side.
			We first try to load the new XML calibration format from "CalibrationFactory".
			If this Filetype is not available we try to load the old binary format from "UserData"
		*/
		cx_status_t downloadCalib(CX_DEVICE_HANDLE hDevice, CX_CALIB_HANDLE& hCalib, const char* tempFileName)
		{
			cx_status_t status = cx_downloadFile(hDevice, "CalibrationFactory", tempFileName);	// make sure the file is locally writable
			if (status == CX_STATUS_OK)
				status = cx_3d_calib_load(tempFileName, "", CX_3D_CALIB_FORMAT_AUTO, &hCalib);	// load default calibration
			
			if (status != CX_STATUS_OK)
			{
				status = cx_downloadFile(hDevice, "UserData", tempFileName);	// make sure the file is locally writable
				if (status == CX_STATUS_OK)
					status = cx_3d_calib_load(tempFileName, "", CX_3D_CALIB_FORMAT_AUTO, &hCalib);	// load default calibration
			}
			return status;
		}

		/** Load calibration from camera using internal buffer.
			We first try to load the new XML calibration format from "CalibrationFactory".
			If this Filetype is not available we try to load the old binary format from "UserData"
		*/
		cx_status_t downloadCalib(CX_DEVICE_HANDLE hDevice, CX_CALIB_HANDLE& hCalib)
		{
			size_t bufferSz = 0;
			cx_status_t status = CX_STATUS_OK;
			cx::Variant val;
			if ((status = cx_getFileInfo(hDevice, CX_FILE_INFO_SIZE, "CalibrationFactory", val)) == CX_STATUS_OK)
			{
				val.get(bufferSz);
				uint8_t* buffer = (uint8_t*)malloc(bufferSz);
				status = cx_downloadFileToBuffer(hDevice, "CalibrationFactory", buffer, &bufferSz);
				if (status == CX_STATUS_OK)
					status = cx_3d_calib_loadFromBuffer(buffer, bufferSz, "", CX_3D_CALIB_FORMAT_AUTO, &hCalib);	// load default calibration
				free(buffer);
			}
			if (status != CX_STATUS_OK)
			{
				if ((status = cx_getFileInfo(hDevice, CX_FILE_INFO_SIZE, "UserData", val)) == CX_STATUS_OK)
				{
					val.get(bufferSz);
					uint8_t* buffer = (uint8_t*)malloc(bufferSz);
					status = cx_downloadFileToBuffer(hDevice, "UserData", buffer, &bufferSz);
					if (status == CX_STATUS_OK)
						status = cx_3d_calib_loadFromBuffer(buffer, bufferSz, "", CX_3D_CALIB_FORMAT_AUTO, &hCalib);	// load default calibration
					free(buffer);
				}
			}
			return status;
		}

		/** Update calibration settings with current acquisition settings from camera
			The following calibration parameters need to be updated with device settings: 
			CX_3D_PARAM_S_SZ, CX_3D_PARAM_S_RR_V/H, CX_3D_PARAM_RANGE_SCALE, CX_3D_PARAM_ROI_XXX.
		*/
		void updateCalibC5(CX_DEVICE_HANDLE hDevice, CX_CALIB_HANDLE hCalib, int aoiSel)
		{
			cx::Variant val;

			// set sensor size parameter from camera (requires for LUT_mode, binary calibration format does not include this parameter)
			std::vector<int64_t> s_size = { 0, 0 };
			cx::checkOk(cx_getParam(hDevice, "SensorWidth", val));
			s_size.at(0) = val;
			cx::checkOk(cx_getParam(hDevice, "SensorHeight", val));
			s_size.at(1) = val;
			val.set(s_size);
			cx::checkOk(cx_3d_calib_set(hCalib, CX_3D_PARAM_S_SZ, val));	//set sensor size in pixel [width, height]	

			// set horizontal resolution reduction caused by sensor binning or decimation
			unsigned rrh = 1;
			if (cx_getParamInfo(hDevice, CX_PARAM_INFO_ACCESSS_MODE, "BinningHorizontal", val) == CX_STATUS_OK)
			{
				if ((int64_t)val > CX_PARAM_ACCESS_WO)
				{
					if (cx_getParamInfo(hDevice, CX_PARAM_INFO_VISIBILITY, "BinningHorizontal", val) == CX_STATUS_OK)
					{
						if ((int64_t)val < CX_PARAM_VISIBILITY_INVISIBLE)
						{
							if (cx_getParam(hDevice, "BinningHorizontal", val) == CX_STATUS_OK)
							{
								if ((unsigned)val > 0)
									rrh *= (unsigned)val;
							}
						}
					}
				}
			}

			if (cx_getParamInfo(hDevice, CX_PARAM_INFO_ACCESSS_MODE, "DecimationHorizontal", val) == CX_STATUS_OK)
			{
				if ((int64_t)val > CX_PARAM_ACCESS_WO)
				{
					if (cx_getParamInfo(hDevice, CX_PARAM_INFO_VISIBILITY, "DecimationHorizontal", val) == CX_STATUS_OK)
					{
						if ((int64_t)val < CX_PARAM_VISIBILITY_INVISIBLE)
						{
							if (cx_getParam(hDevice, "DecimationHorizontal", val) == CX_STATUS_OK)
							{
								if ((unsigned)val > 0)
									rrh *= (unsigned)val;
							}
						}
					}
				}
			}
			val = rrh;
			cx::checkOk(cx_3d_calib_set(hCalib, CX_3D_PARAM_S_RR_H, val));

			// set vertical resolution reduction caused by sensor binning or decimation
			unsigned rrv = 1;
			if (cx_getParamInfo(hDevice, CX_PARAM_INFO_ACCESSS_MODE, "BinningVertical", val) == CX_STATUS_OK)
			{
				if ((int64_t)val > CX_PARAM_ACCESS_WO)
				{
					if (cx_getParamInfo(hDevice, CX_PARAM_INFO_VISIBILITY, "BinningVertical", val) == CX_STATUS_OK)
					{
						if ((int64_t)val < CX_PARAM_VISIBILITY_INVISIBLE)
						{
							if (cx_getParam(hDevice, "BinningVertical", val) == CX_STATUS_OK)
							{
								if((unsigned)val > 0)
									rrv *= (unsigned)val;
							}
						}
					}
				}
			}
			if (cx_getParamInfo(hDevice, CX_PARAM_INFO_ACCESSS_MODE, "DecimationVertical", val) == CX_STATUS_OK)
			{
				if ((int64_t)val > CX_PARAM_ACCESS_WO)
				{
					if (cx_getParamInfo(hDevice, CX_PARAM_INFO_VISIBILITY, "DecimationVertical", val) == CX_STATUS_OK)
					{
						if ((int64_t)val < CX_PARAM_VISIBILITY_INVISIBLE)
						{
							if (cx_getParam(hDevice, "DecimationVertical", val) == CX_STATUS_OK)
							{
								if ((unsigned)val > 0)
									rrv *= (unsigned)val;
							}
						}
					}
				}
			}
			val = rrv;
			cx::checkOk(cx_3d_calib_set(hCalib, CX_3D_PARAM_S_RR_V, val));

			cx::checkOk(cx_getParam(hDevice, "NumSubPixel", val));
			// set range scale depending on the numer of activate subpixel bits
			val = 1.0 / double(1 << (int)val);
			cx::checkOk(cx_3d_calib_set(hCalib, CX_3D_PARAM_RANGE_SCALE, val));

			// set X offset in image space
			cx::checkOk(cx_getParam(hDevice, CX_CAM_IMAGE_OFFSET_X, val));
			cx::checkOk(cx_3d_calib_set(hCalib, CX_3D_PARAM_S_ROI_X, val));

			cx::checkOk(cx_getParam(hDevice, "AoiSelector", val));
			// select the AOI on the camera we want to read the parameter for
			cx::checkOk(cx_setParam(hDevice, "AoiSelector", cx::Variant(aoiSel)));

			// set image-Y offset in calibration, this is needed if we have not absolut coordinates activated in camera, otherwise set to zero
			cx::checkOk(cx_getParam(hDevice, "AbsOffsetPos", val));
			bool absPos = ((int)val == 0) ? false : true;			// TRUE = YES = 1, FALSE = NO = 0
			if (absPos == false)
				cx::checkOk(cx_getParam(hDevice, "AoiOffsetY", val));
			else
				val = int(0);
			cx::checkOk(cx_3d_calib_set(hCalib, CX_3D_PARAM_S_ROI_Y, val));

			// set image width, if reverse-x is true set negative
			cx::checkOk(cx_getParam(hDevice, CX_CAM_IMAGE_REVERSE_X, val));
			bool reverseX = ((int)val == 0) ? false : true;			// TRUE = YES = 1, FALSE = NO = 0
			cx::checkOk(cx_getParam(hDevice, CX_CAM_IMAGE_WIDTH, val));
			int width = val;
			if (reverseX)
				width = -width;
			cx::checkOk(cx_3d_calib_set(hCalib, CX_3D_PARAM_S_ROI_W, cx::Variant(width)));

			// set AOI height, if reverse-y is true set negative
			cx::checkOk(cx_getParam(hDevice, "AoiHeight", val));
			
			/* ALT -----------------> */
			/* int roiHeight = val;   */
			/* <----------------- ALT */
			
			/* NEU -----------------> */
			int roiHeight = 0;
			if (absPos == false) 
				roiHeight = val;
			else 
				roiHeight = int (s_size.at(1)); // Sensor Height 
			/* <----------------- NEU */
			
			cx::checkOk(cx_getParam(hDevice, CX_CAM_IMAGE_REVERSE_Y, val));
			bool reverseY = ((int)val == 0) ? false : true;			// TRUE = YES = 1, FALSE = NO = 0
			if (reverseY)
				roiHeight = -roiHeight;
			cx::checkOk(cx_3d_calib_set(hCalib, CX_3D_PARAM_S_ROI_H, cx::Variant(roiHeight)));
		}

/** Update calibration settings with current acquisition settings from camera
	The following calibration parameters need to be updated with device settings:
	CX_3D_PARAM_S_SZ, CX_3D_PARAM_S_RR_V/H, CX_3D_PARAM_RANGE_SCALE, CX_3D_PARAM_ROI_XXX.
*/
		void updateCalibC6(CX_DEVICE_HANDLE hDevice, CX_CALIB_HANDLE hCalib, int regionId)
		{
			cx::Variant val;

			// set sensor size parameter from camera (requires for LUT_mode, binary calibration format does not include this parameter)
			std::vector<int64_t> s_size = { 0, 0 };
			cx::checkOk(cx_getParam(hDevice, "SensorWidth", val));
			s_size.at(0) = val;
			cx::checkOk(cx_getParam(hDevice, "SensorHeight", val));
			s_size.at(1) = val;
			val.set(s_size);
			cx::checkOk(cx_3d_calib_set(hCalib, CX_3D_PARAM_S_SZ, val));	//set sensor size in pixel [width, height]	

			// set horizontal resolution reduction caused by sensor binning or decimation
			unsigned rrh = 1;
			if (cx_getParamInfo(hDevice, CX_PARAM_INFO_ACCESSS_MODE, "BinningHorizontal", val) == CX_STATUS_OK)
			{
				if ((int64_t)val > CX_PARAM_ACCESS_WO)
				{
					if (cx_getParamInfo(hDevice, CX_PARAM_INFO_VISIBILITY, "BinningHorizontal", val) == CX_STATUS_OK)
					{
						if ((int64_t)val < CX_PARAM_VISIBILITY_INVISIBLE)
						{
							if (cx_getParam(hDevice, "BinningHorizontal", val) == CX_STATUS_OK)
							{
								if ((unsigned)val > 0)
									rrh *= (unsigned)val;
							}
						}
					}
				}
			}

			if (cx_getParamInfo(hDevice, CX_PARAM_INFO_ACCESSS_MODE, "DecimationHorizontal", val) == CX_STATUS_OK)
			{
				if ((int64_t)val > CX_PARAM_ACCESS_WO)
				{
					if (cx_getParamInfo(hDevice, CX_PARAM_INFO_VISIBILITY, "DecimationHorizontal", val) == CX_STATUS_OK)
					{
						if ((int64_t)val < CX_PARAM_VISIBILITY_INVISIBLE)
						{
							if (cx_getParam(hDevice, "DecimationHorizontal", val) == CX_STATUS_OK)
							{
								if ((unsigned)val > 0)
									rrh *= (unsigned)val;
							}
						}
					}
				}
			}
			val = rrh;
			cx::checkOk(cx_3d_calib_set(hCalib, CX_3D_PARAM_S_RR_H, val));

			// set vertical resolution reduction caused by sensor binning or decimation
			unsigned rrv = 1;
			if (cx_getParamInfo(hDevice, CX_PARAM_INFO_ACCESSS_MODE, "BinningVertical", val) == CX_STATUS_OK)
			{
				if ((int64_t)val > CX_PARAM_ACCESS_WO)
				{
					if (cx_getParamInfo(hDevice, CX_PARAM_INFO_VISIBILITY, "BinningVertical", val) == CX_STATUS_OK)
					{
						if ((int64_t)val < CX_PARAM_VISIBILITY_INVISIBLE)
						{
							if (cx_getParam(hDevice, "BinningVertical", val) == CX_STATUS_OK)
							{
								if ((unsigned)val > 0)
									rrv *= (unsigned)val;
							}
						}
					}
				}
			}
			if (cx_getParamInfo(hDevice, CX_PARAM_INFO_ACCESSS_MODE, "DecimationVertical", val) == CX_STATUS_OK)
			{
				if ((int64_t)val > CX_PARAM_ACCESS_WO)
				{
					if (cx_getParamInfo(hDevice, CX_PARAM_INFO_VISIBILITY, "DecimationVertical", val) == CX_STATUS_OK)
					{
						if ((int64_t)val < CX_PARAM_VISIBILITY_INVISIBLE)
						{
							if (cx_getParam(hDevice, "DecimationVertical", val) == CX_STATUS_OK)
							{
								if ((unsigned)val > 0)
									rrv *= (unsigned)val;
							}
						}
					}
				}
			}
			val = rrv;
			cx::checkOk(cx_3d_calib_set(hCalib, CX_3D_PARAM_S_RR_V, val));

			cx::checkOk(cx_getParam(hDevice, "Scan3dExtractionSelector", val));
			cx_getParamInfo(hDevice, CX_PARAM_INFO_RANGE, "Scan3dExtractionSelector", val);
			// seclect extraction module belonging to given AOI

			std::vector<int64_t> enumVals;
			cx::checkOk(val.get(enumVals));

			std::vector<int64_t>::iterator it;
			for (it = enumVals.begin(); it != enumVals.end(); it++)
			{
				val.set(*it);
				cx::checkOk(cx_setParam(hDevice, "Scan3dExtractionSelector", val));

				cx::checkOk(cx_getParamInfo(hDevice, CX_PARAM_INFO_ENUM_INT_VALUE, "Scan3dExtractionSource", val));

				if ((int64_t)val == regionId)
				{
					cx::checkOk(cx_getParam(hDevice, "Scan3dCoordinateScale", val));
					val = double(val);
					cx::checkOk(cx_3d_calib_set(hCalib, CX_3D_PARAM_RANGE_SCALE, val));

					cx::checkOk(cx_getParam(hDevice, "Scan3dCoordinateOffset", val));
					cx::checkOk(cx_3d_calib_set(hCalib, CX_3D_PARAM_RANGE_OFFSET, val));
				}
			}

			// set X offset in image space
			cx::checkOk(cx_getParam(hDevice, CX_CAM_IMAGE_OFFSET_X, val));
			cx::checkOk(cx_3d_calib_set(hCalib, CX_3D_PARAM_S_ROI_X, val));

			cx::checkOk(cx_getParam(hDevice, "RegionSelector", val));
			std::string storedRegion = val;

			// select the AOI on the camera we want to read the parameter for
			cx::checkOk(cx_setParam(hDevice, "RegionSelector", cx::Variant(regionId)));

			// set image width, if reverse-x is true set negative
			cx_status_t result;
			result = cx_getParam(hDevice, CX_CAM_IMAGE_REVERSE_X, val);
			bool reverseX = false;
			if (result == CX_STATUS_OK)
				reverseX = ((int)val == 0) ? false : true;			// TRUE = YES = 1, FALSE = NO = 0
			cx::checkOk(cx_getParam(hDevice, CX_CAM_IMAGE_WIDTH, val));
			int width = val;
			if (reverseX)
				width = -width;
			cx::checkOk(cx_3d_calib_set(hCalib, CX_3D_PARAM_S_ROI_W, cx::Variant(width)));

			// set AOI height, if reverse-y is true set negative
			result = cx_getParam(hDevice, CX_CAM_IMAGE_REVERSE_Y, val);
			bool reverseY = false;
			if (result == CX_STATUS_OK)
				reverseY = ((int)val == 0) ? false : true;			// TRUE = YES = 1, FALSE = NO = 0
			cx::checkOk(cx_getParam(hDevice, "Height", val));
			int roiHeight = val;
			if (reverseY)
				roiHeight = -roiHeight;
			cx::checkOk(cx_3d_calib_set(hCalib, CX_3D_PARAM_S_ROI_H, cx::Variant(roiHeight)));

			cx::checkOk(cx_setParam(hDevice, "RegionSelector", cx::Variant(storedRegion)));
		}

	}
}