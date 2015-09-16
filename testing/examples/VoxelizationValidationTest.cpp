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
// @version $Revision: 929 $ (last changed revision)
// @date $Date: 2015-04-08 14:50:57 +0200 (Mi, 08 Apr 2015) $ (last change date)
// @author $Author: zhangl $ (last changed by)
*/

// this file defines the rttbCoreTests for the test driver
// and all it expects is that you have a function called RegisterTests

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include <iomanip>

#include "litCheckMacros.h"

#include "rttbBaseType.h"
#include "rttbDVHCalculator.h"
#include "rttbGenericMaskedDoseIterator.h"
#include "rttbGenericDoseIterator.h"
#include "rttbNullPointerException.h"
#include "rttbInvalidParameterException.h"
#include "rttbDicomDoseAccessor.h"
#include "rttbDicomFileDoseAccessorGenerator.h"
#include "rttbDicomFileStructureSetGenerator.h"
#include "rttbOTBMaskAccessor.h"
#include "rttbDVHTxtFileReader.h"
#include "rttbBoostMaskAccessor.h"
//#include "rttbITKImageMaskAccessorConverter.h"
//#include "rttbImageWriter.h"


namespace rttb
{

	namespace testing
	{

		/*! @brief VoxelizationValidationTest.
		Compare two differnt voxelizations: OTB and Boost.
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
			//ARGUMENTS: 1: structure file name
			//           2: dose1 file name
			//           3: directory name to write boost mask of all structures
			//           4: directory name to write OTB mask of all structures


			std::string RTSTRUCT_FILENAME ;
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

			OFCondition status;
			DcmFileFormat fileformat;

			/* read dicom-rt dose */
			io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator1(RTDOSE_FILENAME.c_str());
			DoseAccessorPointer doseAccessor1(doseAccessorGenerator1.generateDoseAccessor());
			boost::shared_ptr<core::GeometricInfo> geometricPtr = boost::make_shared<core::GeometricInfo>
			        (doseAccessor1->getGeometricInfo());

			//create a vector of MaskAccessors (one for each structure)
			StructureSetPointer rtStructureSet = io::dicom::DicomFileStructureSetGenerator(
			        RTSTRUCT_FILENAME.c_str()).generateStructureSet();

			std::vector<MaskAccessorPointer> rtStructSetMaskAccessorVec;


			if (rtStructureSet->getNumberOfStructures() > 0)
			{
				for (int j = 0; j < rtStructureSet->getNumberOfStructures(); j++)
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
					                               doseAccessor1->getDoseUID());
					rttb::core::DVH dvh = *(calc.generateDVH());

					clock_t finish(clock());
					std::cout << "OTB Mask Calculation time: " << finish - start << " ms" << std::endl;

					//Write the mask image to a file.
					/*! It takes a long time to write all mask files so that RUN_TESTS causes a timeout error.
						To write all mask files, please use the outcommented code and call the .exe directly!
					*/
					/*rttb::io::itk::ITKImageMaskAccessorConverter itkConverter(spOTBMaskAccessor);
					CHECK(itkConverter.process());
					std::stringstream fileNameSstr;
					fileNameSstr<<OTBMask_DIRNAME<<j<<".mhd";
					rttb::io::itk::ImageWriter writer(fileNameSstr.str(), itkConverter.getITKImage());
					CHECK(writer.writeFile());*/


					clock_t start2(clock());
					//create Boost MaskAccessor
					MaskAccessorPointer boostMaskAccessorPtr = ::boost::make_shared<rttb::masks::boost::BoostMaskAccessor>
					        (rtStructureSet->getStructure(j), geometricPtr);
					CHECK_NO_THROW(boostMaskAccessorPtr->updateMask());

					::boost::shared_ptr<core::GenericMaskedDoseIterator> spMaskedDoseIteratorTmp2 =
					    ::boost::make_shared<core::GenericMaskedDoseIterator>(boostMaskAccessorPtr, doseAccessor1);
					DoseIteratorPointer spMaskedDoseIterator2(spMaskedDoseIteratorTmp2);
					rttb::core::DVHCalculator calc2(spMaskedDoseIterator2, (rtStructureSet->getStructure(j))->getUID(),
					                                doseAccessor1->getDoseUID());
					rttb::core::DVH dvh2 = *(calc2.generateDVH());

					clock_t finish2(clock());
					std::cout << "Boost Mask Calculation and write file time: " << finish2 - start2 << " ms" << std::endl;

					//Write the mask image to a file.
					/*! It takes a long time to write all mask files so that RUN_TESTS causes a timeout error.
						To write all mask files, please use the outcommented code and call the .exe directly!
					*/
					/*rttb::io::itk::ITKImageMaskAccessorConverter itkConverter2(boostMaskAccessorPtr);
					CHECK(itkConverter2.process());
					std::stringstream fileNameSstr2;
					fileNameSstr2<<BoostMask_DIRNAME<<j<<".mhd";
					rttb::io::itk::ImageWriter writer2(fileNameSstr2.str(), itkConverter2.getITKImage());
					CHECK(writer2.writeFile());*/


					//check close of 2 voxelizatin: OTB and Boost
					CHECK_CLOSE(dvh.getMaximum(), dvh2.getMaximum(), 0.1);
					CHECK_CLOSE(dvh.getMinimum(), dvh2.getMinimum(), 0.1);

					if (j != 7) //7: Ref.Pkt, mean = -1.#IND
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

				}
			}



			RETURN_AND_REPORT_TEST_SUCCESS;


		}

	}//testing
}//rttb

