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


#include "BioModelCalcHelper.h"

#include "boost/make_shared.hpp"
#include "boost/shared_ptr.hpp"

#include "rttbExceptionMacros.h"

#include "rttbVirtuosPlanFileDoseAccessorGenerator.h"
#include "rttbDicomFileDoseAccessorGenerator.h"
#include "rttbDicomHelaxFileDoseAccessorGenerator.h"
#include "rttbITKImageFileAccessorGenerator.h"
#include "rttbITKImageAccessorConverter.h"
#include "rttbImageWriter.h"
#include "rttbLQModelAccessor.h"

rttb::core::DoseAccessorInterface::DoseAccessorPointer
rttb::apps::bioModelCalc::loadDose(const std::string& fileName,
                                   const rttb::apps::bioModelCalc::ApplicationData::LoadingStyleArgType& args)
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
rttb::apps::bioModelCalc::loadDicomDose(const std::string& fileName)
{
	rttb::io::dicom::DicomFileDoseAccessorGenerator generator(fileName);
	return generator.generateDoseAccessor();
};

rttb::core::DoseAccessorInterface::DoseAccessorPointer
rttb::apps::bioModelCalc::loadHelaxDose(const std::string& path)
{
	rttb::io::helax::DicomHelaxFileDoseAccessorGenerator generator(path);
	return generator.generateDoseAccessor();
};

rttb::core::DoseAccessorInterface::DoseAccessorPointer
rttb::apps::bioModelCalc::loadITKDose(const std::string& fileName)
{
	rttb::io::itk::ITKImageFileAccessorGenerator generator(fileName);
	return generator.generateDoseAccessor();
};

rttb::core::DoseAccessorInterface::DoseAccessorPointer
rttb::apps::bioModelCalc::loadVirtuosDose(const std::string& fileName, const std::string& planFileName)
{
	rttb::io::virtuos::VirtuosPlanFileDoseAccessorGenerator generator(fileName, planFileName);
	return generator.generateDoseAccessor();
};


void
rttb::apps::bioModelCalc::processData(rttb::apps::bioModelCalc::ApplicationData& appData)
{
	rttb::core::DoseAccessorInterface::DoseAccessorPointer outputAccessor;

	std::cout << std::endl << "generate biomodel... ";
	auto bioModelAccessor = generateBioModel(appData._dose, appData._model, appData._modelParameters);
	std::cout << "done." << std::endl;
	std::cout << std::endl << "generate output image... ";
	io::itk::ITKImageAccessorConverter converter(bioModelAccessor);
	converter.setFailOnInvalidIDs(true);
	converter.process();
	io::itk::ITKImageAccessorConverter::ITKImageType::Pointer itkImage = converter.getITKImage();
	std::cout << "done." << std::endl;
	std::cout << std::endl << "write output image... ";
	io::itk::ImageWriter writer(appData._outputFileName, itkImage);
	writer.writeFile();
	std::cout << "done." << std::endl;
};


rttb::core::AccessorInterface::AccessorPointer rttb::apps::bioModelCalc::generateBioModel(
    rttb::core::DoseAccessorInterface::DoseAccessorPointer dose, const std::string& model,
    const std::vector<double>& modelParameters)
{
	if (model == "LQ")
	{
		return boost::make_shared<rttb::models::LQModelAccessor>(dose, modelParameters.at(0), modelParameters.at(1));
	}
	else
	{
		rttbDefaultExceptionStaticMacro( << "Unknown model selected. Cannot load data. Selected model: "
		                                 << model);
	}
}

