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
#ifndef __DOSE_ACCESSOR_GENERATOR_BASE_H
#define __DOSE_ACCESSOR_GENERATOR_BASE_H

#include <boost/shared_ptr.hpp>

#include "rttbDoseAccessorGeneratorInterface.h"

namespace rttb{
	namespace core
		{
		/*! @class DoseAccessorGeneratorBase
			@brief Abstract class for all DoseAccessor generating classes
		*/
		class DoseAccessorGeneratorBase: public DoseAccessorGeneratorInterface
			{
			public:
				typedef core::DoseAccessorInterface::DoseAccessorPointer DoseAccessorPointer;

			
			private:
				
				
			protected:	
				/*! @brief Dose accessor which should be generated */
				DoseAccessorPointer _doseAccessor;


			public: 
				
			};
		}
	}

#endif
