/*************************************************************************************
@brief Definition of chunk data structs and IDs for cx - API libs
@file : cx_chunk_data_structs.h
@package : cx_base library, AT CX-libraries

@copyright (c) 2017, Automation Technology GmbH.

@version 27.11.2016, initial version cx_types
@version 01.08.2017, revised interface as separate library

*************************************************************************************/
#pragma once
#ifndef CX_CHUNK_DATA_SRUCTS_H_INCLUDED
#define CX_CHUNK_DATA_SRUCTS_H_INCLUDED

#include "cx_struct_packing.h"

#ifdef __cplusplus
extern "C" {
#endif

enum cx_chunk_id {
	CX_CHUNK_IMAGE_INFO_ID = 0xA5A5A5A5,
	CX_CHUNK_FRAME_INFO_ID = 0x11119999,
	CX_CHUNK_CAMERA_INFO_ID = 0x66669999,
	CX_CHUNK_PROFILE_INFO_ID = 0x66669999,
	CX_CHUNK_IRSX_IMAGE_INFO_ID = 0x66668888,
	CX_CHUNK_C6_LINE_INFO_ID = 0x66667777,
	CX_C6_CHUNK_FRAME_INFO_ID = 0x44443333,
	CX_C6_CHUNK_SCAN3D_REGION_INFO_ID = 0x55553333
};

CX_STRUCT_PACKED_START
#if defined(__MINGW32__) || defined(__MINGW64__)
#pragma pack(1)
#endif

/// in memory struct of chunk image info
struct CX_CHUNK_FRAME_INFO
{
	uint32_t sizeYReal;
	uint32_t numChunkAcqInfo;
	uint32_t flag;
} CX_STRUCT_PACKED;

#define CX_CHUNK_FRAME_INFO_SIZE 12

/// in memory struct of chunk camera info with byte order LittleEndian
struct CX_CHUNK_CAMERA_INFO
{
	uint32_t 	timeStamp64L;	//!< time stamp low part
	uint32_t 	timeStamp64H;	//!< time stamp high part. SFNC feature "ChunkTimestamp".
	uint32_t 	frameId;		//!< frame counter. SFNC feature "ChunkFrameID".
	int32_t		encoderValue;	//!< 32 Bit position counter. Note: position counter supports up and down counting. SFNC feature "EncoderValue".
	uint8_t 	lineStatus;		//!< trigger and I/O line status, maps to SFNC feature "ChunkLineStatusAll".
	uint16_t 	AO0;			//!< DAC value
	uint16_t 	AI0;			//!< ADC value
	uint8_t 	INT_idx;		//!< index of active sensor integration time. 
	uint8_t 	AOI_idx;		//!< index of active sensor window (AOI) starting with 0
	uint16_t 	AOI_ys;			//!< AOI y-start, value can change from sensor frame to sensor frame. SFNC feature "ChunkOffsetY".
	uint16_t 	AOI_dy;			//!< AOI y-size, value can change from sensor frame to sensor frame. SFNC feature "ChunkHeight".
	uint16_t 	AOI_xs;			//!< AOI x-start, value can change from sensor frame to sensor frame. SFNC feature "ChunkOffsetX".
	uint16_t 	AOI_trsh;		//!< treshold value used for the active AOI
	uint8_t 	AOI_alg;		//!< algorithm used for the active AOI.
} CX_STRUCT_PACKED;

#define CX_CHUNK_CAMERA_INFO_SIZE 32
#define CX_CHUNK_PROFILE_INFO_SIZE CX_CHUNK_CAMERA_INFO_SIZE

/// in memory struct of chunk irsx image info with byte order LittleEndian
struct CX_CHUNK_IRSX_IMAGE_INFO
{
	uint64_t	timeStamp_Exposure;			// 64 Bit Time stamp in device timestamp unit
	uint64_t	frameId;					// Frame counter (optional reset with acquisition start)
	int32_t		encoderValue;				// position counter value. Position counter supports up and down counting.
	uint16_t	lineStatusAll;				// trigger and I/O line status (more lines…) NEEDS TO BE DISCUSSED
	uint16_t	AO0;						// DAC value
	uint16_t	AI0;						// ADC value
	uint8_t		CFG_idx;					// index of active sensor integration / detector configuration
	uint8_t		reserved;					// 
	uint16_t	AOI_ys;						// AOI y-start, value can change from sensor frame to sensor frame
	uint16_t	AOI_dy;						// AOI y-size, value can change from sensor frame to sensor frame
	uint16_t	AOI_xs;						// AOI x-start, value can change from sensor frame to sensor frame
	uint16_t	AOI_dx;						// AOI x-size, value can change from sensor frame to sensor frame
	uint64_t	timeStamp_Trigger;			// 64 Bit Time stamp in device timestamp unit, latch at trigger
	int32_t		encoderValue_Trigger;		// position counter value, latch at trigger
	uint16_t	AOI_max;					// Image Max-Value
	uint16_t	AOI_min;					// Image Min-Value
	uint64_t	AOI_sum;					// Image Sum-Value
	uint64_t	AOI_focus;					// Focus Value
} CX_STRUCT_PACKED;

#define CX_CHUNK_IRSX_IMAGE_INFO_SIZE 68

struct CX_CHUNK_C6_LINE_INFO
{
	uint64_t	timeStamp_Exposure;			// 64 Bit Time stamp in device timestamp unit
	uint64_t	frameId;					// Frame counter (optional reset with acquisition start)
	int32_t		encoderValue;				// position counter value. Position counter supports up and down counting.
	uint16_t	lineStatusAll;				// trigger and I/O line status (more lines…) NEEDS TO BE DISCUSSED
	uint16_t	AO0;						// DAC value
	uint16_t	AI0;						// ADC value
	uint8_t		CFG_idx;					// index of active sensor integration / detector configuration
	uint8_t		reserved;					// 
	uint16_t	AOI_ys;						// AOI y-start, value can change from sensor frame to sensor frame
	uint16_t	AOI_dy;						// AOI y-size, value can change from sensor frame to sensor frame
	uint16_t	AOI_xs;						// AOI x-start, value can change from sensor frame to sensor frame
	uint16_t	AOI_dx;						// AOI x-size, value can change from sensor frame to sensor frame
	uint64_t	timeStamp_Trigger;			// 64 Bit Time stamp in device timestamp unit, latch at trigger
	int32_t		encoderValue_Trigger;		// position counter value, latch at trigger
	uint16_t	Line_max;					// Image Max-Value
	uint16_t	Line_min;					// Image Min-Value
	uint64_t	Line_sum;					// Image Sum-Value
} CX_STRUCT_PACKED;

#define CX_CHUNK_C6_LINE_INFO_SIZE 60

struct CX_C6_CHUNK_FRAME_INFO
{
	uint64_t	timeStamp;					// Timestamp of the image included in the payload at the time of the Frame Exposure
	uint64_t	frameId;					// Frame counter (optional reset with acquisition start)
	int32_t		encoderValue;				// position counter value at exposure start
	uint16_t	analogOutValue;				// DAC value
	uint16_t	analogInValue;				// ADC value
	uint64_t	triggerTimeStamp;			// Timestamp of the trigger that triggered the Frame exposure
	int32_t		triggerEncoderValue;		// position counter value, latch at trigger
	uint32_t	flags;						// DecimationHorizontal, DecimationVertical, BinningVertical, ReverseY
	uint16_t	lineStatusAll;				// trigger and I/O line status (more lines…)
	uint16_t	reserved0;
	uint32_t    reserved1;
	uint64_t    reserved2;
	uint64_t    reserved3;
} CX_STRUCT_PACKED;

#define CX_C6_CHUNK_FRAME_INFO_SIZE 64

struct CX_C6_CHUNK_SCAN3D_REGION_INFO
{
	uint16_t	regionId;					// siehe ID Definition in MultiPartDma Buffer Organisation
	uint16_t	regionOffsetY;				// Region Offset y der Source Region, value can change from sensor frame to sensor frame
	uint16_t	regionHeight;				// Region Height  y-size (can not be changed during acquisition)
	uint16_t	regionOffsetX;				// Region Offset x der Source Region, value can change from sensor frame to sensor frame
	uint16_t	regionWidth;				// Region Width  x-size (can not be changed during acquisition)
	uint16_t	rangeNumValid;				// Profil-Merkmal: Anzahl der Validen Range Datenpunkte im Profile
	uint32_t 	rangeMax;					// Profil-Merkmal: Range-Max-Value
	uint64_t 	rangeSum;					// Profil-Merkmal: Range-Sum-Value, es werden nur valide Rangewerte aufsummiert
	uint32_t 	rangeMin;					// Profil-Merkmal: Range-Min-Value
	float		coordinateScale_a;			// siehe Multipart 3D Standard
	float		coordinateScale_b;			// siehe Multipart 3D Standard
	float		coordinateScale_c;			// siehe Multipart 3D Standard
	float		coordinateOffset_a;			// siehe Multipart 3D Standard
	float		coordinateOffset_b;			// siehe Multipart 3D Standard
	float		coordinateOffset_c;			// siehe Multipart 3D Standard
	uint32_t 	invalidDataValue;			//
	uint16_t 	outputMode;					// output Mode of the range data, possible values: UncalibratedC, CalibratedC, CalibratedAC, CalibratedAC_Linescan
	uint16_t	flags;						// Image Sum-Value
	uint32_t	reserved;
} CX_STRUCT_PACKED;

#define CX_C6_CHUNK_SCAN3D_REGION_INFO_SIZE 64


#if defined(__MINGW32__) || defined(__MINGW64__)
#pragma pack()
#endif
CX_STRUCT_PACKED_END


#ifndef SWIG
typedef struct CX_CHUNK_FRAME_INFO cx_chunk_frame_info_t;
typedef struct CX_CHUNK_CAMERA_INFO cx_chunk_camera_info_t;
typedef struct CX_CHUNK_CAMERA_INFO cx_chunk_profile_info_t;
typedef struct CX_CHUNK_IRSX_IMAGE_INFO cx_chunk_irsx_image_info_t;
typedef struct CX_CHUNK_C6_LINE_INFO cx_chunk_c6_line_info_t;
typedef struct CX_C6_CHUNK_FRAME_INFO cx_c6_chunk_frame_info_t;
typedef struct CX_C6_CHUNK_SCAN3D_REGION_INFO cx_c6_chunk_scan3d_region_info_t;

static_assert(sizeof(cx_chunk_frame_info_t) == CX_CHUNK_FRAME_INFO_SIZE, "wrong struct size, check struct packing and alignment");
static_assert(sizeof(cx_chunk_camera_info_t) == CX_CHUNK_CAMERA_INFO_SIZE, "wrong struct size, check struct packing and alignment");
static_assert(sizeof(cx_chunk_irsx_image_info_t) == CX_CHUNK_IRSX_IMAGE_INFO_SIZE, "wrong struct size, check struct packing and alignment");
static_assert(sizeof(cx_chunk_c6_line_info_t) == CX_CHUNK_C6_LINE_INFO_SIZE, "wrong struct size, check struct packing and alignment");
static_assert(sizeof(cx_c6_chunk_frame_info_t) == CX_C6_CHUNK_FRAME_INFO_SIZE, "wrong struct size, check struct packing and alignment");
static_assert(sizeof(cx_c6_chunk_scan3d_region_info_t) == CX_C6_CHUNK_SCAN3D_REGION_INFO_SIZE, "wrong struct size, check struct packing and alignment");
#endif	/* SWIG */


#ifdef __cplusplus
}
#endif
#endif /* CX_CHUNK_DATA_SRUCTS_H_INCLUDED */
