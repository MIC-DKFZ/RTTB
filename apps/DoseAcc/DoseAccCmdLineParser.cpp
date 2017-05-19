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
			DoseAccCmdLineParser::DoseAccCmdLineParser(int argc, const char** argv, const std::string& name, const std::string& version,
				const std::string& description, const std::string& contributor, const std::string& category) :
				CmdLineParserBase(name, version, description, contributor, category)
			{
				//REQUIRED
				addOption<std::string>(OPTION_DOSE1_FILENAME, OPTION_GROUP_REQUIRED,
					"File path to the first dose.", 'd', true);
				addInformationForXML(OPTION_DOSE1_FILENAME, cmdlineparsing::XMLGenerator::paramType::INPUT, { "*" });

				addOption<std::string>(OPTION_DOSE2_FILENAME, OPTION_GROUP_REQUIRED,
					"File path to the second dose.", 'e', true);
				addInformationForXML(OPTION_DOSE2_FILENAME, cmdlineparsing::XMLGenerator::paramType::INPUT, { "*" });

				addOption<std::string>(OPTION_OUTPUT_FILENAME, OPTION_GROUP_REQUIRED,
					"File path where the output should be stored.", 'o', true);
				addInformationForXML(OPTION_OUTPUT_FILENAME, cmdlineparsing::XMLGenerator::paramType::OUTPUT, { "*" });

				addOptionWithDefaultValue<std::string>(OPTION_INTERPOLATOR, OPTION_GROUP_REQUIRED,
					"Specifies the interpolator that should be used for mapping. Available options are: "
					"\"nn\": nearest neighbour, \"linear\": linear interpolation, \"rosu\" interpolation based on the concept of Rosu et al..",
					"linear", "linear", 'i', true);
				addInformationForXML(OPTION_INTERPOLATOR, cmdlineparsing::XMLGenerator::paramType::STRINGENUMERATION, { "linear", "nn", "rosu" });

				addOptionWithDefaultValue<double>(OPTION_WEIGHT1, OPTION_GROUP_REQUIRED,
					"Specifies the weight for dose 1.",
					1.0, "1.0", 'w', true);
				addInformationForXML(OPTION_WEIGHT1, cmdlineparsing::XMLGenerator::paramType::DOUBLE);

				addOptionWithDefaultValue<double>(OPTION_WEIGHT2, OPTION_GROUP_REQUIRED,
					"Specifies the weight for dose 2.",
					1.0, "1.0", 'z', true);
				addInformationForXML(OPTION_WEIGHT2, cmdlineparsing::XMLGenerator::paramType::DOUBLE);

				addOptionWithDefaultValue<std::string>(OPTION_REGISTRATION_FILENAME, OPTION_GROUP_REQUIRED,
					"Specifies name and location of the registration file that should be used to map dose 2 before accumulating it with dose 1."
					"The registration should be stored as MatchPoint registration.",
					"", "no mapping", 'r', true);
				addInformationForXML(OPTION_REGISTRATION_FILENAME, cmdlineparsing::XMLGenerator::paramType::INPUT, { "mapr" });

				std::vector<std::string> defaultLoadingStyle;
				defaultLoadingStyle.push_back("dicom");
				std::string doseLoadStyleDescription = "Options are:\n \"dicom\": normal dicom dose\n"
					"\"itk\": use itk image loading\n\"helax\": load a helax dose (choosing this style, the dose path should only be a directory).";

				addOptionWithDefaultValue<std::vector<std::string> >(OPTION_LOAD_STYLE_DOSE1, OPTION_GROUP_REQUIRED,
					"Load style for dose 1. " + doseLoadStyleDescription,
					defaultLoadingStyle, defaultLoadingStyle.at(0),
					't', true, true);
				addInformationForXML(OPTION_LOAD_STYLE_DOSE1, cmdlineparsing::XMLGenerator::paramType::STRINGENUMERATION, { "dicom","itk","helax" });

				addOptionWithDefaultValue<std::vector<std::string> >(OPTION_LOAD_STYLE_DOSE2, OPTION_GROUP_REQUIRED,
					"Load style for dose 2. See " + OPTION_LOAD_STYLE_DOSE1,
					defaultLoadingStyle, defaultLoadingStyle.at(0),
					'u', true, true);
				addInformationForXML(OPTION_LOAD_STYLE_DOSE2, cmdlineparsing::XMLGenerator::paramType::STRINGENUMERATION, { "dicom","itk","helax" });

				addOptionWithDefaultValue<std::string>(OPTION_OPERATOR, OPTION_GROUP_REQUIRED,
					"Specifies the operator used. Available operators are '+' and '*'. Operator '*' has implemented no weight option.",
					"+", "+", 'p', true);
				addInformationForXML(OPTION_OPERATOR, cmdlineparsing::XMLGenerator::paramType::STRINGENUMERATION, { "+","*"});

				addPositionalOption(OPTION_DOSE1_FILENAME, 1);
				addPositionalOption(OPTION_DOSE2_FILENAME, 1);
				addPositionalOption(OPTION_OUTPUT_FILENAME, 1);

				parse(argc, argv);
			}

            void DoseAccCmdLineParser::validateInput() const
			{
                std::vector<std::string> doseLoadStyle1 = get<std::vector<std::string> >(OPTION_LOAD_STYLE_DOSE1);
				std::string doseLoadStyleAbbreviation1 = doseLoadStyle1.at(0);

				if (doseLoadStyleAbbreviation1 != "dicom" 
				    && doseLoadStyleAbbreviation1 != "itk"
				    && doseLoadStyleAbbreviation1 != "helax")
				{
					throw cmdlineparsing::InvalidConstraintException("Unknown load style for dose1 file: " +
					        doseLoadStyleAbbreviation1 +
					        ".\nPlease refer to the help for valid loading style settings.");
				}

                std::vector<std::string> doseLoadStyle2 = get<std::vector<std::string> >(OPTION_LOAD_STYLE_DOSE2);
                std::string doseLoadStyleAbbreviation2 = doseLoadStyle2.at(0);

                if (doseLoadStyleAbbreviation2 != "dicom" 
                    && doseLoadStyleAbbreviation2 != "itk"
                    && doseLoadStyleAbbreviation2 != "helax")
                {
                    throw cmdlineparsing::InvalidConstraintException("Unknown load style for dose2 file: " +
                        doseLoadStyleAbbreviation2 +
                        ".\nPlease refer to the help for valid loading style settings.");
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

                if (operatorString == "*"){
                    double weight1 = get<double>(OPTION_WEIGHT1);
                    double weight2 = get<double>(OPTION_WEIGHT2);
                    if (weight1 != 1.0 || weight2 != 1.0){
                        throw cmdlineparsing::InvalidConstraintException("Operator \"*\" has no weight option implemented. Options --" + OPTION_WEIGHT1 + " and --" + OPTION_WEIGHT2 + " are invalid.");
                    }
                }

			}

            void DoseAccCmdLineParser::printHelp() const
			{
				cmdlineparsing::CmdLineParserBase::printHelp();
                std::cout << " Example:" << std::endl << std::endl;
                std::cout << " DoseAcc dose1.mhd dose2.mhd result.mhd --" + OPTION_LOAD_STYLE_DOSE1 + " itk --" + OPTION_LOAD_STYLE_DOSE2 + " itk --" + OPTION_WEIGHT1 + " 2 -r reg.mapr" << std::endl << std::endl;
                std::cout << " This will accumulate \"dose1.mhd\" and \"dose2.mhd\" by using \"reg.mapr\" to map dose 2.";
                std::cout << " For the accumulation, dose 1 will be multiplied by 2. The resulting dose will be stored in \"result.mhd\"." << std::endl << std::endl;
			}

		}
	}
}
