set SCRIPT_DIR=%cd%
set CX_SDK_ROOT=%CX_SDK_ROOT_64%
set THIRDPARTY_DIR=%CX_SDK_ROOT_64%/ThirdParty/
set CMAKE_GENERATOR=-G "Visual Studio 15 2017 Win64" -T v140 -DCMAKE_SYSTEM_VERSION=8.1

mkdir %SCRIPT_DIR%\build_win_vc140_64
cd %SCRIPT_DIR%\build_win_vc140_64
cmake %CMAKE_GENERATOR% -DCX_SDK_ROOT:PATH="%CX_SDK_ROOT_64%" -DCMAKE_PREFIX_PATH="%CX_SDK_ROOT_64%/cxBaseLib/lib/;%CX_SDK_ROOT_64%/cxCamLib/lib/;%CX_SDK_ROOT_64%/cx3dLib/lib/;%THIRDPARTY_DIR%/opencv-3.4.2/build_win_vc140_64_shared_vtk_static" ../
cmake --build . --config "Release"
cmake --build . --config "Debug" 

cd %SCRIPT_DIR%
