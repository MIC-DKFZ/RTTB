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

#include "rttbOTBMaskAccessor.h"
#include "rttbMappingOutsideOfImageException.h"

#include <boost/make_shared.hpp>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace rttb
{

	namespace masks
	{
		namespace legacy
		{

			OTBMaskAccessor::OTBMaskAccessor(StructTypePointer aStructurePointer,
				const core::GeometricInfo& aGeometricInfo)
				: _spStructure(aStructurePointer), _legacyStructure(*aStructurePointer)
			{
				_spRelevantVoxelVector = MaskVoxelListPointer();
				_geoInfo = aGeometricInfo;

				//generate new structure set uid
				boost::uuids::uuid id;
				boost::uuids::random_generator generator;
				id = generator();
				std::stringstream ss;
				ss << id;
				_maskUID = "OTBMask_" + ss.str();
			}


			OTBMaskAccessor::~OTBMaskAccessor()
			{
			};

			void OTBMaskAccessor::updateMask()
			{
				MaskVoxelList newRelevantVoxelVector;

				if (_spRelevantVoxelVector)
				{
					return; // already calculated
				}


				legacy::Mask legacyMask(&_geoInfo , &_legacyStructure);

				//translate result to expected values
				const std::vector<legacy::DoseVoxel> voxelsInStruct = legacyMask.getDoseVoxelInStructure();

				for (std::vector<legacy::DoseVoxel>::const_iterator it = voxelsInStruct.begin();
					it != voxelsInStruct.end(); ++it)
				{
					rttb::VoxelGridID aVoxelGridID;
					rttb::VoxelGridIndex3D newIndex;

					legacy::LegacyDoseVoxelIndex3D oldIndex = it->getVoxelIndex3D();
					newIndex(0) = oldIndex.x;
					newIndex(1) = oldIndex.y;
					newIndex(2) = oldIndex.z;

					// new architecture
					if (!_geoInfo.convert(newIndex, aVoxelGridID))
					{
						throw core::MappingOutsideOfImageException("mapping outside of image occurred!");
					}

					core::MaskVoxel newMaskVoxel(aVoxelGridID);
					newMaskVoxel.setRelevantVolumeFraction(it->getProportionInStr());

					newRelevantVoxelVector.push_back(newMaskVoxel);
				}

				_spRelevantVoxelVector = boost::make_shared<MaskVoxelList>(newRelevantVoxelVector);
				return;
			}

			OTBMaskAccessor::MaskVoxelListPointer OTBMaskAccessor::getRelevantVoxelVector()
			{
				// if not already generated start voxelization here
				updateMask();
				return _spRelevantVoxelVector;
			}

			OTBMaskAccessor::MaskVoxelListPointer OTBMaskAccessor::getRelevantVoxelVector(float lowerThreshold)
			{
				MaskVoxelListPointer filteredVoxelVectorPointer(new MaskVoxelList);
				updateMask();
				// filter relevant voxels
				OTBMaskAccessor::MaskVoxelList::iterator it = _spRelevantVoxelVector->begin();

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

			bool OTBMaskAccessor::getMaskAt(VoxelGridID aID, core::MaskVoxel& voxel) const
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
					OTBMaskAccessor::MaskVoxelList::iterator it = _spRelevantVoxelVector->begin();

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
				// returns false if mask was not calculated without triggering calculation (otherwise not const!)
				else
				{
					return false;
				}

				return false;

			}

			bool OTBMaskAccessor::getMaskAt(const VoxelGridIndex3D& aIndex, core::MaskVoxel& voxel) const
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
		}
	}
}