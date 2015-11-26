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
// @version $Revision: 1066 $ (last changed revision)
// @date    $Date: 2015-08-19 11:47:07 +0200 (Mi, 19 Aug 2015) $ (last change date)
// @author  $Author: floca $ (last changed by)
*/

#include "DoseToolApplicationData.h"
#include "DoseToolHelper.h"
#include "DoseToolCmdLineParser.h"

#include "boost/shared_ptr.hpp"
#include "boost/make_shared.hpp"

#include "RTToolboxConfigure.h"

#include "rttbException.h"

rttb::apps::doseTool::ApplicationData appData;

int main(int argc, const char** argv)
{
	int result = 0;

	boost::shared_ptr<rttb::apps::doseTool::DoseToolCmdLineParser> argParser;

	try
	{
		std::string appName = "DoseTool";
		std::string appVersion = RTTB_FULL_VERSION_STRING;

		argParser = boost::make_shared<rttb::apps::doseTool::DoseToolCmdLineParser>(argc, argv, appName, appVersion);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return -1;
	}

	// This is vital. The application needs to exit if the "help" or "version" parameter is set
	// because this means the other parameters won't be parsed.

	if (argParser->isSet(argParser->OPTION_HELP) || argParser->isSet(argParser->OPTION_VERSION))
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
			appData._struct = rttb::apps::doseTool::loadStruct(appData._structFileName, appData._structLoadStyle);
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
