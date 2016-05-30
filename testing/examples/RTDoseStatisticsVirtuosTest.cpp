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
// @version $Revision: 1328 $ (last changed revision)
// @date $Date: 2016-04-22 09:50:01 +0200 (Fr, 22 Apr 2016) $ (last change date)
// @author $Author: hentsch $ (last changed by)
*/

// this file defines the rttbCoreTests for the test driver
// and all it expects is that you have a function called RegisterTests

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include "litCheckMacros.h"

#include "rttbDoseStatistics.h"
#include "rttbDoseStatisticsCalculator.h"
#include "rttbVirtuosPlanFileDoseAccessorGenerator.h"
#include "rttbVirtuosFileStructureSetGenerator.h"
#include "rttbBoostMaskAccessor.h"
#include "rttbGenericMaskedDoseIterator.h"
#include "rttbGenericDoseIterator.h"
#include "rttbBaseType.h"

namespace rttb
{
	namespace testing
	{


		/*! @brief RTDoseStatisticsTest. Max, min, mean, standard deviation, variance, Vx, Dx, MOHx, MOCx, MaxOHx,
		MinOCx are tested. Test if calculation in new architecture returns similar results to the original implementation.

		WARNING: The values for comparison need to be adjusted if the input files are changed!*/


		int RTDoseStatisticsVirtuosTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;
			std::string RTDOSE_FILENAME;
			std::string RTSTRUCT_FILENAME;
			std::string RTPLAN_FILENAME;

			if (argc > 3)
			{
				RTDOSE_FILENAME = argv[1];
				RTSTRUCT_FILENAME = argv[2];
				RTPLAN_FILENAME = argv[3];
			}
			else
			{
				std::cout << "at least three arguments required for RTDoseStatisticsVirtuosTest" << std::endl;
				return -1;
			}

			//Structure 2 is RUECKENMARK
			typedef core::GenericDoseIterator::DoseIteratorPointer DoseIteratorPointer;
			typedef core::GenericMaskedDoseIterator::MaskAccessorPointer MaskAccessorPointer;
			typedef rttb::algorithms::DoseStatistics::DoseStatisticsPointer DoseStatisticsPointer;
			typedef core::DoseIteratorInterface::DoseAccessorPointer DoseAccessorPointer;
			typedef core::StructureSetGeneratorInterface::StructureSetPointer StructureSetPointer;
			typedef algorithms::DoseStatisticsCalculator::ResultListPointer ResultListPointer;

			DoseAccessorPointer virtuosDoseAccessor = io::virtuos::VirtuosPlanFileDoseAccessorGenerator(
			            RTDOSE_FILENAME.c_str(),
			            RTPLAN_FILENAME.c_str()).generateDoseAccessor();
			StructureSetPointer virtuosStructureSet = io::virtuos::VirtuosFileStructureSetGenerator(
			            RTSTRUCT_FILENAME.c_str(), RTDOSE_FILENAME.c_str()).generateStructureSet();

			boost::shared_ptr<masks::boost::BoostMaskAccessor> spOTBMaskAccessorVirtuos =
			    boost::make_shared<masks::boost::BoostMaskAccessor>(virtuosStructureSet->getStructure(2),
			            virtuosDoseAccessor->getGeometricInfo());

			spOTBMaskAccessorVirtuos->updateMask();

			MaskAccessorPointer spMaskAccessor(spOTBMaskAccessorVirtuos);

			//create corresponding MaskedDoseIterator

			boost::shared_ptr<core::GenericMaskedDoseIterator> spMaskedDoseIteratorTmp =
			    boost::make_shared<core::GenericMaskedDoseIterator>(spMaskAccessor, virtuosDoseAccessor);

			DoseIteratorPointer spMaskedDoseIterator(spMaskedDoseIteratorTmp);

			rttb::algorithms::DoseStatisticsCalculator doseStatisticsCalculatorVirtuos(spMaskedDoseIterator);


			DoseStatisticsPointer doseStatisticsVirtuos =
			    doseStatisticsCalculatorVirtuos.calculateDoseStatistics(true);

			//comparison values computed with "old" DoseStatistics implementation
			CHECK_CLOSE(doseStatisticsVirtuos->getMinimum(), 6.4089, reducedErrorConstant);
			CHECK_CLOSE(doseStatisticsVirtuos->getMaximum(), 39.0734, reducedErrorConstant);
			CHECK_CLOSE(doseStatisticsVirtuos->getMean(), 22.5779, reducedErrorConstant);
			CHECK_CLOSE(doseStatisticsVirtuos->getStdDeviation(), 6.28857, reducedErrorConstant);

			ResultListPointer maxPositions = doseStatisticsVirtuos->getMaximumVoxelPositions();
			ResultListPointer minPositions = doseStatisticsVirtuos->getMinimumVoxelPositions();
			CHECK_EQUAL(maxPositions->size(), 1);
			CHECK_EQUAL(minPositions->size(), 1);
			CHECK_EQUAL(maxPositions->begin()->first, doseStatisticsVirtuos->getMaximum());
			CHECK_EQUAL(maxPositions->begin()->second, 3570772);
			CHECK_EQUAL(minPositions->begin()->first, doseStatisticsVirtuos->getMinimum());
			CHECK_EQUAL(minPositions->begin()->second, 3571264);

			CHECK_CLOSE(doseStatisticsVirtuos->getDx(0.02 * doseStatisticsVirtuos->getVolume()), 31.8358,
			            reducedErrorConstant);
			CHECK_CLOSE(doseStatisticsVirtuos->getVx(0.9 * doseStatisticsVirtuos->getMaximum()), 0.471747,
			            reducedErrorConstant);
			CHECK_CLOSE(doseStatisticsVirtuos->getMOHx(0.1 * doseStatisticsVirtuos->getVolume()), 31.3266,
			            reducedErrorConstant);
			CHECK_CLOSE(doseStatisticsVirtuos->getMOCx(0.05 * doseStatisticsVirtuos->getVolume()), 9.01261,
			            reducedErrorConstant);
			CHECK_CLOSE(doseStatisticsVirtuos->getMaxOHx(0.95 * doseStatisticsVirtuos->getVolume()), 10.3764,
			            reducedErrorConstant);
			CHECK_CLOSE(doseStatisticsVirtuos->getMinOCx(0.98 * doseStatisticsVirtuos->getVolume()), 31.8373,
			            reducedErrorConstant);

			RETURN_AND_REPORT_TEST_SUCCESS;

		}

	}//testing
}//rttb
