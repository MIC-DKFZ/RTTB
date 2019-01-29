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

#ifndef __RTTBEXCEPTION_H
#define __RTTBEXCEPTION_H

#include <string>
#include <stdexcept>

namespace rttb
{
	namespace core
	{

		/*! @class Exception
			@brief Exception interface used by all RTToolbox exceptions.
		*/

		class Exception : public std::runtime_error
		{
		public:
			explicit Exception(const std::string& aWhat)
				: runtime_error(aWhat)
			{};
		};

	}
}

#endif
