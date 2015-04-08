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
#ifndef __IterativePolygonInTermsOfIntersections_H
#define __IterativePolygonInTermsOfIntersections_H

#include "rttbBaseType.h"
#include "rttbStructure.h"
#include "rttbDoseIteratorInterface.h"
#include "rttbField_LEGACY.h"
#include "rttbMaskType_LEGACY.h" // sollen Typen in Core neu definiert werden?
#include "rttbContour_LEGACY.h"
#include <math.h>
#include <deque>



namespace rttb{
	namespace masks{
		namespace legacy{




			// Die folgende Klasse handhabt die konkrete Speicherung der Schnittpunkte.
			// Kennt sich also mit der Speicherung der Schnittpunkte der Konturen mit den Voxelkanten aus.
			// Information auf Struktur-Polygonebene.
			// Nimmt PolygonInfo den Zugriff auf diese Information ab. 






			class IterativePolygonInTermsOfIntersections{


			public:

				IterativePolygonInTermsOfIntersections();
				~IterativePolygonInTermsOfIntersections(){};

				void MarkRelevantItems( LegacyDoseVoxelIndex3D aDoseIndex );
				bool ItIsAsExpected( masks::legacy::PolygonInTermsOfIntersections Expected_Unified_Polygon_Intersections );
				bool CheckToBeRegarded();
				void ShowSelf();

				class WhichIntersection{

				public:

					WhichIntersection(){
						point_index = 0;
						intersection_index = 0;
						column_raw_or_unified = 0;        
					} 

					~WhichIntersection(){};

					unsigned int point_index;
					unsigned int intersection_index;
					unsigned int column_raw_or_unified;  
					// 0 bedeutet Spalte, 1 bedeutet Zeile und 2 bedeutet vereinheitlichte Liste der Zeilen uns Spaltenschnittpunkte    

				};

				void ResetIntersectionIndex();   
				bool ThisIntersection( LegacyWorldCoordinate2D& the_intersection ); 
				void RememberPosition();
				bool IncremtentIntersection();
				bool IncremtentIntersectionPeriodically();
				bool NextIntersectionPeriodically( LegacyWorldCoordinate2D& the_next_intersection );
				bool NextPointOrIntersectionPeriodically( LegacyWorldCoordinate2D& previous_point_or_intersection , LegacyWorldCoordinate2D& next_point_or_intersection , LegacyDoseVoxelIndex3D& the_dose_index );      
				int  MaximumNumberOfElements();
				bool ThereAreIntersections();
				void JumpToRememberedPosition();
				bool RunForwardToNextIntersection( LegacyWorldCoordinate2D& point_of_interest_start_ref , LegacyWorldCoordinate2D& point_of_interest_end_ref , std::vector<LegacyWorldCoordinate3D>& snippet_intermediate_content_ref , LegacyDoseVoxelIndex3D the_dose_index  );
				bool RunBackwardToNextIntersection( LegacyWorldCoordinate2D& point_of_interest_start_ref , LegacyWorldCoordinate2D& point_of_interest_end_ref , std::vector<LegacyWorldCoordinate3D>& snippet_intermediate_content_ref  );      
				void CleanOut( LegacyWorldCoordinate2D& point_of_interest_start_ref , LegacyWorldCoordinate2D& point_of_interest_end_ref , std::vector<LegacyWorldCoordinate3D>& snippet_intermediate_content_ref );    
				bool NextIntersection( LegacyWorldCoordinate2D& the_next_intersection );    
				bool PreviousPointOrIntersectionPeriodically( LegacyWorldCoordinate2D& previous_point_or_intersection , LegacyWorldCoordinate2D& point_or_intersection , LegacyDoseVoxelIndex3D& the_dose_index );      
				bool PreviousIntersectionPeriodically( LegacyWorldCoordinate2D& the_intersection );   
				void ResetIntersectionIndex( WhichIntersection& intersection_set_to );  
				void SetIntersectionsAndResetIntersectionIndex( const masks::legacy::PolygonInTermsOfIntersections& Polygon_Intersections_In );
				bool CheckIsIntersection( LegacyWorldCoordinate2D& edge_coord , WhichIntersection& the_intersection_index_ref );
				LegacyWorldCoordinate3D GetRestpectivePologonPointInVoxelCoordinates( WhichIntersection& the_intersection_index_ref );   
				LegacyWorldCoordinate3D GetRestpectivePologonPointInVoxelCoordinatesFurther( WhichIntersection& the_intersection_index_ref , int ahead );   
				LegacyWorldCoordinate3D GetRestpectivePologonPointInVoxelCoordinatesPrevious( WhichIntersection& the_intersection_index_ref , unsigned int back );             
				bool selfTest();
				bool CheckCurrentIndexInitForTest();
				void SetCurrentIntersectionIndex( IterativePolygonInTermsOfIntersections::WhichIntersection& intersection_index_new_ref );
				bool CheckCurrentIntersectionIndexIdentical( IterativePolygonInTermsOfIntersections::WhichIntersection& intersection_index_compare_ref );
				void PrintIntersectionIndex();

			private:    

				unsigned int GetRestpectiveIntersectionVectorLength( WhichIntersection& the_intersection_index_ref );
				LegacyWorldCoordinate2D GetRestpectiveIntersection( WhichIntersection& the_intersection_index_ref );
				LegacyWorldCoordinate2D GetRestpectiveIntersectionFurther(  WhichIntersection& the_intersection_index_ref , int ahead );   
				bool PointIndexIsFine( WhichIntersection& the_intersection_index_ref );  
				LegacyWorldCoordinate3D GetFirstPointThisPolygon( WhichIntersection& the_intersection_index_ref );                
				bool AppendIntermediatePointOrCloseForward( LegacyWorldCoordinate2D& point_of_interest_start_ref ,  LegacyWorldCoordinate2D& point_of_interest_end_ref , std::vector<LegacyWorldCoordinate3D>& snippet_intermediate_content_ref );         
				bool AppendIntermediatePointOrCloseBackward(LegacyWorldCoordinate2D& point_of_interest_start_ref ,  LegacyWorldCoordinate2D& point_of_interest_end_ref , std::vector<LegacyWorldCoordinate3D>& snippet_intermediate_content_ref );     
				void AppendThisIntermediatePoint( std::vector<LegacyWorldCoordinate3D>& snippet_intermediate_content_ref ); 
				bool IncrementNeeded();
				bool DecrementNeeded(); 
				bool OneDiffers(  std::vector<LegacyWorldCoordinate3D>&  snippet_intermediate_content_ref ,  LegacyWorldCoordinate2D&  point_of_interest_start_ref  );
				bool IdenticalUniList( masks::legacy::PointLevelVector plv_a , masks::legacy::PointLevelVector plv_b ); 
				bool CloseWithoutIncrement( LegacyWorldCoordinate2D& point_of_interest_start_ref, LegacyWorldCoordinate2D& point_of_interest_end_ref , std::vector<LegacyWorldCoordinate3D>& snippet_intermediate_content_ref ); 
				bool CloseWithoutDecrement( LegacyWorldCoordinate2D& point_of_interest_start_ref,  LegacyWorldCoordinate2D& point_of_interest_end_ref , std::vector<LegacyWorldCoordinate3D>& snippet_intermediate_content_ref );   
				void PeriodicPolygonPointIncrement(); 
				void PeriodicPolygonPointDecrement();     
				void CopyIntersectionIndex( WhichIntersection& to_ref  ,  WhichIntersection& from_ref  );        
				bool CheckIndex();   
				bool DecrementPointOrIntersectionPeriodically();      
				bool DecrementIntersectionPeriodically();         
				void CreateUnifiedListOfRawAndColumnIntersections();         
				void removeThisCoordinateFromBothLists( LegacyWorldCoordinate2D the_coordinate , std::vector<LegacyWorldCoordinate2D>& raw_intersections_work_ref , std::vector<LegacyWorldCoordinate2D>& column_intersections_work_ref );
				bool VoxelInBetween( LegacyDoseVoxelIndex3D aDoseIndex , LegacyWorldCoordinate3D wc3da, LegacyWorldCoordinate3D wc3db );         
				bool IsInside( LegacyWorldCoordinate3D& point_check ,  LegacyDoseVoxelIndex3D& the_dose_index );         

				PolygonInTermsOfIntersections Polygon_Intersections;
				WhichIntersection current_index_internal;
				WhichIntersection current_index_internal_remember; 
				int number_of_intersections;     

			};
		}
	}
} 



#endif 