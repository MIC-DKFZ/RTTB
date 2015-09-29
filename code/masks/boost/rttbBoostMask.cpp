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
			BoostMask::BoostMask(BoostMask::GeometricInfoPointer aDoseGeoInfo, BoostMask::StructPointer aStructure, bool strict)
				:_geometricInfo(aDoseGeoInfo), _structure(aStructure), _strict(strict), _voxelInStructure(::boost::make_shared<MaskVoxelList>()){

				_isUpToDate = false;
				if (!_geometricInfo){
					throw rttb::core::NullPointerException("Error: Geometric info is NULL!");
				}
				else if (!_structure){
					throw rttb::core::NullPointerException("Error: Structure is NULL!");
				}

			}

			void BoostMask::calcMask(){
				if (hasSelfIntersections()){
					if (_strict){
						throw rttb::core::InvalidParameterException("Error: structure has self intersections!");
					}
					else{
						std::cerr << _structure->getLabel() << " has self intersections! It may cause errors in the voxelization results!" << std::endl;
					}
				}

				std::vector<BoostMask::BoostPolygonVector> intersectionSlicePolygonsVector;//store the polygons of intersection slice of each index z

				//For each dose slice, get intersection structure slice and the contours on the structure slice
				for (unsigned int indexZ = 0; indexZ < _geometricInfo->getNumSlices(); indexZ++){
					BoostMask::BoostPolygonVector boostPolygons = getIntersectionSlicePolygons(indexZ);
					BoostMask::BoostPolygonVector::iterator it;
					for (it = boostPolygons.begin(); it != boostPolygons.end(); ++it){
						::boost::geometry::correct(*it);
					}
					intersectionSlicePolygonsVector.push_back(boostPolygons);
				}

				/* Use simple nearest neighbour interpolation (NNI) if dose and structure has different z spacing:
				If dose slice (indexZ) has no intersection with structure slice, but the last and the next do, that means the dose
				slice lies between two structure slices -> use the last slice intersection contours for this slice
				*/
				for (unsigned int indexZ = 1; indexZ < _geometricInfo->getNumSlices() - 1; indexZ++){
					if (intersectionSlicePolygonsVector.at(indexZ).size() == 0 && intersectionSlicePolygonsVector.at(indexZ - 1).size() > 0
						&& intersectionSlicePolygonsVector.at(indexZ + 1).size() > 0)
						intersectionSlicePolygonsVector.at(indexZ) = intersectionSlicePolygonsVector.at(indexZ - 1);
				}

				for (unsigned int indexZ = 0; indexZ < _geometricInfo->getNumSlices(); indexZ++){
					BoostMask::BoostPolygonVector intersectionSlicePolygons = intersectionSlicePolygonsVector.at(indexZ);

					//Get bounding box of this dose slice
					VoxelIndexVector voxelList = getBoundingBox(indexZ, intersectionSlicePolygons);
					rttb::VoxelGridIndex3D gridIndex3D0 = voxelList.at(0);
					rttb::VoxelGridIndex3D gridIndex3D1 = voxelList.at(1);

					for (unsigned int indexX = gridIndex3D0[0]; indexX <= gridIndex3D1[0]; indexX++){
						for (unsigned int indexY = gridIndex3D0[1]; indexY <= gridIndex3D1[1]; indexY++){
							rttb::VoxelGridIndex3D currentIndex;
							currentIndex[0] = indexX;
							currentIndex[1] = indexY;
							currentIndex[2] = indexZ;

							rttb::VoxelGridID gridID;
							_geometricInfo->convert(currentIndex, gridID);

							//Get intersection polygons of the dose voxel and the structure
							BoostPolygonDeque polygons = getIntersections(currentIndex, intersectionSlicePolygons);

							//Calc areas of all intersection polygons
							double intersectionArea = calcArea(polygons);

							double voxelSize = _geometricInfo->getSpacing()[0] * _geometricInfo->getSpacing()[1];

							if (intersectionArea > 0) {
								double volumeFraction = intersectionArea / voxelSize;
								if (volumeFraction > 1 && (volumeFraction - 1) <= 0.0001){
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

				for (unsigned int indexZ = 0; indexZ < _geometricInfo->getNumSlices(); indexZ++){

					rttb::VoxelGridIndex3D currentIndex(0, 0, indexZ);

					BoostMask::BoostPolygonVector boostPolygons = getIntersectionSlicePolygons(currentIndex[2]);

					BoostMask::BoostPolygonVector::iterator it;
					BoostMask::BoostPolygonVector::iterator it2;
					for (it = boostPolygons.begin(); it != boostPolygons.end(); ++it){
						::boost::geometry::correct(*it);
						//test if polygon has self intersection
						if (::boost::geometry::detail::overlay::has_self_intersections(*it, false)){
							hasSelfIntersection = true;
							std::cerr << _structure->getLabel() << " has self intersection polygon! Slice: " << indexZ << ". " << std::endl;
							break;
						}

						//test if the polygons on the same slice has intersection
						for (it2 = boostPolygons.begin(); it2 != boostPolygons.end() && it2 != it; ++it2){
							::boost::geometry::correct(*it2);
							BoostPolygonDeque intersection;
							::boost::geometry::intersection(*it, *it2, intersection);
							if (intersection.size() > 0){
								//if no donut
								if (!(::boost::geometry::within(*it, *it2)) && !(::boost::geometry::within(*it2, *it)))
								{
									hasSelfIntersection = true;
									std::cerr << _structure->getLabel() << ": Two polygons on the same slice has intersection! Slice: " << indexZ << "." << std::endl;
									break;
								}
							}
						}
					}

				}

				return hasSelfIntersection;
			}


			/*Get the 4 voxel index of the bounding box of the polygon in the slice with sliceNumber*/
			BoostMask::VoxelIndexVector BoostMask::getBoundingBox(unsigned int sliceNumber, const BoostPolygonVector& intersectionSlicePolygons){
				if (sliceNumber < 0 || sliceNumber >= _geometricInfo->getNumSlices()){
					throw rttb::core::InvalidParameterException(std::string("Error: slice number is invalid!"));
				}

				rttb::VoxelGridIndex3D currentIndex(0, 0, sliceNumber);

				double xMin = 0;
				double yMin = 0;
				double xMax = 0;
				double yMax = 0;

				BoostPolygonVector::const_iterator it;
				for (it = intersectionSlicePolygons.begin(); it != intersectionSlicePolygons.end(); ++it){
					::boost::geometry::model::box<BoostPoint2D> box;
					::boost::geometry::envelope(*it, box);
					BoostPoint2D minPoint = box.min_corner();
					BoostPoint2D maxPoint = box.max_corner();
					if (it == intersectionSlicePolygons.begin()){
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
				_geometricInfo->indexToWorldCoordinate(VoxelGridIndex3D(0, 0, 0), nullWorldCoord);
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
			BoostMask::BoostPolygonDeque BoostMask::getIntersections(const rttb::VoxelGridIndex3D& aVoxelIndex3D, const BoostPolygonVector& intersectionSlicePolygons){
				BoostMask::BoostPolygonDeque polygonDeque;

				BoostRing2D voxelPolygon = get2DContour(aVoxelIndex3D);
				::boost::geometry::correct(voxelPolygon);

				BoostPolygonVector::const_iterator it;
				for (it = intersectionSlicePolygons.begin(); it != intersectionSlicePolygons.end(); ++it){
					BoostPolygon2D contour = *it;
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
				for (it = aPolygonDeque.begin(); it != aPolygonDeque.end(); ++it){
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
				if (!_isUpToDate){
					calcMask();
				}
				return _voxelInStructure;
			}

			BoostMask::BoostRing2D BoostMask::convert(const rttb::PolygonType& aRTTBPolygon){
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


			BoostMask::BoostPolygonVector BoostMask::getIntersectionSlicePolygons(const rttb::GridIndexType aVoxelGridIndexZ){
				BoostMask::BoostRingVector boostRingVector;

				rttb::PolygonSequenceType polygonSequence = _structure->getStructureVector();

				//get the polygons in the slice and convert to boost rings
				rttb::PolygonSequenceType::iterator it;
				for (it = polygonSequence.begin(); it != polygonSequence.end(); ++it){
					PolygonType rttbPolygon = *it;
					if (!rttbPolygon.empty()){
						double polygonZCoor = rttbPolygon.at(0)[2];
						rttb::WorldCoordinate3D polygonPoint(0, 0, polygonZCoor);
						rttb::VoxelGridIndex3D polygonPointIndex3D;
						_geometricInfo->worldCoordinateToIndex(polygonPoint, polygonPointIndex3D);

						//if the z
						if (aVoxelGridIndexZ == polygonPointIndex3D[2]){
							boostRingVector.push_back(convert(rttbPolygon));
						}

					}
				}

				return checkDonutAndConvert(boostRingVector);
			}

			BoostMask::BoostRing2D BoostMask::get2DContour(const rttb::VoxelGridIndex3D& aVoxelGrid3D){
				BoostMask::BoostRing2D polygon;
				rttb::WorldCoordinate3D rttbPoint;
				_geometricInfo->indexToWorldCoordinate(aVoxelGrid3D, rttbPoint);

				BoostPoint2D point1(rttbPoint[0], rttbPoint[1]);
				::boost::geometry::append(polygon, point1);

				double xSpacing = _geometricInfo->getSpacing()[0];
				double ySpacing = _geometricInfo->getSpacing()[1];

				BoostPoint2D point2(rttbPoint[0] + xSpacing, rttbPoint[1]);
				::boost::geometry::append(polygon, point2);

				BoostPoint2D point3(rttbPoint[0] + xSpacing, rttbPoint[1] + ySpacing);
				::boost::geometry::append(polygon, point3);

				BoostPoint2D point4(rttbPoint[0], rttbPoint[1] + ySpacing);
				::boost::geometry::append(polygon, point4);

				::boost::geometry::append(polygon, point1);

				return polygon;

			}

			BoostMask::BoostPolygonVector BoostMask::checkDonutAndConvert(const BoostMask::BoostRingVector& aRingVector){
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