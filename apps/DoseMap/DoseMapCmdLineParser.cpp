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

#include "DoseMapCmdLineParser.h"

namespace rttb
{
	namespace apps
	{
		namespace doseMap
		{

			DoseMapCmdLineParser::DoseMapCmdLineParser(int argc, const char** argv, const std::string& name,
				const std::string& version) :
				CmdLineParserBase(name, version)
			{
				std::string defaultLoadingStyle;
				defaultLoadingStyle = "dicom";
				std::string doseLoadStyleDescription = "Indicates the load style that should be used for the input dose. Available styles:"
					"\ndicom: normal dicom dose (default)"
					"\nitk: use itk image loading"
					"\nitkDicom: use itk dicom image loading"
					"\nhelax: load a helax dose (choosing this style, the dose path should only be a directory).";

				addOption<std::string>(OPTION_INPUT_DOSE_FILE_NAME, OPTION_GROUP_REQUIRED,
					"The name of the input dose file. Can be omitted if used as positional argument (see above).", 'd', true);

				addOption<std::string>(OPTION_OUTPUT_FILE_NAME, OPTION_GROUP_REQUIRED,
					"The name of the output file. Can be omitted if used as positional argument (see above).", 'o', true);

				addOptionWithDefaultValue<std::string>(OPTION_INTERPOLATOR, OPTION_GROUP_REQUIRED, "Specifies the interpolator that should be used for mapping."
					"Available options are: \"nn\": nearest neighbor,\"linear\": linear interpolation, \"rosu\" interpolation based on the concept of Rosu et al..",
					"linear", "linear", 'i', true);

				addOptionWithDefaultValue<std::string>(OPTION_REG_FILE_NAME, OPTION_GROUP_REQUIRED, "Specifies name and location of the registration file that should be used to map the input dose. "
					"Default is no mapping, thus an identity transform is used. The registration should be stored as MatchPoint registration.", "", "no mapping", 'r',true);

				addOption<std::string>(OPTION_REF_DOSE_FILE, OPTION_GROUP_OPTIONAL, "Specifies name and location of the dose file that should be the reference/template for the grid to map into. "
					"If flag is not specified, the input dose is the reference.", 't');

				addOptionWithDefaultValue<std::string>(OPTION_INPUT_DOSE_LOAD_STYLE, OPTION_GROUP_REQUIRED, doseLoadStyleDescription,
					defaultLoadingStyle, defaultLoadingStyle, 'l', true);

				addOptionWithDefaultValue<std::string>(OPTION_REF_DOSE_LOAD_STYLE, OPTION_GROUP_OPTIONAL, doseLoadStyleDescription,
					defaultLoadingStyle, defaultLoadingStyle, 's', true);

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
					throw cmdlineparsing::InvalidConstraintException("Unknown interpolator: " +
						interpolator +
						".\nPlease refer to the help for valid interpolator settings.");
				}


				std::string inputDoseLoadStyle = get<std::string>(OPTION_INPUT_DOSE_LOAD_STYLE);

				if (inputDoseLoadStyle != "dicom" && inputDoseLoadStyle != "itk" && inputDoseLoadStyle != "itkDicom"  && inputDoseLoadStyle != "helax")
				{
					throw cmdlineparsing::InvalidConstraintException("Unknown load style for input dose file: " +
						inputDoseLoadStyle +
						".\nPlease refer to the help for valid loading style settings.");
				}

				if (isSet(OPTION_REF_DOSE_FILE)){
					std::string refDoseLoadStyle = get<std::string>(OPTION_REF_DOSE_LOAD_STYLE);

					if (refDoseLoadStyle != "dicom"	&& refDoseLoadStyle != "itk" && refDoseLoadStyle != "itkDicom" && refDoseLoadStyle != "helax")
					{
						throw cmdlineparsing::InvalidConstraintException("Unknown load style for reference dose file: " +
							refDoseLoadStyle +
							".\nPlease refer to the help for valid loading style settings.");
					}
				}

			}

			void DoseMapCmdLineParser::printHelp() const
			{
				cmdlineparsing::CmdLineParserBase::printHelp();
				std::cout << "Example:" << std::endl << std::endl;
				std::cout << m_programName <<
					" dose1.mhd result.mhd -r reg.mapr --" << OPTION_INPUT_DOSE_LOAD_STYLE << " itk --" << OPTION_REF_DOSE_LOAD_STYLE+ " itk" <<
					std::endl << std::endl;
				std::cout <<
					"This will map \"dose1.mhd\" by using \"reg.mapr\" into the grid geometry of the input dose. The resulting dose will be stored in \"result.mhd\"."
					<< std::endl;
			}
		}
	}
}
