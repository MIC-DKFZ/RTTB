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


#include "dcmtk/config/osconfig.h"    /* make sure OS specific configuration is included first */

#include "dcmtk/dcmrt/drtdose.h"
#include "dcmtk/dcmrt/drtstrct.h"


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


namespace rttb
{

	namespace testing
	{

		/*! @brief DVHCalculatorTest.
			Test if calculation in new architecture returns similar results to the
			original implementation.

			WARNING: The values for comparison need to be adjusted if the input files are changed!
		*/

		int DVHCalculatorExampleTest(int argc, char* argv[])
		{
			typedef core::GenericDoseIterator::DoseAccessorPointer DoseAccessorPointer;
			typedef core::GenericMaskedDoseIterator::MaskAccessorPointer MaskAccessorPointer;
			typedef core::DVHCalculator::DoseIteratorPointer DoseIteratorPointer;
			typedef core::DVHCalculator::MaskedDoseIteratorPointer MaskedDoseIteratorPointer;
			typedef masks::OTBMaskAccessor::StructTypePointer StructTypePointer;
			typedef core::DVH::DVHPointer DVHPointer;
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


			OFCondition status;
			DcmFileFormat fileformat;

			// read dicom-rt dose
			::DRTDoseIOD rtdose1;
			io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator1(RTDOSE_FILENAME.c_str());
			DoseAccessorPointer doseAccessor1(doseAccessorGenerator1.generateDoseAccessor());

			//create a vector of MaskAccessors (one for each structure)
			StructureSetPointer rtStructureSet = io::dicom::DicomFileStructureSetGenerator(
			        RTSTRUCT_FILENAME.c_str()).generateStructureSet();

			//storeage for mask accessors to reduce time spent on voxelization (perform only once)
			std::vector<MaskAccessorPointer> rtStructSetMaskAccessorVec;

			::DRTDoseIOD rtdose2;
			io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator2(RTDOSE2_FILENAME.c_str());
			DoseAccessorPointer doseAccessor2(doseAccessorGenerator2.generateDoseAccessor());


			::DRTDoseIOD rtdose3;
			io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator3(RTDOSE3_FILENAME.c_str());
			DoseAccessorPointer doseAccessor3(doseAccessorGenerator3.generateDoseAccessor());


			//start evaluation
			clock_t start(clock());

			if (rtStructureSet->getNumberOfStructures() > 0)
			{
				for (int j = 0; j < rtStructureSet->getNumberOfStructures(); j++)
				{
					std::cout << rtStructureSet->getStructure(j)->getLabel() << std::endl;

					//create MaskAccessor for each structure
					boost::shared_ptr<masks::OTBMaskAccessor> spOTBMaskAccessor =
					    boost::make_shared<masks::OTBMaskAccessor>(rtStructureSet->getStructure(j),
					            doseAccessor1->getGeometricInfo());
					spOTBMaskAccessor->updateMask();
					MaskAccessorPointer spMaskAccessor(spOTBMaskAccessor);
					
					//create corresponding MaskedDoseIterator
					boost::shared_ptr<core::GenericMaskedDoseIterator> spMaskedDoseIteratorTmp =
					    boost::make_shared<core::GenericMaskedDoseIterator>(spMaskAccessor, doseAccessor1);
					DoseIteratorPointer spMaskedDoseIterator(spMaskedDoseIteratorTmp);
					//store MaskAccessor for each structure (later reuse)
					rtStructSetMaskAccessorVec.push_back(spMaskAccessor);

					//calculate DVH
					rttb::core::DVHCalculator calc(spMaskedDoseIterator, (rtStructureSet->getStructure(j))->getUID(),
					                               doseAccessor1->getDoseUID());
					rttb::core::DVH dvh = *(calc.generateDVH());


					//DEBUG OUTPUT
					std::cout << "=== Dose 1 Structure "<<j<<"===" << std::endl;
					std::cout << std::setprecision (20) <<"max: "<< dvh.getMaximum()<<std::endl;
					std::cout << std::setprecision (20) <<"min: "<< dvh.getMinimum()<<std::endl;
					std::cout << std::setprecision (20) <<"mean: "<< dvh.getMean()<<std::endl;
					std::cout << std::setprecision (20) <<"median: "<< dvh.getMedian()<<std::endl;
					std::cout << std::setprecision (20) <<"modal: "<< dvh.getModal()<<std::endl;
					std::cout << std::setprecision (20) <<"std: "<< dvh.getStdDeviation()<<std::endl;
					std::cout << std::setprecision (20) <<"var: "<< dvh.getVariance()<<std::endl;
					std::cout << std::setprecision (20) <<"numV: "<< dvh.getNumberOfVoxels()<<std::endl;
					

					//compare explicit values for some results.
					//expected values were generated from the original implementation
					if (j == 0)
					{
						CHECK_CLOSE(5.6658345820895519e-005, dvh.getMaximum(), 1e-3);
						CHECK_CLOSE(5.6658345820895519e-005, dvh.getMinimum(), errorConstant);
						CHECK_CLOSE(5.6658345820895525e-005, dvh.getMean(), errorConstant);
						CHECK_CLOSE(5.6658345820895519e-005, dvh.getMedian(), errorConstant);
						CHECK_CLOSE(5.6658345820895519e-005, dvh.getModal(), errorConstant);
						CHECK_CLOSE(-4.1359030627651384e-025, dvh.getVariance(), errorConstant);
						CHECK_CLOSE(89230.858052685173, dvh.getNumberOfVoxels(), 2e-1);
					}

					if (j == 1)
					{
						CHECK_CLOSE(-1.2407709188295415e-024, dvh.getVariance(), errorConstant);
						CHECK_CLOSE(595.30645355716683, dvh.getNumberOfVoxels(), 1e-3);
					}

					if (j == 2)
					{
						CHECK_CLOSE(-4.1359030627651384e-025, dvh.getVariance(), errorConstant);
						CHECK_CLOSE(1269.9125811291087, dvh.getNumberOfVoxels(), 1e-3);
					}

					if (j == 9)
					{
						CHECK_CLOSE(5.6658345820895519e-005, dvh.getMaximum(), 1e-3);
						CHECK_CLOSE(5.6658345820895519e-005, dvh.getMinimum(), errorConstant);
						CHECK_CLOSE(5.6658345820895519e-005, dvh.getMean(), errorConstant);
						CHECK_CLOSE(5.6658345820895519e-005, dvh.getMedian(), errorConstant);
						CHECK_CLOSE(5.6658345820895519e-005, dvh.getModal(), errorConstant);
						CHECK_CLOSE(0, dvh.getStdDeviation(), errorConstant);
						CHECK_CLOSE(0, dvh.getVariance(), errorConstant);
						CHECK_CLOSE(1328.4918116279605, dvh.getNumberOfVoxels(), 1e-3);
					}

				}
			}

			clock_t finish(clock());

			std::cout << "DVH Calculation time: " << finish - start << " ms" << std::endl;

			clock_t start2(clock());

			for (int j = 0; j < rtStructSetMaskAccessorVec.size(); j++)
			{
				//create corresponding MaskedDoseIterator
				boost::shared_ptr<core::GenericMaskedDoseIterator> spMaskedDoseIteratorTmp =
				    boost::make_shared<core::GenericMaskedDoseIterator>(rtStructSetMaskAccessorVec.at(j),
				            doseAccessor2);
				DoseIteratorPointer spMaskedDoseIterator(spMaskedDoseIteratorTmp);

				//calculate DVH
				rttb::core::DVHCalculator calc(spMaskedDoseIterator, (rtStructureSet->getStructure(j))->getUID(),
				                               doseAccessor2->getDoseUID());
				rttb::core::DVH dvh = *(calc.generateDVH());


				/*//DEBUG OUTPUT
				std::cout << "=== Dose 2 Structure "<<j<<"===" << std::endl;
				std::cout << std::setprecision (20) <<"max: "<< dvh.getMaximum()<<std::endl;
				std::cout << std::setprecision (20) <<"min: "<< dvh.getMinimum()<<std::endl;
				std::cout << std::setprecision (20) <<"mean: "<< dvh.getMean()<<std::endl;
				std::cout << std::setprecision (20) <<"median: "<< dvh.getMedian()<<std::endl;
				std::cout << std::setprecision (20) <<"modal: "<< dvh.getModal()<<std::endl;
				std::cout << std::setprecision (20) <<"std: "<< dvh.getStdDeviation()<<std::endl;
				std::cout << std::setprecision (20) <<"var: "<< dvh.getVariance()<<std::endl;
				std::cout << std::setprecision (20) <<"numV: "<< dvh.getNumberOfVoxels()<<std::endl;
				*/

				if (j == 0)
				{
					CHECK_CLOSE(1.8423272053074631, dvh.getMaximum(), 1e-1);
					CHECK_CLOSE(0.0069001018925373145, dvh.getMinimum(), errorConstant);
					CHECK_CLOSE(0.5534586388640208, dvh.getMean(), errorConstant);
					CHECK_CLOSE(0.42090621544477619, dvh.getMedian(), errorConstant);
					CHECK_CLOSE(0.075901120817910464, dvh.getModal(), errorConstant);
					CHECK_CLOSE(0.44688344565881616, dvh.getStdDeviation(), 1e-4);
					CHECK_CLOSE(0.19970481400389611, dvh.getVariance(), 1e-4);
					CHECK_CLOSE(89230.858052685187, dvh.getNumberOfVoxels(), 1e-1);
				}

				if (j == 4)
				{
					CHECK_CLOSE(1.6264736515373135, dvh.getMaximum(), 1e-3);
					CHECK_CLOSE(0.10433981915522389, dvh.getMinimum(), errorConstant);
					CHECK_CLOSE(0.70820073085773427, dvh.getMean(), errorConstant);
					CHECK_CLOSE(0.71810346124477609, dvh.getMedian(), errorConstant);
					CHECK_CLOSE(0.23936782041492538, dvh.getModal(), errorConstant);
					CHECK_CLOSE(0.36355099006842068, dvh.getStdDeviation(), errorConstant);
					CHECK_CLOSE(0.13216932237972889, dvh.getVariance(), errorConstant);
					CHECK_CLOSE(2299.7105030728999, dvh.getNumberOfVoxels(), 1e-3);
				}
			}

			clock_t finish2(clock());

			std::cout << "Reset dose 2, DVH Calculation time: " << finish2 - start2 << " ms" << std::endl;

			clock_t start3(clock());

			for (int j = 0; j < rtStructSetMaskAccessorVec.size(); j++)
			{
				//create corresponding MaskedDoseIterator
				boost::shared_ptr<core::GenericMaskedDoseIterator> spMaskedDoseIteratorTmp =
				    boost::make_shared<core::GenericMaskedDoseIterator>(rtStructSetMaskAccessorVec.at(j),
				            doseAccessor3);
				DoseIteratorPointer spMaskedDoseIterator(spMaskedDoseIteratorTmp);

				//calculate DVH
				rttb::core::DVHCalculator calc(spMaskedDoseIterator, (rtStructureSet->getStructure(j))->getUID(),
				                               doseAccessor3->getDoseUID());
				rttb::core::DVH dvh = *(calc.generateDVH());


				/*//DEBUG OUTPUT
				std::cout << "=== Dose 3 Structure "<<j<<"===" << std::endl;
				std::cout << std::setprecision (20) <<"max: "<< dvh.getMaximum()<<std::endl;
				std::cout << std::setprecision (20) <<"min: "<< dvh.getMinimum()<<std::endl;
				std::cout << std::setprecision (20) <<"mean: "<< dvh.getMean()<<std::endl;
				std::cout << std::setprecision (20) <<"median: "<< dvh.getMedian()<<std::endl;
				std::cout << std::setprecision (20) <<"modal: "<< dvh.getModal()<<std::endl;
				std::cout << std::setprecision (20) <<"std: "<< dvh.getStdDeviation()<<std::endl;
				std::cout << std::setprecision (20) <<"var: "<< dvh.getVariance()<<std::endl;
				std::cout << std::setprecision (20) <<"numV: "<< dvh.getNumberOfVoxels()<<std::endl;
				*/

				if (j == 1)
				{
					CHECK_CLOSE(0.0010765085705970151, dvh.getMaximum(), 1e-3);
					CHECK_CLOSE(0.00087641404074626872, dvh.getMinimum(), errorConstant);
					CHECK_CLOSE(0.0009788401527774486, dvh.getMean(), errorConstant);
					CHECK_CLOSE(0.00098846697746268666, dvh.getMedian(), errorConstant);
					CHECK_CLOSE(0.00098846697746268666, dvh.getModal(), errorConstant);
					CHECK_CLOSE(3.2977969280849566e-005, dvh.getStdDeviation(), errorConstant);
					CHECK_CLOSE(1.0875464578886574e-009, dvh.getVariance(), errorConstant);
					CHECK_CLOSE(595.30645355716683, dvh.getNumberOfVoxels(), 1e-3);
				}

				if (j == 6)
				{
					CHECK_CLOSE(0.0016589942782835824, dvh.getMaximum(), 1e-3);
					CHECK_CLOSE(0.00027960577723880602, dvh.getMinimum(), errorConstant);
					CHECK_CLOSE(0.0010389077643351956, dvh.getMean(), errorConstant);
					CHECK_CLOSE(0.0011246365706716419, dvh.getMedian(), errorConstant);
					CHECK_CLOSE(0.0013856019627611941, dvh.getModal(), errorConstant);
					CHECK_CLOSE(0.00036431958148461669, dvh.getStdDeviation(), errorConstant);
					CHECK_CLOSE(1.3272875745312625e-007, dvh.getVariance(), errorConstant);
					CHECK_CLOSE(8034.8878045085003, dvh.getNumberOfVoxels(), 1e-2);
				}
			}

			clock_t finish3(clock());

			std::cout << "Reset dose 3, DVH Calculation time: " << finish3 - start3 << " ms" << std::endl;



			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb

