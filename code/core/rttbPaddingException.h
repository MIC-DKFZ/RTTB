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

#ifndef __PADDING_EXCEPTION_H
#define __PADDING_EXCEPTION_H

#include <string>

#include "rttbException.h"


namespace rttb
{
	namespace core
	{

		/*! @class PaddingException
			@brief This exception will be thrown if it can't be guaranteed that a transformation covers only a part of the target space.
		*/
		class PaddingException: public Exception
		{
		public:
			PaddingException(const std::string& aWhat): Exception(aWhat) {}
		};

	}
}

#endif
