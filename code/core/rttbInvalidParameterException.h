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

#ifndef __INVALID_PARAMETER_EXCEPTION_H
#define __INVALID_PARAMETER_EXCEPTION_H

#include <string>
#include <exception>

#include "rttbException.h"

#include "RTTBCoreExports.h"

namespace rttb
{
	namespace core
	{

		/*! @class InvalidParameterException
			@brief This exception will be thrown if any parameter is invalid.
		*/
		class RTTBCore_EXPORT InvalidParameterException : public Exception
		{
		public:
			InvalidParameterException(const std::string& aWhat): Exception(aWhat) {}

			~InvalidParameterException() throw() override {}

			/*! @brief Get the exception description
			*/
			const char* what() const throw() override;

			/*! @brief Get the name of the exception class
			*/
			virtual const char* GetNameOfClass() const;
		};

	}
}
#endif
