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
// @version $Revision: 1145 $ (last changed revision)
// @date    $Date: 2015-10-12 17:06:10 +0200 (Mo, 12 Okt 2015) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/

#include "DoseMapApplicationData.h"

#include "itksys/SystemTools.hxx"
#include "itksys/CommandLineArguments.hxx"

#include "RTToolboxConfigure.h"

namespace rttb
{
	namespace apps
	{
		namespace doseMap
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
				_inputDoseFileName = "";
				_refDoseFileName = "";
				_outputFileName = "";
				_regFileName = "";

				_interpolatorName = "linear";

				_showVersion = false;
				_showHelp = false;

				_fileCount = 0;
			}

			unsigned int
			ParseArgumentsForAppData(int argc, char** argv, ApplicationData& appData)
			{
				itksys::CommandLineArguments cmdParser;

				appData.Reset();

				if (argc > 2)
				{
					appData._inputDoseFileName = argv[1];
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

				cmdParser.AddArgument("--interpolator", itksys::CommandLineArguments::SPACE_ARGUMENT,
				                      &(appData._interpolatorName),
				                      "Specifies the interpolator that should be used for mapping. Available options are: \"nn\": nearest neighbour, \"linear\": linear interpolation, \"rosu\" interpolation based on the concept of Rosu et al.. Default interpolator is \"linear\".");
				cmdParser.AddArgument("-i", itksys::CommandLineArguments::SPACE_ARGUMENT,
				                      &(appData._interpolatorName),
				                      "Specifies the interpolator that should be used for mapping. Available options are: \"nn\": nearest neighbour, \"linear\": linear interpolation, \"rosu\" interpolation based on the concept of Rosu et al.. Default interpolator is \"linear\".");

				cmdParser.AddArgument("--registration", itksys::CommandLineArguments::SPACE_ARGUMENT,
				                      &(appData._regFileName),
				                      "Specifies name and location of the registration file that should be used to map the input dose. Default is no mapping, thus an identity transform is used. The registration should be stored as MatchPoint registration.");
				cmdParser.AddArgument("-r", itksys::CommandLineArguments::SPACE_ARGUMENT, &(appData._regFileName),
				                      "Specifies name and location of the registration file that should be used to map the input dose. Default is no mapping, thus an identity transform is used. The registration should be stored as MatchPoint registration.");

				cmdParser.AddArgument("--template", itksys::CommandLineArguments::SPACE_ARGUMENT,
				                      &(appData._refDoseFileName),
				                      "Specifies name and location of the dose file that should be the reference/template for the grid to mapp into. If flag is not specified, the input dose is the reference.");
				cmdParser.AddArgument("-t", itksys::CommandLineArguments::SPACE_ARGUMENT,
				                      &(appData._refDoseFileName),
				                      "Specifies name and location of the dose file that should be the reference/template for the grid to mapp into. If flag is not specified, the input dose is the reference.");

				cmdParser.AddArgument("--loadStyleInput", itksys::CommandLineArguments::MULTI_ARGUMENT,
				                      &(appData._inputDoseLoadStyle),
				                      "Indicates the load style that should be used for the input dose. Available styles: \"dicom\": normal dicom dose (default); \"virtuos\": load of a virtuos dose (This style is a multi argument. The second argument specifies the virtuos plan file, e.g. : \"--loadStyle1 virtuos myFavorite.pln\"); \"itk\": use itk image loading; \"helax\": load a helax dose (choosing this style, the dose path should only be a directory).");

				cmdParser.AddArgument("--loadStyleReference", itksys::CommandLineArguments::MULTI_ARGUMENT,
				                      &(appData._refDoseLoadStyle),
				                      "Indicates the load style that should be used for the reference dose. Available styles: \"dicom\": normal dicom dose (default); \"virtuos\": load of a virtuos dose (This style is a multi argument. The second argument specifies the virtuos plan file, e.g. : \"--loadStyle2 virtuos myFavorite.pln\"); \"itk\": use itk image loading; \"helax\": load a helax dose (choosing this style, the dose path should only be a directory).");

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
					std::cout << "  DoseMap <DoseInput> <DoseOutput> [options]" << std::endl << std::endl;
					std::cout << "     Dose1: File path to the input dose." << std::endl;
					std::cout << "     DoseOutput: File path where the output should be stored." << std::endl <<
					          std::endl;
					std::cout << "Command-Line Options:" << std::endl << std::endl;
					std::cout << cmdParser.GetHelp() << std::endl << std::endl;
					std::cout << " Example:" << std::endl << std::endl;
					std::cout <<
					          " DoseMap dose1.mhd result.mhd -r reg.mapr --loadStyleInput itk --loadStyleReference itk" <<
					          std::endl << std::endl;
					std::cout <<
					          " This will map \"dose1.mhd\" by using \"reg.mapr\" into the grid geometry of the input dose. The resulting dose will be stored in \"result.mhd\"."
					          << std::endl;
					return 1;
				}

				if (appData._showVersion)
				{
					std::cout << std::endl << "Version: " << RTTB_FULL_VERSION_STRING;
					return 1;
				}

				if (appData._fileCount < 2)
				{
					return 2;
				}

				return 0;
			};

		}
	}
}
