#include <iostream>
#include <deque>
#include <algorithm>

#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/geometries/register/ring.hpp>
#include <boost/geometry/io/wkt/wkt.hpp>
#include <boost/geometry/multi/geometries/multi_polygon.hpp>
#include <boost/make_shared.hpp>

#include "rttbBoostMaskRedesign.h"
#include "rttbNullPointerException.h"
#include "rttbInvalidParameterException.h"

namespace rttb
{
	namespace masks
	{
		namespace boostRedesign
		{
			BoostMask::BoostMask(BoostMask::GeometricInfoPointer aDoseGeoInfo,
			                     BoostMask::StructPointer aStructure, bool strict)
				: _geometricInfo(aDoseGeoInfo), _structure(aStructure), _strict(strict),
				  _voxelInStructure(::boost::make_shared<MaskVoxelList>())
			{

				_isUpToDate = false;

				if (!_geometricInfo)
				{
					throw rttb::core::NullPointerException("Error: Geometric info is NULL!");
				}
				else if (!_structure)
				{
					throw rttb::core::NullPointerException("Error: Structure is NULL!");
				}

			}

			BoostMask::MaskVoxelListPointer BoostMask::getRelevantVoxelVector()
			{
				if (!_isUpToDate)
				{
					calcMask();
				}

				return _voxelInStructure;
			}

			void BoostMask::calcMask()
			{
				preprocessing();
				voxelization();
				_isUpToDate = true;
			}

			void BoostMask::preprocessing()
			{

				rttb::PolygonSequenceType polygonSequence = _structure->getStructureVector();

				//Convert world coordinate polygons to the polygons with geometry coordinate
				rttb::PolygonSequenceType geometryCoordinatePolygonVector;
				rttb::PolygonSequenceType::iterator it;

				for (it = polygonSequence.begin(); it != polygonSequence.end(); ++it)
				{
					PolygonType rttbPolygon = *it;
					PolygonType geometryCoordinatePolygon = worldCoordinateToGeometryCoordinatePolygon(rttbPolygon);
					geometryCoordinatePolygonVector.push_back(geometryCoordinatePolygon);

				}

				//Get global bounding box
				rttb::DoubleVoxelGridIndex3D globalMaxGridIndex(0.0, 0.0, 0);
				rttb::DoubleVoxelGridIndex3D globalMinGridIndex(_geometricInfo->getNumColumns(),
				        _geometricInfo->getNumRows(), _geometricInfo->getNumSlices());

				for (it = geometryCoordinatePolygonVector.begin(); it != geometryCoordinatePolygonVector.end();
				     ++it)
				{
					PolygonType geometryCoordinatePolygon = *it;
					rttb::DoubleVoxelGridIndex3D maxGridIndex;
					rttb::DoubleVoxelGridIndex3D minGridIndex;

					//get min/max for x/y/z of the contour
					calcMinMax(geometryCoordinatePolygon, minGridIndex, maxGridIndex);

					PolygonType::iterator coutIt;
					std::cout << "Polygon: ";

					for (coutIt = geometryCoordinatePolygon.begin(); coutIt != geometryCoordinatePolygon.end();
					     ++coutIt)
					{
						std::cout << (*coutIt).toString() << "; ";
					}

					std::cout << std::endl;
					std::cout << "min: " << minGridIndex.toString() << std::endl;
					std::cout << "max: " << maxGridIndex.toString() << std::endl;



					//check tilt, if more than the error constant throw exception
					if (checkTilt(minGridIndex, maxGridIndex, errorConstant))
					{
						throw rttb::core::Exception("TiltedMaskPlaneException");
					}

					//get global min/max for x/y/z of all contours
					//min and max for x
					if (minGridIndex(0) < globalMinGridIndex(0))
					{
						globalMinGridIndex(0) = minGridIndex(0);
					}

					if (maxGridIndex(0) > globalMaxGridIndex(0))
					{
						globalMaxGridIndex(0) = maxGridIndex(0);
					}

					//min and max for y
					if (minGridIndex(1) < globalMinGridIndex(1))
					{
						globalMinGridIndex(1) = minGridIndex(1);
					}

					if (maxGridIndex(1) > globalMaxGridIndex(1))
					{
						globalMaxGridIndex(1) = maxGridIndex(1);
					}

					//min and max for z
					if (minGridIndex(2) < globalMinGridIndex(2))
					{
						globalMinGridIndex(2) = minGridIndex(2);
					}

					if (maxGridIndex(2) > globalMaxGridIndex(2))
					{
						globalMaxGridIndex(2) = maxGridIndex(2);
					}

				}

				rttb::VoxelGridIndex3D minIndex = VoxelGridIndex3D(GridIndexType(globalMinGridIndex(0) + 0.5),
				                                  GridIndexType(globalMinGridIndex(1) + 0.5), GridIndexType(globalMinGridIndex(2) + 0.5));
				rttb::VoxelGridIndex3D maxIndex = VoxelGridIndex3D(GridIndexType(globalMaxGridIndex(0) + 0.5),
				                                  GridIndexType(globalMaxGridIndex(1) + 0.5), GridIndexType(globalMaxGridIndex(2) + 0.5));
				std::cout << "global min: " << minIndex.toString() << ", globa max: " << maxIndex.toString() <<
				          std::endl;
				_globalBoundingBox.push_back(minIndex);
				_globalBoundingBox.push_back(maxIndex);

				//convert rttb polygon sequence to a map of z index and a vector of boost ring 2d (without holes)
				BoostRingMap ringMap = convertRTTBPolygonSequenceToBoostRingMap(geometryCoordinatePolygonVector);

				//check donut and convert to a map of z index and a vector of boost polygon 2d (with or without holes)
				_geometryCoordinateBoostPolygonMap.clear();
				BoostRingMap::iterator itMap;

				for (itMap = ringMap.begin(); itMap != ringMap.end(); ++itMap)
				{
					BoostPolygonVector polygonVector = checkDonutAndConvert((*itMap).second);
					_geometryCoordinateBoostPolygonMap.insert(std::pair<double, BoostPolygonVector>((*itMap).first,
					        polygonVector));
				}
			}

			void BoostMask::voxelization()
			{
				BoostPolygonMap::iterator it;
				rttb::VoxelGridIndex3D minIndex = _globalBoundingBox.at(0);
				rttb::VoxelGridIndex3D maxIndex = _globalBoundingBox.at(1);
				int globalBoundingBoxSize0 = maxIndex[0] - minIndex[0] + 1;
				int globalBoundingBoxSize1 = maxIndex[1] - minIndex[1] + 1;

				for (it = _geometryCoordinateBoostPolygonMap.begin();
				     it != _geometryCoordinateBoostPolygonMap.end(); ++it)
				{
					BoostArray2D maskArray(boost::extents[globalBoundingBoxSize0][globalBoundingBoxSize1]);

					BoostPolygonVector boostPolygonVec = (*it).second;

					for (unsigned int x = 0; x < globalBoundingBoxSize0; ++x)
					{
						for (unsigned y = 0; y < globalBoundingBoxSize1; ++y)
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

							maskArray[x][y] = volumeFraction;
							std::cout << "(" << x << "," << y << "): " << volumeFraction << std::endl;
						}
					}

					//insert into voxelization map
					_voxelizationMap.insert(std::pair<double, BoostArray2D>((*it).first, maskArray));
				}

			}

			rttb::PolygonType BoostMask::worldCoordinateToGeometryCoordinatePolygon(
			    const rttb::PolygonType& aRTTBPolygon)
			{
				rttb::PolygonType geometryCoordinatePolygon;

				for (unsigned int i = 0; i < aRTTBPolygon.size(); i++)
				{
					rttb::WorldCoordinate3D worldCoordinatePoint = aRTTBPolygon.at(i);

					rttb::DoubleVoxelGridIndex3D geometryCoordinatePoint;
					_geometricInfo->worldCoordinateToGeometryCoordinate(worldCoordinatePoint, geometryCoordinatePoint);

					geometryCoordinatePolygon.push_back(geometryCoordinatePoint);

				}

				return geometryCoordinatePolygon;
			}

			bool BoostMask::checkTilt(const rttb::DoubleVoxelGridIndex3D& minimum,
			                          const rttb::DoubleVoxelGridIndex3D& maximum, double aErrorConstant)
			{
				return (abs(maximum(2) - minimum(2)) > aErrorConstant);
			}

			void BoostMask::calcMinMax(const rttb::PolygonType& aRTTBPolygon,
			                           rttb::DoubleVoxelGridIndex3D& minimum, rttb::DoubleVoxelGridIndex3D& maximum)
			{
				maximum(0) = 0.0;
				maximum(1) = 0.0;
				maximum(2) = 0.0;
				minimum(0) = _geometricInfo->getNumColumns();
				minimum(1) = _geometricInfo->getNumRows();
				minimum(2) = _geometricInfo->getNumSlices();

				//get min and max for x/y/z of a contour
				PolygonType::const_iterator polygonIt;

				for (polygonIt = aRTTBPolygon.begin(); polygonIt < aRTTBPolygon.end(); ++polygonIt)
				{
					rttb::DoubleVoxelGridIndex3D geometryCoordinatePoint = *polygonIt;

					//min and max for x
					if (geometryCoordinatePoint(0) < minimum(0))
					{
						minimum(0) = geometryCoordinatePoint(0);
					}

					if (geometryCoordinatePoint(0) > maximum(0))
					{
						maximum(0) = geometryCoordinatePoint(0);
					}

					//min and max for y
					if (geometryCoordinatePoint(1) < minimum(1))
					{
						minimum(1) = geometryCoordinatePoint(1);
					}

					if (geometryCoordinatePoint(1) > maximum(1))
					{
						maximum(1) = geometryCoordinatePoint(1);
					}

					//min and max for z
					if (geometryCoordinatePoint(2) < minimum(2))
					{
						minimum(2) = geometryCoordinatePoint(2);
					}

					if (geometryCoordinatePoint(2) > maximum(2))
					{
						maximum(2) = geometryCoordinatePoint(2);
					}

				}

			}


			BoostMask::BoostRing2D BoostMask::convertRTTBPolygonToBoostRing(const rttb::PolygonType&
			        aRTTBPolygon)
			{
				BoostMask::BoostRing2D polygon2D;
				BoostPoint2D firstPoint;

				for (unsigned int i = 0; i < aRTTBPolygon.size(); i++)
				{
					rttb::WorldCoordinate3D rttbPoint = aRTTBPolygon.at(i);
					BoostPoint2D boostPoint(rttbPoint[0], rttbPoint[1]);

					if (i == 0)
					{
						firstPoint = boostPoint;
					}

					::boost::geometry::append(polygon2D, boostPoint);
				}

				::boost::geometry::append(polygon2D, firstPoint);
				return polygon2D;
			}

			BoostMask::BoostRingMap BoostMask::convertRTTBPolygonSequenceToBoostRingMap(
			    const rttb::PolygonSequenceType& aRTTBPolygonVector)
			{
				rttb::PolygonSequenceType::const_iterator it;
				BoostMask::BoostRingMap aRingMap;

				for (it = aRTTBPolygonVector.begin(); it != aRTTBPolygonVector.end(); ++it)
				{
					rttb::PolygonType rttbPolygon = *it;
					double zIndex = rttbPolygon.at(0)[2];//get the first z index of the polygon
					BoostMask::BoostRingMap::iterator findIt = aRingMap.find(zIndex);

					//if the z index is found (same slice), add the polygon to vector
					if (findIt != aRingMap.end())
					{
						BoostRingVector ringVector = (*findIt).second;
						ringVector.push_back(convertRTTBPolygonToBoostRing(rttbPolygon));
					}
					//if it is the first z index in the map, insert vector with the polygon
					else
					{
						BoostRingVector ringVector;
						ringVector.push_back(convertRTTBPolygonToBoostRing(rttbPolygon));
						aRingMap.insert(std::pair<double, BoostRingVector>(zIndex, ringVector));
					}
				}

				return aRingMap;
			}

			BoostMask::BoostPolygonVector BoostMask::checkDonutAndConvert(const BoostMask::BoostRingVector&
			        aRingVector)
			{
				//check donut
				BoostMask::BoostRingVector::const_iterator it1;
				BoostMask::BoostRingVector::const_iterator it2;
				BoostMask::BoostPolygonVector boostPolygonVector;
				std::vector<unsigned int> donutIndexVector;//store the outer and inner ring index
				BoostMask::BoostPolygonVector donutVector;//store new generated donut polygon

				//Get donut index and donut polygon
				unsigned int index1 = 0;

				for (it1 = aRingVector.begin(); it1 != aRingVector.end(); it1++, index1++)
				{
					bool it1IsDonut = false;

					//check if the ring is already determined as a donut
					for (unsigned int i = 0; i < donutIndexVector.size(); i++)
					{
						if (donutIndexVector.at(i) == index1)
						{
							it1IsDonut = true;
							break;
						}
					}

					//if not jet, check now
					if (!it1IsDonut)
					{
						bool it2IsDonut = false;
						unsigned int index2 = 0;

						for (it2 = aRingVector.begin(); it2 != aRingVector.end(); it2++, index2++)
						{
							if (it2 != it1)
							{
								BoostMask::BoostPolygon2D polygon2D;

								if (::boost::geometry::within(*it1, *it2))
								{
									::boost::geometry::append(polygon2D, *it2);//append an outer ring to the polygon
									::boost::geometry::interior_rings(polygon2D).resize(1);//create an interior ring
									::boost::geometry::append(polygon2D, *it1, 0);//append a ring to the interior ring
									it2IsDonut = true;
								}
								//if donut
								else if (::boost::geometry::within(*it2, *it1))
								{
									::boost::geometry::append(polygon2D, *it1);//append an outer ring to the polygon
									::boost::geometry::interior_rings(polygon2D).resize(1);//create an interior ring
									::boost::geometry::append(polygon2D, *it2, 0);//append a ring to the interior ring
									it2IsDonut = true;
								}

								if (it2IsDonut)
								{
									donutIndexVector.push_back(index1);
									donutIndexVector.push_back(index2);
									donutVector.push_back(polygon2D);//store donut polygon
									break;//Only store the first donut!
								}
							}
						}
					}
				}

				//Store no donut polygon to boostPolygonVector
				index1 = 0;

				for (it1 = aRingVector.begin(); it1 != aRingVector.end(); it1++, index1++)
				{
					bool it1IsDonut = false;

					//check if the ring is the outer or inner of a donut
					for (unsigned int i = 0; i < donutIndexVector.size(); i++)
					{
						if (donutIndexVector.at(i) == index1)
						{
							it1IsDonut = true;
							break;
						}
					}

					if (!it1IsDonut)
					{
						BoostMask::BoostPolygon2D polygon2D;
						::boost::geometry::append(polygon2D, *it1);
						boostPolygonVector.push_back(polygon2D);//insert the ring, which is not a part of donut
					}
				}

				//Append donut polygon to boostPolygonVector
				BoostMask::BoostPolygonVector::iterator itDonut;

				for (itDonut = donutVector.begin(); itDonut != donutVector.end(); itDonut++)
				{
					boostPolygonVector.push_back(*itDonut);//append donuts
				}

				return boostPolygonVector;
			}

			BoostMask::BoostRing2D BoostMask::get2DContour(const rttb::VoxelGridIndex3D& aVoxelGrid3D)
			{
				BoostMask::BoostRing2D polygon;


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

			/*Get intersection polygons of the contour and a voxel polygon*/
			BoostMask::BoostPolygonDeque BoostMask::getIntersections(const rttb::VoxelGridIndex3D&
			        aVoxelIndex3D, const BoostPolygonVector& intersectionSlicePolygons)
			{
				BoostMask::BoostPolygonDeque polygonDeque;

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

			/*Calculate the intersection area*/
			double BoostMask::calcArea(const BoostPolygonDeque& aPolygonDeque)
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