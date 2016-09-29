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
// @version $Revision: 1221 $ (last changed revision)
// @date    $Date: 2015-12-01 13:43:31 +0100 (Di, 01 Dez 2015) $ (last change date)
// @author  $Author: strubel $ (last changed by)
*/

#include "VoxelizerToolCmdLineParser.h"

namespace rttb
{
	namespace apps
	{
		namespace voxelizerTool
		{

            VoxelizerCmdLineParser::VoxelizerCmdLineParser(int argc, const char** argv, const std::string& name, const std::string& version, 
                const std::string& description, const std::string& contributor, const std::string& category) :
                CmdLineParserBase(name, version, description, contributor, category)
			{
				//REQUIRED
				addOption<std::string>(OPTION_STRUCT_FILE, OPTION_GROUP_REQUIRED,
					"Filename of the structfile (*.dcm)", 's', true);
                addInformationForXML(OPTION_STRUCT_FILE, cmdlineparsing::XMLGenerator::paramType::INPUT, {"dcm"});
				addOption<std::string>(OPTION_REFERENCE_FILE, OPTION_GROUP_REQUIRED,
					"Filename of the reference image (*.dcm)", 'r', true);
                addInformationForXML(OPTION_REFERENCE_FILE, cmdlineparsing::XMLGenerator::paramType::INPUT, { "dcm" });
				addOptionWithDefaultValue<std::string>(OPTION_OUTPUT_FILE_NAME, OPTION_GROUP_REQUIRED,
					"set output file name","out.hdr","out.hdr", 'o', true);
                addInformationForXML(OPTION_OUTPUT_FILE_NAME, cmdlineparsing::XMLGenerator::paramType::OUTPUT, { "nrrd", "hdr" });
				
				addPositionalOption(OPTION_STRUCT_FILE,1);
				addPositionalOption(OPTION_REFERENCE_FILE, 1);
				addPositionalOption(OPTION_OUTPUT_FILE_NAME, 1);

				std::vector<std::string> defaultLoadingStyle;
				defaultLoadingStyle.push_back("dicom");
				
				addOptionWithDefaultValue<std::string>(OPTION_REGEX, OPTION_GROUP_REQUIRED,
					"set a regular expression describing the structs of interest", "", "",'e', true);
                addInformationForXML(OPTION_REGEX, cmdlineparsing::XMLGenerator::paramType::STRING);

				
				addOptionWithDefaultValue<std::vector<std::string>>(OPTION_REFERENCE_FILE_LOAD_STYLE, OPTION_GROUP_REQUIRED,
					"set the load style for the reference file. Available styles are: "
					"dicom: normal dicom dose"
					"itk: use itk image loading.", defaultLoadingStyle, defaultLoadingStyle.at(0), 'y', true);
                addInformationForXML(OPTION_REFERENCE_FILE_LOAD_STYLE, cmdlineparsing::XMLGenerator::paramType::STRINGENUMERATION, { "dicom", "itk" });
				
				//OPTIONAL
				addOption(OPTION_MULTIPLE_STRUCTS, OPTION_GROUP_OPTIONAL,
					"if multiple structs match the regular expression" + OPTION_STRUCT_FILE + ", save all in files\n"
					"If structures 'Kidney_left' and 'Kidney_right' are defined,\n"
					"both are written under the names outputFile_Kidney_left.mhd and outputFile_Kidney_right.mhd",'m');
                addInformationForXML(OPTION_MULTIPLE_STRUCTS, cmdlineparsing::XMLGenerator::paramType::BOOLEAN);
				addOption(OPTION_LEGACY_VOXELIZATION, OPTION_GROUP_OPTIONAL, 
					"to use legacy voxelization",'l');
                addInformationForXML(OPTION_LEGACY_VOXELIZATION, cmdlineparsing::XMLGenerator::paramType::BOOLEAN);
				addOption(OPTION_BOOLEAN_VOXELIZATION, OPTION_GROUP_OPTIONAL,
					"Determines if the voxelization should be binarized (only values 0 or 1), the threshold value is by 0.5",'z');
                addInformationForXML(OPTION_BOOLEAN_VOXELIZATION, cmdlineparsing::XMLGenerator::paramType::BOOLEAN);
				addOption(OPTION_ADDSTRUCTURES, OPTION_GROUP_OPTIONAL,
					"Voxelizes multiple structs in one result file.",'a');
                addInformationForXML(OPTION_ADDSTRUCTURES, cmdlineparsing::XMLGenerator::paramType::BOOLEAN);
				addOption(OPTION_ALLOW_SELF_INTERSECTIONS, OPTION_GROUP_OPTIONAL,
					"If self intersections of polygons should be tolerated.",'i');
                addInformationForXML(OPTION_ALLOW_SELF_INTERSECTIONS, cmdlineparsing::XMLGenerator::paramType::BOOLEAN);
							
				parse(argc, argv);
			}

			void VoxelizerCmdLineParser::validateInput() const
			{
				
				std::vector<std::string> referenceLoadStyle = get<std::vector<std::string> >(OPTION_REFERENCE_FILE_LOAD_STYLE);
				std::string referenceLoadStyleString = referenceLoadStyle.at(0);

				if (referenceLoadStyleString != "dicom" && referenceLoadStyleString != "itk")
				{
					throw cmdlineparsing::InvalidConstraintException("Unknown load style for reference file:" +
						referenceLoadStyleString+
						".\nPlease refer to the help for valid loading style settings.");
				}

				if (get<std::string>(OPTION_OUTPUT_FILE_NAME).find('.') == std::string::npos)
				{
					throw cmdlineparsing::InvalidConstraintException(OPTION_OUTPUT_FILE_NAME + " has to specify a file format (e.g. output.hdr). None is given: " +
						get<std::string>(OPTION_OUTPUT_FILE_NAME) );
				}
			
			}

			void VoxelizerCmdLineParser::printHelp() const
			{
				cmdlineparsing::CmdLineParserBase::printHelp();
				std::cout <<
					"Example: VoxelizerTool -s structFile.dcm -r referenceFile.dcm -e Kidney -o outputFile.mhd -m" <<
					std::endl;
				std::cout <<
					"Computes a voxelization file outputFile.mhd based on the DICOMRT-STRUCT structFile.dcm "
					"in the geometry of referenceFile.dcm where ";
				std::cout << "the name of the struct matches the regular expression 'Kidney'.\n";
				std::cout << "If structures 'Kidney_left' and 'Kidney_right' are defined, ";
				std::cout <<
					"both are written under the names outputFile_Kidney_left.mhd and outputFile_Kidney_right.mhd (parameter -m)."
					<<
					std::endl;
			}
		}
	}
}