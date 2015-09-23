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
// @version $Revision: 877 $ (last changed revision)
// @date    $Date: 2015-01-09 10:51:10 +0100 (Fr, 09 Jan 2015) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/


#include "rttbAccessorWithGeoInfoBase.h"

namespace rttb
{
	namespace core
	{

		const core::GeometricInfo&
		AccessorWithGeoInfoBase::
		getGeometricInfo() const
		{
			return _geoInfo;
		}

		AccessorWithGeoInfoBase::~AccessorWithGeoInfoBase()
		{
		}

		AccessorWithGeoInfoBase::AccessorWithGeoInfoBase()
		{
		}

	}
}

