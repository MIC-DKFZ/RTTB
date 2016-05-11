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

#ifndef __INVALID_DOSE_EXCEPTION_H
#define __INVALID_DOSE_EXCEPTION_H

#include <string>
#include <exception>

#include "rttbException.h"

#include "RTTBCoreExports.h"

namespace rttb
{
	namespace core
	{

		/*! @class InvalidDoseException
			@brief This exception will be thrown if dose is invalid.
		*/
		class RTTBCore_EXPORT InvalidDoseException : public Exception
		{
		public:
			InvalidDoseException(const std::string& aWhat): Exception(aWhat) {}

			virtual ~InvalidDoseException() throw() {}

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
