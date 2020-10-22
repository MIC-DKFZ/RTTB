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

#include "rttbSpatialDoseIndex.h"
#include "rttbException.h"
#include "rttbInvalidParameterException.h"

namespace rttb
{
	namespace indices
	{

		//SpatialDoseIndex::SpatialDoseIndex(DoseTypeGy aDoseReference)
		//	: _doseReference(aDoseReference), _initSuccess(false) {}

  //      SpatialDoseIndex::~SpatialDoseIndex()= default;

		//bool SpatialDoseIndex::init()
		//{
		//	if (!(this->checkInputs()))
		//	{
		//		throw core::InvalidParameterException("Check inputs failed: invalid parameters! ");
		//	}

		//	if (this->calcIndex())
		//	{
		//		_initSuccess = true;
		//	}
		//	else
		//	{
		//		throw core::InvalidParameterException("Index calculation failed! ");
		//	}

		//	return _initSuccess;
		//}

		//void SpatialDoseIndex::setDoseReference(DoseTypeGy aDoseReference)
		//{
		//	_doseReference = aDoseReference;
		//	_initSuccess = false;
		//	init();
		//}

		//DoseTypeGy SpatialDoseIndex::getDoseReference() const
		//{
		//	return _doseReference;
		//}

		//IndexValueType SpatialDoseIndex::getValue() const
		//{
		//	if (_initSuccess)
		//	{
		//		return _value;
		//	}
		//	else
		//	{
		//		throw core::Exception("SpatialDoseIndex init error: init() must be called first!");
		//	}
		//}
	}
}


