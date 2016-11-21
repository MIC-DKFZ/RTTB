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
#ifndef __COVERAGE_INDEX_H
#define __COVERAGE_INDEX_H

#include <string>
#include <vector>

#include "rttbDvhBasedDoseIndex.h"
#include "rttbBaseType.h"
#include "rttbDVHSet.h"

#include "RTTBIndicesExports.h"

namespace rttb
{

	namespace indices
	{

		/*! @class CoverageIndex
		    @brief This class representing a CoverageIndex Object. Coverage Index fraction of the target volume receiving a dose >= the reference dose
			@ingroup indices
		*/
        class RTTBIndices_EXPORT CoverageIndex : public DvhBasedDoseIndex
		{

		protected:
			/*! @brief Calculate conformation number
			    @exception InvalidParameterException Thrown if dvhSet invalid
			*/
			bool calcIndex();


		public:
			/*! @brief Constructor
			*/
			CoverageIndex(DVHSetPtr dvhSet, DoseTypeGy aDoseReference);


			/*! @brief Dose index calculation for tvIndex-th treated volume
			 *  @param tvIndex: index in the vector of DVH TV
			 *  @return Return index value
			    @exception InvalidParameterException Thrown if tvIndex invalid
			*/
			IndexValueType getValueAt(const core::DVHSet::IndexType tvIndex);



		};

	}
}

#endif
