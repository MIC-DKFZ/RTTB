// -----------------------------------------------------------------------
// RTToolbox - DKFZ radiotherapy quantitative evaluation library
//
// Copyright (c) German Cancer Research Center (DKFZ),
// Software development for Integrated Diagnostics and Therapy (SIDT).
// ALL RIGHTS RESERVED.
// See rttbCopyright.txt or
// http://www.dkfz.de/en/sidt/projects/rttb/copyright.html
//
// This software is distributed WITHOUT ANY WARRANTY; without even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE.  See the above copyright notices for more information.
//
//------------------------------------------------------------------------

#include <iostream>

#include "VoxelizerToolHelper.h"
#include "VoxelizerToolCmdLineParser.h"
#include "VoxelizerToolApplicationData.h"

#include "rttbDoseLoader.cpp"
#include "rttbStructLoader.cpp"

int main(int argc, const char** argv)
{
    rttb::apps::voxelizerTool::ApplicationData appData;

	const std::string appCategory = "RT-Toolbox App";
	const std::string appName = "VoxelizerTool";
	const std::string appDesc = "An App to voxelize RT structures in a reference image.";
	const std::string appContributor = "SIDT@DKFZ";
	const std::string appVersion = RTTB_FULL_VERSION_STRING;

	boost::shared_ptr<rttb::apps::voxelizerTool::VoxelizerCmdLineParser> argParser;

	try
	{
		argParser = boost::make_shared<rttb::apps::voxelizerTool::VoxelizerCmdLineParser>(argc, argv, appName, appVersion,
		            appDesc, appContributor, appCategory);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 5;
	}

	// This is vital. The application needs to exit if the "help" or "version" parameter is set
	// because this means the other parameters won't be parsed.

	if (argParser->isSet(argParser->OPTION_HELP) || argParser->isSet(argParser->OPTION_VERSION)
	    || argParser->isSet(argParser->OPTION_XML))
	{
		return 0;
	}

	rttb::apps::voxelizerTool::populateAppData(argParser, appData);

	if (argParser->isSet(argParser->OPTION_ALL_STRUCTS))
	{
		appData._outputFilename = "output.nrrd";
		appData._regEx = ".*";
		appData._multipleStructs = true;
	}

	std::cout << std::endl << "*******************************************" << std::endl;
	std::cout << "Struct file: " << appData._structFile << std::endl;
	std::cout << "Reference Image: " << appData._referenceFile << std::endl;
	std::cout << "Output file: " << appData._outputFilename << std::endl;
	std::cout << "Struct regex: " << appData._regEx << std::endl;
	std::cout << "Add structures: " << appData._addStructures << std::endl;
    std::cout << "Multiple Struct: " << appData._multipleStructs << std::endl;
	std::cout << "Strict voxelization: " << !appData._noStrictVoxelization << std::endl << std::endl;

	std::cout << "reading reference and structure file..." << std::endl;

    try
    {
        appData._dose = rttb::io::utils::loadDose(appData._referenceFile, appData._referenceFileLoadStyle);
    }
    catch (rttb::core::Exception& e)
    {
        std::cerr << "RTTB Error!!!" << std::endl;
        std::cerr << e.what() << std::endl;
        return 2;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error!!!" << std::endl;
        std::cerr << e.what() << std::endl;
        return 2;
    }
    catch (...)
    {
        std::cerr << "Error!!! unknown error while reading input dose image." << std::endl;
        return 2;
    }

    try
    {
        appData._struct = rttb::io::utils::loadStruct(appData._structFile, appData._structFileLoadStyle, appData._regEx);
    }
    catch (rttb::core::Exception& e)
    {
        std::cerr << "RTTB Error!!!" << std::endl;
        std::cerr << e.what() << std::endl;
        return 2;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error!!!" << std::endl;
        std::cerr << e.what() << std::endl;
        return 2;
    }
    catch (...)
    {
        std::cerr << "Error!!! unknown error while reading input struct image." << std::endl;
        return 2;
    }

    std::cout << "done." << std::endl;

    try
    {
      rttb::apps::voxelizerTool::processData(appData);
    }
    catch (rttb::core::Exception& e)
    {
        std::cerr << "RTTB Error while doing voxelization!!!" << std::endl;
        std::cerr << e.what() << std::endl;
        return 2;
    }
    catch (itk::ExceptionObject& err)
    {
        std::cerr << "ExceptionObject caught !" << std::endl;
        std::cerr << err << std::endl;
        return 3;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error!!!" << std::endl;
        std::cerr << e.what() << std::endl;
        return 3;
    }
    catch (...)
    {
        std::cerr << "Error!!! unknown error while doing voxelization." << std::endl;
        return 3;
    }
	return 0;
}
