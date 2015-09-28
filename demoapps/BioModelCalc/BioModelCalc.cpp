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

#include "BioModelCalcApplicationData.h"
#include "BioModelCalcHelper.h"
#include "BioModelCmdLineParser.h"

#include "boost/shared_ptr.hpp"
#include "boost/make_shared.hpp"

#include "RTToolboxConfigure.h"

#include "rttbException.h"

rttb::apps::bioModelCalc::ApplicationData appData;

int main(int argc, const char** argv)
{
	int result = 0;

	boost::shared_ptr<rttb::apps::bioModelCalc::BioModelCmdLineParser> argParser;

	try
	{
		std::string appName = "BioModelCalc";
		std::string appVersion = RTT_FULL_VERSION_STRING;

		argParser = boost::make_shared<rttb::apps::bioModelCalc::BioModelCmdLineParser>(argc, argv, appName, appVersion);
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

	rttb::apps::bioModelCalc::populateAppData(argParser, appData);

	std::cout << std::endl << "*******************************************" << std::endl;
	std::cout << "Dose file:             " << appData._doseFileName << std::endl;
	std::cout << "Bio model output file: " << appData._outputFileName << std::endl;
	std::cout << "Model:                 " << appData._model << std::endl;
	std::cout << "Model parameters:      ";

	for (unsigned int i = 0; i < appData._modelParameters.size(); i++)
	{
		if (i != 0)
		{
			std::cout << ", ";
		}

		std::cout << appData._modelParameters.at(i);
	}

	std::cout << std::endl;

	try
	{
		appData._dose = rttb::apps::bioModelCalc::loadDose(appData._doseFileName, appData._doseLoadStyle);
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
		std::cerr << "Error!!! unknown error while reading input image." << std::endl;
		return 1;
	}

	try
	{
		rttb::apps::bioModelCalc::processData(appData);
	}
	catch (rttb::core::Exception& e)
	{
		std::cerr << "RTTB Error!!!" << std::endl;
		std::cerr << e.what() << std::endl;
		return 2;
	}
	catch (std::exception& e)
	{
		std::cerr << "Error!!!" << std::endl;
		std::cerr << e.what() << std::endl;
		return 2;
	}
	catch (...)
	{
		std::cerr << "Error!!! unknown error while calculating the bioModel or writing the image." << std::endl;
		return 2;
	}

	std::cout << "ENDE" << std::endl;

	return result;
}