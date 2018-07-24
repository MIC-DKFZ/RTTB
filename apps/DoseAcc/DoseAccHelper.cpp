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

#include "DoseAccHelper.h"

#include "boost/make_shared.hpp"

#include "mapRegistrationFileReader.h"

#include "rttbExceptionMacros.h"

#include "rttbITKImageAccessorConverter.h"
#include "rttbSimpleMappableDoseAccessor.h"
#include "rttbMatchPointTransformation.h"
#include "rttbLinearInterpolation.h"
#include "rttbNearestNeighborInterpolation.h"
#include "rttbRosuMappableDoseAccessor.h"
#include "rttbArithmetic.h"
#include "rttbBinaryFunctorAccessor.h"
#include "rttbImageWriter.h"

rttb::apps::doseAcc::ApplicationData::RegistrationType::Pointer
rttb::apps::doseAcc::loadRegistration(const std::string& fileName)
{
	map::io::RegistrationFileReader::Pointer spRegReader = map::io::RegistrationFileReader::New();

	map::io::RegistrationFileReader::LoadedRegistrationPointer spReg;

	std::cout << std::endl << "read registration file... ";
	spReg = spRegReader->read(fileName);
	std::cout << "done." << std::endl;

	ApplicationData::RegistrationType::Pointer resultPtr =
	    dynamic_cast<ApplicationData::RegistrationType*>(spReg.GetPointer());

	if (resultPtr.IsNull())
	{
		rttbDefaultExceptionStaticMacro( <<
		                                 "Loaded registration cannot be used. Only 3D 3D registrations are allowed.");
	}

	return resultPtr;
};

rttb::core::DoseAccessorInterface::Pointer generateNNMappableAccessor(
    const rttb::core::GeometricInfo& geoInfoTargetImage,
    const rttb::core::DoseAccessorInterface::Pointer doseMovingImage,
    const rttb::interpolation::TransformationInterface::Pointer aTransformation)
{
  auto interpolate = boost::make_shared<rttb::interpolation::NearestNeighborInterpolation>();

  return boost::make_shared<rttb::interpolation::SimpleMappableDoseAccessor>(geoInfoTargetImage, doseMovingImage,
    aTransformation, interpolate);
}

rttb::core::DoseAccessorInterface::Pointer generateLinearMappableAccessor(
    const rttb::core::GeometricInfo& geoInfoTargetImage,
    const rttb::core::DoseAccessorInterface::Pointer doseMovingImage,
    const rttb::interpolation::TransformationInterface::Pointer aTransformation)
{
  auto interpolate = boost::make_shared<rttb::interpolation::LinearInterpolation>();

  return boost::make_shared<rttb::interpolation::SimpleMappableDoseAccessor>(geoInfoTargetImage, doseMovingImage,
    aTransformation, interpolate);
}

rttb::core::DoseAccessorInterface::Pointer generateRosuMappableAccessor(
    const rttb::core::GeometricInfo& geoInfoTargetImage,
    const rttb::core::DoseAccessorInterface::Pointer doseMovingImage,
    const rttb::interpolation::TransformationInterface::Pointer aTransformation)
{
  return boost::make_shared<rttb::interpolation::RosuMappableDoseAccessor>(geoInfoTargetImage, doseMovingImage,
    aTransformation);
}

/**Private helper function for processData(). Generates a suitable output accessor
 * (depending on the configuration in appData a suitable accessor pipeline is established)
 * which performs the accumulation of the doses and returns the output.to */
rttb::core::DoseAccessorInterface::Pointer
assembleOutputAccessor(rttb::apps::doseAcc::ApplicationData& appData)
{

	rttb::core::DoseAccessorInterface::Pointer dose2Accessor = appData._dose2;

	if (appData._spReg.IsNotNull())
	{
    auto transform = boost::make_shared<rttb::interpolation::MatchPointTransformation>(appData._spReg);

		if (appData._interpolatorName == "rosu")
		{
			dose2Accessor = generateRosuMappableAccessor(appData._dose1->getGeometricInfo(), appData._dose2,
			                transform);
		}
		else if (appData._interpolatorName == "nn")
		{
			dose2Accessor = generateNNMappableAccessor(appData._dose1->getGeometricInfo(), appData._dose2,
			                transform);
		}
		else if (appData._interpolatorName == "linear")
		{
			dose2Accessor = generateLinearMappableAccessor(appData._dose1->getGeometricInfo(), appData._dose2,
			                transform);
		}
		else
		{
			rttbDefaultExceptionStaticMacro( <<
			                                 "Unkown interpolation type selected. Cannot map dose. Interpolation type: " <<
			                                 appData._interpolatorName);
		}
	}

	rttb::core::DoseAccessorInterface::Pointer outputAccessor;

	if (appData._operator == "+")
	{
		rttb::algorithms::arithmetic::doseOp::AddWeighted addOp(appData._weightDose1, appData._weightDose2);
        outputAccessor = boost::make_shared<rttb::algorithms::BinaryFunctorAccessor<rttb::algorithms::arithmetic::doseOp::AddWeighted> >(appData._dose1, dose2Accessor, addOp);
	}
	else if (appData._operator == "*")
	{
		outputAccessor =
            boost::make_shared<rttb::algorithms::BinaryFunctorAccessor<rttb::algorithms::arithmetic::doseOp::Multiply> >
            (appData._dose1, dose2Accessor, rttb::algorithms::arithmetic::doseOp::Multiply());
	}
	else
	{
		rttbDefaultExceptionStaticMacro( <<
		                                 "Unkown operator selected. Cannot map dose. Operator: " <<
		                                 appData._interpolatorName);
	}

	return outputAccessor;
}

void
rttb::apps::doseAcc::processData(rttb::apps::doseAcc::ApplicationData& appData)
{
	rttb::core::DoseAccessorInterface::Pointer outputAccessor = assembleOutputAccessor(
	            appData);

	std::cout << std::endl << "generate output image... ";
	io::itk::ITKImageAccessorConverter converter(outputAccessor);
	converter.setFailOnInvalidIDs(true);
	converter.process();
	io::itk::ITKImageAccessorConverter::ITKImageType::Pointer itkImage = converter.getITKImage();
	std::cout << "done." << std::endl;

	std::cout << std::endl << "write output image... ";
	io::itk::ImageWriter writer(appData._outputFileName, itkImage.GetPointer());
	writer.writeFile();

	std::cout << "done." << std::endl;
};
