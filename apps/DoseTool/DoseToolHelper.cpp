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
// @version $Revision: 1374 $ (last changed revision)
// @date    $Date: 2016-05-30 14:15:42 +0200 (Mo, 30 Mai 2016) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/


#include "DoseToolHelper.h"

#include "boost/make_shared.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
#include "boost/filesystem.hpp"

#include "DoseToolApplicationData.h"
#include "rttbDicomFileStructureSetGenerator.h"
#include "rttbITKImageFileMaskAccessorGenerator.h"
#include "rttbDoseStatistics.h"
#include "rttbDVH.h"
#include "rttbDVHCalculator.h"
#include "rttbDVHXMLFileWriter.h"
#include "rttbDoseStatisticsCalculator.h"
#include "rttbBoostMaskAccessor.h"
#include "rttbGenericMaskedDoseIterator.h"
#include "rttbDoseStatisticsXMLWriter.h"
#include "rttbVOIindexIdentifier.h"


std::vector<rttb::core::MaskAccessorInterface::MaskAccessorPointer>
rttb::apps::doseTool::generateMasks(
    rttb::apps::doseTool::ApplicationData& appData)
{
	std::vector<core::MaskAccessorInterface::MaskAccessorPointer> maskAccessorPtrVector;

	if (appData._structLoadStyle.front() == "itk")
	{
		maskAccessorPtrVector.push_back(rttb::io::itk::ITKImageFileMaskAccessorGenerator(
		                                    appData._structFileName).generateMaskAccessor());
        appData._structNames.push_back(appData._structNameRegex);
	}
	else
	{
		auto foundIndices = rttb::masks::VOIindexIdentifier::getIndicesByVoiRegex(
		            appData._struct,
		            appData._structNameRegex);
		std::vector<size_t> relevantIndices;

		if (appData._multipleStructsMode)
		{
			relevantIndices = foundIndices;
		}
		else
		{
			if (!foundIndices.empty())
			{
				relevantIndices.push_back(foundIndices.front());
			}
		}

		appData._structIndices = relevantIndices;

		bool strict = !appData._allowSelfIntersection;

		for (size_t i = 0; i < appData._structIndices.size(); i++)
		{
			maskAccessorPtrVector.emplace_back(boost::make_shared<rttb::masks::boost::BoostMaskAccessor>
			    (appData._struct->getStructure(appData._structIndices.at(i)), appData._dose->getGeometricInfo(),
			     strict));
			maskAccessorPtrVector.at(i)->updateMask();
			appData._structNames.push_back(appData._struct->getStructure(appData._structIndices.at(
			                                   i))->getLabel());
		}
	}

	return maskAccessorPtrVector;
}

rttb::core::DoseIteratorInterface::DoseIteratorPointer
rttb::apps::doseTool::generateMaskedDoseIterator(
    rttb::core::MaskAccessorInterface::MaskAccessorPointer maskAccessorPtr,
    rttb::core::DoseAccessorInterface::DoseAccessorPointer doseAccessorPtr)
{
	boost::shared_ptr<core::GenericMaskedDoseIterator> maskedDoseIterator =
	    boost::make_shared<core::GenericMaskedDoseIterator>(maskAccessorPtr, doseAccessorPtr);
	rttb::core::DoseIteratorInterface::DoseIteratorPointer doseIterator(maskedDoseIterator);
	return doseIterator;
}

rttb::algorithms::DoseStatistics::DoseStatisticsPointer
calculateDoseStatistics(
    rttb::core::DoseIteratorInterface::DoseIteratorPointer doseIterator, bool calculateComplexDoseStatistics,
    rttb::DoseTypeGy prescribedDose)
{
	rttb::algorithms::DoseStatisticsCalculator doseStatsCalculator(doseIterator);

	if (calculateComplexDoseStatistics)
	{
		return doseStatsCalculator.calculateDoseStatistics(prescribedDose);
	}
	else
	{
		return doseStatsCalculator.calculateDoseStatistics();
	}
}


rttb::core::DVH::DVHPointer calculateDVH(
    rttb::core::DoseIteratorInterface::DoseIteratorPointer
    doseIterator, rttb::IDType structUID, rttb::IDType doseUID)
{
	rttb::core::DVHCalculator calc(doseIterator, structUID, doseUID);
	rttb::core::DVH::DVHPointer dvh = calc.generateDVH();
	return dvh;
}

std::string rttb::apps::doseTool::assembleFilenameWithStruct(const std::string& originalFilename,
    const std::string& structName)
{
    boost::filesystem::path originalFile(originalFilename);
    std::string newFilename = originalFile.stem().string() + "_" + structName +
        originalFile.extension().string();
    boost::filesystem::path newFile(originalFile.parent_path() / newFilename);
    return newFile.string();
}

/*! @brief Writes the dose statistics as XML to a file
@details adds a <config>....</config> part to the RTTB generated xml where the used files and struct names are stored.
*/
void writeDoseStatisticsFile(
    rttb::algorithms::DoseStatistics::DoseStatisticsPointer statistics,
    const std::string& filename, const std::string& structName,
    rttb::apps::doseTool::ApplicationData& appData)
{
	boost::property_tree::ptree originalTree = rttb::io::other::writeDoseStatistics(statistics);

	//add config part to xml
	originalTree.add("statistics.config.requestedStructRegex", appData._structNameRegex);
	originalTree.add("statistics.config.structName", structName);
	originalTree.add("statistics.config.doseUID", appData._dose->getUID());
	originalTree.add("statistics.config.doseFile", appData._doseFileName);
	originalTree.add("statistics.config.structFile", appData._structFileName);

	boost::property_tree::ptree reorderedTree, configTree, resultsTree;
	configTree = originalTree.get_child("statistics.config");
	resultsTree = originalTree.get_child("statistics.results");
	reorderedTree.add_child("statistics.config", configTree);
	reorderedTree.add_child("statistics.results", resultsTree);

	boost::property_tree::write_xml(filename, reorderedTree, std::locale(),
	                                boost::property_tree::xml_writer_make_settings<std::string>('\t', 1));

}

void writeDVHFile(rttb::core::DVH::DVHPointer dvh, const std::string& filename)
{
	rttb::DVHType typeCum = { rttb::DVHType::Cumulative };
	rttb::io::other::DVHXMLFileWriter dvhWriter(filename, typeCum);
	dvhWriter.writeDVH(dvh);
}

void
rttb::apps::doseTool::processData(rttb::apps::doseTool::ApplicationData& appData)
{
    std::cout << std::endl << "generating masks... ";
    std::vector<core::MaskAccessorInterface::MaskAccessorPointer> maskAccessorPtrVector = generateMasks(
        appData);
    std::cout << "done." << std::endl;

    for (size_t i = 0; i < maskAccessorPtrVector.size(); i++)
    {
        core::DoseIteratorInterface::DoseIteratorPointer spDoseIterator(generateMaskedDoseIterator(
            maskAccessorPtrVector.at(i),
            appData._dose));

        if (appData._computeDoseStatistics)
        {
            std::cout << std::endl << "computing dose statistics... ";
            algorithms::DoseStatistics::DoseStatisticsPointer statistics = calculateDoseStatistics(
                spDoseIterator,
                appData._computeComplexDoseStatistics, appData._prescribedDose);
            std::cout << "done." << std::endl;

            std::cout << std::endl << "writing dose statistics to file... ";
            std::string outputFilename;

            if (appData._multipleStructsMode)
            {
                outputFilename = assembleFilenameWithStruct(appData._doseStatisticOutputFileName,
                    appData._structNames.at(i));
            }
            else
            {
                outputFilename = appData._doseStatisticOutputFileName;
            }

            writeDoseStatisticsFile(statistics, outputFilename, appData._structNames.at(i), appData);
            std::cout << "done." << std::endl;
        }

        if (appData._computeDVH)
        {
            std::cout << std::endl << "computing DVH... ";
            rttb::IDType structUID;
            rttb::IDType doseUID;
            //Generate random UID
            if (appData._structLoadStyle.front() == "itk"){
                structUID = "struct42";
                doseUID = "dose42";
            }
            else {
                structUID = appData._struct->getUID();
                doseUID = appData._dose->getUID();
            }

            core::DVH::DVHPointer dvh = calculateDVH(spDoseIterator, structUID,
                doseUID);
            std::cout << "done." << std::endl;

            std::cout << std::endl << "writing DVH to file... ";
            std::string outputFilename;

            if (appData._multipleStructsMode)
            {
                outputFilename = assembleFilenameWithStruct(appData._dvhOutputFilename, appData._structNames.at(i));
            }
            else
            {
                outputFilename = appData._dvhOutputFilename;
            }


            writeDVHFile(dvh, outputFilename);
            std::cout << "done." << std::endl;
        }
    }

}
