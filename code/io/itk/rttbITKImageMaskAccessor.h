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
#ifndef __ITK_IMAGE_MASK_ACCESSOR_H
#define __ITK_IMAGE_MASK_ACCESSOR_H

#include "rttbMaskAccessorInterface.h"
#include "rttbGeometricInfo.h"
#include "rttbBaseType.h"

#include "itkImage.h"

namespace rttb
{
	namespace io
	{
		namespace itk
		{

			/*! @class ITKImageMaskAccessor
			@brief This class gives access to mask information stored in an itk image
			*/
			class ITKImageMaskAccessor: public core::MaskAccessorInterface
			{
			public:
				typedef ::itk::Image<DoseTypeGy, 3> ITKMaskImageType;
				typedef ::itk::ImageBase<3> ITKImageBaseType;
				typedef core::MaskAccessorInterface::MaskVoxelList MaskVoxelList;
				typedef core::MaskAccessorInterface::MaskVoxelListPointer MaskVoxelListPointer;

				typedef boost::shared_ptr<core::GeometricInfo> GeometricInfoPointer;

			private:

				/** @brief The mask as itkImage */
				ITKMaskImageType::ConstPointer _mask;

				IDType _maskUID;

				GeometricInfoPointer _geoInfo;

				/*! vector containing list of mask voxels*/
				MaskVoxelListPointer _relevantVoxelVector;

				/*! @brief get all required data from the itk image contained in _Mask
					@exception InvalidDoseException if PixelSpacing is 0 or size in any dimension is 0.
				*/
				bool assembleGeometricInfo();


			public:

				~ITKImageMaskAccessor();

				ITKImageMaskAccessor(ITKMaskImageType::ConstPointer aMaskImage);

				/*! @brief voxelization of the given structures according to the original RTToolbox algorithm*/
				void updateMask();

				/*! @brief get vector conatining al relevant voxels that are inside the given structure*/
				MaskVoxelListPointer getRelevantVoxelVector();
				/*! @brief get vector conatining al relevant voxels that have a relevant volume above the given threshold and are inside the given structure*/
				MaskVoxelListPointer getRelevantVoxelVector(float lowerThreshold);

				/*!@brief determine how a given voxel on the dose grid is masked
				* @param aID ID of the voxel in grid.
				* @param voxel Reference to the voxel.
				* @post after a valid call voxel contains the information of the specified grid voxel. If aID is not valid, voxel values are undefined.
				* The relevant volume fraction will be set to zero.
				* @return Indicates of the voxel exists and therefore if parameter voxel contains valid values.*/
				bool getMaskAt(const VoxelGridID aID, core::MaskVoxel& voxel) const;

				bool getMaskAt(const VoxelGridIndex3D& aIndex, core::MaskVoxel& voxel) const;

				/*! @brief give access to GeometricInfo*/
				const core::GeometricInfo& getGeometricInfo() const;

				/* @ brief is true if Mask is on a homogeneous grid */
				// Inhomogeneous grids are not supported at the moment, but if they will
				// be supported in the future the interface does not need to change.
				bool isGridHomogeneous() const
				{
					return true;
				};

				IDType getMaskUID() const
				{
					return _maskUID;
				};

			};

		}
	}
}

#endif
