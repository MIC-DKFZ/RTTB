#include <iostream>
#include <deque>
#include <algorithm>

#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/geometries/register/ring.hpp>
#include <boost/geometry/io/wkt/wkt.hpp>
#include <boost/geometry/multi/geometries/multi_polygon.hpp>
#include <boost/make_shared.hpp>

#include "rttbBoostMask.h"
#include "rttbNullPointerException.h"
#include "rttbInvalidParameterException.h"

namespace rttb
{
	namespace io
	{
		namespace mask
		{
			BoostMask::BoostMask(BoostMask::GeometricInfoPointer aDoseGeoInfo, BoostMask::StructPointer aStructure)
				:_geometricInfo(aDoseGeoInfo), _structure(aStructure), _voxelInStructure(boost::make_shared<MaskVoxelList>()){


					if(! _geometricInfo ){
						throw rttb::core::NullPointerException("Error: Geometric info is NULL!");
					}
					else if(!_structure){
						throw rttb::core::NullPointerException("Error: Structure is NULL!");
					}

			}

			void BoostMask::calcMask(){
				if(hasSelfIntersections()){
					throw rttb::core::InvalidParameterException("Error: structure has self intersections!");
				}

				for(unsigned int i=0; i< _geometricInfo->getNumSlices(); i++){
					VoxelIndexVector voxelList = getBoundingBox(i);
					rttb::VoxelGridIndex3D gridIndex3D0 = voxelList.at(0);
					rttb::VoxelGridIndex3D gridIndex3D1 = voxelList.at(1);


					for(unsigned int indexX = gridIndex3D0[0] ; indexX <= gridIndex3D1[0]; indexX ++  ){
						for(unsigned int indexY = gridIndex3D0[1]; indexY <= gridIndex3D1[1]; indexY ++){
							rttb::VoxelGridIndex3D currentIndex;
							currentIndex[0] = indexX;
							currentIndex[1] = indexY;
							currentIndex[2] = i;

							rttb::VoxelGridID gridID; 
							_geometricInfo->convert(currentIndex, gridID);

							std::deque<BoostMask::BoostPolygon2D> polygons = getIntersections(currentIndex);
							double intersectionArea = calcArea(polygons);

							double voxelSize = _geometricInfo->getSpacing()[0] * _geometricInfo->getSpacing()[1];

							if(intersectionArea > 0) {
								double volumeFraction = intersectionArea/voxelSize;
								if(volumeFraction > 1 && (volumeFraction-1) <= 0.0001){
									volumeFraction = 1;
								}
								core::MaskVoxel maskVoxel = core::MaskVoxel(gridID, (volumeFraction));
								_voxelInStructure->push_back(maskVoxel);//push back the mask voxel in structure
							}
						}
					}
				}

				sort(_voxelInStructure->begin(), _voxelInStructure->end());
			}

			bool BoostMask::hasSelfIntersections(){
				bool hasSelfIntersection = false;

				for(unsigned int indexZ=0; indexZ< _geometricInfo->getNumSlices(); indexZ++){
					rttb::VoxelGridIndex3D currentIndex(0,0,indexZ);

					BoostMask::BoostRingVector boostPolygons = getIntersectionSlicePolygons(currentIndex);

					for(unsigned int i =0; i<boostPolygons.size(); i++){
						boost::geometry::correct(boostPolygons.at(i));
						//test if polygon has self intersection
						if(boost::geometry::detail::overlay::has_self_intersections(boostPolygons.at(i))){
							hasSelfIntersection = true;
							std::cout << "Has self intersection! Slice: "<< indexZ << ", polygon "<<i <<std::endl;
							break;
						}
						//test if the polygons on the same slice has intersection
						for(unsigned int j =0; j<boostPolygons.size() && j!=i; j++){
							boost::geometry::correct(boostPolygons.at(j));
							BoostPolygonDeque intersection;
							boost::geometry::intersection(boostPolygons.at(i), boostPolygons.at(j), intersection);
							if(intersection.size()>0){
								hasSelfIntersection = true;
								std::cout << "Has self intersection! Slice: "<< indexZ << ", polygon "<<i << " and "<<j<<std::endl;
								break;
							}
						}
					}	

				}

				return hasSelfIntersection;
			}


			/*Get the 4 voxel index of the bounding box of the polygon in the slice with sliceNumber*/
			BoostMask::VoxelIndexVector BoostMask::getBoundingBox(unsigned int sliceNumber){
				if(sliceNumber<0 || sliceNumber >= _geometricInfo->getNumSlices()){
					throw rttb::core::InvalidParameterException(std::string("Error: slice number is invalid!"));
				}

				rttb::VoxelGridIndex3D currentIndex(0,0,sliceNumber);

				BoostRingVector boostPolygons = getIntersectionSlicePolygons(currentIndex);

				double xMin = 0;
				double yMin = 0;
				double xMax = 0; 
				double yMax = 0;
				for(unsigned int i =0; i<boostPolygons.size(); i++){
					boost::geometry::correct(boostPolygons.at(i));
					boost::geometry::model::box<BoostPoint2D> box;
					boost::geometry::envelope(boostPolygons.at(i), box);
					BoostPoint2D minPoint = box.min_corner();
					BoostPoint2D maxPoint = box.max_corner();
					if(i==0){
						xMin = minPoint.x();
						yMin = minPoint.y();
						xMax = maxPoint.x();
						yMax = maxPoint.y();
					}
					xMin = std::min(xMin, minPoint.x());
					yMin = std::min(yMin, minPoint.y());
					xMax = std::max(xMax, maxPoint.x());
					yMax = std::max(yMax, maxPoint.y());
				}

				rttb::WorldCoordinate3D nullWorldCoord;
				_geometricInfo->indexToWorldCoordinate(VoxelGridIndex3D(0,0,0),nullWorldCoord);
				rttb::WorldCoordinate3D minWorldCoord(xMin, yMin, nullWorldCoord.z());
				rttb::WorldCoordinate3D maxWorldCoord(xMax, yMax, nullWorldCoord.z());

				rttb::VoxelGridIndex3D index0;
				rttb::VoxelGridIndex3D index1;
				_geometricInfo->worldCoordinateToIndex(minWorldCoord, index0);
				_geometricInfo->worldCoordinateToIndex(maxWorldCoord, index1);

				VoxelIndexVector voxelList;
				voxelList.push_back(index0);
				voxelList.push_back(index1);

				return voxelList;
			}


			/*Get intersection polygons of the contour and a voxel polygon*/
			BoostMask::BoostPolygonDeque BoostMask::getIntersections(const rttb::VoxelGridIndex3D& aVoxelIndex3D){
				BoostMask::BoostPolygonDeque polygonDeque;

				BoostRing2D voxelPolygon = get2DContour(aVoxelIndex3D);
				boost::geometry::correct(voxelPolygon);

				BoostMask::BoostRingVector contourVector = getIntersectionSlicePolygons(aVoxelIndex3D);
				for(unsigned int i=0; i<contourVector.size(); i++){
					BoostRing2D contour = contourVector.at(i);
					boost::geometry::correct(contour);
					BoostPolygonDeque intersection;
					boost::geometry::intersection(voxelPolygon, contour, intersection);

					polygonDeque.insert(polygonDeque.end(), intersection.begin(), intersection.end());
				}

				return polygonDeque;
			}


			/*Calculate the intersection area*/
			double BoostMask::calcArea(std::deque<BoostMask::BoostPolygon2D> aPolygonDeque){
				double area = 0;
				for(unsigned int i=0; i<aPolygonDeque.size(); i++){
					area += boost::geometry::area(aPolygonDeque.at(i));
				}
				return area;
			}

			VoxelGridIndex3D BoostMask::getGridIndex3D(const core::MaskVoxel& aMaskVoxel){
				rttb::VoxelGridIndex3D gridIndex3D; 
				_geometricInfo->convert(aMaskVoxel.getVoxelGridID(), gridIndex3D);
				return gridIndex3D;
			}

			BoostMask::MaskVoxelListPointer BoostMask::getRelevantVoxelVector(){
				calcMask();
				return _voxelInStructure;
			}

			BoostMask::BoostRing2D BoostMask::convert(const rttb::PolygonType& aRTTBPolygon){
				BoostMask::BoostRing2D polygon2D;
				BoostPoint2D firstPoint;
				for(unsigned int i=0; i<aRTTBPolygon.size(); i++){
					rttb::WorldCoordinate3D rttbPoint = aRTTBPolygon.at(i);
					BoostPoint2D boostPoint(rttbPoint[0], rttbPoint[1]); 
					if(i==0){
						firstPoint = boostPoint;
					}
					boost::geometry::append(polygon2D, boostPoint);
				}
				boost::geometry::append(polygon2D, firstPoint);
				return polygon2D;

			}

			BoostMask::BoostRingVector BoostMask::getIntersectionSlicePolygons(const rttb::VoxelGridIndex3D& aVoxelGrid3D){
				BoostRingVector boostPolygonVector;

				rttb::PolygonSequenceType polygonSequence = _structure->getStructureVector();

				for(unsigned int i=0; i<polygonSequence.size(); i++){
					PolygonType rttbPolygon = polygonSequence.at(i);
					if(rttbPolygon.size()>0){
						double polygonZCoor = rttbPolygon.at(0)[2];
						rttb::WorldCoordinate3D polygonPoint(0,0, polygonZCoor);
						rttb::VoxelGridIndex3D polygonPointIndex3D;
						_geometricInfo->worldCoordinateToIndex(polygonPoint, polygonPointIndex3D);

						//if the z
						if(aVoxelGrid3D[2] == polygonPointIndex3D[2]){
							boostPolygonVector.push_back(convert(rttbPolygon));
						}
					}
				}

				return boostPolygonVector;
			}

			BoostMask::BoostRing2D BoostMask::get2DContour(const rttb::VoxelGridIndex3D& aVoxelGrid3D){
				BoostMask::BoostRing2D polygon;
				rttb::WorldCoordinate3D rttbPoint;
				_geometricInfo->indexToWorldCoordinate(aVoxelGrid3D, rttbPoint);

				BoostPoint2D point1 (rttbPoint[0], rttbPoint[1]);
				boost::geometry::append(polygon, point1);

				double xSpacing = _geometricInfo->getSpacing()[0];
				double ySpacing = _geometricInfo->getSpacing()[1];

				BoostPoint2D point2(rttbPoint[0]+xSpacing, rttbPoint[1]);
				boost::geometry::append(polygon, point2);

				BoostPoint2D point3(rttbPoint[0]+xSpacing, rttbPoint[1]+ySpacing);
				boost::geometry::append(polygon, point3);

				BoostPoint2D point4(rttbPoint[0], rttbPoint[1]+ySpacing);
				boost::geometry::append(polygon, point4);

				boost::geometry::append(polygon, point1);

				return polygon;

			}
		}
	}
}