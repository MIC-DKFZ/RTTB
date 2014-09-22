// -----------------------------------------------------------------------
// RTToolbox - DKFZ radiotherapy quantitative evaluation library
//
// (c) Copyright 2007, DKFZ, Heidelberg, Germany
// ALL RIGHTS RESERVED
//
// THIS FILE CONTAINS CONFIDENTIAL AND PROPRIETARY INFORMATION OF DKFZ.
// ANY DUPLICATION, MODIFICATION, DISTRIBUTION, OR
// DISCLOSURE IN ANY FORM, IN WHOLE, OR IN PART, IS STRICTLY PROHIBITED
// WITHOUT THE PRIOR EXPRESS WRITTEN PERMISSION OF DKFZ.
//
//------------------------------------------------------------------------
/*!
// @file
// @version $Revision$ (last changed revision)
// @date $Date$ (last change date)
// @author zhangl (last changed by)
// @author *none* (Reviewer)
// @author zhangl (Programmer)
//
// Subversion HeadURL: $HeadURL: http://sidt-hpc1/dkfz_repository/NotMeVisLab/SIDT/RTToolbox/trunk/testing/core/DVHCalculatorTest.cpp $
*/

#ifndef __MASK_VL_TESTER_H
#define __MASK_VL_TESTER_H

#include <boost/shared_ptr.hpp>

#include "litTesterBase.h"
#include "litString.h"

#include "rttbBaseType.h"
#include "../../code/core/rttbMaskAccessorInterface.h"

namespace rttb{

	namespace testing{
		/*! class MaskVoxelListTester
			@brief Tester class for lists. Compares two given lists for similarity.
			Both lists need to have the same length and have equal values in each element to be considered similar.
		*/
		class MaskVoxelListTester: public lit::TesterBase {

		private:
			typedef core::MaskAccessorInterface::MaskVoxelListPointer MaskVoxelListPointer; 
			typedef core::MaskAccessorInterface::MaskVoxelList MaskVoxelList; 

			MaskVoxelListPointer _referenceList;
			MaskVoxelListPointer _compareList;

			mutable float _maxDifference;
			mutable float _numDifference;

			mutable bool _sameSize;
			mutable bool _masVoxelsDiffer;

			mutable VoxelGridID _failedListIndex;

		public:
			MaskVoxelListTester(MaskVoxelListPointer aReferenceList, MaskVoxelListPointer aCompareList);
			
			/*! Set the dose accessor pointer for the dose comparison. 
			*/
			void setReferenceList(const MaskVoxelListPointer aReferenceList);
			void setCompareList(const MaskVoxelListPointer aCompareList);

			/*! Returns a string that specifies the test the tester currently performs.
		    */
			lit::StringType getTestDescription(void) const;
			lit::StringType getTestName(void) const {return "MaskVoxelListTester";};

		protected:
			/*! performes the test and checks the results.
				@result Indicates if the test was successfull (true) or if it failed (false)
			*/
			bool doCheck(void) const;

			/*! Function will be called be check() if test was succesfull.
			*/
			void handleSuccess(void) const;

			/*! Function will be called be check() if test was a failure.
			*/
			void handleFailure(void) const;

		};

	}
}

#endif