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

#ifndef __INVALID_DOSE_EXCEPTION_H
#define __INVALID_DOSE_EXCEPTION_H

#include <string>

#include "rttbException.h"

namespace rttb
{
	namespace core
	{

		/*! @class InvalidDoseException
			@brief This exception will be thrown if dose is invalid.
		*/
		class InvalidDoseException : public Exception
		{
		public:
      explicit InvalidDoseException(const std::string& aWhat): Exception(aWhat) {}
		};

	}
}

#endif
