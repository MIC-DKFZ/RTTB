#include "rttbBoostMaskVoxelizationThread.h"

namespace rttb
{
	namespace masks
	{
		namespace boost
		{
			BoostMaskVoxelizationThread::BoostMaskVoxelizationThread(const BoostPolygonMap& APolygonMap,
			        const VoxelIndexVector& aGlobalBoundingBox, VoxelizationIndexQueuePointer aResultIndexQueue,
			        VoxelizationQueuePointer aVoxelizationQueue) : _geometryCoordinateBoostPolygonMap(APolygonMap),
				_globalBoundingBox(aGlobalBoundingBox), _resultIndexQueue(aResultIndexQueue),
				_resultVoxelizationQueue(aVoxelizationQueue)
			{
			}


			void BoostMaskVoxelizationThread::operator()()
			{
				rttb::VoxelGridIndex3D minIndex = _globalBoundingBox.at(0);
				rttb::VoxelGridIndex3D maxIndex = _globalBoundingBox.at(1);
				int globalBoundingBoxSize0 = maxIndex[0] - minIndex[0] + 1;
				int globalBoundingBoxSize1 = maxIndex[1] - minIndex[1] + 1;

				BoostPolygonMap::iterator it;

				for (it = _geometryCoordinateBoostPolygonMap.begin();
				     it != _geometryCoordinateBoostPolygonMap.end(); ++it)
				{
					BoostArray2D* maskArray = new BoostArray2D(
					    ::boost::extents[globalBoundingBoxSize0][globalBoundingBoxSize1]);

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

							(*maskArray)[x][y] = volumeFraction;
						}
					}

					//insert into voxelization map
					_resultIndexQueue->push((*it).first);
					_resultVoxelizationQueue->push(maskArray);
				}

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
		}
	}
}