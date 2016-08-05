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

#include "DoseMapCmdLineParser.h"

namespace rttb
{
	namespace apps
	{
		namespace doseMap
		{

			DoseMapCmdLineParser::DoseMapCmdLineParser(int argc, const char** argv, const std::string& name,
				const std::string& version, bool virtuosSupport) :
				CmdLineParserBase(name, version), _virtuosSupport(virtuosSupport)
			{
				typedef double DoseTypeGy;

				std::vector<std::string> defaultLoadingStyle;
				defaultLoadingStyle.push_back("dicom");
				std::string doseLoadStyleDescription = "\"dicom\": normal dicom dose\n"
					"\"itk\": use itk image loading\n\"helax\": load a helax dose (choosing this style, the dose path should only be a directory).";

				addOption<std::string>(OPTION_INPUT_DOSE_FILE_NAME, OPTION_GROUP_REQUIRED,
					"The name of the input dose file. Can be omitted if used as positional argument (see above).", 'd', true);
				addOption<std::string>(OPTION_OUTPUT_FILE_NAME, OPTION_GROUP_REQUIRED,
					"The name of the output file. Can be omitted if used as positional argument (see above).", 'o', true);
				addOptionWithDefaultValue<std::string>(OPTION_INTERPOLATOR, OPTION_GROUP_REQUIRED, "Specifies the interpolator that should be used for mapping."
					"Available options are: \"nn\": nearest neighbour,\"linear\": linear interpolation, \"rosu\" interpolation based on the concept of Rosu et al.. Default interpolator is \"linear\".",
					"linear", "linear", 'i', true);

				addOptionWithDefaultValue<std::string>(OPTION_REG_FILE_NAME, OPTION_GROUP_REQUIRED, "Specifies name and location of the registration file that should be used to map the input dose. "
					"Default is no mapping, thus an identity transform is used. The registration should be stored as MatchPoint registration.", "", "", 'r',true);

				addOption<std::string>(OPTION_REF_DOSE_FILE, OPTION_GROUP_OPTIONAL, "Specifies name and location of the dose file that should be the reference/template for the grid to mapp into. "
					"If flag is not specified, the input dose is the reference.", 't');//??

				addOptionWithDefaultValue<std::vector<std::string> >(OPTION_INPUT_DOSE_LOAD_STYLE, OPTION_GROUP_REQUIRED, "Indicates the load style that should be used for the input dose."
					"Available styles: \"dicom\": normal dicom dose (default); \"virtuos\": load of a virtuos dose (This style is a multi argument. The second argument specifies "
					"the virtuos plan file, e.g. : \"--loadStyle1 virtuos myFavorite.pln\"); \"itk\": use itk image loading; \"helax\": load a helax dose (choosing this style, the dose path should only be a directory)."
					,defaultLoadingStyle, defaultLoadingStyle.at(0),'l',true);
				addOptionWithDefaultValue<std::vector<std::string> >(OPTION_REF_DOSE_LOAD_STYLE, OPTION_GROUP_REQUIRED, "Indicates the load style that should be used for the reference dose."
					"Available styles: \"dicom\": normal dicom dose (default); \"virtuos\": load of a virtuos dose (This style is a multi argument. The second argument specifies the virtuos plan file, e.g. : \"--loadStyle2 virtuos myFavorite.pln\"); \"itk\": use itk image loading; \"helax\": load a helax dose (choosing this style, the dose path should only be a directory)."
					, defaultLoadingStyle, defaultLoadingStyle.at(0), 's', true);


				addPositionalOption(OPTION_INPUT_DOSE_FILE_NAME, 1);
				addPositionalOption(OPTION_OUTPUT_FILE_NAME, 1);


			
				parse(argc, argv);
			}

			void DoseMapCmdLineParser::validateInput() const
			{
				auto interpolator = get<std::string>(OPTION_INTERPOLATOR);
				if (interpolator != "nn" && interpolator != "linear"
					&& interpolator != "rosu")
				{
					throw cmdlineparsing::InvalidConstraintException("Unknown interpolator:" +
						interpolator +
						".\nPlease refer to the help for valid interpolator settings.");
				}


				std::vector<std::string> indoseLoadStyle = get<std::vector<std::string> >(OPTION_INPUT_DOSE_LOAD_STYLE);
				std::string indoseLoadStyleAbbreviation = indoseLoadStyle.at(0);
				if (indoseLoadStyleAbbreviation != "dicom" && indoseLoadStyleAbbreviation != "virtuos"
					&& indoseLoadStyleAbbreviation != "itk"  && indoseLoadStyleAbbreviation != "helax")
				{
					throw cmdlineparsing::InvalidConstraintException("Unknown load style for input dose file:" +
						indoseLoadStyleAbbreviation +
						".\nPlease refer to the help for valid loading style settings.");
				}

				std::vector<std::string> refdoseLoadStyle = get<std::vector<std::string> >(OPTION_REF_DOSE_LOAD_STYLE);
				std::string refdoseLoadStyleAbbreviation = refdoseLoadStyle.at(0);
				if (refdoseLoadStyleAbbreviation != "dicom" && refdoseLoadStyleAbbreviation != "virtuos"
					&& refdoseLoadStyleAbbreviation != "itk"  && refdoseLoadStyleAbbreviation != "helax")
				{
					throw cmdlineparsing::InvalidConstraintException("Unknown load style for referenz dose file:" +
						refdoseLoadStyleAbbreviation +
						".\nPlease refer to the help for valid loading style settings.");
				}


			}

			void DoseMapCmdLineParser::printHelp() const
			{
				cmdlineparsing::CmdLineParserBase::printHelp();
				std::cout << "Example:" << std::endl << std::endl;
				std::cout << m_programName <<
					" dose1.mhd result.mhd -r reg.mapr --inputDoseLoadStyle itk --loadStyleReference itk" <<
					std::endl << std::endl;
				std::cout <<
					"This will map \"dose1.mhd\" by using \"reg.mapr\" into the grid geometry of the input dose. The resulting dose will be stored in \"result.mhd\"."
					<< std::endl;
			}
		}
	}
}
