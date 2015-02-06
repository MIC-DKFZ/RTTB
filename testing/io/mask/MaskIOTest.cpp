// -----------------------------------------------------------------------
// RTToolbox - DKFZ radiotherapy quantitative evaluation library
//
// Copyright (c) German Cancer Research Center (DKFZ),
// Software development for Integrated Diagnostics and Therapy (SIDT).
// ALL RIGHTS RESERVED.
// See rttbCopyright.txt or
// http://www.dkfz.de/en/sidt/projects/rttb/copyright.html [^]
//
// This software is distributed WITHOUT ANY WARRANTY; without even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE. See the above copyright notices for more information.
//
//------------------------------------------------------------------------
/*!
// @file
// @version $Revision: 793 $ (last changed revision)
// @date $Date: 2014-10-10 10:24:45 +0200 (Fr, 10 Okt 2014) $ (last change date)
// @author $Author: hentsch $ (last changed by)
*/

// this file defines the rttbCoreTests for the test driver
// and all it expects is that you have a function called RegisterTests

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include "litCheckMacros.h"

#include "rttbBaseType.h"
#include "rttbGeometricInfo.h"
#include "rttbDoseIteratorInterface.h"



namespace rttb
{

	namespace testing
	{

		/*!@brief MaskIOTest - test the IO for mask data

			WARNING: The values for comparison need to be adjusted if the input files are changed!
		*/

		int MaskIOTest(int argc, char* argv[])
		{
			
			PREPARE_DEFAULT_TEST_REPORTING;
			//ARGUMENTS:
			// 1: mhd file name

			std::string RTDOSE_FILENAME;

			if (argc > 1)
			{
				RTDOSE_FILENAME = argv[1];
			}

			
			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb

