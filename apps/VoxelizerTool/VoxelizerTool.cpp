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
// @version $Revision: 1390 $ (last changed revision)
// @date    $Date: 2016-07-13 09:57:44 +0200 (Mi, 13 Jul 2016) $ (last change date)
// @author  $Author: strubel $ (last changed by)
*/
#include <iostream>

#include "itkMacro.h"

#include "VoxelizerToolHelper.h"
#include "rttbMaskProcess.h"
#include "rttbMaskWriter.h"
#include "rttbStructDataReader.h"
#include "VoxelizerToolCmdLineParser.h"
#include "VoxelizerToolApplicationData.h"
#include "RTToolboxConfigure.h"

rttb::apps::voxelizerTool::ApplicationData appData;

int main(int argc, const char** argv)
{
	typedef rttb::core::MaskAccessorInterface::MaskAccessorPointer MaskAccessorPointer;

    const std::string appCategory = "RT-Toolbox App";
    const std::string appName = "VoxelizerTool";
    const std::string appDesc = "An App to voxelize RT structures in a reference image.";
    const std::string appContributor = "SIDT@DKFZ";
    const std::string appVersion = RTTB_FULL_VERSION_STRING;

	boost::shared_ptr<rttb::apps::voxelizerTool::VoxelizerCmdLineParser> argParser;

	try
	{
        argParser = boost::make_shared<rttb::apps::voxelizerTool::VoxelizerCmdLineParser>(argc, argv, appName, appVersion, appDesc, appContributor, appCategory);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 5;
	}

	// This is vital. The application needs to exit if the "help" or "version" parameter is set
	// because this means the other parameters won't be parsed.

	if (argParser->isSet(argParser->OPTION_HELP) || argParser->isSet(argParser->OPTION_VERSION) || argParser->isSet(argParser->OPTION_XML))
	{
		return 0;
	}

	rttb::apps::voxelizerTool::populateAppData(argParser, appData);

		std::cout << std::endl << "*******************************************" << std::endl;
		std::cout << "Struct file: " << appData._structFile << std::endl;
		std::cout << "Reference Image: " << appData._referenceFile << std::endl;
		std::cout << "Output file: " << appData._outputFilename << std::endl;
		std::cout << "Struct regex: " << appData._regEx << std::endl;
		std::cout << "Legacy Voxelization: " << appData._legacyVoxelization << std::endl;
		std::cout << "Add structures: " << appData._addStructures << std::endl;
		std::cout << "Multiple Struct: " << appData._multipleStructs << std::endl << std::endl;

		boost::shared_ptr<rttb::apps::voxelizerTool::StructDataReader> reader =
			boost::make_shared<rttb::apps::voxelizerTool::StructDataReader>(appData._structFile,
			appData._referenceFile, appData._referenceFileLoadStyle);
		std::cout << "reading reference and structure file..." << std::endl;

		try
		{
			reader->read();
		}
		catch (rttb::core::Exception& e)
		{
			std::cerr << "RTTB Error!!!" << std::endl;
			std::cerr << e.what() << std::endl;
			return 2;
		}
		catch (const std::exception& e)
		{
			std::cerr << "Error!!!" << std::endl;
			std::cerr << e.what() << std::endl;
			return 2;
		}
		catch (...)
		{
			std::cerr << "Error!!! unknown error while reading input image." << std::endl;
			return 2;
		}

		std::cout << "done." << std::endl;

		std::cout << "searching for structs...";
		std::vector<int> listOfCorrectElements;

        std::vector<int> indexOfCorrectElements;
        indexOfCorrectElements = rttb::apps::voxelizerTool::filterForExpression(reader->getAllLabels(),
            appData._regEx);
        std::copy(indexOfCorrectElements.begin(), indexOfCorrectElements.end(),
            std::back_inserter(listOfCorrectElements));

		std::cout << "done." << std::endl;

		boost::shared_ptr<rttb::apps::voxelizerTool::MaskProcess> maskProcessor =
			boost::make_shared<rttb::apps::voxelizerTool::MaskProcess>(reader->getStructureSetPointer(),
			reader->getDoseAccessorPointer(),
			appData._legacyVoxelization, appData._allowSelfIntersections);

		if (!listOfCorrectElements.empty())
		{
			std::string fileName = rttb::apps::voxelizerTool::getFilenameWithoutEnding(
				appData._outputFilename);
			std::string fileEnding = rttb::apps::voxelizerTool::getFileEnding(appData._outputFilename);

			std::vector<MaskAccessorPointer> maskVector;


			std::vector<std::string> labelVector = reader->getAllLabels();

			if (appData._addStructures)
			{
				std::string labelName;

				for (unsigned int i = 0; i < listOfCorrectElements.size(); i++)
				{
					int labelIndex = listOfCorrectElements.at(i);
					maskVector.push_back(maskProcessor->createMask(labelIndex));

					std::string labelOfInterest = labelVector.at(labelIndex);
					rttb::apps::voxelizerTool::removeSpecialCharacters(labelOfInterest);
					labelName += "_" + labelOfInterest;

				}

				boost::shared_ptr<rttb::apps::voxelizerTool::MaskWriter> writer =
					boost::make_shared<rttb::apps::voxelizerTool::MaskWriter>(maskVector,
					appData._booleanVoxelization);
				writer->writeMaskToFile(fileName + labelName + fileEnding);

			}
			else
			{
				unsigned int maxIterationCount = 1;

				if (appData._multipleStructs)
				{
					maxIterationCount = listOfCorrectElements.size();
				}

				for (unsigned int i = 0; i < maxIterationCount; i++)
				{
					std::cout << "creating mask...";
					maskVector.push_back(maskProcessor->createMask(listOfCorrectElements.at(i)));
					std::cout << "done" << std::endl;
					int labelIndex = listOfCorrectElements.at(i);
					std::string labelOfInterest = labelVector.at(labelIndex);
					rttb::apps::voxelizerTool::removeSpecialCharacters(labelOfInterest);
					boost::shared_ptr<rttb::apps::voxelizerTool::MaskWriter> MW =
						boost::make_shared<rttb::apps::voxelizerTool::MaskWriter>(maskVector,
						appData._booleanVoxelization);

					try
					{
						MW->writeMaskToFile(fileName + "_" + labelOfInterest + fileEnding);
					}
					catch (itk::ExceptionObject& err)
					{
						std::cerr << "ExceptionObject caught !" << std::endl;
						std::cerr << err << std::endl;
						return 3;
					}
					catch (const std::exception& e)
					{
						std::cerr << "Error!!!" << std::endl;
						std::cerr << e.what() << std::endl;
						return 3;
					}
					catch (...)
					{
						std::cerr << "Error!!! unknown error while reading input image." << std::endl;
						return 3;
					}
				}

			}
		}
		else
		{
			std::cout << "No struct found" << std::endl;
		}
	
	return 0;
}
