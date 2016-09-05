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


#include "litCheckMacros.h"

#include "rttbDVH.h"

#ifndef __DVH_COMPARER_H
#define __DVH_COMPARER_H

namespace rttb
{

	namespace testing
	{

		typedef core::DVH::DVHPointer DVHPointer;

		/*! Compare 2 dvhs and return the results.
			@result Indicates if the test was successfull (true) or if it failed (false)
		*/
		bool checkEqualDVH(DVHPointer aDVH1, DVHPointer aDVH2);

	}//testing
}//rttb
#endif
