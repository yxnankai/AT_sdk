/**
@file : cx_3d_pointcloud.h
@package : cx_3d library
@brief C-interface for AT 3D library point cloud file I/O functions.
@copyright (c) 2021, Automation Technology GmbH.
@version 20.12.2021, AT: initial version
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
#ifndef CX_3D_POINTCLOUD_H_INCLUDED
#define CX_3D_POINTCLOUD_H_INCLUDED

/**
@defgroup cx_3d_pointcloud	PointCloud file I/O functions
*/

#include "cx_3d_lib.h"

#ifdef __cplusplus
extern "C" {
#endif

CX_3D_API cx_status_t __cdecl cx_3d_pointcloud_load(const char* fileName, cx_img_t* points, cx_img_t* colors, cx_img_t* normals);

CX_3D_API cx_status_t __cdecl cx_3d_pointcloud_save(const char* fileName, cx_img_t* points, cx_img_t* colors, cx_img_t* normals, bool binary);

//! @} cx_3d_pointcloud

#ifdef __cplusplus
}
#endif

#endif /* CX_3D_POINTCLOUD_H_INCLUDED */
