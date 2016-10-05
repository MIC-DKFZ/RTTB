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
/*!
// @file
// @version $Revision: 1495 $ (last changed revision)
// @date $Date: 2016-09-29 16:17:47 +0200 (Do, 29 Sep 2016) $ (last change date)
// @author $Author: hentsch $ (last changed by)
*/

// this file defines the rttbCoreTests for the test driver
// and all it expects is that you have a function called RegisterTests

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>

#include "litCheckMacros.h"

#include "rttbBaseType.h"
#include "rttbGenericMaskedDoseIterator.h"
#include "rttbGenericDoseIterator.h"
#include "rttbDicomDoseAccessor.h"
#include "rttbDicomFileDoseAccessorGenerator.h"
#include "rttbDicomFileStructureSetGenerator.h"
#include "rttbBoostMaskAccessor.h"
#include "rttbITKImageMaskAccessorConverter.h"
#include "rttbImageWriter.h"
#include "rttbBoostMaskRedesign.h"
#include "rttbBoostMaskRedesignAccessor.h"
#include "rttbITKImageAccessorGenerator.h"
#include "rttbITKImageFileAccessorGenerator.h"
#include "rttbInvalidParameterException.h"

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
			typedef core::StructureSetGeneratorInterface::StructureSetPointer StructureSetPointer;

			std::string RTSTRUCT_FILENAME;
			std::string RTDOSE_FILENAME;
			std::string BoostMask_DIRNAME;
			std::string OTBMask_DIRNAME;
			std::string BoostMaskRedesign_DIRNAME;

			std::string RTDose_BoostRedesign;
			std::string RTStr_BoostRedesign;

			if (argc > 7)
			{
				RTSTRUCT_FILENAME = argv[1];
				RTDOSE_FILENAME = argv[2];
				BoostMask_DIRNAME = argv[3];
				OTBMask_DIRNAME = argv[4];
				BoostMaskRedesign_DIRNAME = argv[5];
                RTDose_BoostRedesign = argv[6];
                RTStr_BoostRedesign = argv[7];
			}

			OFCondition status;
			DcmFileFormat fileformat;

			/* read dicom-rt dose */
			io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator1(RTDOSE_FILENAME.c_str());
			DoseAccessorPointer doseAccessor1(doseAccessorGenerator1.generateDoseAccessor());

			//create a vector of MaskAccessors (one for each structure)
			StructureSetPointer rtStructureSet = io::dicom::DicomFileStructureSetGenerator(
			        RTSTRUCT_FILENAME.c_str()).generateStructureSet();

			if (rtStructureSet->getNumberOfStructures() > 0)
			{
                //do not compute struct "Aussenkontur" since it is very large (15000 cm³)
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

					//create Boost MaskAccessor redesign
					clock_t startR(clock());

					MaskAccessorPointer boostMaskRPtr
						= ::boost::make_shared<rttb::masks::boostRedesign::BoostMaskAccessor>
						    (rtStructureSet->getStructure(j), doseAccessor1->getGeometricInfo());
					CHECK_NO_THROW(boostMaskRPtr->updateMask());

					clock_t finishR(clock());
					std::cout << "Boost Mask Redesign Calculation: " << finishR - startR << " ms" <<
						        std::endl;


					rttb::io::itk::ITKImageMaskAccessorConverter itkConverterR(boostMaskRPtr);
					CHECK(itkConverterR.process());

                    boost::filesystem::path redesignFilename(BoostMaskRedesign_DIRNAME);
                    redesignFilename /= boost::lexical_cast<std::string>(j)+".nrrd";
                    rttb::io::itk::ImageWriter writerR(redesignFilename.string(), itkConverterR.getITKImage());
					CHECK(writerR.writeFile());

                    auto subtractedRedesignImage = subtractImages(otbMaskImage, itkConverterR.getITKImage());

                    boost::filesystem::path subtractRedesignFilename(BoostMaskRedesign_DIRNAME);
                    subtractRedesignFilename /= boost::lexical_cast<std::string>(j) + "_subtracted.nrrd";
                    rttb::io::itk::ImageWriter writerRSubtracted(subtractRedesignFilename.string(), subtractedRedesignImage);

                    CHECK(writerRSubtracted.writeFile());
			    }
			}

			/* Exception tests using data with different z spacing of dose and structure */
            io::itk::ITKImageFileAccessorGenerator doseAccessorGenerator2(RTDose_BoostRedesign.c_str());
			DoseAccessorPointer doseAccessor2(doseAccessorGenerator2.generateDoseAccessor());

			StructureSetPointer rtStructureSet2 = io::dicom::DicomFileStructureSetGenerator(
                RTStr_BoostRedesign.c_str()).generateStructureSet();


			if (rtStructureSet2->getNumberOfStructures() > 0)
			{
				for (size_t j = 12; j < 26; j++)
				{
                    std::cout << j << ": " << rtStructureSet2->getStructure(j)->getLabel() << std::endl;

                    //create Boost MaskAccessor
                    MaskAccessorPointer boostMaskAccessorPtr
                        = ::boost::make_shared<rttb::masks::boost::BoostMaskAccessor>
                        (rtStructureSet2->getStructure(j), doseAccessor2->getGeometricInfo());

                    //Two polygons in the same slice exception using boost mask, because of the different z spacing of dose and structure
                    if (j != 21)
                    {
                        CHECK_THROW_EXPLICIT(boostMaskAccessorPtr->updateMask(), rttb::core::InvalidParameterException);
                    }
                    else {
                        CHECK_NO_THROW(boostMaskAccessorPtr->updateMask());
                    }

                    //create Boost MaskAccessor redesign
                    MaskAccessorPointer boostMaskAccessorRedesignPtr
                        = ::boost::make_shared<rttb::masks::boostRedesign::BoostMaskAccessor>
                        (rtStructureSet2->getStructure(j), doseAccessor2->getGeometricInfo());

                    //No exception using redesigned boost mask
                    CHECK_NO_THROW(boostMaskAccessorRedesignPtr->updateMask());
				}
			}

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb

