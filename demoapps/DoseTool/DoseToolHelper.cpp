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
// @version $Revision: 1095 $ (last changed revision)
// @date    $Date: 2015-09-11 11:12:40 +0200 (Fr, 11 Sep 2015) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/


#include "DoseToolHelper.h"

#include "boost/make_shared.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/property_tree/ptree.hpp"

#include "rttbExceptionMacros.h"

#include "rttbVirtuosPlanFileDoseAccessorGenerator.h"
#include "rttbDicomFileDoseAccessorGenerator.h"
#include "rttbDicomHelaxFileDoseAccessorGenerator.h"
#include "rttbITKImageFileAccessorGenerator.h"
#include "rttbStructureSetGeneratorInterface.h"
#include "rttbDicomFileStructureSetGenerator.h"
#include "rttbVirtuosFileStructureSetGenerator.h"
#include "rttbITKImageFileMaskAccessorGenerator.h"
#include "rttbDVHCalculator.h"
#include "rttbDVHXMLFileWriter.h"
#include "rttbDoseStatisticsCalculator.h"
#include "rttbBoostMaskAccessor.h"
#include "rttbGenericMaskedDoseIterator.h"
#include "rttbDoseStatisticsXMLWriter.h"
#include "rttbVOIindexIdentifier.h"

rttb::core::DoseAccessorInterface::DoseAccessorPointer
rttb::apps::doseTool::loadDose(const std::string& fileName,
                               const rttb::apps::doseTool::ApplicationData::LoadingStyleArgType& args)
{
	rttb::core::DoseAccessorInterface::DoseAccessorPointer result;

	std::cout << std::endl << "read dose file... ";

	if (args.empty() || args[0] == "dicom")
	{
		std::cout << "use RTTB dicom IO... ";
		result = loadDicomDose(fileName);
	}
	else if (args[0] == "helax")
	{
		std::cout << "use RTTB Helax IO... ";
		result = loadHelaxDose(fileName);
	}
	else if (args[0] == "itk")
	{
		std::cout << "use RTTB itk IO... ";
		result = loadITKDose(fileName);
	}
	else if (args[0] == "virtuos")
	{
		if (args.size() < 2)
		{
			rttbDefaultExceptionStaticMacro( <<
			                                 "Cannot load virtuos dose. Plan file is missing. Specify plan file as 2nd io stlye argument.");
		}

		std::cout << "use RTTB virtuos IO... " << std::endl;
		std::cout << "                  virtuos plan file: " << args[1] << " ... ";
		result = loadVirtuosDose(fileName, args[1]);
	}
	else
	{
		rttbDefaultExceptionStaticMacro( << "Unknown io style selected. Cannot load data. Selected style: "
		                                 << args[0]);
	}

	std::cout << "done." << std::endl;

	return result;
};

rttb::core::DoseAccessorInterface::DoseAccessorPointer
rttb::apps::doseTool::loadDicomDose(const std::string& fileName)
{
	rttb::io::dicom::DicomFileDoseAccessorGenerator generator(fileName);
	return generator.generateDoseAccessor();
};

rttb::core::DoseAccessorInterface::DoseAccessorPointer
rttb::apps::doseTool::loadHelaxDose(const std::string& path)
{
	rttb::io::helax::DicomHelaxFileDoseAccessorGenerator generator(path);
	return generator.generateDoseAccessor();
};

rttb::core::DoseAccessorInterface::DoseAccessorPointer
rttb::apps::doseTool::loadITKDose(const std::string& fileName)
{
	rttb::io::itk::ITKImageFileAccessorGenerator generator(fileName);
	return generator.generateDoseAccessor();
};

rttb::core::DoseAccessorInterface::DoseAccessorPointer
rttb::apps::doseTool::loadVirtuosDose(const std::string& fileName, const std::string& planFileName)
{
	rttb::io::virtuos::VirtuosPlanFileDoseAccessorGenerator generator(fileName, planFileName);
	return generator.generateDoseAccessor();
};

rttb::core::StructureSetGeneratorInterface::StructureSetPointer rttb::apps::doseTool::loadStruct(
    const std::string& fileName, const ApplicationData::LoadingStyleArgType& args)
{
	rttb::core::StructureSetGeneratorInterface::StructureSetPointer result;

	std::cout << std::endl << "read struct file... ";

	if (args.empty() || args[0] == "dicom")
	{
		std::cout << "use RTTB dicom IO... ";
		result = loadDicomStruct(fileName);
	}
	else if (args[0] == "virtuos")
	{
		std::cout << "use RTTB virtuos IO... " << std::endl;
		std::cout << "                  virtuos CTX file: " << args[1] << " ... ";
		result = loadVirtuosStruct(fileName, args[1]);
	}
	else
	{
		rttbDefaultExceptionStaticMacro( << "Unknown io style selected. Cannot load data. Selected style: "
		                                 << args[0]);
	}

	std::cout << "done." << std::endl;

	return result;
}

rttb::core::StructureSetGeneratorInterface::StructureSetPointer rttb::apps::doseTool::loadDicomStruct(
    const std::string& fileName)
{
	rttb::io::dicom::DicomFileStructureSetGenerator generator(fileName);
	return generator.generateStructureSet();
}

rttb::core::StructureSetGeneratorInterface::StructureSetPointer rttb::apps::doseTool::loadVirtuosStruct(
    const std::string& fileName, const std::string& ctxFileName)
{
	rttb::io::virtuos::VirtuosFileStructureSetGenerator generator(fileName, ctxFileName.c_str());
	return generator.generateStructureSet();
}


void
rttb::apps::doseTool::processData(rttb::apps::doseTool::ApplicationData& appData)
{
	std::cout << std::endl << "generating mask... ";
	core::MaskAccessorInterface::MaskAccessorPointer maskAccessorPtr = generateMask(appData);

	core::DoseIteratorInterface::DoseIteratorPointer spDoseIterator(generateMaskedDoseIterator(maskAccessorPtr,
	        appData._dose));
	std::cout << "done." << std::endl;

	if (appData._computeDoseStatistics)
	{
		std::cout << std::endl << "computing dose statistics... ";
		algorithms::DoseStatistics::DoseStatisticsPointer statistics = calculateDoseStatistics(spDoseIterator,
		        appData._computeComplexDoseStatistics, appData._prescribedDose);
		std::cout << "done." << std::endl;

		std::cout << std::endl << "writing dose statistics to file... ";
		writeDoseStatisticsFile(statistics, appData);
		std::cout << "done." << std::endl;
	}

	if (appData._computeDVH)
	{
		std::cout << std::endl << "computing DVH... ";
		core::DVH::DVHPointer dvh = calculateDVH(spDoseIterator, appData._struct->getUID(), appData._dose->getUID());
		std::cout << "done." << std::endl;

		std::cout << std::endl << "writing DVH to file... ";
		writeDVHFile(dvh, appData._dvhOutputFilename);
		std::cout << "done." << std::endl;

	}
};

void rttb::apps::doseTool::determineStructIndex(rttb::apps::doseTool::ApplicationData& appData)
{
	unsigned int index = rttb::masks::VOIindexIdentifier::getIndexByVoiName(appData._struct, appData._structNameRegex);
	appData._structIndex = index;
	appData._structNameActual = appData._struct->getStructure(index)->getLabel();
}


rttb::core::MaskAccessorInterface::MaskAccessorPointer rttb::apps::doseTool::generateMask(
    rttb::apps::doseTool::ApplicationData& appData)
{
	core::MaskAccessorInterface::MaskAccessorPointer maskAccessorPtr;

	if (appData._structLoadStyle.front() == "itk")
	{
		maskAccessorPtr = rttb::io::itk::ITKImageFileMaskAccessorGenerator(appData._structFileName).generateMaskAccessor();
	}
	else
	{
		determineStructIndex(appData);
		bool strict = !appData._allowSelfIntersection;
		maskAccessorPtr =
		    boost::make_shared<rttb::masks::boost::BoostMaskAccessor>
		    (appData._struct->getStructure(appData._structIndex), appData._dose->getGeometricInfo(), strict);
	}

	maskAccessorPtr->updateMask();
	return maskAccessorPtr;
}

rttb::core::DoseIteratorInterface::DoseIteratorPointer rttb::apps::doseTool::generateMaskedDoseIterator(
    rttb::core::MaskAccessorInterface::MaskAccessorPointer maskAccessorPtr,
    rttb::core::DoseAccessorInterface::DoseAccessorPointer doseAccessorPtr)
{
	boost::shared_ptr<core::GenericMaskedDoseIterator> maskedDoseIterator =
	    boost::make_shared<core::GenericMaskedDoseIterator>(maskAccessorPtr, doseAccessorPtr);
	rttb::core::DoseIteratorInterface::DoseIteratorPointer doseIterator(maskedDoseIterator);
	return doseIterator;
}

rttb::algorithms::DoseStatistics::DoseStatisticsPointer rttb::apps::doseTool::calculateDoseStatistics(
    core::DoseIteratorInterface::DoseIteratorPointer doseIterator, bool calculateComplexDoseStatistics,
    DoseTypeGy prescribedDose)
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


rttb::core::DVH::DVHPointer rttb::apps::doseTool::calculateDVH(core::DoseIteratorInterface::DoseIteratorPointer
        doseIterator, IDType structUID, IDType doseUID)
{
	rttb::core::DVHCalculator calc(doseIterator, structUID, doseUID);
	rttb::core::DVH::DVHPointer dvh = calc.generateDVH();
	return dvh;
}


void rttb::apps::doseTool::writeDoseStatisticsFile(rttb::algorithms::DoseStatistics::DoseStatisticsPointer statistics,
        rttb::apps::doseTool::ApplicationData& appData)
{
	boost::property_tree::ptree originalTree = rttb::io::other::writeDoseStatistics(statistics);

	//add config part to xml
	originalTree.add("statistics.config.requestedStructRegex", appData._structNameRegex);
	originalTree.add("statistics.config.structName", appData._structNameActual);
	originalTree.add("statistics.config.doseUID", appData._dose->getUID());
	originalTree.add("statistics.config.doseFile", appData._doseFileName);
	originalTree.add("statistics.config.structFile", appData._structFileName);

	boost::property_tree::ptree reorderedTree, configTree, resultsTree;
	configTree = originalTree.get_child("statistics.config");
	resultsTree = originalTree.get_child("statistics.results");
	reorderedTree.add_child("statistics.config", configTree);
	reorderedTree.add_child("statistics.results", resultsTree);

	boost::property_tree::write_xml(appData._doseStatisticOutputFileName, reorderedTree, std::locale(),
	                                boost::property_tree::xml_writer_make_settings<std::string>('\t', 1));

}

void rttb::apps::doseTool::writeDVHFile(core::DVH::DVHPointer dvh, const std::string& filename)
{
	DVHType typeCum = { DVHType::Cumulative };
	rttb::io::other::DVHXMLFileWriter dvhWriter(filename, typeCum);
	dvhWriter.writeDVH(dvh);
}
