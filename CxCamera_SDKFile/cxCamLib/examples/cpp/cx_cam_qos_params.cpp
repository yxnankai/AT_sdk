/*! C++ examples for accessing different nodemaps
@file cx_cam_qos_params.cpp

This example demonstrates how to access different nodemaps using different QOS 
parameters, such as different DataStream statistics values, the package size
autonegotiation, the parameter "PassCorruptedFrames", etc. 
The example refers to the Stemmer TL, since these parameters are either not 
standardized, or the TL manufacturers only partially adhere to the GenTL standard.
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

@brief CPP example of using AT Camera library.
@copyright (c) 2022, Automation Technology GmbH.
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

		cx::Variant val;

		// enable Packet-Size Auto negotiation
		val = "Auto";
		cam->setParam("DeviceTL::PacketSizeMode", val);

		// enable "PassCorruptFrames"
		val = true;
		cam->setParam("DataStream::PassCorruptFrames", val);

		// 2. Allocate and queue internal acquisition buffers
		cam->allocAndQueueBuffers(3);

		// 3. Start image acquisition
		cam->startAcquisition();

		std::cout << "Press any key in the OpenCV window to exit ... \n";

		// 4. Acquisition loop

		size_t buffersDelivered = 0, buffersCorrupt = 0;
		while (1)
		{
			// 4. Grab acquisition buffer, wait for valid buffer with optional timeout. Timeout is given in ms.
			cx::DeviceBuffer buffer = cam->waitForBuffer(5000);
			
			cam->getParam("DataStream::NumBuffersDelivered", val);
			buffersDelivered = (size_t)val;

			cam->getParam("DataStream::NumBuffersCorrupt", val);
			buffersCorrupt = (size_t)val;

			std::cout << "NumBuffersDelivered: " << buffersDelivered << ", NumBuffersCorrupt: " << buffersCorrupt << std::endl;

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
