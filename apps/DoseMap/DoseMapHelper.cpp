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

#include "DoseMapHelper.h"

#include <boost/make_shared.hpp>

#include "mapRegistrationFileReader.h"

#include "rttbITKImageAccessorConverter.h"
#include "rttbSimpleMappableDoseAccessor.h"
#include "rttbMatchPointTransformation.h"
#include "rttbLinearInterpolation.h"
#include "rttbNearestNeighborInterpolation.h"
#include "rttbRosuMappableDoseAccessor.h"
#include "rttbArithmetic.h"
#include "rttbBinaryFunctorAccessor.h"
#include "rttbExceptionMacros.h"
#include "rttbImageWriter.h"

rttb::apps::doseMap::ApplicationData::RegistrationType::Pointer
rttb::apps::doseMap::loadRegistration(const std::string& fileName)
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
		mapDefaultExceptionStaticMacro( <<
		                                "Loaded registration cannot be used. Only 3D 3D registrations are allowed.");
	}

	return resultPtr;
};


/**Private helper function for processData(). Generates a suitable output accessor
 * (depending on the configuration in appData a suitable accessor pipeline is established)
 * which performs the accumulation of the doses and returns the output.to */
rttb::core::DoseAccessorInterface::Pointer
assembleOutputAccessor(rttb::apps::doseMap::ApplicationData& appData)
{

	rttb::core::DoseAccessorInterface::Pointer outputAccessor = appData._inputDose;

  auto transform = boost::make_shared<rttb::interpolation::MatchPointTransformation>(appData._spReg);

	if (appData._interpolatorName == "rosu")
	{
		outputAccessor = boost::make_shared<rttb::interpolation::RosuMappableDoseAccessor>(appData._refDose->getGeometricInfo(),
		                             appData._inputDose, transform);
	}
	else
	{
    rttb::interpolation::InterpolationBase::Pointer interpolate = boost::make_shared<rttb::interpolation::LinearInterpolation>();

		if (appData._interpolatorName == "nn")
		{
			interpolate = boost::make_shared<rttb::interpolation::NearestNeighborInterpolation>();
		}
		else if (appData._interpolatorName != "linear")
		{
			mapDefaultExceptionStaticMacro( <<
			                                "Unkown interpolation type selected. Cannot map dose. Interpolation type: " <<
			                                appData._interpolatorName);
		}

    outputAccessor = boost::make_shared<rttb::interpolation::SimpleMappableDoseAccessor>(appData._refDose->getGeometricInfo(),
      appData._inputDose, transform, interpolate);
	}

	return outputAccessor;
}

void
rttb::apps::doseMap::processData(rttb::apps::doseMap::ApplicationData& appData)
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
