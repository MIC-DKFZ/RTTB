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

//Relocate?

#include "rttbMask.h"
#include "rttbIterativePolygonInTermsOfIntersections_LEGACY.h"

#include "rttbNullPointerException.h"

#include <map>
#include <utility>
#include <assert.h>
#include <algorithm>
#include <list>




// Generelle Anmerkung:
// Koordinatensystem von Bild und Strukturen auf Kompatibilitaet pruefen!
// Strukturen auf Gutartigkeit pruefen:
// Ein Polygonzug darf sich nicht mit sich selbst schneiden. Ist eine Struktur innerhalb eines Slices durch 
// zwei Polygonzuege repraesentiert, so muessen diese distinkt sein, duerfen eineander also nicht ueberschneiden!



namespace rttb{
  namespace masks{
	  namespace legacy{


    IterativePolygonInTermsOfIntersections::IterativePolygonInTermsOfIntersections(){
      number_of_intersections = -1;
    }


    bool IterativePolygonInTermsOfIntersections::selfTest(){

      std::cout<< " Hallo, ich bin die funktion selfTest von IterativePolygonInTermsOfIntersections und ich bin noch nicht implementiert. Trotzdem returne ich true. Das musst du aendern. " <<std::endl;
      return true;
    }


    bool IterativePolygonInTermsOfIntersections::CheckCurrentIndexInitForTest(){ 
      if( ( current_index_internal.point_index != 0 ) || ( current_index_internal.intersection_index != 0 ) || ( current_index_internal.column_raw_or_unified != 2 ) )return false; 
      else return true;
    }


    void IterativePolygonInTermsOfIntersections::SetIntersectionsAndResetIntersectionIndex( const masks::legacy::PolygonInTermsOfIntersections& Polygon_Intersections_In ){
      Polygon_Intersections = Polygon_Intersections_In;
      //CreateUnifiedListOfRawAndColumnIntersections();
      ResetIntersectionIndex();  
    }


    void IterativePolygonInTermsOfIntersections::ResetIntersectionIndex(){
      current_index_internal.point_index = 0;
      current_index_internal.intersection_index = 0;
      current_index_internal.column_raw_or_unified = 2; 
    }  


    void IterativePolygonInTermsOfIntersections::PrintIntersectionIndex(){
      std::cout<< " current_index_internal.point_index : " << current_index_internal.point_index <<std::endl;
      std::cout<< " current_index_internal.intersection_index : " << current_index_internal.intersection_index <<std::endl; 
    }  

    unsigned int IterativePolygonInTermsOfIntersections::GetRestpectiveIntersectionVectorLength( IterativePolygonInTermsOfIntersections::WhichIntersection& the_intersection_index_ref ){                  
      if( the_intersection_index_ref.column_raw_or_unified == 0 ){
        return Polygon_Intersections.at( the_intersection_index_ref.point_index ).column_intersections.size();
      }
      else if( the_intersection_index_ref.column_raw_or_unified == 1 ){
        return Polygon_Intersections.at( the_intersection_index_ref.point_index ).raw_intersections.size(); 
      }        
      else{
        return Polygon_Intersections.at( the_intersection_index_ref.point_index ).intersections_raw_and_column.size(); 
      }          
    }


    LegacyWorldCoordinate2D IterativePolygonInTermsOfIntersections::GetRestpectiveIntersection( IterativePolygonInTermsOfIntersections::WhichIntersection& the_intersection_index_ref ){

      LegacyWorldCoordinate2D coordinates_return;                

      if( the_intersection_index_ref.column_raw_or_unified == 0 ){
        coordinates_return = Polygon_Intersections.at( the_intersection_index_ref.point_index ).column_intersections.at( the_intersection_index_ref.intersection_index );
      }
      else if( the_intersection_index_ref.column_raw_or_unified == 1 ){
        coordinates_return = Polygon_Intersections.at( the_intersection_index_ref.point_index ).raw_intersections.at( the_intersection_index_ref.intersection_index ); 
      }                  
      else{
        coordinates_return = Polygon_Intersections.at( the_intersection_index_ref.point_index ).intersections_raw_and_column.at( the_intersection_index_ref.intersection_index );     
      }

      return coordinates_return;

    }



    LegacyWorldCoordinate2D IterativePolygonInTermsOfIntersections::GetRestpectiveIntersectionFurther(  IterativePolygonInTermsOfIntersections::WhichIntersection& the_intersection_index_ref , int ahead ){

      LegacyWorldCoordinate2D coordinates_return;       

      if( GetRestpectiveIntersectionVectorLength( the_intersection_index_ref ) > ( the_intersection_index_ref.intersection_index + ahead ) ){         

        if( the_intersection_index_ref.column_raw_or_unified == 0 ){

          coordinates_return = Polygon_Intersections.at( the_intersection_index_ref.point_index ).column_intersections.at( the_intersection_index_ref.intersection_index + ahead );

        }
        else if( the_intersection_index_ref.column_raw_or_unified == 1 ){

          coordinates_return = Polygon_Intersections.at( the_intersection_index_ref.point_index ).raw_intersections.at( the_intersection_index_ref.intersection_index + ahead ); 

        } 
        else{

          coordinates_return = Polygon_Intersections.at( the_intersection_index_ref.point_index ).intersections_raw_and_column.at( the_intersection_index_ref.intersection_index + ahead ); 

        }

      } 
      else{

        if(      GetRestpectiveIntersectionVectorLength( the_intersection_index_ref ) <= ( the_intersection_index_ref.intersection_index + ahead )          ){

          GridIndexType control_index = ( ( the_intersection_index_ref.intersection_index + ahead ) -  GetRestpectiveIntersectionVectorLength( the_intersection_index_ref ) );
          if( ( control_index >= 0 ) && ( control_index <  GetRestpectiveIntersectionVectorLength( the_intersection_index_ref ) ) ){
            coordinates_return = Polygon_Intersections.at( the_intersection_index_ref.point_index ).intersections_raw_and_column.at(       ( ( the_intersection_index_ref.intersection_index + ahead ) -  GetRestpectiveIntersectionVectorLength( the_intersection_index_ref ) )        );                
          }
          else assert(0); //   the_intersection_index_ref and ahead have been initially unreasonalble

        }

      }                

      return coordinates_return;

    }


    bool IterativePolygonInTermsOfIntersections::PointIndexIsFine( IterativePolygonInTermsOfIntersections::WhichIntersection& the_intersection_index_ref ){

      if(    ( Polygon_Intersections.size() > ( the_intersection_index_ref.point_index ) )   &&   (  ( the_intersection_index_ref.point_index ) >= 0 )   ){
        return true;
      }
      else return false;

    }


    LegacyWorldCoordinate3D IterativePolygonInTermsOfIntersections::GetRestpectivePologonPointInVoxelCoordinates( IterativePolygonInTermsOfIntersections::WhichIntersection& the_intersection_index_ref ){

      LegacyWorldCoordinate3D coordinates_return;                
      if( PointIndexIsFine( the_intersection_index_ref ) ){
        coordinates_return = Polygon_Intersections.at( the_intersection_index_ref.point_index ).contour_point_voxel_coord;                 
      }
      else{
        // exception to be implemented
        assert(0);
      }   
      return coordinates_return;

    }


    LegacyWorldCoordinate3D IterativePolygonInTermsOfIntersections::GetRestpectivePologonPointInVoxelCoordinatesFurther( IterativePolygonInTermsOfIntersections::WhichIntersection& the_intersection_index_ref , int ahead ){

      // Baustelle zyklisch machen

      LegacyWorldCoordinate3D coordinates_return;                

      if(  Polygon_Intersections.size() > ( the_intersection_index_ref.point_index + ahead )  ){ 
        coordinates_return = Polygon_Intersections.at( the_intersection_index_ref.point_index + ahead ).contour_point_voxel_coord;
      }
      else{

        if( (  ahead - (  Polygon_Intersections.size() - the_intersection_index_ref.point_index ) ) >=  Polygon_Intersections.size() ) assert( 0 ); // this must never happen. Probably the contour consists of just one point. That does not make any sense in context of radiotherapy.      

        coordinates_return = Polygon_Intersections.at(  ahead - (  Polygon_Intersections.size() - the_intersection_index_ref.point_index ) ).contour_point_voxel_coord;

      }              

      return coordinates_return;

    }


    LegacyWorldCoordinate3D IterativePolygonInTermsOfIntersections::GetRestpectivePologonPointInVoxelCoordinatesPrevious( IterativePolygonInTermsOfIntersections::WhichIntersection& the_intersection_index_ref , unsigned int back ){

      LegacyWorldCoordinate3D coordinates_return;           

      if(  ( ( the_intersection_index_ref.point_index  ) >= back ) && (  ( the_intersection_index_ref.point_index  ) < ( Polygon_Intersections.size() + back )  ) ){ 

        coordinates_return = Polygon_Intersections.at( the_intersection_index_ref.point_index - back ).contour_point_voxel_coord;         

      }
      else{

        if( the_intersection_index_ref.point_index < back ){

          int control_index = ( Polygon_Intersections.size() - ( back - the_intersection_index_ref.point_index )  );

          if( control_index < 0  ){
            assert(0); // This must never happen. Probably the contour consists of just one point. Each point has already been regarded. That does not make any sense in radiotherapy.
          }

          coordinates_return =  Polygon_Intersections.at( Polygon_Intersections.size() - ( back - the_intersection_index_ref.point_index )  ).contour_point_voxel_coord;

        }

        if( !  (  ( the_intersection_index_ref.point_index  ) < ( Polygon_Intersections.size() + back )  )  ){

          assert(0); // that can't be right the_intersection_index_ref.point_index must have been initially unreasonable

        }

      }              

      return coordinates_return;

    }


    LegacyWorldCoordinate3D IterativePolygonInTermsOfIntersections::GetFirstPointThisPolygon( IterativePolygonInTermsOfIntersections::WhichIntersection& the_intersection_index_ref ){  

		LegacyWorldCoordinate3D first_one_this_Polygon(0,0,0);

      if( Polygon_Intersections.size() > 0 ){    
        first_one_this_Polygon = Polygon_Intersections.at( 0 ).contour_point_voxel_coord; 
        return first_one_this_Polygon;
      }
      else{
        assert(0);
        // exception implementieren
      }

      return first_one_this_Polygon;

    }



    void IterativePolygonInTermsOfIntersections::ResetIntersectionIndex( IterativePolygonInTermsOfIntersections::WhichIntersection& intersection_set_to ){
      current_index_internal.point_index =  intersection_set_to.point_index ;
      current_index_internal.intersection_index =  intersection_set_to.intersection_index ;
      current_index_internal.column_raw_or_unified =  intersection_set_to.column_raw_or_unified ;
    }


    // index goes in for debug and will be removed later ... 
    bool IterativePolygonInTermsOfIntersections::RunForwardToNextIntersection( LegacyWorldCoordinate2D& point_of_interest_start_ref , LegacyWorldCoordinate2D& point_of_interest_end_ref , std::vector<LegacyWorldCoordinate3D>& snippet_intermediate_content_ref , LegacyDoseVoxelIndex3D the_dose_index ){


      bool success = false;
      bool done = false;
      bool success_first_increment = false;

      RememberPosition();

      if( ThisIntersection( point_of_interest_start_ref ) ){

        snippet_intermediate_content_ref.clear();

        if( ! CloseWithoutIncrement( point_of_interest_start_ref, point_of_interest_end_ref , snippet_intermediate_content_ref ) ){                                                                                      

          PeriodicPolygonPointIncrement();

          AppendThisIntermediatePoint( snippet_intermediate_content_ref ); 

          while( !done ){

            // done = AppendIntermediatePointOrCloseForward( point_of_interest_start_ref , point_of_interest_end_ref , snippet_intermediate_content_ref ); 
            done = CloseWithoutIncrement( point_of_interest_start_ref, point_of_interest_end_ref , snippet_intermediate_content_ref );

            if( !done ){

              PeriodicPolygonPointIncrement();
          
              AppendThisIntermediatePoint( snippet_intermediate_content_ref );

            }

          }                                                                      

        } 

      }
      else assert(0);
      
      CleanOut( point_of_interest_start_ref, point_of_interest_end_ref , snippet_intermediate_content_ref );

      JumpToRememberedPosition();

      return success;

    }    


    void IterativePolygonInTermsOfIntersections::CleanOut( LegacyWorldCoordinate2D& point_of_interest_start_ref , LegacyWorldCoordinate2D& point_of_interest_end_ref , std::vector<LegacyWorldCoordinate3D>& snippet_intermediate_content_ref ){

      if( snippet_intermediate_content_ref.size() > 0 ){

        bool im_done_a = false;
        while( (!im_done_a) && (snippet_intermediate_content_ref.size() > 0) ){
        
          if( ( point_of_interest_start_ref.x == snippet_intermediate_content_ref.at( 0 ).x ) && ( point_of_interest_start_ref.y == snippet_intermediate_content_ref.at( 0 ).y ) )snippet_intermediate_content_ref.erase( snippet_intermediate_content_ref.begin() );
          else im_done_a = true;
          
        }
        
        bool im_done_b = false;
        while( (!im_done_b) && (snippet_intermediate_content_ref.size() > 0) ){
          
          if( ( point_of_interest_end_ref.x == snippet_intermediate_content_ref.at( snippet_intermediate_content_ref.size() - 1 ).x ) && ( point_of_interest_end_ref.y == snippet_intermediate_content_ref.at( snippet_intermediate_content_ref.size() - 1 ).y ) )snippet_intermediate_content_ref.pop_back();
          else im_done_b = true;
          
        }
        
      }  
        
    }


    bool IterativePolygonInTermsOfIntersections::RunBackwardToNextIntersection( LegacyWorldCoordinate2D& point_of_interest_start_ref , LegacyWorldCoordinate2D& point_of_interest_end_ref , std::vector<LegacyWorldCoordinate3D>& snippet_intermediate_content_ref  ){

      bool success = false;
      bool done = false;
      bool success_first_increment = false;

      RememberPosition();

      if( ThisIntersection( point_of_interest_start_ref ) ){

        snippet_intermediate_content_ref.clear(); 


        if( ! CloseWithoutDecrement( point_of_interest_start_ref, point_of_interest_end_ref , snippet_intermediate_content_ref )  ){             
   

          AppendThisIntermediatePoint( snippet_intermediate_content_ref );                                                                            

          PeriodicPolygonPointDecrement();  

          while( ( !done ) ){

            done = CloseWithoutDecrement( point_of_interest_start_ref, point_of_interest_end_ref , snippet_intermediate_content_ref );

            if( !done ){

              AppendThisIntermediatePoint( snippet_intermediate_content_ref ); 
              
              PeriodicPolygonPointDecrement(); 

            }  

          }

        }

      }
      else assert(0); // This must never happen !

      CleanOut( point_of_interest_start_ref, point_of_interest_end_ref , snippet_intermediate_content_ref );

      JumpToRememberedPosition();    

      return success;

    }    


    bool IterativePolygonInTermsOfIntersections::AppendIntermediatePointOrCloseForward(  LegacyWorldCoordinate2D& point_of_interest_start_ref , LegacyWorldCoordinate2D& point_of_interest_end_ref , std::vector<LegacyWorldCoordinate3D>& snippet_intermediate_content_ref ){

      bool got_it = false;

      if( Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.size() != 0 ){ 

        
        while( current_index_internal.intersection_index < ( Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.size() - 1 ) ){
        
            current_index_internal.intersection_index++;
            LegacyWorldCoordinate2D point_of_interest = Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.at( current_index_internal.intersection_index );
        
        
        }


        if(   ( static_cast<float>( point_of_interest_start_ref.x ) != static_cast<float>( Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.at(0).x)   ) || (     static_cast<float>( point_of_interest_start_ref.y ) != static_cast<float>( Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.at(0).y )    )     ){

          got_it = true;

          point_of_interest_end_ref = Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.at( 0 );

          snippet_intermediate_content_ref.push_back( Polygon_Intersections.at( current_index_internal.point_index ).contour_point_voxel_coord );

        }
        else{

          // Anmerkung : man kann die else Bedingung auch oben in die if Bedingung mit reinpacken, je nachdem, was der code-reviewer leserlicher findet                                                       
          // sie sind gleich. Die Frage ist nun, ob es Zwischenpunkte gab, ob die Punkte also noch, oder wieder gleich sind.
          if( snippet_intermediate_content_ref.size() > 0 ){

            got_it = true;

            point_of_interest_end_ref = Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.at( 0 );

            snippet_intermediate_content_ref.push_back( Polygon_Intersections.at( current_index_internal.point_index ).contour_point_voxel_coord );

          }

        }


      }
      else{

        snippet_intermediate_content_ref.push_back( Polygon_Intersections.at( current_index_internal.point_index ).contour_point_voxel_coord );      

      }

      return got_it;

    }



    bool IterativePolygonInTermsOfIntersections::AppendIntermediatePointOrCloseBackward(   LegacyWorldCoordinate2D& point_of_interest_start_ref , LegacyWorldCoordinate2D& point_of_interest_end_ref , std::vector<LegacyWorldCoordinate3D>& snippet_intermediate_content_ref ){
    

         if( current_index_internal.intersection_index != ( Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.size() - 1 ) ) assert(0); //this should not happen
 
         while( current_index_internal.intersection_index > 0 ){
       
           current_index_internal.intersection_index--;
           LegacyWorldCoordinate2D point_of_interest = Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.at( current_index_internal.intersection_index );
           if( ( static_cast<float>( point_of_interest.x ) != static_cast<float>( point_of_interest_start_ref.x ) ) && ( static_cast<float>( point_of_interest.y ) != static_cast<float>( point_of_interest_start_ref.y ) ) ){
          
             point_of_interest_end_ref = point_of_interest;
             return true;
          
           }
       
         }


         snippet_intermediate_content_ref.push_back( Polygon_Intersections.at( current_index_internal.point_index ).contour_point_voxel_coord );      

         return false;

    }


    void IterativePolygonInTermsOfIntersections::AppendThisIntermediatePoint( std::vector<LegacyWorldCoordinate3D>& snippet_intermediate_content_ref ){
      snippet_intermediate_content_ref.push_back( Polygon_Intersections.at( current_index_internal.point_index ).contour_point_voxel_coord );      
    }


    bool IterativePolygonInTermsOfIntersections::IncrementNeeded(){
      if( Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.size() <= ( current_index_internal.intersection_index + 1 )  ) return true;
      return false;
    }


    bool IterativePolygonInTermsOfIntersections::DecrementNeeded(){
      bool is_needed = false;
      if( current_index_internal.intersection_index < 1 ) is_needed = true;
      return is_needed;
    }               


    bool IterativePolygonInTermsOfIntersections::CloseWithoutIncrement( LegacyWorldCoordinate2D& point_of_interest_start_ref, LegacyWorldCoordinate2D& point_of_interest_end_ref , std::vector<LegacyWorldCoordinate3D>& snippet_intermediate_content_ref ){
      
      LegacyWorldCoordinate2D point_of_interest;   

      if( CheckIndex() ){
      
        point_of_interest = Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.at( current_index_internal.intersection_index ); 

        if(   (  ( static_cast<float>( point_of_interest.x ) != static_cast<float>( point_of_interest_start_ref.x ) )  ||  ( static_cast<float>( point_of_interest.y ) != static_cast<float>( point_of_interest_start_ref.y ) )  ) ||   OneDiffers(  snippet_intermediate_content_ref , point_of_interest_start_ref  )   ){

           point_of_interest_end_ref = point_of_interest;
           
           return true;
            
        }
        
      }  
      
      if( ! IncrementNeeded() ){
      
        while( current_index_internal.intersection_index < ( Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.size() - 1 )  ){
        
           current_index_internal.intersection_index++;
           
           point_of_interest = Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.at( current_index_internal.intersection_index ); 

           if(  (  static_cast<float>( point_of_interest.x )  !=  static_cast<float>( point_of_interest_start_ref.x )  )  ||  (  static_cast<float>( point_of_interest.y )  != static_cast<float>( point_of_interest_start_ref.y )  )  ){


             point_of_interest_end_ref = point_of_interest;

        
             return true;
            
           }
        
        }
        
        return false;
      
      }
      else return false;
      
    }



    bool IterativePolygonInTermsOfIntersections::OneDiffers(  std::vector<LegacyWorldCoordinate3D>&  snippet_intermediate_content_ref ,  LegacyWorldCoordinate2D&  point_of_interest_start_ref  ){ 
  
       for( GridIndexType i = 0 ; i < snippet_intermediate_content_ref.size() ; i++ ){
       
         LegacyWorldCoordinate3D a_point =  snippet_intermediate_content_ref.at( i );
         
         if( ( a_point.x != point_of_interest_start_ref.x ) || ( a_point.y != point_of_interest_start_ref.y ) )return true; 
       
       }
  
       return false;
  
    }



    bool IterativePolygonInTermsOfIntersections::CloseWithoutDecrement( LegacyWorldCoordinate2D& point_of_interest_start_ref,  LegacyWorldCoordinate2D& point_of_interest_end_ref , std::vector<LegacyWorldCoordinate3D>& snippet_intermediate_content_ref ){
    
      LegacyWorldCoordinate2D point_of_interest;
    
      if(CheckIndex()){
    
        point_of_interest = Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.at( current_index_internal.intersection_index  );          

        if(   ( ( static_cast<float>( point_of_interest.x ) != static_cast<float>( point_of_interest_start_ref.x ) ) || ( static_cast<float>( point_of_interest.y ) != static_cast<float>( point_of_interest_start_ref.y ) ) )  ||  OneDiffers(  snippet_intermediate_content_ref , point_of_interest_start_ref  )    ){
         
          point_of_interest_end_ref = point_of_interest;

          return true;
            
        } 
      
      }

      if( ! DecrementNeeded() ){  

        while( current_index_internal.intersection_index > 0 ){
        
          current_index_internal.intersection_index--;
          
          point_of_interest = Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.at( current_index_internal.intersection_index  );          

          if( ( static_cast<float>( point_of_interest.x ) != static_cast<float>( point_of_interest_start_ref.x ) ) || ( static_cast<float>( point_of_interest.y ) != static_cast<float>( point_of_interest_start_ref.y ) ) ){
         
            point_of_interest_end_ref = point_of_interest;

            return true;
            
          }  
        
        }  
        
        return false;

      
      }
      else return false;
    
    } 



    void IterativePolygonInTermsOfIntersections::PeriodicPolygonPointIncrement(){
      if( current_index_internal.point_index < ( Polygon_Intersections.size() - 1 )  ){
        current_index_internal.point_index++;
      }
      else{
        current_index_internal.point_index = 0;
      } 
      current_index_internal.intersection_index = 0;
    }


    void IterativePolygonInTermsOfIntersections::PeriodicPolygonPointDecrement(){
      if( current_index_internal.point_index > 0 ){
        current_index_internal.point_index--;
      }
      else{
        current_index_internal.point_index = ( Polygon_Intersections.size() - 1 );
      } 
      if(  Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.size()  >  0 ) current_index_internal.intersection_index = Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.size() - 1 ;
      else current_index_internal.intersection_index = 0; 
    }



    ///
    ///  returns true, in case                 intersection
    ///
    bool IterativePolygonInTermsOfIntersections::CheckIsIntersection( LegacyWorldCoordinate2D& edge_coord , IterativePolygonInTermsOfIntersections::WhichIntersection& the_intersection_index_ref ){
      // Baustelle all control paths should return a value

		LegacyWorldCoordinate2D local_current_position = {0,0};

      RememberPosition();

      ResetIntersectionIndex();

      bool was_able_to_increment = true;

      while(  was_able_to_increment == true  ){

   //     if( CheckToBeRegarded() ){   

        if( ThisIntersection( local_current_position ) ){   


          if(  ( static_cast<float>( edge_coord.x ) == static_cast<float>( local_current_position.x ) ) && (  static_cast<float>( edge_coord.y ) == static_cast<float>( local_current_position.y )  )  ){

            IterativePolygonInTermsOfIntersections::WhichIntersection& current_index_internal_ref = current_index_internal;
            CopyIntersectionIndex( the_intersection_index_ref , current_index_internal_ref );

            JumpToRememberedPosition();

            return true;

          }      

        }  

     //   }  

        was_able_to_increment = IncremtentIntersection();

      }            

      JumpToRememberedPosition();    

      return false;

    }


    void IterativePolygonInTermsOfIntersections::CopyIntersectionIndex( IterativePolygonInTermsOfIntersections::WhichIntersection& to_ref  ,  IterativePolygonInTermsOfIntersections::WhichIntersection& from_ref  ){
      to_ref.point_index = from_ref.point_index;
      to_ref.intersection_index = from_ref.intersection_index;
      to_ref.column_raw_or_unified = from_ref.column_raw_or_unified;      
    }


    bool IterativePolygonInTermsOfIntersections::IncremtentIntersection(){

     

        if( Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.size() >  ( current_index_internal.intersection_index + 1 )  ){
          current_index_internal.intersection_index++;
          return true;
        }
        else{

          bool success = false;

          while(   success == false   ){

            if( Polygon_Intersections.size() > ( current_index_internal.point_index + 1  )  ){

              current_index_internal.intersection_index = 0;
              current_index_internal.point_index++;

              if( Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.size() > 0 ){

                return true;

              }

            }
            else return false;

          }   

        }

      

      return false;

    }


    bool IterativePolygonInTermsOfIntersections::IncremtentIntersectionPeriodically(){

      if( ( CheckIndex() ) && ( ThereAreIntersections() ) ){

        if( Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.size() >  ( current_index_internal.intersection_index + 1 )  ){
          current_index_internal.intersection_index++;
          return true;
        }
        else{

          bool success = false;

          while(   success == false   ){

            if( Polygon_Intersections.size() > ( current_index_internal.point_index + 1  )  ){

              current_index_internal.intersection_index = 0;
              current_index_internal.point_index++;

              if( Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.size() > 0 ){

                return true;

              }

            }
            else{

              current_index_internal.intersection_index = 0;
              current_index_internal.point_index = 0;

              if( Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.size() > 0 ){

                return true;

              }

            }

          }   

        }

      }
      else return false;
	  return false;

    }


    bool IterativePolygonInTermsOfIntersections::CheckIndex(){

      if(   Polygon_Intersections.size() > current_index_internal.point_index   ){

        if(  ( Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.size() > current_index_internal.intersection_index ) ){
          return true;
        }
        else return false;

      }
      else return false;

    }


    // Caution : This function can end up with a bad intersedction index.
    bool IterativePolygonInTermsOfIntersections::DecrementPointOrIntersectionPeriodically(){


      if( /*CheckIndex() &&*/ ( ThereAreIntersections() ) ){

        if( 0 < ( current_index_internal.intersection_index  ) ){
          current_index_internal.intersection_index--;
          return true;
        }
        else{

          if(  ( 0 <  ( current_index_internal.point_index )  )   ){


            current_index_internal.point_index--;
            if( Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.size() > 0 ) current_index_internal.intersection_index = Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.size() - 1;
            else current_index_internal.intersection_index = 0;

            return true;


          }
          else if( Polygon_Intersections.size() > 0 ){

            current_index_internal.point_index = Polygon_Intersections.size() - 1;

            if( Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.size() > 0 ) current_index_internal.intersection_index = ( Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.size() - 1 );
            else current_index_internal.intersection_index = 0;


          }   
          else assert(0); // this should never happen since there are intersections, see ThereAreIntersections()

        }

      }
      else{
      std::cout<< "  could not decrement !!!!!!!"  <<std::endl;
      
      bool check_index = CheckIndex();
      std::cout<< " check_index : " << check_index <<std::endl;

      bool there_are_intersections = ThereAreIntersections();
      std::cout<< " there_are_intersections : " << there_are_intersections <<std::endl;
       

       return false;
      }  
	  return false;
    }



    bool IterativePolygonInTermsOfIntersections::DecrementIntersectionPeriodically(){

      if( CheckIndex() && ( ThereAreIntersections() ) ){

        if( 0 < ( current_index_internal.intersection_index  ) ){
          current_index_internal.intersection_index--;
          return true;
        }
        else{

          while(  ( 0 <  ( current_index_internal.point_index )  )   ){


            current_index_internal.point_index--;
            if( Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.size() > 0 ) current_index_internal.intersection_index = Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.size() - 1;
            else current_index_internal.intersection_index = 0;

            if( Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.size() > 0 ){

              return true;

            }

          }

          if( current_index_internal.point_index != 0 ){
            assert(0); // This should never happen.
          }  
          else if( Polygon_Intersections.size() > 0 ){

            current_index_internal.point_index = Polygon_Intersections.size() - 1;

            if( Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.size() > 0 ) current_index_internal.intersection_index = ( Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.size() - 1 );
            else{


              while(  ( 0 <  ( current_index_internal.point_index )  )   ){

                current_index_internal.point_index--;
                if( Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.size() > 0 ) current_index_internal.intersection_index = Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.size() - 1;
                else current_index_internal.intersection_index = 0;

                if( Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.size() > 0 ){

                  return true;

                }

              }

              assert(0); // should never get here, since there are interesections

            } 

          }   
          else assert(0); // this should never happen since there are intersections, see ThereAreIntersections()

        }

      }
      else return false;
	  return false;
    }


    bool IterativePolygonInTermsOfIntersections::CheckToBeRegarded(){

      return Polygon_Intersections.at( current_index_internal.point_index ).to_be_Regarded;

    }


    bool IterativePolygonInTermsOfIntersections::PreviousIntersectionPeriodically( LegacyWorldCoordinate2D& the_intersection ){

      if( DecrementIntersectionPeriodically() ){
        the_intersection = Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.at( current_index_internal.intersection_index );
        return true;
      }
      else{  
        the_intersection.x =0;
        the_intersection.y = 0;
        return false;
      }
      return false;

    }



    bool IterativePolygonInTermsOfIntersections::IsInside( LegacyWorldCoordinate3D& point_check ,  LegacyDoseVoxelIndex3D& the_dose_index ){

      if( ( point_check.x > the_dose_index.x ) && ( point_check.x < ( the_dose_index.x + 1 ) ) && ( point_check.y > the_dose_index.y ) && ( point_check.y < ( the_dose_index.y + 1 ) ) ){

        return true;

      }
      else return false;

    }





    bool IterativePolygonInTermsOfIntersections::ThisIntersection( LegacyWorldCoordinate2D& the_intersection ){

      if( CheckIndex() ){
        the_intersection = Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.at( current_index_internal.intersection_index );
        return true;
      }
      else{
        the_intersection.x =0;
        the_intersection.y = 0;
        return false;
      }

    }
    
    
    void IterativePolygonInTermsOfIntersections::ShowSelf(){
    
      if( Polygon_Intersections.size() > 0 ){
    
        for( GridIndexType counter = 0 ; counter < Polygon_Intersections.size() ; counter++ ){

       std::cout<<   "  Polygon_Intersections.at( counter ).contour_point_voxel_coord.z  : "  <<   Polygon_Intersections.at( counter ).contour_point_voxel_coord.z  <<std::endl;

            if( ( Polygon_Intersections.at( counter ).contour_point_voxel_coord.x >= 78 ) && ( Polygon_Intersections.at( counter ).contour_point_voxel_coord.x <= 79 ) && ( Polygon_Intersections.at( counter ).contour_point_voxel_coord.y >= 70 ) && ( Polygon_Intersections.at( counter ).contour_point_voxel_coord.y <= 71 ) ){
              std::cout<<   "  Polygon_Intersections.at( counter ).contour_point_voxel_coord.x  : "  <<   Polygon_Intersections.at( counter ).contour_point_voxel_coord.x  <<std::endl;
              std::cout<<   "  Polygon_Intersections.at( counter ).contour_point_voxel_coord.y  : "  <<   Polygon_Intersections.at( counter ).contour_point_voxel_coord.y  <<std::endl;
            }

            for( GridIndexType internal_counter = 0 ; internal_counter < Polygon_Intersections.at( counter ).intersections_raw_and_column.size() ; internal_counter++ ){

               if( ( Polygon_Intersections.at( counter ).intersections_raw_and_column.at( internal_counter ).x >= 78 ) && ( Polygon_Intersections.at( counter ).intersections_raw_and_column.at( internal_counter ).x <= 79 ) && ( Polygon_Intersections.at( counter ).intersections_raw_and_column.at( internal_counter ).y >= 70 ) && ( Polygon_Intersections.at( counter ).intersections_raw_and_column.at( internal_counter ).y <= 71 ) ){            
                 std::cout<<   "  Polygon bla ns_raw_and_column.at( internal_counter ).x  : "  <<   Polygon_Intersections.at( counter ).intersections_raw_and_column.at( internal_counter ).x  <<std::endl;
                 std::cout<<   "  Polygon bla ns_raw_and_column.at( internal_counter ).y  : "  <<   Polygon_Intersections.at( counter ).intersections_raw_and_column.at( internal_counter ).y  <<std::endl;              
               }
            
            }
          
        }

        
      }  
      
    }


    bool IterativePolygonInTermsOfIntersections::NextIntersection( LegacyWorldCoordinate2D& the_next_intersection ){

      if( IncremtentIntersection() ){
        the_next_intersection = Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.at( current_index_internal.intersection_index );
        return true;
      }
      else{
        the_next_intersection.x =0;
        the_next_intersection.y = 0;
        return false;
      }

    }




    bool IterativePolygonInTermsOfIntersections::NextIntersectionPeriodically( LegacyWorldCoordinate2D& the_next_intersection ){

      if( IncremtentIntersectionPeriodically() ){

        the_next_intersection = Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.at( current_index_internal.intersection_index );
        return true;
      }
      else{

        the_next_intersection.x =0;
        the_next_intersection.y = 0;
        return false;
      }

    }



    bool IterativePolygonInTermsOfIntersections::NextPointOrIntersectionPeriodically( LegacyWorldCoordinate2D& previous_point_or_intersection , LegacyWorldCoordinate2D& next_point_or_intersection , LegacyDoseVoxelIndex3D& the_dose_index ){



      if( IncremtentIntersectionPeriodically() ){

        // In der naechsten Zeile wird erst mal geprueft, ob eben um einen Konturpunkt weiter gesprungen wurde.
        // Falls das der Fall ist ( current_index_internal.intersection_index == 0 ), wird geprueft, ob 
        // der Punkt, der eben uebersprungen wurde ungleich dem previous_point_or_intersection ist

// Falls es nicht direkt einen naechsten Schnittpunt gib, ohne dass ein Konturpunkt dazwischen leigt, springt 
// IncremtentIntersectionPeriodically() so lange ueber Kontur-Zwischenpunkte hinweg, bis man an einem 
// Konturpunkt angelangt ist, auf den ein Schnittpunkt folgt. Zunaechst wird in diesem Fall dann dieser Konturpunkt, 
// nicht der darauf folgende Schnittpunkt betrachtet.

        if(  ( current_index_internal.intersection_index == 0 )   /*&&  (  IsInside( Polygon_Intersections.at( current_index_internal.point_index ).contour_point_voxel_coord , the_dose_index )  )*/    &&   ( ( static_cast<float>( previous_point_or_intersection.x )  != static_cast<float>( Polygon_Intersections.at( current_index_internal.point_index ).contour_point_voxel_coord.x ) )  ||  ( static_cast<float>( previous_point_or_intersection.y )  != static_cast<float>( Polygon_Intersections.at( current_index_internal.point_index ).contour_point_voxel_coord.y ) ) )     ){

//Hier erst den Kontur-Punkt anschauen
//erst wenn der nicht geht, dann schauen, ob es einen Schnittpunkt gibt und den Pruefen. 
//Dabei darauf achten,
//dass der current_index_internal.intersection_index null sein kann, obwohl es kein nulltes Element gibt !!


          next_point_or_intersection.x =Polygon_Intersections.at( current_index_internal.point_index ).contour_point_voxel_coord.x;
          next_point_or_intersection.y = Polygon_Intersections.at( current_index_internal.point_index ).contour_point_voxel_coord.y;

          if(     ( static_cast<float>( previous_point_or_intersection.x )  != static_cast<float>( next_point_or_intersection.x )  )    ||  ( static_cast<float>( previous_point_or_intersection.y )  != static_cast<float>( next_point_or_intersection.y )     )   ) return true;     

        }
        else{
        
          if( Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.size() <= current_index_internal.intersection_index ) assert(0); // this should never happen, since IncrementIntersection should not end up with a bad index
        
          next_point_or_intersection = Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.at( current_index_internal.intersection_index );

          if(     ( static_cast<float>( previous_point_or_intersection.x )  != static_cast<float>( next_point_or_intersection.x )  )    ||  ( static_cast<float>( previous_point_or_intersection.y )  != static_cast<float>( next_point_or_intersection.y )     )  ) return true;  
         
        
        }

      }
      else{

        assert(0);
        
        return false;
        
      }

      return false;


    }





    bool IterativePolygonInTermsOfIntersections::PreviousPointOrIntersectionPeriodically(  LegacyWorldCoordinate2D& previous_point_or_intersection ,  LegacyWorldCoordinate2D& point_or_intersection , LegacyDoseVoxelIndex3D& the_dose_index ){

      if(  /*(  IsInside( Polygon_Intersections.at( current_index_internal.point_index ).contour_point_voxel_coord , the_dose_index )  ) && */ ( current_index_internal.intersection_index == 0 ) &&  ( ( static_cast<float>( previous_point_or_intersection.x )  != static_cast<float>( Polygon_Intersections.at( current_index_internal.point_index ).contour_point_voxel_coord.x ) )  ||  ( static_cast<float>( previous_point_or_intersection.y )  != static_cast<float>( Polygon_Intersections.at( current_index_internal.point_index ).contour_point_voxel_coord.y ) ) ) ){

        point_or_intersection.x =Polygon_Intersections.at( current_index_internal.point_index ).contour_point_voxel_coord.x;
        point_or_intersection.y = Polygon_Intersections.at( current_index_internal.point_index ).contour_point_voxel_coord.y;

        if(       ( static_cast<float>( previous_point_or_intersection.x )  != static_cast<float>( Polygon_Intersections.at( current_index_internal.point_index ).contour_point_voxel_coord.x ) )  ||  ( static_cast<float>( previous_point_or_intersection.y )  != static_cast<float>( Polygon_Intersections.at( current_index_internal.point_index ).contour_point_voxel_coord.y ) )      )return true;

      }
      else{
       

        if( DecrementPointOrIntersectionPeriodically() ){
        
          if( Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.size() > current_index_internal.intersection_index ){
          
            point_or_intersection = Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.at( current_index_internal.intersection_index );
            if(     ( static_cast<float>( previous_point_or_intersection.x )  != static_cast<float>( Polygon_Intersections.at( current_index_internal.point_index ).contour_point_voxel_coord.x ) )  ||  ( static_cast<float>( previous_point_or_intersection.y )  != static_cast<float>( Polygon_Intersections.at( current_index_internal.point_index ).contour_point_voxel_coord.y ) )     )return true;          
          
          }
          else{
          
            point_or_intersection.x =Polygon_Intersections.at( current_index_internal.point_index ).contour_point_voxel_coord.x;
            point_or_intersection.y = Polygon_Intersections.at( current_index_internal.point_index ).contour_point_voxel_coord.y;
            if(      ( static_cast<float>( previous_point_or_intersection.x )  != static_cast<float>( Polygon_Intersections.at( current_index_internal.point_index ).contour_point_voxel_coord.x ) )  ||  ( static_cast<float>( previous_point_or_intersection.y )  != static_cast<float>( Polygon_Intersections.at( current_index_internal.point_index ).contour_point_voxel_coord.y ) )     )return true;
          
          } 
          
        }
        else{ 
        
        
          bool decrement = true;
        
          while(decrement){
         
            decrement = DecrementPointOrIntersectionPeriodically();

            if( decrement ){
        
              if( Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.size() > current_index_internal.intersection_index ){
          
                point_or_intersection = Polygon_Intersections.at( current_index_internal.point_index ).intersections_raw_and_column.at( current_index_internal.intersection_index );
                if(     ( static_cast<float>( previous_point_or_intersection.x )  != static_cast<float>( Polygon_Intersections.at( current_index_internal.point_index ).contour_point_voxel_coord.x ) )  ||  ( static_cast<float>( previous_point_or_intersection.y )  != static_cast<float>( Polygon_Intersections.at( current_index_internal.point_index ).contour_point_voxel_coord.y ) )     )return true;          
          
              }
              else{
          
                point_or_intersection.x =Polygon_Intersections.at( current_index_internal.point_index ).contour_point_voxel_coord.x;
                point_or_intersection.y = Polygon_Intersections.at( current_index_internal.point_index ).contour_point_voxel_coord.y;
                if(      ( static_cast<float>( previous_point_or_intersection.x )  != static_cast<float>( Polygon_Intersections.at( current_index_internal.point_index ).contour_point_voxel_coord.x ) )  ||  ( static_cast<float>( previous_point_or_intersection.y )  != static_cast<float>( Polygon_Intersections.at( current_index_internal.point_index ).contour_point_voxel_coord.y ) )     )return true;
          
              } 
          
            } 
          
          
          }
        
          assert(0); // this must not happen
          
          
        }
        return false;


      }

      return false;

    }


    int IterativePolygonInTermsOfIntersections::MaximumNumberOfElements(){

      if( number_of_intersections == (-1) ){   

        int resulting_number = 0;

        for( GridIndexType counter = 0 ; counter <  Polygon_Intersections.size() ; counter++ ){

          resulting_number += Polygon_Intersections.at( counter ).intersections_raw_and_column.size();

        }

        number_of_intersections = resulting_number;

        return number_of_intersections; 

      }
      else return number_of_intersections; 

    }


    bool IterativePolygonInTermsOfIntersections::ThereAreIntersections(){
      if( number_of_intersections == (-1) ) number_of_intersections = MaximumNumberOfElements();
      if( number_of_intersections > 0 )return true;
      else return false;
    } 


    void IterativePolygonInTermsOfIntersections::CreateUnifiedListOfRawAndColumnIntersections(){

      // iteration along polygon

      for( unsigned int point_index = 0 ; point_index < ( Polygon_Intersections.size() ); point_index++ ){ 

        if( Polygon_Intersections.at( point_index ).intersections_raw_and_column.size() == 0 ){

          bool do_continue = true;

          Polygon_Intersections.at( point_index ).intersections_raw_and_column.clear();

          unsigned int raw_index = 0;
          unsigned int column_index = 0;

          LegacyWorldCoordinate3D coordinates_return = Polygon_Intersections.at( point_index ).contour_point_voxel_coord;  

          std::vector<LegacyWorldCoordinate2D> raw_intersections_work = Polygon_Intersections.at( point_index ).raw_intersections;
          std::vector<LegacyWorldCoordinate2D>& raw_intersections_work_ref = raw_intersections_work;
          std::vector<LegacyWorldCoordinate2D> column_intersections_work = Polygon_Intersections.at( point_index ).column_intersections;
          std::vector<LegacyWorldCoordinate2D>& column_intersections_work_ref = column_intersections_work;

          double last_distance = -1;

          while( do_continue ){                            

            LegacyWorldCoordinate2D coordinates_raw_intersection;
            coordinates_raw_intersection.x =0;
            coordinates_raw_intersection.y = 0;

            LegacyWorldCoordinate2D coordinates_column_intersection;
            coordinates_column_intersection.x =0;
            coordinates_column_intersection.y = 0;

            LegacyWorldCoordinate2D coordinates_final_intersection;
            coordinates_final_intersection.x =0;
            coordinates_final_intersection.y = 0;      

            double distance_raw = 1000000000;
            double distance_column = 1000000000;

            if( raw_intersections_work.size() > 0 ){

              coordinates_raw_intersection = raw_intersections_work.at( 0 );
              distance_raw = (  coordinates_raw_intersection.x - coordinates_return.x ) *  (  coordinates_raw_intersection.x - coordinates_return.x ) + (  coordinates_raw_intersection.y - coordinates_return.y ) *  (  coordinates_raw_intersection.y - coordinates_return.y ); 
              distance_raw = sqrt( distance_raw );
              if( distance_raw < last_distance )assert(0); // this must never happen

            }

            if( column_intersections_work.size() > 0 ){

              coordinates_column_intersection = column_intersections_work.at( 0 );  
              distance_column = (  coordinates_column_intersection.x - coordinates_return.x ) *  (  coordinates_column_intersection.x - coordinates_return.x ) + (  coordinates_column_intersection.y - coordinates_return.y ) *  (  coordinates_column_intersection.y - coordinates_return.y ); 
              distance_column = sqrt( distance_column );
              if( distance_column < last_distance )assert(0); // this must never happen

            }

            if(  ( distance_raw != 1000000000  ) && ( distance_column != 1000000000  ) ){

              if( distance_raw < distance_column ){
                coordinates_final_intersection = raw_intersections_work.at( raw_index );
                Polygon_Intersections.at( point_index ).intersections_raw_and_column.push_back( coordinates_raw_intersection );           
                last_distance = distance_raw;
              }
              else if( distance_raw > distance_column ){
                coordinates_final_intersection = column_intersections_work.at( column_index );
                Polygon_Intersections.at( point_index ).intersections_raw_and_column.push_back( coordinates_column_intersection );         
                last_distance = distance_column;
              }
              else if( distance_raw == distance_column ){
                coordinates_final_intersection = column_intersections_work.at( column_index );
                Polygon_Intersections.at( point_index ).intersections_raw_and_column.push_back( coordinates_column_intersection );           
                // man muss diesen Punkt nicht zweimal beachten.         
                // deshalb nicht : Polygon_Intersections.at( point_index ).intersections_raw_and_column.push_back( coordinates_raw_intersection );            
                last_distance = distance_column;
              }

            }
            else if(   distance_raw != 1000000000   ){
              coordinates_final_intersection = raw_intersections_work.at( raw_index );
              Polygon_Intersections.at( point_index ).intersections_raw_and_column.push_back( coordinates_raw_intersection );  
              last_distance = distance_raw;
            }
            else if(   distance_column !=  1000000000   ){
              coordinates_final_intersection = column_intersections_work.at( column_index );
              Polygon_Intersections.at( point_index ).intersections_raw_and_column.push_back( coordinates_column_intersection ); 
              last_distance = distance_column;
            }
            else{

              do_continue = false;

            }

            /*   std::cout <<  "   Here are the values : "  << std::endl;

            std::cout <<  "  coordinates_return.x : "  <<  coordinates_return.x   << std::endl;
            std::cout <<  "  coordinates_return.y : "  <<  coordinates_return.y   << std::endl;



            std::cout <<  "  coordinates_final_intersection.x : "  << coordinates_final_intersection.x << std::endl;
            std::cout <<  "  coordinates_final_intersection.y : "  << coordinates_final_intersection.y << std::endl;      

            for( int counter = 0 ; counter < Polygon_Intersections.at( point_index ).intersections_raw_and_column.size() ; counter++  ){

            std::cout <<  " Polygon_Intersections.at( point_index ).intersections_raw_and_column.at( counter ).x  : "  <<  Polygon_Intersections.at( point_index ).intersections_raw_and_column.at( counter ).x   << std::endl;     
            std::cout <<  " Polygon_Intersections.at( point_index ).intersections_raw_and_column.at( counter ).y  : "  <<  Polygon_Intersections.at( point_index ).intersections_raw_and_column.at( counter ).y   << std::endl; 

            }

            for( int counter = 0 ; counter < raw_intersections_work.size() ; counter++  ){

            std::cout <<  " raw_intersections_work.at( counter ).x  : "  <<  raw_intersections_work.at( counter ).x   << std::endl;     
            std::cout <<  " raw_intersections_work.at( counter ).y  : "  <<  raw_intersections_work.at( counter ).y   << std::endl; 

            }


            for( int counter = 0 ; counter < column_intersections_work.size() ; counter++  ){

            std::cout <<  " column_intersections_work.at( counter ).x  : "  <<  column_intersections_work.at( counter ).x   << std::endl;     
            std::cout <<  " column_intersections_work.at( counter ).y  : "  <<  column_intersections_work.at( counter ).y   << std::endl;  

            }  */


            if(  ( distance_raw != 1000000000  ) || ( distance_column != 1000000000  ) )  removeThisCoordinateFromBothLists( coordinates_final_intersection , raw_intersections_work_ref , column_intersections_work_ref );

          }

        }

      }

    }



    void IterativePolygonInTermsOfIntersections::removeThisCoordinateFromBothLists( LegacyWorldCoordinate2D the_coordinate , std::vector<LegacyWorldCoordinate2D>& raw_intersections_work_ref , std::vector<LegacyWorldCoordinate2D>& column_intersections_work_ref ){

      LegacyWorldCoordinate2D coordinates_this_intersection;
      coordinates_this_intersection.x =0;
      coordinates_this_intersection.y = 0;      

      std::vector<LegacyWorldCoordinate2D>::iterator iter;


      for( GridIndexType counter = 0 ; counter < raw_intersections_work_ref.size() ; counter++ ){
        coordinates_this_intersection = raw_intersections_work_ref.at( counter );
        if( ( coordinates_this_intersection.x == the_coordinate.x ) && ( coordinates_this_intersection.y == the_coordinate.y ) ){ 
          iter = raw_intersections_work_ref.begin();
          raw_intersections_work_ref.erase( iter );   
        } 
        else break;          
      }  

      for( GridIndexType counter = 0 ; counter < column_intersections_work_ref.size() ; counter++ ){
        coordinates_this_intersection = column_intersections_work_ref.at( counter );
        if( ( coordinates_this_intersection.x == the_coordinate.x ) && ( coordinates_this_intersection.y == the_coordinate.y ) ){  
          iter = column_intersections_work_ref.begin();
          column_intersections_work_ref.erase( iter );  
        } 
        else break;          
      }

    } 



    bool IterativePolygonInTermsOfIntersections::ItIsAsExpected( masks::legacy::PolygonInTermsOfIntersections Expected_Unified_Polygon_Intersections  ){


      if( Expected_Unified_Polygon_Intersections.size() != Polygon_Intersections.size() ) return false;

      for( GridIndexType a_counter = 0 ; a_counter < Expected_Unified_Polygon_Intersections.size() ; a_counter++ ){

        if( ! IdenticalUniList( Expected_Unified_Polygon_Intersections.at( a_counter )  ,  Polygon_Intersections.at( a_counter )  )  ) return false;

      }                               

      return true;

    } 



    bool IterativePolygonInTermsOfIntersections::IdenticalUniList( masks::legacy::PointLevelVector plv_a , masks::legacy::PointLevelVector plv_b ){

      if( plv_a.intersections_raw_and_column.size() != plv_b.intersections_raw_and_column.size() ) return false;

      LegacyWorldCoordinate2D wco2da;
      wco2da.x =0;
      wco2da.y = 0; 

      LegacyWorldCoordinate2D wco2db;
      wco2db.x =0;
      wco2db.y = 0;

      for( GridIndexType this_counter = 0 ; this_counter < plv_a.intersections_raw_and_column.size() ; this_counter++  ){
        wco2da = plv_a.intersections_raw_and_column.at( this_counter );
        wco2db = plv_b.intersections_raw_and_column.at( this_counter );
        if(  (  static_cast<float>( wco2da.x ) != static_cast<float>( wco2db.x )  ) || (  static_cast<float>( wco2da.y ) != static_cast<float>( wco2db.y )  )   ) return false;
      }

      return true;

    } 


    void IterativePolygonInTermsOfIntersections::RememberPosition(){

      IterativePolygonInTermsOfIntersections::WhichIntersection& intersection_index_to_ref = current_index_internal_remember;
      IterativePolygonInTermsOfIntersections::WhichIntersection& intersection_index_from_ref = current_index_internal;

      CopyIntersectionIndex( intersection_index_to_ref , intersection_index_from_ref );   

    }


    void IterativePolygonInTermsOfIntersections::JumpToRememberedPosition(){

      IterativePolygonInTermsOfIntersections::WhichIntersection& intersection_index_from_ref = current_index_internal_remember;
      IterativePolygonInTermsOfIntersections::WhichIntersection& intersection_index_to_ref = current_index_internal;

      CopyIntersectionIndex( intersection_index_to_ref , intersection_index_from_ref );   

    }


    void IterativePolygonInTermsOfIntersections::SetCurrentIntersectionIndex( IterativePolygonInTermsOfIntersections::WhichIntersection& intersection_index_new_ref ){

      IterativePolygonInTermsOfIntersections::WhichIntersection& intersection_index_to_ref = current_index_internal;

      CopyIntersectionIndex( intersection_index_to_ref , intersection_index_new_ref );   

    }
    
    
    bool IterativePolygonInTermsOfIntersections::CheckCurrentIntersectionIndexIdentical( IterativePolygonInTermsOfIntersections::WhichIntersection& intersection_index_compare_ref ){
   
      if( ( current_index_internal.point_index == intersection_index_compare_ref.point_index ) &&   ( current_index_internal.intersection_index == intersection_index_compare_ref.intersection_index )   ){
        return true;
      }
      else{
       return false;
      }
   
    }    


    void IterativePolygonInTermsOfIntersections::MarkRelevantItems( LegacyDoseVoxelIndex3D aDoseIndex ){

      WhichIntersection the_intersection_index;  // the_intersection_index initialized to zero by constructor

      for( the_intersection_index.point_index = 0 ; the_intersection_index.point_index < ( Polygon_Intersections.size() ) ; the_intersection_index.point_index++ ){
        Polygon_Intersections.at( the_intersection_index.point_index ).to_be_Regarded = false;
      }

      for( the_intersection_index.point_index = 0 ; the_intersection_index.point_index < ( Polygon_Intersections.size() - 1 ) ; the_intersection_index.point_index++ ){

        LegacyWorldCoordinate3D wc3da = Polygon_Intersections.at( the_intersection_index.point_index ).contour_point_voxel_coord;
        LegacyWorldCoordinate3D wc3db = Polygon_Intersections.at( the_intersection_index.point_index + 1 ).contour_point_voxel_coord;

        if(  VoxelInBetween( aDoseIndex , wc3da , wc3db )   ){
          Polygon_Intersections.at( the_intersection_index.point_index ).to_be_Regarded = true;
          Polygon_Intersections.at( the_intersection_index.point_index + 1 ).to_be_Regarded = true;
        }  

      }

      if(  Polygon_Intersections.size() > 0 ){

        LegacyWorldCoordinate3D wc3da = Polygon_Intersections.at(  ( Polygon_Intersections.size() - 1 ) ).contour_point_voxel_coord;
        LegacyWorldCoordinate3D wc3db = Polygon_Intersections.at( 0 ).contour_point_voxel_coord;        

        if(  VoxelInBetween( aDoseIndex , wc3da , wc3db )   ){
          Polygon_Intersections.at( ( Polygon_Intersections.size() - 1 ) ).to_be_Regarded = true;
          Polygon_Intersections.at( 0 ).to_be_Regarded = true;
        }  

      }

    }




    bool IterativePolygonInTermsOfIntersections::VoxelInBetween( LegacyDoseVoxelIndex3D aDoseIndex , LegacyWorldCoordinate3D wc3da, LegacyWorldCoordinate3D wc3db ){

      LegacyWorldCoordinate3D wc3dc_min;
      LegacyWorldCoordinate3D wc3dc_max;

      if( wc3da.x <= wc3db.x ){  
        wc3dc_min.x =wc3da.x;
        wc3dc_max.x =wc3db.x;
      }  
      else{
        wc3dc_min.x =wc3db.x;
        wc3dc_max.x =wc3da.x;    
      }

      if(   static_cast<float>(wc3dc_min.x) > static_cast<float>( aDoseIndex.x + 1.1 )   ) return false;
      if(   static_cast<float>(wc3dc_max.x) < static_cast<float>( aDoseIndex.x - 0.1 ) ) return false;

      if( wc3da.y <= wc3db.y ){  
        wc3dc_min.y = wc3da.y;
        wc3dc_max.y = wc3db.y;
      }  
      else{
        wc3dc_min.y = wc3db.y;
        wc3dc_max.y = wc3da.y;    
      }

      if(   static_cast<float>(wc3dc_min.y) > static_cast<float>( aDoseIndex.y + 1.1 )   ) return false;
      if(   static_cast<float>(wc3dc_max.y) < static_cast<float>( aDoseIndex.y - 0.1 ) ) return false;

      if( wc3da.z <= wc3db.z ){  
        wc3dc_min.z = wc3da.z;
        wc3dc_max.z = wc3db.z;
      }  
      else{
        wc3dc_min.z = wc3db.z;
        wc3dc_max.z = wc3da.z;    
      }    


      if(   static_cast<float>(wc3dc_min.z) > static_cast<float>( aDoseIndex.z + 1.1 )   ) return false; // it can not be in between
      if(   static_cast<float>(wc3dc_max.z) < static_cast<float>( aDoseIndex.z - 0.1 ) ) return false;

      return true; // can't be sure 

    }


}//namespace
  }//namespace
}//namespace



