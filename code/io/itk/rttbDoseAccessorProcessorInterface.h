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
#ifndef __DOSE_ACCESSOR_PROCESSOR_INTERFACE_H
#define __DOSE_ACCESSOR_PROCESSOR_INTERFACE_H

#include "rttbDoseAccessorInterface.h"

namespace rttb
{
	namespace core
	{
		/*! @class DoseAccessorProcessorInterface
			@brief Interface for all DoseAccessor generating classes
		*/
		class DoseAccessorProcessorInterface
		{
		public:
			typedef core::DoseAccessorInterface::DoseAccessorPointer DoseAccessorPointer;


		private:
			DoseAccessorProcessorInterface(const
			                               DoseAccessorProcessorInterface&); //not implemented on purpose -> non-copyable
			DoseAccessorProcessorInterface& operator=(const
			        DoseAccessorProcessorInterface&);//not implemented on purpose -> non-copyable


		protected:
			DoseAccessorProcessorInterface() {};
			virtual ~DoseAccessorProcessorInterface() {};

		public:

			/*! @brief Sets the DoseAccessor that should be processed
				@pre passed accessor must point to a valid instance.
			*/
			virtual void setDoseAccessor(DoseAccessorPointer accessor) = 0;

			/*! @brief Process the passed DoseAccessor
				@return if the processing was successful.
			*/
			virtual bool process() = 0;
		};
	}
}

#endif
