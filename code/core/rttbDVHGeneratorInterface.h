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
#ifndef __DVH_GENERATOR_INTERFACE_H
#define __DVH_GENERATOR_INTERFACE_H

#include <boost/shared_ptr.hpp>

#include "rttbDVH.h"

namespace rttb
{
	namespace core
	{
		/*! @class DVHGeneratorInterface
			@brief Interface for all DVH generating classes
		*/
		class DVHGeneratorInterface
		{
		public:
			typedef core::DVH::DVHPointer DVHPointer;
		protected:
			DVHPointer _dvh;
		public:
			/*! @brief Generate DVH
				@return Return new shared pointer of DVH.
			*/
			virtual DVHPointer generateDVH() = 0;
		};
	}
}

#endif
