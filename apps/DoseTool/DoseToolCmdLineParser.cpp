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

#include "DoseToolCmdLineParser.h"

namespace rttb
{
	namespace apps
	{
		namespace doseTool
		{

			DoseToolCmdLineParser::DoseToolCmdLineParser(int argc, const char** argv, const std::string& name, const std::string& version,
				const std::string& description, const std::string& contributor, const std::string& category) :
				CmdLineParserBase(name, version, description, contributor, category)
			{
				//REQUIRED
				using DoseTypeGy = double;
				addOption<std::string>(OPTION_DOSE_FILE, OPTION_GROUP_REQUIRED,
					"The name of the dose file. Can be omitted if used as "
					"positional argument (see above).", 'd', true);
				addInformationForXML(OPTION_DOSE_FILE, cmdlineparsing::XMLGenerator::paramType::INPUT, { "*" });

				addOption<std::string>(OPTION_STRUCT_FILE, OPTION_GROUP_REQUIRED,
					"The name of the struct file. Can be omitted if used as "
					"positional argument (see above).", 's', true);
				addInformationForXML(OPTION_STRUCT_FILE, cmdlineparsing::XMLGenerator::paramType::INPUT, { "*" });

				addOptionWithDefaultValue<std::string>(OPTION_STRUCT_NAME, OPTION_GROUP_REQUIRED,
					"The name of the struct as regular expression. Can be omitted if used as "
					"positional argument or with itk struct loadingStyle (see above).", "", "", 'n', true);
				addInformationForXML(OPTION_STRUCT_NAME, cmdlineparsing::XMLGenerator::paramType::STRING);

				addPositionalOption(OPTION_DOSE_FILE, 1);
				addPositionalOption(OPTION_STRUCT_FILE, 1);
				addPositionalOption(OPTION_STRUCT_NAME, 1);
				addPositionalOption(OPTION_DOSE_STATISTICS, 1);

				std::vector<std::string> defaultLoadingStyle;
				defaultLoadingStyle.push_back("dicom");
				std::string doseLoadStyleDescription = "\"dicom\": normal dicom dose\n"
					"\"itk\": use itk image loading\n\"helax\": load a helax dose (choosing this style, the dose path should only be a directory).";

				addOptionWithDefaultValue<std::vector<std::string> >(OPTION_DOSE_LOAD_STYLE, OPTION_GROUP_REQUIRED,
					doseLoadStyleDescription,
					defaultLoadingStyle, defaultLoadingStyle.at(0),
					't', true, true);
				addInformationForXML(OPTION_DOSE_LOAD_STYLE, cmdlineparsing::XMLGenerator::paramType::STRINGENUMERATION, { "dicom", "itk", "helax" });

				std::string structLoadStyleDescription = "\"dicom\": normal dicom dose\n"
					"\"itk\": use itk image loading\"";

				addOptionWithDefaultValue<std::vector<std::string> >(OPTION_STRUCT_LOAD_STYLE,
					OPTION_GROUP_REQUIRED, structLoadStyleDescription,
					defaultLoadingStyle, defaultLoadingStyle.at(0),
					'u', true, true);
				addInformationForXML(OPTION_STRUCT_LOAD_STYLE, cmdlineparsing::XMLGenerator::paramType::STRINGENUMERATION, { "dicom", "itk"});

				//OPTIONAL
				addOption<std::string>(OPTION_DOSE_STATISTICS, OPTION_GROUP_OPTIONAL,
					"If dose statistics should be computed. The argument is the output file. Can be omitted if used as "
					"positional argument (see above).", 'y');
				addInformationForXML(OPTION_DOSE_STATISTICS, cmdlineparsing::XMLGenerator::paramType::OUTPUT, { "*" });

				addOption<std::string>(OPTION_DVH, OPTION_GROUP_OPTIONAL,
					"If the DVH should be computed. The argument is the output file", 'z');
				addInformationForXML(OPTION_DVH, cmdlineparsing::XMLGenerator::paramType::OUTPUT, { "*" });

				addOption(OPTION_COMPLEX_STATISTICS, OPTION_GROUP_OPTIONAL,
					"If the complex dose statistics (Dx, Vx, MOHx, MOCx, MaxOHx, MinOCx) should be computed.", 'f');
				addInformationForXML(OPTION_COMPLEX_STATISTICS, cmdlineparsing::XMLGenerator::paramType::BOOLEAN);

				addOption<DoseTypeGy>(OPTION_PRESCRIBED_DOSE, OPTION_GROUP_OPTIONAL,
					"The prescribed dose in Gy.", 'p');
				addInformationForXML(OPTION_PRESCRIBED_DOSE, cmdlineparsing::XMLGenerator::paramType::DOUBLE);

				addOption(OPTION_ALLOW_SELF_INTERSECTION_STRUCT, OPTION_GROUP_OPTIONAL,
					"If a struct file contains self intersecting contours: Allow the processing of these structures and ignore potential problems."
					"WARNING: only use this parameter if you know what you are doing.", 'a');
				addInformationForXML(OPTION_ALLOW_SELF_INTERSECTION_STRUCT, cmdlineparsing::XMLGenerator::paramType::BOOLEAN);

				addOption(OPTION_MULTIPLE_STRUCTS_MODE, OPTION_GROUP_OPTIONAL,
					"If the regex agrees with multiple structs: write a dose statistic for every struct file."
					"The struct name will be appended to the chosen output filename.", 'm');
				addInformationForXML(OPTION_MULTIPLE_STRUCTS_MODE, cmdlineparsing::XMLGenerator::paramType::BOOLEAN);

				parse(argc, argv);
			}

			void DoseToolCmdLineParser::validateInput() const
			{
				std::vector<std::string> doseLoadStyle = get<std::vector<std::string> >(OPTION_DOSE_LOAD_STYLE);
				std::string doseLoadStyleAbbreviation = doseLoadStyle.at(0);

				if (doseLoadStyleAbbreviation != "dicom"
				    && doseLoadStyleAbbreviation != "itk"
				    && doseLoadStyleAbbreviation != "helax")
				{
					throw cmdlineparsing::InvalidConstraintException("Unknown load style for dose file:" +
					        doseLoadStyleAbbreviation +
					        ".\nPlease refer to the help for valid loading style settings.");
				}

				std::vector<std::string> structLoadStyle = get<std::vector<std::string> >(OPTION_STRUCT_LOAD_STYLE);
				std::string structLoadStyleAbbreviation = structLoadStyle.at(0);

				if (structLoadStyleAbbreviation != "dicom"
				    && structLoadStyleAbbreviation != "itk")
				{
					throw cmdlineparsing::InvalidConstraintException("Unknown load style for struct file:" +
					        structLoadStyleAbbreviation +
					        ".\nPlease refer to the help for valid loading style settings.");
				}

				if (structLoadStyleAbbreviation == "dicom"
				    || structLoadStyleAbbreviation == "helax")
				{
					if (get<std::string>(OPTION_STRUCT_NAME) == "")
					{
						throw cmdlineparsing::InvalidConstraintException("The struct name (--" + OPTION_STRUCT_NAME +
						        ") has to be defined for dicom or helax struct files.");
					}
				}

				if (!isSet(OPTION_DVH) && !isSet(OPTION_DOSE_STATISTICS))
				{
					throw cmdlineparsing::InvalidConstraintException("Neither the Dose statistics (--" +
					        OPTION_DOSE_STATISTICS +
					        "), nor the DVH (--" + OPTION_DVH + ") option was used.");
				}

				if (isSet(OPTION_DOSE_STATISTICS) && isSet(OPTION_COMPLEX_STATISTICS))
				{
					if (!isSet(OPTION_PRESCRIBED_DOSE))
					{
						throw cmdlineparsing::InvalidConstraintException("The prescribed dose (--" + OPTION_PRESCRIBED_DOSE
						        +
						        ") has to be set for computation of complex dose statistics.");
					}
					else
					{
						if (get<double>(OPTION_PRESCRIBED_DOSE) <= 0)
						{
							throw cmdlineparsing::InvalidConstraintException("The prescribed dose (--" + OPTION_PRESCRIBED_DOSE
							        +
							        ") has to be >0!");
						}
					}
				}
			}

			void DoseToolCmdLineParser::printHelp() const
			{
				cmdlineparsing::CmdLineParserBase::printHelp();
				std::cout << "Example:" << std::endl << std::endl;
				std::cout << m_programName <<
				          " dose.dcm struct.dcm Liver result.xml --" + OPTION_DVH + " dvh.xml" <<
				          std::endl << std::endl;
				std::cout <<
				          "This will calculate the Dose statistic for liver using \"dose.dcm\" and the struct file \"struct.dcm\" and will write the dose statistics to \"result.xml\". "
				          " The DVH is computed as well, its values are written to \"dvh.xml\". "
				          << std::endl;
			}

		}
	}
}
