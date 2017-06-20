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

#include <boost/make_shared.hpp>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "DummyMaskAccessor.h"
#include "rttbNullPointerException.h"

namespace rttb
{
	namespace testing
	{

		DummyMaskAccessor::DummyMaskAccessor(const core::GeometricInfo& aGeometricInfo)
		{
			_spRelevantVoxelVector = MaskVoxelListPointer();
			_geoInfo = aGeometricInfo;

			boost::uuids::uuid id;
			boost::uuids::random_generator generator;
			id = generator();
			std::stringstream ss;
			ss << id;
			_maskUID = "DummyMask_" + ss.str();
		}

		DummyMaskAccessor::DummyMaskAccessor(const core::GeometricInfo& aGeometricInfo,
		                                     MaskVoxelListPointer voxelListPtr)
		{
			_spRelevantVoxelVector = voxelListPtr;
			_geoInfo = aGeometricInfo;

			boost::uuids::uuid id;
			boost::uuids::random_generator generator;
			id = generator();
			std::stringstream ss;
			ss << id;
			_maskUID = "DummyMask_" + ss.str();
		}

		void DummyMaskAccessor::updateMask()
		{
			MaskVoxelList newRelevantVoxelVector;

			if (_spRelevantVoxelVector)
			{
				return;
			}

			for (int i = 0; i < _geoInfo.getNumberOfVoxels(); i++)
			{

				if ((double(rand()) / RAND_MAX) > 0.5)
				{
					core::MaskVoxel newMaskVoxel(i);
					newMaskVoxel.setRelevantVolumeFraction((double(rand()) / RAND_MAX));

					newRelevantVoxelVector.push_back(newMaskVoxel);
				}
			}

			_spRelevantVoxelVector = boost::make_shared<MaskVoxelList>(newRelevantVoxelVector);
			return;
		}

		DummyMaskAccessor::MaskVoxelListPointer DummyMaskAccessor::getRelevantVoxelVector()
		{
			updateMask();
			return _spRelevantVoxelVector;
		}

		DummyMaskAccessor::MaskVoxelListPointer DummyMaskAccessor::getRelevantVoxelVector(
		    float lowerThreshold)
		{
      auto filteredVoxelVectorPointer = boost::make_shared<MaskVoxelList>();
			updateMask();
			DummyMaskAccessor::MaskVoxelList::iterator it = _spRelevantVoxelVector->begin();

			while (it != _spRelevantVoxelVector->end())
			{
				if ((*it).getRelevantVolumeFraction() > lowerThreshold)
				{
					filteredVoxelVectorPointer->push_back(*it);
				}

				++it;
			}

			return filteredVoxelVectorPointer;
		}

		bool DummyMaskAccessor::getMaskAt(VoxelGridID aID, core::MaskVoxel& voxel) const
		{
			voxel.setRelevantVolumeFraction(0);

			if (!_geoInfo.validID(aID))
			{
				return false;
			}

			if (_spRelevantVoxelVector)
			{
				DummyMaskAccessor::MaskVoxelList::iterator it = _spRelevantVoxelVector->begin();

				while (it != _spRelevantVoxelVector->end())
				{
					if ((*it).getVoxelGridID() == aID)
					{
						voxel = (*it);
						return true;
					}

					++it;
				}
			}
			else
			{
				return false;
			}

			return false;

		}

		bool DummyMaskAccessor::getMaskAt(const VoxelGridIndex3D& aIndex, core::MaskVoxel& voxel) const
		{
			VoxelGridID aVoxelGridID;

			if (_geoInfo.convert(aIndex, aVoxelGridID))
			{
				return getMaskAt(aVoxelGridID, voxel);
			}
			else
			{
				return false;
			}
		}

	}
}