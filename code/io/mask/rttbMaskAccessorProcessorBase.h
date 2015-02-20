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
// @version $Revision: 793 $ (last changed revision)
// @date    $Date: 2014-10-10 10:24:45 +0200 (Fr, 10 Okt 2014) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/
#ifndef __MASK_ACCESSOR_PROCESSOR_BASE_H
#define __MASK_ACCESSOR_PROCESSOR_BASE_H

#include <boost/shared_ptr.hpp>

#include "rttbMaskAccessorProcessorInterface.h"

namespace rttb
{
	namespace core
	{
		/*! @class MaskAccessorProcessorBase
			@brief Abstract class for all MaskAccessor converter classes
		*/
		class MaskAccessorProcessorBase: public MaskAccessorProcessorInterface
		{
		public:
			typedef core::MaskAccessorInterface::MaskAccessorPointer MaskAccessorPointer;

			virtual void setMaskAccessor(MaskAccessorPointer accessor)
			{
				_maskAccessor = accessor;
			};

		private:
			MaskAccessorProcessorBase(const
			                          MaskAccessorProcessorBase&); //not implemented on purpose -> non-copyable
			MaskAccessorProcessorBase& operator=(const
			                                     MaskAccessorProcessorBase&);//not implemented on purpose -> non-copyable

		protected:
			MaskAccessorProcessorBase() {};
			virtual ~MaskAccessorProcessorBase() {};

			/*! @brief Mask accessor which should be generated */
			MaskAccessorPointer _maskAccessor;
		};
	}
}

#endif
