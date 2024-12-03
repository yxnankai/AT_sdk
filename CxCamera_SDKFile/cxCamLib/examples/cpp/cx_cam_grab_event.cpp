/**
@package CxCamLib library
@file cx_cam_grab_event.cpp
@brief CPP example of using AT Camera library.
@copyright (c) 2017, Automation Technology GmbH.
@version 04.09.2017, AT: initial version
@version 02.08.2018, AT: auto-scale image view to Mono8
@version 31.12.2021, AT: refactored to use the C++ wrapper classes

This example shows the registration and handling of different event-types provided by the cxCamLib.
1. (Find and) connect a camera device.
2. Enable some GEV events on the device.
3. Register events to be fired by the cxCamLib.
3. Allocate and queue internal buffers.
4. Start acquisition.
5. Grab acquisition buffer, wait for valid buffer with optional timeout. Timeout is given in ms.
6. Get image data from buffer and do some processing on the image data or get a copy for later use.
	\note img holds a reference to the image data in the DeviceBuffer, if you need the image data after cx_queueBuffer you need to copy it!
7. Queue back the buffer to the devices acquisition engine.
	\note From now on img is not valid anymore and might be overwritten with new image data at any time!
8. Stop acquisition.
9. Cleanup.

/note{OpenCV dependencies}
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
*/

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
#include "AT/cx/DeviceEvent.h"
using namespace AT;


void OnDeviceEvent(CX_DEVICE_HANDLE hDevice, const char* ev_name, void* userParam, CX_EVENT_DATA_HANDLE hEventData)
{
	// event handler for device events
	cout << endl << "DeviceEvent: " << ev_name << endl;
}

#define ACQUISITION_START_NODE			"AcquisitionStart"
#define EVENT_ACQUISITION_START_ID		"9012"				//event id of "EventAcquisitionStart "
#define EVENT_LOG_MESSAGE_DATA_ID		"9300"				//event id of "EventLogData"

void OnEvent(CX_DEVICE_HANDLE hDevice, const char* name, void* userParam, CX_EVENT_DATA_HANDLE hEventData)
{
	// event handler for gev and node events
	cx::Variant val;
	std::string eventName = name;


	if (eventName == EVENT_ACQUISITION_START_ID || eventName == CX_CAM_EVENT_ACQUISITION_START)
	{
		cout << "GevEvent: " << eventName;

		// handle gev event		
		if (cx_getEventData(hDevice, hEventData, CX_CAM_EVENT_DATA_TIMESTAMP, &val) == CX_STATUS_OK)
		{
			cout << " " << CX_CAM_EVENT_DATA_TIMESTAMP << ": " << val.data.i;
		}

		cout << endl;
	}
	else if (eventName == ACQUISITION_START_NODE)
	{
		// handle node event	
		cout << "NodeEvent: " << ACQUISITION_START_NODE;

		if (cx_getEventData(hDevice, hEventData, CX_CAM_EVENT_DATA_TIMESTAMP, &val) == CX_STATUS_OK)
		{
			cout << " " << CX_CAM_EVENT_DATA_TIMESTAMP << ": " << val.data.i;
		}

		cout << endl;
	}
	else if (eventName == CX_CAM_EVENT_LOG_MESSAGE)
	{
		cout << "LogMessage:";
		// handle log message event		
		if (cx_getEventData(hDevice, hEventData, CX_CAM_EVENT_DATA_TIMESTAMP, &val) == CX_STATUS_OK)
		{
			cout << " " << CX_CAM_EVENT_DATA_TIMESTAMP << ": " << val.data.i;
		}
		if (cx_getEventData(hDevice, hEventData, CX_CAM_LOG_EVENT_DATA_LEVEL, &val) == CX_STATUS_OK)
		{
			cout << " " << CX_CAM_LOG_EVENT_DATA_LEVEL << ": " << val.data.i;
		}
		if (cx_getEventData(hDevice, hEventData, CX_CAM_LOG_EVENT_DATA_TEXT, &val) == CX_STATUS_OK)
		{
			cout << " " << CX_CAM_LOG_EVENT_DATA_TEXT << ": " << (std::string)val;
		}
		cout << endl;
	}
}

/** Image Processing
	We convert the acquired image into an OpenCV image,
	normalize the image to Mono8 and display it using OpenCV highgui function.
*/
void doProcessing(cx::ImagePtr img)
{
	double minVal, maxVal;
	imageMinMax(minVal, maxVal, img);
	std::cout << "Pixel range is: " << minVal << " - " << maxVal << std::endl;
#ifdef USE_OPENCV
	cv::Mat imgmat = cx::cvUtils::imageCopyToMat(*img, false);		// imgmat is referencing data of img, the actual image data are not copied!
	cv::minMaxLoc(imgmat, &minVal, &maxVal);
	imgmat -= minVal;
	imgmat.convertTo(imgmat, CV_8U, 255.0 / (maxVal - minVal));
	cv::imshow("image", imgmat);
#endif
}

int main(int argc, char* argv[])
{
	// 1. (Find and)connect a camera device.
	std::string uri;

	if (argc > 1)
		uri = argv[1];
	else
		uri = cx::discoverAndChooseDevice(true)->deviceURI;

	// connect the camera with given URI, you can also open the camera with a valid URI without the discovering step above
	// e.g. uri = "gev://169.254.239.221/?mac=00-50-C2-8E-DD-EE"
	// append the following string to use e.g. the MatrixVision TL instead of the Stemmer TL
	//
	uri += "&lib=mvGenTLProducer.cti";
	auto cam = cx::DeviceFactory::openDevice(uri);
	std::cout << "Open Device: " << uri.c_str() << endl;

	cx::Variant val;
	// 2. Enable some GEV events on the device (enable "AcquisitionStart" and "LogMessage" events)
	val = "AcquisitionStart"; // select "AcquisitionStart" event
	cam->setParam("EventSelector", val);
	val = 1; // switch event notification on
	cam->setParam("EventNotification", val);
	
	try
	{
		val = "LogMessage"; // select "LogMessage" event
		cam->setParam("EventSelector", val);
		val = 1; // switch event notification on
		cam->setParam("EventNotification", val);
	}
	catch (...)
	{
		std::cout << "No event named \"LogMessage\" available in XML.";
	}
	
	try
	{
		val = false; // switch off GEV Multipart
		cam->setParam("GevSCCFGMultiPart", val);
	}
	catch (...) {}

	{
		// 3. Register events to be fired by the cxCamLib.

		//register the device event connection lost
		cx::DeviceEvent connectionLostEvent(cam, CX_CAM_EVENT_CONNECTION_LOST, OnDeviceEvent, NULL);
		//register the device event connection restored
		cx::DeviceEvent connectionRestoredEvent(cam, CX_CAM_EVENT_CONNECTION_RESTORED, OnDeviceEvent, NULL);
		
		// register gev event callback by name
		cx::DeviceEvent acquisitionStartGevEvent(cam, CX_CAM_EVENT_ACQUISITION_START, OnEvent, NULL);
		// register gev event callback by id
		cx::DeviceEvent acquisitionStartGevIdEvent(cam, EVENT_ACQUISITION_START_ID, OnEvent, NULL);
		
		// register node event callback
		cx::DeviceEvent acquisitionStartNodeEvent(cam, ACQUISITION_START_NODE, OnEvent, NULL);

		// register log event callback
		cx::DeviceEvent logEvent(cam, CX_CAM_EVENT_LOG_MESSAGE, OnEvent, NULL);

		// 3. Allocate and queue internal buffers.
		cam->allocAndQueueBuffers(3);

		// 4. Start acquisition.
		cam->startAcquisition();

		// 5. Grab acquisition buffer, wait for valid buffer with optional timeout. Timeout is given in ms.
		cx::DeviceBuffer buffer = cam->waitForBuffer(5000);

		// 6. Get image data from buffer and do some processing on the image data or get a copy for later use.
		// \note img holds a reference to the image data in the DeviceBuffer, if you need the image data after cx_queueBuffer you need to copy it!
		auto img = buffer.getImage();

		// do the actual image processing
		doProcessing(img);

		// 7. Queue back the buffer to the devices acquisition engine.
		//	\note From now on img is not valid anymore and might be overwritten with new image data at any time!
		buffer.queueBuffer();

		// 8. Stop acquisition.
		cam->stopAcquisition();
	
#ifdef USE_OPENCV
		cv::waitKey(0);
#else
		_getch();
#endif
	} // note: the events will become unregistered when leaving this scope

	// 9. Cleanup
	cam->freeBuffers();
	cam->close();

	return 0;
}
