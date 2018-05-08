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

#include <limits>
#include "rttbHomogeneityIndex.h"
#include "rttbInvalidParameterException.h"
#include "rttbExceptionMacros.h"

namespace rttb
{
	namespace indices
	{

		HomogeneityIndex::HomogeneityIndex(core::DVHSet::Pointer dvhSet, DoseTypeGy aDoseReference)
			: DvhBasedDoseIndex(dvhSet, aDoseReference)
		{
			init();
		}

		bool HomogeneityIndex::calcIndex()
		{
			double max = 0;
			double min = std::numeric_limits<double>::max();
			std::vector<core::DVH> dvhTVSet = this->_dvhSet->getTargetVolumeSet();
			std::vector<core::DVH>::iterator it;

			for (it = dvhTVSet.begin(); it != dvhTVSet.end(); ++it)
			{
				core::DVH dvh = *(it);

				if (it == dvhTVSet.begin())
				{
					min = dvh.getMinimum();
				}

				if (dvh.getMaximum() > max)
				{
					max = dvh.getMaximum();
				}

				if (dvh.getMinimum() < min)
				{
					min = dvh.getMinimum();
				}
			}

			if (this->getDoseReference() != 0)
			{
				_value = (max - min) / this->getDoseReference();
			}
			else
			{
				rttbExceptionMacro(core::InvalidParameterException,
				                   << "Reference dose " << this->getDoseReference() <<
				                   " invalid: Volume of reference dose should not be 0!");
			}

			return true;

		}

		IndexValueType HomogeneityIndex::getValueAt(core::DVHSet::IndexType tvIndex)
		{
			std::vector<core::DVH> dvhTVSet = this->_dvhSet->getTargetVolumeSet();

			if (tvIndex >= dvhTVSet.size())
			{
				rttbExceptionMacro(core::InvalidParameterException,
				                   << "tvIndex invalid: it should be <" << dvhTVSet.size() << "!");
			}

			core::DVH dvh = dvhTVSet.at(tvIndex);

			if (this->getDoseReference() <= 0)
			{
				rttbExceptionMacro(core::InvalidParameterException,
				                   << "Reference dose " << this->getDoseReference() <<
				                   " invalid: Volume of reference dose should not be 0!");
			}

			return (dvh.getMaximum() - dvh.getMinimum()) / this->getDoseReference();
		}

	}//end namespace indices
}//end namespace rttb
