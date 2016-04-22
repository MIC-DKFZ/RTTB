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

#include "itkMacro.h"

#include "rttbVoxelizerHelper.h"
#include "rttbMaskProcess.h"
#include "rttbMaskWriter.h"
#include "rttbStructDataReader.h"
#include "rttbCommandOptions.h"

int main(int argc, char* argv[])
{
	typedef rttb::core::MaskAccessorInterface::MaskAccessorPointer MaskAccessorPointer;

	boost::shared_ptr<rttb::apps::voxelizer::CommandOptions> co =
	    boost::make_shared<rttb::apps::voxelizer::CommandOptions>();

	if (!co->command(argc, argv))
	{
		return 1;
	}

	if (co->isReturnAfterHelp())
	{
		return 0;
	}

	rttb::apps::voxelizer::Parameters params = co->getParameters();

	boost::shared_ptr<rttb::apps::voxelizer::StructDataReader> reader =
	    boost::make_shared<rttb::apps::voxelizer::StructDataReader>(params.structFile,
	            params.referenceFile, params.referenceFileLoadStyle);
	std::cout << "reading reference and structure file...";

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

	for (size_t i = 0; i < params.regEx.size(); i++)
	{
		std::vector<int> indexOfCorrectElements;
		indexOfCorrectElements = rttb::apps::voxelizer::filterForExpression(reader->getAllLabels(),
		                         params.regEx.at(i));
		std::copy(indexOfCorrectElements.begin(), indexOfCorrectElements.end(),
		          std::back_inserter(listOfCorrectElements));
	}

	std::cout << "done." << std::endl;

	boost::shared_ptr<rttb::apps::voxelizer::MaskProcess> maskProcessor =
	    boost::make_shared<rttb::apps::voxelizer::MaskProcess>(reader->getStructureSetPointer(),
	            reader->getDoseAccessorPointer(),
	            params.legacyVoxelization, params.allowSelfIntersections);

	if (!listOfCorrectElements.empty())
	{
		std::string fileName = rttb::apps::voxelizer::getFilenameWithoutEnding(
		                           params.outputFilename);
		std::string fileEnding = rttb::apps::voxelizer::getFileEnding(params.outputFilename);

		std::vector<MaskAccessorPointer> maskVector;

		if (params.addStructures)
		{
			std::string labelName;

			for (size_t i = 0; i < listOfCorrectElements.size(); i++)
			{
				int labelIndex = listOfCorrectElements.at(i);
				maskVector.push_back(maskProcessor->createMask(labelIndex));
				std::vector<std::string> labelVector = reader->getAllLabels();
				std::string labelOfInterest = labelVector.at(labelIndex);
				rttb::apps::voxelizer::removeSpecialCharacters(labelOfInterest);
				labelName += "_" + labelOfInterest;

			}

			boost::shared_ptr<rttb::apps::voxelizer::MaskWriter> writer =
			    boost::make_shared<rttb::apps::voxelizer::MaskWriter>(maskVector,
			            params.booleanVoxelization);
			writer->writeMaskToFile(fileName  + labelName + fileEnding);

		}
		else
		{
			unsigned int maxIterationCount = 1;

			if (params.multipleStructs)
			{
				maxIterationCount = listOfCorrectElements.size();
			}

			for (unsigned int i = 0; i < maxIterationCount; i++)
			{
				std::cout << "creating mask...";
				maskVector.push_back(maskProcessor->createMask(listOfCorrectElements.at(i)));
				std::cout << "done" << std::endl;
				int labelIndex = listOfCorrectElements.at(i);
				std::vector<std::string> labelVector = reader->getAllLabels();
				std::string labelOfInterest = labelVector.at(labelIndex);
				rttb::apps::voxelizer::removeSpecialCharacters(labelOfInterest);
				boost::shared_ptr<rttb::apps::voxelizer::MaskWriter> MW =
				    boost::make_shared<rttb::apps::voxelizer::MaskWriter>(maskVector,
				            params.booleanVoxelization);

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

