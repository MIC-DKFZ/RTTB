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

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include "litCheckMacros.h"

#include "rttbBaseType.h"
#include "rttbGenericDoseIterator.h"
#include "rttbDoseIteratorInterface.h"
#include "rttbNullPointerException.h"
#include "rttbDoseStatistics.h"
#include "rttbInvalidDoseException.h"

#include "../core/DummyDoseAccessor.h"

namespace rttb{
	namespace testing{

		typedef core::GenericDoseIterator::DoseAccessorPointer DoseAccessorPointer;
		typedef core::DoseIteratorInterface::DoseIteratorPointer DoseIteratorPointer;
		typedef rttb::algorithms::DoseStatistics::ResultListPointer ResultListPointer;

		/*! @brief DoseStatisticsTest - test the API of DoseStatistics
			1) test constructors 
			2) test setDoseIterator
			3) test getNumberOfVoxels
			4) get statistical values
			floating point accuracy requires this otherwise the test 4 fails!
		*/

		int DoseStatisticsTest(int argc, char* argv[] )
			{
			PREPARE_DEFAULT_TEST_REPORTING;

			boost::shared_ptr<DummyDoseAccessor> spTestDoseAccessor = boost::make_shared<DummyDoseAccessor>();
			DoseAccessorPointer spDoseAccessor(spTestDoseAccessor);
			const std::vector<DoseTypeGy>* doseVals = spTestDoseAccessor->getDoseVector();

			boost::shared_ptr<core::GenericDoseIterator> spTestDoseIterator = 
				boost::make_shared<core::GenericDoseIterator>(spDoseAccessor);
			DoseIteratorPointer spDoseIterator (spTestDoseIterator);

			DoseIteratorPointer spDoseIteratorNull;

			//1) test constructors 
			// the values cannot be accessed from outside, therefore correct default values are not tested
			CHECK_NO_THROW(rttb::algorithms::DoseStatistics myEmptyDoseStat);
			rttb::algorithms::DoseStatistics myEmptyDoseStat;
			rttb::algorithms::DoseStatistics myDoseStatToBeFilled;

			CHECK_THROW_EXPLICIT(rttb::algorithms::DoseStatistics myDoseStats(spDoseIteratorNull),core::NullPointerException);

			CHECK_NO_THROW(rttb::algorithms::DoseStatistics myDoseStats(spDoseIterator));
			rttb::algorithms::DoseStatistics myDoseStats(spDoseIterator);

			//2) test setDoseIterator
			CHECK_THROW_EXPLICIT(myDoseStatToBeFilled.setDoseIterator(spDoseIteratorNull),core::NullPointerException);
			CHECK_NO_THROW(myDoseStatToBeFilled.setDoseIterator(spDoseIterator));

			//3) test getNumberOfVoxels
			CHECK_THROW_EXPLICIT(myEmptyDoseStat.getNumberOfVoxels(),core::InvalidDoseException);
			CHECK_EQUAL(myDoseStats.getNumberOfVoxels(),doseVals->size());
			CHECK_EQUAL(myDoseStatToBeFilled.getNumberOfVoxels(),doseVals->size());	  

			//4) get statistical values
      CHECK_THROW_EXPLICIT(myEmptyDoseStat.getNumberOfVoxels(),core::InvalidDoseException);
			CHECK_EQUAL(myDoseStats.getNumberOfVoxels(),doseVals->size());
			CHECK_EQUAL(myDoseStatToBeFilled.getNumberOfVoxels(),doseVals->size());	

			//compute values for comparison
			DoseStatisticType maximum = 0;
			DoseStatisticType minimum = 1000000;
			DoseStatisticType mean = 0;
			std::vector<DoseTypeGy>::const_iterator doseIt = doseVals->begin(); 
			while(doseIt != doseVals->end())
				{
				if (maximum < *doseIt)
					{
					maximum = *doseIt;
					}
				if (minimum > *doseIt)
					{
					minimum = *doseIt;
					}
				mean += *doseIt;
				doseIt++;
				}
			mean /=doseVals->size();
			float compMean = (int(mean*100))/100;

			boost::shared_ptr< std::vector<std::pair<DoseTypeGy,VoxelGridID> > > myResultPairs = 
				boost::make_shared< std::vector<std::pair<DoseTypeGy,VoxelGridID> > >();
			ResultListPointer spMyResultPairs(myResultPairs);
			boost::shared_ptr< std::vector<std::pair<DoseTypeGy,VoxelGridID> > > myResultPairs2 = 
				boost::make_shared< std::vector<std::pair<DoseTypeGy,VoxelGridID> > >();
			ResultListPointer spMyResultPairs2(myResultPairs2);

      CHECK_THROW_EXPLICIT(myEmptyDoseStat.getMaximum(spMyResultPairs),core::InvalidDoseException);
			CHECK_EQUAL(myDoseStats.getMaximum(spMyResultPairs),maximum);
			CHECK_EQUAL(myDoseStatToBeFilled.getMaximum(spMyResultPairs2),maximum);
			CHECK(spMyResultPairs->size()>0);

			CHECK_EQUAL(spMyResultPairs->size(),spMyResultPairs2->size());
			std::vector<std::pair<DoseTypeGy,VoxelGridID> >::iterator pairIt = spMyResultPairs->begin();
			std::vector<std::pair<DoseTypeGy,VoxelGridID> >::iterator pairIt2 = spMyResultPairs2->begin();

			for (;pairIt != spMyResultPairs->end();++pairIt)
				{
				CHECK_EQUAL(pairIt->first,pairIt2->first);
				CHECK_EQUAL(pairIt->second,pairIt2->second);
				++pairIt2;
				}

			spMyResultPairs = boost::make_shared< std::vector<std::pair<DoseTypeGy,VoxelGridID> > >();
			spMyResultPairs2 = boost::make_shared< std::vector<std::pair<DoseTypeGy,VoxelGridID> > >();
      CHECK_THROW_EXPLICIT(myEmptyDoseStat.getMinimum(spMyResultPairs),core::InvalidDoseException);
			CHECK_EQUAL(myDoseStats.getMinimum(spMyResultPairs),minimum);
			CHECK_EQUAL(myDoseStatToBeFilled.getMinimum(spMyResultPairs2),minimum);
			CHECK(spMyResultPairs->size()>0);

			CHECK_EQUAL(spMyResultPairs->size(),spMyResultPairs2->size());
			pairIt = spMyResultPairs->begin();
			pairIt2 = spMyResultPairs2->begin();

			for (;pairIt != spMyResultPairs->end();++pairIt)
				{
				CHECK_EQUAL(pairIt->first,pairIt2->first);
				CHECK_EQUAL(pairIt->second,pairIt2->second);
				++pairIt2;
				}

      CHECK_THROW_EXPLICIT(myEmptyDoseStat.getMean(),core::InvalidDoseException);
			float tmpMean = myDoseStats.getMean();
			tmpMean = (int(tmpMean*100))/100;
			CHECK_EQUAL(tmpMean,compMean);
			tmpMean = myDoseStatToBeFilled.getMean();
			tmpMean = (int(tmpMean*100))/100;
			CHECK_EQUAL(tmpMean,compMean);

			//generate specific example dose
			maximum = 9.5;
			minimum = 2.5;
			mean = 6;
			int sizeTemplate = 500;
			std::vector<DoseTypeGy> aDoseVector;
			for (int i = 0; i < sizeTemplate; i++){	
				aDoseVector.push_back(maximum);
				aDoseVector.push_back(minimum);
				}

			core::GeometricInfo geoInfo = spTestDoseAccessor->getGeometricInfo();
			geoInfo.setNumRows(20);
			geoInfo.setNumColumns(10);
			geoInfo.setNumSlices(5);

			boost::shared_ptr<DummyDoseAccessor> spTestDoseAccessor2 = 
				boost::make_shared<DummyDoseAccessor>(aDoseVector,geoInfo);
			DoseAccessorPointer spDoseAccessor2(spTestDoseAccessor2);

			boost::shared_ptr<core::GenericDoseIterator> spTestDoseIterator2 = 
				boost::make_shared<core::GenericDoseIterator>(spDoseAccessor2);
			DoseIteratorPointer spDoseIterator2 (spTestDoseIterator2);

			rttb::algorithms::DoseStatistics myDoseStats2(spDoseIterator2);

			spMyResultPairs = boost::make_shared< std::vector<std::pair<DoseTypeGy,VoxelGridID> > >();
			CHECK_EQUAL(myDoseStats2.getMaximum(spMyResultPairs),maximum);
			CHECK(spMyResultPairs->size()>0);

			CHECK_EQUAL(spMyResultPairs->size(),sizeTemplate);
			pairIt = spMyResultPairs->begin();

			for (;pairIt != spMyResultPairs->end();++pairIt)
				{
				CHECK_EQUAL(pairIt->first,maximum);
				}

			spMyResultPairs = boost::make_shared< std::vector<std::pair<DoseTypeGy,VoxelGridID> > >();
			int maxmialFound = 200; 
			CHECK_EQUAL(myDoseStats2.getMinimum(spMyResultPairs, maxmialFound),minimum);
			CHECK(spMyResultPairs->size()>0);

			CHECK_EQUAL(spMyResultPairs->size(),maxmialFound);
			pairIt = spMyResultPairs->begin();

			for (;pairIt != spMyResultPairs->end();++pairIt)
				{
				CHECK_EQUAL(pairIt->first,minimum);
				}

			CHECK_EQUAL(myDoseStats2.getMean(),mean);

			RETURN_AND_REPORT_TEST_SUCCESS;
			}

		}//end namespace testing
	}//end namespace rttb
