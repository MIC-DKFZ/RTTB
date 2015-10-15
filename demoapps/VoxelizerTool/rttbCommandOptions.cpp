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
// @version $Revision$ (last changed revision)
// @date    $Date$ (last change date)
// @author  $Author$ (last changed by)
*/
#include <iostream>

#include "rttbCommandOptions.h"
#include "rttbVoxelizerHelper.h"

namespace rttb
{
	namespace apps
	{
		namespace voxelizer
		{

			CommandOptions::CommandOptions():
				PARAM_HELP("help"),
				PARAM_STRUCT_FILE("structFile"),
				PARAM_REFERENCE_FILE("referenceFile"),
				PARAM_OUT_FILE("output"),
				PARAM_REGEX("struct"),
				PARAM_MULTISTRUCT("multipleStructs"),
				PARAM_LEGACY_VOXELIZATION("legacyVoxelization"),
				PARAM_BOOST_VOXELIZATION("boostVoxelization"),
				PARAM_BOOLEAN_VOXELIZATION("booleanVoxelization"),
				PARAM_ADDSTRUCTURES("addStructures"),
				PARAM_ALLOW_SELF_INTERSECTIONS("selfIntersections"),
				_returnAfterHelp(false)
			{

				_params.multipleStructs = false;
				_params.legacyVoxelization = false;
				_params.booleanVoxelization  = false;
				_params.addStructures = false;

				std::vector<std::string> defaultRefLoadingStyle;
				defaultRefLoadingStyle.push_back("dicom");


				po::options_description required("Required arguments");
				addOption(required, PARAM_STRUCT_FILE, "s", po::value<std::string>(&_params.structFile)->required(),
				          "Filename of the structfile (*.dcm)");
				addOption(required, PARAM_REFERENCE_FILE, "r", po::value<std::string>(&_params.referenceFile)->required(),
				          "Filename of the reference image (*.dcm)");
				addOption(required, PARAM_REFERENCE_FILE_LOAD_STYLE, "y",
				          po::value<std::vector<std::string> >(&_params.referenceFileLoadStyle)->required()->default_value(
				              defaultRefLoadingStyle, "dicom"),
				          "set the load style for the reference file. Available styles are:\n"
				          "\"dicom\": normal dicom dose\n"
				          "\"itk\": use itk image loading.");
				addOption(required, PARAM_REGEX, "e",
				          po::value<std::vector<std::string>>(&_params.regEx)->multitoken()->required(),
				          "set a regular expression describing the structs of interest");
				addOption(required, PARAM_OUT_FILE, "o",
				          po::value<std::string>(&_params.outputFilename)->default_value("out.hdr"), "set output file name ");
				addOption(required, PARAM_BOOST_VOXELIZATION, "b", po::bool_switch()->default_value(true), "to use boost voxelization");

				po::options_description optional("Optional arguments");
				addOption(optional, PARAM_HELP, "h", nullptr, "Display help message");
				addOption(optional, PARAM_MULTISTRUCT, "m", po::bool_switch(&_params.multipleStructs)->default_value(false),
				          "if multiple structs match the regular expression (--struct), save all in files");
				addOption(optional, PARAM_LEGACY_VOXELIZATION, "l", po::bool_switch(&_params.legacyVoxelization)->default_value(false),
				          "to use legacy voxelization");
				addOption(optional, PARAM_BOOLEAN_VOXELIZATION, "v",
				          po::bool_switch(&_params.booleanVoxelization)->default_value(false),
				          "Determines if the voxelization should be binarized (only values 0 or 1)");
				addOption(optional, PARAM_ADDSTRUCTURES, "a", nullptr, "Voxelizes multiple structs in one result file.");
				addOption(optional, PARAM_ALLOW_SELF_INTERSECTIONS, "i",
				          po::bool_switch(&_params.allowSelfIntersections)->default_value(false),
				          "If self intersections of polygons should be tolerated.");

				_description.add(required).add(optional);
			}

			void CommandOptions::addOption(po::options_description& o, const std::string& name,
			                               const std::string& shortcut, const po::value_semantic* valueSemantic,
			                               const std::string& description)
			{
				if (valueSemantic)
				{
					o.add_options()((name + std::string(",") + shortcut).c_str(), valueSemantic, description.c_str());
				}
				else
				{
					o.add_options()((name + std::string(",") + shortcut).c_str(), description.c_str());
				}
			}

			void CommandOptions::showHelp() const
			{
				std::cout << "Usage: VoxelizerTool [options] \n";
				std::cout << _description << std::endl;
				std::cout << "Example: VoxelizerTool -s structFile.dcm -r referenceFile.dcm -e Kidney -o outputFile.mhd -m" <<
				          std::endl;
				std::cout <<
				          "Computes a voxelization file outputFile.mhd based on the DICOMRT-STRUCT structFile.dcm in the geometry of referenceFile.dcm where";
				std::cout << "the name of the struct matches the regular expression 'Kidney'." << std::endl;
				std::cout << "If structures 'Kidney_left' and 'Kidney_right' are defined, ";
				std::cout <<
				          "both are written under the names outputFile_Kidney_left.mhd and outputFile_Kidney_right.mhd (parameter -m)." <<
				          std::endl;
			}

			bool CommandOptions::command(int argc, char* argv[])
			{
				try
				{
					po::variables_map var;
					po::store(po::command_line_parser(argc, argv).options(_description).run(), var);

					if (var.count(PARAM_HELP))
					{
						showHelp();
						_returnAfterHelp = true;
						return true;
					}

					po::notify(var);

					if (var.count(PARAM_BOOST_VOXELIZATION))
					{
						_params.legacyVoxelization = false;
					}

					if (_params.outputFilename.find('.') == std::string::npos)
					{
						std::cout << "--output has to specify a file format (e.g. output.hdr). None is given: " << _params.outputFilename <<
						          std::endl;
						return false;
					}

					if (_params.referenceFileLoadStyle.empty() || (_params.referenceFileLoadStyle.at(0) != "dicom"
					        && _params.referenceFileLoadStyle.at(0) != "itk"))
					{
						std::cout << "Unknown load style:" + _params.referenceFileLoadStyle.at(0) +
						          ".\nPlease refer to the help for valid loading style settings." << std::endl;
						return false;
					}

					if (var.count(PARAM_ADDSTRUCTURES))
					{
						_params.addStructures = true;
						_params.multipleStructs = false;
					}
				}
				catch (const std::exception& e)
				{
					std::cout << "Error: " << e.what() << std::endl;
					return false;
				}

				return true;
			}
		}
	}
}