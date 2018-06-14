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

#ifndef __RTTB_COMMON_H
#define __RTTB_COMMON_H

#include <boost/shared_ptr.hpp>

#define rttbClassMacro(className, superClassName)                                                          \
  typedef className Self;                                                                                  \
  typedef superClassName Superclass;                                                                       \
  typedef boost::shared_ptr<Self> Pointer;                                                                 \
  typedef boost::shared_ptr<const Self> ConstPointer;                                                      \
  static const char *getStaticNameOfClass() { return #className; }                                         \
  virtual const char *getNameOfClass() const override                                                      \
  {                                                                                                        \
    return #className;                                                                                     \
  }

#define rttbClassMacroNoParent(className)                                                                  \
  typedef className Self;                                                                                  \
  typedef boost::shared_ptr<Self> Pointer;                                                                 \
  typedef boost::shared_ptr<const Self> ConstPointer;                                                      \
  static const char *getStaticNameOfClass() { return #className; }                                         \
  virtual const char *getNameOfClass() const                                                               \
  {                                                                                                        \
    return #className;                                                                                     \
  }

#endif
