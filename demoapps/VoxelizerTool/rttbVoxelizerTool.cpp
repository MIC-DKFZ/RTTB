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

	boost::shared_ptr<rttb::apps::voxelizer::CommandOptions> CO =
	    boost::make_shared<rttb::apps::voxelizer::CommandOptions>();

	if (!CO->command(argc, argv))
	{
		return EXIT_FAILURE;
	}

	rttb::apps::voxelizer::Parameters params = CO->getParameters();

	if (!params.structFile.empty() && !params.referenceFile.empty())
	{
		boost::shared_ptr<rttb::apps::voxelizer::StructDataReader> SDR =
		    boost::make_shared<rttb::apps::voxelizer::StructDataReader>(params.structFile,
		            params.referenceFile);

		std::vector<int> ListOfCorrectElements;

		for (int i = 0; i < params.regEx.size(); i++)
		{
			std::vector<int> IndexOfCorrectElements;
			IndexOfCorrectElements = rttb::apps::voxelizer::filterForExpression(SDR->getAllLabels(),
			                         params.regEx.at(i));

			for (int k = 0 ; k < IndexOfCorrectElements.size() ; k++)
			{
				ListOfCorrectElements.push_back(IndexOfCorrectElements.at(k));
			}
		}

		boost::shared_ptr<rttb::apps::voxelizer::MaskProcess> MP =
		    boost::make_shared<rttb::apps::voxelizer::MaskProcess>(SDR->getStructureSetPointer(),
		            SDR->getDoseAccessorPointer(),
		            params.legacyVoxelization);

		if (!ListOfCorrectElements.empty())
		{
			std::string FileName = rttb::apps::voxelizer::getFilenameWithoutEnding(
			                           params.outputFilename);
			std::string FileEnding = rttb::apps::voxelizer::getFileEnding(params.outputFilename);

			std::vector<MaskAccessorPointer> MaskPointer;

			if (params.addStructures)
			{
				std::string LabelName;

				for (int i = 0; i < ListOfCorrectElements.size(); i++)
				{
					MaskPointer.push_back(MP->createMask(ListOfCorrectElements.at(i)));

					LabelName += "_" + rttb::apps::voxelizer::getLabelFromList(SDR->getAllLabels(),
					             ListOfCorrectElements.at(i));
				}

				boost::shared_ptr<rttb::apps::voxelizer::MaskWriter> MW =
				    boost::make_shared<rttb::apps::voxelizer::MaskWriter>(MaskPointer,
				            params.booleanVoxelization);
				MW->writeMaskToFile(FileName  + LabelName + FileEnding);
			}
			else
			{

				std::string LabelName;

				if (params.multipleStructs)
				{
					for (unsigned int i = 0; i < ListOfCorrectElements.size(); i++)
					{
						MaskPointer.push_back(MP->createMask(ListOfCorrectElements.at(i)));

						LabelName = rttb::apps::voxelizer::getLabelFromList(SDR->getAllLabels(),
						            ListOfCorrectElements.at(i));
						boost::shared_ptr<rttb::apps::voxelizer::MaskWriter> MW =
						    boost::make_shared<rttb::apps::voxelizer::MaskWriter>(MaskPointer,
						            params.booleanVoxelization);
						MW->writeMaskToFile(FileName + "_" + LabelName + FileEnding);
					}
				}
				else
				{
					MaskPointer.push_back(MP->createMask(ListOfCorrectElements.at(0)));

					LabelName = rttb::apps::voxelizer::getLabelFromList(SDR->getAllLabels(),
					            ListOfCorrectElements.at(0));
					boost::shared_ptr<rttb::apps::voxelizer::MaskWriter> MW =
					    boost::make_shared<rttb::apps::voxelizer::MaskWriter>(MaskPointer,
					            params.booleanVoxelization);
					MW->writeMaskToFile(FileName + "_" + LabelName + FileEnding);

				}

			}
		}
		else
		{
			std::cout << "No struct found" << std::endl;
		}
	}

	return EXIT_SUCCESS;
}

