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
#ifndef __MASK_ACCESSOR_PROCESSOR_INTERFACE_H
#define __MASK_ACCESSOR_PROCESSOR_INTERFACE_H

#include "rttbMaskAccessorInterface.h"

namespace rttb
{
	namespace core
	{
		/*! @class MaskAccessorProcessorInterface
			@brief Interface for all MaskAccessor converter classes
		*/
		class MaskAccessorProcessorInterface
		{
		public:
			typedef core::MaskAccessorInterface::MaskAccessorPointer MaskAccessorPointer;


		private:
			MaskAccessorProcessorInterface(const
			                               MaskAccessorProcessorInterface&) = delete; //not implemented on purpose -> non-copyable
			MaskAccessorProcessorInterface& operator=(const
			        MaskAccessorProcessorInterface&) = delete;//not implemented on purpose -> non-copyable


		protected:
			MaskAccessorProcessorInterface() = default;
			virtual ~MaskAccessorProcessorInterface() = default;

		public:

			/*! @brief Sets the MaskAccessor that should be processed
				@pre passed accessor must point to a valid instance.
			*/
			virtual void setMaskAccessor(MaskAccessorPointer accessor) = 0;

			/*! @brief Process the passed MaskAccessor
				@return if the processing was successful.
			*/
			virtual bool process() = 0;
		};
	}
}

#endif
