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
#ifndef __DOSE_ACCESSOR_PROCESSOR_BASE_H
#define __DOSE_ACCESSOR_PROCESSOR_BASE_H

#include <boost/shared_ptr.hpp>

#include "rttbDoseAccessorProcessorInterface.h"

namespace rttb{
	namespace core
		{
		/*! @class DoseAccessorProcessorBase
			@brief Abstract class for all DoseAccessor generating classes
		*/
		class DoseAccessorProcessorBase: public DoseAccessorProcessorInterface
			{
			public:
				typedef core::DoseAccessorInterface::DoseAccessorPointer DoseAccessorPointer;

				virtual void setDoseAccessor(DoseAccessorPointer accessor)
				{
				  _doseAccessor = accessor;
				};				

			private:
				DoseAccessorProcessorBase(const DoseAccessorProcessorBase&); //not implemented on purpose -> non-copyable
				DoseAccessorProcessorBase& operator=(const DoseAccessorProcessorBase&);//not implemented on purpose -> non-copyable
				
			protected:	
				DoseAccessorProcessorBase(){};
				virtual ~DoseAccessorProcessorBase(){};

				/*! @brief Dose accessor which should be generated */
				DoseAccessorPointer _doseAccessor;
			};
		}
	}

#endif
