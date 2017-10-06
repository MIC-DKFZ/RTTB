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

#ifndef __INDEX_OUT_OF_BOUNDS_EXCEPTION_H
#define __INDEX_OUT_OF_BOUNDS_EXCEPTION_H

#include <string>
#include <exception>

#include "rttbException.h"
#include "RTTBCoreExports.h"

namespace rttb
{

	namespace core
	{

		/*! @class IndexOutOfBoundsException
			@brief This exception will be thrown if any index out of bound.
		*/
        class RTTBCore_EXPORT IndexOutOfBoundsException : public Exception
		{
		public:
			IndexOutOfBoundsException(const std::string& aWhat): Exception(aWhat) {}

			~IndexOutOfBoundsException() throw() override {}

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
