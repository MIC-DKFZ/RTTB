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
/*!
// @file
// @version $Revision: 1315 $ (last changed revision)
// @date    $Date: 2016-04-12 14:18:24 +0200 (Di, 12 Apr 2016) $ (last change date)
// @author  $Author: floca $ (last changed by)
*/

#include "DoseToolApplicationData.h"
#include "DoseToolHelper.h"
#include "DoseToolCmdLineParser.h"

#include "boost/shared_ptr.hpp"
#include "boost/make_shared.hpp"

#include "RTToolboxConfigure.h"

#include "rttbException.h"

/*! \file DoseTool.cpp main function for DoseTool 
*/
int main(int argc, const char** argv)
{
	int result = 0;

    rttb::apps::doseTool::ApplicationData appData;
	boost::shared_ptr<rttb::apps::doseTool::DoseToolCmdLineParser> argParser;

	const std::string appCategory = "RT-Toolbox App";
	const std::string appName = "DoseTool";
	const std::string appDesc = "An App to calculate the dose statistics and compute the DVH. This app is currently under development and in an experimental state.";
	const std::string appContributor = "SIDT@DKFZ";
	const std::string appVersion = RTTB_FULL_VERSION_STRING;

	try
	{
		argParser = boost::make_shared<rttb::apps::doseTool::DoseToolCmdLineParser>(argc, argv, appName,
		            appVersion, appDesc, appContributor, appCategory);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 5;
	}

	// This is vital. The application needs to exit if the "help" or "version" parameter is set
	// because this means the other parameters won't be parsed.

	if (argParser->isSet(argParser->OPTION_HELP) || argParser->isSet(argParser->OPTION_VERSION) || argParser->isSet(argParser->OPTION_XML))
	{
		return 0;
	}

	rttb::apps::doseTool::populateAppData(argParser, appData);

	std::cout << std::endl << "*******************************************" << std::endl;
	std::cout << "Dose file:   " << appData._doseFileName << std::endl;
	std::cout << "Struct file: " << appData._structFileName << std::endl;
	std::cout << "Struct name: " << appData._structNameRegex << std::endl;

	if (appData._computeDoseStatistics)
	{
		std::cout << "Dose statistic output file: " << appData._doseStatisticOutputFileName << std::endl;
		std::cout << "Compute complex statistics: " << appData._computeComplexDoseStatistics << std::endl;

		if (appData._computeComplexDoseStatistics)
		{
			std::cout << "Prescribed dose: " << appData._prescribedDose << std::endl;
		}

		std::cout << "Allow self intersections: " << appData._allowSelfIntersection << std::endl;
	}

	if (appData._computeDVH)
	{
		std::cout << "DVH output file: " << appData._dvhOutputFilename << std::endl;
	}

	try
	{
		appData._dose = rttb::apps::doseTool::loadDose(appData._doseFileName, appData._doseLoadStyle);
	}
	catch (rttb::core::Exception& e)
	{
		std::cerr << "RTTB Error!!!" << std::endl;
		std::cerr << e.what() << std::endl;
		return 1;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error!!!" << std::endl;
		std::cerr << e.what() << std::endl;
		return 1;
	}
	catch (...)
	{
		std::cerr << "Error!!! unknown error while reading dose image." << std::endl;
		return 1;
	}

	//loading of structure file not necessary in ITK case as it can be used directly as mask input.
	if (appData._structLoadStyle.front() != "itk")
	{
		try
		{
			appData._struct = rttb::apps::doseTool::loadStruct(appData._structFileName,
			                  appData._structLoadStyle, appData._structNameRegex);
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
			std::cerr << "Error!!! unknown error while reading struct image." << std::endl;
			return 2;
		}
	}

	try
	{
		rttb::apps::doseTool::processData(appData);
	}
	catch (rttb::core::Exception& e)
	{
		std::cerr << "RTTB Error!!!" << std::endl;
		std::cerr << e.what() << std::endl;
		return 3;
	}
	catch (std::exception& e)
	{
		std::cerr << "Error!!!" << std::endl;
		std::cerr << e.what() << std::endl;
		return 3;
	}
	catch (...)
	{
		std::cerr << "Error!!! unknown error while processing the data or writing the image." << std::endl;
		return 3;
	}

	return result;
}
