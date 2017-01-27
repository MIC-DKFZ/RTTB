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
// @version $Revision: 1377 $ (last changed revision)
// @date    $Date: 2016-06-07 10:26:43 +0200 (Di, 07 Jun 2016) $ (last change date)
// @author  $Author: strubel $ (last changed by)
*/
#ifndef __MaskProcess_h
#define __MaskProcess_h

#include "rttbStructureSetGeneratorInterface.h"
#include "rttbMaskAccessorInterface.h"
#include "rttbGenericDoseIterator.h"

namespace rttb
{
	namespace apps
	{
		namespace voxelizerTool
		{
			/*! @class MaskProcess
			@brief Creates a mask with the struct and reference object
			*/
			class MaskProcess
			{

			public:
				typedef core::MaskAccessorInterface::MaskAccessorPointer MaskAccessorPointer;
				typedef core::GenericDoseIterator::DoseAccessorPointer DoseAccessorPointer;
				typedef core::StructureSetGeneratorInterface::StructureSetPointer StructureSetPointer;

				/*!@brief Constructor
				@details save the rtStructureSet (structfile) object into _rtStructureSet and
				* doseAccessor (referencefile) object into _doseAccessor
				*/
				MaskProcess(StructureSetPointer rtStructureSet, DoseAccessorPointer doseAccessor,
				            bool strict);
				/**@brief create a mask with _rtStructureSet and _doseAccessor object.
				@return a mask object
				*/
				MaskAccessorPointer createMask(unsigned int numberOfStructure) const;
			private:
				StructureSetPointer _rtStructureSet;
				DoseAccessorPointer _doseAccessor;
				bool _strict;
			};
		}
	}
}
#endif