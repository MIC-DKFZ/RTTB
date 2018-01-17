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
#ifndef __MASK_ACCESSOR_GENERATOR_INTERFACE_H
#define __MASK_ACCESSOR_GENERATOR_INTERFACE_H


#include "rttbMaskAccessorInterface.h"

namespace rttb
{
	namespace core
	{
		/*! @class MaskAccessorGeneratorInterface
			@brief Interface for all MaskAccessor generating classes
		*/
		class MaskAccessorGeneratorInterface
		{
		public:
			using MaskAccessorPointer = core::MaskAccessorInterface::MaskAccessorPointer;


		private:
			MaskAccessorGeneratorInterface(const
			                               MaskAccessorGeneratorInterface&); //not implemented on purpose -> non-copyable
			MaskAccessorGeneratorInterface& operator=(const
			        MaskAccessorGeneratorInterface&);//not implemented on purpose -> non-copyable


		protected:
			MaskAccessorGeneratorInterface() = default;
			virtual ~MaskAccessorGeneratorInterface() = default;

		public:


			/*! @brief Generate MaskAccessor
				@return Return shared pointer of MaskAccessor.
			*/
			virtual MaskAccessorPointer generateMaskAccessor() = 0;
		};
	}
}

#endif
