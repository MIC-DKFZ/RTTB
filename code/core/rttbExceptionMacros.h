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

#include <sstream>

#include "rttbException.h"

/*!@def rttbDefaultExceptionMacro
* This macro is used to throw a basic ExceptionObject within an object method.
* It will generate the exception message and throw the exception.\n
* Use rttbDefaultExceptionStaticMacro() for other objects that do not compli with the api or for
* static functions.\n
* Use rttbExceptionMacro() if you want to specifiy a arbitrary exception class that should be thrown.
* Example usage looks like:
* rttbDefaultExceptionMacro(<< "this is an exception" << this->SomeVariable);
* @ingroup Exception
* @todo for future extension of the exception classes: Use if file and line are supported in Exception
* ::rttb::core::Exception e_(__FILE__, __LINE__, message.str().c_str());
*/
#define rttbDefaultExceptionMacro(x) \
  { \
    std::ostringstream message; \
    message << "Exception: " \
            << "(" << this << "): " x; \
    ::rttb::core::Exception e_(message.str().c_str()); \
    throw e_; /* Explicit naming to work around Intel compiler bug.  */ \
  }

/*!@def mapExceptionMacro
* This macro is used to throw the passed exception class within an object method.
* The macro presumes that the object owns a method this->GetNameOfClass().\n
* The macro will set file name, line number and function signiture to the exception
* and log the exception as error in the logbook before throwing it.\n
* Use rttbExceptionStaticMacro() for other objects that do not compli with the api or for
* static functions.\n
* @sa mapGenericExceptionMacro
*
* Example usage looks like:
* rttbExceptionMacro(AnExceptionClass, << "this is an exception" << this->SomeVariable);
* @ingroup Exception
* @todo for future extension of the exception classes: Use if file and line are supported in Exception
* EType e_(__FILE__, __LINE__, message.str().c_str());
*/
#define rttbExceptionMacro( EType, x) \
  { \
    std::ostringstream  message; \
    message << "Exception: "\
            << "(" << this << "): " x; \
    EType e_(message.str().c_str()); \
    throw e_; /* Explicit naming to work around Intel compiler bug.  */ \
  }

/*!@def rttbDefaultExceptionStaticMacro
* This macro is used to throw a basic ExceptionObject within a static method.
* It will generate the exception message and throw the exception.\n
* Use rttbExceptionStaticMacro() if you want to specifiy a arbitrary exception class that should be thrown.
* Example usage looks like:
* rttbDefaultExceptionMacro(<< "this is an exception" << SomeVariable);
* @ingroup Exception
* @todo for future extension of the exception classes: Use if file and line are supported in Exception
* ::rttb::core::Exception e_(__FILE__, __LINE__, message.str().c_str());
*/
#define rttbDefaultExceptionStaticMacro(x) \
  { \
    std::ostringstream message; \
    message << "Exception: " x; \
    ::rttb::core::Exception e_(message.str().c_str()); \
    throw e_; /* Explicit naming to work around Intel compiler bug.  */ \
  }

/*!@def rttbExceptionStaticMacro
* This macro is used to throw the passed exception class within a static method.
* The macro will set file name, line number and function signiture to the exception
* and log the exception as error in the logbook before throwing it.\n
* Use mapGenericExceptionMacro() for other objects that do not compli with the api or for
* static functions.\n
* @sa mapGenericExceptionMacro
*
* Example usage looks like:
* rttbExceptionMacro(AnExceptionClass, << "this is an exception" << this->SomeVariable);
* @ingroup Exception
* @todo for future extension of the exception classes: Use if file and line are supported in Exception
* EType e_(__FILE__, __LINE__, message.str().c_str());
*/
#define rttbExceptionStaticMacro( EType, x) \
  { \
    std::ostringstream  message; \
    message << "Exception: " x; \
    EType e_(message.str().c_str()); \
    throw e_; /* Explicit naming to work around Intel compiler bug.  */ \
  }
