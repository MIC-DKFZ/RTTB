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

#include "rttbGenericDoseIterator.h"
#include "rttbInvalidParameterException.h"

namespace rttb
{
	namespace core
	{

		GenericDoseIterator::GenericDoseIterator(DoseAccessorPointer aDoseAccessor) : DoseIteratorInterface(
			    aDoseAccessor)
		{
			_currentDoseVoxelGridID = 0;
			_currentVoxelVolume = 0;
		}

		bool GenericDoseIterator::reset()
		{
			_currentDoseVoxelGridID = 0;

			if (_spDoseAccessor->isGridHomogeneous())
			{
				SpacingVectorType3D voxelSizeVec = (_spDoseAccessor->getGeometricInfo()).getSpacing();
				_currentVoxelVolume = voxelSizeVec(0) * voxelSizeVec(1) * voxelSizeVec(2) / 1000;
			}
			else
			{
				_currentVoxelVolume = 0;
				throw InvalidParameterException("Inhomogeneous grids are currently not supported!");
			}

			return true;
		}

		void GenericDoseIterator::next()
		{
			if (_spDoseAccessor->getGeometricInfo().getNumberOfVoxels() > _currentDoseVoxelGridID)
			{
				++_currentDoseVoxelGridID;
			}
		}

		bool GenericDoseIterator::isPositionValid() const
		{
			return _spDoseAccessor->getGeometricInfo().validID(_currentDoseVoxelGridID);
		}

		DoseVoxelVolumeType GenericDoseIterator::getCurrentVoxelVolume() const
		{
			if (_spDoseAccessor->isGridHomogeneous())
			{
				return _currentVoxelVolume;
			}
			else
			{
				throw InvalidParameterException("Inhomogeneous grids are currently not supported!");
			}
		}

		DoseTypeGy GenericDoseIterator::getCurrentDoseValue() const
		{
			if (isPositionValid())
			{
				return _spDoseAccessor->getValueAt(_currentDoseVoxelGridID);
			}
			else
			{
				return 0;
			}
		}

	}//end: namespace core
}//end: namespace rttb