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
// @version $Revision: 1334 $ (last changed revision)
// @date    $Date: 2016-04-22 11:13:22 +0200 (Fr, 22 Apr 2016) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/


#include "DoseTool4VHelper.h"

#include "rttbExceptionMacros.h"

#include "rttbDicomFileDoseAccessorGenerator.h"
#include "rttbDicomHelaxFileDoseAccessorGenerator.h"
#include "rttbITKImageFileAccessorGenerator.h"
#include "rttbVirtuosPlanFileDoseAccessorGenerator.h"
#include "rttbDicomFileStructureSetGenerator.h"
#include "rttbVirtuosFileStructureSetGenerator.h"

rttb::core::DoseAccessorInterface::DoseAccessorPointer
rttb::apps::doseTool4V::loadDose(const std::string& fileName,
const LoadingStyleArgType& args)
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
}

rttb::core::DoseAccessorInterface::DoseAccessorPointer
rttb::apps::doseTool4V::loadDicomDose(const std::string& fileName)
{
	rttb::io::dicom::DicomFileDoseAccessorGenerator generator(fileName);
	return generator.generateDoseAccessor();
}

rttb::core::DoseAccessorInterface::DoseAccessorPointer
rttb::apps::doseTool4V::loadHelaxDose(const std::string& path)
{
	rttb::io::helax::DicomHelaxFileDoseAccessorGenerator generator(path);
	return generator.generateDoseAccessor();
}

rttb::core::DoseAccessorInterface::DoseAccessorPointer
rttb::apps::doseTool4V::loadITKDose(const std::string& fileName)
{
	rttb::io::itk::ITKImageFileAccessorGenerator generator(fileName);
	return generator.generateDoseAccessor();
}

rttb::core::DoseAccessorInterface::DoseAccessorPointer
rttb::apps::doseTool4V::loadVirtuosDose(const std::string& fileName, const std::string& planFileName)
{
	rttb::io::virtuos::VirtuosPlanFileDoseAccessorGenerator generator(fileName, planFileName);
	return generator.generateDoseAccessor();
}

rttb::core::StructureSetGeneratorInterface::StructureSetPointer rttb::apps::doseTool4V::loadStruct(
    const std::string& fileName, const LoadingStyleArgType& args,
    const std::string& structNameRegex)
{
	rttb::core::StructureSetGeneratorInterface::StructureSetPointer result;

	std::cout << std::endl << "read struct file... ";

	if (args.empty() || args[0] == "dicom")
	{
		std::cout << "use RTTB dicom IO... ";
		result = loadDicomStruct(fileName, structNameRegex);
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

rttb::core::StructureSetGeneratorInterface::StructureSetPointer
rttb::apps::doseTool4V::loadDicomStruct(
const std::string& fileName, const std::string& structNameRegex)
{
	rttb::io::dicom::DicomFileStructureSetGenerator generator(fileName);

	if (!structNameRegex.empty())
	{
		generator.setStructureLableFilterActive(true);
		generator.setFilterRegEx(structNameRegex);
	}

	return generator.generateStructureSet();
}

rttb::core::StructureSetGeneratorInterface::StructureSetPointer
rttb::apps::doseTool4V::loadVirtuosStruct(
    const std::string& fileName, const std::string& ctxFileName)
{
	rttb::io::virtuos::VirtuosFileStructureSetGenerator generator(fileName, ctxFileName.c_str());
	return generator.generateStructureSet();
}


