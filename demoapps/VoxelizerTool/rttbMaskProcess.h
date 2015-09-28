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
// @version $Revision: 5506 $ (last changed revision)
// @date    $Date: 2015-07-30 14:45:24 +0200 (Do, 30 Jul 2015) $ (last change date)
// @author  $Author: strubel $ (last changed by)
*/
#ifndef __MaskProcess_h
#define __MaskProcess_h

#include "rttbDicomFileStructureSetGenerator.h"
#include "rttbOTBMaskAccessor.h"

namespace rttb
{
	namespace apps
	{
		namespace voxelizer
		{
			/*! @class MaskProcess
			@brief MaskProcess create a Mask with the struct and reference object
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
				            bool legacyVoxelization);
				/**@brief create a mask with _rtStructureSet and _doseAccessor object.
				@return a mask object
				*/
				MaskAccessorPointer createMask(unsigned int numberOfStructure) const;
			private:
				StructureSetPointer _rtStructureSet;
				DoseAccessorPointer _doseAccessor;
				bool _legacyVoxelization;
			};
		}
	}
}
#endif