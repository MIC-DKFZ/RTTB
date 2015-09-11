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

#ifndef __NEAREST_NEIGHBOR_INTERPOLATION_H
#define __NEAREST_NEIGHBOR_INTERPOLATION_H

#include "rttbInterpolationBase.h"

namespace rttb
{
	namespace interpolation
	{

		/*! @class NearestNeighborInterpolation
			@brief Nearest Neighbor interpolation
			@ingroup interpolation
		*/
		class NearestNeighborInterpolation : public InterpolationBase
		{
		public:
			/*! @brief Constructor
			*/
			NearestNeighborInterpolation() {};

			/*! @brief Returns the interpolated value (the nearest voxel value given by _spOriginalData->getGeometricInfo().worldCoordinateToIndex())
			*/
			DoseTypeGy getValue(const WorldCoordinate3D& aWorldCoordinate) const;
		};

	}
}

#endif
