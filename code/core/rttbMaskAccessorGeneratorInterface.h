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
// @version $Revision: 707 $ (last changed revision)
// @date    $Date: 2014-09-04 16:37:24 +0200 (Do, 04 Sep 2014) $ (last change date)
// @author  $Author: floca $ (last changed by)
*/
#ifndef __MASK_ACCESSOR_GENERATOR_INTERFACE_H
#define __MASK_ACCESSOR_GENERATOR_INTERFACE_H


#include "rttbMaskAccessorInterface.h"

namespace rttb{
	namespace core
		{
		/*! @class MaskAccessorGeneratorInterface
			@brief Interface for all MaskAccessor generating classes
		*/
		class MaskAccessorGeneratorInterface
			{
			public:
				typedef core::MaskAccessorInterface::MaskAccessorPointer MaskAccessorPointer;

			
			private:
				MaskAccessorGeneratorInterface(const MaskAccessorGeneratorInterface&); //not implemented on purpose -> non-copyable
				MaskAccessorGeneratorInterface& operator=(const MaskAccessorGeneratorInterface&);//not implemented on purpose -> non-copyable
				
				
			protected:	
				MaskAccessorGeneratorInterface() {};
				virtual ~MaskAccessorGeneratorInterface() {};

			public: 
				
				
				/*! @brief Generate MaskAccessor 
					@return Return shared pointer of MaskAccessor. 
				*/
				virtual MaskAccessorPointer generateMaskAccessor() = 0;
			};
		}
	}

#endif
