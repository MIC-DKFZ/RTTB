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
// @version $Revision: 1127 $ (last changed revision)
// @date    $Date: 2015-10-01 13:33:33 +0200 (Do, 01 Okt 2015) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/


#ifndef __BOOST_MASK_VOXELIZATION_THREAD_H
#define __BOOST_MASK_VOXELIZATION_THREAD_H

#include "rttbBaseType.h"
#include "rttbGeometricInfo.h"
#include "rttbMaskVoxel.h"

#include <boost/multi_array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lockfree/queue.hpp>

namespace rttb
{
	namespace masks
	{
		namespace boostRedesign
		{
			/*! @class BoostMaskGenerateMaskVoxelListThread
			*
			*/
			class BoostMaskVoxelizationThread
			{

			public:


				BoostMaskVoxelizationThread();


				void operator()()
				{

					/*for (it = _geometryCoordinateBoostPolygonMap.begin();
					     it != _geometryCoordinateBoostPolygonMap.end(); ++it)
					{
						BoostArray2D maskArray(boost::extents[globalBoundingBoxSize0][globalBoundingBoxSize1]);

						BoostPolygonVector boostPolygonVec = (*it).second;

						for (unsigned int x = 0; x < globalBoundingBoxSize0; ++x)
						{
							for (unsigned int y = 0; y < globalBoundingBoxSize1; ++y)
							{
								rttb::VoxelGridIndex3D currentIndex;
								currentIndex[0] = x + minIndex[0];
								currentIndex[1] = y + minIndex[1];
								currentIndex[2] = 0;

								//Get intersection polygons of the dose voxel and the structure
								BoostPolygonDeque polygons = getIntersections(currentIndex, boostPolygonVec);

								//Calc areas of all intersection polygons
								double volumeFraction = calcArea(polygons);

								if (volumeFraction > 1 && (volumeFraction - 1) <= errorConstant)
								{
									volumeFraction = 1;
								}
								else if (volumeFraction < 0 || (volumeFraction - 1) > errorConstant)
								{
									throw rttb::core::InvalidParameterException("Mask calculation failed! The volume fraction should >= 0 and <= 1!");
								}

								maskArray[x][y] = volumeFraction;
							}
						}

						//insert into voxelization map
						_voxelizationMap.insert(std::pair<double, BoostArray2D>((*it).first, maskArray));
					}*/
				}

			private:
				typedef ::boost::geometry::model::polygon< ::boost::geometry::model::d2::point_xy<double> >
				BoostPolygon2D;
				typedef std::vector<BoostPolygon2D> BoostPolygonVector;//polygon with or without holes
				typedef std::map<double, BoostPolygonVector>
				BoostPolygonMap;//map of the z index with the vector of boost 2d polygon

				BoostPolygonMap _geometryCoordinateBoostPolygonMap;
			};

		}


	}
}

#endif