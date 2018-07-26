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

#ifndef __GENERIC_MASKED_DOSE_ITERATOR_NEW_H
#define __GENERIC_MASKED_DOSE_ITERATOR_NEW_H

#include <boost/shared_ptr.hpp>

#include "rttbBaseType.h"
#include "rttbMaskedDoseIteratorInterface.h"
#include "rttbMaskAccessorInterface.h"

#include "RTTBCoreExports.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

namespace rttb
{
	namespace core
	{
		/*! @class GenericMaskedDoseIterator
			@brief This is a templated class representing a generic masked dose iterator for a VoxelizationPolicy.
			@see testing/GenericMaskedDoseIteratorTest.cpp for more information.
		*/
        class RTTBCore_EXPORT GenericMaskedDoseIterator : public MaskedDoseIteratorInterface
		{
		public:
			using MaskVoxelListPointer = MaskAccessorInterface::MaskVoxelListPointer;
			using MaskVoxelList = MaskAccessorInterface::MaskVoxelList;
			using MaskAccessorPointer = MaskedDoseIteratorInterface::MaskAccessorPointer;
			using DoseAccessorPointer = MaskedDoseIteratorInterface::DoseAccessorPointer;

		private:

			using MaskVoxelListIterator = MaskVoxelList::const_iterator;

			/*! The current index position of the vector _maskVoxelVec*/
			MaskVoxelListIterator _currentMaskPos;

			/*! vector of MaskVoxel, as defined in the voxelization*/
			MaskVoxelListPointer _maskVoxelVec;

			/*! the volume in cm^3 of the current dose voxel*/
			DoseVoxelVolumeType _currentVoxelVolume;


		public:

			GenericMaskedDoseIterator(MaskAccessorPointer aSpMask, DoseAccessorPointer aDoseAccessor)
				: MaskedDoseIteratorInterface(aSpMask, aDoseAccessor) {};

			/*! @brief Set the position on the first index. Use also as initialization.
			*/
			bool reset() override;

			/*! move to next mask position. The validity of the position is not checked here.
			*/
			void next() override;

			/*! @brief Volume of one voxel (in cm3)
				@exception InvalidParameterException  if a inhomogeneous grid is defined in the dose accessors, because
				these grids are currently not supported.
			*/
			DoseVoxelVolumeType getCurrentVoxelVolume() const override;

			FractionType getCurrentRelevantVolumeFraction() const override;

			inline MaskVoxelListPointer getMaskVoxelVec() const
			{
				return _maskVoxelVec;
			};

			/*! Check first if the position inside the maskedVoxelVector is valid. If so, check if the gridID at the
				current position in the MaskedVoxelVector is valid in the dose and mask grid.
			*/
			bool isPositionValid() const override;

			/*! @brief get current VoxelGridID (on dose voxel grid)*/
			VoxelGridID getCurrentVoxelGridID() const override;

			/*! @return current dose value multiplied by current volume fraction*/
			DoseTypeGy getCurrentMaskedDoseValue() const override;

			/*! @return current dose value without masking*/
			DoseTypeGy getCurrentDoseValue() const override;
		};
	}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
