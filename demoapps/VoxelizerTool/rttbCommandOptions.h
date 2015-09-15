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
// @version $Revision: 5506 $ (last changed revision)
// @date    $Date: 2015-07-30 14:45:24 +0200 (Do, 30 Jul 2015) $ (last change date)
// @author  $Author: strubel $ (last changed by)
*/
#ifndef __CommandOptions_h
#define __CommandOptions_h

#include <boost/program_options.hpp>

namespace rttb
{
	namespace apps
	{
		namespace voxelizer
		{
			namespace po = boost::program_options;

			/**	@brief Simple struct holding all variables. */
			struct Parameters
			{
				/**	@brief Filename of the StructFile*/
				std::string structFile;
				/**	@brief Filename of the ReferenceFile*/
				std::string referenceFile;
				/**	@brief  Output Filename*/
				std::string outputFilename;
				/**	@brief  Expressions from User*/
				std::vector<std::string> regEx;
				/**	@brief  for more than one struct*/
				bool multipleStructs;
				/**	@brief add: legacyVoxelization=false means boostVoxelization is turned on*/
				bool legacyVoxelization;
				/** @brief voxelization should be binarized to value 0 or 1 */
				bool booleanVoxelization ;
				/** @brief multiple structures voxelization should be combined in one file*/
				bool addStructures;

			};

			/*! @class CommandOptions
			@brief CommandOptions Class, everything about the command line
			*/
			class CommandOptions
			{

			public:
				/**
				@details add the description for command line options
				* add parameter --h or --help for help informations,
				* --Structfile= to set a struct file,
				* --Reference= to set the reference file,
				* --Output= to set an output file name and
				* --struct= to add an expression like(SPINE).
				*/
				CommandOptions();
				/**
				@brief		Validates the given input parameters and fills the corresponding variables.
				@param argc	Number of arguments.
				@param argv Array of individual arguments.
				@return		true if all arguments were valid (i.e. none were missing or incorrect), otherwise false.
				*/
				bool command(int argc, char* argv[]);

				const Parameters& getParameters() const
				{
					return _params;
				}

			private:
				/**
				*@brief Adds an option to the given option container.
				*@param o				Option container the new option should be added to.
				*@param name			Name of the new option (i.e. the actual parameter, e.g. "test" adds the option with the parameter "--test").
				*@param shortcut		A single letter that is used as a shortcut for this option (e.g. if the name is "test, the shortcut might be "t", so you can call the parameter with "-t" instead of "--test").
				*@param valueSemantic	The desired value semantic object, i.e. the object that binds this option's value to a variable.
				*@param description		The description text for this option.
				*/
				void addOption(po::options_description& o, const std::string& name, const std::string& shortcut,
				               const po::value_semantic* valueSemantic, const std::string& description);

				/*! @brief print out the dicription.*/
				void showHelp()const;

				const std::string PARAM_STRUCT_FILE;
				const std::string PARAM_REFERENCE_FILE;
				const std::string PARAM_OUT_FILE;
				const std::string PARAM_REGEX;
				const std::string PARAM_MULTISTRUCT;
				const std::string PARAM_HELP;
				const std::string PARAM_LEGACY_VOXELIZATION;
				const std::string PARAM_BOOST_VOXELIZATION;
				const std::string PARAM_BOOLEAN_VOXELIZATION;
				const std::string PARAM_ADDSTRUCTURES;

				/*! create description object */
				po::options_description _description;
				Parameters _params;

				/**@brief minimum arguments that must be present on the Command Line*/
				int _minArguments;
			};
		}
	}
}
#endif