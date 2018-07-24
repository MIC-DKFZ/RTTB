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

#ifndef __DVH_GENERATOR_INTERFACE_H
#define __DVH_GENERATOR_INTERFACE_H

#include <boost/shared_ptr.hpp>

#include "rttbDVH.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

namespace rttb
{
	namespace core
	{
		/*! @class DVHGeneratorInterface
			@brief Interface for all DVH generating classes
		*/
		class DVHGeneratorInterface
		{
		protected:
      core::DVH::Pointer _dvh;
		public:
			/*! @brief Generate DVH
				@return Return new shared pointer of DVH.
			*/
			virtual core::DVH::Pointer generateDVH() = 0;
		};
	}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
