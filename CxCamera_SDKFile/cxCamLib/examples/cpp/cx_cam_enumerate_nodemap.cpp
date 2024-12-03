/*! C++ example that iterates the GenICam XML NodeMap tree
@file cx_cam_enumerate_nodemap.cpp

This example iterates the GenICam XML NodeMap tree of the selected device and 
writes the found parameters together with meta information to the file "nodemap.txt".
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
@version 04.09.2017, AT: initial version
@version 02.08.2018, AT: Bug-fix: node attribute -> not implemented feature
*************************************************************************************/
#include <assert.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <algorithm>
using namespace std;

#include "cx_cam_common.h"
using namespace AT;


void printParam(cx::DevicePtr dev, const std::string& name, std::ostream& outstr, size_t indentation_level=0)
{
	cx::Variant val, display_name, type, access, cat_children;

	display_name = (std::string)name;

	// get display name, type, visibility, access mode and the value of this node
	dev->getParamInfo(CX_PARAM_INFO_TYPE, name.c_str(), type);
	dev->getParamInfo(CX_PARAM_INFO_ACCESSS_MODE, name.c_str(), access);
	if ((int)access == CX_PARAM_ACCESS_RO || (int)access == CX_PARAM_ACCESS_RW)
		dev->getParamInfo(CX_PARAM_INFO_DISPLAYNAME, name.c_str(), display_name);
	if ((int)type == CX_PARAM_CATEGORY && ((int)access == CX_PARAM_ACCESS_RO || (int)access == CX_PARAM_ACCESS_RW))
		dev->getParamInfo(CX_PARAM_INFO_CATEGORY_CHILDS, name.c_str(), cat_children);
	if ((int)access == CX_PARAM_ACCESS_RO || (int)access == CX_PARAM_ACCESS_RW)
		dev->getParam(name.c_str(), val);

	std::string wsp = "                                                                                                    ";

	// write display name and value of the node
	outstr << wsp.substr(0, std::min(indentation_level, (size_t)60));
	outstr << (std::string)display_name;
	outstr << wsp.substr(0, std::max(60 - (indentation_level + ((std::string)display_name).length()), (size_t)0));

	ostringstream valuestr;

	switch (val.type)
	{
	case CX_VT_INT:
		valuestr << val.data.i;
		break;
	case CX_VT_REAL:
		valuestr << val.data.r;
		break;
	case CX_VT_STRING:
		valuestr << (char*)(val.data.a.buf);
		break;
	case CX_VT_EMPTY:
		valuestr << "-";
		break;
	default:
		valuestr << "<Array>";
	};

	outstr << valuestr.str() << wsp.substr(0, std::max(40 - valuestr.str().length(), (size_t)0));

	// write some meta-information of the node
	printParamInfo(dev, name, outstr);

	outstr << endl;

	// call this function recursive for category nodes
	if ((int)type == CX_PARAM_CATEGORY)
	{
		std::string childName = "";
		for (int i = 0; i < cat_children.data.a.len; i++)
		{
			char c;
			if ((c = ((char*)cat_children.data.a.buf)[i]) == '\0')
			{
				if (!childName.empty())
				{
					std::cout << "Node: " << childName << std::endl;
					printParam(dev, childName, outstr, indentation_level + 2);
				}
				childName = "";
			}
			else
				childName += c;
		}
	}
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

		// open output file stream
		std::ofstream ofs;
		ofs.open("nodemap.txt", std::ofstream::out | std::ofstream::trunc);
		
		// call tree iteration function with XML root-node
		printParam(cam, "Root", ofs);
		
		// close filestream
		ofs.close();
		
		//close device
		cam->close();
	}
	catch (const cx::RuntimeError& err)
	{
		std::cerr << "cx runtime exception: " << err.what() << endl;
		exit(-3);
	}
	return 0;
}
