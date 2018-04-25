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

#include "BioModelCmdLineParser.h"

namespace rttb
{
	namespace apps
	{
		namespace bioModelCalc
		{
			BioModelCmdLineParser::BioModelCmdLineParser(int argc, const char** argv, const std::string& name, const std::string& version,
				const std::string& description, const std::string& contributor, const std::string& category) :
				CmdLineParserBase(name, version, description, contributor, category)
			{
				//REQUIRED
				addOption<std::string>(OPTION_DOSE_FILE, OPTION_GROUP_REQUIRED,
					"The name of the dose file. Can be omitted if used as "
					"positional argument (see above).", 'd', true);
				addInformationForXML(OPTION_DOSE_FILE, cmdlineparsing::XMLGenerator::paramType::INPUT, { "*" });

				addOption<std::string>(OPTION_OUTPUT_FILE, OPTION_GROUP_REQUIRED,
					"The name of the output file. Can be omitted if used as "
					"positional argument (see above).", 'o', true);
				addInformationForXML(OPTION_OUTPUT_FILE, cmdlineparsing::XMLGenerator::paramType::OUTPUT, { "*" });

				addPositionalOption(OPTION_DOSE_FILE, 1);
				addPositionalOption(OPTION_OUTPUT_FILE, 1);

				addOptionWithDefaultValue<std::string>(OPTION_MODEL, OPTION_GROUP_REQUIRED,
					"The used radiobiological model the dose should be analyzed with. Available models are:\n \"LQ\", Formula: exp(-(alpha * D + beta * D^2/n))",
					"LQ", "LQ", 'm');
				addInformationForXML(OPTION_MODEL, cmdlineparsing::XMLGenerator::paramType::STRINGENUMERATION, { "LQ" });

				addOptionWithDefaultValue<double>(OPTION_DOSE_SCALING, OPTION_GROUP_REQUIRED,
					"Dose scaling that should be applied.", 1.0, "1.0", 'e');
				addInformationForXML(OPTION_DOSE_SCALING, cmdlineparsing::XMLGenerator::paramType::DOUBLE);

				std::string defaultLoadingStyle;
				defaultLoadingStyle = "itk";

				std::string doseLoadStyleDescription = "The loading style for the dose. Available styles are:"
					"\ndicom: normal dicom dose"
					"\nitk: use itk image loading"
					"\nitkDicom: use itk dicom image loading"
					"\nhelax: load a helax dose (choosing this style, the dose path should only be a directory).";

				addOptionWithDefaultValue<std::string>(OPTION_LOAD_STYLE, OPTION_GROUP_REQUIRED, doseLoadStyleDescription,
					defaultLoadingStyle, defaultLoadingStyle,
					't', true, true);
				addInformationForXML(OPTION_LOAD_STYLE, cmdlineparsing::XMLGenerator::paramType::STRING);

				//OPTIONAL
				addOption<std::vector<double> >(OPTION_MODEL_PARAMETERS, OPTION_GROUP_OPTIONAL,
					"The parameters for the radiobiological model.", 'p', false, true);
				addInformationForXML(OPTION_MODEL_PARAMETERS, cmdlineparsing::XMLGenerator::paramType::STRING);

				addOption<std::vector<std::string> >(OPTION_MODEL_PARAMETER_MAPS, OPTION_GROUP_OPTIONAL,
					"The parameters maps as itk readable image files for the radiobiological model.", 'a', false, true);
				addInformationForXML(OPTION_MODEL_PARAMETER_MAPS, cmdlineparsing::XMLGenerator::paramType::INPUT, { "*" });

				addOption<unsigned int>(OPTION_N_FRACTIONS, OPTION_GROUP_OPTIONAL,
					"The number of fractions (n in the formula).", 'f');
				addInformationForXML(OPTION_N_FRACTIONS, cmdlineparsing::XMLGenerator::paramType::INTEGER);

				addOptionWithDefaultValue<std::string>(OPTION_LOAD_STYLE_PARAMETER_MAPS, OPTION_GROUP_OPTIONAL, doseLoadStyleDescription,
					defaultLoadingStyle, defaultLoadingStyle,
					'u', true, true);
				addInformationForXML(OPTION_LOAD_STYLE_PARAMETER_MAPS, cmdlineparsing::XMLGenerator::paramType::STRING);

				parse(argc, argv);
			}

			void BioModelCmdLineParser::validateInput() const
			{
				std::string model = get<std::string>(OPTION_MODEL);

				if (model != "LQ")
				{
					throw cmdlineparsing::InvalidConstraintException("Unknown model: " + model +
					        ".\nPlease refer to the help for valid models.");
				}
				else
				{
                    if (!isSet(OPTION_MODEL_PARAMETERS) && !isSet(OPTION_MODEL_PARAMETER_MAPS)){
                        throw cmdlineparsing::InvalidConstraintException("Either the model parameters or model parameter maps must be specified!");
                    }

                    if ((isSet(OPTION_MODEL_PARAMETERS) && get<std::vector<double> >(OPTION_MODEL_PARAMETERS).size() != 2) || (isSet(OPTION_MODEL_PARAMETER_MAPS) && get<std::vector<std::string> >(OPTION_MODEL_PARAMETER_MAPS).size() != 2))
					{
						throw cmdlineparsing::InvalidConstraintException("The LQ Model requires two parameters or parameter maps!");
					}
				}

				std::string loadStyle = get<std::string>(OPTION_LOAD_STYLE);

				if (loadStyle != "dicom"
				    && loadStyle != "itk"
				    && loadStyle != "itkDicom"
				    && loadStyle != "helax")
				{
					throw cmdlineparsing::InvalidConstraintException("Unknown load style: " + loadStyle +
					        ".\nPlease refer to the help for valid loading style settings.");
				}

				double doseScaling = get<double>(OPTION_DOSE_SCALING);

				if (doseScaling <= 0)
				{
					throw cmdlineparsing::InvalidConstraintException("Negative dose scaling is not allowed. Dose scaling has to be >0.");
				}
			}

			void BioModelCmdLineParser::printHelp() const
			{
				cmdlineparsing::CmdLineParserBase::printHelp();
				std::cout << "Example:" << std::endl << std::endl;
				std::cout << m_programName <<
				          " dose.mhd result.mhd -m LQ -p 0.2 0.02" <<
				          std::endl << std::endl;
				std::cout <<
				          "This will calculate the Linear quadratic (LQ) BioModel from  \"dose.mhd\" and will write the result to \"result.mhd\". "
				          "The alpha and beta parameters for the LQ model are 0.2 and 0.02, respectively."
				          << std::endl;
			}

		}
	}
}
