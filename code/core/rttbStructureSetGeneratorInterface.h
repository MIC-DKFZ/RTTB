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
#ifndef __STRUCTURE_SET_GENERATOR_INTERFACE_H
#define __STRUCTURE_SET_GENERATOR_INTERFACE_H

#include <boost/shared_ptr.hpp>

#include "rttbStructureSet.h"

namespace rttb{
	namespace core
		{
		/*! @class StructureSetGeneratorInterface
			@brief Interface for all structure set generating classes
		*/
		class StructureSetGeneratorInterface
			{
			public:
				typedef boost::shared_ptr<StructureSet> StructureSetPointer;


			
			private:
				StructureSetGeneratorInterface(const StructureSetGeneratorInterface&); //not implemented on purpose -> non-copyable
				StructureSetGeneratorInterface& operator=(const StructureSetGeneratorInterface&);//not implemented on purpose -> non-copyable
				
				
			protected:	
				StructureSetGeneratorInterface() {};
				virtual ~StructureSetGeneratorInterface() {};

			public: 
				
				
				/*! @brief Generate StructureSet 
					@return Return shared pointer of StructureSet. 
				*/
				virtual StructureSetPointer generateStructureSet() = 0;
			};
		}
	}

#endif
