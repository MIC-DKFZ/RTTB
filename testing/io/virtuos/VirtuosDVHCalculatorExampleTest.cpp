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
#include "rttbOTBMaskAccessor.h"
#include "rttbVirtuosPlanFileDoseAccessorGenerator.h"
#include "rttbVirtuosDoseAccessor.h"
#include "rttbVirtuosFileStructureSetGenerator.h"


namespace rttb
{

	namespace testing
	{

		/*! @brief VirtuosDVHCalculatorExampleTest.
		Test if calculation in new architecture returns similar results to the
		original implementation.

		WARNING: The values for comparison need to be adjusted if the input files are changed!
		*/

		int VirtuosDVHCalculatorExampleTest(int argc, char* argv[])
		{
			typedef core::GenericDoseIterator::DoseAccessorPointer DoseAccessorPointer;
			typedef core::GenericMaskedDoseIterator::MaskAccessorPointer MaskAccessorPointer;
			typedef core::DVHCalculator::DoseIteratorPointer DoseIteratorPointer;
			typedef core::DVHCalculator::MaskedDoseIteratorPointer MaskedDoseIteratorPointer;
			typedef masks::OTBMaskAccessor::StructTypePointer StructTypePointer;
			typedef core::DVH::DVHPointer DVHPointer;
			typedef core::StructureSetGeneratorInterface::StructureSetPointer StructureSetPointer;

			PREPARE_DEFAULT_TEST_REPORTING;
			//ARGUMENTS: 1: virtuos dose file name
			//           2: virtuos structure file name
			//           3: virtuos plan file name
			//           4: virtuos CT file name

			std::string Virtuos_Dose_File;
			std::string Virtuos_Structure_File;
			std::string Virtuos_Plan_File;
			std::string Virtuos_CT_File;

			if (argc > 1)
			{
				Virtuos_Dose_File = argv[1];
			}

			if (argc > 2)
			{
				Virtuos_Structure_File = argv[2];
			}

			if (argc > 3)
			{
				Virtuos_Plan_File = argv[3];
			}

			if (argc > 4)
			{
				Virtuos_CT_File = argv[4];
			}


			//Virtuos DVH Test

			io::virtuos::VirtuosPlanFileDoseAccessorGenerator doseAccessorGeneratorVirtuos(Virtuos_Dose_File,
			        Virtuos_Plan_File);
			DoseAccessorPointer doseAccessorVirtuos(doseAccessorGeneratorVirtuos.generateDoseAccessor());

			StructureSetPointer rtStructureSetVirtuos = io::virtuos::VirtuosFileStructureSetGenerator(
			            Virtuos_Structure_File, Virtuos_CT_File).generateStructureSet();

			for (int i = 1; i < rtStructureSetVirtuos->getNumberOfStructures(); i++)
			{
				//if(i=6 || i==8){//todo: assertion bug!
				if (i == 8)
				{

					//create MaskAccessor for each structure
					boost::shared_ptr<masks::OTBMaskAccessor> spOTBMaskAccessorVirtuos =
					    boost::make_shared<masks::OTBMaskAccessor>(rtStructureSetVirtuos->getStructure(i),
					            doseAccessorVirtuos->getGeometricInfo());

					spOTBMaskAccessorVirtuos->updateMask();

					MaskAccessorPointer spMaskAccessor(spOTBMaskAccessorVirtuos);

					//create corresponding MaskedDoseIterator

					boost::shared_ptr<core::GenericMaskedDoseIterator> spMaskedDoseIteratorTmp =
					    boost::make_shared<core::GenericMaskedDoseIterator>(spMaskAccessor, doseAccessorVirtuos);

					DoseIteratorPointer spMaskedDoseIterator(spMaskedDoseIteratorTmp);

					std::cout << "dvh calc" << std::endl;
					rttb::core::DVHCalculator* calc;

					if (i == 6)
					{
						calc = new rttb::core::DVHCalculator(spMaskedDoseIterator,
						                                     (rtStructureSetVirtuos->getStructure(i))->getUID(), doseAccessorVirtuos->getDoseUID(), 0.367944);
					}
					else
					{
						calc = new rttb::core::DVHCalculator(spMaskedDoseIterator,
						                                     (rtStructureSetVirtuos->getStructure(i))->getUID(), doseAccessorVirtuos->getDoseUID(), 0.510107);
					}


					DVHPointer dvhPtr = calc->generateDVH();

					if (i == 6)
					{
						CHECK_CLOSE(42.497532, dvhPtr->getMaximum(), errorConstant);
						CHECK_CLOSE(2.7595800000000001, dvhPtr->getMinimum(), errorConstant);
						CHECK_CLOSE(7.4752642058590695, dvhPtr->getMean(), errorConstant);
						CHECK_CLOSE(6.4390200000000002, dvhPtr->getMedian(), errorConstant);
						CHECK_CLOSE(5.7031320000000001, dvhPtr->getModal(), errorConstant);
						CHECK_CLOSE(3.5065188466477824, dvhPtr->getStdDeviation(), errorConstant);
						CHECK_CLOSE(12.295674421896095, dvhPtr->getVariance(), errorConstant);
						CHECK_CLOSE(177218.04900734947, dvhPtr->getNumberOfVoxels(), 1e-2);
					}

					if (i == 8)
					{
						CHECK_CLOSE(68.099284499999996, dvhPtr->getMaximum(), errorConstant);
						CHECK_CLOSE(24.7401895, dvhPtr->getMinimum(), errorConstant);
						CHECK_CLOSE(54.384709827101481, dvhPtr->getMean(), errorConstant);
						CHECK_CLOSE(54.836502499999995, dvhPtr->getMedian(), errorConstant);
						CHECK_CLOSE(54.836502499999995, dvhPtr->getModal(), errorConstant);
						CHECK_CLOSE(3.3345924130915088, dvhPtr->getStdDeviation(), errorConstant);
						CHECK_CLOSE(11.119506561447452, dvhPtr->getVariance(), errorConstant);
						CHECK_CLOSE(338856.04793872859, dvhPtr->getNumberOfVoxels(), 1e-2);
					}

				}

			}

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb

