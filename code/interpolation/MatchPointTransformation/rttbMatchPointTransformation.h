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
#ifndef __MATCHPOINT_TRANSFORMATION_H
#define __MATCHPOINT_TRANSFORMATION_H

#include "mapRegistration.h"

#include "rttbTransformationInterface.h"

#include "RTTBInterpolationMatchPointTransformationExports.h"

namespace rttb
{
	namespace interpolation
	{
		/*! @class MatchPointTransformation
		@brief This class can deal with dose information that has to be transformed into another geometry than the original dose image (transformation specified by MatchPoint registration object).
		@ingroup interpolation
		*/
        class RTTBInterpolationMatchPointTransformation_EXPORT MatchPointTransformation : public TransformationInterface
		{
		public:
			static const unsigned int TargetDimension3D = 3;
			static const unsigned int MovingDimension3D = 3;
			typedef map::core::Registration<MovingDimension3D, TargetDimension3D> Registration3D3DType;
			typedef map::core::Registration<MovingDimension3D, TargetDimension3D>::MovingPointType
			MovingPointType;
			typedef map::core::Registration<MovingDimension3D, TargetDimension3D>::TargetPointType
			TargetPointType;

			/*! @brief Constructor.
				@param aRegistration registration given in MatchPoint format (note the use of pointer since itkSmartPointer does not support inheritance)
				@pre all input parameters have to be valid
				@exception core::NullPointerException if one input parameter is nullptr
				@exception core::PaddingException if the transformation is undefined and if _acceptPadding==false
			*/
			MatchPointTransformation(const Registration3D3DType* aRegistration);

			~MatchPointTransformation() override = default;;

			/*! @brief performs a transformation targetImage --> movingImage
			*/
			bool transformInverse(const WorldCoordinate3D& worldCoordinateTarget,
			                      WorldCoordinate3D& worldCoordinateMoving) const override;

			/*! @brief performs a transformation movingImage --> targetImage
			*/
			bool transform(const WorldCoordinate3D& worldCoordinateMoving,
			               WorldCoordinate3D& worldCoordinateTarget) const override;

		protected:
			void convert(const WorldCoordinate3D& aWorldCoordinate, TargetPointType& aTargetPoint) const;
			void convert(const MovingPointType& aMovingPoint, WorldCoordinate3D& aWorldCoordinate) const;

		private:
			//! Has to be a Pointer type because of inheritance issues with itkSmartPointer (that doesn't recognize the inheritance)
			const Registration3D3DType* _pRegistration;
		};
	}
}

#endif
