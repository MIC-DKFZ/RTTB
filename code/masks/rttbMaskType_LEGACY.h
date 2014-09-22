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
#ifndef __MASK_TYPE_H
#define __MASK_TYPE_H

#include "rttbBaseType_LEGACY.h"
#include <list>


namespace rttb{
	namespace masks{
		namespace legacy
  {

    /*! @brief PointLevelVector
                 This structure represents one point of the structure and the intersections of the following 
                 contour segment with the voxel edges. Contour segment, that is a line connecting this contour point and the next one.
                 contour_point_voxel_coord represents the contour point in voxel coordinates.
                 intersections_raw_and_column represents all the intersections with the voxel eges sorted in a way,
                 so that the one that is closest to the contour point represented by this PointLevelVector is the first one. 
                 The one that is furthest from the contour point represented by this PointLevelVector is the last one 
                 that appears in intersections_raw_and_column.
                 raw_intersections is just like intersections_raw_and_column, but represents the intersections with the voxel edges 
                 that are parallel to the voxel raws.
                 column_intersections is like raw_intersections but represents the intersections with the voxel edges that 
                 are parallel to the voxel columns.
    */

    struct PointLevelVector{

      PointLevelVector(){
        to_be_Regarded = 1;
      }		  

      ~PointLevelVector(){
        raw_intersections.clear();
        column_intersections.clear();
        intersections_raw_and_column.clear();	  		  
      }

	  std::vector<LegacyWorldCoordinate2D> raw_intersections;
      std::vector<LegacyWorldCoordinate2D> column_intersections;
      std::vector<LegacyWorldCoordinate2D> intersections_raw_and_column;
      LegacyWorldCoordinate3D contour_point_voxel_coord; 

      bool to_be_Regarded;
    };

    /*! @typedef correspoinding_structure_vector
    */
    typedef  std::vector<int> correspoinding_structure_vector;	

    /*! @typedef PolygonLevelVector
    */
    typedef std::vector<PointLevelVector> PolygonLevelVector;

    /*! @typedef SliceLevelVector
    */
    typedef std::vector<PolygonLevelVector> SliceLevelVector;

    /*! @typedef field_content_type
    */
    typedef int field_content_type;

    /*! @typedef VoxelIntersectionPointsMap
    */
    typedef std::map<  LegacyDoseVoxelIndex3D,std::vector<LegacyWorldCoordinate3D>  > VoxelIntersectionPointsMap; 

    /*! @brief Characterizes the relationship between the voxel edge (in this position that this CharacterizeIntersection object belongs to) 
                and the contour that is in touch with the edge right here. 
               real_intersection for example means that here the structure really intersects the voxel edge.
               inside_to_edge means that the structure, coming from inside the voxel touches the voxel edge here and then runs along the voxel edge, touching rather than intersecting.
               edge_to_edge means that the structure goes along the edge and has a contour point here. So it comes and goes along the edge and has a point here.
               outside_to_edge means that here the contour comes from outside the voxel and continues to run along the edge. So here in this position represented by this CharacterizeIntersection the contour does not actually run into the voxel.
               inside_inside_touches means that the contour comes from inside the voxel, does have a point here in this position right on the edge and continues its way going back into the voxel. So the contour just touches the voxel edge and does not really intersect here.
               corner : this is just a corner. 
               unknown nobody knows what that is.               
    */    
    enum CharacterizeIntersection{ unknown = -1 , real_intersection = 0 , edge_to_edge = 1 , inside_to_edge = 2 , edge_to_inside = 3 , outside_to_edge = 4 , edge_to_outside = 5 , inside_inside_touches = 6, outside_outside_touches = 7, corner = 8 }; 

    typedef LegacyPolygonType::iterator LegacyPolygonTypeIterator;

    typedef std::vector<PointLevelVector> PolygonInTermsOfIntersections;

    typedef std::vector<PolygonInTermsOfIntersections> StructureInTermsOfIntersections;

    typedef std::list<LegacyUnsignedIndex3D> UnsignedIndexList;

		}
  }
} 







#endif 