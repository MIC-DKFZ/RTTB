// -----------------------------------------------------------------------
// RTToolbox - DKFZ radiotherapy quantitative evaluation library
//
// Copyright (c) German Cancer Research Center (DKFZ),
// Software development for Integrated Diagnostics and Therapy (SIDT).
// ALL RIGHTS RESERVED.
// See rttbCopyright.txt or
// http://www.dkfz.de/en/sidt/projects/rttb/copyright.html [^]
//
// This software is distributed WITHOUT ANY WARRANTY; without even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE. See the above copyright notices for more information.
//
//------------------------------------------------------------------------
/*!
// @file
// @version $Revision: 3987 $ (last changed revision)
// @date $Date: 2012-10-08 09:50:17 +0200 (Mo, 08 Okt 2012) $ (last change date)
// @author $Author: mang $ (last changed by)
*/

#include "DummyTransformation.h"

namespace rttb
{
	namespace testing
	{
		bool DummyTransformation::transformInverse(const WorldCoordinate3D& worldCoordinateTarget,
		        WorldCoordinate3D& worldCoordinateMoving) const
		{
			worldCoordinateMoving = worldCoordinateTarget;
			return true;
		}

		bool DummyTransformation::transform(const WorldCoordinate3D& worldCoordinateMoving,
		                                    WorldCoordinate3D& worldCoordinateTarget) const
		{
			worldCoordinateTarget = worldCoordinateMoving;
			return true;
		}
	}
}
