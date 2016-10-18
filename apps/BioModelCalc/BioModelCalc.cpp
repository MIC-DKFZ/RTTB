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
// @version $Revision: 1334 $ (last changed revision)
// @date    $Date: 2016-04-22 11:13:22 +0200 (Fr, 22 Apr 2016) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
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
		std::string appVersion = RTTB_FULL_VERSION_STRING;

		argParser = boost::make_shared<rttb::apps::bioModelCalc::BioModelCmdLineParser>(argc, argv, appName,
		            appVersion);
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


	for (size_t i = 0; i < appData._modelParameters.size(); i++)
	{
		if (i != 0)
		{
			std::cout << ", ";
		}

		std::cout << appData._modelParameters.at(i);
	}
    for (size_t i = 0; i < appData._modelParameterMapsFilename.size(); i++)
    {
        if (i != 0)
        {
            std::cout << ", ";
        }

        std::cout << appData._modelParameterMapsFilename.at(i);
    }

	std::cout << std::endl;
	std::cout << "Dose scaling:          " << appData._doseScaling << std::endl;
    if (argParser->isSet(argParser->OPTION_N_FRACTIONS)){
        std::cout << "#Fractions:            " << appData._nFractions << std::endl;
    }

	std::cout << std::endl;

	try
	{
		appData._dose = rttb::apps::bioModelCalc::loadDose(appData._doseFileName, appData._doseLoadStyle);
        for (const auto& filename : appData._modelParameterMapsFilename){
            appData._modelParameterMaps.push_back(rttb::apps::bioModelCalc::loadDose(filename, appData._parameterMapsLoadStyle));
        }
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
		std::cerr << "Error!!! unknown error while calculating the bioModel or writing the image." <<
		          std::endl;
		return 2;
	}

	return result;
}
