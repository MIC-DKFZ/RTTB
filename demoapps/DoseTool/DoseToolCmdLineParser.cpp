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

#include "DoseToolCmdLineParser.h"

namespace rttb
{
	namespace apps
	{
		namespace doseTool
		{

			DoseToolCmdLineParser::DoseToolCmdLineParser(int argc, const char** argv, const std::string& name,
			        const std::string& version) :
				CmdLineParserBase(name, version)
			{
				typedef double DoseTypeGy;
				addOption<std::string>(OPTION_DOSE_FILE, OPTION_GROUP_REQUIRED,
				                       "The name of the dose file. Can be omitted if used as "
				                       "positional argument (see above).", 'd', true);
				addOption<std::string>(OPTION_STRUCT_FILE, OPTION_GROUP_REQUIRED,
				                       "The name of the struct file. Can be omitted if used as "
				                       "positional argument (see above).", 's', true);
				addOptionWithDefaultValue<std::string>(OPTION_STRUCT_NAME, OPTION_GROUP_REQUIRED,
				                                       "The name of the struct as regular expression. Can be omitted if used as "
				                                       "positional argument or with itk struct loadingStyle (see above).", "", "", 'n', true);
				addPositionalOption(OPTION_DOSE_FILE, 1);
				addPositionalOption(OPTION_STRUCT_FILE, 1);
				addPositionalOption(OPTION_STRUCT_NAME, 1);
				addPositionalOption(OPTION_DOSE_STATISTICS, 1);

				std::vector<std::string> defaultLoadingStyle;
				defaultLoadingStyle.push_back("dicom");
				addOptionWithDefaultValue<std::vector<std::string> >(OPTION_DOSE_LOAD_STYLE, OPTION_GROUP_REQUIRED,
				        "The loading style for the dose. Available styles are:\n"
				        "\"dicom\": normal dicom dose\n"
				        "\"virtuos\": load of a virtuos dose (This style is a multi argument. The second argument specifies the virtuos plan file, e.g. : \"--"
				        + OPTION_DOSE_LOAD_STYLE + " virtuos myFavorite.pln\")\n"
				        "\"itk\": use itk image loading\n\"helax\": load a helax dose (choosing this style, the dose path should only be a directory).",
				        defaultLoadingStyle, defaultLoadingStyle.at(0),
				        't', true, true);
				addOptionWithDefaultValue<std::vector<std::string> >(OPTION_STRUCT_LOAD_STYLE, OPTION_GROUP_REQUIRED,
				        "The loading style for the dose. Available styles are:\n"
				        "\"dicom\": normal dicom dose\n"
				        "\"virtuos\": load of a virtuos dose (This style is a multi argument. The second argument specifies the virtuos plan file, e.g. : \"--"
				        + OPTION_DOSE_LOAD_STYLE + " virtuos myFavorite.pln\")\n"
				        "\"itk\": use itk image loading.",
				        defaultLoadingStyle, defaultLoadingStyle.at(0),
				        'u', true, true);

				addOption<std::string>(OPTION_DOSE_STATISTICS, OPTION_GROUP_OPTIONAL,
				                       "If dose statistics should be computed. The argument is the output file. Can be omitted if used as "
				                       "positional argument (see above).", 'y');

				addOption<std::string>(OPTION_DVH, OPTION_GROUP_OPTIONAL,
				                       "If the DVH should be computed. The argument is the output file", 'z');

				addOption(OPTION_COMPLEX_STATISTICS, OPTION_GROUP_OPTIONAL,
				          "If the complex dose statistics (Dx, Vx, MOHx, MOCx, MaxOHx, MinOCx) should be computed.", 'x');
				addOption<DoseTypeGy>(OPTION_PRESCRIBED_DOSE, OPTION_GROUP_OPTIONAL,
				                      "The prescribed dose in Gy.", 'p');
				addOption(OPTION_ALLOW_SELF_INTERSECTION_STRUCT, OPTION_GROUP_OPTIONAL,
				          "If a struct file contains self intersecting contours: Allow the processing of these structures and ignore potential problems."
				          "WARNING: only use this parameter if you know what you are doing.", 'a');
				addOption(OPTION_MULTIPLE_STRUCTS_MODE, OPTION_GROUP_OPTIONAL,
				          "If the regex agrees with multiple structs: write a dose statistic for every struct file."
				          "The struct name will be appended to the chosen output filename.", 'm');

				parse(argc, argv);
			}

			void DoseToolCmdLineParser::validateInput() const
			{
				std::vector<std::string> doseLoadStyle = get<std::vector<std::string> >(OPTION_DOSE_LOAD_STYLE);
				std::string doseLoadStyleAbbreviation = doseLoadStyle.at(0);

				if (doseLoadStyleAbbreviation != "dicom" && doseLoadStyleAbbreviation != "virtuos" && doseLoadStyleAbbreviation != "itk"
				    && doseLoadStyleAbbreviation != "helax")
				{
					throw cmdlineparsing::InvalidConstraintException("Unknown load style for dose file:" + doseLoadStyleAbbreviation +
					        ".\nPlease refer to the help for valid loading style settings.");
				}

				if (doseLoadStyleAbbreviation == "virtuos")
				{
					if (doseLoadStyle.size() < 2)
					{
						throw cmdlineparsing::InvalidConstraintException("Cannot load virtuos dose. Plan file is missing. Specify plan file as 2nd io style argument.");
					}
				}

				std::vector<std::string> structLoadStyle = get<std::vector<std::string> >(OPTION_STRUCT_LOAD_STYLE);
				std::string structLoadStyleAbbreviation = structLoadStyle.at(0);

				if (structLoadStyleAbbreviation != "dicom" && structLoadStyleAbbreviation != "virtuos"
				    && structLoadStyleAbbreviation != "itk")
				{
					throw cmdlineparsing::InvalidConstraintException("Unknown load style for struct file:" + structLoadStyleAbbreviation +
					        ".\nPlease refer to the help for valid loading style settings.");
				}

				if (structLoadStyleAbbreviation == "dicom" || structLoadStyleAbbreviation == "virtuos"
				    || structLoadStyleAbbreviation == "helax")
				{
					if (get<std::string>(OPTION_STRUCT_NAME) == "")
					{
						throw cmdlineparsing::InvalidConstraintException("The struct name (--" + OPTION_STRUCT_NAME +
						        ") has to be defined for dicom, virtuos or helax struct files.");
					}
				}

				if (structLoadStyleAbbreviation == "virtuos")
				{
					if (structLoadStyle.size() < 2)
					{
						throw cmdlineparsing::InvalidConstraintException("Cannot load virtuos struct file. CTX file is missing. Specify CTX file as 2nd io style argument.");
					}
				}

				if (!isSet(OPTION_DVH) && !isSet(OPTION_DOSE_STATISTICS))
				{
					throw cmdlineparsing::InvalidConstraintException("Neither the Dose statistics (--" + OPTION_DOSE_STATISTICS +
					        "), nor the DVH (--" + OPTION_DVH + ") option was used.");
				}

				if (isSet(OPTION_DOSE_STATISTICS) && isSet(OPTION_COMPLEX_STATISTICS))
				{
					if (!isSet(OPTION_PRESCRIBED_DOSE))
					{
						throw cmdlineparsing::InvalidConstraintException("The prescribed dose (--" + OPTION_PRESCRIBED_DOSE +
						        ") has to be set for computation of complex dose statistics.");
					}
					else
					{
						if (get<double>(OPTION_PRESCRIBED_DOSE) <= 0)
						{
							throw cmdlineparsing::InvalidConstraintException("The prescribed dose (--" + OPTION_PRESCRIBED_DOSE +
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
