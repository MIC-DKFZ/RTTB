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

#ifndef __ACCESSOR_WITH_GEO_INFO_BASE_H
#define __ACCESSOR_WITH_GEO_INFO_BASE_H

#include "rttbAccessorInterface.h"

#include "RTTBCoreExports.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

namespace rttb
{
	namespace core
	{

		/*! @class AccessorWithGeoInfoBase
		@brief Base class for all accessor implementations that have there own geometric info.
		*/
		class RTTBCore_EXPORT AccessorWithGeoInfoBase : public core::AccessorInterface
		{
		protected:
      virtual void assembleGeometricInfo()=0;
			core::GeometricInfo _geoInfo;

		public:
			~AccessorWithGeoInfoBase() override;

			AccessorWithGeoInfoBase();

			const core::GeometricInfo& getGeometricInfo() const override;

		};
	}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
