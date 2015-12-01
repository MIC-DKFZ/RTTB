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
// @date $Date$ (last change date)
// @author  $Author$ (last changed by)
*/

#ifndef __SELF_INTERSECTING_STRUCTURE_EXCEPTION_H
#define __SELF_INTERSECTING_STRUCTURE_EXCEPTION_H

#include <string>
#include <exception>

#include "rttbException.h"

namespace rttb
{
	namespace masks
	{

		/*! @class SelfIntersectingStructureException
			@brief This exception will be thrown in case a Structure intersects with itself in a context where
			this is not allowed.
		*/
		class SelfIntersectingStructureException: public core::Exception
		{
		public:
			SelfIntersectingStructureException(const std::string& aWhat): Exception(aWhat) {}

			virtual ~SelfIntersectingStructureException() throw() {}

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
