#include "BioModelCmdLineParser.h"

namespace rttb
{
	namespace apps
	{
		namespace bioModelCalc
		{
			BioModelCmdLineParser::BioModelCmdLineParser(int argc, const char** argv, const std::string& name, const std::string& version,
				const std::string& description, const std::string& contributor, const std::string& category, bool virtuosSupport) :
				CmdLineParserBase(name, version, description, contributor, category), _virtuosSupport(virtuosSupport)
			{
				//REQUIRED
				addOption<std::string>(OPTION_DOSE_FILE, OPTION_GROUP_REQUIRED,
					"The name of the dose file. Can be omitted if used as "
					"positional argument (see above).", 'd', true);
				addInformationForXML(OPTION_DOSE_FILE, cmdlineparsing::XMLGenerator::paramType::INPUT, { "mhd", "*" });

				addOption<std::string>(OPTION_OUTPUT_FILE, OPTION_GROUP_REQUIRED,
					"The name of the output file. Can be omitted if used as "
					"positional argument (see above).", 'o', true);
				addInformationForXML(OPTION_OUTPUT_FILE, cmdlineparsing::XMLGenerator::paramType::OUTPUT, { "mhd", "*" });

				addPositionalOption(OPTION_DOSE_FILE, 1);
				addPositionalOption(OPTION_OUTPUT_FILE, 1);

				addOptionWithDefaultValue<std::string>(OPTION_MODEL, OPTION_GROUP_REQUIRED,
					"The used radiobiological model the dose should be analyzed with. Available models are:\n \"LQ\", Formula: exp(-(alpha * D + beta * D^2/n))",
					"LQ", "LQ", 'm');
				addInformationForXML(OPTION_MODEL, cmdlineparsing::XMLGenerator::paramType::STRINGENUMERATION, { "LQ" });

				addOptionWithDefaultValue<double>(OPTION_DOSE_SCALING, OPTION_GROUP_REQUIRED,
					"Dose scaling that should be applied.", 1.0, "1.0", 'e');
				addInformationForXML(OPTION_DOSE_SCALING, cmdlineparsing::XMLGenerator::paramType::DOUBLE);

				std::vector<std::string> defaultLoadingStyle;
				defaultLoadingStyle.push_back("itk");

				std::string doseLoadStyleDescription = "The loading style for the dose. Available styles are:\n"
					"\"dicom\": normal dicom dose\n"
					"\"itk\": use itk image loading\n"
					"\"helax\": load a helax dose (choosing this style, the dose path should only be a directory).";


				if (_virtuosSupport)
				{
					doseLoadStyleDescription += "\n"
						"\"virtuos\": load of a virtuos dose (This style is a multi argument. The second argument specifies the virtuos plan file, e.g. : \"--"
						+ OPTION_LOAD_STYLE + " virtuos myFavorite.pln\")";
				}

				addOptionWithDefaultValue<std::vector<std::string> >(OPTION_LOAD_STYLE, OPTION_GROUP_REQUIRED, doseLoadStyleDescription,
					defaultLoadingStyle, defaultLoadingStyle.at(0),
					't', true, true);
				addInformationForXML(OPTION_LOAD_STYLE, cmdlineparsing::XMLGenerator::paramType::STRINGENUMERATION, { "itk", "dicom", "helax" });

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

				addOptionWithDefaultValue<std::vector<std::string> >(OPTION_LOAD_STYLE_PARAMETER_MAPS, OPTION_GROUP_OPTIONAL, doseLoadStyleDescription,
					defaultLoadingStyle, defaultLoadingStyle.at(0),
					'u', true, true);
				addInformationForXML(OPTION_LOAD_STYLE_PARAMETER_MAPS, cmdlineparsing::XMLGenerator::paramType::STRINGENUMERATION, { "itk", "dicom", "helax" });

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

				std::vector<std::string> loadStyle = get<std::vector<std::string> >(OPTION_LOAD_STYLE);
				std::string loadStyleAbbreviation = loadStyle.at(0);

				if (loadStyleAbbreviation != "dicom" && (!_virtuosSupport || loadStyleAbbreviation != "virtuos")
				    && loadStyleAbbreviation != "itk"
				    && loadStyleAbbreviation != "helax")
				{
					throw cmdlineparsing::InvalidConstraintException("Unknown load style: " + loadStyleAbbreviation +
					        ".\nPlease refer to the help for valid loading style settings.");
				}
				else if (_virtuosSupport && loadStyleAbbreviation == "virtuos")
				{
					if (loadStyle.size() < 2)
					{
						throw cmdlineparsing::InvalidConstraintException("Cannot load virtuos dose. Plan file is missing. Specify plan file as 2nd io style argument.");
					}
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
