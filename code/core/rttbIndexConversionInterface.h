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

#ifndef __INDEX_CONVERSION_INTERFACE_NEW_H
#define __INDEX_CONVERSION_INTERFACE_NEW_H

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

#include <rttbCommon.h>
#include "RTTBCoreExports.h"

namespace rttb
{

	namespace core
	{
		/*! @class IndexConversionInterface
			@brief This class represents the conversion of 3D grid indices to 1D grid IDs.
		*/
		class RTTBCore_EXPORT IndexConversionInterface
		{
    public:
      rttbClassMacroNoParent(IndexConversionInterface);
		private:
			IndexConversionInterface(const
			                         IndexConversionInterface&) = delete; //not implemented on purpose -> non-copyable
			IndexConversionInterface& operator=(const
			                                    IndexConversionInterface&) = delete;//not implemented on purpose -> non-copyable

		protected:
			IndexConversionInterface() = default;
			virtual ~IndexConversionInterface() = default;

		};
	}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
