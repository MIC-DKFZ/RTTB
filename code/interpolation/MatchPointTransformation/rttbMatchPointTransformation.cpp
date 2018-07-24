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

#include "rttbMatchPointTransformation.h"
#include "rttbNullPointerException.h"

namespace rttb
{
	namespace interpolation
	{
		MatchPointTransformation::MatchPointTransformation(
		    const Registration3D3DType* aRegistration): _pRegistration(aRegistration)
		{
			//handle null pointers
			if (aRegistration == nullptr)
			{
				throw core::NullPointerException("Pointer to registration is nullptr.");
			}
		}

		void MatchPointTransformation::convert(const WorldCoordinate3D& aWorldCoordinate,
		                                       TargetPointType& aTargetPoint) const
		{
			assert(aWorldCoordinate.size() == 3);
			assert(aTargetPoint.Length == 3);

			for (unsigned int i = 0; i < aTargetPoint.Length; ++i)
			{
				aTargetPoint[i] = aWorldCoordinate[i];
			}
		}

		void MatchPointTransformation::convert(const MovingPointType& aMovingPoint,
		                                       WorldCoordinate3D& aWorldCoordinate) const
		{
			assert(aWorldCoordinate.size() == 3);
			assert(aMovingPoint.Length == 3);

			for (unsigned int i = 0; i < aMovingPoint.Length; ++i)
			{
				aWorldCoordinate[i] = aMovingPoint[i];
			}
		}

		bool MatchPointTransformation::transformInverse(const WorldCoordinate3D&
		        worldCoordinateTarget, WorldCoordinate3D& worldCoordinateMoving) const
		{
			TargetPointType aTargetPoint;
			MovingPointType aMovingPoint;

			convert(worldCoordinateTarget, aTargetPoint);
			bool ok = _pRegistration->mapPointInverse(aTargetPoint, aMovingPoint);
			convert(aMovingPoint, worldCoordinateMoving);
			return ok;
		}

		bool MatchPointTransformation::transform(const WorldCoordinate3D& worldCoordinateMoving,
		        WorldCoordinate3D& worldCoordinateTarget) const
		{
			TargetPointType aTargetPoint;
			MovingPointType aMovingPoint;

			convert(worldCoordinateMoving, aMovingPoint);
			bool ok = _pRegistration->mapPoint(aMovingPoint, aTargetPoint);
			convert(aTargetPoint, worldCoordinateTarget);
			return ok;
		}

	}
}
