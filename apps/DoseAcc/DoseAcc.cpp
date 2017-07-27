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
// @version $Revision: 1107 $ (last changed revision)
// @date    $Date: 2015-09-17 12:47:41 +0200 (Do, 17 Sep 2015) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/

#include "DoseAccApplicationData.h"
#include "DoseAccHelper.h"
#include "DoseAccCmdLineParser.h"

#include "boost/shared_ptr.hpp"
#include "boost/make_shared.hpp"

#include "RTToolboxConfigure.h"

#include "rttbDoseLoader.cpp"

int main(int argc, const char** argv)
{
	int result = 0;

    rttb::apps::doseAcc::ApplicationData appData;
    boost::shared_ptr<rttb::apps::doseAcc::DoseAccCmdLineParser> argParser;
      
	const std::string appCategory = "RT-Toolbox App";
	const std::string appName = "DoseAcc";
	const std::string appDesc = "An App to accumulate two doses. The GUI for this app is currently under development and in an experimental state.";
	const std::string appContributor = "SIDT@DKFZ";
    const std::string appVersion = RTTB_FULL_VERSION_STRING;

    try
    {

        argParser = boost::make_shared<rttb::apps::doseAcc::DoseAccCmdLineParser>(argc, argv, appName,
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

    rttb::apps::doseAcc::populateAppData(argParser, appData);

	std::cout << std::endl << "*******************************************" << std::endl;
	std::cout << "Dose 1 file:        " << appData._dose1FileName << std::endl;
	std::cout << "Dose 2 file:        " << appData._dose2FileName << std::endl;
	std::cout << "Dose output file:   " << appData._outputFileName << std::endl;

	if (!(appData._regFileName.empty()))
	{
		std::cout << "Registration file: " << appData._regFileName << std::endl;
	}

	std::cout << "Dose 1 weight:      " << appData._weightDose1 << std::endl;
	std::cout << "Dose 2 weight:      " << appData._weightDose2 << std::endl;
	std::cout << "Operator:           " << appData._operator << std::endl;

	try
	{
		appData._dose1 = rttb::io::utils::loadDose(appData._dose1FileName, appData._dose1LoadStyle);
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

	try
	{
		appData._dose2 = rttb::io::utils::loadDose(appData._dose2FileName, appData._dose2LoadStyle);
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

	if (!(appData._regFileName.empty()))
	{
		try
		{
			appData._spReg = rttb::apps::doseAcc::loadRegistration(appData._regFileName);
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
			std::cerr << "Error!!! unknown error while reading registration file." << std::endl;
			return 5;
		}
	}

	try
	{
		rttb::apps::doseAcc::processData(appData);
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
