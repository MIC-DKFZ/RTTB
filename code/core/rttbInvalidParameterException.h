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

#ifndef __INVALID_PARAMETER_EXCEPTION_H
#define __INVALID_PARAMETER_EXCEPTION_H

#include <string>

#include "rttbException.h"

namespace rttb
{
	namespace core
	{

		/*! @class InvalidParameterException
			@brief This exception will be thrown if any parameter is invalid.
		*/
		class InvalidParameterException : public Exception
		{
		public:
      explicit InvalidParameterException(const std::string& aWhat): Exception(aWhat) {}
		};

	}
}
#endif
