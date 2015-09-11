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
// @version $Revision: 5603 $ (last changed revision)
// @date    $Date: 2015-08-24 15:17:30 +0200 (Mo, 24 Aug 2015) $ (last change date)
// @author  $Author: strubel $ (last changed by)
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
				PARAM_LEGACY("legacyVoxelization"),
				PARAM_BOOST("boostVoxelization"),
				PARAM_BOOLEANVOXELIZATION("booleanVoxelization"),
				PARAM_ADDSTRUCTURES("addStructures")
			{

				params.multipleStructs = false;
				params.legacyVoxelization = false;
				params.booleanVoxelization  = false;
				params.addStructures = false;

				po::options_description required("Required arguments");
				addOption(required, PARAM_STRUCT_FILE, "s", po::value<std::string>(&params.structFile),
				          "Filename of the structfile (*.dcm)");
				addOption(required, PARAM_REFERENCE_FILE, "r", po::value<std::string>(&params.referenceFile),
				          "Filename of the reference image (*.dcm)");

				addOption(required, PARAM_REGEX, "e",
				          po::value<std::vector<std::string>>(&params.regEx)->multitoken(),
				          "set a regular expression describing the structs of interest");

				po::options_description optional("Optional arguments");
				addOption(optional, PARAM_HELP, "h", nullptr, "Display help message");
				addOption(optional, PARAM_OUT_FILE, "o",
				          po::value<std::string>(&params.outputFilename)->default_value("out.hdr"), "set output file name ");
				addOption(optional, PARAM_MULTISTRUCT, "m", nullptr,
				          "if multiple structs are found, save all in files");
				addOption(optional, PARAM_BOOST, "b", nullptr, "for boost voxelization");
				addOption(optional, PARAM_LEGACY, "l", nullptr,
				          "for legacy voxelization");
				addOption(optional, PARAM_BOOLEANVOXELIZATION, "v", nullptr,
				          "Determines if the voxelization should have only boolean values (0 or 1)");
				addOption(optional, PARAM_ADDSTRUCTURES, "a", nullptr,
				          "");

				description.add(required).add(optional);
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

			void CommandOptions::showHelp()const
			{
				std::cout << "Usage: VoxelizerTool -s <arg> -r <arg> [optional] \n";
				std::cout << description << std::endl;
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
					_minArguments = 7;
					po::store(po::command_line_parser(argc, argv).options(description).run(), var);
					po::notify(var);

					if (argc < _minArguments)
					{
						showHelp();
						return false;
					}

					if (var.count(PARAM_HELP))
					{
						showHelp();
						return true;
					}

					if (!var.count(PARAM_STRUCT_FILE))
					{
						throw std::runtime_error("Please use the parameter -s or --structfile");
					}
					else
					{
						if (getFileEnding(params.structFile) !=  ".dcm")
						{
							throw std::runtime_error("Please check your struct file: " + params.structFile);
						}
					}

					if (!var.count(PARAM_REFERENCE_FILE))
					{
						throw std::runtime_error("Please use the parameter -r or --referencefile");
					}
					else
					{
						if (getFileEnding(params.referenceFile) != ".dcm")
						{
							throw std::runtime_error("Please check your reference file: " + params.referenceFile);
						}
					}

					if (!var.count(PARAM_OUT_FILE))
					{
						params.outputFilename = "out.hdr";
					}

					if (var.count(PARAM_REGEX))
					{

						if (var.count(PARAM_MULTISTRUCT))
						{
							params.multipleStructs = true;
						}
						else
						{
							params.multipleStructs = false;
						}
					}

					if (var.count(PARAM_LEGACY))
					{
						params.legacyVoxelization = true;
					}

					if (var.count(PARAM_BOOLEANVOXELIZATION))
					{
						params.booleanVoxelization = true;
					}

					if (var.count(PARAM_ADDSTRUCTURES))
					{
						params.addStructures = true;
						params.multipleStructs = false;
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