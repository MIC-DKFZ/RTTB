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


namespace rttb
{

	namespace testing
	{

		/*! @brief DVHCalculatorTest.
		Test if calculation in new architecture returns similar results to the
		original implementation.

		WARNING: The values for comparison need to be adjusted if the input files are changed!

		This test can be used to get more detailed information, but it will always fail, because differences in voxelization accuracy,
		especially the ones caused by the change from double to float precission will not cause considerable deviations in the structure
		sizes, which correspond to considerable differences in the calculated DVHs.

		Even in double precission differences up to 0.005 between values from old and new implementation can occure.
		*/

		int DVHCalculatorComparisonTest(int argc, char* argv[])
		{
			typedef core::GenericDoseIterator::DoseAccessorPointer DoseAccessorPointer;
			typedef core::GenericMaskedDoseIterator::MaskAccessorPointer MaskAccessorPointer;
			typedef core::DVHCalculator::DoseIteratorPointer DoseIteratorPointer;
			typedef core::StructureSetGeneratorInterface::StructureSetPointer StructureSetPointer;

			PREPARE_DEFAULT_TEST_REPORTING;
			//ARGUMENTS: 1: structure file name
			//           2: dose1 file name
			//           3: dose2 file name
			//           4: dose3 file name

			std::string RTSTRUCT_FILENAME;
			std::string RTDOSE_FILENAME;
			std::string RTDOSE2_FILENAME;
			std::string RTDOSE3_FILENAME;
			std::string COMPARISON_DVH_FOLDER;


			if (argc > 1)
			{
				RTSTRUCT_FILENAME = argv[1];
			}

			if (argc > 2)
			{
				RTDOSE_FILENAME = argv[2];
			}

			if (argc > 3)
			{
				RTDOSE2_FILENAME = argv[3];
			}

			if (argc > 4)
			{
				RTDOSE3_FILENAME = argv[4];
			}

			if (argc > 5)
			{
				COMPARISON_DVH_FOLDER = argv[5];
			}


			OFCondition status;
			DcmFileFormat fileformat;

			/* read dicom-rt dose */
			io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator1(RTDOSE_FILENAME.c_str());
			DoseAccessorPointer doseAccessor1(doseAccessorGenerator1.generateDoseAccessor());

			//create a vector of MaskAccessors (one for each structure)
			StructureSetPointer rtStructureSet = io::dicom::DicomFileStructureSetGenerator(
			        RTSTRUCT_FILENAME.c_str()).generateStructureSet();

			std::vector<MaskAccessorPointer> rtStructSetMaskAccessorVec;

			::DRTDoseIOD rtdose2;
			io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator2(RTDOSE2_FILENAME.c_str());
			DoseAccessorPointer doseAccessor2(doseAccessorGenerator2.generateDoseAccessor());


			::DRTDoseIOD rtdose3;
			io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator3(RTDOSE3_FILENAME.c_str());
			DoseAccessorPointer doseAccessor3(doseAccessorGenerator3.generateDoseAccessor());


			double maxDifference = 0;
			double difference = 0;
			double minDifference = 1000;
			clock_t start(clock());

			if (rtStructureSet->getNumberOfStructures() > 0)
			{
				for (int j = 0; j < rtStructureSet->getNumberOfStructures(); j++)
				{

					//create MaskAccessor
					::boost::shared_ptr<masks::legacy::OTBMaskAccessor> spOTBMaskAccessor =
					    ::boost::make_shared<masks::legacy::OTBMaskAccessor>(rtStructureSet->getStructure(j),
					            doseAccessor1->getGeometricInfo());
					spOTBMaskAccessor->updateMask();
					MaskAccessorPointer spMaskAccessor(spOTBMaskAccessor);
					//create corresponding MaskedDoseIterator
					::boost::shared_ptr<core::GenericMaskedDoseIterator> spMaskedDoseIteratorTmp =
					    ::boost::make_shared<core::GenericMaskedDoseIterator>(spMaskAccessor, doseAccessor1);
					DoseIteratorPointer spMaskedDoseIterator(spMaskedDoseIteratorTmp);
					//store MaskAccessor
					rtStructSetMaskAccessorVec.push_back(spMaskAccessor);

					rttb::core::DVHCalculator calc(spMaskedDoseIterator, (rtStructureSet->getStructure(j))->getUID(),
					                               doseAccessor1->getUID());
					std::string dvhFileName = "dvh1";
					std::string label = (rtStructureSet->getStructure(j))->getLabel();
					dvhFileName.append(label);

					if (dvhFileName.find("/") != std::string::npos)
					{
						dvhFileName.replace(dvhFileName.find("/"), 1, "");
					}

					std::cout << "=== Dose 1: " << label << "===" << std::endl;
					rttb::io::other::DVHTxtFileReader dvhOriginalReader = rttb::io::other::DVHTxtFileReader(
					            COMPARISON_DVH_FOLDER + dvhFileName + ".txt");
					rttb::core::DVH dvhOrig = *(dvhOriginalReader.generateDVH());
					rttb::core::DVH::DataDifferentialType dvhOrigData = dvhOrig.getDataDifferential();

					rttb::core::DVH dvh = *(calc.generateDVH());
					rttb::core::DVH::DataDifferentialType dvhData = dvh.getDataDifferential();

					CHECK_EQUAL(dvhOrig, dvh);
					CHECK_CLOSE(dvhOrig.getMaximum(), dvh.getMaximum(), errorConstant);
					CHECK_CLOSE(dvhOrig.getMinimum(), dvh.getMinimum(), errorConstant);
					CHECK_CLOSE(dvhOrig.getMean(), dvh.getMean(), errorConstant);

					rttb::core::DVH::DataDifferentialType::iterator it;
					rttb::core::DVH::DataDifferentialType::iterator itOrig;
					itOrig = dvhOrigData.begin();

					for (it = dvhData.begin(); it != dvhData.end() || itOrig != dvhOrigData.end(); ++it, ++itOrig)
					{
						CHECK_CLOSE(*(itOrig), *(it), errorConstant);
						std::cout << std::setprecision(20) << "difference: " << abs(*(itOrig) - * (it)) << std::endl;
						difference = abs(*(itOrig) - * (it));

						if (difference < minDifference)
						{
							minDifference = difference;
						}

						if (difference > maxDifference)
						{
							maxDifference = difference;
						}
					}
				}
			}

			clock_t finish(clock());
			std::cout << std::setprecision(20) << "max(difference): " << maxDifference << std::endl;
			std::cout << std::setprecision(20) << "min(difference): " << minDifference << std::endl;

			std::cout << "DVH Calculation time: " << finish - start << " ms" << std::endl;

			maxDifference = 0;
			minDifference = 1000;

			clock_t start2(clock());

			for (int j = 0; j < rtStructSetMaskAccessorVec.size(); j++)
			{
				//create corresponding MaskedDoseIterator
				::boost::shared_ptr<core::GenericMaskedDoseIterator> spMaskedDoseIteratorTmp =
				    ::boost::make_shared<core::GenericMaskedDoseIterator>(rtStructSetMaskAccessorVec.at(j),
				            doseAccessor2);
				DoseIteratorPointer spMaskedDoseIterator(spMaskedDoseIteratorTmp);

				rttb::core::DVHCalculator calc(spMaskedDoseIterator, (rtStructureSet->getStructure(j))->getUID(),
				                               doseAccessor2->getUID());
				std::string dvhFileName = "dvh2";
				std::string label = (rtStructureSet->getStructure(j))->getLabel();
				dvhFileName.append(label);

				if (dvhFileName.find("/") != std::string::npos)
				{
					dvhFileName.replace(dvhFileName.find("/"), 1, "");
				}

				std::cout << "=== Dose 2: " << label << "===" << std::endl;
				rttb::io::other::DVHTxtFileReader dvhOriginalReader = rttb::io::other::DVHTxtFileReader(
				            COMPARISON_DVH_FOLDER + dvhFileName + ".txt");
				rttb::core::DVH dvhOrig = *(dvhOriginalReader.generateDVH());
				rttb::core::DVH::DataDifferentialType dvhOrigData = dvhOrig.getDataDifferential();

				rttb::core::DVH dvh = *(calc.generateDVH());
				rttb::core::DVH::DataDifferentialType dvhData = dvh.getDataDifferential();

				CHECK_EQUAL(dvhOrig, dvh);
				CHECK_CLOSE(dvhOrig.getMaximum(), dvh.getMaximum(), errorConstant);
				CHECK_CLOSE(dvhOrig.getMinimum(), dvh.getMinimum(), errorConstant);
				CHECK_CLOSE(dvhOrig.getMean(), dvh.getMean(), errorConstant);

				rttb::core::DVH::DataDifferentialType::iterator it;
				rttb::core::DVH::DataDifferentialType::iterator itOrig;
				itOrig = dvhOrigData.begin();

				for (it = dvhData.begin(); it != dvhData.end() || itOrig != dvhOrigData.end(); ++it, ++itOrig)
				{
					CHECK_CLOSE(*(itOrig), *(it), errorConstant);
					std::cout << std::setprecision(20) << "difference: " << abs(*(itOrig) - * (it)) << std::endl;
					difference = abs(*(itOrig) - * (it));

					if (difference > 10)
					{
						std::cout << "large difference: " << abs(*(itOrig) - * (it)) << " = " << *(itOrig) << " - " << *
						          (it) << std::endl;
					}

					if (difference < minDifference)
					{
						minDifference = difference;
					}

					if (difference > maxDifference)
					{
						maxDifference = difference;
					}
				}
			}

			clock_t finish2(clock());

			std::cout << std::setprecision(20) << "max(difference): " << maxDifference << std::endl;
			std::cout << std::setprecision(20) << "min(difference): " << minDifference << std::endl;

			std::cout << "Reset dose 2, DVH Calculation time: " << finish2 - start2 << " ms" << std::endl;

			maxDifference = 0;
			minDifference = 1000;

			clock_t start3(clock());

			for (int j = 0; j < rtStructSetMaskAccessorVec.size(); j++)
			{
				//create corresponding MaskedDoseIterator
				::boost::shared_ptr<core::GenericMaskedDoseIterator> spMaskedDoseIteratorTmp =
				    ::boost::make_shared<core::GenericMaskedDoseIterator>(rtStructSetMaskAccessorVec.at(j),
				            doseAccessor3);
				DoseIteratorPointer spMaskedDoseIterator(spMaskedDoseIteratorTmp);

				rttb::core::DVHCalculator calc(spMaskedDoseIterator, (rtStructureSet->getStructure(j))->getUID(),
				                               doseAccessor3->getUID());
				std::string dvhFileName = "dvh3";
				std::string label = (rtStructureSet->getStructure(j))->getLabel();
				dvhFileName.append(label);

				if (dvhFileName.find("/") != std::string::npos)
				{
					dvhFileName.replace(dvhFileName.find("/"), 1, "");
				}

				std::cout << "=== Dose 3: " << label << "===" << std::endl;
				rttb::io::other::DVHTxtFileReader dvhOriginalReader = rttb::io::other::DVHTxtFileReader(
				            COMPARISON_DVH_FOLDER + dvhFileName + ".txt");
				rttb::core::DVH dvhOrig = *(dvhOriginalReader.generateDVH());
				rttb::core::DVH::DataDifferentialType dvhOrigData = dvhOrig.getDataDifferential();

				rttb::core::DVH dvh = *(calc.generateDVH());
				rttb::core::DVH::DataDifferentialType dvhData = dvh.getDataDifferential();

				CHECK_EQUAL(dvhOrig, dvh);
				CHECK_CLOSE(dvhOrig.getMaximum(), dvh.getMaximum(), errorConstant);
				CHECK_CLOSE(dvhOrig.getMinimum(), dvh.getMinimum(), errorConstant);
				CHECK_CLOSE(dvhOrig.getMean(), dvh.getMean(), errorConstant);

				rttb::core::DVH::DataDifferentialType::iterator it;
				rttb::core::DVH::DataDifferentialType::iterator itOrig;
				itOrig = dvhOrigData.begin();

				for (it = dvhData.begin(); it != dvhData.end() || itOrig != dvhOrigData.end(); ++it, ++itOrig)
				{
					CHECK_CLOSE(*(itOrig), *(it), errorConstant);
					std::cout << std::setprecision(20) << "difference: " << abs(*(itOrig) - * (it)) << std::endl;

					if (difference > 10)
					{
						std::cout << "large difference: " << abs(*(itOrig) - * (it)) << " = " << *(itOrig) << " - " << *
						          (it) << std::endl;
					}

					difference = abs(*(itOrig) - * (it));

					if (difference < minDifference)
					{
						minDifference = difference;
					}

					if (difference > maxDifference)
					{
						maxDifference = difference;
					}
				}
			}

			clock_t finish3(clock());

			std::cout << std::setprecision(20) << "max(difference): " << maxDifference << std::endl;
			std::cout << std::setprecision(20) << "min(difference): " << minDifference << std::endl;

			std::cout << "Reset dose 3, DVH Calculation time: " << finish3 - start3 << " ms" << std::endl;


			RETURN_AND_REPORT_TEST_SUCCESS;


		}

	}//testing
}//rttb

