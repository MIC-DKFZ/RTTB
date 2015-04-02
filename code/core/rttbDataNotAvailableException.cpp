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
// @version $Revision: 741 $ (last changed revision)
// @date    $Date: 2014-09-16 16:34:22 +0200 (Di, 16 Sep 2014) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
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
