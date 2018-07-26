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

#ifndef __MAPPABLE_DOSE_ACCESSOR_BASE_H
#define __MAPPABLE_DOSE_ACCESSOR_BASE_H

#include <rttbCommon.h>

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
      rttbClassMacro(MappableDoseAccessorInterface,core::DoseAccessorInterface)
		protected:
			core::DoseAccessorInterface::ConstPointer _spOriginalDoseDataMovingImage;
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
				@exception core::NullPointerException if one input parameter is nullptr
			*/
			MappableDoseAccessorInterface(const core::GeometricInfo& geoInfoTargetImage,
                                    core::DoseAccessorInterface::ConstPointer doseMovingImage, const TransformationInterface::Pointer aTransformation,
			                              bool acceptPadding = true,
			                              DoseTypeGy defaultOutsideValue = 0.0): _spOriginalDoseDataMovingImage(doseMovingImage),
				_spTransformation(aTransformation), _geoInfoTargetImage(geoInfoTargetImage),
				_acceptPadding(acceptPadding), _defaultOutsideValue(defaultOutsideValue)
			{
				//handle null pointers
				if (doseMovingImage == nullptr || aTransformation == nullptr)
				{
					throw core::NullPointerException("Pointers to input accessors/transformation cannot be nullptr.");
				}
			}

			/*! @brief Virtual destructor of base class
			*/
			~MappableDoseAccessorInterface() override = default;

			inline const core::GeometricInfo& getGeometricInfo() const override
			{
				return _geoInfoTargetImage;
			};

			inline GridSizeType getGridSize() const override
			{
				return _geoInfoTargetImage.getNumberOfVoxels();
			};

			const IDType getUID() const override
			{
				return _spOriginalDoseDataMovingImage->getUID();
			};
		};
	}
}

#endif
