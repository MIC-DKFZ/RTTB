// -----------------------------------------------------------------------
// RTToolbox - DKFZ radiotherapy quantitative evaluation library
//
// Copyright (c) German Cancer Research Center (DKFZ),
// Software development for Integrated Diagnostics and Therapy (SIDT).
// ALL RIGHTS RESERVED.
// See rttbCopyright.txt or
// http://www.dkfz.de/en/sidt/projects/rttb/copyright.html [^]
//
// This software is distributed WITHOUT ANY WARRANTY; without even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE. See the above copyright notices for more information.
//
//------------------------------------------------------------------------

// this file defines the rttbCoreTests for the test driver
// and all it expects is that you have a function called RegisterTests

#include <boost/make_shared.hpp>
#include <boost/filesystem.hpp>

#include "litCheckMacros.h"

#include "rttbGenericMaskedDoseIterator.h"
#include "rttbGenericDoseIterator.h"
#include "rttbDicomFileDoseAccessorGenerator.h"
#include "rttbDicomFileStructureSetGenerator.h"
#include "rttbBoostMaskAccessor.h"
#include "rttbITKImageMaskAccessorConverter.h"
#include "rttbImageWriter.h"
#include "rttbBoostMask.h"

#include "itkSubtractImageFilter.h"
#include "itkImageFileReader.h"

namespace rttb
{

    namespace testing
    {
        io::itk::ITKImageMaskAccessor::ITKMaskImageType::Pointer subtractImages(const io::itk::ITKImageMaskAccessor::ITKMaskImageType::Pointer image1, const io::itk::ITKImageMaskAccessor::ITKMaskImageType::Pointer image2)
        {
            typedef itk::SubtractImageFilter <io::itk::ITKImageMaskAccessor::ITKMaskImageType, io::itk::ITKImageMaskAccessor::ITKMaskImageType >
                SubtractImageFilterType;

            SubtractImageFilterType::Pointer subtractFilter = SubtractImageFilterType::New();
            subtractFilter->SetInput1(image1);
            subtractFilter->SetInput2(image2);
            //since the image origin may be modified through the writing process a bit
            subtractFilter->SetCoordinateTolerance(5e-2);
            subtractFilter->Update();

            return subtractFilter->GetOutput();
        }


        /*! @brief VoxelizationValidationTest.
        Compare the new boost voxelization to the OTB voxelization
        Check the creating of new boost masks for files where the old boost voxelization failed.
        */
        int VoxelizationValidationTest(int argc, char* argv[])
        {
            PREPARE_DEFAULT_TEST_REPORTING;

            typedef core::GenericDoseIterator::DoseAccessorPointer DoseAccessorPointer;
            typedef core::GenericMaskedDoseIterator::MaskAccessorPointer MaskAccessorPointer;
            typedef core::StructureSet::Pointer StructureSetPointer;

            std::string RTSTRUCT_FILENAME;
            std::string RTDOSE_FILENAME;
            std::string BoostMask_DIRNAME;
            std::string OTBMask_DIRNAME;

            if (argc > 4)
            {
                RTSTRUCT_FILENAME = argv[1];
                RTDOSE_FILENAME = argv[2];
                BoostMask_DIRNAME = argv[3];
                OTBMask_DIRNAME = argv[4];
            }

            //create directory
            boost::filesystem::create_directories(BoostMask_DIRNAME);

            /* read dicom-rt dose */
            io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator1(RTDOSE_FILENAME.c_str());
            DoseAccessorPointer doseAccessor1(doseAccessorGenerator1.generateDoseAccessor());

            //create a vector of MaskAccessors (one for each structure)
            StructureSetPointer rtStructureSet = io::dicom::DicomFileStructureSetGenerator(
                RTSTRUCT_FILENAME.c_str()).generateStructureSet();

            if (rtStructureSet->getNumberOfStructures() > 0)
            {
                //do not compute structure "Aussenkontur" since it is very large (15000 cm³)
                for (size_t j = 1; j < rtStructureSet->getNumberOfStructures(); j++)
                {
                    std::cout << j << ": " << rtStructureSet->getStructure(j)->getLabel() << std::endl;

                    //read OTB mask image
                    boost::filesystem::path otbMaskFilename(OTBMask_DIRNAME);
                    otbMaskFilename /= boost::lexical_cast<std::string>(j)+".mhd";

                    typedef itk::ImageFileReader<io::itk::ITKImageMaskAccessor::ITKMaskImageType> ReaderType;

                    ReaderType::Pointer readerOTB = ReaderType::New();
                    readerOTB->SetFileName(otbMaskFilename.string());
                    readerOTB->Update();
                    const io::itk::ITKImageMaskAccessor::ITKMaskImageType::Pointer otbMaskImage = readerOTB->GetOutput();

                    //create Boost MaskAccessor
                    clock_t startR(clock());

                    MaskAccessorPointer boostMaskRPtr
                        = ::boost::make_shared<rttb::masks::boost::BoostMaskAccessor>
                        (rtStructureSet->getStructure(j), doseAccessor1->getGeometricInfo());
                    CHECK_NO_THROW(boostMaskRPtr->updateMask());

                    clock_t finishR(clock());
                    std::cout << "Boost Mask Calculation: " << finishR - startR << " ms" <<
                        std::endl;


                    rttb::io::itk::ITKImageMaskAccessorConverter itkConverterR(boostMaskRPtr);
                    CHECK(itkConverterR.process());

                    boost::filesystem::path redesignFilename(BoostMask_DIRNAME);
                    redesignFilename /= boost::lexical_cast<std::string>(j)+".nrrd";
                    rttb::io::itk::ImageWriter writerR(redesignFilename.string(), itkConverterR.getITKImage().GetPointer());
                    CHECK(writerR.writeFile());

                    auto subtractedRedesignImage = subtractImages(otbMaskImage, itkConverterR.getITKImage());

                    boost::filesystem::path subtractRedesignFilename(BoostMask_DIRNAME);
                    subtractRedesignFilename /= boost::lexical_cast<std::string>(j)+"_subtracted.nrrd";
                    rttb::io::itk::ImageWriter writerRSubtracted(subtractRedesignFilename.string(), subtractedRedesignImage.GetPointer());

                    CHECK(writerRSubtracted.writeFile());
                }
            }

            RETURN_AND_REPORT_TEST_SUCCESS;
        }

    }//testing
}//rttb

