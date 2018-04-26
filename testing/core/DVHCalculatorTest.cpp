// -----------------------------------------------------------------------
// RTToolbox - DKFZ radiotherapy quantitative evaluation library
//
// Copyright (c) German Cancer Research Center (DKFZ),
// Software development for Integrated Diagnostics and Therapy (SIDT).
// ALL RIGHTS RESERVED.
// See rttbCopyright.txt or
// http://www.dkfz.de/en/sidt/projects/rttb/copyright.html
//
// This software is distributed WITHOUT ANY WARRANTY; without even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE.  See the above copyright notices for more information.
//
//------------------------------------------------------------------------
/*!
// @file
// @version $Revision$ (last changed revision)
// @date    $Date$ (last change date)
// @author  $Author$ (last changed by)
*/

// this file defines the rttbCoreTests for the test driver
// and all it expects is that you have a function called RegisterTests

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include "litCheckMacros.h"

#include "rttbBaseType.h"
#include "rttbDVHCalculator.h"
#include "rttbGenericMaskedDoseIterator.h"
#include "rttbGeometricInfo.h"
#include "rttbGenericDoseIterator.h"
#include "rttbNullPointerException.h"
#include "rttbInvalidParameterException.h"
#include "DummyDoseAccessor.h"
#include "DummyMaskAccessor.h"

namespace rttb
{

	namespace testing
	{
		typedef core::GenericDoseIterator::DoseAccessorPointer DoseAccessorPointer;
		typedef core::GenericMaskedDoseIterator::MaskAccessorPointer MaskAccessorPointer;
		typedef core::DVHCalculator::DoseIteratorPointer DoseIteratorPointer;
		typedef core::DVHCalculator::MaskedDoseIteratorPointer MaskedDoseIteratorPointer;


		/*!@brief DVHTest - test the API of DVH
		 1) test constructors (values as expected?)
		*/

		int DVHCalculatorTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			//create null pointer to DoseIterator
			DoseIteratorPointer spDoseIteratorNull;

			const IDType structureID = "myStructure";
			const IDType doseIDNull = "myDose";


			//1) test constructors (values as expected?)
			CHECK_THROW_EXPLICIT(core::DVHCalculator myDVHCalc(spDoseIteratorNull, structureID, doseIDNull),
			                     core::NullPointerException);


			//create dummy DoseAccessor
			boost::shared_ptr<DummyDoseAccessor> spTestDoseAccessor = boost::make_shared<DummyDoseAccessor>();
			DoseAccessorPointer spDoseAccessor(spTestDoseAccessor);
			const std::vector<DoseTypeGy>* doseVals = spTestDoseAccessor->getDoseVector();
			//create corresponding DoseIterator
			boost::shared_ptr<core::GenericDoseIterator> spTestDoseIterator =
			    boost::make_shared<core::GenericDoseIterator>(spDoseAccessor);
			DoseIteratorPointer spDoseIterator(spTestDoseIterator);

			const IDType doseID = spDoseAccessor->getUID();

			CHECK_NO_THROW(core::DVHCalculator myDVHCalc(spDoseIterator, structureID, doseID));
			CHECK_THROW_EXPLICIT(core::DVHCalculator myDVHCalc(spDoseIterator, structureID, doseID, -1),
			                     core::InvalidParameterException);

			int numBins = 21;
			DoseTypeGy binSize = 0;
			DoseStatisticType maximum = 0;
			std::vector<DoseTypeGy>::const_iterator doseIt = doseVals->begin();

			while (doseIt != doseVals->end())
			{
				if (maximum < *doseIt)
				{
					maximum = *doseIt;
				}

				++doseIt;
			}

			binSize = maximum * 1.5 / numBins;

			CHECK_NO_THROW(core::DVHCalculator myDVHCalc(spDoseIterator, structureID, doseID, binSize,
			               numBins));
			CHECK_THROW_EXPLICIT(core::DVHCalculator myDVHCalc(spDoseIterator, structureID, doseID, 0, 0),
			                     core::InvalidParameterException);//aNumberOfBins must be >=0
			core::DVHCalculator myDVHCalc(spDoseIterator, structureID, doseID, binSize, 1);
			CHECK_THROW_EXPLICIT(myDVHCalc.generateDVH(),
			                     core::InvalidParameterException);//_numberOfBins must be > max(aDoseIterator)/aDeltaD!

      //generateDVH (only test basic functionality here and test it in RTTBDicomIOTests and RTTBITKIOTests)

       //create dummy DoseAccessor with values 0 to check if _deltaD is set to 0.1
      std::vector<DoseTypeGy> zeros(1000, 0);
      core::GeometricInfo geoInfo;
      geoInfo.setNumColumns(10);
      geoInfo.setNumRows(10);
      geoInfo.setNumSlices(10);
      geoInfo.setSpacing({1.0, 1.0, 1.0});

      boost::shared_ptr<DummyDoseAccessor> spTestDoseAccessorZeros = boost::make_shared<DummyDoseAccessor>(zeros, geoInfo);
      DoseAccessorPointer spDoseAccessorZeros(spTestDoseAccessorZeros);
      boost::shared_ptr<core::GenericDoseIterator> spTestDoseIteratorZeros =
        boost::make_shared<core::GenericDoseIterator>(spDoseAccessorZeros);
      DoseIteratorPointer spDoseIteratorZeros(spTestDoseIteratorZeros);
      CHECK_NO_THROW(core::DVHCalculator myDVHCalc2(spDoseIteratorZeros, structureID, doseID, 0,
        numBins));
      core::DVHCalculator myDVHCalc2(spDoseIteratorZeros, structureID, doseID, 0,
        numBins);
      core::DVH::Pointer dvh;
      CHECK_NO_THROW(dvh = myDVHCalc2.generateDVH());
      CHECK(dvh);
      CHECK_CLOSE(dvh->getDeltaD(), 0.1, errorConstant);

			//create dummy MaskAccessor
			boost::shared_ptr<DummyMaskAccessor> spTestMaskAccessor =
			    boost::make_shared<DummyMaskAccessor>(spDoseAccessor->getGeometricInfo());
			MaskAccessorPointer spMaskAccessor(spTestMaskAccessor);
			//create corresponding MaskedDoseIterator
			boost::shared_ptr<core::GenericMaskedDoseIterator> spTestMaskedDoseIterator =
			    boost::make_shared<core::GenericMaskedDoseIterator>(spMaskAccessor, spDoseAccessor);
			MaskedDoseIteratorPointer spMaskedDoseIterator(spTestMaskedDoseIterator);

			CHECK_NO_THROW(core::DVHCalculator myDVHCalc3(spMaskedDoseIterator, structureID, doseID));
      core::DVHCalculator myDVHCalc3(spMaskedDoseIterator, structureID, doseID);

      CHECK_NO_THROW(dvh = myDVHCalc3.generateDVH());
      CHECK(dvh);

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//end namespace testing
}//end namespace rttb
