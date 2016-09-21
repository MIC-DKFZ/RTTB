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
// @version $Revision: 1132 $ (last changed revision)
// @date    $Date: 2015-10-06 14:48:56 +0200 (Di, 06 Okt 2015) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/


#include "DoseAccHelper.h"

#include "mapRegistrationFileReader.h"
#include "itkImageFileWriter.h"

#include "rttbExceptionMacros.h"

#include "rttbDicomFileDoseAccessorGenerator.h"
#include "rttbDicomHelaxFileDoseAccessorGenerator.h"

#include "rttbITKImageAccessorConverter.h"
#include "rttbSimpleMappableDoseAccessor.h"
#include "rttbMatchPointTransformation.h"
#include "rttbLinearInterpolation.h"
#include "rttbNearestNeighborInterpolation.h"
#include "rttbRosuMappableDoseAccessor.h"
#include "rttbITKImageFileAccessorGenerator.h"
#include "rttbArithmetic.h"
#include "rttbBinaryFunctorAccessor.h"

rttb::core::DoseAccessorInterface::DoseAccessorPointer
rttb::apps::doseAcc::loadDose(const std::string& fileName,
                              const rttb::apps::doseAcc::ApplicationData::LoadingStyleArgType& args)
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
	else
	{
		rttbDefaultExceptionStaticMacro( << "Unknown io style selected. Cannot load data. Selected style: "
		                                 << args[0]);
	}

	std::cout << "done." << std::endl;

	return result;
};

rttb::core::DoseAccessorInterface::DoseAccessorPointer
rttb::apps::doseAcc::loadDicomDose(const std::string& fileName)
{
	rttb::io::dicom::DicomFileDoseAccessorGenerator generator(fileName);
	return generator.generateDoseAccessor();
};

rttb::core::DoseAccessorInterface::DoseAccessorPointer
rttb::apps::doseAcc::loadHelaxDose(const std::string& path)
{
	rttb::io::helax::DicomHelaxFileDoseAccessorGenerator generator(path);
	return generator.generateDoseAccessor();
};

rttb::core::DoseAccessorInterface::DoseAccessorPointer
rttb::apps::doseAcc::loadITKDose(const std::string& fileName)
{
	rttb::io::itk::ITKImageFileAccessorGenerator generator(fileName);
	return generator.generateDoseAccessor();
};

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

rttb::core::DoseAccessorInterface::DoseAccessorPointer generateNNMappableAccessor(
    const rttb::core::GeometricInfo& geoInfoTargetImage,
    const rttb::core::DoseAccessorInterface::DoseAccessorPointer doseMovingImage,
    const rttb::interpolation::TransformationInterface::Pointer aTransformation)
{
	rttb::interpolation::InterpolationBase::Pointer interpolate =
	    rttb::interpolation::NearestNeighborInterpolation::Pointer(new
	            rttb::interpolation::NearestNeighborInterpolation());

	return rttb::core::DoseAccessorInterface::DoseAccessorPointer(
	           new rttb::interpolation::SimpleMappableDoseAccessor(geoInfoTargetImage, doseMovingImage,
	                   aTransformation, interpolate));
}

rttb::core::DoseAccessorInterface::DoseAccessorPointer generateLinearMappableAccessor(
    const rttb::core::GeometricInfo& geoInfoTargetImage,
    const rttb::core::DoseAccessorInterface::DoseAccessorPointer doseMovingImage,
    const rttb::interpolation::TransformationInterface::Pointer aTransformation)
{
	rttb::interpolation::InterpolationBase::Pointer interpolate =
	    rttb::interpolation::LinearInterpolation::Pointer(new
	            rttb::interpolation::LinearInterpolation());

	return rttb::core::DoseAccessorInterface::DoseAccessorPointer(
	           new rttb::interpolation::SimpleMappableDoseAccessor(geoInfoTargetImage, doseMovingImage,
	                   aTransformation, interpolate));
}

rttb::core::DoseAccessorInterface::DoseAccessorPointer generateRosuMappableAccessor(
    const rttb::core::GeometricInfo& geoInfoTargetImage,
    const rttb::core::DoseAccessorInterface::DoseAccessorPointer doseMovingImage,
    const rttb::interpolation::TransformationInterface::Pointer aTransformation)
{
	return rttb::core::DoseAccessorInterface::DoseAccessorPointer(
	           new rttb::interpolation::RosuMappableDoseAccessor(geoInfoTargetImage, doseMovingImage,
	                   aTransformation));
}

/**Private helper function for processData(). Generates a suitable output accessor
 * (depending on the configuration in appData a suitable accessor pipeline is established)
 * which performs the accumulation of the doses and returns the output.to */
rttb::core::DoseAccessorInterface::DoseAccessorPointer
assembleOutputAccessor(rttb::apps::doseAcc::ApplicationData& appData)
{

	rttb::core::DoseAccessorInterface::DoseAccessorPointer dose2Accessor = appData._dose2;

	if (appData._spReg.IsNotNull())
	{
		rttb::interpolation::TransformationInterface::Pointer transform =
		    rttb::interpolation::TransformationInterface::Pointer(new
		            rttb::interpolation::MatchPointTransformation(appData._spReg));

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

	rttb::core::DoseAccessorInterface::DoseAccessorPointer outputAccessor;

	if (appData._operator == "+")
	{
		rttb::algorithms::arithmetic::doseOp::AddWeighted addOp(appData._weightDose1, appData._weightDose2);
		outputAccessor =
		    rttb::core::DoseAccessorInterface::DoseAccessorPointer(new
		            rttb::algorithms::BinaryFunctorAccessor<rttb::algorithms::arithmetic::doseOp::AddWeighted>
		            (appData._dose1, dose2Accessor, addOp));
	}
	else if (appData._operator == "*")
	{
		outputAccessor =
		    rttb::core::DoseAccessorInterface::DoseAccessorPointer(new
		            rttb::algorithms::BinaryFunctorAccessor<rttb::algorithms::arithmetic::doseOp::Multiply>
		            (appData._dose1, dose2Accessor, rttb::algorithms::arithmetic::doseOp::Multiply()));
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
	rttb::core::DoseAccessorInterface::DoseAccessorPointer outputAccessor = assembleOutputAccessor(
	            appData);

	std::cout << std::endl << "generate output image... ";
	io::itk::ITKImageAccessorConverter converter(outputAccessor);
	converter.setFailOnInvalidIDs(true);
	converter.process();
	io::itk::ITKImageAccessorConverter::ITKImageType::Pointer itkImage = converter.getITKImage();
	std::cout << "done." << std::endl;

	typedef ::itk::ImageFileWriter<io::itk::ITKImageAccessorConverter::ITKImageType> WriterType;

	std::cout << std::endl << "write output image... ";
	WriterType::Pointer writer = WriterType::New();
	writer->SetInput(itkImage);
	writer->SetFileName(appData._outputFileName);
	writer->SetUseCompression(true);
	writer->Write();
	std::cout << "done." << std::endl;
};
