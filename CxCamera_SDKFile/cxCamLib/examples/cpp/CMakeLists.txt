# file: CMakeLists.txt
# package: cxCamLib
# brief: CMake script to create the cxCamLib examples.
# copyright (c) 2018, Automation Technology GmbH

cmake_minimum_required (VERSION 2.8)

string(REPLACE "\\" "/" CX_SDK_ROOT_ENV $ENV{CX_SDK_ROOT})
set(CX_SDK_ROOT ${CX_SDK_ROOT_ENV} CACHE PATH "Path to cxSDK root.")

# Test if CMake project is built as stand-alone or if it is included from another top level CMake project.
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
	project(CxCamLib_Examples CXX)
	
    if(CMAKE_SYSTEM_NAME STREQUAL Linux)
        # use, i.e. don't skip the full RPATH for the build tree
        SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

        # when building, don't use the install RPATH already
        # (but later on when installing)
        SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

        SET(CMAKE_INSTALL_RPATH "/opt/cxSupportPackage/cxSDK/cxBaseLib/lib/:/opt/cxSupportPackage/cxSDK/cxCamLib/lib/:/opt/cxSupportPackage/cxSDK/ThirdParty/lib/:/opt/cxSupportPackage/cxSDK/ThirdParty/opencv-3.4.2/${AT_PLATFORM_SUBPATH}_shared_vtk_static/lib/")

        # add the automatically determined parts of the RPATH
        # which point to directories outside the build tree to the install RPATH
        SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
    endif()
	
	set(CMAKE_PREFIX_PATH "${CX_SDK_ROOT}/ThirdParty/libs/opencv-3.4.2/build_win_vc120_64_static;${CX_SDK_ROOT}/cxBaseLib/build_win_vc120_64;${CX_SDK_ROOT}/cxCamLib/build_win_vc120_64;${AT_THIRDPARTY_DIR}/Poco-1.9.0/build_win_vc120_64_static" CACHE STRING "Path used for searching by FIND_XXX(), with appropriate suffixes added.")
	set(AT_BUILD_CX_CAM_API_TEST OFF CACHE BOOL "Build AT internal examples.")
	set(USE_OPENCV ON CACHE BOOL "Build examples with OpenCV support for image display.")
	
	# prerequisite CxBaseLib sub-modules
	find_package(CxBaseLib REQUIRED)
	find_package(CxCamLib REQUIRED)
	
	get_target_property(PROP_VAL AT::CxBaseLib IMPORTED_IMPLIB_RELEASE)
	message("IMPORTED_IMPLIB_RELEASE : ${PROP_VAL}")
	
	get_target_property(PROP_VAL AT::CxBaseLib IMPORTED_IMPLIB_DEBUG)
	message("IMPORTED_IMPLIB_DEBUG : ${PROP_VAL}")
	
	get_target_property(PROP_VAL AT::CxBaseLib IMPORTED_IMPLIB)
	message("IMPORTED_IMPLIB : ${PROP_VAL}")
	
	if(USE_OPENCV)
		# prerequisite Thirdparty libs
		find_package(OpenCV REQUIRED COMPONENTS core highgui imgcodecs imgproc)
	endif()
	
	if(AT_BUILD_CX_CAM_API_TEST)
		find_package(Poco REQUIRED COMPONENTS Foundation)
	endif()
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
			${CX_SDK_ROOT}/cxCamLib/wrapper/cpp/include/AT/cx/Device.h
			${CX_SDK_ROOT}/cxCamLib/wrapper/cpp/include/AT/cx/DeviceBuffer.h
			${CX_SDK_ROOT}/cxCamLib/wrapper/cpp/include/AT/cx/DeviceFactory.h
			${CX_SDK_ROOT}/cxCamLib/wrapper/cpp/include/AT/cx/DeviceEvent.h
			${CX_SDK_ROOT}/cxCamLib/wrapper/cpp/include/AT/cx/DeviceInfo.h
			${CX_SDK_ROOT}/cxCamLib/wrapper/cpp/include/AT/cx/DevicePort.h
			${CMAKE_CURRENT_SOURCE_DIR}/cx_cam_common.h
		)

		set(EXAMPLE_SRC_FILES
			${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/cx_cam_common.cpp
		)
		
		if(USE_OPENCV)
			list(APPEND EXAMPLE_INCLUDE_FILES 
				${CX_SDK_ROOT}/cxBaseLib/wrapper/cpp/include/AT/cx/CVUtils.h
			)
		else()
			list(APPEND EXAMPLE_INCLUDE_FILES 
				${CMAKE_CURRENT_SOURCE_DIR}/cli_utils.h
			)
			list(APPEND EXAMPLE_SRC_FILES 
				${CMAKE_CURRENT_SOURCE_DIR}/cli_utils.cpp
			)
		endif()

		# Add an executable to the project using the specified source files.
		add_executable(${TARGET_NAME}
			${EXAMPLE_INCLUDE_FILES}
			${EXAMPLE_SRC_FILES}
		) 
		
		# Specify the project's subfolder (filter) in the IDE (unfortunately this affects only the VisualStudio project)
		set_target_properties (${TARGET_NAME} PROPERTIES FOLDER CxCamLib/examples)
        if(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
            set_target_properties(${TARGET_NAME} PROPERTIES LINK_FLAGS "-Wl,--allow-shlib-undefined")
        endif()

		# Specify libraries or flags to use when linking a given target and/or its dependents.
		target_link_libraries(${TARGET_NAME}
			PUBLIC AT::CxCamLib
			PUBLIC AT::CxBaseLib
		)
		if(USE_OPENCV)
			target_link_libraries(${TARGET_NAME}
				PUBLIC opencv_core
				PUBLIC opencv_highgui
				PUBLIC opencv_imgcodecs
				PUBLIC opencv_imgproc
			)
		endif()

		# Specify compiler definitions and flags valid for this target.
		target_compile_definitions(${TARGET_NAME} PRIVATE -D_CONSOLE -DAT_NO_AUTOMATIC_LIBS)
		if(USE_OPENCV)
			target_compile_definitions(${TARGET_NAME} PRIVATE -DUSE_OPENCV)
		endif()
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

add_example(cx_cam_enumerate_nodemap)
add_example(cx_cam_grab_continuous)
add_example(cx_cam_grab_event)
add_example(cx_cam_nodemap_param)
add_example(cx_cam_snap_image)
add_example(cx_cam_qos_params)
add_example(ir_snap_temperature_image)
