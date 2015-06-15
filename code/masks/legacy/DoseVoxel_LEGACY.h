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
// @version $Revision: 707 $ (last changed revision)
// @date    $Date: 2014-09-04 16:37:24 +0200 (Do, 04 Sep 2014) $ (last change date)
// @author  $Author: floca $ (last changed by)
*/

#ifndef __RT_DOSE_VOXEL_H
#define __RT_DOSE_VOXEL_H


//#include "rttbStructure.h"
#include "rttbGeometricInfo.h"
#include "rttbBaseType.h"
#include "rttbBaseType_LEGACY.h"
#include "rttbStructure_LEGACY.h"

namespace rttb{

  namespace masks
  {
namespace legacy{
    /*! @class DoseVoxel
    *	@brief This is a class representing a DoseVoxel. 
    */

    class DoseVoxel
    {

    private: 
      /*! @brief Voxel index */
      LegacyDoseVoxelIndex3D _voxelIndex3D;

      /*! @brief WorldCoordinate (_x,_y,_z): the left upper point, the point in voxel with minimal x y and z coordinate*/
      WorldCoordinate _x;
      WorldCoordinate _y;
      WorldCoordinate _z;
      /*! @brief(_x+_deltaX, _y+_deltaY,_z+_deltaZ): the point in voxel with maximal x y and z coordinate*/
      WorldCoordinate _deltaX;
      WorldCoordinate _deltaY;
      WorldCoordinate _deltaZ;

      /*! @brief Geometric information*/
	  const core::GeometricInfo* _geoInfo;

      /*! @brief The proportion of voxel which is inside a given structure: 0~1 */
      FractionType _proportionInStr;


    public: 

      /* @brief Standard Constructor*/
      DoseVoxel();

      /*! @brief Constructor
      @param aDoseIndex: a dose voxel index, aGeometricInfo: the geometric information (imagePosition, ImageOrientation, 
      pixelSpacing and getSliceThickness())
      */
      DoseVoxel(const VoxelGridIndex3D& aDoseIndex, const core::GeometricInfo* aGeometricInfo);
	  //same method with legacy data structures
	  DoseVoxel(const LegacyDoseVoxelIndex3D& aDoseIndex, const core::GeometricInfo* aGeometricInfo);

      /*! @brief Operator =
      */
      bool operator==(const DoseVoxel& doseVoxel) const;

      /*! @brief Operator <
      */
      bool operator<(const DoseVoxel& doseVoxel) const;


      /*! @brief Initialisation of the dose voxel with dose voxel index and DICOM-RT information. Transform dose index to world coordinate 
      *  @param imagePosition: RT Image Position(3002,0012), 
      *  @param getImageOrientationRow(), getImageOrientationColumn(): Image Orientation (Patient) (0020,0037), 
      *  @param getPixelSpacingRow(), getPixelSpacingColumn(): Pixel Spacing (0028,0030)
      *  @param getSliceThickness(): Slice Thickness (0018,0050). 
      *  @exception RTTBInvalidParameterException thrown if aDoseIndex /Pixel spacing/slice thickness invalid
      *
      */

      //void setDoseIndex(const VoxelGridIndex3D& aDoseIndex, const WorldCoordinate3D& imagePosition, 
        //const ImageOrientation& getImageOrientationRow(), const ImageOrientation& getImageOrientationColumn(), 
        //double getPixelSpacingRow(), double getPixelSpacingColumn(), double getSliceThickness());
      void setDoseVoxel(const VoxelGridIndex3D& aDoseIndex, const core::GeometricInfo* aGeometricInfo);
	  //same method with legacy data structures
	  void setDoseVoxel(const LegacyDoseVoxelIndex3D& aDoseIndex, const core::GeometricInfo* aGeometricInfo);


      /*! @brief Initialisation of the dose voxel with world coordinate. Set dose voxel with the left upper point and delta
      *
      */

      void setDoseVoxel(WorldCoordinate3D aWorldCoordinate, WorldCoordinate3D aVoxelDelta);
	  //same method with legacy data structures
	  void setDoseVoxel(LegacyWorldCoordinate3D aWorldCoordinate, LegacyWorldCoordinate3D aVoxelDelta);
	  
      /*! @brief Tests if a point is inside the voxel
      *
      */
      bool pointInVoxel(const WorldCoordinate3D& aPoint) const;
	  //same method with legacy data structures
	  bool pointInVoxel(const LegacyWorldCoordinate3D& aPoint) const;
	  
      /*! @brief Calculates how many relative volume of the voxel inside the structure.
      *  @return Return the relative volume. >0: voxel in structure, =0: voxel not in structure, <0: error
      */
	  double voxelInStructure(const StructureLegacy& aRTStructure) const;


      /*! @brief Calculates how many relative volume of the voxel inside the structure, with the corresponding polygon index number. 
      *         It will be tested if the dose voxel in any Polygon with the same z coord..
      *  @param sliceNumber: fist index number of the polygon stored in strVector (ranked by z coord. of the polygon) if the z coord. 
      *                      of the dose voxel between the last Polygon and this Polygon!!! 
      *          
      *  @return Return the relative volume. >0: voxel in structure, =0: voxel not in structure, <0: error
      *  @pre aRTStructure must not be NULL
      */
      double voxelInStructure(const StructureLegacy& aRTStructure, int sliceNumber) const;


      /*! @brief Calculates how many relative volume of the voxel inside the corresponding polygon with polygon index number. 
      *     
      *  @param polygonIndex: the index number of the polygon stored in strVector (ranked by z coord. of the polygon) 
      *          
      *  @return Return the relative volume. >0: voxel in structure, =0: voxel not in structure, <0: error
      *  @pre aRTStructure must not be NULL
      */
      double voxelInStructureAtIndex(const StructureLegacy& aRTStructure, int polygonIndex) const;


      /*! @brief Tests if a point is inside a polygon. 
      *  @return Return true if the point is inside the polygon or the point is on an edge of the polygon
      ! Make sure that the point and the list of positions lies both in the z-plane
      */
      //bool pointInPolygon(const PolygonType& aPolygon, const WorldCoordinate3D& aPoint) const;

      /*! @brief Tests if the voxel intersect the polygon defined by the given list of positions.
      ! Make sure that the point and the list of positions lies both in the z-plane
      */
      //public: bool voxelInROI(vector<WorldCoordinate3D*> aRoi);





      /*! @brief get the first corresponding slice number: 
      *  !!!Important: not the actual slice number, but the index number of the polygon stored in the structure vector (ranked by z coord. of the polygon)
      if the z coord. of the dose voxel between the last Polygon and this Polygon!!!
      *  @return i from (0~strVector.size()-1): voxel between the i-th and (i+1)-th polygon or _z on the i-th polygon; 
      *          -1: out of structure and _z smaller than z coordi.of the first polygon
      *	    	strVector.size(): out of structure and _z bigger than z coordi.of the last polygon
      */

      int getCorrespondingSlice(const StructureLegacy& aRTStructure) const;

      /*! @brief Get polygon indexs in the same contour-slice, which has intersection with this voxel. If the voxel intersect > 1 Slice, the slice with smallert 
      * index will be used.
      *  @return Return vector of index number of the polygons stored in the structure vector(ranked by z coord. of the polygon), if size=0: out of structure
      */

      std::vector<int> getZIntersectPolygonIndexVector(const StructureLegacy& aRTStructure) const;

      /*! @brief Get the left upper point of the dose voxel in world coordinate.
      *
      */
      WorldCoordinate3D getLeftUpperPoint() const;
	  LegacyWorldCoordinate3D getLeftUpperPointLegacy() const;

      /*! @brief Get (_deltaX, _deltaY, _deltaZ) of the dose voxel in world coordinate.
      *
      */
      WorldCoordinate3D getDelta() const;
	  LegacyWorldCoordinate3D getDeltaLegacy() const;

      /*! @brief Calculate the interseciton points with the segment [firstPoint, secontPoint]*/
      std::vector<WorldCoordinate3D> calcIntersectionPoints(WorldCoordinate3D firstPoint, WorldCoordinate3D secondPoint);
	  //same method with legacy data structures
	  std::vector<LegacyWorldCoordinate3D> calcIntersectionPoints(LegacyWorldCoordinate3D firstPoint, LegacyWorldCoordinate3D secondPoint);

      /*! @brief Get the voxel index*/
      const LegacyDoseVoxelIndex3D& getVoxelIndex3D() const;	  

      /*! @brief Get the geometric information*/
      const core::GeometricInfo* getGeometricInfo() const;

      /*! @brief Set the voxel proportion inside a given structure*/
      void setProportionInStr(FractionType aProportionInStr);

      /*! @brief Get the voxel proportion inside a given structure*/
      FractionType getProportionInStr() const;
    };
}
  }
}

#endif
