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
	namespace masks
	{
		namespace boost
		{
			BoostMask::BoostMask(BoostMask::GeometricInfoPointer aDoseGeoInfo, BoostMask::StructPointer aStructure)
				:_geometricInfo(aDoseGeoInfo), _structure(aStructure), _voxelInStructure(::boost::make_shared<MaskVoxelList>()){

					_isUpToDate = false;
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

				std::vector<BoostMask::BoostRingVector> intersectionSlicePolygonsVector;//store the polygons of intersection slice of each index z

				//For each dose slice, get intersection structure slice and the contours on the structure slice
				for(unsigned int indexZ=0; indexZ< _geometricInfo->getNumSlices(); indexZ++){
					BoostMask::BoostRingVector boostPolygons = getIntersectionSlicePolygons(indexZ);
					BoostMask::BoostRingVector::iterator it;
					for(it = boostPolygons.begin(); it != boostPolygons.end(); ++it){
						::boost::geometry::correct(*it);
					}
					intersectionSlicePolygonsVector.push_back(boostPolygons);
				}

				/* Use simple nearest neighbour interpolation (NNI) if dose and structure has different z spacing:
				If dose slice (indexZ) has no intersection with structure slice, but the last and the next do, that means the dose
				slice lies between two structure slices -> use the last slice intersection contours for this slice 			
				*/
				for(unsigned int indexZ=1; indexZ<_geometricInfo->getNumSlices()-1; indexZ++){
					if(intersectionSlicePolygonsVector.at(indexZ).size() == 0 && intersectionSlicePolygonsVector.at(indexZ-1).size() > 0
						&& intersectionSlicePolygonsVector.at(indexZ+1).size() > 0)
						intersectionSlicePolygonsVector.at(indexZ) = intersectionSlicePolygonsVector.at(indexZ-1);
				}

				for(unsigned int indexZ=0; indexZ< _geometricInfo->getNumSlices(); indexZ++){
					BoostMask::BoostRingVector intersectionSlicePolygons = intersectionSlicePolygonsVector.at(indexZ);

					//Get bounding box of this dose slice
					VoxelIndexVector voxelList = getBoundingBox(indexZ, intersectionSlicePolygons);
					rttb::VoxelGridIndex3D gridIndex3D0 = voxelList.at(0);
					rttb::VoxelGridIndex3D gridIndex3D1 = voxelList.at(1);

					for(unsigned int indexX = gridIndex3D0[0] ; indexX <= gridIndex3D1[0]; indexX ++  ){
						for(unsigned int indexY = gridIndex3D0[1]; indexY <= gridIndex3D1[1]; indexY ++){
							rttb::VoxelGridIndex3D currentIndex;
							currentIndex[0] = indexX;
							currentIndex[1] = indexY;
							currentIndex[2] = indexZ;

							rttb::VoxelGridID gridID; 
							_geometricInfo->convert(currentIndex, gridID);

							//Get intersection polygons of the dose voxel and the structure
							std::deque<BoostMask::BoostPolygon2D> polygons = getIntersections(currentIndex, intersectionSlicePolygons);

							//Calc areas of all intersection polygons
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

					BoostMask::BoostRingVector boostPolygons = getIntersectionSlicePolygons(currentIndex[2]);

					BoostMask::BoostRingVector::iterator it;
					BoostMask::BoostRingVector::iterator it2;
					for(it = boostPolygons.begin(); it != boostPolygons.end(); ++it){
						::boost::geometry::correct(*it);
						//test if polygon has self intersection
						if(::boost::geometry::detail::overlay::has_self_intersections(*it)){
							hasSelfIntersection = true;
							std::cout << "Has self intersection polygon! Slice: "<< indexZ << ". " <<std::endl;
							break;
						}

						//test if the polygons on the same slice has intersection
						for(it2 = boostPolygons.begin(); it2 != boostPolygons.end() && it2 != it; ++it2){
							::boost::geometry::correct(*it2);
							BoostPolygonDeque intersection;
							::boost::geometry::intersection(*it, *it2, intersection);
							if(intersection.size()>0){
								hasSelfIntersection = true;
								std::cout << "Two polygons on the same slice has intersection! Slice: "<< indexZ << "." <<std::endl;
								break;
							}
						}
					}	

				}

				return hasSelfIntersection;
			}


			/*Get the 4 voxel index of the bounding box of the polygon in the slice with sliceNumber*/
			BoostMask::VoxelIndexVector BoostMask::getBoundingBox(unsigned int sliceNumber, const BoostRingVector& intersectionSlicePolygons){
				if(sliceNumber<0 || sliceNumber >= _geometricInfo->getNumSlices()){
					throw rttb::core::InvalidParameterException(std::string("Error: slice number is invalid!"));
				}

				rttb::VoxelGridIndex3D currentIndex(0,0,sliceNumber);

				double xMin = 0;
				double yMin = 0;
				double xMax = 0; 
				double yMax = 0;

				BoostRingVector::const_iterator it;
				for(it = intersectionSlicePolygons.begin(); it != intersectionSlicePolygons.end(); ++it){
					::boost::geometry::model::box<BoostPoint2D> box;
					::boost::geometry::envelope(*it, box);
					BoostPoint2D minPoint = box.min_corner();
					BoostPoint2D maxPoint = box.max_corner();
					if(it == intersectionSlicePolygons.begin()){
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
			BoostMask::BoostPolygonDeque BoostMask::getIntersections(const rttb::VoxelGridIndex3D& aVoxelIndex3D, const BoostRingVector& intersectionSlicePolygons){
				BoostMask::BoostPolygonDeque polygonDeque;

				BoostRing2D voxelPolygon = get2DContour(aVoxelIndex3D);
				::boost::geometry::correct(voxelPolygon);

				BoostRingVector::const_iterator it;
				for(it = intersectionSlicePolygons.begin(); it != intersectionSlicePolygons.end(); ++it){
					BoostRing2D contour = *it;
					::boost::geometry::correct(contour);
					BoostPolygonDeque intersection;
					::boost::geometry::intersection(voxelPolygon, contour, intersection);

					polygonDeque.insert(polygonDeque.end(), intersection.begin(), intersection.end());
				}

				return polygonDeque;
			}


			/*Calculate the intersection area*/
			double BoostMask::calcArea(const BoostPolygonDeque& aPolygonDeque){
				double area = 0;

				BoostPolygonDeque::const_iterator it;
				for(it = aPolygonDeque.begin(); it != aPolygonDeque.end(); ++it){
					area += ::boost::geometry::area(*it);
				}
				return area;
			}

			VoxelGridIndex3D BoostMask::getGridIndex3D(const core::MaskVoxel& aMaskVoxel){
				rttb::VoxelGridIndex3D gridIndex3D; 
				_geometricInfo->convert(aMaskVoxel.getVoxelGridID(), gridIndex3D);
				return gridIndex3D;
			}

			BoostMask::MaskVoxelListPointer BoostMask::getRelevantVoxelVector(){
				if(!_isUpToDate){
					calcMask();
				}
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
					::boost::geometry::append(polygon2D, boostPoint);
				}
				::boost::geometry::append(polygon2D, firstPoint);
				return polygon2D;

			}

			BoostMask::BoostRingVector BoostMask::getIntersectionSlicePolygons(const rttb::GridIndexType aVoxelGridIndexZ){
				BoostRingVector boostPolygonVector;

				rttb::PolygonSequenceType polygonSequence = _structure->getStructureVector();
				rttb::PolygonSequenceType::iterator it;
				for(it = polygonSequence.begin(); it != polygonSequence.end(); ++it){
					PolygonType rttbPolygon = *it;
					if(!rttbPolygon.empty()){
						double polygonZCoor = rttbPolygon.at(0)[2];
						rttb::WorldCoordinate3D polygonPoint(0,0, polygonZCoor);
						rttb::VoxelGridIndex3D polygonPointIndex3D;
						_geometricInfo->worldCoordinateToIndex(polygonPoint, polygonPointIndex3D);

						//if the z
						if(aVoxelGridIndexZ == polygonPointIndex3D[2]){
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
				::boost::geometry::append(polygon, point1);

				double xSpacing = _geometricInfo->getSpacing()[0];
				double ySpacing = _geometricInfo->getSpacing()[1];

				BoostPoint2D point2(rttbPoint[0]+xSpacing, rttbPoint[1]);
				::boost::geometry::append(polygon, point2);

				BoostPoint2D point3(rttbPoint[0]+xSpacing, rttbPoint[1]+ySpacing);
				::boost::geometry::append(polygon, point3);

				BoostPoint2D point4(rttbPoint[0], rttbPoint[1]+ySpacing);
				::boost::geometry::append(polygon, point4);

				::boost::geometry::append(polygon, point1);

				return polygon;

			}
		}
	}
}