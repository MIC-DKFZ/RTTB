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
#ifndef __DOSE_INDEX_H
#define __DOSE_INDEX_H


#include "rttbBaseType.h"
#include "rttbDVHSet.h"

#include "RTTBIndicesExports.h"

namespace rttb
{
	namespace indices
	{
		/*! @class DoseIndex
			@brief This is the interface for dose/plan comparison indices.
			@ingroup indices
		*/
        class RTTBIndices_EXPORT DoseIndex
		{
		protected:

			IndexValueType _value;

			DoseTypeGy _doseReference;

			/*! @brief If init() successful*/
			bool _initSuccess;

			/*! @brief Initialize the calculation. It should be called in constructor or if any parameter of the calcualtion is changed.
			@return Return true if successfully
			@exception InvalidParameterException thrown if any input is invalid or index calculation failed
			*/
			bool init();

			/*! @brief Dose index calculation */
			virtual bool calcIndex() = 0;

			/*! @brief Check all inputs for the index calculation*/
			virtual bool checkInputs() = 0;

		public:

			/*! @brief Constructor with the referece dose*/
			DoseIndex(DoseTypeGy aDoseReference);

			/*! @brief Set the reference dose
			*/
			void setDoseReference(DoseTypeGy aDoseReference);

			/*! @brief Get the reference dose
			*/
			DoseTypeGy getDoseReference() const;

			/*! @brief Get the value of dose/plan comparison index
			  @return Return the value of this index
			@exception Exception Thrown if the class was not initialized previously.
			*/
			IndexValueType getValue() const;

			/*! @brief Get the value of dose/plan comparison index for a treated volume with the index in the DVH treated volume set
			@param tvIndex index in the DVH in the current set of DVH subset for target volume: use DVHSet.getTargetVolumeSet()
			*/
			virtual IndexValueType getValueAt(const core::DVHSet::IndexType tvIndex) = 0;
		};
	}
}


#endif
