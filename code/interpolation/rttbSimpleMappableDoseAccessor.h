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
#ifndef __SIMPLE_MAPPABLE_DOSE_ACCESSOR_H
#define __SIMPLE_MAPPABLE_DOSE_ACCESSOR_H

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "rttbBaseType.h"
#include "rttbInterpolationBase.h"
#include "rttbLinearInterpolation.h"
#include "rttbTransformationInterface.h"
#include "rttbMappableDoseAccessorInterface.h"

#include "RTTBInterpolationExports.h"

namespace rttb
{
	namespace interpolation
	{

		/*! @class SimpleMappableDoseAccessor
		@brief Class for dose mapping based on simple trilinear interpolation
		@ingroup interpolation
		*/
        class RTTBInterpolation_EXPORT SimpleMappableDoseAccessor : public MappableDoseAccessorInterface
		{
		private:
			InterpolationBase::Pointer _spInterpolation;
		public:
			typedef boost::shared_ptr<SimpleMappableDoseAccessor> Pointer;

			/*! @brief Constructor. Just hands values over to base class constructor.
				@param aInterpolation the used interpolation.
				@sa MappableDoseAccessorBase
			*/
			SimpleMappableDoseAccessor(const core::GeometricInfo& geoInfoTargetImage,
			                           const DoseAccessorPointer doseMovingImage,
			                           const TransformationInterface::Pointer aTransformation,
			                           const InterpolationBase::Pointer aInterpolation = ::boost::make_shared<LinearInterpolation>(),
			                           bool acceptPadding = true,
			                           DoseTypeGy defaultOutsideValue = 0.0);

			/*! @brief Virtual destructor of class
			*/
			virtual ~SimpleMappableDoseAccessor() {};
			/*! @brief Returns the dose for a given voxel grid id. Plain trilinear interpolation is performed.
				@sa getDoseAt(const VoxelGridIndex3D& aIndex)
			*/
			GenericValueType getValueAt(const VoxelGridID aID) const;

			/*! @brief Returns the dose for a given voxel grid index. Plain trilinear interpolation is performed.
				@return the dose or if (isOutside==true && _acceptPadding==true) then _defaultValue
				@exception core::MappingOutsideOfImageException if the point is mapped outside and if _acceptPadding==false, possibly returning _defaultValue)
			*/
			GenericValueType getValueAt(const VoxelGridIndex3D& aIndex) const;
		};
	}
}

#endif
