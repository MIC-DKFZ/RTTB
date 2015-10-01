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

#include <iostream>
#include <sstream>

#include "rttbDataNotAvailableException.h"

namespace rttb
{
	namespace core
	{

		const char* DataNotAvailableException::what() const throw()
		{
			return rttb_what.c_str();
		}

		const char* DataNotAvailableException::GetNameOfClass() const
		{
			return "DataNotAvailableException";
		}

	}//end namespace core
}//end namespace rttb
