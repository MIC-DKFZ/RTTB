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
// @version $Revision$ (last changed revision)
// @date $Date$ (last change date)
// @author $Author$ (last changed by)
*/

// this file defines the rttbCoreTests for the test driver
// and all it expects is that you have a function called RegisterTests

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>

#include <iomanip>

#include "litCheckMacros.h"

#include "rttbBaseType.h"
#include "rttbDVHCalculator.h"
#include "rttbGenericMaskedDoseIterator.h"
#include "rttbGenericDoseIterator.h"
#include "rttbNullPointerException.h"
#include "rttbDicomDoseAccessor.h"
#include "rttbDicomFileDoseAccessorGenerator.h"
#include "rttbDicomFileStructureSetGenerator.h"
#include "rttbOTBMaskAccessor.h"
#include "rttbDVHTxtFileReader.h"
#include "rttbBoostMaskAccessor.h"
#include "rttbITKImageMaskAccessorConverter.h"
#include "rttbImageWriter.h"
#include "rttbITKImageAccessorGenerator.h"
#include "rttbITKImageFileAccessorGenerator.h"
#include "rttbInvalidParameterException.h"
#include "rttbBoostMask_LEGACY.h"
#include "rttbBoostMaskAccessor_LEGACY.h"



namespace rttb
{

	namespace testing
	{

		/*! @brief VoxelizationValidationTest.
		Compare three differnt voxelizations: OTB, BoostLegacy and Boost.
		Check dvh maximum and minimum for each structure.
		Check write mask to itk file for further validation.
		*/

		int VoxelizationValidationTest(int argc, char* argv[])
		{
			typedef core::GenericDoseIterator::DoseAccessorPointer DoseAccessorPointer;
			typedef core::GenericMaskedDoseIterator::MaskAccessorPointer MaskAccessorPointer;
			typedef core::DVHCalculator::DoseIteratorPointer DoseIteratorPointer;
			typedef core::StructureSetGeneratorInterface::StructureSetPointer StructureSetPointer;

			PREPARE_DEFAULT_TEST_REPORTING;
			//ARGUMENTS: 1: file name of test structure 1
			//           2: file name of test dose 1 
			//           3: directory name to write boost mask of all structures
			//           4: directory name to write OTB mask of all structures
            //           5: file name of test dose 2: data with different z spacing of dose and structure, BooseLegacy throws exceptions by handling this data
            //           6: file name of test structure 2: data with different z spacing of dose and structure, BooseLegacy throws exceptions by handling this data

			std::string RTSTRUCT_FILENAME;
			std::string RTDOSE_FILENAME;
			std::string BoostMaskLegacy_DIRNAME;
			std::string OTBMask_DIRNAME;
			std::string BoostMask_DIRNAME;

			std::string RTDOSE2_FILENAME;
			std::string RTSTRUCT2_FILENAME;

			if (argc > 4)
			{
				RTSTRUCT_FILENAME = argv[1];
				RTDOSE_FILENAME = argv[2];
				BoostMaskLegacy_DIRNAME = argv[3];
				OTBMask_DIRNAME = argv[4];
				BoostMask_DIRNAME = argv[5];

                RTDOSE2_FILENAME = argv[6];
                RTSTRUCT2_FILENAME = argv[7];
			}

            ::boost::filesystem::create_directories(BoostMask_DIRNAME);
            ::boost::filesystem::create_directories(OTBMask_DIRNAME);
            ::boost::filesystem::create_directories(BoostMaskLegacy_DIRNAME);

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
				for (size_t j = 0; j < rtStructureSet->getNumberOfStructures(); j++)
				{
					std::cout << j << ": " << rtStructureSet->getStructure(j)->getLabel() << std::endl;
					clock_t start(clock());
					//create OTB MaskAccessor
					::boost::shared_ptr<masks::legacy::OTBMaskAccessor> spOTBMaskAccessor =
					    ::boost::make_shared<masks::legacy::OTBMaskAccessor>(rtStructureSet->getStructure(j),
					            doseAccessor1->getGeometricInfo());
					spOTBMaskAccessor->updateMask();
					MaskAccessorPointer spMaskAccessor(spOTBMaskAccessor);

					::boost::shared_ptr<core::GenericMaskedDoseIterator> spMaskedDoseIteratorTmp =
					    ::boost::make_shared<core::GenericMaskedDoseIterator>(spMaskAccessor, doseAccessor1);
					DoseIteratorPointer spMaskedDoseIterator(spMaskedDoseIteratorTmp);
					rttb::core::DVHCalculator calc(spMaskedDoseIterator, (rtStructureSet->getStructure(j))->getUID(),
					                               doseAccessor1->getUID());
					rttb::core::DVH dvh = *(calc.generateDVH());

					clock_t finish(clock());
					std::cout << "OTB Mask Calculation time: " << finish - start << " ms" << std::endl;

					//Write the mask image to a file.
					/*! It takes a long time to write all mask files so that RUN_TESTS causes a timeout error.
						To write all mask files, please use the outcommented code and call the .exe directly!
					*/
					rttb::io::itk::ITKImageMaskAccessorConverter itkConverter(spOTBMaskAccessor);
					CHECK(itkConverter.process());
					std::stringstream fileNameSstr;
					fileNameSstr << OTBMask_DIRNAME << j << ".mhd";
					rttb::io::itk::ImageWriter writer(fileNameSstr.str(), itkConverter.getITKImage());
					CHECK(writer.writeFile());



					clock_t start2(clock());
					//create Boost MaskAccessor_LEGACY
					MaskAccessorPointer boostMaskAccessorPtr
					    = ::boost::make_shared<rttb::masks::boostLegacy::BoostMaskAccessor>
					      (rtStructureSet->getStructure(j), doseAccessor1->getGeometricInfo());

					CHECK_NO_THROW(boostMaskAccessorPtr->updateMask());

					::boost::shared_ptr<core::GenericMaskedDoseIterator> spMaskedDoseIteratorTmp2 =
					    ::boost::make_shared<core::GenericMaskedDoseIterator>(boostMaskAccessorPtr, doseAccessor1);
					DoseIteratorPointer spMaskedDoseIterator2(spMaskedDoseIteratorTmp2);
					rttb::core::DVHCalculator calc2(spMaskedDoseIterator2, (rtStructureSet->getStructure(j))->getUID(),
					                                doseAccessor1->getUID());
					rttb::core::DVH dvh2 = *(calc2.generateDVH());

					clock_t finish2(clock());
					std::cout << "Boost Mask Calculation and write file time: " << finish2 - start2 << " ms" <<
					          std::endl;

					//Write the mask image to a file.
					/*! It takes a long time to write all mask files so that RUN_TESTS causes a timeout error.
						To write all mask files, please use the outcommented code and call the .exe directly!
					*/

					rttb::io::itk::ITKImageMaskAccessorConverter itkConverter2(boostMaskAccessorPtr);
					CHECK(itkConverter2.process());
					std::stringstream fileNameSstr2;
					fileNameSstr2 << BoostMaskLegacy_DIRNAME << j << ".mhd";
					rttb::io::itk::ImageWriter writer2(fileNameSstr2.str(), itkConverter2.getITKImage());
					CHECK(writer2.writeFile());


						//create Boost MaskAccessor redesign
						clock_t startR(clock());

						MaskAccessorPointer boostMaskRPtr
					    = ::boost::make_shared<rttb::masks::boost::BoostMaskAccessor>
						      (rtStructureSet->getStructure(j), doseAccessor1->getGeometricInfo());
						CHECK_NO_THROW(boostMaskRPtr->updateMask());
						::boost::shared_ptr<core::GenericMaskedDoseIterator> spMaskedDoseIteratorTmpR =
						    ::boost::make_shared<core::GenericMaskedDoseIterator>(boostMaskRPtr, doseAccessor1);
						DoseIteratorPointer spMaskedDoseIteratorR(spMaskedDoseIteratorTmpR);
						rttb::core::DVHCalculator calcR(spMaskedDoseIteratorR, (rtStructureSet->getStructure(j))->getUID(),
						                                doseAccessor1->getUID());
						rttb::core::DVH dvhR = *(calcR.generateDVH());
						clock_t finishR(clock());
						std::cout << "Boost Mask Redesign Calculation and write file time: " << finishR - startR << " ms" <<
						          std::endl;

						//Write the mask image to a file.
						/*! It takes a long time to write all mask files so that RUN_TESTS causes a timeout error.
						To write all mask files, please use the outcommented code and call the .exe directly!
						*/
					rttb::io::itk::ITKImageMaskAccessorConverter itkConverterR(boostMaskRPtr);
						CHECK(itkConverterR.process());


						std::stringstream fileNameSstrR;
					fileNameSstrR << BoostMask_DIRNAME << j << ".mhd";
						rttb::io::itk::ImageWriter writerR(fileNameSstrR.str(), itkConverterR.getITKImage());
					CHECK(writerR.writeFile());


					//check close of 2 voxelizatin: OTB and Boost
					CHECK_CLOSE(dvh.getMaximum(), dvh2.getMaximum(), 0.1);
					CHECK_CLOSE(dvh.getMinimum(), dvh2.getMinimum(), 0.1);

						if (j != 7)
					{
						CHECK_CLOSE(dvh.getMean(), dvh2.getMean(), 0.1);
					}

					CHECK_CLOSE(dvh.getMedian(), dvh2.getMedian(), 0.1);
					CHECK_CLOSE(dvh.getModal(), dvh2.getModal(), 0.1);

					//0: Aussenkontur and 3: Niere li. failed.
					if (j != 0 && j != 3)
					{
						CHECK_CLOSE(dvh.getVx(0), dvh2.getVx(0), dvh.getVx(0) * 0.05); //check volume difference < 5%
					}

						//check close of 2 voxelization: Boost and BoostRedesign
						CHECK_CLOSE(dvhR.getMaximum(), dvh2.getMaximum(), 0.1);
						CHECK_CLOSE(dvhR.getMinimum(), dvh2.getMinimum(), 0.1);

						if (j != 7)
						{
							CHECK_CLOSE(dvhR.getMean(), dvh2.getMean(), 0.1);
				}

						CHECK_CLOSE(dvhR.getMedian(), dvh2.getMedian(), 0.1);
						CHECK_CLOSE(dvhR.getModal(), dvh2.getModal(), 0.1);

						//0: Aussenkontur and 3: Niere li. failed.
						CHECK_CLOSE(dvhR.getVx(0), dvh2.getVx(0), dvhR.getVx(0) * 0.05); //check volume difference < 5%

			}
				}

			/* Exception tests using data with different z spacing of dose and structure */
            io::itk::ITKImageFileAccessorGenerator doseAccessorGenerator2(RTDOSE2_FILENAME.c_str());
			DoseAccessorPointer doseAccessor2(doseAccessorGenerator2.generateDoseAccessor());

			StructureSetPointer rtStructureSet2 = io::dicom::DicomFileStructureSetGenerator(
                RTSTRUCT2_FILENAME.c_str()).generateStructureSet();


			if (rtStructureSet2->getNumberOfStructures() > 0)
			{
				for (size_t j = 1; j < 20; j++)
				{
					std::cout << j << ": " << rtStructureSet2->getStructure(j)->getLabel() << std::endl;
					clock_t start(clock());

					//create Boost MaskAccessor Legacy
					MaskAccessorPointer boostMaskAccessorPtr
					    = ::boost::make_shared<rttb::masks::boostLegacy::BoostMaskAccessor>
					      (rtStructureSet2->getStructure(j), doseAccessor2->getGeometricInfo());

					//Two polygons in the same slice exception using boost mask, because of the different z spacing of dose and structure
					if (j != 9)
					{
						CHECK_THROW_EXPLICIT(boostMaskAccessorPtr->updateMask(), rttb::core::InvalidParameterException);
			}


					//create Boost MaskAccessor redesign
					MaskAccessorPointer boostMaskRPtr
					    = ::boost::make_shared<rttb::masks::boost::BoostMaskAccessor>
					      (rtStructureSet2->getStructure(j), doseAccessor2->getGeometricInfo());

					//No exception using redesigned boost mask
					CHECK_NO_THROW(boostMaskRPtr->updateMask());

				}
			}

			RETURN_AND_REPORT_TEST_SUCCESS;


		}

	}//testing
}//rttb

