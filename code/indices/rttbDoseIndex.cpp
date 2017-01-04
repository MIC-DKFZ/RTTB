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

#include "rttbDoseIndex.h"
#include "rttbException.h"
#include "rttbInvalidParameterException.h"

namespace rttb
{
	namespace indices
	{

		DoseIndex::DoseIndex(DoseTypeGy aDoseReference)
			: _doseReference(aDoseReference), _initSuccess(false) {}

        DoseIndex::~DoseIndex(){}

		bool DoseIndex::init()
		{
			if (!(this->checkInputs()))
			{
				throw core::InvalidParameterException("Check inputs failed: invalid parameters! ");
			}

			if (this->calcIndex())
			{
				_initSuccess = true;
			}
			else
			{
				throw core::InvalidParameterException("Index calculation failed! ");
			}

			return _initSuccess;
		}

		void DoseIndex::setDoseReference(DoseTypeGy aDoseReference)
		{
			_doseReference = aDoseReference;
			_initSuccess = false;
			init();
		}

		DoseTypeGy DoseIndex::getDoseReference() const
		{
			return _doseReference;
		}

		IndexValueType DoseIndex::getValue() const
		{
			if (_initSuccess)
			{
				return _value;
			}
			else
			{
				throw core::Exception("DoseIndex init error: init() must be called first!");
			}
		}
	}
}


