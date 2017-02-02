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
#include "rttbBoostMaskAccessor.h"
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
			typedef masks::boost::BoostMaskAccessor::StructTypePointer StructTypePointer;
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

			if (argc > 4)
			{
				Virtuos_Dose_File = argv[1];
				Virtuos_Structure_File = argv[2];
				Virtuos_Plan_File = argv[3];
				Virtuos_CT_File = argv[4];
			}


			//Virtuos DVH Test

			io::virtuos::VirtuosPlanFileDoseAccessorGenerator doseAccessorGeneratorVirtuos(Virtuos_Dose_File,
			        Virtuos_Plan_File);
			DoseAccessorPointer doseAccessorVirtuos(doseAccessorGeneratorVirtuos.generateDoseAccessor());

			StructureSetPointer rtStructureSetVirtuos = io::virtuos::VirtuosFileStructureSetGenerator(
			            Virtuos_Structure_File, Virtuos_CT_File).generateStructureSet();


			//create MaskAccessor for structure DARM
			boost::shared_ptr<masks::boost::BoostMaskAccessor> spMaskAccessorVirtuos =
			    boost::make_shared<masks::boost::BoostMaskAccessor>(rtStructureSetVirtuos->getStructure(4),
			            doseAccessorVirtuos->getGeometricInfo());

			spMaskAccessorVirtuos->updateMask();

			MaskAccessorPointer spMaskAccessor(spMaskAccessorVirtuos);

			//create corresponding MaskedDoseIterator

			boost::shared_ptr<core::GenericMaskedDoseIterator> spMaskedDoseIteratorTmp =
			    boost::make_shared<core::GenericMaskedDoseIterator>(spMaskAccessor, doseAccessorVirtuos);

			DoseIteratorPointer spMaskedDoseIterator(spMaskedDoseIteratorTmp);

			rttb::core::DVHCalculator* calc;
			CHECK_NO_THROW(calc = new rttb::core::DVHCalculator(spMaskedDoseIterator,
			        (rtStructureSetVirtuos->getStructure(4))->getUID(), doseAccessorVirtuos->getUID()));

			DVHPointer dvhPtr;
			CHECK_NO_THROW(dvhPtr = calc->generateDVH());

			CHECK_CLOSE(4.08178, dvhPtr->getMaximum(), errorConstant);
			CHECK_CLOSE(0.0151739, dvhPtr->getMinimum(), errorConstant);
            CHECK_CLOSE(0.755357, dvhPtr->getMean(), errorConstant);
			CHECK_CLOSE(0.440044, dvhPtr->getMedian(), errorConstant);
			CHECK_CLOSE(0.0151739, dvhPtr->getModal(), errorConstant);
			CHECK_CLOSE(0.835792, dvhPtr->getStdDeviation(), errorConstant);
			CHECK_CLOSE(0.698549, dvhPtr->getVariance(), errorConstant);
            //reduced error constant because values differ with boost versions
            CHECK_CLOSE(46572.03509, dvhPtr->getNumberOfVoxels(), 0.01);

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb

