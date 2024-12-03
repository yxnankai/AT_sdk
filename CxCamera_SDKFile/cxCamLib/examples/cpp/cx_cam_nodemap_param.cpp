/*! C++ example demonstrating parameter access.
@file cx_cam_nodemap_param.cpp

Demonstrates how to read and write parameters and how to get more detailed 
information about a parameter.
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

@copyright (c) 2017, Automation Technology GmbH.
@version 04.09.2017, initial version
@version 02.08.2018, add access mode check prior to call cx_setParam() 
*************************************************************************************/
#include <assert.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include<array>
using namespace std;

#include "cx_cam_common.h"
using namespace AT;


void printParam(cx::DevicePtr dev, const std::string& name, std::ostream& os)
{
	os << "Parameter: " << name << endl << "Info: ";
	cx::printParamInfo(dev, name, os);
	os << endl << endl;
}

int main(int argc, char* argv[])
{
	try
	{
		std::string uri;
		if (argc > 1)
			uri = argv[1];
		else
			uri = cx::discoverAndChooseDevice(true)->deviceURI;

		// connect the camera with given URI, you can also open the camera with a valid URI without the discovering step above
		// e.g. uri = "gev://169.254.239.221/?mac=00-50-C2-8E-DD-EE"
		auto cam = cx::DeviceFactory::openDevice(uri);
		std::cout << "Open Device: " << uri.c_str() << endl;

		// read and print some nodemap perameters
		// parameter of type Integer
		printParam(cam, "Width", std::cout);
		// parameter of type Float
		printParam(cam, "DeviceTemperature", std::cout);
		// parameter of type String
		printParam(cam, "DeviceVendorName", std::cout);
		// parameter of type Enum
		printParam(cam, "DeviceTemperatureSelector", std::cout);
		// parameter of type Command
		printParam(cam, "AcquisitionStart", std::cout);
		// parameter of type Category
		printParam(cam, "TransportLayerControl", std::cout);
		std::cout << endl;

		// write some nodemap parameters
		cx::Variant val;
		cam->getParamInfo(CX_PARAM_INFO_ACCESSS_MODE, "DeviceTemperatureSelector", val);
		if ((int)val == CX_PARAM_ACCESS_RW)		// check if node is writable
			cam->setParam("DeviceTemperatureSelector", "Mainboard");

		// Cleanup
		cam->close();
	}
	catch (const cx::RuntimeError& err)
	{
		std::cerr << "cx runtime exception: " << err.what() << endl;
		exit(-3);
	}
	return 0;
}

