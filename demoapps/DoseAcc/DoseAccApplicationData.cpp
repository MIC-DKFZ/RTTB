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
// @version $Revision$ (last changed revision)
// @date    $Date$ (last change date)
// @author  $Author$ (last changed by)
*/

#include "DoseAccApplicationData.h"

#include "itksys/SystemTools.hxx"
#include "itksys/CommandLineArguments.hxx"

#include "RTToolboxConfigure.h"

namespace rttb
{
	namespace apps
	{
		namespace doseAcc
		{

			int unknown_argument(const char* argument, void* call_data)
			{
				std::cout << "Got unknown argument: \"" << argument << "\"" << std::endl;
				return 0;
			}

			ApplicationData::
			ApplicationData()
			{
				this->Reset();
			}

			void
			ApplicationData::
			Reset()
			{
				_dose1FileName = "";
				_dose2FileName = "";
				_outputFileName = "";
				_regFileName = "";
        _dose1VirtuosPlanFileName = "";
        _dose2VirtuosPlanFileName = "";


				_showVersion = false;
				_showHelp = false;
				
				_weightDose1 = 1.0;
				_weightDose2 = 1.0;

				_fileCount = 0;
			}

			unsigned int
			ParseArgumentsForAppData(int argc, char** argv, ApplicationData& appData)
			{
			    itksys::CommandLineArguments cmdParser;
			
				appData.Reset();

				if (argc > 3)
				{
					appData._dose1FileName = argv[1];
					++appData._fileCount;
					--argc;
					++argv;

					appData._dose2FileName = argv[1];
					++appData._fileCount;
					--argc;
					++argv;
					
					appData._outputFileName = argv[1];
					++appData._fileCount;
					--argc;
					++argv;
				}

				cmdParser.Initialize(argc, argv);

				cmdParser.SetUnknownArgumentCallback(unknown_argument);

				cmdParser.AddArgument("--weight1", itksys::CommandLineArguments::SPACE_ARGUMENT, &(appData._weightDose1),
									  "Specifies the weight for dose 1. If not specified the weight is 1.0.");
				cmdParser.AddArgument("-w1", itksys::CommandLineArguments::SPACE_ARGUMENT, &(appData._weightDose1),
									  "Specifies the weight for dose 1. If not specified the weight is 1.0.");
				cmdParser.AddArgument("--weight2", itksys::CommandLineArguments::SPACE_ARGUMENT, &(appData._weightDose2),
									  "Specifies the weight for dose 2. If not specified the weight is 1.0.");
				cmdParser.AddArgument("-w2", itksys::CommandLineArguments::SPACE_ARGUMENT, &(appData._weightDose2),
									  "Specifies the weight for dose 2. If not specified the weight is 1.0.");

				cmdParser.AddArgument("--registration", itksys::CommandLineArguments::SPACE_ARGUMENT, &(appData._regFileName),
									  "Specifies name and location of the registration file that should be used to map dose 2 befor accumulating it with dose 1. Default is no mapping, thus direct accumulation. The registration should be stored as MatchPoint registration.");
				cmdParser.AddArgument("-r", itksys::CommandLineArguments::SPACE_ARGUMENT, &(appData._regFileName),
									  "Specifies name and location of the registration file that should be used to map dose 2 befor accumulating it with dose 1. Default is no mapping, thus direct accumulation. The registration should be stored as MatchPoint registration.");

        cmdParser.AddArgument("--virtuosPlan1", itksys::CommandLineArguments::SPACE_ARGUMENT, &(appData._dose1VirtuosPlanFileName),
									  "Indicates that the dose file of dose 1 should be handled as Virtuos dose and specifies the plan file that should be used to load the dose.");
        cmdParser.AddArgument("-v1", itksys::CommandLineArguments::SPACE_ARGUMENT, &(appData._dose1VirtuosPlanFileName),
									  "Indicates that the dose file of dose 1 should be handled as Virtuos dose and specifies the plan file that should be used to load the dose.");

        cmdParser.AddArgument("--virtuosPlan2", itksys::CommandLineArguments::SPACE_ARGUMENT, &(appData._dose2VirtuosPlanFileName),
									  "Indicates that the dose file of dose 2 should be handled as Virtuos dose and specifies the plan file that should be used to load the dose.");
        cmdParser.AddArgument("-v2", itksys::CommandLineArguments::SPACE_ARGUMENT, &(appData._dose2VirtuosPlanFileName),
									  "Indicates that the dose file of dose 2 should be handled as Virtuos dose and specifies the plan file that should be used to load the dose.");

        cmdParser.AddArgument("-v", itksys::CommandLineArguments::NO_ARGUMENT, &(appData._showVersion),
									  "Shows the version of the program.");

				cmdParser.AddArgument("-h", itksys::CommandLineArguments::NO_ARGUMENT, &(appData._showHelp),
									  "Shows this help information for the program.");
				cmdParser.AddArgument("--help", itksys::CommandLineArguments::NO_ARGUMENT, &(appData._showHelp),
									  "Shows this help information for the program.");
				cmdParser.AddArgument("-?", itksys::CommandLineArguments::NO_ARGUMENT, &(appData._showHelp),
									  "Shows this help information for the program.");
				cmdParser.AddArgument("/h", itksys::CommandLineArguments::NO_ARGUMENT, &(appData._showHelp),
									  "Shows this help information for the program.");
				cmdParser.AddArgument("/?", itksys::CommandLineArguments::NO_ARGUMENT, &(appData._showHelp),
									  "Shows this help information for the program.");

				if (!cmdParser.Parse())
				{
					std::cerr << "Wrong command line option or insufficient number of arguments." << std::endl;
					std::cerr << "The last correct argument was: " << argv[cmdParser.GetLastArgument()] << std::endl <<
							  "Use one of the following flags for more information:" << std::endl;
					std::cerr << "-? or --help" << std::endl;
					return 3;
				};

				if (appData._showHelp)
				{
					std::cout << std::endl << "Usage: " << std::endl << std::endl;
					std::cout << "  DoseAcc <Dose1> <Dose2> <DoseOutput> [options]" << std::endl << std::endl;
					std::cout << "     Dose1: File path to the 1st dose." << std::endl;
					std::cout << "     Dose2: File path to the 2nd dose." << std::endl;
					std::cout << "     DoseOutput: File path where the output should be stored." << std::endl << std::endl;
					std::cout << "Command-Line Options:" << std::endl << std::endl;
					std::cout << cmdParser.GetHelp() << std::endl << std::endl;
					std::cout << " Example:" << std::endl << std::endl;
					std::cout << " DoseCalc dose1.mhd dose2.mhd result.mhd -w1 2 -r reg.mapr" << std::endl << std::endl;
					std::cout <<
							  " This will accumulate \"dose1.mhd\" and \"dose2.mhd\" by using \"reg.mapr\" to map dose 2. For the accumlation dose 1 will be multiplied by 2. The resulting dose will be stored in \"result.mhd\"."
							  << std::endl;
					return 1;
				}

				if (appData._showVersion)
				{
					std::cout << std::endl << "Version: " << RTT_FULL_VERSION_STRING;
					return 1;
				}

				if (appData._fileCount < 3)
				{
					return 2;
				}

				return 0;
			};

		}
	}
}
