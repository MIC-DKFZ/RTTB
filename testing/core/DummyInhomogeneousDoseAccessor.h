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
#ifndef __DUMMY_DOSE_INHOMOGENEOUS_ACCESSOR_H
#define __DUMMY_DOSE_INHOMOGENEOUS_ACCESSOR_H

#include "DummyDoseAccessor.h"


namespace rttb
{
	namespace testing
	{

		/*! @class DummyInhomogeneousDoseAccessor
			@brief A dummy DoseAccessor for testing with inhomogeneous grid
		*/
		class DummyInhomogeneousDoseAccessor: public DummyDoseAccessor
		{
		public:
			~DummyInhomogeneousDoseAccessor();

			/*! @brief A dummy DoseAccessor for testing filled with random dose values between 0 and 100.
				    The default grid size is [11,10,5]
			    */
      DummyInhomogeneousDoseAccessor();

      bool isGridHomogeneous() const override;
		};
	}
}

#endif
