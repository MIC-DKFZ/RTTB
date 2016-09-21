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
// @version $Revision: 1374 $ (last changed revision)
// @date    $Date: 2016-05-30 14:15:42 +0200 (Mo, 30 Mai 2016) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/

#include "DoseAccCmdLineParser.h"

namespace rttb
{
	namespace apps
	{
		namespace doseAcc
		{

			DoseAccCmdLineParser::DoseAccCmdLineParser(int argc, const char** argv, const std::string& name,
			        const std::string& version, bool virtuosSupport) :
				CmdLineParserBase(name, version), _virtuosSupport(virtuosSupport)
			{
                addOption<std::string>(OPTION_DOSE1_FILENAME, OPTION_GROUP_REQUIRED,
                    "File path to the first dose.", 'd', true);
                addOption<std::string>(OPTION_DOSE2_FILENAME, OPTION_GROUP_REQUIRED,
                    "File path to the second dose.", 'e', true);
                addOption<std::string>(OPTION_OUTPUT_FILENAME, OPTION_GROUP_REQUIRED,
                    "File path where the output should be stored.", 'o', true);
				addOptionWithDefaultValue<std::string>(OPTION_INTERPOLATOR, OPTION_GROUP_REQUIRED,
				                       "Specifies the interpolator that should be used for mapping. Available options are: "
                                       "\"nn\": nearest neighbour, \"linear\": linear interpolation, \"rosu\" interpolation based on the concept of Rosu et al..", 
                                       "linear", "linear", 'i', true);
                addOptionWithDefaultValue<double>(OPTION_WEIGHT1, OPTION_GROUP_REQUIRED,
                    "Specifies the weight for dose 1.",
                    1.0, "1.0", 'w', true);
                addOptionWithDefaultValue<double>(OPTION_WEIGHT2, OPTION_GROUP_REQUIRED,
                    "Specifies the weight for dose 2.",
                    1.0, "1.0", 'z', true);
                addOptionWithDefaultValue<std::string>(OPTION_REGISTRATION_FILENAME, OPTION_GROUP_REQUIRED,
                    "Specifies name and location of the registration file that should be used to map dose 2 before accumulating it with dose 1."
                    "The registration should be stored as MatchPoint registration.",
                    "", "no mapping", 'r', true);
		
                std::vector<std::string> defaultLoadingStyle;
                defaultLoadingStyle.push_back("dicom");
                std::string doseLoadStyleDescription = "Options are:\n \"dicom\": normal dicom dose\n"
                    "\"itk\": use itk image loading\n\"helax\": load a helax dose (choosing this style, the dose path should only be a directory).";


                if (_virtuosSupport)
                {
                    doseLoadStyleDescription += "\n"
                        "\"virtuos\": load of a virtuos dose (This style is a multi argument. The second argument specifies the virtuos plan file, e.g. : \"--"
                        + OPTION_LOAD_STYLE_DOSE1 + " or " + OPTION_LOAD_STYLE_DOSE2 + " virtuos myFavorite.pln\")";
                }

                addOptionWithDefaultValue<std::vector<std::string> >(OPTION_LOAD_STYLE_DOSE1, OPTION_GROUP_REQUIRED,
                    "Load style for dose 1. "+ doseLoadStyleDescription,
                    defaultLoadingStyle, defaultLoadingStyle.at(0),
                    't', true, true);

                addOptionWithDefaultValue<std::vector<std::string> >(OPTION_LOAD_STYLE_DOSE2, OPTION_GROUP_REQUIRED,
                    "Load style for dose 2. See " + OPTION_LOAD_STYLE_DOSE1,
                    defaultLoadingStyle, defaultLoadingStyle.at(0),
                    'u', true, true);

                addOptionWithDefaultValue<std::string>(OPTION_OPERATOR, OPTION_GROUP_REQUIRED,
                    "Specifies the operator used. Available operators are '+' and '*'.",
                    "+", "+", 'p', true);

                addPositionalOption(OPTION_DOSE1_FILENAME, 1);
                addPositionalOption(OPTION_DOSE2_FILENAME, 1);
                addPositionalOption(OPTION_OUTPUT_FILENAME, 1);

				parse(argc, argv);
			}

            void DoseAccCmdLineParser::validateInput() const
			{
                std::vector<std::string> doseLoadStyle1 = get<std::vector<std::string> >(OPTION_LOAD_STYLE_DOSE1);
				std::string doseLoadStyleAbbreviation1 = doseLoadStyle1.at(0);

				if (doseLoadStyleAbbreviation1 != "dicom" && (!_virtuosSupport || doseLoadStyleAbbreviation1 != "virtuos")
				    && doseLoadStyleAbbreviation1 != "itk"
				    && doseLoadStyleAbbreviation1 != "helax")
				{
					throw cmdlineparsing::InvalidConstraintException("Unknown load style for dose1 file: " +
					        doseLoadStyleAbbreviation1 +
					        ".\nPlease refer to the help for valid loading style settings.");
				}

				if (_virtuosSupport && doseLoadStyleAbbreviation1 == "virtuos")
				{
					if (doseLoadStyle1.size() < 2)
					{
						throw cmdlineparsing::InvalidConstraintException("Cannot load virtuos dose. Plan file is missing. Specify plan file as 2nd io style argument.");
					}
				}

                std::vector<std::string> doseLoadStyle2 = get<std::vector<std::string> >(OPTION_LOAD_STYLE_DOSE2);
                std::string doseLoadStyleAbbreviation2 = doseLoadStyle2.at(0);

                if (doseLoadStyleAbbreviation2 != "dicom" && (!_virtuosSupport || doseLoadStyleAbbreviation2 != "virtuos")
                    && doseLoadStyleAbbreviation2 != "itk"
                    && doseLoadStyleAbbreviation2 != "helax")
                {
                    throw cmdlineparsing::InvalidConstraintException("Unknown load style for dose2 file: " +
                        doseLoadStyleAbbreviation2 +
                        ".\nPlease refer to the help for valid loading style settings.");
                }

                if (_virtuosSupport && doseLoadStyleAbbreviation2 == "virtuos")
                {
                    if (doseLoadStyle2.size() < 2)
                    {
                        throw cmdlineparsing::InvalidConstraintException("Cannot load virtuos dose. Plan file is missing. Specify plan file as 2nd io style argument.");
                    }
                }

                std::string interpolator = get<std::string>(OPTION_INTERPOLATOR);
                if (interpolator != "nn"  && interpolator != "linear"
                    && interpolator != "rosu")
                {
                    throw cmdlineparsing::InvalidConstraintException("Unknown interpolator: " +
                        interpolator +
                        ".\nPlease refer to the help for valid interpolator settings.");
                }

                std::string operatorString = get<std::string>(OPTION_OPERATOR);
                if (operatorString != "+"  && operatorString != "*")
                {
                    throw cmdlineparsing::InvalidConstraintException("Unknown operator: " +
                        operatorString +
                        ".\nPlease refer to the help for valid operator settings.");
                }

			}

            void DoseAccCmdLineParser::printHelp() const
			{
				cmdlineparsing::CmdLineParserBase::printHelp();
                std::cout << " Example:" << std::endl << std::endl;
                std::cout << " DoseAcc dose1.mhd dose2.mhd result.mhd --" + OPTION_LOAD_STYLE_DOSE1 + " itk --" + OPTION_LOAD_STYLE_DOSE2 + " itk --" + OPTION_WEIGHT1 + " 2 -r reg.mapr" << std::endl << std::endl;
                std::cout << " This will accumulate \"dose1.mhd\" and \"dose2.mhd\" by using \"reg.mapr\" to map dose 2.";
                std::cout << " For the accumulation, dose 1 will be multiplied by 2. The resulting dose will be stored in \"result.mhd\"." << std::endl << std::endl;
                if (_virtuosSupport){
                    std::cout << " DoseAcc dose1.dcm dose2.dos.gz result.mhd --" + OPTION_LOAD_STYLE_DOSE2 + " virtuos dose2.pln -r reg.mapr" << std::endl << std::endl;
                    std::cout << " This will accumulate \"dose1.dcm\" (using default dicom io) and \"dose2.dos.gz\" (using virtuos io and plan file dose2.pln)";
                    std::cout << " by using \"reg.mapr\" to map dose 2. The resulting dose will be stored in \"result.mhd\"." << std::endl;
                }
			}

		}
	}
}
