/**
@file : cx_3d_metric.h
@package : cx_3d library
@brief C-interface for AT 3D library metric functions.
@copyright (c) 2017, Automation Technology GmbH.
@version 18.10.2016, initial version
@version 01.08.2017, revised interface
@version 08.06.2022, added CX_3D_CALIB_MODEL_N_POLY correction
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
#ifndef CX_3D_METRIC_H_INCLUDED
#define CX_3D_METRIC_H_INCLUDED

/**
@defgroup cx_base			Basic types
@defgroup cx_3d_calib		Calibration handling
@defgroup cx_3d_metric		3D image calculation
*/

#include "cx_3d_lib.h"

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************************/
//! @addtogroup cx_3d_calib
//! @{

/** Create a new calibration
	@param[out] phCalib		handle to the new created calibration, set to CX_INVALID_HANDLE if failed
*/
CX_3D_API cx_status_t __cdecl cx_3d_calib_new(CX_CALIB_HANDLE* phCalib);

/**	Release calibration, frees all resources internally used.
	@param hCalib		handle of the calibration to release
	@return CX_STATUS_OK on success, error value otherwise.
*/
CX_3D_API cx_status_t __cdecl cx_3d_calib_release(CX_CALIB_HANDLE hCalib);

//! file format flags
enum cx_3d_calib_format_flags {
	CX_3D_CALIB_FORMAT_AUTO			= 0x000,	//!< checks format automatically when loading, uses default format when saving
	CX_3D_CALIB_FORMAT_BINARY		= 0x0001,	//!< AT Binary format (little endian), see \ref cx_calib3d_binaryFormat
	CX_3D_CALIB_FORMAT_XML			= 0x0002,	//!< AT XML format, capable of storing multiple Calib-sets in one file
	CX_3D_CALIB_FORMAT_SAL3D		= 0x0003,	//!< SAL3D ASCII format 
	CX_3D_CALIB_FORMAT_MASK			= 0x000F,	//!< mask for type of file format
	CX_3D_CALIB_FORMAT_APPEND		= 0x0010	//!< flag indicates append mode, only supported when saving XML format. In append mode it tries to open file first and append to existing content. If file does not exist it will be created.
};

/**	Load calibration from file.
	@param[in] fileName		name of calibration file
	@param[in] id			id of the calibration to load, only supported for XML format. Empty causes loading of the default calib case.
	@param formatFlags		hint what kind of format to load, see \ref cx_3d_calib_format_flags definitions, if zero tries to load default format.
	@param[out] phCalib		returns handle of the loaded calibration, set to CX_INVALID_HANDLE if failed
	@return CX_STATUS_OK on success, error value otherwise.
			CX_STATUS_BAD_FORMAT if file format is unknown or does not match the format given by formatFlags
*/
CX_3D_API cx_status_t __cdecl cx_3d_calib_load(const char* fileName, const char* id, int formatFlags, CX_CALIB_HANDLE* phCalib);

/**	Load calibration from buffer.
	@param[in] buffer		input buffer with calibration file
	@param[in] bufferSz		size of data in input buffer in bytes
	@param[in] id			id of the calibration to load, only supported for XML format. Empty causes loading of the default calib case.
	@param formatFlags		hint what kind of format to load, see \ref cx_3d_calib_format_flags definitions.
	@param[out] phCalib		returns handle of the loaded calibration, set to CX_INVALID_HANDLE if failed
	@return CX_STATUS_OK on success, error value otherwise.
	CX_STATUS_BAD_FORMAT if file format is unknown or does not match the format given by formatFlags
*/
CX_3D_API cx_status_t __cdecl cx_3d_calib_loadFromBuffer(const uint8_t* buffer, size_t bufferSz, const char* id, int formatFlags, CX_CALIB_HANDLE* phCalib);

/**	Save calibration to file.
	@param hCalib			handle of the calibration to store
	@param[in] fileName		name of calibration file. The file might already contain valid data when XML format is written and CX_3D_CALIB_FORMAT_APPEND was set.
	@param[in] id			id of the calibration to save under, only supported for XML format. If empty string is given it creates an id internally "cal<idx>".
	@param formatFlags		format to save, see \ref cx_3d_calib_format_flags definitions, if zero saves as default format.
	@return CX_STATUS_OK on success, error value otherwise.
*/
CX_3D_API cx_status_t __cdecl cx_3d_calib_save(CX_CALIB_HANDLE hCalib, const char* fileName, const char* id, int formatFlags);

/**	Save calibration to buffer.
	@param hCalib			handle of the calibration to store
	@param[inout] buffer	input/output buffer with calibration file, might contain valid input data when XML file is written and CX_3D_CALIB_FORMAT_APPEND was set
	@param[inout] bufferSz	size of output buffer in bytes on input, size of valid data on output.
	@param[in] id			id of the calibration to save under, only supported for XML format. If empty string it saves the default calib case.
	@param formatFlags		hint what kind of format to save, see \ref cx_3d_calib_format_flags definitions.
	@return CX_STATUS_OK on success, error value otherwise.
	CX_STATUS_BAD_FORMAT if file format is unknown or does not match the format given by formatFlags
	\note
		If *bufferSz is zero or buffer=NULL the required buffer size is returned in bufferSz.
*/
CX_3D_API cx_status_t __cdecl cx_3d_calib_saveToBuffer(CX_CALIB_HANDLE hCalib, uint8_t* buffer, size_t* bufferSz, const char* id, int formatFlags);

//! Definition of paramter CX_3D_PARAM_MODEL
enum cx_3d_calib_model {
	CX_3D_CALIB_MODEL_P_MASK		= 0x00000F,	//!< 3D geometric model type
	CX_3D_CALIB_MODEL_P_UNDEFINED	= 0x000000,	//!< undefined, e.g. after initialization
	CX_3D_CALIB_MODEL_P_HOM			= 0x000001,	//!< homography camera model, transforms image-plane to laser-plane, [3x3] matrix
	CX_3D_CALIB_MODEL_P_PH			= 0x000002,	//!< pinhole camera model, transforms image-plane to 3D space, [3x3][3x4] matrix
	CX_3D_CALIB_MODEL_L_MASK		= 0x000F00,	//!< correction model for lens correction on sensor side
	CX_3D_CALIB_MODEL_L_NONE		= 0x000000,	//!< no lens distortion correction
	CX_3D_CALIB_MODEL_L_POLY		= 0x000100,	//!< lens correction model with polynom 3rd or 4th order
	CX_3D_CALIB_MODEL_L_BC			= 0x000200,	//!< Brown-Conrady lens correction model with 3, 5 or 7 parameters, 2 parameters for center of distortion, 1-3 parameter for radial distortion and 2 parameter for tangential distortion, \see https://en.wikipedia.org/wiki/Distortion_(optics)
	CX_3D_CALIB_MODEL_L_DIV			= 0x000300,	//!< division lens correction model, typically just 3 parameters used, 2 parameters for center of distortion + 1 for distortion model \see https://en.wikipedia.org/wiki/Distortion_(optics)
	CX_3D_CALIB_MODEL_C_MASK		= 0x0F0000,	//!< additional correction model for lens and laser line correction on object side
	CX_3D_CALIB_MODEL_C_NONE		= 0x000000,	//!< no additional correction
	CX_3D_CALIB_MODEL_C_POLY		= 0x010000,	//!< object side polynomial correction for lens and laser supports 2nd, 3rd and 4th order
	CX_3D_CALIB_MODEL_N_MASK		= 0xF00000,	//!< additional correction model for laser line straightness
	CX_3D_CALIB_MODEL_N_NONE		= 0x000000,	//!< no additional correction
	CX_3D_CALIB_MODEL_N_POLY		= 0x100000	//!< additional laser straightness correction
};

//! Definition of paramter CX_3D_PARAM_TARGET
enum cx_3d_calib_target {
	CX_3D_TARGET_MASK				= 0x00FF,	//!< mask for type of target
	CX_3D_TARGET_FACETS_12			= 0x0001,	//!< multi facet object with 12 points, aka linear calibration target
	CX_3D_TARGET_TRAPEZOID			= 0x0002,	//!< single isosceles trapezium with four points, aka static calibration target
	CX_3D_TARGET_SAWTOOTH			= 0x0003,	//!< Multi-Triangle with three points per triangle, aka zigzag calibration target
	CX_3D_TARGET_MULTI_TRAPEZOID	= 0x0004,	//!< Multi-Trapezoid with four points per isosceles trapezium
	CX_3D_TARGET_STEP				= 0x0005,	//!< Step height target with four points
	CX_3D_TARGET_CHESSBOARD			= 0x0006	//!< chessboard with NxM cross-points, can be used for intrinsic model and/or lens correction
};

/**	Parameter for cx_3d_calib_get/set, cx_3d_target_get/set
*/
typedef enum cx_3d_calib_param {
	CX_3D_PARAM_ID			= 0x0000,	//!< calib case ID, RO property, internally set when reading or writing. The id is used to identify multiple calibrations in one file.
	CX_3D_PARAM_MODEL		= 0x0001,	//!< model type, see \ref cx_3d_calib_model
	CX_3D_PARAM_TARGET		= 0x0002,	//!< calibration target type this calibration was calculated from, see \ref cx_3d_calib_target
	CX_3D_PARAM_NAME		= 0x0003,	//!< name of calibration as string
	CX_3D_PARAM_SENSOR_SN	= 0x0004,	//!< serial number of the sensor the calibration was made for, returned as string
	CX_3D_PARAM_DATE		= 0x0005,	//!< date time of calibration as string,  should be formated according to ISO 8601 "CCYY-MM-DDThh:mm:ss[Z|(+|-)hh:mm]", e.g. "2017-11-15T18:32:52"
	CX_3D_PARAM_CREATOR		= 0x0006,	//!< information about the creator of the calibration as string
	CX_3D_PARAM_DESCRIPTION = 0x0007,	//!< optional description as string
	CX_3D_PARAM_RANGE_SCALE = 0x0008,	//!< factor determined by number of subpixel in range data
	CX_3D_PARAM_F			= 0x0009,	//!< focal length of lens in mm, only required if sensor range angle is not zero
	CX_3D_PARAM_S_ROI_X		= 0x000A,	//!< x-offset in sensor image coordinates in units of pixels, typically matches with sensor region OffsetX
	CX_3D_PARAM_S_ROI_Y		= 0x000B,	//!< y-offset in sensor image coordinates in units of pixels, typically matches with sensor region OffsetY
	CX_3D_PARAM_S_ROI_W		= 0x000C,	//!< horizontal size of sensor aoi in integer pixel coordinates. Negative value means image x-values are horizontal fliped. Parameter is only necessary if flipped, otherwise can be 0.
	CX_3D_PARAM_S_ROI_H		= 0x000D,	//!< vertical size of sensor aoi in integer pixel coordinates. Negative value means image y-values are vertically fliped. Parameter is only necessary if flipped, otherwise can be 0.
	CX_3D_PARAM_S_ROI		= 0x000E,	//!< same as above but you can set all four values at once
	CX_3D_PARAM_S_R			= 0x000F,	//!< rotation angles of sensor in respect to the optical axis, x = Scheimpflug angle, 0=normal perpendicular to optical axis, unit radiant.
	CX_3D_PARAM_S_SZ		= 0x0010,	//!< sensor size in pixel
	CX_3D_PARAM_S_PSZ		= 0x0011,	//!< sensor pixel size in mm
	CX_3D_PARAM_CP			= 0x0012,	//!< principal point in pixel units
	CX_3D_PARAM_P			= 0x0013,	//!< camera model parameters, e.g. for homography matrix 3x3 doubles
	CX_3D_PARAM_L			= 0x0014,	//!< lens correction parameters in image plane
	CX_3D_PARAM_C			= 0x0015,	//!< additional correction parameters on object side in laser plane, e.g. for laser correction
	CX_3D_PARAM_SY			= 0x0016,	//!< distance from one profile to next, is multiplied with given y-coordinate
	CX_3D_PARAM_SXY			= 0x0017,	//!< x-axis skew parameter dependent on translation direction
	CX_3D_PARAM_SZY			= 0x0018,	//!< z-axis skew parameter dependent on translation direction
	CX_3D_PARAM_R			= 0x0019,	//!< transformation parameter (rotation) from Anchor (Sensor-CS) to Transformed (World-CS) (defines Pose) = RotationX,Y,Z
	CX_3D_PARAM_T			= 0x001A,	//!< transformation parameter (translation) from Anchor (Sensor-CS) to Transformed (World-CS) (defines location) = TranslationX,Y,Z
	/** accuracy statistic
		Statistic stored in the order
		[mean(x), std(x), max(abs(x));
		mean(y), std(y), max(abs(y));
		mean(z), std(z), max(abs(z));
		mean(d), std(d), max(abs(d))]
		In case of intrinsic calibration only, 2d points x and z components are set, y channel is set to zero.
	*/
	CX_3D_PARAM_A_STAT		= 0x001B,
	CX_3D_PARAM_L_STAT		= 0x001C,			//!< lens correction error statistics in pixels [ mean, std, max, ...]
	CX_3D_PARAM_FOV			= 0x001D,			//!< calibrated FOV, returned as 9 doubles [near-z, xl, xr, center-z, xl, xr, far-z, xl, xr]
	CX_3D_PARAM_ROC			= 0x001E,			//!< valid region of calibration in sensor pixel coordinates, [x0, y0, dx, dy] this can be used to calculate FOV in target space
	CX_3D_PARAM_METRIC_O	= 0x001F,			//!< metric offset for conversion pixel value to world coordinates, offsetA,B,C
	CX_3D_PARAM_METRIC_S	= 0x0020,			//!< metric scale factor for conversion pixel value to world coordinates, scaleA,B,C
	CX_3D_PARAM_METRIC_IDV	= 0x0021,			//!< invalid data value for marking invalid data points, only Z coordinate is marked with this value. If pixel_format is integer it must be rounded to integer.
	CX_3D_PARAM_METRIC_CACHE_MODE	= 0x0022,	//!< 0 = don't use LUT optimization, 1=use bilinear LUT. When calling with argument > 0, the internal cache gets updated. This can be used to trigger a final cache update after manually setting all parameters. This prevents a cache update at first call to metric functions cx_3d_range2calibratedABC, cx_3d_range2rectifiedC.
	CX_3D_PARAM_CALIB_EPS			= 0x0023,	//!< the desired accuracy at which the iterative computation of the calibration model stops
	CX_3D_PARAM_CALIB_MAX_ITER		= 0x0024,	//!< the maximum number of iterations to compute the calibration model, 0=no iteration limit
	CX_3D_PARAM_METRIC_CACHE_PARAMS = 0x0025,	//!< sampling division values used for cache mode for x and z. Given as integer arrays with two numbers. Only values are the power of 2 of the actual division value. Default is [4, 5] which means a division value of 16 and 32 for x and z. When reading this parameter you get back the division values as well as the actual size of the luts.
	CX_3D_PARAM_METRIC_CACHE_LUTX	= 0x0026,	//!< Read the current X-Lut values, There is no write access for this parameter.
	CX_3D_PARAM_METRIC_CACHE_LUTZ	= 0x0027,	//!< Read the current Z-Lut values, There is no write access for this parameter.
	CX_3D_PARAM_S_RR_H	= 0x0028, 				//!< Reduction value of horizontal resolution, sensor x-values are multiplied with this value. Value should be set to SFNC BinningHorizontal * DecimationHorizontal; unsigned integer value, default = 1 = no reduction.
	CX_3D_PARAM_S_RR_V	= 0x0029, 				//!< Reduction value of vertical resolution, sensor range(y)-values are multiplied with this value. Value should be set to SFNC BinningVertical * DecimationVertical; unsigned integer value, default = 1 = no reduction.
	CX_3D_PARAM_RANGE_OFFSET = 0x002A,			//!< Offset when transforming a pixel from relative coordinates to world coordinates
	CX_3D_PARAM_N = 0x002B						//!< correction parameters for laser straightness correction
} cx_3d_calib_param_t; 

/**	get parameter of the calibration model
	@param hCalib		handle of the calibration
	@param[in] param	parameter to get, see \ref cx_3d_calib_param defines for possible values.
	@param[out] val		pointer to variable for return.
	@return CX_STATUS_OK on success
			CX_STATUS_INVALID_PARAMETER if parameter is not available.
*/
CX_3D_API cx_status_t __cdecl cx_3d_calib_get(CX_CALIB_HANDLE hCalib, cx_3d_calib_param_t param, cx_variant_t* val);

/**	set parameter of the calibration model
	@param hCalib		handle of the calibration
	@param[in] param	parameter to set, see \ref cx_3d_calib_param defines for possible values.
	@param[in] val		pointer to value. If variant data type does not match expected type for parameter CX_STATUS_INVALID_PARAMETER is returned.
	@return CX_STATUS_OK on success
			CX_STATUS_INVALID_PARAMETER if parameter is not available.
*/
CX_3D_API cx_status_t __cdecl cx_3d_calib_set(CX_CALIB_HANDLE hCalib, cx_3d_calib_param_t param, const cx_variant_t* val);

/** Calculate world coordinates from sensor pixel coordinates
	
	@param hCalib		handle of the calibration
	@param[in] pi		array of points in sensor coordinates (image x, encoder y, image z)
	@param[out] po		resulting array of points in world coordinates (x, y, z)
	@param numPoints	number of points in pi and po
	@return CX_STATUS_OK on success
	
	\note
		The function cx_3d_sensor2world does not support the internal LUT accelerated calculation. 
		For conversion of range image to point cloud use the faster function cx_3d_range2calibratedABC or cx_3d_rangeWithChunk2calibratedABC.
*/
CX_3D_API cx_status_t __cdecl cx_3d_sensor2world(CX_CALIB_HANDLE hCalib, const cx_point3r_t* pi, cx_point3r_t* po, unsigned numPoints);

/** Calculate sensor coordinates from world coordinates.
	
	@param hCalib		handle of the calibration
	@param[in] pi		array of points in world coordinates (x, y, z)
	@param[out] po		resulting array of points in sensor coordinates (image x, encoder y, image z)
	@param numPoints	number of points in pi and po
	@return CX_STATUS_OK on success
	
	\note
		For calibration models with lens distortion correction the transformation from world to sensor coordinates can not be fully inverse calculated. 
		In this case the function returns an estimation of the sensor coordinate.
*/
CX_3D_API cx_status_t __cdecl cx_3d_world2sensor(CX_CALIB_HANDLE hCalib, const cx_point3r_t* pi, cx_point3r_t* po, unsigned numPoints);

/** function for comparison of two calibrations
*/
CX_3D_API cx_status_t __cdecl cx_3d_calib_compare(CX_CALIB_HANDLE hCalib1, CX_CALIB_HANDLE hCalib2, int flags);

//! @} cx_3d_calib

/*************************************************************************************/
// GenICam Scan3D equivalent functions for calculation of images in 3D world coordinates
//! @addtogroup cx_3d_metric
//! @{ 
//! GenICam Scan3D equivalent functions for calculation of images in 3D world coordinates

//! Definition of metric flags
enum cx_3d_metric_flags
{
	CX_3D_METRIC_RECTIFICATION_ALG_MASK	= 0x000F,	//!< mask for rectification algorithm 
	CX_3D_METRIC_NEAREST_POINT			= 0x0000,	//!< use nearest point algorithm for calculating recified image
	CX_3D_METRIC_INTERP_IDW				= 0x0001,	//!< use inverse distance weighting (IDW) interpolation algorithm for calculating recified image
	CX_3D_METRIC_MARK_Z_INVALID_DATA	= 0x0010,	//!< mark invalid data with value of CX_3D_PARAM_METRIC_IDV. Only Z coordinate is marked.
	CX_3D_METRIC_FILL_HOLES				= 0x0020,	//!< fill holes in Z-Map with linear interpolation of neighborhood pixel when no projection was found for a pixel position.
	CX_3D_METRIC_USE_CHUNK_Y			= 0x0100,	//!< chunkVec contains Y coordinate per profile that should be used for transformation. E.g. encoder position from chunk data. 
	CX_3D_METRIC_USE_CHUNK_Z0			= 0x0200,	//!< chunkVec contains Z0 coordinate per profile that should be used for transformation. E.g. aoi-ys when using AOI-Y tracking and absolut value is false. \note  It is similar to a dynamic change of parameter CX_3D_PARAM_S_ROI_Y.
	CX_3D_METRIC_USE_CHUNK_X0			= 0x0400,	//!< chunkVec contains X0 coordinate per profile that should be used for transformation. E.g. aoi-xs when using AOI-X tracking. \note  It is similar to a dynamic change of parameter CX_3D_PARAM_S_ROI_X.
};

/** Calculate a 2.5D Point Cloud grid image with three components A,B,C from range image.
	
	The 3D world-coordinate is given by:
	\f$x=offsetA + scaleA*A(i,j)\f$ ,  
	\f$y=offsetB + scaleB*B(i,j)\f$ , 
	\f$z=offsetC + scaleC*C(i,j)\f$

	@param hCalib				handle of the calibration to apply	
	@param[in] rangeImg			input range image
	@param[in] chunk			optional chunk data, if Null parameter is omitted. If used it must contain one cx_chunk_profile_info_t per row of range image, see also flags CX_3D_METRIC_USE_CHUNK_XXX of \ref cx_3d_metric_flags.
	@param[out] pointCloudImg	resulting point cloud image. Image must be allocated by caller.
								The image pixel format and dimenions are used for zMap calculation.
								Pixelformats CX_PF_COORD3D_ABC8, CX_PF_COORD3D_ABC8_PLANAR, CX_PF_COORD3D_ABC16, CX_PF_COORD3D_ABC16_PLANAR, CX_PF_COORD3D_ABC32f and CX_PF_COORD3D_ABC32f_PLANAR are supported.
								In case the image dimension does not fit the input rangeImg dimensions or the given pixelformat is not supported we return error status CX_STATUS_WRONG_BUFFER_SIZE.
	@param flags				supported flags: CX_3D_METRIC_MARK_Z_INVALID_DATA, see \ref cx_3d_metric_flags

	C Example:
	
	\code{.c}
		CX_CALIB_HANDLE hCalib=CX_INVALID_HANDLE;
		cx_img_t pcImg;
		cx_3d_calib_load("calib.xml", CX_3D_CALIB_FORMAT_AUTO, &hCalib);
		cx_image_alloc(&pcImg, rangeImg.height, rangeImg.width, CX_PF_COORD3D_ABC32f);
		cx_3d_range2calibratedABC(hCalib, &rangeImg, &pcImg, CX_3D_METRIC_MARK_Z_INVALID_DATA);
		
		// further processing ...
		
		cx_image_free(&pcImg);
		cx_3d_calib_release(hCalib);
	\endcode

	C++ Example:
	
	\code{.cpp}
		cx::c3d::Calib calib;
		calib.load("calib.xml");
		calib.setParam(CX_3D_PARAM_METRIC_IDV, float(NAN));	// Set invalid data value (IDV). That value is used to mark invalid values in rectified image.
		cx::c3d::PointCloud pc(rangeImg.height, rangeImg.width, CX_PF_COORD3D_ABC32f, cx::Point3f(1.0f, 1.0f, 1.0f), cx::Point3f(0.0f, 0.0f, 0.0f));
		cx::c3d::calculatePointCloud(calib, rangeImg, pc, CX_PF_COORD3D_ABC32f);
	\endcode
*/
CX_3D_API cx_status_t __cdecl cx_3d_range2calibratedABC(CX_CALIB_HANDLE hCalib, const cx_img_t* rangeImg, cx_img_t* pointCloudImg, int flags);

/** Calculate a 2.5D Point Cloud grid image with three components A,B,C from range image.
	
	The 3D world-coordinate is given by:
	\f$x=offsetA + scaleA*A(i,j)\f$ ,  
	\f$y=offsetB + scaleB*B(i,j)\f$ , 
	\f$z=offsetC + scaleC*C(i,j)\f$
	
	Function allows the usage of chunk data values for calculation, see also \ref cx::c3d::Chunk and \ref CX_CHUNK_CAMERA_INFO.

	@param hCalib				handle of the calibration to apply
	@param[in] rangeImg			input range image
	@param[in] xs				optional chunk data vector of AOI-xs values. If Null parameter is omitted. If used it must point to a vector of xs-values with number of values be equal to number of rows of range image.
	@param[in] ys				optional chunk data vector of AOI-ys values. If Null parameter is omitted. If used it must point to a vector of ys-values with number of values be equal to number of rows of range image.
	@param[in] encoderValue		optional chunk data vector of position counter values. If Null parameter is omitted. If used it must point to a vector of position counter values with number of values be equal to number of rows of range image.
	@param[out] pointCloudImg	resulting point cloud image. Image must be allocated by caller. 
								The image pixel format and dimenions are used for zMap calculation. 
								Pixelformats Coord3D_ABC8, CX_PF_COORD3D_ABC8_PLANAR, Coord3D_ABC16, CX_PF_COORD3D_ABC16_PLANAR, Coord3D_ABC32f and CX_PF_COORD3D_ABC32f_PLANAR are supported.
								In case the image dimension does not fit the input rangeImg dimensions or the given pixelformat is not supported we return error status CX_STATUS_WRONG_BUFFER_SIZE.
	@param flags				supported flags: CX_3D_METRIC_MARK_Z_INVALID_DATA, see \ref cx_3d_metric_flags

	C Example:

	\code{.c}
		CX_CALIB_HANDLE hCalib=CX_INVALID_HANDLE;
		cx_img_t pcImg;
		cx_3d_calib_load("calib.xml", CX_3D_CALIB_FORMAT_AUTO, &hCalib);
		cx_image_alloc(&pcImg, rangeImg.height, rangeImg.width, CX_PF_COORD3D_ABC32f);
		cx_3d_rangeWithChunk2calibratedABC(hCalib, &rangeImg, xs, ys, encoderValue, &pcImg, CX_3D_METRIC_MARK_Z_INVALID_DATA);
		
		// further processing ...
		
		cx_image_free(&pcImg);
		cx_3d_calib_release(hCalib);
	\endcode

	C++ Example:

	\code{.cpp}
		cx::c3d::Calib calib;
		calib.load("calib.xml");
		calib.setParam(CX_3D_PARAM_METRIC_IDV, float(NAN));	// Set invalid data value (IDV). That value is used to mark invalid values in rectified image.
		cx::c3d::PointCloud pc(rangeImg.height, rangeImg.width, CX_PF_COORD3D_ABC32f, cx::Point3f(1.0f, 1.0f, 1.0f), cx::Point3f(0.0f, 0.0f, 0.0f));
		cx::c3d::calculatePointCloud(calib, rangeImg, xs, ys, encoderValue, pc);
	\endcode
*/
CX_3D_API cx_status_t __cdecl cx_3d_rangeWithChunk2calibratedABC(CX_CALIB_HANDLE hCalib, const cx_img_t* rangeImg, const uint16_t* xs, const uint16_t* ys, const int32_t* encoderValue, cx_img_t* pointCloudImg, int flags);

/** Calculate a rectified depth image (aka Z-Map) with uniform sampling from range image.

	The 3D world-coordinate \f$pixel(i,j)\f$ is given by:
	\f$x=offsetA + scaleA*i\f$, 
	\f$y=offsetB + scaleB*j\f$, 
	\f$z=offsetC + zMap(i, j)*scaleC\f$

	@param[in] hCalib			handle of the calibration to apply
	@param[in] rangeImg			input range image, suported input pixel types are CX_PF_MONO_8 and CX_PF_MONO_16
	@param[inout] rectifiedImg	resulting rectified image. Image must be allocated by caller. 
								The image pixel format and dimensions are used for zMap calculation. Pixelformats CX_PF_COORD3D_C16 and CX_PF_COORD3D_C32f are supported.
	@param flags				flags controlling algorithm and invalid data handling, see \ref cx_3d_metric_flags
	 
	\note
		Function ignores range values of zero also when flag CX_3D_METRIC_MARK_Z_INVALID_DATA is not set.

	C Example:

	\code{.c}
		CX_CALIB_HANDLE hCalib=CX_INVALID_HANDLE;
		cx_img_t zMap;
		cx_variant_t var;
		float ivd = NAN;
		cx_variant_init(&var);
		float zMap_offset[] = {-50.0f, 0.0f, 0.0f};	// offset in 3d coordinate system
		float zMap_scale[] = {0.1f, 0.1f, 1.0f};	// scale, e.g. mm/pixel, in case of float pixel format we don't want to scale Z

		cx_3d_calib_load("calib.xml", CX_3D_CALIB_FORMAT_AUTO, &hCalib);
		cx_variant_set_vecf(&var, zMap_offset, 3);
		cx_3d_calib_set(hCalib, CX_3D_PARAM_METRIC_O, &var);
		cx_variant_set_vecf(&var, zMap_scale, 3);
		cx_3d_calib_set(hCalib, CX_3D_PARAM_METRIC_S, &var);
		cx_variant_set_float(&var, ivd);
		cx_3d_calib_set(hCalib, CX_3D_PARAM_METRIC_IDV, &var);		// Set invalid data value (IDV). That value is used to mark invalid values in rectified image.
		cx_image_alloc(&zMap, zMap_dy, zMap_dx, CX_PF_COORD3D_C32f);
		cx_3d_range2rectifiedC(hCalib, &rangeImg, &zMap, CX_3D_METRIC_MARK_Z_INVALID_DATA | CX_3D_METRIC_INTERP_IDW);
		
		// further processing ...
		
		cx_image_free(zMap);
		cx_3d_calib_release(hCalib);
	\endcode

	C++ Example:

	\code{.cpp} 
		cx::c3d::Calib calib;
		calib.load("calib.xml");
		float ivd = NAN;
		calib.setParam(CX_3D_PARAM_METRIC_IDV, ivd);			// Set invalid data value (IDV). That value is used to mark invalid values in rectified image.
		cx::Point3f zMap_offset(-50.0f, 0.0f, 0.0f);	// offset in 3d coordinate system
		cx::Point3f zMap_scale(0.1f, 0.1f, 1.0f);		// scale, e.g. mm/pixel, in case of float pixel format we don't want to scale Z
		cx::c3d::ZMap zMap(zMap_dy, zMap_dx, CX_PF_COORD3D_C32f, zMap_scale, zMap_offset);
		cx::c3d::calculateZMap(calib, rangeImg, zMap, CX_3D_METRIC_MARK_Z_INVALID_DATA | CX_3D_METRIC_INTERP_IDW);
		
		// further processing, e.g. calculate point cloud from zMap and visualize ...

		cx::c3d::PointCloud pc;
		cx::c3d::convertToPointCloud(zMap, pc, ivd);
	\endcode
*/
CX_3D_API cx_status_t __cdecl cx_3d_range2rectifiedC(CX_CALIB_HANDLE hCalib, const cx_img_t* rangeImg, cx_img_t* rectifiedImg, int flags);

/** a rectified depth image (aka Z-Map) with uniform sampling from range image.
	Function allows the usage of chunk data values for calculation, see also \ref cx::c3d::Chunk and \ref CX_CHUNK_CAMERA_INFO.

	The 3D world-coordinate \f$pixel(i,j)\f$ is given by:
	\f$x=offsetA + scaleA*i\f$, 
	\f$y=offsetB + scaleB*j\f$, 
	\f$z=offsetC + zMap(i, j)*scaleC\f$

	@param[in] hCalib			handle of the calibration to apply
	@param[in] rangeImg			input range image, suported input pixel types are CX_PF_MONO_8 and CX_PF_MONO_16
	@param[in] xs				optional chunk data vector of AOI-xs values. If Null parameter is omitted. If used it must point to a vector of xs-values with number of values be equal to number of rows of range image.
	@param[in] ys				optional chunk data vector of AOI-ys values. If Null parameter is omitted. If used it must point to a vector of ys-values with number of values be equal to number of rows of range image.
	@param[in] encoderValue		optional chunk data vector of position counter values. If Null parameter is omitted. If used it must point to a vector of position counter values with number of values be equal to number of rows of range image.
	@param[inout] rectifiedImg	resulting rectified image. Image must be allocated by caller.
								The image pixel format and dimensions are used for zMap calculation. Pixelformats CX_PF_COORD3D_C16 and CX_PF_COORD3D_C32f are supported.
	@param flags				flags controlling algorithm and invalid data handling, see \ref cx_3d_metric_flags

	\note
		It ignores range values of zero also when flag CX_3D_METRIC_MARK_Z_INVALID_DATA is not set.

	C++ Example:

	\code{.cpp}
		cx::c3d::Calib calib;
		calib.load("calib.xml");
		float ivd = NAN;
		calib.setParam(CX_3D_PARAM_METRIC_IDV, ivd);			// Set invalid data value (IDV). That value is used to mark invalid values in rectified image.
		cx::Point3f zMap_offset(-50.0f, 0.0f, 0.0f);	// offset in 3d coordinate system
		cx::Point3f zMap_scale(0.1f, 0.1f, 1.0f);		// scale, e.g. mm/pixel, in case of float pixel format we don't want to scale Z
		cx::c3d::ZMap zMap(zMap_dy, zMap_dx, CX_PF_COORD3D_C32f, zMap_scale, zMap_offset);
		cx::c3d::Chunk chunkData(cx_chunk);
		cx::c3d::calculateZMap(calib, rangeImg, zMap, chunkData.getXs(), chunkData.getYs(), chunkData.getPosCounter(), CX_3D_METRIC_MARK_Z_INVALID_DATA | CX_3D_METRIC_INTERP_IDW);

		// further processing, e.g. calculate point cloud from zMap and visualize ...

		cx::c3d::PointCloud pc;
		cx::c3d::convertToPointCloud(zMap, pc, ivd);
	\endcode

	For further examples see also \ref cx_3d_range2rectifiedC.
*/
CX_3D_API cx_status_t __cdecl cx_3d_rangeWithChunk2rectifiedC(CX_CALIB_HANDLE hCalib, const cx_img_t* rangeImg, const uint16_t* xs, const uint16_t* ys, const int32_t* encoderValue, cx_img_t* rectifiedImg, int flags);

//! @} cx_3d_metric

#ifdef __cplusplus
}
#endif

#endif /* CX_3D_METRIC_H_INCLUDED */
