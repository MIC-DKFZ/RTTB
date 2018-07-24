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

#include "rttbITKTransformation.h"
#include "rttbNullPointerException.h"

namespace rttb
{
	namespace interpolation
	{
		ITKTransformation::ITKTransformation(const Transform3D3DType* aTransformation):
			_pTransformation(aTransformation)
		{
			//handle null pointer
			if (aTransformation == nullptr)
			{
				throw core::NullPointerException("Pointer to registration is nullptr.");
			}

		}

		void ITKTransformation::convert(const WorldCoordinate3D& aWorldCoordinate,
		                                InputPointType& aInputPoint) const
		{
			assert(aWorldCoordinate.size() == 3);
			assert(aInputPoint.Length == 3);

			for (unsigned int i = 0; i < aInputPoint.Length; ++i)
			{
				aInputPoint[i] = aWorldCoordinate[i];
			}
		}

		void ITKTransformation::convert(const OutputPointType& aOutputPoint,
		                                WorldCoordinate3D& aWorldCoordinate) const
		{
			assert(aWorldCoordinate.size() == 3);
			assert(aOutputPoint.Length == 3);

			for (unsigned int i = 0; i < aOutputPoint.Length; ++i)
			{
				aWorldCoordinate[i] = aOutputPoint[i];
			}
		}

		bool ITKTransformation::transformInverse(const WorldCoordinate3D&
		        worldCoordinateTarget, WorldCoordinate3D& worldCoordinateMoving) const
		{
			InputPointType aTargetPoint;
			OutputPointType aMovingPoint;

			convert(worldCoordinateTarget, aTargetPoint);
			aMovingPoint = _pTransformation->TransformPoint(aTargetPoint);
			convert(aMovingPoint, worldCoordinateMoving);
			//TransformPoint has no return value...
			return true;
		}

		bool ITKTransformation::transform(const WorldCoordinate3D&
		                                  worldCoordinateMoving, WorldCoordinate3D& worldCoordinateTarget) const
		{
			OutputPointType aTargetPoint;
			InputPointType aMovingPoint;

			convert(worldCoordinateMoving, aMovingPoint);
			aTargetPoint = _pTransformation->TransformPoint(aMovingPoint);
			convert(aTargetPoint, worldCoordinateTarget);
			//TransformPoint has no return value...
			return true;
		}

	}
}
