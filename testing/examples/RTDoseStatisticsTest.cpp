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

#include <math.h>

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include "dcmtk/config/osconfig.h"    /* make sure OS specific configuration is included first */
#include "dcmtk/dcmrt/drtdose.h"
#include "dcmtk/dcmdata/dcfilefo.h"

#include "litCheckMacros.h"
#include "rttbDoseStatistics.h"
#include "rttbDicomDoseAccessor.h"
#include "rttbDicomFileDoseAccessorGenerator.h"
#include "rttbGenericDoseIterator.h"
#include "rttbNullPointerException.h"
#include "rttbBaseType.h"

namespace rttb
{
	namespace testing
	{

		/*! @brief RTDoseStatisticsTest. Max, min, mean, standard deviation, variance, Vx, Dx, MOHx, MOCx, MaxOHx,
		MinOCx are tested. Test if calculation in new architecture returns similar results to the original implementation.

		WARNING: The values for comparison need to be adjusted if the input files are changed!*/
		int RTDoseStatisticsTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			typedef core::GenericDoseIterator::DoseAccessorPointer DoseAccessorPointer;
			typedef core::GenericDoseIterator::DoseIteratorPointer DoseIteratorPointer;
			typedef algorithms::DoseStatistics::ResultListPointer ResultListPointer;

			//ARGUMENTS: 1: dose1 file name
			//           2: dose2 file name

			std::string RTDOSE_FILENAME;
			std::string RTDOSE2_FILENAME;

			if (argc > 1)
			{
				RTDOSE_FILENAME = argv[1];
			}

			if (argc > 2)
			{
				RTDOSE2_FILENAME = argv[2];
			}


			OFCondition status;
			DcmFileFormat fileformat;
			double max, min;

			const double expectedVal = 5.64477e-005;
			const double reducedErrorConstant = 0.0001;

			/*Test NullPointerException*/

			rttb::algorithms::DoseStatistics doseStatistics;
			typedef boost::shared_ptr<std::vector<std::pair<DoseTypeGy, VoxelGridID> > > ResultsVectorPointer;

			ResultsVectorPointer spResults =
			    boost::make_shared<std::vector<std::pair<DoseTypeGy, VoxelGridID> > >();

			ResultListPointer minListPtr(spResults);
			ResultListPointer maxListPtr(spResults);

			::DRTDoseIOD rtdoseDKFZ;
			io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator1(RTDOSE_FILENAME.c_str());
			DoseAccessorPointer doseAccessor1(doseAccessorGenerator1.generateDoseAccessor());

			//create corresponding DoseIterator
			boost::shared_ptr<core::GenericDoseIterator> spDoseIteratorTmp =
			    boost::make_shared<core::GenericDoseIterator>(doseAccessor1);
			DoseIteratorPointer spDoseIterator(spDoseIteratorTmp);
			doseStatistics.setDoseIterator(spDoseIterator);

			CHECK_CLOSE(doseStatistics.getMean(), expectedVal, errorConstant);
			CHECK_CLOSE(doseStatistics.getStdDeviation(), 0, errorConstant);
			CHECK_CLOSE(doseStatistics.getVariance(), 0, errorConstant);

			DoseTypeGy vx = doseStatistics.getVx(expectedVal);
			CHECK_EQUAL(vx, doseStatistics.getVx(0));
			CHECK_CLOSE(expectedVal, doseStatistics.getDx(vx), reducedErrorConstant);

			max = doseStatistics.getMaximum(maxListPtr);
			CHECK_CLOSE(doseStatistics.getMaximum(maxListPtr), expectedVal, errorConstant);
			CHECK_EQUAL(doseStatistics.getVx(max + reducedErrorConstant), 0);

			min = doseStatistics.getMinimum(minListPtr);
			CHECK_CLOSE(min, expectedVal, errorConstant);
			CHECK_EQUAL(minListPtr->size(), 100);
			min = doseStatistics.getMinimum(minListPtr, 50);
			CHECK_EQUAL(minListPtr->size(), 50);

			DoseTypeGy wholeVolume = doseStatistics.getVx(min);
			CHECK_CLOSE(doseStatistics.getDx(wholeVolume), min, 0.001);
			CHECK_CLOSE(doseStatistics.getMOHx(vx), doseStatistics.getMean(), reducedErrorConstant);
			CHECK_CLOSE(doseStatistics.getMOCx(20000), doseStatistics.getMean(), reducedErrorConstant);
			CHECK_CLOSE(doseStatistics.getMinOCx(20000), doseStatistics.getMean(), reducedErrorConstant);

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb