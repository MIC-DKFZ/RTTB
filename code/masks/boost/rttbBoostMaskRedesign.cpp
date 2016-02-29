#include <iostream>
#include <deque>
#include <algorithm>
#include <limits>

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
				generateMaskVoxelList();
				_isUpToDate = true;
			}

			void BoostMask::preprocessing()
			{
				rttb::PolygonSequenceType polygonSequence = _structure->getStructureVector();

				//Convert world coordinate polygons to the polygons with geometry coordinate
				rttb::PolygonSequenceType geometryCoordinatePolygonVector;
				rttb::PolygonSequenceType::iterator it;
				rttb::DoubleVoxelGridIndex3D globalMaxGridIndex(std::numeric_limits<double>::lowest(),
				        std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest());
				rttb::DoubleVoxelGridIndex3D globalMinGridIndex(_geometricInfo->getNumColumns(),
				        _geometricInfo->getNumRows(), 0);

				for (it = polygonSequence.begin(); it != polygonSequence.end(); ++it)
				{
					PolygonType rttbPolygon = *it;
					PolygonType geometryCoordinatePolygon;

					//1. convert polygon to geometry coordinate polygons
					//2. calculate global min/max
					//3. check if polygon is planar
					if (!preprocessingPolygon(rttbPolygon, geometryCoordinatePolygon, globalMinGridIndex,
					                          globalMaxGridIndex, errorConstant))
					{
						throw rttb::core::Exception("TiltedMaskPlaneException");
					}

					geometryCoordinatePolygonVector.push_back(geometryCoordinatePolygon);
				}

				rttb::VoxelGridIndex3D minIndex = VoxelGridIndex3D(GridIndexType(globalMinGridIndex(0) + 0.5),
				                                  GridIndexType(globalMinGridIndex(1) + 0.5), GridIndexType(globalMinGridIndex(2) + 0.5));
				rttb::VoxelGridIndex3D maxIndex = VoxelGridIndex3D(GridIndexType(globalMaxGridIndex(0) + 0.5),
				                                  GridIndexType(globalMaxGridIndex(1) + 0.5), GridIndexType(globalMaxGridIndex(2) + 0.5));
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

				if (_globalBoundingBox.size() < 2)
				{
					throw rttb::core::InvalidParameterException("Bounding box calculation failed! ");
				}

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
				}

			}

			void BoostMask::generateMaskVoxelList()
			{
				if (_globalBoundingBox.size() < 2)
				{
					throw rttb::core::InvalidParameterException("Bounding box calculation failed! ");
				}

				//check homogeneus of the voxelization plane (the contours plane)
				if (!calcVoxelizationThickness(_voxelizationThickness))
				{
					throw rttb::core::InvalidParameterException("Error: The contour plane should be homogeneus!");
				}

				rttb::VoxelGridIndex3D minIndex = _globalBoundingBox.at(0);
				rttb::VoxelGridIndex3D maxIndex = _globalBoundingBox.at(1);
				int globalBoundingBoxSize0 = maxIndex[0] - minIndex[0] + 1;
				int globalBoundingBoxSize1 = maxIndex[1] - minIndex[1] + 1;


				for (unsigned int indexZ = 0; indexZ < _geometricInfo->getNumSlices(); ++indexZ)
				{
					//calculate weight vector
					std::map<double, double> weightVectorForZ;
					calcWeightVector(indexZ, weightVectorForZ);

					//For each x,y, calc sum of all voxelization plane, use weight vector
					for (unsigned int x = 0; x < globalBoundingBoxSize0; ++x)
					{
						for (unsigned int y = 0; y < globalBoundingBoxSize1; ++y)
						{
							rttb::VoxelGridIndex3D currentIndex;
							currentIndex[0] = x + minIndex[0];
							currentIndex[1] = y + minIndex[1];
							currentIndex[2] = indexZ;
							rttb::VoxelGridID gridID;
							_geometricInfo->convert(currentIndex, gridID);
							double volumeFraction = 0;

							std::map<double, double>::iterator it;

							for (it = weightVectorForZ.begin(); it != weightVectorForZ.end(); ++it)
							{
								BoostArrayMap::iterator findVoxelizationIt = _voxelizationMap.find((*it).first);
								double weight = (*it).second;

								if (findVoxelizationIt == _voxelizationMap.end())
								{
									throw rttb::core::InvalidParameterException("Error: The contour plane should be homogeneus!");
								}

								BoostArray2D voxelizationArray = (*findVoxelizationIt).second;
								//calc sum of all voxelization plane, use weight
								volumeFraction += voxelizationArray[x][y] * weight;
							}


							if (volumeFraction > 1 && (volumeFraction - 1) <= errorConstant)
							{
								volumeFraction = 1;
							}
							else if (volumeFraction < 0 || (volumeFraction - 1) > errorConstant)
							{
								throw rttb::core::InvalidParameterException("Mask calculation failed! The volume fraction should >= 0 and <= 1!");
							}

							//insert mask voxel if volumeFraction > 0
							if (volumeFraction > 0)
							{
								core::MaskVoxel maskVoxel = core::MaskVoxel(gridID, volumeFraction);
								_voxelInStructure->push_back(maskVoxel);//push back the mask voxel in structure
							}
						}

					}

				}




			}

			bool BoostMask::preprocessingPolygon(const rttb::PolygonType& aRTTBPolygon,
			                                     rttb::PolygonType& geometryCoordinatePolygon, rttb::DoubleVoxelGridIndex3D& minimum,
			                                     rttb::DoubleVoxelGridIndex3D& maximum, double aErrorConstant) const
			{

				double minZ = _geometricInfo->getNumSlices();
				double maxZ =  0.0;

				for (unsigned int i = 0; i < aRTTBPolygon.size(); i++)
				{
					rttb::WorldCoordinate3D worldCoordinatePoint = aRTTBPolygon.at(i);

					//convert to geometry coordinate polygon
					rttb::DoubleVoxelGridIndex3D geometryCoordinatePoint;
					_geometricInfo->worldCoordinateToGeometryCoordinate(worldCoordinatePoint, geometryCoordinatePoint);

					geometryCoordinatePolygon.push_back(geometryCoordinatePoint);

					//calculate the current global min/max
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

					//check planar
					if (geometryCoordinatePoint(2) < minZ)
					{
						minZ = geometryCoordinatePoint(2);
					}

					if (geometryCoordinatePoint(2) > maxZ)
					{
						maxZ = geometryCoordinatePoint(2);
					}

				}

				return (abs(maxZ - minZ) <= aErrorConstant);
			}


			BoostMask::BoostRing2D BoostMask::convertRTTBPolygonToBoostRing(const rttb::PolygonType&
			        aRTTBPolygon) const
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
					bool isFirstZ = true;

					if (!aRingMap.empty())
					{
						BoostMask::BoostRingMap::iterator findIt = findNearestKey(aRingMap, zIndex, errorConstant);

						//if the z index is found (same slice), add the polygon to vector
						if (findIt != aRingMap.end())
						{
							//BoostRingVector ringVector = ;
							(*findIt).second.push_back(convertRTTBPolygonToBoostRing(rttbPolygon));
							isFirstZ = false;
						}
					}

					//if it is the first z index in the map, insert vector with the polygon
					if (isFirstZ)
					{
						BoostRingVector ringVector;
						ringVector.push_back(convertRTTBPolygonToBoostRing(rttbPolygon));
						aRingMap.insert(std::pair<double, BoostRingVector>(zIndex, ringVector));
					}

				}

				return aRingMap;
			}

			BoostMask::BoostRingMap::iterator BoostMask::findNearestKey(BoostMask::BoostRingMap&
			        aBoostRingMap, double aIndex, double aErrorConstant)
			{
				BoostMask::BoostRingMap::iterator find = aBoostRingMap.find(aIndex);

				//if find a key equivalent to aIndex, found
				if (find != aBoostRingMap.end())
				{
					return find;
				}
				else
				{
					BoostMask::BoostRingMap::iterator lowerBound = aBoostRingMap.lower_bound(aIndex);

					//if all keys go before aIndex, check the last key
					if (lowerBound == aBoostRingMap.end())
					{
						lowerBound = --aBoostRingMap.end();
					}

					//if the lower bound very close to aIndex, found
					if (abs((*lowerBound).first - aIndex) <= aErrorConstant)
					{
						return lowerBound;
					}
					else
					{
						//if the lower bound is the beginning, not found
						if (lowerBound == aBoostRingMap.begin())
						{
							return aBoostRingMap.end();
						}
						else
						{
							BoostMask::BoostRingMap::iterator lowerBound1 = --lowerBound;//the key before the lower bound

							//if the key before the lower bound very close to a Index, found
							if (abs((*lowerBound1).first - aIndex) <= aErrorConstant)
							{
								return lowerBound1;
							}
							//else, not found
							else
							{
								return aBoostRingMap.end();
							}
						}
					}

				}
			}

			BoostMask::BoostPolygonVector BoostMask::checkDonutAndConvert(const BoostMask::BoostRingVector&
			        aRingVector) const
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

			BoostMask::BoostRing2D BoostMask::get2DContour(const rttb::VoxelGridIndex3D& aVoxelGrid3D) const
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
			        aVoxelIndex3D, const BoostPolygonVector& intersectionSlicePolygons) const
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
			double BoostMask::calcArea(const BoostPolygonDeque& aPolygonDeque) const
			{
				double area = 0;

				BoostPolygonDeque::const_iterator it;

				for (it = aPolygonDeque.begin(); it != aPolygonDeque.end(); ++it)
				{
					area += ::boost::geometry::area(*it);
				}

				return area;
			}

			bool BoostMask::calcVoxelizationThickness(double& aThickness) const
			{
				BoostArrayMap::const_iterator it = _voxelizationMap.begin();
				BoostArrayMap::const_iterator it2 = ++_voxelizationMap.begin();
				double lastContourIndexZ;

				if (_voxelizationMap.size() <= 1)
				{
					aThickness = 1;
					return true;
				}

				double thickness = 0;

				for (;
				     it != _voxelizationMap.end(), it2 != _voxelizationMap.end(); ++it, ++it2)
				{
					if (thickness == 0)
					{
						thickness = (*it2).first - (*it).first;
					}
					else
					{
						//if no homogeneous, return false
						if (thickness != ((*it2).first - (*it).first))
						{
							return false;
						}
					}

				}

				if (thickness != 0)
				{
					aThickness = thickness;
				}
				else
				{
					aThickness = 1;
				}

				return true;
			}

			void BoostMask::calcWeightVector(const rttb::VoxelGridID& aIndexZ,
			                                 std::map<double, double>& weightVector) const
			{
				BoostArrayMap::const_iterator it;

				double indexZMin = aIndexZ - 0.5;
				double indexZMax = aIndexZ + 0.5;

				for (it = _voxelizationMap.begin(); it != _voxelizationMap.end(); ++it)
				{
					double voxelizationPlaneIndexMin = (*it).first - 0.5 * _voxelizationThickness;
					double voxelizationPlaneIndexMax = (*it).first + 0.5 * _voxelizationThickness;
					double weight = 0;

					if ((voxelizationPlaneIndexMin < indexZMin) && (voxelizationPlaneIndexMax > indexZMin))
					{
						if (voxelizationPlaneIndexMax < indexZMax)
						{
							weight = voxelizationPlaneIndexMax - indexZMin;
						}
						else
						{
							weight = 1;
						}
					}
					else if ((voxelizationPlaneIndexMin >= indexZMin) && (voxelizationPlaneIndexMin < indexZMax))
					{
						if (voxelizationPlaneIndexMax < indexZMax)
						{
							weight = _voxelizationThickness;
						}
						else
						{
							weight = indexZMax - voxelizationPlaneIndexMin;
						}
					}

					weightVector.insert(std::pair<double, double>((*it).first, weight));
				}
			}

		}
	}
}