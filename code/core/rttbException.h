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

#ifndef __RTTBEXCEPTION_H
#define __RTTBEXCEPTION_H

#include <string>
#include <stdexcept>

#include "RTTBCoreExports.h"

namespace rttb
{
	namespace core
	{

		/*! @class Exception
			@brief Exception interface used by all RTToolbox exceptions.
		*/

		class RTTBCore_EXPORT Exception : public std::runtime_error
		{
		public:
			Exception(const std::string& aWhat)
				: runtime_error(aWhat)
			{};
		};

	}
}

#endif
