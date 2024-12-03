/*! C++ example of using cxCamLib for the acquisition of temperature images.
@file ir_snap_temperature_image.cpp

This example shows how to snap temperature images from a calibrated IRSX Camera:
- Snap image with radiometric pixel format "Temperature Linear 0.4"
- Snap image with radiometric pixel format "Flux Linear" and calculate a
	temperature image by applying the R,B,F,O values read from the camera
	to the signal values S using the RBFO-Model:
	T = B / log(R / (S - O) + F) [Kelvin].

\note This example uses OpenCV only for image display and can also be executed without the use of OpenCV. To remove the OpenCV dependencies, do the following:
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

@copyright (c) 2018, Automation Technology GmbH.
@version 25.05.2018, initial version
*************************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
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

#define RADIOMETRIC_PIXELFORMAT_FLUX_LINEAR  "Flux_linear"
#define RADIOMETRIC_PIXELFORMAT_T_LINEAR_0_4 "T_linear_0_4"
#define RADIOMETRIC_PIXELFORMAT_T_LINEAR_0_04 "T_linear_0_04"
#define RADIOMETRIC_PIXELFORMAT_T_LINEAR_0_01 "T_linear_0_01"

/**
	Calculate the temperature T in Kelvin from the given signal S by applying this formula:
		T = B / log(R / (S - O) + F) [Kelvin],
	where R,B,F,O are the Planck constants taken from the calibrated camera.
*/
class Tcalc
{
public:
	#define ASY_SAFEGUARD 1.0002

	double R;	//!< calibration parameter R
	double B;	//!< calibration parameter B
	double F;	//!< calibration parameter F
	double O;	//!< calibration parameter O
	double e;	//!< emmisivity

	// temperature conversion
	double calculateTemperature(double S) const
	{
		// object signal
		double dObjSig = S/e - O;
		if (dObjSig > 0.0)
		{
			double dbl_reg = R / dObjSig + F;
			if (F <= 1.0)
			{
				if (dbl_reg < ASY_SAFEGUARD)
					dbl_reg = ASY_SAFEGUARD; // Don't get above a R/(1-F) (horizontal) asymptote
			}
			else
			{	// if (mCalibPar.F > 1.0)
				double tmp = F * ASY_SAFEGUARD;
				if (dbl_reg < tmp)
					dbl_reg = tmp;			// Don't get too close to a B/ln(F) (vertical) asymptote
			}
			return B / log(dbl_reg);
		}
		return 0.0;
	}
};

/** Image Processing
	We convert the acquired image into an OpenCV image,
	normalize the image to Mono8 and display it using OpenCV highgui function.
*/
void doProcessingLinearTemperature(cx::ImagePtr img, double linK = 0.4)
{
	double minVal, maxVal;
	imageMinMax(minVal, maxVal, img);
	std::cout << "Temperature range is:" << minVal * linK - 273.15 << "°C - " << maxVal * linK - 273.15 << "°C\n";
#ifdef USE_OPENCV
	cv::Mat imgmat = cx::cvUtils::imageCopyToMat(*img, false);		// imgmat is referencing data of img, the actual image data are not copied!
	imgmat -= minVal;
	imgmat.convertTo(imgmat, CV_8U, 255.0 / (maxVal - minVal));
	cv::imshow("image", imgmat);
#endif
}

void doProcessingTemperatureConversion(cx::ImagePtr img, const Tcalc& t)
{
	double minVal, maxVal, minTemp, maxTemp;
	imageMinMax(minVal, maxVal, img);
	minTemp = t.calculateTemperature(minVal) - 273.15;
	maxTemp = t.calculateTemperature(maxVal) - 273.15;
	std::cout << "Temperature range is:" << minTemp << "°C - " << maxTemp << "°C\n";
#ifdef USE_OPENCV
	cv::Mat imgmat = cx::cvUtils::imageCopyToMat(*img, false);		// imgmat is referencing data of img, the actual image data are not copied!
	cv::Mat tempmat(img->height(), img->width(), CV_64F);
	for (int row = 0; row < tempmat.rows; row++)
		for (int col = 0; col < tempmat.cols; col++)
			tempmat.at<double>(row, col) = t.calculateTemperature(imgmat.at<uint16_t>(row, col)) - 273.15;

	// normalize tempmat for display
	tempmat = (tempmat - minTemp) / (maxTemp - minTemp);
	cv::imshow("image", tempmat);
#endif
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
		cam->allocAndQueueBuffers();

		cx::Variant val;

		/************************************************************************/
		/* I) Snap image with radiometric pixel format "Temperature Linear 0.4" */
		/************************************************************************/
		// 2.a Check and switch Radiometric Pixel Format to "Temperature Linear 0.4"
		cam->getParam("RadiometricPixelFormat", val);
		std::string pixelFormat = val;
		std::cout << "Current Radiometric Pixel Format is: " << pixelFormat << "." << std::endl;
		double linK = (pixelFormat == RADIOMETRIC_PIXELFORMAT_T_LINEAR_0_4) ? 0.4 : 0.04;

		// wait for camera to switch and do internal updates
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		// 3. Start image acquisition
		cam->startAcquisition();

		// 4. Grab acquisition buffer, wait for valid buffer with optional timeout. Timeout is given in ms.
		cx::DeviceBuffer buffer = cam->waitForBuffer(2000);

		// 5. get image data from buffer and do some processing on the image data (or get a copy for later use)
		// \note img holds a reference to the image data in the DeviceBuffer, if you need the image data after cx_queueBuffer you need to clone the image!
		{
			auto img = buffer.getImage();

			// do the actual image processing
			doProcessingLinearTemperature(img, linK);
		}

		// 6. Queue back the buffer to the devices acquisition engine.
		// \note From now on any references to the buffer images are not valid anymore and might be overwritten with new image data at any time!
		buffer.queueBuffer();

		// 7. Stop acquisition
		cam->stopAcquisition();

#ifdef USE_OPENCV
		std::cout << "press 'q' in image window to close application" << endl;
		cv::waitKey(0);
#else
		std::cout << "press any key in the terminal window to close application" << endl;
		_getch();
#endif

		/*****************************************************************************************************************************/
		/* II) Snap image with radiometric pixel format "Flux Linear" and calculate temperature image by applying R,B,F,O Parameters */
		/*****************************************************************************************************************************/
		// 2.a Switch Radiometric Pixel Format to "Flux Linear" and get RBFO Parameters
		std::cout << "Switching to: Flux Linear.\n";
		cam->setParam("RadiometricPixelFormat", RADIOMETRIC_PIXELFORMAT_FLUX_LINEAR);
		// wait for camera to switch and do internal updates
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		Tcalc tcal;
		cam->getParam("Cust::R", val);
		tcal.R = val;
		cam->getParam("Cust::B", val);
		tcal.B = val;
		cam->getParam("Cust::F", val);
		tcal.F = val;
		cam->getParam("Cust::O", val);
		tcal.O = val;
		cam->getParam("ObjectEmissivity", val);
		tcal.e = val;
		std::cout << "Calibration parameters from Camera: R: " << tcal.R << ", B: " << tcal.B << ", F: " << tcal.F << ", O: " << tcal.O << ", e: " << tcal.e << std::endl;
		
		// 3. Start image acquisition
		cam->startAcquisition();

		// 4. Grab acquisition buffer, wait for valid buffer with optional timeout. Timeout is given in ms.
		buffer = cam->waitForBuffer(1000);

		// 5. get image data from buffer and do some processing on the image data (or get a copy for later use)
		// \note img holds a reference to the image data in the DeviceBuffer, if you need the image data after cx_queueBuffer you need to clone the image!
		{
			auto img = buffer.getImage();
			doProcessingTemperatureConversion(img, tcal);
		}
		// 6. Queue back the buffer to the devices acquisition engine.
		// \note From now on any references to the buffer images are not valid anymore and might be overwritten with new image data at any time!
		buffer.queueBuffer();

		// 7. Stop acquisition
		cam->stopAcquisition();

		// 8. Cleanup
		cam->freeBuffers();
		cam->close();

#ifdef USE_OPENCV
		std::cout << "press 'q' in image window to close application" << endl;
		cv::waitKey(0);
#else
		std::cout << "press any key in the terminal window to close application" << endl;
		_getch();
#endif
	}
	catch (const cx::RuntimeError& err)
	{
		std::cerr << "cx runtime exception: " << err.what() << endl;
		exit(-3);
	}
	return 0;
}
