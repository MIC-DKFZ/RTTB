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

#ifndef __NULL_POINTER_EXCEPTION_H
#define __NULL_POINTER_EXCEPTION_H

#include <string>
#include <exception>

#include "rttbException.h"

#include "RTTBCoreExports.h"


namespace rttb
{
	namespace core
	{

		/*! @class NullPointerException
			@brief This exception will be thrown if any pointer is nullptr.
		*/
		class RTTBCore_EXPORT NullPointerException : public Exception
		{
		public:
			NullPointerException(const std::string& aWhat): Exception(aWhat) {}

			virtual ~NullPointerException() throw() {}

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
