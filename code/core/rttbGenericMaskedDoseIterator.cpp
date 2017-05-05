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

#include "rttbGenericMaskedDoseIterator.h"
#include "rttbInvalidParameterException.h"

namespace rttb
{
	namespace core
	{

		bool GenericMaskedDoseIterator::reset()
		{
			_maskVoxelVec = _spMask->getRelevantVoxelVector();
			_currentMaskPos = _maskVoxelVec->begin();

			core::GeometricInfo geoInfo = _spDoseAccessor->getGeometricInfo();
			_currentVoxelVolume = geoInfo.getSpacing()(0) * geoInfo.getSpacing()(1) * geoInfo.getSpacing()(
			                          2) / 1000;

			return true;
		}

		void GenericMaskedDoseIterator::next()
		{
			++_currentMaskPos;
		}

		DoseVoxelVolumeType GenericMaskedDoseIterator::getCurrentVoxelVolume() const
		{
			if (_spDoseAccessor->isGridHomogeneous())
			{
				return _currentVoxelVolume;
			}
			else
			{
				throw InvalidParameterException("Inhomogeneous grids are currently not supported! ");
			}
		}

		FractionType GenericMaskedDoseIterator::getCurrentRelevantVolumeFraction() const
		{
			if (!(_currentMaskPos == _maskVoxelVec->end()))
			{
				assert(_spMask->getGeometricInfo().validID(_currentMaskPos->getVoxelGridID()));
				return _currentMaskPos->getProportionInStr();
			}

			return 0;
		}

		bool GenericMaskedDoseIterator::isPositionValid() const
		{
			if (_currentMaskPos == _maskVoxelVec->end())
			{
				return false;
			}

			return _spDoseAccessor->getGeometricInfo().validID(_currentMaskPos->getVoxelGridID()) &&
			       _spMask->getGeometricInfo().validID(_currentMaskPos->getVoxelGridID());
		}

		VoxelGridID GenericMaskedDoseIterator::getCurrentVoxelGridID() const
		{
			return _currentMaskPos->getVoxelGridID();
		}


		DoseTypeGy GenericMaskedDoseIterator::getCurrentMaskedDoseValue() const
		{
			assert(isPositionValid());
			return getCurrentDoseValue() * getCurrentRelevantVolumeFraction();
		}

		DoseTypeGy GenericMaskedDoseIterator::getCurrentDoseValue() const
		{
			assert(_spDoseAccessor->getGeometricInfo().validID(_currentMaskPos->getVoxelGridID()));
			return _spDoseAccessor->getValueAt(_currentMaskPos->getVoxelGridID());
		}

	}//end namespace core
}//end namespace rttb