# file: CMakeLists.txt
# package: AT-Framework
# brief: CMake script to create the Cx3dLib examples.
# copyright (c) 2018, Automation Technology GmbH

cmake_minimum_required (VERSION 2.8)

string(REPLACE "\\" "/" CX_SDK_ROOT_ENV $ENV{CX_SDK_ROOT})
set(CX_SDK_ROOT ${CX_SDK_ROOT_ENV} CACHE PATH "Path to cxSDK root.")

# Test if CMake project is built as stand-alone or if it is included from another top level CMake project.
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
	project(Cx3dLib_Examples CXX)
	
    if(CMAKE_SYSTEM_NAME STREQUAL Linux)
        # use, i.e. don't skip the full RPATH for the build tree
        SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

        # when building, don't use the install RPATH already
        # (but later on when installing)
        SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

        SET(CMAKE_INSTALL_RPATH "/opt/cxSupportPackage/cxSDK/cxBaseLib/lib/:/opt/cxSupportPackage/cxSDK/cxCamLib/lib/:/opt/cxSupportPackage/cxSDK/cx3dLib/lib/:/opt/cxSupportPackage/cxSDK/ThirdParty/lib/:/opt/cxSupportPackage/cxSDK/ThirdParty/opencv-3.4.2/${AT_PLATFORM_SUBPATH}_shared_vtk_static/lib/")

        # add the automatically determined parts of the RPATH
        # which point to directories outside the build tree to the install RPATH
        SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
    endif()
	
	set(CMAKE_PREFIX_PATH "${CX_SDK_ROOT}/ThirdParty/opencv-3.4.2/build_win_vc140_64_shared_vtk_static;${CX_SDK_ROOT}/cxBaseLib/lib/build_win_vc140_64;${CX_SDK_ROOT}/cxCamLib/lib/build_win_vc140_64;${CX_SDK_ROOT}/cx3dLib/lib/build_win_vc140_64" CACHE STRING "Path used for searching by FIND_XXX(), with appropriate suffixes added.")
	set(AT_BUILD_CX_3D_API_TEST OFF CACHE BOOL "Build AT internal examples.")
	
	# prerequisite CxBaseLib sub-modules
	find_package(CxBaseLib REQUIRED)
	find_package(CxCamLib REQUIRED)
	find_package(Cx3dLib REQUIRED)
	
	# prerequisite Thirdparty libs
	find_package(OpenCV REQUIRED COMPONENTS opencv_core opencv_highgui opencv_imgcodecs opencv_imgproc opencv_features2d opencv_viz)
endif()

function(add_example TARGET_NAME)
	if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}.cpp)
		# Define variables of type "list of strings" containing the (absolute) paths of all header- and source-files belonging to the project.
		# Recommendation: According to the logical grouping of source files in the project, for each group one variable is defined.
		set(EXAMPLE_INCLUDE_FILES
			${CX_SDK_ROOT}/cxBaseLib/wrapper/cpp/include/AT/cx/base.h
			${CX_SDK_ROOT}/cxBaseLib/wrapper/cpp/include/AT/cx/exception.h
			${CX_SDK_ROOT}/cxBaseLib/wrapper/cpp/include/AT/cx/Variant.h
			${CX_SDK_ROOT}/cxBaseLib/wrapper/cpp/include/AT/cx/Image.h
			${CX_SDK_ROOT}/cxBaseLib/wrapper/cpp/include/AT/cx/Chunk.h
			${CX_SDK_ROOT}/cxBaseLib/wrapper/cpp/include/AT/cx/Point3.h
			${CX_SDK_ROOT}/cx3dLib/wrapper/cpp/include/AT/cx/c3d/Calib.h
			${CX_SDK_ROOT}/cx3dLib/wrapper/cpp/include/AT/cx/c3d/PointCloud.h
			${CX_SDK_ROOT}/cx3dLib/wrapper/cpp/include/AT/cx/c3d/ZMap.h
			${CMAKE_CURRENT_SOURCE_DIR}/cx_3d_common.h
		)

		set(EXAMPLE_SRC_FILES
			${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/cx_3d_common.cpp
		)

		# Add an executable to the project using the specified source files.
		add_executable(${TARGET_NAME}
			${EXAMPLE_INCLUDE_FILES}
			${EXAMPLE_SRC_FILES}
		) 

		# Specify the project's subfolder (filter) in the IDE (unfortunately this affects only the VisualStudio project)
		set_target_properties (${TARGET_NAME} PROPERTIES FOLDER Cx3dLib/examples)
		if(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
            set_target_properties(${TARGET_NAME} PROPERTIES LINK_FLAGS "-Wl,--allow-shlib-undefined")
        endif()
		
		
		# Specify libraries or flags to use when linking a given target and/or its dependents.
		target_link_libraries(${TARGET_NAME}
			PUBLIC AT::Cx3dLib
			PUBLIC AT::CxBaseLib
			PUBLIC opencv_core
			PUBLIC opencv_calib3d
			PUBLIC opencv_highgui
			PUBLIC opencv_imgcodecs
			PUBLIC opencv_imgproc
			PUBLIC opencv_features2d
			PUBLIC opencv_viz
		)

		# Specify compiler definitions and flags valid for this target.
		target_compile_definitions(${TARGET_NAME} PRIVATE -D_CONSOLE -DAT_NO_AUTOMATIC_LIBS)
		target_compile_options(${TARGET_NAME} PRIVATE ${AT_ADDITIONAL_COMPILER_FLAGS})

		# Define a grouping for source files in IDE project generation.
		source_group("Header Files" FILES ${EXAMPLE_INCLUDE_FILES})
		source_group("Source Files" FILES ${EXAMPLE_SRC_FILES})
		
		install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}.cpp 
				DESTINATION examples/cpp
				COMPONENT ${TARGET_NAME} 
			)
		
	endif()
endfunction()

function(add_example_cam TARGET_NAME)
	if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}.cpp)
		# Define variables of type "list of strings" containing the (absolute) paths of all header- and source-files belonging to the project.
		# Recommendation: According to the logical grouping of source files in the project, for each group one variable is defined.
		set(EXAMPLE_INCLUDE_FILES
			${CX_SDK_ROOT}/cxBaseLib/wrapper/cpp/include/AT/cx/base.h
			${CX_SDK_ROOT}/cxBaseLib/wrapper/cpp/include/AT/cx/exception.h
			${CX_SDK_ROOT}/cxBaseLib/wrapper/cpp/include/AT/cx/Variant.h
			${CX_SDK_ROOT}/cxBaseLib/wrapper/cpp/include/AT/cx/Image.h
			${CX_SDK_ROOT}/cxBaseLib/wrapper/cpp/include/AT/cx/Chunk.h
			${CX_SDK_ROOT}/cxBaseLib/wrapper/cpp/include/AT/cx/Point3.h
			${CX_SDK_ROOT}/cx3dLib/wrapper/cpp/include/AT/cx/c3d/Calib.h
			${CX_SDK_ROOT}/cx3dLib/wrapper/cpp/include/AT/cx/c3d/PointCloud.h
			${CX_SDK_ROOT}/cx3dLib/wrapper/cpp/include/AT/cx/c3d/ZMap.h
			${CMAKE_CURRENT_SOURCE_DIR}/cx_3d_common.h
			${CX_CAM_ROOT}/cxCamLib/include/cx_cam.h
			${CX_CAM_ROOT}/cxCamLib/include/cx_cam_param.h
			${CX_CAM_ROOT}/cxCamLib/wrapper/cpp/include/AT/cx/Device.h
			${CX_CAM_ROOT}/cxCamLib/wrapper/cpp/include/AT/cx/DeviceBuffer.h
			${CX_CAM_ROOT}/cxCamLib/wrapper/cpp/include/AT/cx/DeviceEvent.h
			${CX_CAM_ROOT}/cxCamLib/wrapper/cpp/include/AT/cx/DeviceFactory.h
			${CX_CAM_ROOT}/cxCamLib/wrapper/cpp/include/AT/cx/DeviceInfo.h
			${CMAKE_CURRENT_SOURCE_DIR}/cx_cam_common.h
		)

		set(EXAMPLE_SRC_FILES
			${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/cx_3d_common.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/cx_3d_common2.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/cx_cam_common.cpp
		)

		# Add an executable to the project using the specified source files.
		add_executable(${TARGET_NAME}
			${EXAMPLE_INCLUDE_FILES}
			${EXAMPLE_SRC_FILES}
		) 


		# Specify the project's subfolder (filter) in the IDE (unfortunately this affects only the VisualStudio project)
		set_target_properties (${TARGET_NAME} PROPERTIES FOLDER Cx3dLib/examples)
		if(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
            set_target_properties(${TARGET_NAME} PROPERTIES LINK_FLAGS "-Wl,--allow-shlib-undefined")
        endif()

		# Specify libraries or flags to use when linking a given target and/or its dependents.
		target_link_libraries(${TARGET_NAME}
			PUBLIC AT::Cx3dLib
			PUBLIC AT::CxBaseLib
			PUBLIC AT::CxCamLib
			PUBLIC opencv_core
			PUBLIC opencv_highgui
			PUBLIC opencv_imgcodecs
			PUBLIC opencv_imgproc
			PUBLIC opencv_features2d
			PUBLIC opencv_viz
		)

		# Specify compiler definitions and flags valid for this target.
		target_compile_definitions(${TARGET_NAME} PRIVATE -D_CONSOLE -DAT_NO_AUTOMATIC_LIBS)
		target_compile_options(${TARGET_NAME} PRIVATE ${AT_ADDITIONAL_COMPILER_FLAGS})

		# Define a grouping for source files in IDE project generation.
		source_group("Header Files" FILES ${EXAMPLE_INCLUDE_FILES})
		source_group("Source Files" FILES ${EXAMPLE_SRC_FILES})
		
		install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}.cpp 
				DESTINATION examples/cpp
				COMPONENT ${TARGET_NAME} 
			)
		
	endif()
endfunction()

add_example_cam(c5_snap_multipart_pointcloud)
add_example_cam(c6_snap_multipart_pointcloud)
add_example_cam(cx_3d_snap_point_cloud)
add_example(cx_3d_calib_TargetFacets12)
add_example(cx_3d_calib_TargetSawtooth_multiple_RangeFiles)
add_example(cx_3d_calib_TargetSawtooth_single_RangeFile)
add_example(cx_3d_create_zMap)
add_example(cx_3d_show_point_cloud)
add_example_cam(cx_3d_grab_point_cloud_continuous)
