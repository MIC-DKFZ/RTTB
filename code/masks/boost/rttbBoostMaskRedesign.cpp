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
			BoostMask::BoostMask(BoostMask::GeometricInfoPointer aDoseGeoInfo, BoostMask::StructPointer aStructure, bool strict)
				:_geometricInfo(aDoseGeoInfo), _structure(aStructure), _strict(strict), _voxelInStructure(::boost::make_shared<MaskVoxelList>())
			{

				_isUpToDate = false;
				if (!_geometricInfo){
					throw rttb::core::NullPointerException("Error: Geometric info is NULL!");
				}
				else if (!_structure){
					throw rttb::core::NullPointerException("Error: Structure is NULL!");
				}

			}

			BoostMask::MaskVoxelListPointer BoostMask::getRelevantVoxelVector()
			{
				if (!_isUpToDate){
					calcMask();
				}
				return _voxelInStructure;
			}

			void BoostMask::calcMask()
			{
				preprocessing();
				_isUpToDate = true;
			}

			void BoostMask::preprocessing()
			{
				rttb::PolygonSequenceType polygonSequence = _structure->getStructureVector();

				//Convert world coordinate polygons to the polygons with geometry coordinate
				rttb::PolygonSequenceType geometryCoordinatePolygonVector;
				rttb::PolygonSequenceType::iterator it;
				rttb::DoubleVoxelGridIndex3D globalMaxGridIndex(0.0, 0.0, 0);
				rttb::DoubleVoxelGridIndex3D globalMinGridIndex(_geometricInfo->getNumColumns(), _geometricInfo->getNumRows(), 0);
				for (it = polygonSequence.begin(); it != polygonSequence.end(); ++it){
					PolygonType rttbPolygon = *it;
					PolygonType geometryCoordinatePolygon;

					//1. convert polygon to geometry coordinate polygons
					//2. calculate global min/max
					//3. check if polygon is planar
					if (!preprocessingPolygon(rttbPolygon, geometryCoordinatePolygon, globalMinGridIndex, globalMaxGridIndex, errorConstant))
					{
						throw rttb::core::Exception("TiltedMaskPlaneException");
					}
					geometryCoordinatePolygonVector.push_back(geometryCoordinatePolygon);
				}

				_globalBoundingBox.push_back(globalMinGridIndex);
				_globalBoundingBox.push_back(globalMaxGridIndex);

				//convert rttb polygon sequence to a map of z index and a vector of boost ring 2d (without holes)
				BoostRingMap ringMap = convertRTTBPolygonSequenceToBoostRingMap(geometryCoordinatePolygonVector);

				//check donut and convert to a map of z index and a vector of boost polygon 2d (with or without holes)
				_geometryCoordinateBoostPolygonMap.clear();
				BoostRingMap::iterator itMap;
				for (itMap = ringMap.begin(); itMap != ringMap.end(); ++itMap){
					BoostPolygonVector polygonVector = checkDonutAndConvert((*itMap).second);
					_geometryCoordinateBoostPolygonMap.insert(std::pair<double, BoostPolygonVector>((*itMap).first, polygonVector));
				}
			}

			bool BoostMask::preprocessingPolygon(const rttb::PolygonType& aRTTBPolygon, rttb::PolygonType& geometryCoordinatePolygon, rttb::DoubleVoxelGridIndex3D& minimum, rttb::DoubleVoxelGridIndex3D& maximum, double aErrorConstant)
			{
				
				double minZ = _geometricInfo->getNumSlices();
				double maxZ =  0.0;
				for (unsigned int i = 0; i < aRTTBPolygon.size(); i++){
					rttb::WorldCoordinate3D worldCoordinatePoint = aRTTBPolygon.at(i);

					//convert to geometry coordinate polygon
					rttb::DoubleVoxelGridIndex3D geometryCoordinatePoint;
					_geometricInfo->worldCoordinateToGeometryCoordinate(worldCoordinatePoint, geometryCoordinatePoint);

					geometryCoordinatePolygon.push_back(geometryCoordinatePoint);

					//calculate the current global min/max
					//min and max for x
					if (geometryCoordinatePoint(0) < minimum(0)){
						minimum(0) = geometryCoordinatePoint(0);
					}
					if (geometryCoordinatePoint(0) > maximum(0)){
						maximum(0) = geometryCoordinatePoint(0);
					}
					//min and max for y
					if (geometryCoordinatePoint(1) < minimum(1)){
						minimum(1) = geometryCoordinatePoint(1);
					}
					if (geometryCoordinatePoint(1) > maximum(1)){
						maximum(1) = geometryCoordinatePoint(1);
					}
					//min and max for z
					if (geometryCoordinatePoint(2) < minimum(2)){
						minimum(2) = geometryCoordinatePoint(2);
					}
					if (geometryCoordinatePoint(2) > maximum(2)){
						maximum(2) = geometryCoordinatePoint(2);
					}

					//check planar
					if (geometryCoordinatePoint(2) < minZ){
						minZ = geometryCoordinatePoint(2);
					}
					if (geometryCoordinatePoint(2) > maxZ){
						maxZ = geometryCoordinatePoint(2);
					}

				}

				return (abs(maxZ - minZ) <= aErrorConstant);
			}


			BoostMask::BoostRing2D BoostMask::convertRTTBPolygonToBoostRing(const rttb::PolygonType& aRTTBPolygon)
			{
				BoostMask::BoostRing2D polygon2D;
				BoostPoint2D firstPoint;
				for (unsigned int i = 0; i < aRTTBPolygon.size(); i++){
					rttb::WorldCoordinate3D rttbPoint = aRTTBPolygon.at(i);
					BoostPoint2D boostPoint(rttbPoint[0], rttbPoint[1]);
					if (i == 0){
						firstPoint = boostPoint;
					}
					::boost::geometry::append(polygon2D, boostPoint);
				}
				::boost::geometry::append(polygon2D, firstPoint);
				return polygon2D;
			}

			BoostMask::BoostRingMap BoostMask::convertRTTBPolygonSequenceToBoostRingMap(const rttb::PolygonSequenceType& aRTTBPolygonVector)
			{
				rttb::PolygonSequenceType::const_iterator it;
				BoostMask::BoostRingMap aRingMap;
				for (it = aRTTBPolygonVector.begin(); it != aRTTBPolygonVector.end(); ++it){
					rttb::PolygonType rttbPolygon = *it;
					double zIndex = rttbPolygon.at(0)[2];//get the first z index of the polygon
					if (!aRingMap.empty())
					{
						BoostMask::BoostRingMap::const_iterator findIt = findNearestKey(aRingMap, zIndex, errorConstant);

						//if the z index is found (same slice), add the polygon to vector
						if (findIt != aRingMap.end()){
							BoostRingVector ringVector = (*findIt).second;
							ringVector.push_back(convertRTTBPolygonToBoostRing(rttbPolygon));
						}					
					}

					//if it is the first z index in the map, insert vector with the polygon
					BoostRingVector ringVector;
					ringVector.push_back(convertRTTBPolygonToBoostRing(rttbPolygon));
					aRingMap.insert(std::pair<double, BoostRingVector>(zIndex, ringVector));

				}
				return aRingMap;
			}

			BoostMask::BoostRingMap::const_iterator BoostMask::findNearestKey(const BoostMask::BoostRingMap& aBoostRingMap, double aIndex, double aErrorConstant)
			{
				BoostMask::BoostRingMap::const_iterator find = aBoostRingMap.find(aIndex);
				//if find a key equivalent to aIndex, found
				if (find != aBoostRingMap.end())
				{
					return find;
				}
				else
				{
					BoostMask::BoostRingMap::const_iterator lowerBound = aBoostRingMap.lower_bound(aIndex);
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
							BoostMask::BoostRingMap::const_iterator lowerBound1 = --lowerBound;//the key before the lower bound
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

			BoostMask::BoostPolygonVector BoostMask::checkDonutAndConvert(const BoostMask::BoostRingVector& aRingVector)
			{
				//check donut
				BoostMask::BoostRingVector::const_iterator it1;
				BoostMask::BoostRingVector::const_iterator it2;
				BoostMask::BoostPolygonVector boostPolygonVector;
				std::vector<unsigned int> donutIndexVector;//store the outer and inner ring index
				BoostMask::BoostPolygonVector donutVector;//store new generated donut polygon

				//Get donut index and donut polygon
				unsigned int index1 = 0;
				for (it1 = aRingVector.begin(); it1 != aRingVector.end(); it1++, index1++){
					bool it1IsDonut = false;
					//check if the ring is already determined as a donut
					for (unsigned int i = 0; i < donutIndexVector.size(); i++){
						if (donutIndexVector.at(i) == index1){
							it1IsDonut = true;
							break;
						}
					}
					//if not jet, check now
					if (!it1IsDonut){
						bool it2IsDonut = false;
						unsigned int index2 = 0;
						for (it2 = aRingVector.begin(); it2 != aRingVector.end(); it2++, index2++){
							if (it2 != it1){
								BoostMask::BoostPolygon2D polygon2D;
								if (::boost::geometry::within(*it1, *it2)){
									::boost::geometry::append(polygon2D, *it2);//append an outer ring to the polygon
									::boost::geometry::interior_rings(polygon2D).resize(1);//create an interior ring
									::boost::geometry::append(polygon2D, *it1, 0);//append a ring to the interior ring
									it2IsDonut = true;
								}
								//if donut
								else if (::boost::geometry::within(*it2, *it1)){
									::boost::geometry::append(polygon2D, *it1);//append an outer ring to the polygon
									::boost::geometry::interior_rings(polygon2D).resize(1);//create an interior ring
									::boost::geometry::append(polygon2D, *it2, 0);//append a ring to the interior ring
									it2IsDonut = true;
								}
								if (it2IsDonut){
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
				for (it1 = aRingVector.begin(); it1 != aRingVector.end(); it1++, index1++){
					bool it1IsDonut = false;
					//check if the ring is the outer or inner of a donut
					for (unsigned int i = 0; i < donutIndexVector.size(); i++){
						if (donutIndexVector.at(i) == index1){
							it1IsDonut = true;
							break;
						}
					}
					if (!it1IsDonut){
						BoostMask::BoostPolygon2D polygon2D;
						::boost::geometry::append(polygon2D, *it1);
						boostPolygonVector.push_back(polygon2D);//insert the ring, which is not a part of donut
					}
				}

				//Append donut polygon to boostPolygonVector
				BoostMask::BoostPolygonVector::iterator itDonut;
				for (itDonut = donutVector.begin(); itDonut != donutVector.end(); itDonut++){
					boostPolygonVector.push_back(*itDonut);//append donuts
				}

				return boostPolygonVector;
			}

		}
	}
}