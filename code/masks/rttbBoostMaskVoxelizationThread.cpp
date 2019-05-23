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

#include "rttbBoostMaskVoxelizationThread.h"

#include "rttbInvalidParameterException.h"

#include <boost/geometry.hpp>
#include <boost/make_shared.hpp>

namespace rttb
{
	namespace masks
	{
		namespace boost
		{
			BoostMaskVoxelizationThread::BoostMaskVoxelizationThread(const BoostPolygonMap& APolygonMap,
                const VoxelIndexVector& aGlobalBoundingBox, BoostArrayMapPointer anArrayMap, ::boost::shared_ptr<std::mutex> aMutex, bool strict) : _geometryCoordinateBoostPolygonMap(APolygonMap),
                _globalBoundingBox(aGlobalBoundingBox), _resultVoxelization(anArrayMap), _mutex(aMutex), _strict(strict)
			{
			}

			void BoostMaskVoxelizationThread::operator()()
			{
				rttb::VoxelGridIndex3D minIndex = _globalBoundingBox.at(0);
				rttb::VoxelGridIndex3D maxIndex = _globalBoundingBox.at(1);
				const unsigned int globalBoundingBoxSize0 = maxIndex[0] - minIndex[0] + 1;
				const unsigned int globalBoundingBoxSize1 = maxIndex[1] - minIndex[1] + 1;

                std::map<double, ::boost::shared_ptr<BoostArray2D> > voxelizationMapInThread;

                 for (auto & it : _geometryCoordinateBoostPolygonMap)
				{
                    BoostArray2D maskArray(::boost::extents[globalBoundingBoxSize0][globalBoundingBoxSize1]);

					BoostPolygonVector boostPolygonVec = it.second;

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
                            volumeFraction = correctForErrorAndStrictness(volumeFraction, _strict);
                            if (volumeFraction < 0 || volumeFraction > 1 )
                            {
                                throw rttb::core::InvalidParameterException("Mask calculation failed! The volume fraction should >= 0 and <= 1!");
                            }

							maskArray[x][y] = volumeFraction;
						}
					}

                    voxelizationMapInThread.insert(std::pair<double, BoostArray2DPointer>(it.first, ::boost::make_shared<BoostArray2D>(maskArray)));
				}
                //insert gathered values into voxelization map
                std::unique_lock<std::mutex> lock(*_mutex);
                _resultVoxelization->insert(voxelizationMapInThread.begin(), voxelizationMapInThread.end());

			}

			/*Get intersection polygons of the contour and a voxel polygon*/
			BoostMaskVoxelizationThread::BoostPolygonDeque BoostMaskVoxelizationThread::getIntersections(
			    const rttb::VoxelGridIndex3D&
			    aVoxelIndex3D, const BoostPolygonVector& intersectionSlicePolygons)
			{
				BoostMaskVoxelizationThread::BoostPolygonDeque polygonDeque;

				BoostRing2D voxelPolygon = get2DContour(aVoxelIndex3D);
				::boost::geometry::correct(voxelPolygon);

				BoostPolygonVector::const_iterator it;

				for (it = intersectionSlicePolygons.begin(); it != intersectionSlicePolygons.end(); ++it)
				{
					BoostPolygon2D contour = *it;
					::boost::geometry::correct(contour);
					
					BoostPolygonDeque intersection;
					::boost::geometry::intersection(voxelPolygon, contour, intersection);
					polygonDeque.insert(polygonDeque.end(), intersection.begin(), intersection.end());
				}

				return polygonDeque;
			}

			BoostMaskVoxelizationThread::BoostRing2D BoostMaskVoxelizationThread::get2DContour(
			    const rttb::VoxelGridIndex3D& aVoxelGrid3D)
			{
				BoostRing2D polygon;


				BoostPoint2D point1(aVoxelGrid3D[0] - 0.5, aVoxelGrid3D[1] - 0.5);
				::boost::geometry::append(polygon, point1);

				BoostPoint2D point2(aVoxelGrid3D[0] + 0.5, aVoxelGrid3D[1] - 0.5);
				::boost::geometry::append(polygon, point2);

				BoostPoint2D point3(aVoxelGrid3D[0] + 0.5, aVoxelGrid3D[1] + 0.5);
				::boost::geometry::append(polygon, point3);

				BoostPoint2D point4(aVoxelGrid3D[0] - 0.5, aVoxelGrid3D[1] + 0.5);
				::boost::geometry::append(polygon, point4);

				::boost::geometry::append(polygon, point1);

				return polygon;

			}

			/*Calculate the intersection area*/
			double BoostMaskVoxelizationThread::calcArea(const BoostPolygonDeque& aPolygonDeque)
			{
				double area = 0;

				BoostPolygonDeque::const_iterator it;

				for (it = aPolygonDeque.begin(); it != aPolygonDeque.end(); ++it)
				{
					area += ::boost::geometry::area(*it);
				}

				return area;
			}

            double BoostMaskVoxelizationThread::correctForErrorAndStrictness(double volumeFraction, bool strict) const
            {
                if (strict){
                    if (volumeFraction > 1 && (volumeFraction - 1) <= errorConstant)
                    {
                        volumeFraction = 1;
                    }
                }
                else {
                    if (volumeFraction > 1){
                        volumeFraction = 1;
                    }
                    else if (volumeFraction < 0){
                        volumeFraction = 0;
                    }
                }
                return volumeFraction;
            }

        }
	}
}
