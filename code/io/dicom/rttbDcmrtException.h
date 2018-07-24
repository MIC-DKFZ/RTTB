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

#ifndef __DCMRT_EXCEPTION_H
#define __DCMRT_EXCEPTION_H

#include <string>

#include "rttbException.h"

namespace rttb
{
	namespace io
	{
		namespace dicom
		{

			/*! @class DcmrtException
			@brief This class represents a DcmrtException. Any dcmrt error will throw this exception.
			*/
			class DcmrtException: public core::Exception
			{
			public:
				DcmrtException(const std::string& aWhat): Exception(aWhat) {}
			};
		}
	}

}

#endif
