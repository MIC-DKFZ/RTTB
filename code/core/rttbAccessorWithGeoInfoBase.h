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
#ifndef __ACCESSOR_WITH_GEO_INFO_BASE_H
#define __ACCESSOR_WITH_GEO_INFO_BASE_H

#include "rttbAccessorInterface.h"

namespace rttb
{
	namespace core
	{

		/*! @class AccessorWithGeoInfoBase
		@brief Base class for all accessor implementations that have there own geometric info.
		*/
		class AccessorWithGeoInfoBase: public core::AccessorInterface
		{
		protected:
			core::GeometricInfo _geoInfo;

		public:
			~AccessorWithGeoInfoBase();

			AccessorWithGeoInfoBase();

			virtual const core::GeometricInfo& getGeometricInfo() const;

		};
	}
}

#endif