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

#include <limits>

#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/geometries/register/ring.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread/thread.hpp>

#include "rttbBoostMask.h"
#include "rttbNullPointerException.h"
#include "rttbInvalidParameterException.h"
#include "rttbBoostMaskGenerateMaskVoxelListThread.h"
#include "rttbBoostMaskVoxelizationThread.h"


namespace rttb
{
	namespace masks
	{
		namespace boost
		{


			BoostMask::BoostMask(BoostMask::GeometricInfoPointer aDoseGeoInfo,
			                     BoostMask::StructPointer aStructure, bool strict, unsigned int numberOfThreads)
				: _geometricInfo(aDoseGeoInfo), _structure(aStructure),
                _strict(strict), _numberOfThreads(numberOfThreads), _voxelizationThickness(0.0),
				  _voxelInStructure(::boost::make_shared<MaskVoxelList>())
			{

				_isUpToDate = false;

				if (_geometricInfo == nullptr)
				{
					throw rttb::core::NullPointerException("Error: Geometric info is nullptr!");
				}
				else if (_structure == nullptr)
				{
					throw rttb::core::NullPointerException("Error: Structure is nullptr!");
				}

				if (_numberOfThreads == 0)
				{
					_numberOfThreads = ::boost::thread::hardware_concurrency();
                    if (_numberOfThreads == 0)
                    {
                        throw rttb::core::InvalidParameterException("Error: detection of the number of hardware threads is not possible. Please specify number of threads for voxelization explicitly as parameter in BoostMask.");
                    }
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
				rttb::DoubleVoxelGridIndex3D globalMaxGridIndex(std::numeric_limits<double>::min(),
				        std::numeric_limits<double>::min(), std::numeric_limits<double>::min());
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

				rttb::VoxelGridIndex3D minIndex = VoxelGridIndex3D(GridIndexType(globalMinGridIndex(0) ),
				                                  GridIndexType(globalMinGridIndex(1) ), GridIndexType(globalMinGridIndex(2) ));
				rttb::VoxelGridIndex3D maxIndex = VoxelGridIndex3D(GridIndexType(globalMaxGridIndex(0) ),
				                                  GridIndexType(globalMaxGridIndex(1) ), GridIndexType(globalMaxGridIndex(2) ));

				_globalBoundingBox.push_back(minIndex);
				_globalBoundingBox.push_back(maxIndex);

				//convert rttb polygon sequence to a map of z index and a vector of boost ring 2d (without holes)
				_ringMap = convertRTTBPolygonSequenceToBoostRingMap(geometryCoordinatePolygonVector);

			}

			void BoostMask::voxelization()
			{

				if (_globalBoundingBox.size() < 2)
				{
					throw rttb::core::InvalidParameterException("Bounding box calculation failed! ");
				}

				BoostRingMap::iterator itMap;

				size_t mapSizeInAThread = _ringMap.size() / _numberOfThreads;
				unsigned int count = 0;
				unsigned int countThread = 0;
				BoostPolygonMap polygonMap;
				std::vector<BoostPolygonMap> polygonMapVector;

				//check donut and convert to a map of z index and a vector of boost polygon 2d (with or without holes)
				for (itMap = _ringMap.begin(); itMap != _ringMap.end(); ++itMap)
				{
					//the vector of all boost 2d polygons with the same z grid index(donut polygon is accepted).
					BoostPolygonVector polygonVector = checkDonutAndConvert((*itMap).second);

					if (count == mapSizeInAThread && countThread < (_numberOfThreads - 1))
					{
						polygonMapVector.push_back(polygonMap);
						polygonMap.clear();
						count = 0;
						countThread++;
					}

					polygonMap.insert(std::pair<double, BoostPolygonVector>((*itMap).first,
					                  polygonVector));
					count++;

				}

                _voxelizationMap = ::boost::make_shared<std::map<double, BoostArray2DPointer> >();

				polygonMapVector.push_back(polygonMap); //insert the last one

				//generate voxelization map, multi-threading
				::boost::thread_group threads;
                auto aMutex = ::boost::make_shared<::boost::shared_mutex>();

				for (unsigned int i = 0; i < polygonMapVector.size(); ++i)
				{
					BoostMaskVoxelizationThread t(polygonMapVector.at(i), _globalBoundingBox,
                        _voxelizationMap, aMutex, _strict);
					threads.create_thread(t);
				}

				threads.join_all();
			}

			void BoostMask::generateMaskVoxelList()
			{
				if (_globalBoundingBox.size() < 2)
				{
					throw rttb::core::InvalidParameterException("Bounding box calculation failed! ");
				}

				//check homogeneous of the voxelization plane (the contours plane)
				if (!calcVoxelizationThickness(_voxelizationThickness))
				{
					throw rttb::core::InvalidParameterException("Error: The contour plane should be homogeneous!");
				}



				::boost::thread_group threads;
                auto aMutex = ::boost::make_shared<::boost::shared_mutex>();

				unsigned int sliceNumberInAThread = _geometricInfo->getNumSlices() / _numberOfThreads;

				//generate mask voxel list, multi-threading
				for (unsigned int i = 0; i < _numberOfThreads; ++i)
				{
					unsigned int beginSlice = i * sliceNumberInAThread;
					unsigned int endSlice;

					if (i < _numberOfThreads - 1)
					{
						endSlice = (i + 1) * sliceNumberInAThread;
					}
					else
					{
						endSlice = _geometricInfo->getNumSlices();
					}


					BoostMaskGenerateMaskVoxelListThread t(_globalBoundingBox, _geometricInfo, _voxelizationMap,
					                                       _voxelizationThickness, beginSlice, endSlice,
					                                       _voxelInStructure, aMutex);

					threads.create_thread(t);

				}

				threads.join_all();

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

				return (std::abs(maxZ - minZ) <= aErrorConstant);
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
			    const rttb::PolygonSequenceType& aRTTBPolygonVector) const
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
			        aBoostRingMap, double aIndex, double aErrorConstant) const
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
					if (std::abs((*lowerBound).first - aIndex) <= aErrorConstant)
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
							if (std::abs((*lowerBound1).first - aIndex) <= aErrorConstant)
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

				for (it1 = aRingVector.begin(); it1 != aRingVector.end(); ++it1, index1++)
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

						for (it2 = aRingVector.begin(); it2 != aRingVector.end(); ++it2, index2++)
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

				for (it1 = aRingVector.begin(); it1 != aRingVector.end(); ++it1, index1++)
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

				for (itDonut = donutVector.begin(); itDonut != donutVector.end(); ++itDonut)
				{
					boostPolygonVector.push_back(*itDonut);//append donuts
				}

				return boostPolygonVector;
			}

			bool BoostMask::calcVoxelizationThickness(double& aThickness) const
			{

				if (_voxelizationMap->size() <= 1)
				{
					aThickness = 1;
					return true;
				}

				double thickness = 0;

                auto it = _voxelizationMap->cbegin();
                auto it2 = ++_voxelizationMap->cbegin();
				for (;
				     it != _voxelizationMap->cend() && it2 != _voxelizationMap->cend(); ++it, ++it2)
				{
					if (thickness == 0)
					{
						thickness = it2->first - it->first;
					}
					else
					{
                        double curThickness = it2->first - it->first;
						//if not homogeneous (leave out double imprecisions), return false
						if (std::abs(thickness-curThickness)>errorConstant)
						{
							//return false;
							std::cout << "Two polygons are far from each other?" << std::endl;
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
		}
	}
}
