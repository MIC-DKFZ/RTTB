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

#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <utility>

#include "rttbBioModel.h"


namespace rttb
{
	namespace models
	{
		bool BioModel::init(const double doseFactor)
		{
			if (_dvh != NULL)
			{
				_value = this->calcModel(doseFactor);
				return true;
			}

			return false;
		}

		void BioModel::setDVH(const DVHPointer aDVH)
		{
			_dvh = aDVH;
		}

		const BioModel::DVHPointer BioModel::getDVH() const
		{
			return _dvh;
		}

		const BioModelValueType BioModel::getValue() const
		{
			return _value;
		}


	}//end namespace models
}//end namespace rttb
