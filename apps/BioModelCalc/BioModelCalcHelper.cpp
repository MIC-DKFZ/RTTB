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


#include "BioModelCalcHelper.h"

#include "boost/make_shared.hpp"
#include "boost/shared_ptr.hpp"

#include "rttbExceptionMacros.h"

#include "BioModelCalcApplicationData.h"
#include "rttbITKImageAccessorConverter.h"
#include "rttbImageWriter.h"
#include "rttbLQModelAccessor.h"

void
rttb::apps::bioModelCalc::processData(rttb::apps::bioModelCalc::ApplicationData& appData)
{
	rttb::core::DoseAccessorInterface::DoseAccessorPointer outputAccessor;

	std::cout << std::endl << "generate biomodel... ";
    rttb::core::AccessorInterface::AccessorPointer bioModelAccessor;
    if (!appData._modelParameters.empty()){
        bioModelAccessor = generateBioModel(appData._dose, appData._model, appData._modelParameters, appData._nFractions,
            appData._doseScaling);
    }
    else {
        bioModelAccessor = generateBioModelWithMaps(appData._dose, appData._model, appData._modelParameterMaps, appData._nFractions,
            appData._doseScaling);
    }
	std::cout << "done." << std::endl;
	std::cout << std::endl << "generate output image... ";
	io::itk::ITKImageAccessorConverter converter(bioModelAccessor);
	converter.setFailOnInvalidIDs(true);
	converter.process();
	io::itk::ITKImageAccessorConverter::ITKImageType::Pointer itkImage = converter.getITKImage();
	std::cout << "done." << std::endl;
	std::cout << std::endl << "write output image... ";
	io::itk::ImageWriter writer(appData._outputFileName, itkImage.GetPointer());
	writer.writeFile();
	std::cout << "done." << std::endl;
}


rttb::core::AccessorInterface::AccessorPointer rttb::apps::bioModelCalc::generateBioModel(
    rttb::core::DoseAccessorInterface::DoseAccessorPointer dose, const std::string& model,
    const std::vector<double>& modelParameters, unsigned int nFractions, double doseScaling)
{
	if (model == "LQ")
	{
		return boost::make_shared<rttb::models::LQModelAccessor>(dose, modelParameters.at(0),
		        modelParameters.at(1), nFractions,
		        doseScaling);
	}
	else
	{
		rttbDefaultExceptionStaticMacro( << "Unknown model selected. Cannot load data. Selected model: "
		                                 << model);
	}
}

rttb::core::AccessorInterface::AccessorPointer rttb::apps::bioModelCalc::generateBioModelWithMaps(
    rttb::core::DoseAccessorInterface::DoseAccessorPointer dose, const std::string& model,
    const std::deque<rttb::core::AccessorInterface::AccessorPointer>& modelParameterMaps, unsigned int nFractions, double doseScaling)
{
    if (model == "LQ")
    {
        return boost::make_shared<rttb::models::LQModelAccessor>(dose, modelParameterMaps.at(0),
            modelParameterMaps.at(1), nFractions, doseScaling);
    }
    else
    {
        rttbDefaultExceptionStaticMacro(<< "Unknown model selected. Cannot load data. Selected model: "
            << model);
    }
}

