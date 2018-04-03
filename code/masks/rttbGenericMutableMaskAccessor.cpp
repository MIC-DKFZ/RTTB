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

#include "rttbGenericMutableMaskAccessor.h"
#include "rttbMaskVoxel.h"
#include "rttbNullPointerException.h"

#include <boost/make_shared.hpp>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace rttb
{

	namespace masks
	{


		GenericMutableMaskAccessor::GenericMutableMaskAccessor(const core::GeometricInfo& aGeometricInfo) :
			_geoInfo(aGeometricInfo), _spRelevantVoxelVector(MaskVoxelListPointer())
		{
			//generate new structure set uid
			boost::uuids::uuid id;
			boost::uuids::random_generator generator;
			id = generator();
			std::stringstream ss;
			ss << id;
			_maskUID = "GenericMutableMask_" + ss.str();
		}

		GenericMutableMaskAccessor::~GenericMutableMaskAccessor() = default;

		void GenericMutableMaskAccessor::updateMask() {}

		GenericMutableMaskAccessor::MaskVoxelListPointer
		GenericMutableMaskAccessor::getRelevantVoxelVector()
		{
			return _spRelevantVoxelVector;
		}

		GenericMutableMaskAccessor::MaskVoxelListPointer GenericMutableMaskAccessor::getRelevantVoxelVector(
		    float lowerThreshold)
		{
      auto filteredVoxelVectorPointer = boost::make_shared<MaskVoxelList>();
			// filter relevant voxels
			auto it = _spRelevantVoxelVector->begin();

			while (it != _spRelevantVoxelVector->end())
			{
				if ((*it).getRelevantVolumeFraction() > lowerThreshold)
				{
					filteredVoxelVectorPointer->push_back(*it);
				}

				++it;
			}

			// if mask calculation was not successful this is empty!
			return filteredVoxelVectorPointer;
		}

		bool GenericMutableMaskAccessor::getMaskAt(VoxelGridID aID, core::MaskVoxel& voxel) const
		{
			//initialize return voxel
			voxel.setRelevantVolumeFraction(0);

			//check if ID is valid
			if (!_geoInfo.validID(aID))
			{
				return false;
			}

			//determine how a given voxel on the dose grid is masked
			if (_spRelevantVoxelVector)
			{
				auto it = _spRelevantVoxelVector->begin();

				while (it != _spRelevantVoxelVector->end())
				{
					if ((*it).getVoxelGridID() == aID)
					{
						voxel = (*it);
						return true;
					}

					++it;
				}

				//aID is not in mask
				voxel.setRelevantVolumeFraction(0);
			}

			return false;
		}

		bool GenericMutableMaskAccessor::getMaskAt(const VoxelGridIndex3D& aIndex,
		        core::MaskVoxel& voxel) const
		{
			//convert VoxelGridIndex3D to VoxelGridID
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

		void GenericMutableMaskAccessor::setMaskAt(VoxelGridID aID, const core::MaskVoxel& voxel)
		{
			//check if ID is valid
			if (!_geoInfo.validID(aID))
			{
				return;
			}

			//determine how a given voxel on the dose grid is masked
			if (_spRelevantVoxelVector)
			{
				auto it = _spRelevantVoxelVector->begin();

				while (it != _spRelevantVoxelVector->end())
				{
					if ((*it).getVoxelGridID() == aID)
					{
						(*it) = voxel;
						return;
					}

					++it;
				}

				//not sID is not found in existing voxels
				_spRelevantVoxelVector->push_back(voxel);
			}
		}

		void GenericMutableMaskAccessor::setMaskAt(const VoxelGridIndex3D& aIndex,
		        const core::MaskVoxel& voxel)
		{
			//convert VoxelGridIndex3D to VoxelGridID
			VoxelGridID aVoxelGridID;

			if (_geoInfo.convert(aIndex, aVoxelGridID))
			{
				setMaskAt(aVoxelGridID, voxel);
			}
		}

		void GenericMutableMaskAccessor::setRelevantVoxelVector(MaskVoxelListPointer aVoxelListPointer)
		{
			_spRelevantVoxelVector = MaskVoxelListPointer(aVoxelListPointer);
		}

	}
}