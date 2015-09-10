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
// @version $Revision: 741 $ (last changed revision)
// @date    $Date: 2014-09-16 16:34:22 +0200 (Di, 16 Sep 2014) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/


#include "DoseMapHelper.h"

#include "mapRegistrationFileReader.h"
#include "itkImageFileWriter.h"

#include "rttbVirtuosPlanFileDoseAccessorGenerator.h"
#include "rttbDicomFileDoseAccessorGenerator.h"
#include "rttbDicomHelaxFileDoseAccessorGenerator.h"

#include "rttbITKImageFileDoseAccessorGenerator.h"
#include "rttbDicomFileDoseAccessorGenerator.h"
#include "rttbSimpleMappableDoseAccessor.h"
#include "rttbMatchPointTransformation.h"
#include "rttbLinearInterpolation.h"
#include "rttbNearestNeighborInterpolation.h"
#include "rttbRosuMappableDoseAccessor.h"
#include "rttbITKImageDoseAccessorConverter.h"
#include "rttbArithmetic.h"
#include "rttbBinaryFunctorDoseAccessor.h"
#include "rttbExceptionMacros.h"


rttb::core::DoseAccessorInterface::DoseAccessorPointer
rttb::apps::doseMap::loadDose(const std::string& fileName,
                              const rttb::apps::doseMap::ApplicationData::LoadingStyleArgType& args)
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
rttb::apps::doseMap::loadDicomDose(const std::string& fileName)
{
  rttb::io::dicom::DicomFileDoseAccessorGenerator generator(fileName);
  return generator.generateDoseAccessor();
};

rttb::core::DoseAccessorInterface::DoseAccessorPointer
rttb::apps::doseMap::loadHelaxDose(const std::string& path)
{
  rttb::io::helax::DicomHelaxFileDoseAccessorGenerator generator(path);
  return generator.generateDoseAccessor();
};

rttb::core::DoseAccessorInterface::DoseAccessorPointer
rttb::apps::doseMap::loadITKDose(const std::string& fileName)
{
  rttb::io::itk::ITKImageFileDoseAccessorGenerator generator(fileName);
  return generator.generateDoseAccessor();
};

rttb::core::DoseAccessorInterface::DoseAccessorPointer
rttb::apps::doseMap::loadVirtuosDose(const std::string& fileName, const std::string& planFileName)
{
  rttb::io::virtuos::VirtuosPlanFileDoseAccessorGenerator generator(fileName, planFileName);
  return generator.generateDoseAccessor();
};

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
rttb::core::DoseAccessorInterface::DoseAccessorPointer
assembleOutputAccessor(rttb::apps::doseMap::ApplicationData& appData)
{

  rttb::core::DoseAccessorInterface::DoseAccessorPointer outputAccessor = appData._inputDose;

  rttb::interpolation::TransformationInterface::Pointer transform =
    rttb::interpolation::TransformationInterface::Pointer(new
        rttb::interpolation::MatchPointTransformation(appData._spReg));

  if (appData._interpolatorName == "rosu")
  {
    outputAccessor = rttb::core::DoseAccessorInterface::DoseAccessorPointer(
                       new rttb::interpolation::RosuMappableDoseAccessor(appData._refDose->getGeometricInfo(),
                           appData._inputDose, transform));
  }
  else
  {
    rttb::interpolation::InterpolationBase::Pointer interpolate =
      rttb::interpolation::LinearInterpolation::Pointer(new
          rttb::interpolation::LinearInterpolation());

    if (appData._interpolatorName == "nn")
    {
      interpolate = rttb::interpolation::NearestNeighborInterpolation::Pointer(new
                    rttb::interpolation::NearestNeighborInterpolation());
    }
    else if (appData._interpolatorName != "linear")
    {
      mapDefaultExceptionStaticMacro( <<
                                      "Unkown interpolation type selected. Cannot map dose. Interpolation type: " <<
                                      appData._interpolatorName);
    }

    outputAccessor = rttb::core::DoseAccessorInterface::DoseAccessorPointer(
                       new rttb::interpolation::SimpleMappableDoseAccessor(appData._refDose->getGeometricInfo(),
                           appData._inputDose, transform, interpolate));
  }

  return outputAccessor;
}

void
rttb::apps::doseMap::processData(rttb::apps::doseMap::ApplicationData& appData)
{
  rttb::core::DoseAccessorInterface::DoseAccessorPointer outputAccessor = assembleOutputAccessor(
        appData);

  std::cout << std::endl << "generate output image... ";
  io::itk::ITKImageDoseAccessorConverter converter(outputAccessor);
  converter.setFailOnInvalidIDs(true);
  converter.process();
  io::itk::ITKDoseImageType::Pointer itkImage = converter.getITKImage();
  std::cout << "done." << std::endl;

  typedef ::itk::ImageFileWriter<io::itk::ITKDoseImageType> WriterType;

  std::cout << std::endl << "write output image... ";
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(itkImage);
  writer->SetFileName(appData._outputFileName);
  writer->Write();
  std::cout << "done." << std::endl;
};