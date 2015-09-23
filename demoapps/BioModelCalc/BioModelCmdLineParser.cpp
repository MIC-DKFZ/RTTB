#include "BioModelCmdLineParser.h"

namespace rttb
{
	namespace apps
	{
		namespace bioModelCalc
		{

			BioModelCmdLineParser::BioModelCmdLineParser(int argc, const char** argv, const std::string& name,
			        const std::string& version) :
				CmdLineParserBase(name, version)
			{
				addOption<std::string>(OPTION_DOSE_FILE, OPTION_GROUP_REQUIRED,
				                       "The name of the dose file. Can be omitted if used as "
				                       "positional argument (see above).", 'd', true);
				addOption<std::string>(OPTION_OUTPUT_FILE, OPTION_GROUP_REQUIRED,
				                       "The name of the output file. Can be omitted if used as "
				                       "positional argument (see above).", 'o', true);
				addPositionalOption(OPTION_DOSE_FILE, 1);
				addPositionalOption(OPTION_OUTPUT_FILE, 1);
				addOptionWithDefaultValue<std::string>(OPTION_MODEL, OPTION_GROUP_REQUIRED,
				                                       "The used radiobiological model the dose should be analyzed with. Available models are:\n \"LQ\"", "LQ", "LQ", 'm');
				addOption<std::vector<double> >(OPTION_MODEL_PARAMETERS, OPTION_GROUP_REQUIRED,
				                                "The parameters for the radiobiological model.", 'p', false, true);
				std::vector<std::string> defaultLoadingStyle;
				defaultLoadingStyle.push_back("itk");
				addOptionWithDefaultValue<std::vector<std::string> >(OPTION_LOAD_STYLE, OPTION_GROUP_REQUIRED,
				        "The loading style for the dose. Available styles are:\n"
				        "\"dicom\": normal dicom dose\n"
				        "\"virtuos\": load of a virtuos dose (This style is a multi argument. The second argument specifies the virtuos plan file, e.g. : \"--loadStyle virtuos myFavorite.pln\")\n"
				        "\"itk\": use itk image loading\n\"helax\": load a helax dose (choosing this style, the dose path should only be a directory).",
				        defaultLoadingStyle, defaultLoadingStyle.at(0),
				        's', true, true);

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
					if (get<std::vector<double> >(OPTION_MODEL_PARAMETERS).size() != 2)
					{
						throw cmdlineparsing::InvalidConstraintException("The LQ Model requires two parameters!");
					}
				}

				std::vector<std::string> loadStyle = get<std::vector<std::string> >(OPTION_LOAD_STYLE);
				std::string loadStyleAbbreviation = loadStyle.at(0);

				if (loadStyleAbbreviation != "dicom" && loadStyleAbbreviation != "virtuos" && loadStyleAbbreviation != "itk"
				    && loadStyleAbbreviation != "helax")
				{
					throw cmdlineparsing::InvalidConstraintException("Unknown load style:" + loadStyleAbbreviation +
					        ".\nPlease refer to the help for valid loading style settings.");
				}

				if (loadStyleAbbreviation == "virtuos")
				{
					if (loadStyle.size() < 2)
					{
						throw cmdlineparsing::InvalidConstraintException("Cannot load virtuos dose. Plan file is missing. Specify plan file as 2nd io style argument.");
					}
				}
			}

			void BioModelCmdLineParser::printHelp() const
			{
				cmdlineparsing::CmdLineParserBase::printHelp();
				std::cout << "Example:" << std::endl << std::endl;
				std::cout << m_programName <<
				          " dose.mhd result.mhd -s itk -m LQ -p 0.2 0.02" <<
				          std::endl << std::endl;
				std::cout <<
				          "This will calculate the Linear quadratic (LQ) BioModel from  \"dose.mhd\" and will write the result to \"result.mhd\". "
				          "The alpha and beta parameters for the LQ model are 0.2 and 0.02, respectively."
				          << std::endl;
			}

		}
	}
}
