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
// @version $Revision: 1221 $ (last changed revision)
// @date    $Date: 2015-12-01 13:43:31 +0100 (Di, 01 Dez 2015) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/

#include "DoseMapApplicationData.h"
#include "DoseMapHelper.h"
#include "DoseMapCmdLineParser.h"

#include "boost/shared_ptr.hpp"
#include "boost/make_shared.hpp"

#include "RTToolboxConfigure.h"

#include "rttbException.h"

#include "mapDummyRegistrationAlgorithm.h"

/**
 Main function of dose mapper.
 @retval 0 normal program execution
 @retval 2 not enough required input files.
 @retval 4 Error loading input dose file
 @retval 5 Error loading reference dose file
 @retval 6 Error loading registration
 @retval 9 Error while mapping or storing result.
*/
int main(int argc,  const char** argv)
{
	int result = 0;

    rttb::apps::doseMap::ApplicationData appData;
	boost::shared_ptr<rttb::apps::doseMap::DoseMapCmdLineParser> argParser;
	
	try
	{
		std::string appName = "DoseMap";
		std::string appVersion = RTTB_FULL_VERSION_STRING;

		argParser = boost::make_shared<rttb::apps::doseMap::DoseMapCmdLineParser>(argc, argv, appName,
			appVersion);

	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 2;
	}


	// This is vital. The application needs to exit if the "help" or "version" parameter is set
	// because this means the other parameters won't be parsed.

	if (argParser->isSet(argParser->OPTION_HELP) || argParser->isSet(argParser->OPTION_VERSION))
	{
		return 0;
	}
	try{
		rttb::apps::doseMap::populateAppData(argParser, appData);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;	
	}

	std::cout << std::endl << "*******************************************" << std::endl;
	std::cout << "Input dose file:              " << appData._inputDoseFileName << std::endl;
	std::cout << "Input dose file load style:   " << appData._inputDoseLoadStyle.at(0) << std::endl;
	std::cout << "Output file:                  " << appData._outputFileName << std::endl;

	if (!(appData._regFileName.empty()))
	{
		std::cout << "Registration file:            " << appData._regFileName << std::endl;
	}

	if (!(appData._refDoseFileName.empty()))
	{
		std::cout << "Reference dose file:          " << appData._refDoseFileName << std::endl;
		std::cout << "Reference dose style:         " << appData._refDoseLoadStyle.at(0) << std::endl;
	}

	try
	{
		appData._inputDose = rttb::apps::doseMap::loadDose(appData._inputDoseFileName,
		                     appData._inputDoseLoadStyle);
	}
	catch (::itk::ExceptionObject& e)
	{
		std::cerr << "Error!!!" << std::endl;
		std::cerr << e << std::endl;
		return 4;
	}
	catch (std::exception& e)
	{
		std::cerr << "Error!!!" << std::endl;
		std::cerr << e.what() << std::endl;
		return 4;
	}
	catch (...)
	{
		std::cerr << "Error!!! unknown error while reading input image." << std::endl;
		return 4;
	}

	if (!(appData._refDoseFileName.empty()))
	{
		try
		{
			appData._refDose = rttb::apps::doseMap::loadDose(appData._refDoseFileName,
			                   appData._refDoseLoadStyle);
		}
		catch (::itk::ExceptionObject& e)
		{
			std::cerr << "Error!!!" << std::endl;
			std::cerr << e << std::endl;
			return 5;
		}
		catch (std::exception& e)
		{
			std::cerr << "Error!!!" << std::endl;
			std::cerr << e.what() << std::endl;
			return 5;
		}
		catch (...)
		{
			std::cerr << "Error!!! unknown error while reading reference image." << std::endl;
			return 5;
		}
	}
	else
	{
		appData._refDose = appData._inputDose;
	}

	if (!(appData._regFileName.empty()))
	{
		try
		{
			appData._spReg = rttb::apps::doseMap::loadRegistration(appData._regFileName);
		}
		catch (::itk::ExceptionObject& e)
		{
			std::cerr << "Error!!!" << std::endl;
			std::cerr << e << std::endl;
			return 6;
		}
		catch (std::exception& e)
		{
			std::cerr << "Error!!!" << std::endl;
			std::cerr << e.what() << std::endl;
			return 6;
		}
		catch (...)
		{
			std::cerr << "Error!!! unknown error while reading registration file." << std::endl;
			return 6;
		}
	}
	else
	{
		//generate dummy identity registration
		typedef map::algorithm::DummyRegistrationAlgorithm<3>	DummyRegType;
		DummyRegType::Pointer regAlg = DummyRegType::New();

		appData._spReg = regAlg->getRegistration();
	}

	try
	{
		rttb::apps::doseMap::processData(appData);
	}
	catch (::itk::ExceptionObject& e)
	{
		std::cerr << "Error!!!" << std::endl;
		std::cerr << e << std::endl;
		return 9;
	}
	catch (std::exception& e)
	{
		std::cerr << "Error!!!" << std::endl;
		std::cerr << e.what() << std::endl;
		return 9;
	}
	catch (...)
	{
		std::cerr << "Error!!! unknown error while mapping and writing image." << std::endl;
		return 9;
	}

	std::cout << std::endl;

	return result;
}
