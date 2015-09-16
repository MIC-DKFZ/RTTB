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
		return EXIT_FAILURE;
	}

	if (co->isReturnAfterHelp())
	{
		return EXIT_SUCCESS;
	}

	rttb::apps::voxelizer::Parameters params = co->getParameters();

	boost::shared_ptr<rttb::apps::voxelizer::StructDataReader> SDR;

	try
	{
		SDR = boost::make_shared<rttb::apps::voxelizer::StructDataReader>(params.structFile,
		        params.referenceFile);
	}
	catch (rttb::core::Exception& e)
	{
		std::cerr << "RTTB Error!!!" << std::endl;
		std::cerr << e.what() << std::endl;
		return 1;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error!!!" << std::endl;
		std::cerr << e.what() << std::endl;
		return 1;
	}
	catch (...)
	{
		std::cerr << "Error!!! unknown error while reading input image." << std::endl;
		return 1;
	}

	std::vector<int> listOfCorrectElements;

	for (int i = 0; i < params.regEx.size(); i++)
	{
		std::vector<int> indexOfCorrectElements;
		indexOfCorrectElements = rttb::apps::voxelizer::filterForExpression(SDR->getAllLabels(),
		                         params.regEx.at(i));
		std::copy(indexOfCorrectElements.begin(), indexOfCorrectElements.end(), std::back_inserter(listOfCorrectElements));
	}

	boost::shared_ptr<rttb::apps::voxelizer::MaskProcess> MP =
	    boost::make_shared<rttb::apps::voxelizer::MaskProcess>(SDR->getStructureSetPointer(),
	            SDR->getDoseAccessorPointer(),
	            params.legacyVoxelization);

	if (!listOfCorrectElements.empty())
	{
		std::string fileName = rttb::apps::voxelizer::getFilenameWithoutEnding(
		                           params.outputFilename);
		std::string fileEnding = rttb::apps::voxelizer::getFileEnding(params.outputFilename);

		std::vector<MaskAccessorPointer> maskVector;

		if (params.addStructures)
		{
			std::string labelName;

			for (int i = 0; i < listOfCorrectElements.size(); i++)
			{
				maskVector.push_back(MP->createMask(listOfCorrectElements.at(i)));
				int labelIndex = listOfCorrectElements.at(i);
				std::vector<std::string> labelVector = SDR->getAllLabels();
				std::string labelOfInterest = labelVector.at(labelIndex);
				rttb::apps::voxelizer::removeSpecialCharacters(labelOfInterest);
				labelName += "_" + labelOfInterest;

			}

			boost::shared_ptr<rttb::apps::voxelizer::MaskWriter> MW =
			    boost::make_shared<rttb::apps::voxelizer::MaskWriter>(maskVector,
			            params.booleanVoxelization);
			MW->writeMaskToFile(fileName  + labelName + fileEnding);

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
				maskVector.push_back(MP->createMask(listOfCorrectElements.at(i)));
				int labelIndex = listOfCorrectElements.at(i);
				std::vector<std::string> labelVector = SDR->getAllLabels();
				std::string labelOfInterest = labelVector.at(labelIndex);
				rttb::apps::voxelizer::removeSpecialCharacters(labelOfInterest);
				boost::shared_ptr<rttb::apps::voxelizer::MaskWriter> MW =
				    boost::make_shared<rttb::apps::voxelizer::MaskWriter>(maskVector,
				            params.booleanVoxelization);
				MW->writeMaskToFile(fileName + "_" + labelOfInterest + fileEnding);
			}

		}
	}
	else
	{
		std::cout << "No struct found" << std::endl;
	}

	return EXIT_SUCCESS;
}

