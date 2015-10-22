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

#include "litCheckMacros.h"

#include "rttbDoseStatistics.h"
#include "rttbDoseStatisticsCalculator.h"
#include "rttbDicomDoseAccessor.h"
#include "rttbDicomFileDoseAccessorGenerator.h"
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
		PREPARE_DEFAULT_TEST_REPORTING;

		/*! @brief RTDoseStatisticsTest. Max, min, mean, standard deviation, variance, Vx, Dx, MOHx, MOCx, MaxOHx,
		MinOCx are tested. Test if calculation in new architecture returns similar results to the original implementation.

		WARNING: The values for comparison need to be adjusted if the input files are changed!*/

		const double reducedErrorConstant = 0.0001;
		const double expectedVal = 5.64477e-005;
		const double volume = 24120.;

		void testWithDummyDoseData(const std::string& doseFilename);
		void testWithRealVirtuosDoseDataAndStructure(const std::string& doseFilename, const std::string& structFilename,
		        const std::string& planFilename, unsigned int structureNr);

		int RTDoseStatisticsTest(int argc, char* argv[])
		{

			std::string RTDOSE_FILENAME;
			std::string RTDOSE2_FILENAME;
			std::string RTSTRUCT_FILENAME;
			std::string RTPLAN_FILENAME;

			if (argc > 4)
			{
				RTDOSE_FILENAME = argv[1];
				RTDOSE2_FILENAME = argv[2];
				RTSTRUCT_FILENAME = argv[3];
				RTPLAN_FILENAME = argv[4];
			}
			else
			{
				std::cout << "at least four arguments required for RTDoseStatisticsTest" << std::endl;
				return -1;
			}

			testWithDummyDoseData(RTDOSE_FILENAME);
			//Structure 2 is RUECKENMARK
			testWithRealVirtuosDoseDataAndStructure(RTDOSE2_FILENAME, RTSTRUCT_FILENAME, RTPLAN_FILENAME, 2);

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

		void testWithDummyDoseData(const std::string& doseFilename)
		{
			typedef core::GenericDoseIterator::DoseAccessorPointer DoseAccessorPointer;
			typedef core::GenericDoseIterator::DoseIteratorPointer DoseIteratorPointer;

			io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator1(doseFilename.c_str());
			DoseAccessorPointer doseAccessor1(doseAccessorGenerator1.generateDoseAccessor());

			//create corresponding DoseIterator
			::boost::shared_ptr<core::GenericDoseIterator> spDoseIteratorTmp =
			    ::boost::make_shared<core::GenericDoseIterator>(doseAccessor1);
			DoseIteratorPointer spDoseIterator(spDoseIteratorTmp);
			rttb::algorithms::DoseStatisticsCalculator doseStatisticsCalculator(spDoseIterator);

			std::vector<double> precomputedDoseValues;
			precomputedDoseValues.push_back(0);
			precomputedDoseValues.push_back(0.5);
			precomputedDoseValues.push_back(1);
			std::vector<double> precomputedVolumeValues;
			precomputedVolumeValues.push_back(20000/volume);
			precomputedVolumeValues.push_back(1);

			rttb::algorithms::DoseStatistics::DoseStatisticsPointer doseStatistics =
			    doseStatisticsCalculator.calculateDoseStatistics(precomputedDoseValues, precomputedVolumeValues);

			CHECK_CLOSE(doseStatistics->getMean(), expectedVal, errorConstant);
			CHECK_CLOSE(doseStatistics->getStdDeviation(), 0, errorConstant);
			CHECK_CLOSE(doseStatistics->getVariance(), 0, errorConstant);

			double dummy;
			DoseTypeGy vx = doseStatistics->getVx(expectedVal,true,dummy);
			CHECK_EQUAL(vx, doseStatistics->getVx(0));
			CHECK_CLOSE(expectedVal, doseStatistics->getDx(vx), reducedErrorConstant);

			CHECK_CLOSE(doseStatistics->getMaximum(), expectedVal, errorConstant);

			CHECK_CLOSE(doseStatistics->getMinimum(), expectedVal, errorConstant);
			auto minListPtr = doseStatistics->getMinimumPositions();
			auto maxListPtr = doseStatistics->getMaximumPositions();
			CHECK_EQUAL(maxListPtr->size(), 10);
			CHECK_EQUAL(minListPtr->size(), 10);

			CHECK_CLOSE(doseStatistics->getDx(24120), doseStatistics->getMinimum(), 0.001);
			CHECK_CLOSE(doseStatistics->getMOHx(24120), doseStatistics->getMean(), reducedErrorConstant);
			CHECK_CLOSE(doseStatistics->getMOCx(20000), doseStatistics->getMean(), reducedErrorConstant);
			CHECK_CLOSE(doseStatistics->getMinOCx(20000), doseStatistics->getMean(), reducedErrorConstant);
		}

		void testWithRealVirtuosDoseDataAndStructure(const std::string& doseFilename, const std::string& structFilename,
		        const std::string& planFilename, unsigned int structureNr)
		{
			typedef core::GenericDoseIterator::DoseIteratorPointer DoseIteratorPointer;
			typedef core::GenericMaskedDoseIterator::MaskAccessorPointer MaskAccessorPointer;
			typedef rttb::algorithms::DoseStatistics::DoseStatisticsPointer DoseStatisticsPointer;

			auto virtuosDoseAccessor = io::virtuos::VirtuosPlanFileDoseAccessorGenerator(doseFilename.c_str(),
			                           planFilename.c_str()).generateDoseAccessor();
			auto virtuosStructureSet = io::virtuos::VirtuosFileStructureSetGenerator(
			                               structFilename.c_str(), doseFilename.c_str()).generateStructureSet();

			boost::shared_ptr<masks::boost::BoostMaskAccessor> spOTBMaskAccessorVirtuos =
			    boost::make_shared<masks::boost::BoostMaskAccessor>(virtuosStructureSet->getStructure(structureNr),
			            virtuosDoseAccessor->getGeometricInfo());

			spOTBMaskAccessorVirtuos->updateMask();

			MaskAccessorPointer spMaskAccessor(spOTBMaskAccessorVirtuos);

			//create corresponding MaskedDoseIterator

			boost::shared_ptr<core::GenericMaskedDoseIterator> spMaskedDoseIteratorTmp =
			    boost::make_shared<core::GenericMaskedDoseIterator>(spMaskAccessor, virtuosDoseAccessor);

			DoseIteratorPointer spMaskedDoseIterator(spMaskedDoseIteratorTmp);

			rttb::algorithms::DoseStatisticsCalculator doseStatisticsCalculatorVirtuos(spMaskedDoseIterator);


			DoseStatisticsPointer doseStatisticsVirtuos = doseStatisticsCalculatorVirtuos.calculateDoseStatistics(true);

			//comparison values computed with "old" DoseStatistics implementation
			CHECK_CLOSE(doseStatisticsVirtuos->getMinimum(), 6.4089, reducedErrorConstant);
			CHECK_CLOSE(doseStatisticsVirtuos->getMaximum(), 39.0734, reducedErrorConstant);
			CHECK_CLOSE(doseStatisticsVirtuos->getMean(), 22.5779, reducedErrorConstant);
			CHECK_CLOSE(doseStatisticsVirtuos->getStdDeviation(), 6.28857, reducedErrorConstant);

			auto maxPositions = doseStatisticsVirtuos->getMaximumPositions();
			auto minPositions = doseStatisticsVirtuos->getMinimumPositions();
			CHECK_EQUAL(maxPositions->size(), 1);
			CHECK_EQUAL(minPositions->size(), 1);
			CHECK_EQUAL(maxPositions->begin()->first, doseStatisticsVirtuos->getMaximum());
			CHECK_EQUAL(maxPositions->begin()->second, 3570772);
			CHECK_EQUAL(minPositions->begin()->first, doseStatisticsVirtuos->getMinimum());
			CHECK_EQUAL(minPositions->begin()->second, 3571264);

			CHECK_CLOSE(doseStatisticsVirtuos->getDx(0.02 * doseStatisticsVirtuos->getVolume()), 31.8358, reducedErrorConstant);
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
		}

	}//testing
}//rttb