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
#ifndef __MAPPABLE_DOSE_ACCESSOR_BASE_H
#define __MAPPABLE_DOSE_ACCESSOR_BASE_H

#include <boost/shared_ptr.hpp>

#include "rttbDoseAccessorInterface.h"
#include "rttbGeometricInfo.h"
#include "rttbBaseType.h"
#include "rttbTransformationInterface.h"
#include "rttbNullPointerException.h"

namespace rttb
{
	namespace interpolation
	{

		/*! @class MappableDoseAccessorInterface
		@brief Interface for dealing with dose information that has to be transformed into another geometry than the original dose image
		@details implementation of strategy is done by derived class (e.g. SimpleMappableDoseAccessor or RosuMappableDoseAccessor. Transformation is defined in TransformationInterface
		@ingroup interpolation
		*/
		class MappableDoseAccessorInterface: public core::DoseAccessorInterface
		{
		public:
			typedef boost::shared_ptr<MappableDoseAccessorInterface> Pointer;
		protected:
			DoseAccessorPointer _spOriginalDoseDataMovingImage;
			TransformationInterface::Pointer _spTransformation;

			core::GeometricInfo _geoInfoTargetImage;
			bool _acceptPadding;
			DoseTypeGy _defaultOutsideValue;

		public:
			/*! @brief Constructor.
				@param geoInfoTargetImage target image geometry
				@param doseMovingImage dose of moving image
				@param aTransformation the transformation
				@param acceptPadding is mapping outside the image allowed
				@param defaultOutsideValue the default outside voxel value if accepptPadding=true
				@pre all input parameters have to be valid
				@exception core::NullPointerException if one input parameter is NULL
			*/
			MappableDoseAccessorInterface(const core::GeometricInfo& geoInfoTargetImage,
			                              const DoseAccessorPointer doseMovingImage, const TransformationInterface::Pointer aTransformation,
			                              bool acceptPadding = true,
			                              DoseTypeGy defaultOutsideValue = 0.0): _spOriginalDoseDataMovingImage(doseMovingImage),
				_spTransformation(aTransformation), _geoInfoTargetImage(geoInfoTargetImage),
				_acceptPadding(acceptPadding), _defaultOutsideValue(defaultOutsideValue)
			{
				//handle null pointers
				if (doseMovingImage == NULL || aTransformation == NULL)
				{
					throw core::NullPointerException("Pointers to input accessors/transformation cannot be NULL.");
				}
			}

			/*! @brief Virtual destructor of base class
			*/
			virtual ~MappableDoseAccessorInterface() {};

			inline const core::GeometricInfo& getGeometricInfo() const
			{
				return _geoInfoTargetImage;
			};

			inline GridSizeType getGridSize() const
			{
				return _geoInfoTargetImage.getNumberOfVoxels();
			};

			/*! @brief Returns the dose for a given VoxelGridID (convenience function that handles conversion VoxelGridID->VoxelGridIndex3D)
				@sa getDoseAt(const VoxelGridIndex3D& aIndex)
			*/
			DoseTypeGy getDoseAt(const VoxelGridID aID) const = 0;

			/*! @brief Returns the dose for a given VoxelGridIndex3D
			*/
			virtual DoseTypeGy getDoseAt(const VoxelGridIndex3D& aIndex) const = 0 ;

			const IDType getDoseUID() const
			{
				return _spOriginalDoseDataMovingImage->getDoseUID();
			};
		};
	}
}

#endif
