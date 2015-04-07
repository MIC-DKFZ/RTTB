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

#include "rttbMaskVoxel.h"
#include "rttbInvalidParameterException.h"

namespace rttb{
	namespace core{

		MaskVoxel::MaskVoxel(const rttb::VoxelGridID &aVoxelGridID){
			if(aVoxelGridID<0)
				{
				std::cout << aVoxelGridID<<std::endl;
				throw InvalidParameterException("VoxelGridID is not valid!");
				}		
			else
				{
				_voxelGridID=aVoxelGridID;
				_volumeFraction=1;
				}
			}

		MaskVoxel::MaskVoxel(const rttb::VoxelGridID &aVoxelGridID, FractionType aVolumeFraction){
			if(aVoxelGridID<0)
				{
				std::cout << aVoxelGridID<<std::endl;
				throw InvalidParameterException("VoxelGridID is not valid!");
				}
			else if(aVolumeFraction<0 || aVolumeFraction>1)
				{
				std::cout << aVolumeFraction<<std::endl;
				throw InvalidParameterException("Volume fraction needs to be between 0 and 1!");
				}			
			else
				{
				_voxelGridID=aVoxelGridID;
				_volumeFraction=aVolumeFraction;
				}
			}

		bool MaskVoxel::operator==(const MaskVoxel& voxel) const{ 
			return ( (_voxelGridID==voxel.getVoxelGridID()) && (_volumeFraction==voxel.getRelevantVolumeFraction()) );
			}

		bool MaskVoxel::operator<(const MaskVoxel& maskVoxel) const{
			return (_voxelGridID < maskVoxel.getVoxelGridID());
		}

		const VoxelGridID& MaskVoxel::getVoxelGridID() const{
			return _voxelGridID;
			}

		void MaskVoxel::setRelevantVolumeFraction(FractionType aVolumeFraction){
			if(aVolumeFraction<0 || aVolumeFraction>1)
				{
				std::cout << aVolumeFraction<<std::endl;
				throw InvalidParameterException("Volume fraction needs to be between 0 and 1!");
				}			
			_volumeFraction=aVolumeFraction;
			}

		FractionType MaskVoxel::getRelevantVolumeFraction() const{
			return _volumeFraction;
			}

		}//end namespace core
}//end namespace rttb
