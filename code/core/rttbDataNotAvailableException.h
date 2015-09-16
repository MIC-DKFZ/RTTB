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

#ifndef __DATA_NOT_AVAILABLE_EXCEPTION_H
#define __DATA_NOT_AVAILABLE_EXCEPTION_H

#include <string>
#include <exception>

#include "rttbException.h"


namespace rttb
{
	namespace core
	{

		/*! @class DataNotAvailableException
			@brief This exception will be thrown if the requested data is not available.
		*/
		class DataNotAvailableException : public Exception
		{
		public:
			DataNotAvailableException(const std::string& aWhat) : Exception(aWhat) {}

			virtual ~DataNotAvailableException() throw() {}

			/*! @brief Get the exception description
			*/
			virtual const char* what() const throw();

			/*! @brief Get the name of the exception class
			*/
			virtual const char* GetNameOfClass() const;
		};

	}
}

#endif
