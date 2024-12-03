/** C++ example of continues image aquisition using cxCamLib.
\example cx_cam_grab_continuous.cpp

This example shows how to grab and process a continuous stream of images.
1. (Find and) connect a camera device.
2. Allocate and queue internal buffers.
3. Start acquisition.
4. LOOP
	1. Grab an image buffer.
	2. Get image from buffer (and do some processing on the image data, here display the data).
	3. Queue back the image buffer.
5. Stop acquisition.
6. Cleanup.

\note
This example uses OpenCV only for image display and can also be executed without the use of OpenCV. To remove the OpenCV dependencies, do the following:
- For CMake-based projects: Call CMake wit -DUSE_OPENCV:BOOL=OFF
- For VisualStudio-Projects: Open the project properties and remove "USE_OPENCV" from <em>C/C++->Preprocessor/PreprocessorDefinitions</em> for all configurations.
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

copyright (c) 2017, Automation Technology GmbH.
version 04.09.2017, AT: initial version
version 02.08.2018, AT: auto-scale image view to Mono8
*************************************************************************************/

#ifndef USE_OPENCV
#define USE_OPENCV
#endif

#include <assert.h>
#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;

#ifdef USE_OPENCV
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "AT/cx/CVUtils.h"
#if defined(_MSC_VER)
#ifdef _DEBUG
#pragma comment(lib,"opencv_highgui342d.lib")
#pragma comment(lib,"opencv_imgcodecs342d.lib")
#pragma comment(lib,"opencv_imgproc342d.lib")
#pragma comment(lib,"opencv_core342d.lib")
#else
#pragma comment(lib,"opencv_highgui342.lib")
#pragma comment(lib,"opencv_imgcodecs342.lib")
#pragma comment(lib,"opencv_imgproc342.lib")
#pragma comment(lib,"opencv_core342.lib")
#endif
#endif
using namespace cv;
#else
#include "cli_utils.h"
#endif

#include "cx_cam_common.h"
using namespace AT;


/** Image Processing
	We convert the acquired image into an OpenCV image,
	normalize the image to Mono8 and display it using OpenCV highgui function.
*/
void doProcessing(cx::ImagePtr img)
{
	static uint count = 0;
	double minVal, maxVal;
	imageMinMax(minVal, maxVal, img);
	std::cout << "Image#: " << count << ", pixel range is: " << minVal << " - " << maxVal << std::endl;
#ifdef USE_OPENCV
	cv::Mat imgmat = cx::cvUtils::imageCopyToMat(*img, false);		// imgmat is referencing data of img, the actual image data are not copied!
	cv::minMaxLoc(imgmat, &minVal, &maxVal);
	imgmat -= minVal;
	imgmat.convertTo(imgmat, CV_8U, 255.0 / (maxVal - minVal));
	cv::imshow("image", imgmat);
#endif
	count++;
}


int main(int argc, char* argv[])
{
	try
	{
		// 1. discover and connect a device.
		std::string uri;
		if (argc > 1)
			uri = argv[1];
		else
			uri = cx::discoverAndChooseDevice(true)->deviceURI;

		auto cam = cx::DeviceFactory::openDevice(uri);
		std::cout << "Open Device: " << uri.c_str() << endl;

		// 2. Allocate and queue internal acquisition buffers
		cam->allocAndQueueBuffers(3);

		// 3. Start image acquisition
		cam->startAcquisition();

		std::cout << "Press any key in the OpenCV window to exit ... \n";

		// 4. Acquisition loop
		while(1)
		{
				// 4. Grab acquisition buffer, wait for valid buffer with optional timeout. Timeout is given in ms.
				cx::DeviceBuffer buffer = cam->waitForBuffer(5000);
			
				// 5. get image data from buffer and do some processing on the image data (or get a copy for later use)
				// \note img holds a reference to the image data in the DeviceBuffer, if you need the image data after cx_queueBuffer you need to clone the image!
				{
					auto img = buffer.getImage();

					// do the actual image processing
					doProcessing(img);
				}

#ifdef USE_OPENCV
				if (cv::waitKey(1) != -1) 
					break;
#else
				if (_kbhit() > 0)
					break;
#endif

				// 6. Queue back the buffer to the devices acquisition engine.
				// \note From now on any references to the buffer images are not valid anymore and might be overwritten with new image data at any time!
				buffer.queueBuffer();
			}

		// 7. Stop acquisition
		cam->stopAcquisition();
		
		// 8. Cleanup
		cam->freeBuffers();
		cam->close();
	}
	catch (const cx::RuntimeError& err)
	{
		std::cerr << "cx runtime exception: " << err.what() << endl;
		exit(-3);
	}
	return 0;
}
