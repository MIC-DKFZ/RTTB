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

#include "rttbMask.h"

#include "rttbNullPointerException.h"
#include "rttbSelfIntersectingStructureException.h"

#include <map>
#include <utility>
#include <assert.h>
#include <algorithm>
#include <list>

using namespace std;




namespace rttb{
  namespace masks{
	  namespace legacy{


    Mask::Mask( DoseIteratorInterface* aDoseIter  , StructureLegacy* aStructure ) { 
      
      MaskField.resize(1);
      for(int i = 0 ; i < MaskField.size() ; i++ )MaskField.at(i) = NULL;
    
      FieldOfIntersections = NULL;
      do_detailed_subvox = false;
      
      this->_doseIter=aDoseIter;
      this->_structure=aStructure;
      //this->_withSubVoxelFraction=withSubVoxelFraction;

      if(!_doseIter)
        throw core::NullPointerException("aDoseIter must not be NULL!");
      if(!_structure)
        throw core::NullPointerException("aStructure must not be NULL!");

      this->_doseIter->start();
      this->GInf.push_back( _doseIter->getGeometricInfo() );

      //std::cout << "init begin"<<std::endl;
      Init();           

      try{

        if( ContourFine() ) Calc();
        else throw rttb::masks::SelfIntersectingStructureException( " This contour is not okay. It interscts itself. That doesn't make sense in radiotherapy.  " );
      
      }
      catch( exception& e ){
         cout<< e.what() <<endl;
         bool this_contour_is_not_okay_terminating_the_program = 0;
         assert(this_contour_is_not_okay_terminating_the_program); // user is supposed to get things right and to think about structures before continuation
      }


      //for( int i = 0 ; i < MaskField.size() ; i++ )MaskField.at(i)->~FieldOfScalar();
      //MaskField.resize(0);

      sort(_doseVoxelInStructure.begin(),_doseVoxelInStructure.end());
      this->calcDoseData();

      this->clearMaskField();

    }

    Mask::Mask( core::GeometricInfo* aDoseGeoInfo  , StructureLegacy* aStructure ) { 

      if(!aDoseGeoInfo){
        throw core::NullPointerException("aDoseGeoInfo must not be NULL!");
      }

      MaskField.resize(1);
      for(int i = 0 ; i < MaskField.size() ; i++ )
      {
        MaskField.at(i) = NULL;
      }

      FieldOfIntersections = NULL;
      do_detailed_subvox = false;

      if(!aStructure){
        throw core::NullPointerException("aStructure must not be NULL!");
      }
      this->_structure=aStructure;
      //this->_withSubVoxelFraction=withSubVoxelFraction;

      this->GInf.push_back( *aDoseGeoInfo );

      //std::cout << "init begin"<<std::endl;
      Init();           

      try{
        if( ContourFine() ) 
          Calc();
        else 
          throw rttb::masks::SelfIntersectingStructureException( " This contour is not okay. It intersects itself. That doesn't make sense in radiotherapy.  " );
      }
      catch( exception& e ){
        cout<< e.what() <<endl;
        bool this_contour_is_not_okay_terminating_the_program = 0;
        assert(this_contour_is_not_okay_terminating_the_program); 
      }


      //for( int i = 0 ; i < MaskField.size() ; i++ )MaskField.at(i)->~FieldOfScalar();
      //MaskField.resize(0);

      sort(_doseVoxelInStructure.begin(),_doseVoxelInStructure.end());

      this->clearMaskField();

    }
/*          
	Mask::Mask(rttb::core::DoseIteratorInterface *aDoseIter, const std::vector<legacy::DoseVoxel>& aDoseVoxelVector){
      _structure=NULL;
      FieldOfIntersections = NULL;
      do_detailed_subvox = false;
    
      MaskField.resize(1);
      for(int i = 0 ; i < MaskField.size() ; i++ )MaskField.at(i) = NULL;
    
      _doseIter=aDoseIter;
      if(!_doseIter)
        throw core::NullPointerException("aDoseIter must not be NULL!");
      this->_doseIter->start();
      this->GInf.push_back( _doseIter->getGeometricInfo() );
      //Get valid DoseVoxel
      for(int i=0;i<aDoseVoxelVector.size();i++){
        DoseVoxel voxel=aDoseVoxelVector.at(i);
        if(voxel.getVoxelIndex3D().x<GInf.at(0)getNumColumns() && voxel.getVoxelIndex3D().y<GInf.at(0)getNumRows() && voxel.getVoxelIndex3D().z<GInf.at(0).getNumSlices())
          _doseVoxelInStructure.push_back(voxel);
      }
      
      sort(_doseVoxelInStructure.begin(),_doseVoxelInStructure.end());
      this->calcDoseData();

      this->clearMaskField();

    }

*/

    Mask::~Mask(){

      /*if( FieldOfIntersections != NULL )FieldOfIntersections->~FieldOfPointer();
      
      for( int i = 0 ; i < MaskField.size() ; i++ ){
        if(MaskField.at(i)!=NULL)MaskField.at(i)->~FieldOfScalar();
      }
      MaskField.resize(1);
      for(int i = 0 ; i < MaskField.size() ; i++ )MaskField.at(i) = NULL;*/

    }

    void Mask::clearMaskField(){
      if( FieldOfIntersections != NULL )FieldOfIntersections->~FieldOfPointer();

      for( int i = 0 ; i < MaskField.size() ; i++ ){
        if(MaskField.at(i)!=NULL)MaskField.at(i)->~FieldOfScalar();
      }
      MaskField.resize(1);
      for(int i = 0 ; i < MaskField.size() ; i++ )MaskField.at(i) = NULL;
    }

    void Mask::resetDose(DoseIteratorInterface* aDoseIterator){
      _doseIter=aDoseIterator;
      if(!_doseIter)
        throw core::NullPointerException("aDoseIter must not be NULL!");
      this->_doseIter->start();

      //if geometric info not changed or if the voxelization external
      if(_doseIter->getGeometricInfo()==this->GInf.at(0) || this->_structure==NULL){
        //std::cout << "reset dose."<<std::endl;
        this->calcDoseData();
      }
      else{
        //std::cout << "recalculate mask"<<std::endl;
        this->GInf.push_back( _doseIter->getGeometricInfo() );

        Init();
          
        Calc();
        sort(_doseVoxelInStructure.begin(),_doseVoxelInStructure.end());
        this->calcDoseData();

        this->clearMaskField();

      }
    }


    /*const std::vector<LegacyDoseVoxelIndex3D>& Mask::getDoseVoxelIndexInStructure() const{
      return doseVoxelIndexInStructure;
    }*/

    /*! @brief Get the dose voxels which lie inside the structure
    *  @return Return the vector of DoseVoxel which are inside the structure (with voxel proportion information)
    */
    const std::vector<DoseVoxel>& Mask::getDoseVoxelInStructure() const{
      return this->_doseVoxelInStructure;
    }

    /*! @brief Set the dose voxels which lie inside the structure
    *  @return Set the vector of DoseVoxel which are inside the structure (with voxel proportion information)
    */
    void Mask::setDoseVoxelInStructure(std::vector<DoseVoxel> aDoseVoxelVector){
      _doseVoxelInStructure=aDoseVoxelVector;

    }

    const DoseIteratorInterface& Mask::getDoseIterator() const{
      return *_doseIter;
    }

    const StructureLegacy& Mask::getRTStructure() const{
      return *_structure;
    }

    /*const std::vector<DoseTypeGy> Mask::getDoseData() const{
      return _doseData;
    }*/



    //get absolute dose data of the voxels which are inside the structure;
    void Mask::calcDoseData(){
      _doseData.clear();
      /*this->_doseIter->start();
      std::vector<DoseTypeGy> doseDis;//dose distribution
      while(_doseIter->hasNextVoxel()){
        doseDis.push_back(_doseIter->next());
      }*/

      _doseIter->start();
      int count=0;
      
      std::vector<DoseVoxel>::iterator it=_doseVoxelInStructure.begin();
      while(_doseIter->hasNextVoxel() && it!=_doseVoxelInStructure.end()){
        // Anmerkung von Martina fuer Lanlan: Eigentlich ist der Witz beim Iterator gerade, dass man sich ueber die 
        // Innereien des Iterators als Programmierer, der ihn einsetzt keine Gedanken machen muss. Hier ist die fehlerfreie Funktion
        // des Codes jedoch von einigen Eigenschaften des Iterators, insbesondere von der Richtung in der iteriert wird,
        // abhaengig. Das kommt daher, dass der Programmierer den Iterator von innen kannte und Eigenschaften nutzt, die er eigentlich 
        // nicht kennem wuerde, als Anwendungsprogrammierer des Iterators. Weil der Programmierer den Iterator aber selbst geschrieben hatte, 
        // deshalb wuste er es hier ...
        // Das kann zur Folgen haben, dass nach einer Aenderung in Code des Iterators Mask nicht mehr korrekt 
        // funktioniert (z.B. wenn die Richtung der iteration im Koordinatensystem sich umkehren wuerde) man kann darueber
        // nachdenken ob das problematisch ist ....
        // Es ist ja auch so, dass die RTToolbox dafuer gemacht ist erweitert zu werden. Es koennen andere Iteratoren dazu kommen.
        // Wollen wir hier spezifischer werden was den Typ des erlaubten Iterators betrifft? Das wuerde helfen.
        double doseValue=_doseIter->next();
        LegacyDoseVoxelIndex3D doseVoxel=(*it).getVoxelIndex3D();
        if(count==doseVoxel.z*GInf.at(0).getNumColumns()*GInf.at(0).getNumRows()+doseVoxel.y*GInf.at(0).getNumColumns()+doseVoxel.x){
          _doseData.push_back(doseValue);
          it++;
        }
        count++;
      }

    }


    void Mask::Calc(){
      //std::cout << "calc begin"<<std::endl;
    
      SetContentUnknown();
    
      for( int resolution_level = ( GInf.size() - 1 ) ; resolution_level >= 0  ; resolution_level-- ){

        //std::cout << "GetIntersections"<<std::endl;
        GetIntersections(resolution_level);

        //std::cout << "MarkVoxelsTouchedByStructure"<<std::endl;
        MarkVoxelsTouchedByStructure(resolution_level);

        //SeperateRegionsQuick(); //or:
        //setInsideVoxel(resolution_level); //author: Lanlan
        //std::cout << "setInsideVoxelMultiRes"<<std::endl;
        setInsideVoxelMultiRes(resolution_level);

      }
      
      //TestVoxelMaskUppward();

      //std::cout << "SetDoseVoxelInStructureVector"<<std::endl;
      SetDoseVoxelInStructureVector();     

    } 
    
    
    void Mask::TestVoxelMaskUppward(){
    
     for( int resolution_level = ( GInf.size() - 1 ) ; resolution_level >= 0  ; resolution_level--  ){
    
      for( int index_z = 0 ; index_z < GInf.at(resolution_level).getNumSlices() ; index_z++ ){

        LegacyUnsignedIndex3D index_internal;                    
        index_internal.x = 0;
        index_internal.y = 0;
        index_internal.z = index_z;   

        for( int resolution_level = ( GInf.size() - 1 ) ; resolution_level >= 0  ; resolution_level-- ){
        
          if( ( MarkInterestingArea.at(resolution_level).at( index_z ).index_end.x != 0 ) && ( MarkInterestingArea.at(resolution_level).at( index_z ).index_end.y != 0 ) ){

            for(  index_internal.x = MarkInterestingArea.at(resolution_level).at( index_z ).index_begin.x ; index_internal.x <= MarkInterestingArea.at(resolution_level).at( index_z ).index_end.x  ; index_internal.x++ ){
              for( index_internal.y = MarkInterestingArea.at(resolution_level).at( index_z ).index_begin.y ; index_internal.y <= MarkInterestingArea.at(resolution_level).at( index_z ).index_end.y  ; index_internal.y++ ){
               

                if( MaskField.at( resolution_level )->GetData( index_internal.x, index_internal.y, index_internal.z ) == brightness_inside ){
                
                  CheckUppwardBlockSameIntensity( resolution_level , index_internal, brightness_inside );
     
                }      

                if( MaskField.at( resolution_level )->GetData( index_internal.x, index_internal.y, index_internal.z ) == brightness_outside ){

                  CheckUppwardBlockSameIntensity( resolution_level , index_internal, brightness_outside );

                }       
                
                if( MaskField.at( resolution_level )->GetData( index_internal.x, index_internal.y, index_internal.z ) == brightness_border ){
                
                  CheckUppwardOneOutOfThisBlockHasSameIntensity( resolution_level , index_internal , brightness_border );
                
                }      

                if( MaskField.at( resolution_level )->GetData( index_internal.x, index_internal.y, index_internal.z ) == brightness_unknown )assert(0); // This should not happen since each voxel must be regarded.





              } // for( index_internal.y

            } // for(  index_internal.x


          } // if( ( MarkInterestingArea.at(resolution_level).at( index_z

        } // for( int resolution_level
      
      }
    
     } // resolution_level
    
    }
    

    void Mask::CheckUppwardBlockSameIntensity( int resolution_level_in , LegacyUnsignedIndex3D index_in , int brightness ){

      if( resolution_level_in > 0 ){

        for( int resolution_level = ( resolution_level_in - 1 ) ; resolution_level >= 0  ; resolution_level-- ){
        
             int resolution_difference = ( resolution_level_in -  resolution_level );
        
             double block_length = GetBlockLengthThisResolutionLevel( resolution_difference );
        
             LegacyUnsignedIndex3D index_internal; 
             index_internal.x = index_in.x * block_length; 
             index_internal.y = index_in.y * block_length; 
             index_internal.z = index_in.z; 
        
             if( ( index_internal.x >= MarkInterestingArea.at(resolution_level).at( index_internal.z ).index_begin.x ) && ( index_internal.x <= ( MarkInterestingArea.at(resolution_level).at( index_internal.z ).index_end.x - block_length )  ) &&  ( index_internal.y >= MarkInterestingArea.at(resolution_level).at( index_internal.z ).index_begin.y ) && ( index_internal.y <= ( MarkInterestingArea.at(resolution_level).at( index_internal.z ).index_end.y - block_length )  )  ){
             if( ( index_in.x >= MarkInterestingArea.at(resolution_level_in).at( index_in.z ).index_begin.x ) && ( index_in.x <= ( MarkInterestingArea.at(resolution_level_in).at( index_in.z ).index_end.x - block_length )  ) &&  ( index_in.y >= MarkInterestingArea.at(resolution_level_in).at( index_in.z ).index_begin.y ) && ( index_in.y <= ( MarkInterestingArea.at(resolution_level_in).at( index_in.z ).index_end.y - block_length )  )  ){ 
              
               bool its_fine = MaskField.at( resolution_level )->CheckBlockSameIntensity( index_internal.x , index_internal.y, index_internal.z , block_length , brightness ); 


               assert( its_fine );
        
             }
             }
        
        }
        
      }  

    }


    void Mask::SetUppwardBlockThisIntensity( int resolution_level_in , LegacyUnsignedIndex3D index_in , int brightness ){

      if( resolution_level_in > 0 ){

        for( int resolution_level = ( resolution_level_in - 1 ) ; resolution_level >= 0  ; resolution_level-- ){
        
             int resolution_difference = ( resolution_level_in -  resolution_level );
        
             double block_length = GetBlockLengthThisResolutionLevel( resolution_difference );
        
             LegacyUnsignedIndex3D index_internal; 
             index_internal.x = index_in.x * block_length; 
             index_internal.y = index_in.y * block_length; 
             index_internal.z = index_in.z; 
        
  
        
             if( ( index_internal.x >= MarkInterestingArea.at(resolution_level).at( index_internal.z ).index_begin.x ) && ( index_internal.x <= ( MarkInterestingArea.at(resolution_level).at( index_internal.z ).index_end.x - block_length )  ) &&  ( index_internal.y >= MarkInterestingArea.at(resolution_level).at( index_internal.z ).index_begin.y ) && ( index_internal.y <= ( MarkInterestingArea.at(resolution_level).at( index_internal.z ).index_end.y - block_length )  )  ){
               if( ( index_in.x >= MarkInterestingArea.at(resolution_level_in).at( index_in.z ).index_begin.x ) && ( index_in.x <= ( MarkInterestingArea.at(resolution_level_in).at( index_in.z ).index_end.x - block_length )  ) &&  ( index_in.y >= MarkInterestingArea.at(resolution_level_in).at( index_in.z ).index_begin.y ) && ( index_in.y <= ( MarkInterestingArea.at(resolution_level_in).at( index_in.z ).index_end.y - block_length )  )  ){        

                 MaskField.at( resolution_level )->SetBlockThisIntensity( index_internal.x , index_internal.y, index_internal.z , block_length , brightness ); 

               }
             }
        
        }
        
      }  

    }


    void Mask::CheckUppwardOneOutOfThisBlockHasSameIntensity( int resolution_level_in , LegacyUnsignedIndex3D index_in , int brightness ){

      if( ( resolution_level_in > 0 ) && ( resolution_level_in < (MaskField.size()-1) ) ){

        for( int resolution_level = ( resolution_level_in - 1 ) ; resolution_level >= 0  ; resolution_level-- ){
        
             int resolution_difference = ( resolution_level_in -  resolution_level );
        
             double block_length = GetBlockLengthThisResolutionLevel( resolution_difference );

             LegacyUnsignedIndex3D index_internal; 
             index_internal.x = index_in.x * block_length; 
             index_internal.y = index_in.y * block_length; 
             index_internal.z = index_in.z; 

             if( ( index_internal.x >= MarkInterestingArea.at(resolution_level).at( index_internal.z ).index_begin.x ) && ( index_internal.x <= ( MarkInterestingArea.at(resolution_level).at( index_internal.z ).index_end.x - block_length )  ) &&  ( index_internal.y >= MarkInterestingArea.at(resolution_level).at( index_internal.z ).index_begin.y ) && ( index_internal.y <= ( MarkInterestingArea.at(resolution_level).at( index_internal.z ).index_end.y - block_length )  )  ){
             if( ( index_in.x >= MarkInterestingArea.at(resolution_level_in).at( index_in.z ).index_begin.x ) && ( index_in.x <= ( MarkInterestingArea.at(resolution_level_in).at( index_in.z ).index_end.x - block_length )  ) &&  ( index_in.y >= MarkInterestingArea.at(resolution_level_in).at( index_in.z ).index_begin.y ) && ( index_in.y <= ( MarkInterestingArea.at(resolution_level_in).at( index_in.z ).index_end.y - block_length )  )  ){ 
        
             bool its_fine = MaskField.at( resolution_level )->CheckOneOutOfThisBlockHasSameIntensity( index_internal.x , index_internal.y, index_internal.z , block_length , brightness ); 
        
             assert( its_fine );

             }
             
             }
        
        }
        
      } 
  
    }


    bool Mask::ContourFine(){
    
      //std::cout << "ContourFine begin"<<std::endl;
      LegacyPolygonSequenceType strVector = _structure->getLegacyStructureVector();
      LegacyPolygonSequenceType& strVector_ref = strVector;  
    
      correspoinding_structure_vector str_v;      
      std::vector<int> wich_slice;
                              
      for(unsigned int struct_index = 0 ; struct_index < Intersections.at(0).size() ; struct_index++ ){          

          wich_slice.clear(); 
    
          /*   if( strVector.at(  struct_index ).size() < 2 ){
            std::cerr<<  " The Polygon consists of just one point ! That doesn't make sense in Radiotherapy. Program will be terminated. "  <<std::endl; 
            return false; 
          }  */
          
          /*for( int slice = 0 ; slice < SliceStructureCorrespondency.size() ; slice++ ){
          
            for( int structure_number_this_slice = 0; structure_number_this_slice < SliceStructureCorrespondency.at( slice ).size() ; structure_number_this_slice++ ){
            
              if(  SliceStructureCorrespondency.at( slice ).at( structure_number_this_slice ) == struct_index ){
          
                for(  int structure_number_this_slice_internal = 0; structure_number_this_slice_internal < SliceStructureCorrespondency.at( slice ).size() ; structure_number_this_slice_internal++  ){
                  if( SliceStructureCorrespondency.at( slice ).at( structure_number_this_slice_internal ) != struct_index )wich_slice.push_back(  SliceStructureCorrespondency.at( slice ).at( structure_number_this_slice_internal ) );
                }
              
              }
            
            } 
          
          } 
      
          for(unsigned int this_counter = 0 ; this_counter < wich_slice.size() ; this_counter++ ){ 
     
            if( struct_index == wich_slice.at( this_counter ) )assert(0); // this must not happen
            if( DoesIntersect( wich_slice.at( this_counter ) , struct_index ) )return false;
     
          }*/
          
          //if( DoesIntersect( struct_index , struct_index ) )return false;
          
          for( int struct_index_a = 0 ; struct_index_a < Intersections.at(0).size() ; struct_index_a++  ){
          
            for(   int struct_index_b = 0 ; struct_index_b < Intersections.at(0).size() ; struct_index_b++    ){
              //std::cout << "("<<struct_index_a<<","<<struct_index_b<<")"<<std::endl;
            
              if(   DoesIntersect( struct_index_a , struct_index_b )   ) {
                //std::cout << "DoesIntersect ja"<<std::endl;
                return false;
              }
            
            
            }
          
          }

      } // for(unsigned int struct_index = 0 ; struct_index < Intersections.at().size() ; struct_index++ )
      //std::cout << "ContourFine end"<<std::endl;
    
      return true;
    
    }

    

    bool Mask::DoesIntersect( int struct_index_a , int struct_index_b ) const
	{
      LegacyWorldCoordinate3D firstPoint (0,0,0);  
               
      LegacyWorldCoordinate3D secondPoint (0,0,0); 

      LegacyWorldCoordinate3D thirdPoint (0,0,0);    
               
      LegacyWorldCoordinate3D fourthPoint (0,0,0); 
         
      const PolygonInTermsOfIntersections& pitoiA = Intersections.at(0).at( struct_index_a );
      const PolygonInTermsOfIntersections& pitoiB = Intersections.at(0).at( struct_index_b );

      PolygonInTermsOfIntersections::const_iterator itA=pitoiA.begin();
      PolygonInTermsOfIntersections::const_iterator itA2=pitoiA.begin()+1;
      if(itA2==pitoiA.end())itA2=pitoiA.begin();
      PolygonInTermsOfIntersections::const_iterator itB=pitoiB.begin();
      PolygonInTermsOfIntersections::const_iterator itB2=pitoiB.begin()+1;
      if(itB2==pitoiB.end())itB2=pitoiB.begin();

      int i=0;
      int j=0;
      for( ; itA !=pitoiA.end() ; itA++ ){
      //for(;i<pitoiA.size();){
      
        firstPoint =(*itA).contour_point_voxel_coord;  
        if(itA2!=pitoiA.end()) {
          secondPoint=(*itA2).contour_point_voxel_coord;
          itA2++;
        }
        //firstPoint =pitoiA.at(i).contour_point_voxel_coord; 
        //if( pitoiA.size() > ( i+1 ) ) secondPoint = pitoiA.at( i+1 ).contour_point_voxel_coord;       
        else secondPoint = pitoiA.at( 0 ).contour_point_voxel_coord;
      
        for(  ; itB!=pitoiB.end() ;itB++  ){  
        //for( ; j<pitoiB.size() ;  ){
      
          thirdPoint = (*itB).contour_point_voxel_coord; 
          if(itB2!=pitoiB.end()){
            fourthPoint=(*itB2).contour_point_voxel_coord;
            itB2++;
          }
          //thirdPoint = pitoiB.at(j).contour_point_voxel_coord;
          //if( pitoiB.size() > ( j+1 ) ) fourthPoint = pitoiB.at( j+1 ).contour_point_voxel_coord;       
          else fourthPoint = pitoiB.at( 0 ).contour_point_voxel_coord;

          //std::cout <<firstPoint.toString()<<","<<secondPoint.toString()<<","<<thirdPoint.toString()<<","<<fourthPoint.toString()<<std::endl;
      
          if( firstPoint.z != fourthPoint.z )return false;
      
          if( i != j ){
             
            if( HasIntersectionsThisPair( firstPoint , secondPoint, thirdPoint , fourthPoint  ) ) return true;

            
          
          }
          j++;
      
        }
        i++;
      
      
      
      }     

      return false;
     
    }


    
                   
    bool Mask::HasIntersectionsThisPair( const LegacyWorldCoordinate3D& firstPoint ,  const LegacyWorldCoordinate3D& secondPoint,  const LegacyWorldCoordinate3D& thirdPoint ,  const LegacyWorldCoordinate3D& fourthPoint  ) const
	{
    
       // x =   ( secondPoint.x - firstPoint.x ) * alpha +  firstPoint.x;
       // y =   ( secondPoint.y - firstPoint.y ) * alpha +  firstPoint.y;
       
       // x =   ( fourthPoint.x - thirdPoint.x ) * beta +  thirdPoint.x;
       // y =   ( fourthPoint.y - thirdPoint.y ) * beta +  thirdPoint.y;
          
       // x = x
       // alpha   =  (  ( fourthPoint.x - thirdPoint.x ) * beta +  thirdPoint.x - firstPoint.x  ) / ( secondPoint.x - firstPoint.x );   
          
       // y = y       
       // alpha   =  (  ( fourthPoint.y - thirdPoint.y ) * beta +  thirdPoint.y - firstPoint.y  ) / ( secondPoint.y - firstPoint.y );       
          
       // (  ( fourthPoint.x - thirdPoint.x ) * beta +  thirdPoint.x - firstPoint.x  ) / ( secondPoint.x - firstPoint.x ) = (  ( fourthPoint.y - thirdPoint.y ) * beta +  thirdPoint.y - firstPoint.y  ) / ( secondPoint.y - firstPoint.y );   
          
       // (  ( fourthPoint.x - thirdPoint.x ) * beta +  thirdPoint.x - firstPoint.x  ) / ( secondPoint.x - firstPoint.x ) = (  ( fourthPoint.y - thirdPoint.y ) * beta +  thirdPoint.y - firstPoint.y  ) / ( secondPoint.y - firstPoint.y )           
       // (  ( fourthPoint.x - thirdPoint.x ) / ( secondPoint.x - firstPoint.x ) ) * beta +   ( ( thirdPoint.x - firstPoint.x  ) / ( secondPoint.x - firstPoint.x ) )  =   (  ( fourthPoint.y - thirdPoint.y ) / ( secondPoint.y - firstPoint.y ) ) * beta +  ( ( thirdPoint.y - firstPoint.y  ) / ( secondPoint.y - firstPoint.y ) )

       // (  ( fourthPoint.x - thirdPoint.x ) / ( secondPoint.x - firstPoint.x ) ) * beta +   ( ( thirdPoint.x - firstPoint.x  ) / ( secondPoint.x - firstPoint.x ) )  =   (  ( fourthPoint.y - thirdPoint.y ) / ( secondPoint.y - firstPoint.y ) ) * beta +  ( ( thirdPoint.y - firstPoint.y  ) / ( secondPoint.y - firstPoint.y ) )
    
       // (       (  ( fourthPoint.x - thirdPoint.x ) / ( secondPoint.x - firstPoint.x ) )  -     (  ( fourthPoint.y - thirdPoint.y ) / ( secondPoint.y - firstPoint.y ) )       )   * beta      =     ( ( thirdPoint.y - firstPoint.y  ) / ( secondPoint.y - firstPoint.y ) )  -    ( ( thirdPoint.x - firstPoint.x  ) / ( secondPoint.x - firstPoint.x ) ) ;


       // Spezialfall : (  ( secondPoint.y - firstPoint.y ) == 0   )
       
       // y =     firstPoint.y =   ( fourthPoint.y - thirdPoint.y ) * beta +  thirdPoint.y;
       // also :
       // beta =  ( firstPoint.y -  thirdPoint.y ) / ( fourthPoint.y - thirdPoint.y )

       // Spezialfall : (   ( secondPoint.x - firstPoint.x ) == 0   )

       // x =  firstPoint.x = ( fourthPoint.x - thirdPoint.x ) * beta +  thirdPoint.x;
       // also:
       // beta = ( firstPoint.x -  thirdPoint.x ) /  ( fourthPoint.x - thirdPoint.x )


      if(  ( secondPoint.y - firstPoint.y ) == 0   ){
      
        if( ( fourthPoint.y - thirdPoint.y ) != 0 ){
        
          float beta =  ( firstPoint.y -  thirdPoint.y ) / ( fourthPoint.y - thirdPoint.y );

          float x =   ( fourthPoint.x - thirdPoint.x ) * beta +  thirdPoint.x;

          float x_min = secondPoint.x;
          float x_max = firstPoint.x;

          if( secondPoint.x > firstPoint.x ){ 
            x_min = firstPoint.x;
            x_max = secondPoint.x;
          }
          
          bool it_is_in_between = 1;
          if( (x < x_min) || (x > x_max) ) it_is_in_between = 0; 


          if( (beta>0)&&(beta<1) && it_is_in_between ){
            return true;
          }  
          else return false;         
        
        }
        else return false;

      } 

      if(  ( secondPoint.x - firstPoint.x ) == 0   ){
      
        if(   ( fourthPoint.x - thirdPoint.x ) != 0 ){
        
          float beta = ( firstPoint.x -  thirdPoint.x ) /  ( fourthPoint.x - thirdPoint.x );      

          float y =   ( fourthPoint.y - thirdPoint.y ) * beta +  thirdPoint.y;

          float y_min = secondPoint.y;
          float y_max = firstPoint.y;

          if( secondPoint.y > firstPoint.y ){ 
            y_min = firstPoint.y;
            y_max = secondPoint.y;
          }
          
          bool it_is_in_between = 1;
          if( (y < y_min) || (y > y_max) ) it_is_in_between = 0; 
          
          if( (beta>0)&&(beta<1) && it_is_in_between ){

            return true;
          }  
          else return false;  
        }
        else return false;

      }
      
      
      if(  (  ( secondPoint.y - firstPoint.y ) == 0   ) ||  (  ( secondPoint.x - firstPoint.x ) == 0   ) ) assert(0);
      

      if(    (        (  ( fourthPoint.x - thirdPoint.x ) / ( secondPoint.x - firstPoint.x ) )  -     (  ( fourthPoint.y - thirdPoint.y ) / ( secondPoint.y - firstPoint.y ) )     )  == 0  ){
        return false;   // parallel, schneiden sich nicht ! 
      }  
         

      if(    (        (  ( fourthPoint.x - thirdPoint.x ) / ( secondPoint.x - firstPoint.x ) )  -     (  ( fourthPoint.y - thirdPoint.y ) / ( secondPoint.y - firstPoint.y ) )     )  == 0  ) assert( 0 );

    
         float  beta      =    (          ( ( thirdPoint.y - firstPoint.y  ) / ( secondPoint.y - firstPoint.y ) )   -    ( ( thirdPoint.x - firstPoint.x  ) / ( secondPoint.x - firstPoint.x ) )        )           /         (       (  ( fourthPoint.x - thirdPoint.x ) / ( secondPoint.x - firstPoint.x ) )  -     (  ( fourthPoint.y - thirdPoint.y ) / ( secondPoint.y - firstPoint.y ) )       )      ;
         float  alpha     =    (  ( fourthPoint.x - thirdPoint.x ) * beta +  thirdPoint.x - firstPoint.x  ) / ( secondPoint.x - firstPoint.x );     
    
         if(     ( (beta>0)&&(beta<1)  )  &&    ( (alpha>0) && (alpha<1) )    ){   // falls diese Bedingung nicht erfuellt ist, gibt es zwar einen Schnittpunkt der Geraden, aber nicht im relevanten Bereich           
           
           return true;
       
         }

      return false;
    
    }



              

    void Mask::FloodFill4( UnsignedIndexList indexList,int oldcolor,int newcolor,int resolution_level) 
    { 


      LegacyUnsignedIndex3D index;
      index.x = 0;
      index.y = 0;
      index.z = 0;

      while(!indexList.empty()){
         LegacyUnsignedIndex3D frontIndex=indexList.front();
        index.z = frontIndex.z;
        if(MaskField.at( resolution_level )->GetData( frontIndex.x, frontIndex.y, frontIndex.z )==oldcolor){
          //insert index
          if(frontIndex.y+1<=MarkInterestingArea.at(resolution_level).at( frontIndex.z ).index_end.y && MaskField.at( resolution_level )->GetData( frontIndex.x, frontIndex.y+1, frontIndex.z )==oldcolor){
            index.x=frontIndex.x;
            index.y=frontIndex.y+1;
            indexList.push_back(index);
          }
          if(frontIndex.y-1>=MarkInterestingArea.at(resolution_level).at( frontIndex.z ).index_begin.y && MaskField.at( resolution_level )->GetData( frontIndex.x, frontIndex.y-1, frontIndex.z )==oldcolor ){
            index.x=frontIndex.x;
            index.y=frontIndex.y-1;
            indexList.push_back(index);
          }
          if(frontIndex.x-1>=MarkInterestingArea.at(resolution_level).at( frontIndex.z ).index_begin.x && MaskField.at( resolution_level )->GetData( frontIndex.x-1, frontIndex.y, frontIndex.z )==oldcolor){
            index.x=frontIndex.x-1;
            index.y=frontIndex.y;
            indexList.push_back(index);
          }
          if(frontIndex.x+1<=MarkInterestingArea.at(resolution_level).at( frontIndex.z ).index_end.x && MaskField.at( resolution_level )->GetData( frontIndex.x+1, frontIndex.y, frontIndex.z )==oldcolor){
            index.x=frontIndex.x+1;
            index.y=frontIndex.y;
            indexList.push_back(index);
          }

          //mark newcolor
          MaskField.at( resolution_level )->PutData( frontIndex.x, frontIndex.y, frontIndex.z, newcolor );
          SetUppwardBlockThisIntensity( resolution_level , frontIndex , newcolor );
          //delete front
          indexList.pop_front();

        }
        else{
          //delete front
          indexList.pop_front();
        }
      }

    } 



    void Mask::setInsideVoxelPreviousVersion(int resolution_level){

  /*    //doseVoxelIndexInStructure.clear();
      _doseVoxelInStructure.clear();

      LegacyUnsignedIndex3D index;
      index.x = 0;
      index.y = 0;
      index.z = 0; 
      
      bool first=true;
           
      for( index.z = 0 ; index.z < GInf.at(resolution_level).getNumSlices() ; index.z++ ){
 
        LegacyUnsignedIndex3D index_internal;                    
        index_internal.x = 0;
        index_internal.y = 0;
        index_internal.z = index.z;    

        /*First: Mark all voxels as unknown*/
/*        if( ( MarkInterestingArea.at(resolution_level).at( index.z ).index_end.x != 0 ) && ( MarkInterestingArea.at(resolution_level).at( index.z ).index_end.y != 0 ) ){

          for(  index_internal.x = MarkInterestingArea.at(resolution_level).at( index.z ).index_begin.x ; index_internal.x <= MarkInterestingArea.at(resolution_level).at( index.z ).index_end.x  ; index_internal.x++ ){
            for( index_internal.y = MarkInterestingArea.at(resolution_level).at( index.z ).index_begin.y ; index_internal.y <= MarkInterestingArea.at(resolution_level).at( index.z ).index_end.y  ; index_internal.y++ ){

              //set all no border voxels as outside
              if( MaskField.at( resolution_level )->GetData( index_internal.x, index_internal.y, index_internal.z ) != brightness_border ){
                MaskField.at( resolution_level )->PutData( index_internal.x, index_internal.y, index_internal.z, brightness_unknown );
                //MaskField.at( resolution_level )->PutData( index_internal.x, index_internal.y, index_internal.z, brightness_inside );

              }//end if
              else{
                LegacyDoseVoxelIndex3D index2;	   
                index2.x = index_internal.x;

                index2.y = index_internal.y;

                index2.z = index_internal.z;
                
                DoseVoxel doseVoxel=DoseVoxel(index2,this->GInf.at(resolution_level));
                //std::cout <<doseVoxel.getLeftUpperPoint().toString()<<";";
              }

            }//end for index_internal.y
          }//end for index_internal.x
          //std::cout << std::endl;

        }//end if

        LegacyDoseVoxelIndex3D index2;

        index2.x = 0;
        index2.y = 0;
        index2.z = index.z;

        /*Mark all voxels outside the structure: using FloodFill4*/
/*        bool firstVoxelOutStr=false;
        for(  index2.x = MarkInterestingArea.at(resolution_level).at( index.z ).index_begin.x ; index2.x <= MarkInterestingArea.at(resolution_level).at( index.z ).index_end.x && !firstVoxelOutStr  ; index2.x++ ){
          for( index2.y = MarkInterestingArea.at(resolution_level).at( index.z ).index_begin.y ; index2.y <= MarkInterestingArea.at(resolution_level).at( index.z ).index_end.y && !firstVoxelOutStr  ; index2.y++ ){

            DoseVoxel doseVoxel=DoseVoxel(index2,this->GInf.at(resolution_level));
            if(doseVoxel.voxelInStructure(*_structure)==0){
              //std::cout << "first outside voxel: "<<index2.toString()<<std::endl;
              firstVoxelOutStr=true;
              break;
            }//end if

          }//end for index2.y
          if(firstVoxelOutStr)
            break;
        }//end for index2.x

        if(firstVoxelOutStr)
          this->FloodFill4(index2.x,index2.y,index2.z,brightness_unknown, brightness_outside,resolution_level);


        //get corresponding polygons   
        DoseVoxel doseVoxelZPlane=DoseVoxel(index2,this->GInf.at(resolution_level));
        std::vector<int> sliceV=doseVoxelZPlane.getZIntersectPolygonIndexVector(*_structure); 

        for(int i=0;i<sliceV.size();i++){
          LegacyPolygonType polygon=_structure->getLegacyStructureVector().at(sliceV.at(i));

          Contour contour=Contour(polygon);//get the contour in this slice

          LegacyPolygonType box=contour.getMinimalBox();
          Contour boxContour=Contour(box);

          bool firstVoxelInStr=false;


          /*Mark all voxels inside a polygon of the structure: using FloodFill4*/
/*          for(  index2.x = MarkInterestingArea.at(resolution_level).at( index.z ).index_begin.x ; index2.x <= MarkInterestingArea.at(resolution_level).at( index.z ).index_end.x && !firstVoxelInStr  ; index2.x++ ){

            for( index2.y = MarkInterestingArea.at(resolution_level).at( index.z ).index_begin.y ; index2.y <= MarkInterestingArea.at(resolution_level).at( index.z ).index_end.y && !firstVoxelInStr  ; index2.y++ ){
              if(MaskField.at( resolution_level )->GetData( index2.x, index2.y, index2.z )==brightness_unknown) {

                DoseVoxel doseVoxel=DoseVoxel(index2,this->GInf.at(resolution_level));

                //if doseVoxel inside the box

                LegacyWorldCoordinate2D luP={doseVoxel.getLeftUpperPoint().x,doseVoxel.getLeftUpperPoint().y};

                //if left upper point of this voxel inside the box

                if(luP.x>=box.at(0).x && luP.y>=box.at(0).y && luP.x<=box.at(2).x && luP.y<=box.at(2).y){

                  if(doseVoxel.voxelInStructure(*_structure)==1){

                    //std::cout << "first inside voxel: "<<index2.toString()<<std::endl;	

                    firstVoxelInStr=true;				

                    break;

                  }//end if

                }//end if

              }

            }//end for index2.y
            if(firstVoxelInStr)
              break;
          }//end for index2.x

          if(firstVoxelInStr)
            this->FloodFill4(index2.x,index2.y,index2.z,brightness_unknown,brightness_inside,resolution_level);

        }//end for i
        
        //check unknown voxels
        int sizeUnknown=0;
        for(  index2.x = MarkInterestingArea.at(resolution_level).at( index.z ).index_begin.x ; index2.x <= MarkInterestingArea.at(resolution_level).at( index.z ).index_end.x   ; index2.x++ ){
          for( index2.y = MarkInterestingArea.at(resolution_level).at( index.z ).index_begin.y ; index2.y <= MarkInterestingArea.at(resolution_level).at( index.z ).index_end.y   ; index2.y++ ){
            if(MaskField.at( resolution_level )->GetData(index2.x,index2.y,index2.z)==brightness_unknown){
              //std::cout << index2.toString()<<"---";
              //sizeUnknown++;
              DoseVoxel doseVoxel=DoseVoxel(index2,this->GInf.at(resolution_level));
              double voxelInStr=doseVoxel.voxelInStructure(*_structure);
              if(voxelInStr>0){
                this->FloodFill4(index2.x,index2.y,index2.z,brightness_unknown,brightness_inside,resolution_level);
              }
              else{
                this->FloodFill4(index2.x,index2.y,index2.z,brightness_unknown, brightness_outside,resolution_level);
              }
            }
          }
        }
        /*if(sizeUnknown!=0)
        std::cout << "Unknown: "<<sizeUnknown<<std::endl;*/

/*      }//end for index.z
*/      

    }



    void Mask::setInsideVoxel( int resolution_level , std::vector< UnsignedIndexList > indexListInside, std::vector< UnsignedIndexList > indexListOutside ){


      _doseVoxelInStructure.clear();

      LegacyUnsignedIndex3D index;
      index.x = 0;
      index.y = 0;
      index.z = 0; 
      
      bool first=true;
           
      for( index.z = 0 ; index.z < GInf.at(resolution_level).getNumSlices() ; index.z++ ){

        if( indexListOutside.at(index.z).size() == 0 ){
 
          LegacyDoseVoxelIndex3D index2;

          index2.x = 0;
          index2.y = 0;
          index2.z = index.z;
          /*Mark all voxels outside the structure: using FloodFill4*/
          bool firstVoxelOutStr=false;
          for(  index2.x = MarkInterestingArea.at(resolution_level).at( index.z ).index_begin.x ; index2.x <= MarkInterestingArea.at(resolution_level).at( index.z ).index_end.x && !firstVoxelOutStr  ; index2.x++ ){
            for( index2.y = MarkInterestingArea.at(resolution_level).at( index.z ).index_begin.y ; index2.y <= MarkInterestingArea.at(resolution_level).at( index.z ).index_end.y && !firstVoxelOutStr  ; index2.y++ ){

              DoseVoxel doseVoxel=DoseVoxel(index2,&(this->GInf.at(resolution_level)));
              if(doseVoxel.voxelInStructure(*_structure)==0){

                 LegacyUnsignedIndex3D indexUint;
                indexUint.x = static_cast<Uint16>(index2.x);
                indexUint.y = static_cast<Uint16>(index2.y);
                indexUint.z = static_cast<Uint16>(index2.z);

                indexListOutside.at(index.z).push_back( indexUint );
                firstVoxelOutStr=true;
                break;
                
              }//end if

            }//end for index2.y
            if(firstVoxelOutStr)
              break;
          }//end for index2.x

        }

        if( indexListOutside.at(index.z).size() > 0 )
          this->FloodFill4(indexListOutside.at(index.z),brightness_unknown, brightness_outside,resolution_level);

        if( indexListInside.at(index.z).size() == 0 ){

          LegacyDoseVoxelIndex3D index2;

          index2.x = 0;
          index2.y = 0;
          index2.z = index.z;

          //get corresponding polygons   
          DoseVoxel doseVoxelZPlane=DoseVoxel(index2,&(this->GInf.at(resolution_level)));
          std::vector<int> sliceV=doseVoxelZPlane.getZIntersectPolygonIndexVector(*_structure); 

          for(int i=0;i<sliceV.size();i++){
            const LegacyPolygonType polygon=_structure->getLegacyStructureVector().at(sliceV.at(i));

            Contour contour=Contour(polygon);//get the contour in this slice

            LegacyPolygonType box=contour.getMinimalBox();
            Contour boxContour=Contour(box);

            bool firstVoxelInStr=false;


            /*Mark all voxels inside a polygon of the structure: using FloodFill4*/
            for(  index2.x = MarkInterestingArea.at(resolution_level).at( index.z ).index_begin.x ; index2.x <= MarkInterestingArea.at(resolution_level).at( index.z ).index_end.x && !firstVoxelInStr  ; index2.x++ ){

              for( index2.y = MarkInterestingArea.at(resolution_level).at( index.z ).index_begin.y ; index2.y <= MarkInterestingArea.at(resolution_level).at( index.z ).index_end.y && !firstVoxelInStr  ; index2.y++ ){
                if(MaskField.at( resolution_level )->GetData( index2.x, index2.y, index2.z )==brightness_unknown) {

                  DoseVoxel doseVoxel=DoseVoxel(index2,&(this->GInf.at(resolution_level)));

                  //if doseVoxel inside the box

                  LegacyWorldCoordinate2D luP={doseVoxel.getLeftUpperPoint().x(),doseVoxel.getLeftUpperPoint().y()};

                  //if left upper point of this voxel inside the box

                  if(luP.x>=box.at(0).x && luP.y>=box.at(0).y && luP.x<=box.at(2).x && luP.y<=box.at(2).y){

                    if(doseVoxel.voxelInStructure(*_structure)==1){

                      //std::cout << "first inside voxel: "<<index2.toString()<<std::endl;	

                      firstVoxelInStr=true;				

                      break;

                    }//end if

                  }//end if

                }

              }//end for index2.y
              if(firstVoxelInStr)
                break;
            }//end for index2.x

            if(firstVoxelInStr){
                 LegacyUnsignedIndex3D indexUint;
                indexUint.x = static_cast<Uint16>(index2.x);
                indexUint.y = static_cast<Uint16>(index2.y);
                indexUint.z = static_cast<Uint16>(index2.z);
                indexListInside.at(index.z).push_back(indexUint);
            }

          }//end for i

        }
     
        if(indexListInside.at(index.z).size() > 0)this->FloodFill4(indexListInside.at(index.z),brightness_unknown,brightness_inside,resolution_level);        

        //check unknown voxels
        int sizeUnknown=0;

        LegacyDoseVoxelIndex3D index2;

        index2.x = 0;
        index2.y = 0;
        index2.z = index.z;
       
        for(  index2.x = MarkInterestingArea.at(resolution_level).at( index.z ).index_begin.x ; index2.x <= MarkInterestingArea.at(resolution_level).at( index.z ).index_end.x   ; index2.x++ ){
          for( index2.y = MarkInterestingArea.at(resolution_level).at( index.z ).index_begin.y ; index2.y <= MarkInterestingArea.at(resolution_level).at( index.z ).index_end.y   ; index2.y++ ){
            if(MaskField.at( resolution_level )->GetData(index2.x,index2.y,index2.z)==brightness_unknown){
              //std::cout << index2.toString()<<"---";
              //sizeUnknown++;
              DoseVoxel doseVoxel=DoseVoxel(index2,&(this->GInf.at(resolution_level)));
              double voxelInStr=doseVoxel.voxelInStructure(*_structure);
              if(voxelInStr>0){
                indexListInside.at(index.z).resize(0);
                 LegacyUnsignedIndex3D indexUint;
                indexUint.x = static_cast<Uint16>(index2.x);
                indexUint.y = static_cast<Uint16>(index2.y);
                indexUint.z = static_cast<Uint16>(index2.z);
                indexListInside.at(index.z).push_back(indexUint);
                this->FloodFill4(indexListInside.at(index.z),brightness_unknown,brightness_inside,resolution_level);
              }
              else{
                indexListOutside.at(index.z).resize(0);
                 LegacyUnsignedIndex3D indexUint;
                indexUint.x = static_cast<Uint16>(index2.x);
                indexUint.y = static_cast<Uint16>(index2.y);
                indexUint.z = static_cast<Uint16>(index2.z);
                indexListOutside.at(index.z).push_back(indexUint);
                this->FloodFill4(indexListOutside.at(index.z),brightness_unknown, brightness_outside,resolution_level);
              }
            }
          }
        }
        /*if(sizeUnknown!=0)
        std::cout << "Unknown: "<<sizeUnknown<<std::endl;*/

      }//end for index.z

    }


    void Mask::CountThisColor( int resolution_level){
    
      int counter_inside = 0;
      int counter_outside = 0;
      int counter_border = 0;
      int counter_unknown = 0;
    
      for( int x = 0 ; x < MaskField.at( resolution_level )->GetDimX() ; x++ ){
        for( int y = 0 ; y < MaskField.at( resolution_level )->GetDimY() ; y++ ){
          for( int z = 0 ; z < MaskField.at( resolution_level )->GetDimZ() ; z++ ){
          
              if( MaskField.at( resolution_level )->GetData(x,y,z) == brightness_inside ) counter_inside++;
              if( MaskField.at( resolution_level )->GetData(x,y,z) == brightness_outside ) counter_outside++;
              if( MaskField.at( resolution_level )->GetData(x,y,z) == brightness_border ) counter_border++;
              if( MaskField.at( resolution_level )->GetData(x,y,z) == brightness_unknown ) counter_unknown++;
          
          }
        }
      }
      
      std::cout<< " counter_inside : " << counter_inside <<std::endl;
     // std::cout<< " counter_outside : " << counter_outside <<std::endl;
      std::cout<< " counter_border : " << counter_border <<std::endl;
      std::cout<< " counter_unknown : " << counter_unknown <<std::endl;
    
    }

    void Mask::setInsideVoxelMultiRes( int resolution_level ){
    
      std::vector< UnsignedIndexList > indexListInside; 
      std::vector< UnsignedIndexList > indexListOutside;

      for( int z = 0 ; z < GInf.at(resolution_level).getNumSlices() ; z++ ){      
         UnsignedIndexList a_list;
         a_list.resize(0);
         indexListInside.push_back(a_list);
         indexListOutside.push_back(a_list);
      }

      if( resolution_level < ( MaskField.size() - 1 ) )MaskField.at( resolution_level )->GetBorderRegion( MarkInterestingArea.at(resolution_level), indexListInside , indexListOutside, brightness_unknown, brightness_inside, brightness_outside  ); 
      setInsideVoxel( resolution_level , indexListInside, indexListOutside );      
    
    }
    

                
    void Mask::SetContentUnknown(){

     for( int resolution_level = 0 ; resolution_level < GInf.size() ; resolution_level++ ){

      for( int index_z = 0 ; index_z < GInf.at(resolution_level).getNumSlices() ; index_z++ ){
 

        LegacyUnsignedIndex3D index_internal;                    
        index_internal.x = 0;
        index_internal.y = 0;
        index_internal.z = index_z;    


        /*First: Mark all voxels as unknown*/
        if( ( MarkInterestingArea.at(resolution_level).at( index_z ).index_end.x != 0 ) && ( MarkInterestingArea.at(resolution_level).at( index_z ).index_end.y != 0 ) ){

          for(  index_internal.x = MarkInterestingArea.at(resolution_level).at( index_z ).index_begin.x ; index_internal.x <= MarkInterestingArea.at(resolution_level).at( index_z ).index_end.x  ; index_internal.x++ ){
            for( index_internal.y = MarkInterestingArea.at(resolution_level).at( index_z ).index_begin.y ; index_internal.y <= MarkInterestingArea.at(resolution_level).at( index_z ).index_end.y  ; index_internal.y++ ){


              //set all no border voxels as outside
              if( MaskField.at( resolution_level )->GetData( index_internal.x, index_internal.y, index_internal.z ) != brightness_border ){
                MaskField.at( resolution_level )->PutData( index_internal.x, index_internal.y, index_internal.z, brightness_unknown );
                //MaskField.at( resolution_level )->PutData( index_internal.x, index_internal.y, index_internal.z, brightness_inside );

              }//end if


            }//end for index_internal.y
          }//end for index_internal.x
          //std::cout << std::endl;

        }//end if
        
      }   

     }

    }
    

    void Mask::SeperateRegionsQuick(){
    
      //doseVoxelIndexInStructure.clear();
      _doseVoxelInStructure.clear();

      LegacyUnsignedIndex3D index;
      index.x = 0;
      index.y = 0;
      index.z = 0; 

      for( index.z = 0 ; index.z < GInf.at(0).getNumSlices() ; index.z++ ){

        LegacyUnsignedIndex3D index_internal;
        index_internal.x = 0;
        index_internal.y = 0;
        index_internal.z = index.z;    

        if( ( MarkInterestingArea.at(0).at( index.z ).index_end.x != 0 ) && ( MarkInterestingArea.at(0).at( index.z ).index_end.y != 0 ) ){

          for(  index_internal.x = MarkInterestingArea.at(0).at( index.z ).index_begin.x ; index_internal.x <= MarkInterestingArea.at(0).at( index.z ).index_end.x  ; index_internal.x++ ){
            for( index_internal.y = MarkInterestingArea.at(0).at( index.z ).index_begin.y ; index_internal.y <= MarkInterestingArea.at(0).at( index.z ).index_end.y  ; index_internal.y++ ){

              if( MaskField.at( 0 )->GetData( index_internal.x, index_internal.y, index_internal.z ) != brightness_border ){

                MaskField.at( 0 )->PutData( index_internal.x, index_internal.y, index_internal.z, brightness_inside );

              }

            }
          }

        }

        index.y = MarkInterestingArea.at(0).at( index.z ).index_begin.y;      

        for( index.x = MarkInterestingArea.at(0).at( index.z ).index_begin.x ; index.x <= MarkInterestingArea.at(0).at( index.z ).index_end.x ; index.x++ ){

          if( MaskField.at( 0 )->GetData( index.x, index.y, index.z ) == brightness_inside ){
            MaskField.at( 0 )->PutData( index.x, index.y, index.z, brightness_outside );
            ConnectArea( brightness_inside , brightness_outside , index , MarkInterestingArea.at(0).at( index.z ).index_begin , MarkInterestingArea.at(0).at( index.z ).index_end );
          }

        }

        index.y = MarkInterestingArea.at(0).at( index.z ).index_end.y; 

        for( index.x = MarkInterestingArea.at(0).at( index.z ).index_begin.x ; index.x <= MarkInterestingArea.at(0).at( index.z ).index_end.x ; index.x++ ){

          if( MaskField.at( 0 )->GetData( index.x, index.y, index.z ) == brightness_inside ){
            MaskField.at( 0 )->PutData( index.x, index.y, index.z, brightness_outside );
            ConnectArea( brightness_inside , brightness_outside , index , MarkInterestingArea.at(0).at( index.z ).index_begin , MarkInterestingArea.at(0).at( index.z ).index_end );
          }

        }

        index.x = MarkInterestingArea.at(0).at( index.z ).index_begin.x;

        for( index.y = MarkInterestingArea.at(0).at( index.z ).index_begin.y ; index.y <= MarkInterestingArea.at(0).at( index.z ).index_end.y  ; index.y++ ){

          if( MaskField.at( 0 )->GetData( index.x, index.y, index.z ) == brightness_inside ){
            MaskField.at( 0 )->PutData( index.x, index.y, index.z, brightness_outside );
            ConnectArea( brightness_inside , brightness_outside , index , MarkInterestingArea.at(0).at( index.z ).index_begin , MarkInterestingArea.at(0).at( index.z ).index_end );
          }

        }

        index.x = MarkInterestingArea.at(0).at( index.z ).index_end.x;

        for( index.y = MarkInterestingArea.at(0).at( index.z ).index_begin.y ; index.y <= MarkInterestingArea.at(0).at( index.z ).index_end.y  ; index.y++ ){

          if( MaskField.at( 0 )->GetData( index.x, index.y, index.z ) == brightness_inside ){
            MaskField.at( 0 )->PutData( index.x, index.y, index.z, brightness_outside );
            ConnectArea( brightness_inside , brightness_outside , index , MarkInterestingArea.at(0).at( index.z ).index_begin , MarkInterestingArea.at(0).at( index.z ).index_end );
          }

        }



      }       
    

    }






    void Mask::SetDoseVoxelInStructureVector(){

      _doseVoxelInStructure.clear();

      DoseVoxel aDoseVoxel; 

      LegacyUnsignedIndex3D index;
      index.x = 0;
      index.y = 0;
      index.z = 0;

      LegacyDoseVoxelIndex3D aDoseIndex;
      aDoseIndex.x = 0;
      aDoseIndex.y = 0;
      aDoseIndex.z = 0;

      double fraction = 0;


      for( index.z = 0 ; index.z < GInf.at(0).getNumSlices() ; index.z++ ){      

      if( FieldOfIntersections ){
          FieldOfIntersections->SetZero();
          GetIntersectionInformationThisSlice( index.z );
          
      }       
      
      for( index.x = 0 ; index.x < GInf.at(0).getNumColumns() ; index.x++ ){
        for( index.y = 0 ; index.y < GInf.at(0).getNumRows() ; index.y++ ){


            aDoseIndex.x = index.x;
            aDoseIndex.y = index.y;
            aDoseIndex.z = index.z;
            

            if(  brightness_inside == MaskField.at( 0 )->GetData( index.x, index.y, index.z ) ){
            
              //doseVoxelIndexInStructure.push_back( aDoseIndex );
              fraction = 1.0;


              DoseVoxel doseVoxel=DoseVoxel(aDoseIndex,&(GInf.at(0)));
              doseVoxel.setProportionInStr(fraction);
              this->_doseVoxelInStructure.push_back(doseVoxel);
              // und jetzt fraction irgendwie anhaengen an geeignete Struktur und dann raus damit
              //std::cout << "inside: "<<aDoseIndex.toString()<<"; ";

            }
            if(  brightness_border == MaskField.at( 0 )->GetData( index.x, index.y, index.z )  ){


               
              if( !do_detailed_subvox ){ /*Lanlan*/
                
                
                DoseVoxel doseVoxel=DoseVoxel(aDoseIndex,&(GInf.at(0)));
                fraction=doseVoxel.voxelInStructure(*_structure);//simple test
                if(fraction>0){
                  //doseVoxelIndexInStructure.push_back( aDoseIndex );
                  doseVoxel.setProportionInStr(fraction);
                  this->_doseVoxelInStructure.push_back(doseVoxel);
                } 


              
              }
              else{ /*Martina..............*/

                //doseVoxelIndexInStructure.push_back( aDoseIndex );
              
                fraction = 1.0; 
              

              
                fraction = GetFractionInside( aDoseIndex ); 
                
                if( ( fraction < 0 ) || ( fraction > 1.0 ) ) assert(0);  


 
                DoseVoxel doseVoxel=DoseVoxel(aDoseIndex,&(GInf.at(0)));
                doseVoxel.setProportionInStr( fraction ); 
                _doseVoxelInStructure.push_back( doseVoxel );

              }  



            }          
          }
        }
      }




      double total_volume = 0;

      for( int counter = 0 ; counter <  _doseVoxelInStructure.size() ; counter++ ){

        total_volume +=   _doseVoxelInStructure.at( counter ).getProportionInStr();

      }

      //std::cout<< "  total_volume : " << total_volume  <<std::endl;


      //std::cout<< " _doseVoxelInStructure.size() " << _doseVoxelInStructure.size() <<std::endl;



    }





    void Mask::ConnectArea( field_content_type brightness_from , field_content_type brightness_to , LegacyUnsignedIndex3D start_index, LegacyDoseVoxelIndex2D index_begin, LegacyDoseVoxelIndex2D index_end  ){

      if(  ( start_index.x >= index_begin.x ) && ( start_index.y >= index_begin.y ) && ( start_index.x <= index_end.x ) && ( start_index.y <= index_end.y )  ){ 

		  std::deque<LegacyUnsignedIndex2D> index_list;

        LegacyUnsignedIndex2D this_index;
        this_index.x = start_index.x;
        this_index.y = start_index.y;

        LegacyUnsignedIndex2D another_index;
        another_index.x = 0;
        another_index.y = 0;

        MaskField.at( 0 )->PutData( start_index.x, start_index.y, start_index.z, brightness_to );
        index_list.push_back( this_index );

        int counter = 1;  

        while( index_list.size() > 0 ){

          //    std::cout<< " counter " << counter <<std::endl;

          this_index = index_list.back();
          index_list.pop_back();
          counter--;

          if( this_index.x > index_begin.x ){
            another_index.x = this_index.x - 1;
            another_index.y = this_index.y;
            if( MaskField.at( 0 )->GetData( another_index.x , another_index.y , start_index.z ) == brightness_from ){
              index_list.push_back( another_index );
              counter++;
              MaskField.at( 0 )->PutData( another_index.x, another_index.y, start_index.z, brightness_to ); 
            }
          }

          if( this_index.y > index_begin.y ){
            another_index.x = this_index.x;
            another_index.y = this_index.y - 1;
            if( MaskField.at( 0 )->GetData( another_index.x , another_index.y , start_index.z ) == brightness_from ){
              index_list.push_back( another_index );
              counter++;
              MaskField.at( 0 )->PutData( another_index.x, another_index.y, start_index.z, brightness_to );
            }
          }    


          if( this_index.x < index_end.x ){
            another_index.x = this_index.x + 1;
            another_index.y = this_index.y;
            if( MaskField.at( 0 )->GetData( another_index.x , another_index.y , start_index.z ) == brightness_from ){
              index_list.push_back( another_index );
              counter++;
              MaskField.at( 0 )->PutData( another_index.x, another_index.y, start_index.z, brightness_to );          
            }      
          }

          if( this_index.y < index_end.y ){
            another_index.x = this_index.x;
            another_index.y = this_index.y + 1;
            if( MaskField.at( 0 )->GetData( another_index.x , another_index.y , start_index.z ) == brightness_from ){
              index_list.push_back( another_index );
              counter++;
              MaskField.at( 0 )->PutData( another_index.x, another_index.y, start_index.z, brightness_to );       
            }      
          }    


        }

      }  

    } 


    // die folgende Funktion prueft immer erst, ob es sich um einen Schnittpunkt mit der Ecke handelt. Falls nicht wird er in die Reihe der Kantenschnittpunkte einsortiert. Betrachtet wird lediglich die erste Ecke.
    void Mask::SetThisIntersectionToCornerOrEdge( unsigned int index_x , unsigned int index_y , IterativePolygonInTermsOfIntersections::WhichIntersection intersection , LegacyWorldCoordinate2D coord2D, unsigned int edge_number, IntersectionsThisVoxel& voxel_intersections ){
                                        
                                                               
       if(edge_number==0){
         SetThisIntersection( index_x , (index_x+1) , index_y ,  intersection , coord2D.x, coord2D.y , voxel_intersections, edge_number ); 
       }
       else if(edge_number==1){
         SetThisIntersection( index_y , (index_y+1) , ( index_x + 1 ) ,  intersection , coord2D.y, coord2D.x , voxel_intersections, edge_number );
         //SetThisIntersectionYIncreasing( index_x , index_y , intersection , coord2D , voxel_intersections );
       }
       else if(edge_number==2){
         SetThisIntersection( ( index_x + 1 ) , index_x , ( index_y + 1 ) ,  intersection , coord2D.x, coord2D.y , voxel_intersections, edge_number );
         //SetThisIntersectionXDecreasing( index_x , index_y , intersection , coord2D , voxel_intersections );
       }
       else if(edge_number==3){
         SetThisIntersection( ( index_y + 1 ) , index_y , index_x , intersection , coord2D.y, coord2D.x , voxel_intersections, edge_number );
         //SetThisIntersectionYDecreasing( index_x , index_y , intersection , coord2D , voxel_intersections );
       }    
       else assert(0);
                 
    } 

    

    void Mask::SetThisIntersection( unsigned int corner_of_interest , unsigned int corner_next, unsigned int corner_fixed ,  IterativePolygonInTermsOfIntersections::WhichIntersection intersection , double coord_of_interest , double coord_fixed, IntersectionsThisVoxel& voxel_intersections , unsigned int edge_number ){
    
       if( coord_fixed != static_cast<double>(corner_fixed)  ) assert(0); // this should never happen since coord_fixed is the coordinate which is the same all along the edge that starts with corner_fixed


       if( coord_of_interest == static_cast<double>(corner_of_interest) ){  // falls der Schnittpunkt direkt am Anfang der Kante liegt also der Anfangspunkt der Kante ist

         voxel_intersections.corner_intersections_intersection_coord.at(edge_number).push_back(  intersection );  // dann haenge den Schnittpunkt an die Liste der Schnittpunkte mit eben diesem Anfangspunkt der Kante an           
         return;
         
       }  
       else{       
        
         if( voxel_intersections.edge_intersections_intersection_coord.at(edge_number).size() > 0 ){  // falls die Kante schon Schnittpunkte hat       
           double first_coord_double = voxel_intersections.edge_intersections_vox_coord.at(edge_number).at(0).at(0);          
           // In der naechsten Zeile wird geprueft, ob der Schnittpunkt zwischen dem Anfang der Kante und dem ersten bereits bekannten Schnittpunkt liegt. Falls ja, wird er entsprechend eingefuegt.
           if( CompareIfBetweenInsertAtBeginning( coord_of_interest , static_cast<double>(corner_of_interest) , first_coord_double , edge_number, intersection , voxel_intersections )  ) return;     
         }      
         // naechste Zeile: Pruefe, ob der Schnittpunkt in der Liste der Schnittpunkte mit dieser Kante schon vorhanden ist und falls ja, verlaengere die Liste der Punkte, die eben in diesem Punkt schneiden um den neuen Schnittpunkt. 
         // Puntemaessig ist das zwar der selbe, der schon mindestens einmal da ist, aber intersection ist ja eine Art Index im PolygonOfIntersections 
         // und muss dann hier einsortiert werden. Denn das Polygon kann tatsaechlich mehrfach durch den selben Punkt laufen.   
         // Falls der Schnittpunkt noch nicht vorkommt, aber zwischen zweien ist, die bereits vorkommen, wird er entsprechend eingefuegt.      
         if(  CheckIfIndenticalOrBetweenRespectiveVectorElementsAndDoInsert( coord_of_interest , edge_number, intersection , voxel_intersections ) ) return;
            
         double that_coord_double = 0;

         // Die folgenden beiden Zeilen sind dafuer zustaendig that_coord_double mit entweder dem letzten bereits bekannten Schnittpunkt dieser Kante,
         // oder alternativ mit dem Anfangspunkt der Kante zu belegen.
         if( voxel_intersections.edge_intersections_vox_coord.at(edge_number).size() > 0 ) that_coord_double = voxel_intersections.edge_intersections_vox_coord.at(edge_number).at(  ( voxel_intersections.edge_intersections_intersection_coord.at(edge_number).size() - 1 )  ).at(0) ;
         else that_coord_double = static_cast<double>( corner_of_interest );          

         if( that_coord_double == coord_of_interest  ){
          
              if(  coord_of_interest  ==  static_cast<double>( corner_of_interest )  ) assert( 0 );  // Das kann nicht sein, denn falls der Schnittpunkt wirklich der erste Punkt der Kante waere, muesste das ganz zu Beginn dieser Funktion bereits erkannt worden sein und die Funktion haette dann bereits returned. Falls that_coord_double also coord_of_interest (eizusortierender Punk) ist, kann dieser nicht der Anfangspunkt der Kante sein.  
         
              // der einzusortierende Punkt coord_of_interest ist gerade gleich dem letzten bereits gefundenen Schnittpunkt that_coord_double und wird der Liste seinder Vorkommnisse entsprechend hinzugefuegt. 
              voxel_intersections.edge_intersections_vox_coord.at(edge_number).at(      ( voxel_intersections.edge_intersections_intersection_coord.at(edge_number).size() - 1 )    ).push_back( coord_of_interest );
              voxel_intersections.edge_intersections_intersection_coord.at(edge_number).at(    ( voxel_intersections.edge_intersections_intersection_coord.at(edge_number).size() - 1 )    ).push_back( intersection ); 
              return;              
         
         }
         else{

           // Hier wird geprueft, ob der betrachtete Punkt zwischen dem letzten bereits gefundenen Schnittpunkt und dem Ende der Kante liegt. Falls ja, wird er vor dem Kantenende entsprechend eingefuegt.
           if( CompareIfBetweenAppend( coord_of_interest , that_coord_double, static_cast<double>( corner_next ), edge_number, intersection , voxel_intersections )  ){

             return;

           }
         
           if( coord_of_interest == static_cast<double>( corner_next ) ) assert(0); // this case is not supposed to be considered here. Its the next edge and should have gone elsewhere. Das Ende der Kante ist der Anfang der naechsten Kante uns haette daher bei dieser naechsten Kante landen muessen.    
         }
         assert( 0 ); // assigned to the wrong edge by programmer. It may be the second corner attachted to this edge, however, it is not supposed to be considered here.         
       }
    
    }



    void Mask::SetThisIntersectionXIncreasing( unsigned int index_x , unsigned int index_y ,  IterativePolygonInTermsOfIntersections::WhichIntersection intersection , LegacyWorldCoordinate2D coord2D , IntersectionsThisVoxel& voxel_intersections , unsigned int edge_number ){
    
       if( coord2D.y != static_cast<double>(index_y)  ) assert(0); // this should never happen 

       if( coord2D.x == static_cast<double>(index_x) ){
 
         voxel_intersections.corner_intersections_intersection_coord.at(0).push_back(  intersection );             
         return;
         
       }  
       else{
        
         if( voxel_intersections.edge_intersections_intersection_coord.at(0).size() > 0 ){
         
           double first_coord_double = voxel_intersections.edge_intersections_vox_coord.at(0).at(0).at(0);
           if( CompareIfBetweenInsertAtBeginning( coord2D.x , static_cast<double>(index_x) , first_coord_double , edge_number, intersection , voxel_intersections )  ) return;
         
         }
        
         if(  CheckIfIndenticalOrBetweenRespectiveVectorElementsAndDoInsert( coord2D.x , edge_number, intersection , voxel_intersections ) ) return;
              
         double that_coord_double = 0;

         if( voxel_intersections.edge_intersections_vox_coord.at(0).size() > 0 ) that_coord_double = voxel_intersections.edge_intersections_vox_coord.at(0).at(  ( voxel_intersections.edge_intersections_intersection_coord.at(0).size() - 1 )  ).at(0) ;
         else that_coord_double = static_cast<double>( index_x ); 
         

         if( that_coord_double == coord2D.x  ){
          
              if(  coord2D.x  ==  static_cast<double>( index_x )  ) assert( 0 ); 
         
              voxel_intersections.edge_intersections_vox_coord.at(0).at(      ( voxel_intersections.edge_intersections_intersection_coord.at(0).size() - 1 )    ).push_back( coord2D.x );
              voxel_intersections.edge_intersections_intersection_coord.at(0).at(    ( voxel_intersections.edge_intersections_intersection_coord.at(0).size() - 1 )    ).push_back( intersection ); 
              return;              
         
         }
         else{

           if( CompareIfBetweenAppend( coord2D.x , that_coord_double, static_cast<double>( index_x + 1 ), edge_number, intersection , voxel_intersections )  ) return;
         
           if( coord2D.x == static_cast<double>( index_x + 1 ) ) assert(0); // this case is not supposed to be treated here.
         
         }
 
         assert( 0 ); // assigned to the wrong edge by programmer. It may be the second corner attachted to this edge, however, it is not supposed to be treated here.
         
       }
      
    }


    bool Mask::CompareIfBetweenAppend( double value_to_compare , double small_value_to_compare_with , double big_value_to_compare_with, unsigned int edge_number, IterativePolygonInTermsOfIntersections::WhichIntersection intersection , IntersectionsThisVoxel& voxel_intersections ){

           if( ( ( value_to_compare < big_value_to_compare_with ) && ( value_to_compare >  small_value_to_compare_with  ) ) || ( ( value_to_compare > big_value_to_compare_with ) && ( value_to_compare <  small_value_to_compare_with  ) )  ){
          
              std::vector< double > double_vec;
              double_vec.push_back(value_to_compare) ;
              
              std::vector< IterativePolygonInTermsOfIntersections::WhichIntersection > intersection_vec;
              intersection_vec.push_back( intersection );

              voxel_intersections.edge_intersections_vox_coord.at(edge_number).push_back(  double_vec );
              voxel_intersections.edge_intersections_intersection_coord.at(edge_number).push_back( intersection_vec ); 
              return true;            
           
           }
           else return false;
           
    }       


    bool Mask::CheckIfIndenticalOrBetweenRespectiveVectorElementsAndDoInsert( double value_to_compare , unsigned int edge_number, IterativePolygonInTermsOfIntersections::WhichIntersection intersection , IntersectionsThisVoxel& voxel_intersections ){

         for( int i = 0 ; i < ( static_cast<int>(voxel_intersections.edge_intersections_intersection_coord.at(edge_number).size()) - 1 ) ; i++ ){                

            double the_coord_double = voxel_intersections.edge_intersections_vox_coord.at(edge_number).at(i).at(0) ;
           
            if( value_to_compare == the_coord_double ){
   
              voxel_intersections.edge_intersections_vox_coord.at(edge_number).at(i).push_back( value_to_compare );
              voxel_intersections.edge_intersections_intersection_coord.at(edge_number).at(i).push_back( intersection ); 
              return true;
              
            }
            else{
        
              double another_double = voxel_intersections.edge_intersections_vox_coord.at(edge_number).at(i+1).at(0) ;
              
              if( ( ( another_double > value_to_compare ) && ( the_coord_double < value_to_compare ) ) ||  ( ( another_double < value_to_compare ) && ( the_coord_double > value_to_compare ) )   ){
         
                std::vector< double > double_vec;
                double_vec.push_back( value_to_compare ) ;
                std::vector< IterativePolygonInTermsOfIntersections::WhichIntersection > intersection_vec;
                intersection_vec.push_back( intersection ); 
                intersection_coord_iterator_type it = voxel_intersections.edge_intersections_intersection_coord.at(edge_number).begin() + ( i + 1 );   
    
                voxel_intersections.edge_intersections_vox_coord.at(edge_number).insert( ( voxel_intersections.edge_intersections_vox_coord.at(edge_number).begin() + ( i + 1  ) ) , double_vec ); 
                voxel_intersections.edge_intersections_intersection_coord.at(edge_number).insert( it , intersection_vec );
                
                return true;
              
              }
            }                                   
                   
         }
         
         return false;

    }

    
    bool Mask::CompareIfBetweenInsertAtBeginning( double value_to_compare , double small_value_to_compare_with , double big_value_to_compare_with, unsigned int edge_number, IterativePolygonInTermsOfIntersections::WhichIntersection intersection , IntersectionsThisVoxel& voxel_intersections ){

           if( ( ( value_to_compare > small_value_to_compare_with ) && ( value_to_compare < big_value_to_compare_with ) ) || ( ( value_to_compare < small_value_to_compare_with ) && ( value_to_compare > big_value_to_compare_with ) )  ){
           
                std::vector< double > double_vec;
                double_vec.push_back(value_to_compare) ;
                std::vector< IterativePolygonInTermsOfIntersections::WhichIntersection > intersection_vec;
                intersection_vec.push_back( intersection ); 
                intersection_coord_iterator_type it = voxel_intersections.edge_intersections_intersection_coord.at(edge_number).begin();  
    
                voxel_intersections.edge_intersections_vox_coord.at(edge_number).insert( voxel_intersections.edge_intersections_vox_coord.at(edge_number).begin(), double_vec ); 
                voxel_intersections.edge_intersections_intersection_coord.at(edge_number).insert( it , intersection_vec );
                
                return true;
           
           }
           return false;
           
    }
    

    void Mask::SetThisIntersectionYIncreasing( unsigned int index_x , unsigned int index_y ,  IterativePolygonInTermsOfIntersections::WhichIntersection intersection , LegacyWorldCoordinate2D coord2D , IntersectionsThisVoxel& voxel_intersections ){
    
       if( coord2D.x != static_cast<double>(index_x+1)  ) assert(0); // this should never happen
       
       // to be implemented 
       
    
    }
    
    
    void Mask::SetThisIntersectionXDecreasing( unsigned int index_x , unsigned int index_y ,  IterativePolygonInTermsOfIntersections::WhichIntersection intersection , LegacyWorldCoordinate2D coord2D , IntersectionsThisVoxel& voxel_intersections ){
    
       if( coord2D.y != static_cast<double>(index_y+1)  ) assert(0); // this should never happen 
    
       // to be implemented
    
    }
    
    
    void Mask::SetThisIntersectionYDecreasing( unsigned int index_x , unsigned int index_y ,  IterativePolygonInTermsOfIntersections::WhichIntersection intersection , LegacyWorldCoordinate2D coord2D , IntersectionsThisVoxel& voxel_intersections ){
    
        if( coord2D.x != static_cast<double>(index_x)  ) assert(0); // this should never happen 
    
        // to be implemented
    
    }        


    void Mask::Init(){

      SliceStructureCorrespondency.clear();
      do_detailed_subvox = true;
	  if( do_detailed_subvox ){
		  FieldOfIntersections = new rttb::masks::legacy::FieldOfPointer<VoxelIntersectionsVectorPointer>( GInf.at(0).getNumColumns() , GInf.at(0).getNumRows() , 1 );
	  }
     
      for( int i = 0 ; i < MaskField.size() ; i++ ){
        if(MaskField.at(i)!=NULL)MaskField.at(i)->~FieldOfScalar();
      }
      MaskField.resize(0);
      rttb::masks::legacy::FieldOfScalar<field_content_type>* a_field_of_scalar = new rttb::masks::legacy::FieldOfScalar<field_content_type>( GInf.at(0).getNumColumns() , GInf.at(0).getNumRows() , GInf.at(0).getNumSlices() ); 
      MaskField.push_back( a_field_of_scalar );

      brightness_inside = 50;
      brightness_border = 10;
      brightness_outside = 0;    
      brightness_unknown = -10;

      LegacyDoseVoxelIndex3D doseVoxelIndex;
      doseVoxelIndex.x = 0;
      doseVoxelIndex.y = 0;
      doseVoxelIndex.z = 0 ;

      int currentSliceNumber=0;

      LegacyPolygonSequenceType strVector = _structure->getLegacyStructureVector();
      LegacyPolygonSequenceType& strVector_ref = strVector;
      StructureInTermsOfIntersections inters;
      inters.resize( strVector.size() );
      Intersections.push_back( inters );


      StructureInTermsOfIntersections sitoi=Intersections.at(0);
      //StructureInTermsOfIntersections::

      for(unsigned int i = 0 ; i < strVector.size() ; i++ ){

        Intersections.at(0).at( i ).resize( strVector.at( i ).size() );

        LegacyWorldCoordinate2D index_coord;
        index_coord.x = 0;
        index_coord.y = 0;

        LegacyWorldCoordinate3D contour_point_world;
        contour_point_world.x = 0;
        contour_point_world.y = 0;
        contour_point_world.z = 0;
        
        for(unsigned int j = 0 ; j < strVector.at( i ).size() ; j++ ){

          contour_point_world = strVector.at( i ).at( j );
          index_coord   = GetDoubleIndex2D( contour_point_world.x  , contour_point_world.y  );
          Intersections.at(0).at( i ).at( j ).contour_point_voxel_coord.x = index_coord.x; 
          Intersections.at(0).at( i ).at( j ).contour_point_voxel_coord.y = index_coord.y;
          Intersections.at(0).at( i ).at( j ).contour_point_voxel_coord.z = GetZIndex(contour_point_world.z);      


        }  

      } 


      // iteration slices
      for( doseVoxelIndex.z = 0 ; doseVoxelIndex.z < GInf.at(0).getNumSlices() ; doseVoxelIndex.z++ ){

        correspoinding_structure_vector str_v;         

        LegacyWorldCoordinate2D the_index;
        the_index.x = 0;
        the_index.y = 0;   

        DoseVoxel doseVoxel; 
        //from dose index to worldcoordinate
        doseVoxel.setDoseVoxel( doseVoxelIndex , &(GInf.at(0)) );

        str_v=doseVoxel.getZIntersectPolygonIndexVector(*_structure ); //Lanlan


        SliceStructureCorrespondency.push_back( str_v );  


      }

      
      InitInterestingArea();  
      InitMultiResolution(); 

    }





    void Mask::InitInterestingArea(){

      MarkInterestingArea.clear();

      std::vector<LegacyArea2D> interesting_area_this_resolution;

      LegacyDoseVoxelIndex3D doseVoxelIndex;
      doseVoxelIndex.x = 0;
      doseVoxelIndex.y = 0;
      doseVoxelIndex.z = 0 ;

      int currentSliceNumber=0;

      LegacyPolygonSequenceType strVector = _structure->getLegacyStructureVector();
      LegacyPolygonSequenceType& strVector_ref = strVector;

      // iteration slices
      for( doseVoxelIndex.z = 0 ; doseVoxelIndex.z < GInf.at(0).getNumSlices() ; doseVoxelIndex.z++ ){   

        LegacyWorldCoordinate1D x_min = 1000000000;
        LegacyWorldCoordinate1D x_max = -1000000000;
        LegacyWorldCoordinate1D y_min = 1000000000;
        LegacyWorldCoordinate1D y_max = -1000000000;

        LegacyWorldCoordinate1D x_index_min = 1000000000;
        LegacyWorldCoordinate1D x_index_max = -1000000000;
        LegacyWorldCoordinate1D y_index_min = 1000000000;
        LegacyWorldCoordinate1D y_index_max = -1000000000;      

        LegacyWorldCoordinate2D the_index;
        the_index.x = 0;
        the_index.y = 0;   

        DoseVoxel doseVoxel; 
        //from dose index to worldcoordinate
        doseVoxel.setDoseVoxel( doseVoxelIndex ,&( GInf.at(0)) );
        std::vector<int> sliceV = doseVoxel.getZIntersectPolygonIndexVector( *_structure ); 



        //if( ( strVector.size() != currentSliceNumber ) && ( currentSliceNumber !=  (-1) ) ){  
        for(int indexV=0; indexV<sliceV.size();indexV++){
          currentSliceNumber=sliceV.at(indexV);

          // iteration polygon

          for( int i = 0 ; i <  strVector.at(currentSliceNumber).size() ; i++){

            LegacyWorldCoordinate3D firstPoint = strVector.at(currentSliceNumber).at(i);
            //LegacyWorldCoordinate3D secondPoint = strVector.at(currentSliceNumber).at(i+1);

            if( firstPoint.x < x_min ){
              x_min = firstPoint.x;
            }   
            if( firstPoint.y < y_min ){
              y_min = firstPoint.y;
            }
            if( firstPoint.x > x_max ){
              x_max = firstPoint.x;
            }   
            if( firstPoint.y > y_max ){
              y_max = firstPoint.y;
            }              

            the_index = GetDoubleIndex2D( firstPoint.x , firstPoint.y );

            if( the_index.x < x_index_min ){
              x_index_min = the_index.x;
            }   
            if( the_index.y < y_index_min ){
              y_index_min = the_index.y;
            }
            if( the_index.x > x_index_max ){
              x_index_max = the_index.x;
            }   
            if( the_index.y > y_index_max ){
              y_index_max = the_index.y;
            }           

          }
        }//end for indexV


        if(sliceV.size()>0){
          LegacyArea2D area_2d;
          area_2d.x_begin = x_min;
          area_2d.x_end = x_max;
          area_2d.y_begin = y_min;
          area_2d.y_end = y_max;	 

          int small_enough_x =   static_cast<int>(floor( x_index_min ) - 1);	
          if( small_enough_x < 0 ) small_enough_x = 0;			     
          area_2d.index_begin.x = small_enough_x;

          int small_enough_y =  static_cast<int>(floor( y_index_min ) - 1);	
          if( small_enough_y < 0 ) small_enough_y = 0;			     
          area_2d.index_begin.y = small_enough_y;

          int large_enough_x =  static_cast<int>(floor( x_index_max ) + 1);	
          if( large_enough_x >= GInf.at(0).getNumColumns() ) large_enough_x = ( GInf.at(0).getNumColumns() - 1 );			     
          area_2d.index_end.x = large_enough_x;

          int large_enough_y =  static_cast<int>(floor( y_index_max ) + 1);	
          if( large_enough_y >= GInf.at(0).getNumRows() ) large_enough_y = ( GInf.at(0).getNumRows() - 1 );  
          area_2d.index_end.y = large_enough_y;	               
          interesting_area_this_resolution.push_back( area_2d ); // std::vector<LegacyArea2D> 
        }
        else{

          LegacyArea2D area_2d;
          area_2d.Init();		
          interesting_area_this_resolution.push_back( area_2d );  

        }

      }

      MarkInterestingArea.push_back( interesting_area_this_resolution );

    }


    void Mask::InitMultiResolution(){
    
      bool done = false;
      
      
      while( done == false ){      
      
      int new_dim_x = MaskField.at( 0 )->GetDimX() / static_cast<int>( GetBlockLengthThisResolutionLevel( MaskField.size() ) ) + 1;
      int new_dim_y = MaskField.at( 0 )->GetDimY() / static_cast<int>( GetBlockLengthThisResolutionLevel( MaskField.size() ) ) + 1;      
      


         if( new_dim_x > 20 && new_dim_y > 20 ){
 
                                        
            core::GeometricInfo NewGeomInf;                     
                                 
            NewGeomInf.setImagePositionPatient(GInf.at( GInf.size() - 1 ).getImagePositionPatient());

            NewGeomInf.setOrientationMatrix(GInf.at( GInf.size() - 1 ).getOrientationMatrix());

            NewGeomInf.setSliceThickness(GInf.at( GInf.size() - 1 ).getSliceThickness());
            NewGeomInf.setNumSlices(GInf.at( GInf.size() - 1 ).getNumSlices());            
                   
            NewGeomInf.setNumColumns(new_dim_x); 
            NewGeomInf.setNumRows(new_dim_y);                 
                   
            double power = 1;
            for( int i = 0 ; i < GInf.size() ; i++ ){
               power *= 2;                 
            }                    
            
            NewGeomInf.setPixelSpacingRow(GInf.at( 0 ).getPixelSpacingRow() * power);
            NewGeomInf.setPixelSpacingColumn(GInf.at( 0 ).getPixelSpacingColumn() * power);            
	           	      
            GInf.push_back( NewGeomInf );     	           	      
	           	           
            rttb::masks::legacy::FieldOfScalar<field_content_type>* a_field_of_scalar = new rttb::masks::legacy::FieldOfScalar<field_content_type>( GInf.at( GInf.size() - 1 ).getNumColumns() , GInf.at( GInf.size() - 1  ).getNumRows() , GInf.at( GInf.size() - 1  ).getNumSlices() ); 
            MaskField.push_back( a_field_of_scalar );           
         
            std::vector<LegacyArea2D> interesting_area_this_resolution;

      
            for( int z = 0 ; z < MarkInterestingArea.at( 0 ).size() ; z++  ){      
         
                LegacyArea2D NewArea;
                NewArea.x_begin = MarkInterestingArea.at( 0 ).at( z ).x_begin;
                NewArea.x_end = MarkInterestingArea.at( 0 ).at( z ).x_end;
                NewArea.y_begin = MarkInterestingArea.at( 0 ).at( z ).y_begin;
                NewArea.y_end = MarkInterestingArea.at( 0 ).at( z ).y_end;

                NewArea.index_begin.x = ( MarkInterestingArea.at( 0 ).at( z ).index_begin.x / power ); 
                if( NewArea.index_begin.x > 0 ) NewArea.index_begin.x -= 1;
                NewArea.index_begin.y = ( MarkInterestingArea.at( 0 ).at( z ).index_begin.y / power );
                if( NewArea.index_begin.y > 0 ) NewArea.index_begin.y -= 1;
                NewArea.index_end.x = ( MarkInterestingArea.at( 0 ).at( z ).index_end.x / power );
                NewArea.index_end.x += 1;
                if( NewArea.index_end.x >= MaskField.at( MaskField.size() - 1 )->GetDimX()   ) NewArea.index_end.x = ( MaskField.at( MaskField.size() - 1 )->GetDimX() - 1 );  
                NewArea.index_end.y = ( MarkInterestingArea.at( 0 ).at( z ).index_end.y / power );   
                NewArea.index_end.y += 1;         
	               if( NewArea.index_end.y >= MaskField.at( MaskField.size() - 1 )->GetDimY()  ) NewArea.index_end.y = ( MaskField.at( MaskField.size() - 1 )->GetDimY() - 1 );         
         
                interesting_area_this_resolution.push_back( NewArea );
         
            }
            
            MarkInterestingArea.push_back( interesting_area_this_resolution );        
         
         }
         else done = true;
      
      }
      
      /*
      std::cout<< "  MaskField.size() " << MaskField.size() <<std::endl;
      std::cout<< "  MarkInterestingArea.size() " << MarkInterestingArea.size() <<std::endl;
      std::cout<< "  GInf.size() " << GInf.size() <<std::endl;
    
      for( int i = 0 ; i < MaskField.size() ; i++ ){
      
        std::cout<< "  i : " << i <<std::endl;
        std::cout<< " MaskField.at(  i )->GetDimX()  : " << MaskField.at(  i )->GetDimX() <<std::endl;
        std::cout<< " MaskField.at(  i )->GetDimY()  : " << MaskField.at(  i )->GetDimY() <<std::endl;
        
        std::cout<< " GInf.at( i ).getPixelSpacingRow()  : " << GInf.at( i ).getPixelSpacingRow() <<std::endl;
        std::cout<< " GInf.at( i )->pixelSpacingColumn  : " << GInf.at( i ).getPixelSpacingColumn() <<std::endl;
        std::cout<< " GInf.at( i )->sliceThickness  : " << GInf.at( i ).getSliceThickness() <<std::endl;
        std::cout<< " GInf.at( i )->column  : " << GInf.at( i )getNumColumns() <<std::endl;
        std::cout<< " GInf.at( i )->row  : " << GInf.at( i )getNumRows() <<std::endl;
        std::cout<< " GInf.at( i )->numberOfFrames  : " << GInf.at( i ).getNumSlices() <<std::endl;
                
        for( int z = 0 ; z < MarkInterestingArea.at( 0 ).size() ; z++ ){        
          
          if(  ( MarkInterestingArea.at( i ).at( z ).index_begin.x ) || ( MarkInterestingArea.at( i ).at( z ).index_begin.y ) || ( MarkInterestingArea.at( i ).at( z ).index_end.x > 1 ) || ( MarkInterestingArea.at( i ).at( z ).index_end.y > 1 )  ){
            
          std::cout<< " z : " << z <<std::endl;      
          std::cout<< " MarkInterestingArea.at( i ).at( z ).index_begin.x  : " << MarkInterestingArea.at( i ).at( z ).index_begin.x  <<std::endl;
          std::cout<< " MarkInterestingArea.at( i ).at( z ).index_begin.y  : " << MarkInterestingArea.at( i ).at( z ).index_begin.y  <<std::endl;
          std::cout<< " MarkInterestingArea.at( i ).at( z ).index_end.x  : " << MarkInterestingArea.at( i ).at( z ).index_end.x  <<std::endl;
          std::cout<< " MarkInterestingArea.at( i ).at( z ).index_end.y  : " << MarkInterestingArea.at( i ).at( z ).index_end.y  <<std::endl;

          }

        }

      
      }*/

     
      InitIntersections();
     
    }
    
    
    
    void Mask::InitIntersections(){
    
      for( int counter = 1 ; counter < GInf.size() ; counter++ ){
    
        LegacyPolygonSequenceType strVector = _structure->getLegacyStructureVector();
        LegacyPolygonSequenceType& strVector_ref = strVector;
        StructureInTermsOfIntersections inters;
        inters.resize( strVector.size() );
        Intersections.push_back( inters );

        for(unsigned int i = 0 ; i < strVector.size() ; i++ ){

          Intersections.at(counter).at( i ).resize( strVector.at( i ).size() );

          LegacyWorldCoordinate2D index_coord;
          index_coord.x = 0;
          index_coord.y = 0;

          LegacyWorldCoordinate3D contour_point_world;
          contour_point_world.x = 0;
          contour_point_world.y = 0;
          contour_point_world.z = 0;
        
          for(unsigned int j = 0 ; j < strVector.at( i ).size() ; j++ ){

            contour_point_world = strVector.at( i ).at( j );
            index_coord   = GetDoubleIndex2D( contour_point_world.x  , contour_point_world.y , counter  );
            Intersections.at(counter).at( i ).at( j ).contour_point_voxel_coord.x = index_coord.x; 
            Intersections.at(counter).at( i ).at( j ).contour_point_voxel_coord.y = index_coord.y;
            Intersections.at(counter).at( i ).at( j ).contour_point_voxel_coord.z = GetZIndex(contour_point_world.z);      


          }  

        }
      
      }     
      
      if( Intersections.size() != GInf.size() ) assert(0);
      
      /*
      for( int counter = 0 ; counter < GInf.size() ; counter++ ){

        std::cout<<  " next resolution level "  <<  counter  <<std::endl;
      
        for( int i = 0 ; i < Intersections.at(counter).size() ; i++ ){
        
          std::cout<<  " next structure index : "  <<  i  <<std::endl;
      
          for( int j = 0 ; j < Intersections.at(counter).at( i ).size() ; j++ ){

            std::cout<<  " coordinates of the point : "  <<  j  <<std::endl;


            std::cout<< "  x : "  <<  Intersections.at(counter).at( i ).at( j ).contour_point_voxel_coord.x <<std::endl; 
            std::cout<< "  y : "  <<  Intersections.at(counter).at( i ).at( j ).contour_point_voxel_coord.y <<std::endl; 
            std::cout<< "  z : "  <<  Intersections.at(counter).at( i ).at( j ).contour_point_voxel_coord.z <<std::endl;        


      
          }
      
        }
        
      }
      */  
      
    }


    void Mask::GetIntersections( int resolution_level ){

      LegacyPolygonSequenceType strVector = _structure->getLegacyStructureVector();
      LegacyPolygonSequenceType& strVector_ref = strVector;

      LegacyWorldCoordinate3D firstPoint;
      firstPoint.x = 0;
      firstPoint.y = 0;
      firstPoint.z = 0;
      LegacyWorldCoordinate3D secondPoint;
      secondPoint.x = 0;
      secondPoint.y = 0;
      secondPoint.z = 0;

           
      // iteration z direction
      for(unsigned int struct_index = 0 ; struct_index < strVector.size() ; struct_index++ ){               


        // iteration polygon
        for( int point_index = 0 ; point_index < ( strVector.at(struct_index).size() - 1 ); point_index++ ){


          firstPoint = strVector.at(struct_index).at( point_index );
          secondPoint = strVector.at(struct_index).at( point_index + 1 );


          std::vector<LegacyWorldCoordinate2D>& raw_intersections_ref = Intersections.at(resolution_level).at( struct_index ).at( point_index ).raw_intersections;
          std::vector<LegacyWorldCoordinate2D>& column_intersections_ref = Intersections.at(resolution_level).at( struct_index ).at( point_index ).column_intersections;

          std::vector<LegacyWorldCoordinate2D>& intersections_raw_and_column_ref = Intersections.at(resolution_level).at( struct_index ).at( point_index ).intersections_raw_and_column;


          GoIntersectRaw( firstPoint , secondPoint , raw_intersections_ref, resolution_level );
          GoIntersectColumn( firstPoint , secondPoint , column_intersections_ref, resolution_level  ); 
          
          UnifyRawAndColumn( firstPoint , raw_intersections_ref , column_intersections_ref , intersections_raw_and_column_ref, resolution_level );         


        }


        // the connection between the last and the first Point of the Polygon needs to be checked as well

        if( strVector.at(struct_index).size() > 0 ){

          firstPoint = strVector.at( struct_index ).at(  ( strVector.at(struct_index).size() - 1 )  );
          secondPoint = strVector.at( struct_index ).at( 0 );

          std::vector<LegacyWorldCoordinate2D>& raw_intersections_ref = Intersections.at(resolution_level).at( struct_index ).at(   ( strVector.at(struct_index).size() - 1 )    ).raw_intersections;
          std::vector<LegacyWorldCoordinate2D>& column_intersections_ref = Intersections.at(resolution_level).at( struct_index ).at(     ( strVector.at(struct_index).size() - 1 )      ).column_intersections;

          std::vector<LegacyWorldCoordinate2D>& intersections_raw_and_column_ref = Intersections.at(resolution_level).at( struct_index ).at(   ( strVector.at(struct_index).size() - 1 )    ).intersections_raw_and_column;

          GoIntersectRaw( firstPoint , secondPoint , raw_intersections_ref, resolution_level );
          GoIntersectColumn( firstPoint , secondPoint , column_intersections_ref, resolution_level );

          UnifyRawAndColumn( firstPoint, raw_intersections_ref , column_intersections_ref, intersections_raw_and_column_ref, resolution_level );  

        }      


      } 

      /*for( int struct_index = 0 ; struct_index < strVector.size() ; struct_index++ ){               

      // iteration polygon

      for( int point_index = 0 ; point_index < ( strVector.at(struct_index).size() - 1 ); point_index++ ){


      std::vector<LegacyWorldCoordinate2D>& raw_intersections_ref = Intersections.at(0).at(0).at( struct_index ).at( point_index ).raw_intersections;

      std::vector<LegacyWorldCoordinate2D>& column_intersections_ref = Intersections.at(0).at( struct_index ).at( point_index ).column_intersections;


      for(int i=0;i<raw_intersections_ref.size();i++)
      std::cout << raw_intersections_ref.at(i).toString()<<"---";

      for(int i=0;i<column_intersections_ref.size();i++)
      std::cout << column_intersections_ref.at(i).toString()<<"---";
      std::cout << std::endl;
      }
      }*/

    }              


    void Mask::UnifyRawAndColumn( LegacyWorldCoordinate3D the_firstPoint, std::vector<LegacyWorldCoordinate2D>& raw_intersections_ref , std::vector<LegacyWorldCoordinate2D>& column_intersections_ref ,  std::vector<LegacyWorldCoordinate2D>& intersections_raw_and_column_ref, int resolution_level ){
      
      if( ( column_intersections_ref.size() > 0 ) && ( raw_intersections_ref.size() == 0 ) ){
            intersections_raw_and_column_ref =  column_intersections_ref;
      } 
      
      if( ( raw_intersections_ref.size() > 0 ) && ( column_intersections_ref.size() == 0 ) ){
           intersections_raw_and_column_ref =  raw_intersections_ref;
      }
        
      
      if( ( intersections_raw_and_column_ref.size() == 0 ) ){ // falls das nicht der Fall ist ist der Verlauf der Konturpunkte horizontal oder vertikal. Dann gibt es sowiso nur horizontale oder vertikale Schnittpunkte und die Vereinigung ist in GoIntersect... bereits geschehen.
 
        bool done = false;

        int internal_counter = 0;
        int counter = 0 ; 

        LegacyWorldCoordinate2D rawPoint;
        rawPoint.x = 0;
        rawPoint.y = 0;
        
        LegacyWorldCoordinate2D columnPoint;
        columnPoint.x = 0;
        columnPoint.y = 0; 
 
 
        LegacyWorldCoordinate2D firstPoint = GetDoubleIndex2D( the_firstPoint.x , the_firstPoint.y, resolution_level );
 
 
        while( ! done ){
      

                  if( counter < raw_intersections_ref.size() )rawPoint = raw_intersections_ref.at( counter );
                  else if( raw_intersections_ref.size() > 0 ) rawPoint = raw_intersections_ref.at( raw_intersections_ref.size() - 1 );
                  if( internal_counter < column_intersections_ref.size() )columnPoint = column_intersections_ref.at( internal_counter );
                  else if( column_intersections_ref.size() > 0 ) columnPoint = column_intersections_ref.at( column_intersections_ref.size() - 1 );
                
                  double distance_raw = ( firstPoint.y - rawPoint.y ) * ( firstPoint.y - rawPoint.y ) + ( firstPoint.x - rawPoint.x ) * ( firstPoint.x - rawPoint.x );
                  double distance_column = ( firstPoint.x - columnPoint.x ) * ( firstPoint.x - columnPoint.x ) +  ( firstPoint.y - columnPoint.y ) * ( firstPoint.y - columnPoint.y );
                
                      
                
                
                  if( distance_raw == distance_column ){ 
                     if( internal_counter >= column_intersections_ref.size() ){
                       counter++;
                     }
                     else{
                      intersections_raw_and_column_ref.push_back( columnPoint );
                      internal_counter++; 
                      counter++;           
                     }
                  }
                  else if( ( ( distance_raw > distance_column ) &&  ( internal_counter < column_intersections_ref.size() )   ) || ( counter >= ( raw_intersections_ref.size() ) ) ){
                      intersections_raw_and_column_ref.push_back( columnPoint );
                      internal_counter++; 
                  }  
                  else if(  ( distance_raw < distance_column )  ||  ( internal_counter >= column_intersections_ref.size()  )  ){
                    if( ( counter < ( raw_intersections_ref.size() ) )   ){ 
                      intersections_raw_and_column_ref.push_back( rawPoint );
                      counter++;
                    } 
                  }
                  
                  if(  ( internal_counter >= column_intersections_ref.size() )  &&  (  counter >= raw_intersections_ref.size()  )  )done = true;
 
 
        }
        
        
    /*    for( int i = 0 ; i < column_intersections_ref.size() ; i++ ){
        
          LegacyWorldCoordinate2D Point = column_intersections_ref.at( i );
          std::cout<<  " column_intersections_ref.at( i ) Point.x !!!!!!!!!!!!!!!!!!!!!!!!!!!!: "  << Point.x  <<std::endl;
          std::cout<<  " column_intersections_ref.at( i ) Point.y !!!!!!!!!!!!!!!!!!!!!!!!!!!!!: "  << Point.y  <<std::endl;
        
        }
        

        for( int i = 0 ; i < raw_intersections_ref.size() ; i++ ){
        
          LegacyWorldCoordinate2D Point = raw_intersections_ref.at( i );
          std::cout<<  " raw_intersections_ref.at( i ) Point.x !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1111 : "  << Point.x  <<std::endl;
          std::cout<<  " raw_intersections_ref.at( i ) Point.y !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1 : "  << Point.y  <<std::endl;
        
        }




        for( int i = 0 ; i < intersections_raw_and_column_ref.size() ; i++ ){
        
          LegacyWorldCoordinate2D Point = intersections_raw_and_column_ref.at( i );
          std::cout<<  " intersections_raw_and_column_ref.at( i ) Point.x !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! : "  << Point.x  <<std::endl;
          std::cout<<  " intersections_raw_and_column_ref.at( i ) Point.y !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1 : "  << Point.y  <<std::endl;
        
        }   */





 
      }
      
    }


    void Mask::GoIntersectRaw( LegacyWorldCoordinate3D firstPoint , LegacyWorldCoordinate3D secondPoint , std::vector<LegacyWorldCoordinate2D>& raw_intersections_ref, int resolution_level ){


      if( (  secondPoint.y - firstPoint.y  ) != 0 ){
        if( (  secondPoint.x - firstPoint.x  ) != 0 ){


          /*           
          Siehe GoIntersectColumn (ist eigentlich das selbe)
          Auf der einen Gerade liegt das Strutkur Segment und die andere Gerade ist die Spalte 
          Beziehungsweise die Zeile. 
          Zwei Geleichungen und zwei Unbekannte ....
          */


          /*

          NEBENRECHNUNG :

          linear connection between both points :

          x = firstPoint.x + (  secondPoint.x - firstPoint.x  ) * lambda 
          y = firstPoint.y + (  secondPoint.y - firstPoint.y  ) * lambda  

          with lambda between zero and one representing the regarded structure segment

          equation raw :

          x = imagePositionPatient.x + x_offset_raw * (raw_index-0.5) +  imageOrientationRow.x * pixelSpacingRow * (the_index.x-0.5)
          y = imagePositionPatient.y + y_offset_raw * (raw_index-0.5) +  imageOrientationRow.y * pixelSpacingRow * (the_index.x-0.5)

          x_offset_raw gibt die rauemliche Verschiebung beim aendern des zeilen index an 
          raw_index repraesentiert den index der betrachteten Zeile also die y-Position innerhalb der Spalte

          x_offset_raw = imageOrientationColumn.x*pixelSpacingColumn;
          y_offset_raw = imageOrientationColumn.y*pixelSpacingColumn; 

          unknown : lambda and the_index.x 
          the_index.x  and  raw_index represent the voxel

          Solution :

          First Equation:  imagePositionPatient.x + x_offset_raw * (raw_index-0.5) +  imageOrientationRow.x * pixelSpacingRow * (the_index.x-0.5) =  firstPoint.x + (  secondPoint.x - firstPoint.x  ) * lambda 
          Second Equation: imagePositionPatient.y + y_offset_raw * (raw_index-0.5) +  imageOrientationRow.y * pixelSpacingRow * (the_index.x-0.5) =  firstPoint.y + (  secondPoint.y - firstPoint.y  ) * lambda


          Lambda from second equation:
          ( imagePositionPatient.y + y_offset_raw * (raw_index-0.5) +  imageOrientationRow.y * pixelSpacingRow * (the_index.x-0.5) - firstPoint.y  )  / (  secondPoint.y - firstPoint.y  ) =  lambda 


          First Equation:
          ( imagePositionPatient.x + x_offset_raw * (raw_index-0.5) +  imageOrientationRow.x * pixelSpacingRow * (the_index.x-0.5) - firstPoint.x  ) / (  secondPoint.x - firstPoint.x  ) = lambda  


          First into Second Equation :
          imagePositionPatient.y + y_offset_raw * (raw_index-0.5) - firstPoint.y  -  ( imagePositionPatient.x - firstPoint.x  + x_offset_raw * (raw_index-0.5) ) *  (  secondPoint.y - firstPoint.y  ) / (  secondPoint.x - firstPoint.x  ) =   -  imageOrientationRow.y * pixelSpacingRow * (the_index.x-0.5)  +  (  secondPoint.y - firstPoint.y  ) / (  secondPoint.x - firstPoint.x  )* (  imageOrientationRow.x * pixelSpacingRow * (the_index.x-0.5) )  


          First into Second Equation :
          imagePositionPatient.y + y_offset_raw * (raw_index-0.5) - firstPoint.y  -  ( imagePositionPatient.x - firstPoint.x  + x_offset_raw * (raw_index-0.5) ) *  (  secondPoint.y - firstPoint.y  ) / (  secondPoint.x - firstPoint.x  ) =  (the_index.x-0.5) * ( -  imageOrientationRow.y * pixelSpacingRow + (  secondPoint.y - firstPoint.y  ) / (  secondPoint.x - firstPoint.x  )* (  imageOrientationRow.x * pixelSpacingRow )   )                          
          First into SecondEquation :
          (the_index.x-0.5) = imagePositionPatient.y + y_offset_raw * (raw_index-0.5) - firstPoint.y  -  ( imagePositionPatient.x - firstPoint.x  + x_offset_raw * (raw_index-0.5) ) *  (  secondPoint.y - firstPoint.y  ) / (  secondPoint.x - firstPoint.x  )  / (    ( -  imageOrientationRow.y * pixelSpacingRow + (  secondPoint.y - firstPoint.y  ) / (  secondPoint.x - firstPoint.x  )* (  imageOrientationRow.x * pixelSpacingRow )   )     )

          Der andere index ist raw_index und daher bereits bekannt. 
          Vorsicht, bevor ein Schnittpunkt als solcher akzeptiert werden kann muss untersucht werden 
          ob lambda zwischen null und eins liegt
          da ansonsten Schnittpunkte der Segmentgeraden betrachtet werden, 
          die ausserhalb des eigentlichen Struktursegmentes liegen.

          */


          double x_offset_raw = GInf.at(resolution_level).getImageOrientationColumn()(0) * GInf.at(resolution_level).getPixelSpacingColumn();
          double y_offset_raw = GInf.at(resolution_level).getImageOrientationColumn()(1) * GInf.at(resolution_level).getPixelSpacingColumn();

          LegacyWorldCoordinate2D index_first_point;
          index_first_point.x = 0;
          index_first_point.y = 0;
          LegacyWorldCoordinate2D index_second_point;
          index_second_point.x = 0;
          index_second_point.y = 0;

          // indicees der beiden uebergebenen Punkte bestimmen
          index_first_point   = GetDoubleIndex2D( firstPoint.x  , firstPoint.y, resolution_level  );
          index_second_point  = GetDoubleIndex2D( secondPoint.x , secondPoint.y, resolution_level );

          double index_diff = ( index_second_point.y - index_first_point.y );



          int begin_ind = 0;
          int end_ind  = 0;
          bool do_swap = 0;

          if( index_diff > 0 ){

            begin_ind =  static_cast<int>( floor( index_first_point.y  ) );
            end_ind   =  static_cast<int>( floor( index_second_point.y  ) + 1 );

          }
          else if( index_diff < 0 ){

            do_swap = 1;
            begin_ind =  static_cast<int>( floor( index_second_point.y ) );
            end_ind   =  static_cast<int>( floor( index_first_point.y  ) + 1 );        

          }
          else{

            begin_ind =  0;
            end_ind   =  0;          

          }

          if( begin_ind < 0 ) begin_ind = 0;
          if( end_ind > ( GInf.at(resolution_level).getNumRows() - 1 ) ) end_ind = GInf.at(resolution_level).getNumRows() - 1;           

          LegacyWorldCoordinate2D coord2D;
          coord2D.x = 0;
          coord2D.y = 0;
          LegacyWorldCoordinate3D the_index;
          the_index.x = 0;
          the_index.y = 0;
          the_index.z = 0;

          double multi_res_offset = GetWorldCoordinateOffsetThisResolutionLevel( resolution_level );

          //std::cout <<"go intersection raw: "<<std::endl;
          for( int raw_index = begin_ind ; raw_index <= end_ind ; raw_index++  ){

            // alt
            //the_index.x = 0.5 + ( GInf.at(resolution_level).getImagePositionPatient()(1) + y_offset_raw * (raw_index-0.5) - firstPoint.y  -  ( GInf.at(resolution_level).getImagePositionPatient()(0) - firstPoint.x  + x_offset_raw * (raw_index-0.5) ) * ( (  secondPoint.y - firstPoint.y  ) / (  secondPoint.x - firstPoint.x  )  )  )        /               (    (  (-1)  *  GInf.at(resolution_level).getImageOrientationRow()(1) * GInf.at(resolution_level).getPixelSpacingRow() + (  secondPoint.y - firstPoint.y  ) / (  secondPoint.x - firstPoint.x  )* (  GInf.at(resolution_level).getImageOrientationRow()(0) * GInf.at(resolution_level).getPixelSpacingRow() )  )  );

            // neu multi resoltuion
            the_index.x = multi_res_offset + ( GInf.at(resolution_level).getImagePositionPatient()(1) + y_offset_raw * (raw_index-multi_res_offset) - firstPoint.y  -  ( GInf.at(resolution_level).getImagePositionPatient()(0) - firstPoint.x  + x_offset_raw * (raw_index-multi_res_offset) ) * ( (  secondPoint.y - firstPoint.y  ) / (  secondPoint.x - firstPoint.x  )  )  )        /               (    (  (-1)  *  GInf.at(resolution_level).getImageOrientationRow()(1) * GInf.at(resolution_level).getPixelSpacingRow() + (  secondPoint.y - firstPoint.y  ) / (  secondPoint.x - firstPoint.x  )* (  GInf.at(resolution_level).getImageOrientationRow()(0) * GInf.at(resolution_level).getPixelSpacingRow() )  )  );
            
            coord2D.x = the_index.x;
            coord2D.y = raw_index;
           
            // alt
            // double lambda =  ( GInf.at(resolution_level).getImagePositionPatient()(0) + x_offset_raw * (raw_index-0.5) +  GInf.at(resolution_level).getImageOrientationRow()(0) * GInf.at(resolution_level).getPixelSpacingRow() * (the_index.x-0.5) - firstPoint.x  ) / (  secondPoint.x - firstPoint.x  ) ;
           
            // neu multi resolution
            double lambda =  ( GInf.at(resolution_level).getImagePositionPatient()(0) + x_offset_raw * (raw_index-multi_res_offset) +  GInf.at(resolution_level).getImageOrientationRow()(0) * GInf.at(resolution_level).getPixelSpacingRow() * (the_index.x-multi_res_offset) - firstPoint.x  ) / (  secondPoint.x - firstPoint.x  ) ;
                      
           
            if( ( lambda >= 0 ) && ( lambda <= 1 ) ){ 
              raw_intersections_ref.push_back( coord2D );
              //std::cout << coord2D.toString()<<std::endl;
              //std::cout <<"("<<x_offset_raw<<","<<y_offset_raw<<")"<<std::endl;

              /*
              core::LegacyDoseVoxelIndex3D index={static_cast<int>(coord2D.x),static_cast<int>(coord2D.y),static_cast<int>(this->GetZIndex(firstPoint.z) )};
              DoseVoxel voxel=DoseVoxel(index,GInf.at(resolution_level));
              std::vector<LegacyWorldCoordinate3D> interPs=voxel.calcIntersectionPoints(firstPoint, secondPoint);
              this->voxel_intersectionPoints.insert(VoxelIntersectionPointsMap::value_type(index, interPs));!!!Lanlan */

              /*std::cout << "intersections: "<<std::endl;
              for(int i=0; i<interPs.size();i++)
              std::cout << interPs.at(i).toString()<<", ";
              std::cout << std::endl;*/


              /*std::vector<LegacyWorldCoordinate3D> vecW;
              LegacyWorldCoordinate3D p1={firstPoint.x, (firstPoint.y+(end_ind-raw_index)*GInf.at(resolution_level).getPixelSpacingRow()), firstPoint.z};
              LegacyWorldCoordinate3D p2={firstPoint.x, (firstPoint.y+(end_ind-raw_index+1)*GInf.at(resolution_level).getPixelSpacingRow()), firstPoint.z};
              vecW.push_back(p1);
              vecW.push_back(p2);
              LegacyDoseVoxelIndex3D dosevoxel={static_cast<int>(coord2D.x),static_cast<int>(coord2D.y),static_cast<int>( this->GetZIndex(firstPoint.z) )};
              this->voxel_intersectionPoints.insert(VoxelIntersectionPointsMap::value_type(dosevoxel, vecW));*/
            }


          }
          if( do_swap ){

            std::vector<LegacyWorldCoordinate2D> raw_intersections_to_be_swaped;

            for( int this_one = ( raw_intersections_ref.size() - 1 ) ; this_one >= 0 ; this_one-- ){

              raw_intersections_to_be_swaped.push_back( raw_intersections_ref.at( this_one ) );

            }

            raw_intersections_ref.swap( raw_intersections_to_be_swaped );


          }

        }
        else{
          GoGetRawIntersectionsAlongThisColumn( firstPoint , secondPoint ,  raw_intersections_ref , resolution_level );
        }

      }
    }




    void Mask::GoGetRawIntersectionsAlongThisColumn( LegacyWorldCoordinate3D firstPoint , LegacyWorldCoordinate3D secondPoint , std::vector<LegacyWorldCoordinate2D>& raw_intersections_ref, int resolution_level ){

      LegacyWorldCoordinate2D index_first_point;
      index_first_point.x = 0;
      index_first_point.y = 0;
      LegacyWorldCoordinate2D index_second_point;
      index_second_point.x = 0;
      index_second_point.y = 0;

      if( firstPoint.x != secondPoint.x ) assert(0);

      // indicees der beiden uebergebenen Punkte bestimmen
      index_first_point   = GetDoubleIndex2D( firstPoint.x  , firstPoint.y, resolution_level  );
      index_second_point  = GetDoubleIndex2D( secondPoint.x , secondPoint.y, resolution_level );

      double index_diff = ( index_second_point.y - index_first_point.y );

      int begin_ind = 0;
      int end_ind  = 0;
      bool do_swap = 0;

      if( index_diff > 0 ){

        begin_ind =  static_cast<int>( floor( index_first_point.y  ) );
        if( floor( index_first_point.y  ) < index_first_point.y ) begin_ind += 1;
        end_ind   =  static_cast<int>( floor( index_second_point.y  ) );

      }
      else if( index_diff < 0 ){

        do_swap = 1;
        begin_ind =  static_cast<int>( floor( index_second_point.y ) );
        if( floor( index_second_point.y  ) < index_second_point.y ) begin_ind += 1;
        end_ind   =  static_cast<int>( floor( index_first_point.y  ) );      

      }
      else{

        begin_ind =  0;
        end_ind   =  0;          

      }

      if( begin_ind < 0 ) begin_ind = 0;
      if( end_ind > ( GInf.at(resolution_level).getNumRows() - 1 ) ) end_ind = GInf.at(resolution_level).getNumRows() - 1;           

      LegacyWorldCoordinate2D coord2D;
      coord2D.x = 0;
      coord2D.y = 0;

      //std::cout <<"go intersection raw along column: "<<std::endl;
      for( int raw_index = begin_ind ; raw_index <= end_ind ; raw_index++  ){

        coord2D.x = index_first_point.x;
        coord2D.y = raw_index;
        raw_intersections_ref.push_back( coord2D );

        //std::cout << coord2D.toString()<<std::endl;
        //std::cout <<"("<<firstPoint.x<<","<<(firstPoint.y+(raw_index-begin_ind)*GInf.at(resolution_level).getPixelSpacingRow())<<")"<<std::endl;

        //std::vector<LegacyWorldCoordinate3D> vecW;
        //LegacyWorldCoordinate3D p1={firstPoint.x, (firstPoint.y+(raw_index-begin_ind)*GInf.at(resolution_level).getPixelSpacingRow()), firstPoint.z};
        //LegacyWorldCoordinate3D p2={firstPoint.x, (firstPoint.y+(raw_index-begin_ind+1)*GInf.at(resolution_level).getPixelSpacingRow()), firstPoint.z};
        //vecW.push_back(p1);
        //vecW.push_back(p2);
        //LegacyDoseVoxelIndex3D dosevoxel={static_cast<int>(coord2D.x),static_cast<int>(coord2D.y),static_cast<int>(this->GetZIndex(firstPoint.z))};
        //this->voxel_intersectionPoints.insert(VoxelIntersectionPointsMap::value_type(dosevoxel, vecW));

        //core::LegacyDoseVoxelIndex3D index={static_cast<int>(coord2D.x),static_cast<int>(coord2D.y),0};
        /*DoseVoxel voxel=DoseVoxel(dosevoxel,GInf.at(resolution_level));
        std::vector<LegacyWorldCoordinate3D> interPs=voxel.calcIntersectionPoints(firstPoint, secondPoint);
        this->voxel_intersectionPoints.insert(VoxelIntersectionPointsMap::value_type(dosevoxel, interPs));*/

        /*std::cout << "intersections: "<<std::endl;
        for(int i=0; i<interPs.size();i++)
        std::cout << interPs.at(i).toString()<<", ";
        std::cout << std::endl;*/
      }
      if( do_swap ){

        std::vector<LegacyWorldCoordinate2D> raw_intersections_to_be_swaped;

        for( int this_one = ( raw_intersections_ref.size() - 1 ) ; this_one >= 0 ; this_one-- ){

          raw_intersections_to_be_swaped.push_back( raw_intersections_ref.at( this_one ) );

        }

        raw_intersections_ref.swap( raw_intersections_to_be_swaped );


      }     
      

    }




    void Mask::GoIntersectColumn( LegacyWorldCoordinate3D firstPoint , LegacyWorldCoordinate3D secondPoint , std::vector<LegacyWorldCoordinate2D>& column_intersections_ref, int resolution_level ){

      if( (  secondPoint.x - firstPoint.x  ) != 0 ){

        if( (  secondPoint.y - firstPoint.y  ) != 0 ){

          /*         
          Siehe GoIntersectRaw (ist eigentlich das selbe)  
          Auf der einen Gerade liegt das Strutkur Segment und die andere Gerade ist die Spalte 
          Beziehungsweise die Zeile. 
          Zwei Geleichungen und zwei Unbekannte ....
          */

          /*

          NEBENRECHNUNG :

          linear connection between the two points :

          x = firstPoint.x + (  secondPoint.x - firstPoint.x  ) * lambda 
          y = firstPoint.y + (  secondPoint.y - firstPoint.y  ) * lambda  

          with lambda between zero and one representing the regarded structure segment

          equation column :

          x_offset_column gibt die rauemliche Verschiebung beim aendern des spalten index an 
          column_index repraesentiert den index der betrachteten Spalte also die x  Position innerhalb der Zeile

          x = imagePositionPatient.x + x_offset_column * column_index + imageOrientationColumn.x * pixelSpacingColumn * the_index.y 
          y = imagePositionPatient.y + y_offset_column * column_index + imageOrientationColumn.y * pixelSpacingColumn * the_index.y 

          x_offset_column = imageOrientationRow.x * pixelSpacingRow;
          y_offset_column = imageOrientationRow.y * pixelSpacingRow; 


          Erste Geleuchung :    x = firstPoint.x + (  secondPoint.x - firstPoint.x  ) * lambda =  imagePositionPatient.x + x_offset_column * column_index + imageOrientationColumn.x * pixelSpacingColumn * the_index.y 
          Zweite Geleuchung :   y = firstPoint.y + (  secondPoint.y - firstPoint.y  ) * lambda =  imagePositionPatient.y + y_offset_column * column_index + imageOrientationColumn.y * pixelSpacingColumn * the_index.y

          Erste Gleuchung aufloesen nach lambda :
          Erste Geleuchung :    lambda =  ( imagePositionPatient.x + x_offset_column * column_index + imageOrientationColumn.x * pixelSpacingColumn * the_index.y - firstPoint.x  )  / ( secondPoint.x - firstPoint.x  )
          Lambda in zweite Gleichung einsetzen :
          firstPoint.y + (  secondPoint.y - firstPoint.y  ) *  ( imagePositionPatient.x + x_offset_column * column_index  - firstPoint.x  )  / ( secondPoint.x - firstPoint.x  ) - imagePositionPatient.y + y_offset_column * column_index   =  imageOrientationColumn.y * pixelSpacingColumn * the_index.y  -   (  imageOrientationColumn.x * pixelSpacingColumn  )  * (  secondPoint.y - firstPoint.y  ) / ( secondPoint.x - firstPoint.x  )  * (  the_index.y ) =    the_index.y *   (  imageOrientationColumn.y * pixelSpacingColumn    -   (  imageOrientationColumn.x * pixelSpacingColumn  )  * (  secondPoint.y - firstPoint.y  ) / ( secondPoint.x - firstPoint.x  )    ) 
          Lambda in zweite Gleichung einsetzen :  
          the_index.y =   (  firstPoint.y + (  secondPoint.y - firstPoint.y  ) *  ( imagePositionPatient.x + x_offset_column * column_index  - firstPoint.x  )  / ( secondPoint.x - firstPoint.x  ) - imagePositionPatient.y + y_offset_column * column_index ) /     (  imageOrientationColumn.y * pixelSpacingColumn    -   (  imageOrientationColumn.x * pixelSpacingColumn  )  * (  secondPoint.y - firstPoint.y  ) / ( secondPoint.x - firstPoint.x  )    ) 

          */

          /*
          
          Nach Koordinatentransformation nochmal ueberlegt:
          

          Erste Geleuchung :    x = firstPoint.x + (  secondPoint.x - firstPoint.x  ) * lambda =  imagePositionPatient.x + x_offset_column * ( column_index - 0.5 ) + imageOrientationColumn.x * pixelSpacingColumn * ( the_index.y - 0.5 ) 
          Zweite Geleuchung :   y = firstPoint.y + (  secondPoint.y - firstPoint.y  ) * lambda =  imagePositionPatient.y + y_offset_column * ( column_index - 0.5 ) + imageOrientationColumn.y * pixelSpacingColumn * ( the_index.y - 0.5 )

          Erste Gleuchung aufloesen nach lambda :
          Erste Geleuchung :    lambda =  ( imagePositionPatient.x + x_offset_column * ( column_index - 0.5 ) + imageOrientationColumn.x * pixelSpacingColumn * ( the_index.y - 0.5 ) - firstPoint.x  )  / ( secondPoint.x - firstPoint.x  )
          Lambda in zweite Gleichung einsetzen :
          firstPoint.y + (  secondPoint.y - firstPoint.y  ) *  ( imagePositionPatient.x + x_offset_column * ( column_index - 0.5 )  - firstPoint.x  )  / ( secondPoint.x - firstPoint.x  ) - imagePositionPatient.y + y_offset_column * ( column_index - 0.5 )   =  imageOrientationColumn.y * pixelSpacingColumn * ( the_index.y - 0.5 )  -   (  imageOrientationColumn.x * pixelSpacingColumn  )  * (  secondPoint.y - firstPoint.y  ) / ( secondPoint.x - firstPoint.x  )  * (  the_index.y - 0.5 ) =    ( the_index.y - 0.5 ) *   (  imageOrientationColumn.y * pixelSpacingColumn    -   (  imageOrientationColumn.x * pixelSpacingColumn  )  * (  secondPoint.y - firstPoint.y  ) / ( secondPoint.x - firstPoint.x  )    ) 
          Lambda in zweite Gleichung einsetzen :  
          ( the_index.y - 0.5 ) =   (  firstPoint.y + (  secondPoint.y - firstPoint.y  ) *  ( imagePositionPatient.x + x_offset_column * ( column_index - 0.5 )  - firstPoint.x  )  / ( secondPoint.x - firstPoint.x  ) - imagePositionPatient.y + y_offset_column * ( column_index - 0.5 ) ) /     (  imageOrientationColumn.y * pixelSpacingColumn    -   (  imageOrientationColumn.x * pixelSpacingColumn  )  * (  secondPoint.y - firstPoint.y  ) / ( secondPoint.x - firstPoint.x  )    ) 

          
          
          */


          double x_offset_column = GInf.at(resolution_level).getImageOrientationRow()(0) * GInf.at(resolution_level).getPixelSpacingRow();
          double y_offset_column = GInf.at(resolution_level).getImageOrientationRow()(1) * GInf.at(resolution_level).getPixelSpacingRow();

          LegacyWorldCoordinate2D index_first_point;
          index_first_point.x = 0;
          index_first_point.y = 0;
          LegacyWorldCoordinate2D index_second_point;
          index_second_point.x = 0;
          index_second_point.y = 0;

          // indicees der beiden uebergebenen Punkte bestimmen
          index_first_point   = GetDoubleIndex2D( firstPoint.x  , firstPoint.y, resolution_level  );
          index_second_point  = GetDoubleIndex2D( secondPoint.x , secondPoint.y, resolution_level );

          double index_diff = ( index_second_point.x - index_first_point.x );

          int begin_ind = 0;
          int end_ind   = 0;
          bool do_swap = 0;

          if( index_diff > 0 ){

            begin_ind =  static_cast<int>( floor( index_first_point.x  ) );
            end_ind   =  static_cast<int>( floor( index_second_point.x ) + 1 );

          }
          else if( index_diff < 0 ){

            do_swap = 1;
            begin_ind =  static_cast<int>( floor( index_second_point.x  ) );
            end_ind   =  static_cast<int>( floor( index_first_point.x ) + 1 );      

          }
          else{

            begin_ind =  0;
            end_ind   =  0;              
          }

          if( begin_ind < 0 ) begin_ind = 0;
          if( end_ind > ( GInf.at(resolution_level).getNumColumns() - 1 ) ) end_ind = GInf.at(resolution_level).getNumColumns() - 1;           

          LegacyWorldCoordinate2D coord2D;
          coord2D.x = 0;
          coord2D.y = 0;
          LegacyWorldCoordinate3D the_index;
          the_index.x = 0;
          the_index.y = 0;
          the_index.z = 0;


          double multi_res_offset = GetWorldCoordinateOffsetThisResolutionLevel( resolution_level );

          for( int column_index = begin_ind ; column_index <= end_ind ; column_index++  ){

            //ganz alt 
            //the_index.y = (  firstPoint.y + (  secondPoint.y - firstPoint.y  ) *  ( GInf.at(resolution_level).getImagePositionPatient()(0) + x_offset_column * column_index  - firstPoint.x  )  / ( secondPoint.x - firstPoint.x  ) - GInf.at(resolution_level).getImagePositionPatient()(1) + y_offset_column * column_index ) / ( GInf.at(resolution_level).getImageOrientationColumn()(1) * GInf.at(resolution_level).getPixelSpacingColumn() - ( GInf.at(resolution_level).getImageOrientationColumn()(0) * GInf.at(resolution_level).getPixelSpacingColumn() ) * (  secondPoint.y - firstPoint.y  ) / ( secondPoint.x - firstPoint.x  )  ) ;
            //alt       
            //the_index.y = 0.5 + (  firstPoint.y + (  secondPoint.y - firstPoint.y  ) *  ( GInf.at(resolution_level).getImagePositionPatient()(0) + x_offset_column * ( column_index - 0.5 )  - firstPoint.x  )  / ( secondPoint.x - firstPoint.x  ) - GInf.at(resolution_level).getImagePositionPatient()(1) + y_offset_column * ( column_index - 0.5 ) ) / ( GInf.at(resolution_level).getImageOrientationColumn()(1) * GInf.at(resolution_level).getPixelSpacingColumn() - ( GInf.at(resolution_level).getImageOrientationColumn()(0) * GInf.at(resolution_level).getPixelSpacingColumn() ) * (  secondPoint.y - firstPoint.y  ) / ( secondPoint.x - firstPoint.x  )  ) ;             

            //neu multi res       
            the_index.y = multi_res_offset + (  firstPoint.y + (  secondPoint.y - firstPoint.y  ) *  ( GInf.at(resolution_level).getImagePositionPatient()(0) + x_offset_column * ( column_index - multi_res_offset )  - firstPoint.x  )  / ( secondPoint.x - firstPoint.x  ) - GInf.at(resolution_level).getImagePositionPatient()(1) + y_offset_column * ( column_index - multi_res_offset ) ) / ( GInf.at(resolution_level).getImageOrientationColumn()(1) * GInf.at(resolution_level).getPixelSpacingColumn() - ( GInf.at(resolution_level).getImageOrientationColumn()(0) * GInf.at(resolution_level).getPixelSpacingColumn() ) * (  secondPoint.y - firstPoint.y  ) / ( secondPoint.x - firstPoint.x  )  ) ;             
                        

            coord2D.y = the_index.y;
            coord2D.x = column_index;
      
            //alt
            //double lambda =  ( GInf.at(resolution_level).getImagePositionPatient()(0) + x_offset_column * ( column_index - 0.5 ) + GInf.at(resolution_level).getImageOrientationColumn()(0) * GInf.at(resolution_level).getPixelSpacingColumn() * ( the_index.y - 0.5 ) - firstPoint.x  )  / ( secondPoint.x - firstPoint.x  );

            //neu multi resolution 
            double lambda =  ( GInf.at(resolution_level).getImagePositionPatient()(0) + x_offset_column * ( column_index - multi_res_offset ) + GInf.at(resolution_level).getImageOrientationColumn()(0) * GInf.at(resolution_level).getPixelSpacingColumn() * ( the_index.y - multi_res_offset ) - firstPoint.x  )  / ( secondPoint.x - firstPoint.x  );
      
            if( ( lambda >= 0 ) && ( lambda <= 1 ) ){

              column_intersections_ref.push_back( coord2D );

            }



          }
          if( do_swap ){

            std::vector<LegacyWorldCoordinate2D> column_intersections_to_be_swaped;

            for( int this_one = ( column_intersections_ref.size() - 1 ) ; this_one >= 0 ; this_one-- ){

              column_intersections_to_be_swaped.push_back( column_intersections_ref.at( this_one ) );

            }

            column_intersections_ref.swap( column_intersections_to_be_swaped );


          }


        }
        else{
          GoGetColumnIntersectionsAlongThisRaw( firstPoint , secondPoint , column_intersections_ref, resolution_level );    
        }


      }


    }


    bool Mask::TestGoIntersectColumnByComparisonWithExpectedResult( LegacyWorldCoordinate3D firstPoint , LegacyWorldCoordinate3D secondPoint , std::vector<LegacyWorldCoordinate2D>& column_intersections_ref, std::vector<LegacyWorldCoordinate2D>& expected_intersections_ref ){

      column_intersections_ref.clear();

      int resolution_level = 0;
      GoIntersectColumn( firstPoint , secondPoint , column_intersections_ref, resolution_level );

      return IdenticalIntersectionsForTest( column_intersections_ref , expected_intersections_ref );

    }


    bool Mask::TestGoIntersectRawByComparisonWithExpectedResult( LegacyWorldCoordinate3D firstPoint , LegacyWorldCoordinate3D secondPoint , std::vector<LegacyWorldCoordinate2D>& raw_intersections_ref, std::vector<LegacyWorldCoordinate2D>& expected_intersections_ref ){

      raw_intersections_ref.clear();

      int resolution_level = 0;
      GoIntersectRaw( firstPoint , secondPoint , raw_intersections_ref, resolution_level );

      return IdenticalIntersectionsForTest( raw_intersections_ref , expected_intersections_ref );

    }




    bool Mask::IdenticalIntersectionsForTest( std::vector<LegacyWorldCoordinate2D>& calculated_intersections_ref , std::vector<LegacyWorldCoordinate2D>& expected_intersections_ref ){

      if( calculated_intersections_ref.size() != expected_intersections_ref.size() ){
        std::cout<< " Size Problem ! " <<std::endl;
        std::cout<< " calculated_intersections_ref.size()  :   "  << calculated_intersections_ref.size()  <<std::endl;
        std::cout<< " expected_intersections_ref.size()    :   "  << expected_intersections_ref.size()    <<std::endl;
        if( calculated_intersections_ref.size() < expected_intersections_ref.size() ){
          for( int counter = 0 ; counter < calculated_intersections_ref.size() ; counter++ ){
            std::cout<< " expected_intersections_ref.at( counter ).x : " << expected_intersections_ref.at( counter ).x <<std::endl;
            std::cout<< " calculated_intersections_ref.at( counter ).x : " << calculated_intersections_ref.at( counter ).x <<std::endl;
            std::cout<< " expected_intersections_ref.at( counter ).y : " << expected_intersections_ref.at( counter ).y <<std::endl;
            std::cout<< "  calculated_intersections_ref.at( counter ).y : " <<  calculated_intersections_ref.at( counter ).y <<std::endl;
          }
        }
        else{
          for( int counter = 0 ; counter < expected_intersections_ref.size() ; counter++ ){
            std::cout<< " expected_intersections_ref.at( counter ).x : " << expected_intersections_ref.at( counter ).x <<std::endl;
            std::cout<< " calculated_intersections_ref.at( counter ).x : " << calculated_intersections_ref.at( counter ).x <<std::endl;
            std::cout<< " expected_intersections_ref.at( counter ).y : " << expected_intersections_ref.at( counter ).y <<std::endl;
            std::cout<< "  calculated_intersections_ref.at( counter ).y : " <<  calculated_intersections_ref.at( counter ).y <<std::endl; 
          }
        }
        return false;
      }  

      for( int counter = 0 ; counter < expected_intersections_ref.size() ; counter++ ){

        double diffa = ( ( expected_intersections_ref.at( counter ).x - calculated_intersections_ref.at( counter ).x ) * ( expected_intersections_ref.at( counter ).x - calculated_intersections_ref.at( counter ).x  )  ); 
        double diffb = ( ( expected_intersections_ref.at( counter ).y - calculated_intersections_ref.at( counter ).y ) * ( expected_intersections_ref.at( counter ).y - calculated_intersections_ref.at( counter ).y  )  );

        diffa = sqrt( diffa );
        diffb = sqrt( diffb );

        if( (   diffa > 0.001  ) || (  diffb > 0.001  ) ) {

          std::cout<<  "  Unexpectd coordinate !  "  <<std::endl;
          return false;
        }

      }

      return true;

    }




    void Mask::GoGetColumnIntersectionsAlongThisRaw( LegacyWorldCoordinate3D firstPoint , LegacyWorldCoordinate3D secondPoint , std::vector<LegacyWorldCoordinate2D>& column_intersections_ref, int resolution_level  ){

      LegacyWorldCoordinate2D index_first_point;
      index_first_point.x = 0;
      index_first_point.y = 0;
      LegacyWorldCoordinate2D index_second_point;
      index_second_point.x = 0;
      index_second_point.y = 0;
      
      if( firstPoint.y != secondPoint.y ) assert(0);

      // indicees der beiden uebergebenen Punkte bestimmen
      index_first_point   = GetDoubleIndex2D( firstPoint.x  , firstPoint.y, resolution_level  );
      index_second_point  = GetDoubleIndex2D( secondPoint.x , secondPoint.y, resolution_level );

      double index_diff = ( index_second_point.x - index_first_point.x );

      int begin_ind = 0;
      int end_ind  = 0;
      bool do_swap = 0;

      if( index_diff > 0 ){

        begin_ind =  static_cast<int>( floor( index_first_point.x  ) );
        if( floor( index_first_point.x  ) < index_first_point.x ) begin_ind += 1;
        end_ind   =  static_cast<int>( floor( index_second_point.x  ) );
      }
      else if( index_diff < 0 ){

        do_swap = 1;
        begin_ind =  static_cast<int>( floor( index_second_point.x ) );
        if( floor( index_second_point.x  ) < index_second_point.x ) begin_ind += 1;
        end_ind   =  static_cast<int>( floor( index_first_point.x  ) );      

      }
      else{

        begin_ind =  0;
        end_ind   =  0;          

      }

      if( begin_ind < 0 ) begin_ind = 0;
      if( end_ind > ( GInf.at(resolution_level).getNumRows() - 1 ) ) end_ind = GInf.at(resolution_level).getNumRows() - 1;           

      LegacyWorldCoordinate2D coord2D;
      coord2D.x = 0;
      coord2D.y = 0;

      for( int column_index = begin_ind ; column_index <= end_ind ; column_index++  ){

        coord2D.x = column_index;
        coord2D.y = index_first_point.y;
        column_intersections_ref.push_back( coord2D );

      }
      if( do_swap ){

        std::vector<LegacyWorldCoordinate2D> column_intersections_to_be_swaped;

        for( int this_one = ( column_intersections_ref.size() - 1 ) ; this_one >= 0 ; this_one-- ){

          column_intersections_to_be_swaped.push_back( column_intersections_ref.at( this_one ) );

        }

        column_intersections_ref.swap( column_intersections_to_be_swaped );

      }
 

    }



    void Mask::ShowIntersections(){

      LegacyWorldCoordinate2D coord2D;
      coord2D.x = 0;
      coord2D.y = 0;
    
      for( unsigned int resolution_level = 0 ; resolution_level < Intersections.size() ; resolution_level++ ){
       
        std::cout<< "  resolution_level : " <<  resolution_level <<std::endl;
       
        for(unsigned int struct_index = 0 ; struct_index < Intersections.at(resolution_level).size() ; struct_index++ ){         
       
          std::cout<< "  struct_index : " << struct_index <<std::endl;
       
          for(unsigned int point_index = 0 ; point_index < ( Intersections.at(resolution_level).at( struct_index ).size() ); point_index++ ){

            std::cout<< " point_index : " << point_index <<std::endl;

            for(unsigned int intersect_index = 0 ; intersect_index <  Intersections.at(resolution_level).at(struct_index).at( point_index ).intersections_raw_and_column.size() ; intersect_index++ ){

              std::cout<< "  intersect_index : " << intersect_index <<std::endl; 
               
              coord2D = Intersections.at(resolution_level).at(struct_index).at( point_index ).intersections_raw_and_column.at( intersect_index );      


              std::cout<<  " x : " << coord2D.x  <<std::endl;



              std::cout<<  " y : " << coord2D.y  <<std::endl; 




            }
          
          }  
    
        }
      
      }

      //assert(0);
    
    }



    void Mask::MarkVoxelsTouchedByStructure( int resolution_level ){

      // In allen Slices die dem aktuellen Polygon zugeordnet sind muessen die zu den
      // Schnittpunkten der Contour gehoerigen Voxel gesetzt werden.
      // (siehe SliceStructureCorrespondency bzw. correspoinding_structure_vector unter correspoinding_structure )
      // Es muessen alle eintraege von SliceStructureCorrespondency durchsucht werden um festzustellen,
      // ob es sich um ein slice des dose cubes handelt, das diesem Strukturelement zugeordnet ist.
      // Dabei muss beachtet werden, dass eine Structurelement fuer mehrere verschiedene Slices 
      // das zugehoerige sein kann.  
      
      LegacyWorldCoordinate2D coord2D;
      coord2D.x = 0;
      coord2D.y = 0;

      //if(  resolution_level == 0 ) ShowIntersections();

      std::vector<int> wich_slice;

      for(unsigned int struct_index = 0 ; struct_index < Intersections.at(resolution_level).size() ; struct_index++ ){          

        correspoinding_structure_vector str_v;
        wich_slice.clear();

        // die entsprechenden slices des dose cubes finden, die zu dieser Struktur gehoeren und die Voxel des MaskField belegen
        for(unsigned int index_z = 0 ; index_z < GInf.at(resolution_level).getNumSlices() ; index_z++ ){

          str_v = SliceStructureCorrespondency.at( index_z );  
          for(unsigned int count = 0 ; count < str_v.size() ; count++ ){           
            if( str_v.at( count ) == struct_index )wich_slice.push_back( index_z );	
        
          }

        }

        // iteration polygon
        for(unsigned int point_index = 0 ; point_index < ( Intersections.at(resolution_level).at( struct_index ).size() ); point_index++ ){


          for(unsigned int intersect_index = 0 ; intersect_index <  Intersections.at(resolution_level).at(struct_index).at( point_index ).intersections_raw_and_column.size() ; intersect_index++ ){

           coord2D = Intersections.at(resolution_level).at(struct_index).at( point_index ).intersections_raw_and_column.at( intersect_index );

            
           if(  ( coord2D.x >= 0 ) && ( coord2D.y >= 0 ) && ( coord2D.x < MaskField.at( resolution_level )->GetDimX() ) && ( coord2D.y < MaskField.at( resolution_level )->GetDimY() )  ){
            
            unsigned int index_x = static_cast<unsigned int>( floor( coord2D.x ) );
            unsigned int index_y = static_cast<unsigned int>( floor( coord2D.y ) );

            for(unsigned int this_counter = 0 ; this_counter < wich_slice.size() ; this_counter++ ){ 

              unsigned int index_z = wich_slice.at( this_counter );
             
              MaskField.at( resolution_level )->PutData( index_x , index_y , index_z , brightness_border );
                   
              if( ( floor( coord2D.x ) == coord2D.x ) && ( index_x > 0 ) ){

               MaskField.at( resolution_level )->PutData( ( index_x - 1 ) , index_y , index_z , brightness_border );
                
              } 
              if( ( floor( coord2D.y ) == coord2D.y ) && ( index_y > 0 ) ){
 
               MaskField.at( resolution_level )->PutData( index_x  , ( index_y - 1 ) , index_z , brightness_border );      

              }              
              if( ( floor( coord2D.x ) == coord2D.x ) && ( floor( coord2D.y ) == coord2D.y ) && ( index_y > 0 ) && ( index_x > 0 ) ){

               MaskField.at( resolution_level )->PutData( ( index_x - 1 )  , ( index_y - 1 ) , index_z , brightness_border );  
              
              } 


            }
             

           }

          }

          for(unsigned int this_counter = 0 ; this_counter < wich_slice.size() ; this_counter++ ){ 

            unsigned int index_z = wich_slice.at( this_counter );

            // Das Siemens Planunssystem schreibt aus ungeklaertem Gruend Daten raus, deren Voxelkoordinaten ausserhalb des Bildes zu liegen kommen. Eigentlich macht das keinen Sinn. Wir wissen, dass unsere Toolbox Welt-Voxel-Koordinaten Umrechnung DICOM conform ist. 
            int intx = floor( Intersections.at(resolution_level).at(struct_index).at( point_index ).contour_point_voxel_coord.x  );
            int inty = floor( Intersections.at(resolution_level).at(struct_index).at( point_index ).contour_point_voxel_coord.y  );

            
            if(  ( intx >= 0 ) && ( inty >= 0 ) &&  ( intx < MaskField.at( resolution_level )->GetDimX() ) && ( inty < MaskField.at( resolution_level )->GetDimY() )  ){

              Uint16 uintx = static_cast<Uint16>(intx);
              Uint16 uinty = static_cast<Uint16>(inty);

              MaskField.at( resolution_level )->PutData( uintx   , uinty , index_z , brightness_border ); 

            }

          }

        } // point index

      }     
 
                                     
    }





    // like MarkVoxelsTouchedByStructure 
    void Mask::GetIntersectionInformationThisSlice( unsigned int index_z ){

      // In allen Slices die dem aktuellen Polygon zugeordnet sind muessen die zu den
      // Schnittpunkten der Contour gehoerigen Voxel gesetzt werden.
      // (siehe SliceStructureCorrespondency bzw. correspoinding_structure_vector unter correspoinding_structure )
      // Es muessen alle eintraege von SliceStructureCorrespondency durchsucht werden um festzustellen,
      // ob es sich um ein slice des dose cubes handelt, das diesem Strukturelement zugeordnet ist.
      // Dabei muss beachtet werden, dass eine Structurelement fuer mehrere verschiedene Slices 
      // das zugehoerige sein kann.  

      LegacyWorldCoordinate2D coord2D;
      coord2D.x = 0;
      coord2D.y = 0;

      correspoinding_structure_vector str_v;
      str_v = SliceStructureCorrespondency.at( index_z );  
         
      int resolution_level = 0;    
          
      for(unsigned int count = 0 ; count < str_v.size() ; count++ ){  

        unsigned int struct_index = str_v.at( count ); 

        // iteration polygon
        for(unsigned int point_index = 0 ; point_index < ( Intersections.at(resolution_level).at( struct_index ).size() ); point_index++ ){


          for(unsigned int intersect_index = 0 ; intersect_index <  Intersections.at(resolution_level).at(struct_index).at( point_index ).intersections_raw_and_column.size() ; intersect_index++ ){

            coord2D = Intersections.at(resolution_level).at(struct_index).at( point_index ).intersections_raw_and_column.at( intersect_index );


            if(  ( coord2D.x >= 0 ) && ( coord2D.y >= 0 ) && ( coord2D.x < MaskField.at( resolution_level )->GetDimX() ) && ( coord2D.y < MaskField.at( resolution_level )->GetDimY() )  ){

            
             unsigned int index_x = static_cast<unsigned int>( floor( coord2D.x ) );
             unsigned int index_y = static_cast<unsigned int>( floor( coord2D.y ) );

              
              //MaskField.at( resolution_level )->PutData( index_x , index_y , index_z , brightness_border );
                            
              if( FieldOfIntersections ){
                 
                if( floor( coord2D.y ) == coord2D.y ){
                  unsigned int voxel_side = 0;
                  AddToFieldOfIntersections( index_x , index_y , 0 , struct_index , point_index , intersect_index , coord2D, voxel_side );
                }
                else if( floor( coord2D.x ) == coord2D.x ){
                  unsigned int voxel_side = 3;
                  AddToFieldOfIntersections( index_x , index_y , 0 , struct_index , point_index , intersect_index , coord2D, voxel_side );                
                }
                
              }  
                              
              if( ( floor( coord2D.x ) == coord2D.x ) && ( index_x > 0 ) ){

               //MaskField.at( resolution_level )->PutData( ( index_x - 1 ) , index_y , index_z , brightness_border );
               
               if( FieldOfIntersections ){ // Ecke wird gegebenenfalls mit angehaengt
                   unsigned int voxel_side = 1;
                   
            
                   AddToFieldOfIntersections( ( index_x - 1 ) , index_y , 0 , struct_index , point_index , intersect_index , coord2D, voxel_side );                
               } 
                
              } 
              if( ( floor( coord2D.y ) == coord2D.y ) && ( index_y > 0 ) ){

               //MaskField.at( resolution_level )->PutData( index_x  , ( index_y - 1 ) , index_z , brightness_border );    

               if( FieldOfIntersections ){
               
                 if( floor( coord2D.x ) == coord2D.x ){ // Ecke wird gegebenenfalls nicht mit angehaengt, deshalb Fallunterscheidunn noetig, damit sie angehaengt wird ( Ecke wird nur an die Kante angehaengt an deren Anfang sie steht)
                   unsigned int voxel_side = 3;
                   AddToFieldOfIntersections( index_x , ( index_y - 1 ) , 0 , struct_index , point_index , intersect_index , coord2D, voxel_side );
                 }
                 else{
                   unsigned int voxel_side = 2;
                   AddToFieldOfIntersections( index_x , ( index_y - 1 ) , 0 , struct_index , point_index , intersect_index , coord2D, voxel_side );
                 }
               
               } 

              }              
              if( ( floor( coord2D.x ) == coord2D.x ) && ( floor( coord2D.y ) == coord2D.y ) && ( index_y > 0 ) && ( index_x > 0 ) ){


               //MaskField.at( resolution_level )->PutData( ( index_x - 1 )  , ( index_y - 1 ) , index_z , brightness_border );

               if( FieldOfIntersections ){
                   unsigned int voxel_side = 2;        

                   AddToFieldOfIntersections( ( index_x - 1 ) , ( index_y - 1 ) , 0 , struct_index , point_index , intersect_index , coord2D, voxel_side );
               }
              
              } 


           }

          }



        }

      }        

    }




    void Mask::AddToFieldOfIntersections( unsigned int index_x , unsigned int index_y , unsigned int index_z , unsigned int struct_index , unsigned int point_index , unsigned int intersect_index , LegacyWorldCoordinate2D coord2D, unsigned int voxel_side ){

      int which_one = GetRespectiveIntersections( index_x , index_y , 0 , struct_index );

      IterativePolygonInTermsOfIntersections::WhichIntersection which_intersection;
      which_intersection.point_index = point_index;
      which_intersection.intersection_index = intersect_index;
      which_intersection.column_raw_or_unified = 2;        
              
      bool go = IsItToBeRegarded( struct_index , point_index , intersect_index, index_x , index_y );

      if( go ){
      

        if( which_one != (-1)  ){
           
           SetThisIntersectionToCornerOrEdge( index_x , index_y , which_intersection , coord2D , voxel_side, FieldOfIntersections->GetData( index_x , index_y, index_z )->at(which_one)  );           
      
        }
        else{

           ExtendFieldOfIntersections( index_x , index_y , 0 , struct_index );

           int which_one = GetRespectiveIntersections( index_x , index_y , 0 , struct_index );

           if(  which_one == (-1)  )assert( 0 );
    
           
    
           SetThisIntersectionToCornerOrEdge( index_x , index_y , which_intersection ,  coord2D , voxel_side, FieldOfIntersections->GetData( index_x , index_y, index_z )->at(which_one) );



        }
      
      }


    }



    bool Mask::IsItToBeRegarded( unsigned int struct_index , unsigned int point_index , unsigned int intersect_index , int index_x , int index_y ){
    // Muss betrachtet werden, falls der Konturpunkt sich in x oder durch y vom Schnittpunkt der zusaetzlich durch den intersect_index gegeben ist unterscheidet. 
    // Falls Schnittpunkt mit der Kante, der zusaetzlich durch intersect_index gegeben ist und der Konturpunkt nicht in x oder y unterscheiden,
    // wird der vorherige Konturpunkt betrachtet. Und zwar der erste, der sich von dem intialen tatsaechlich unterscheidet.
    // Wenn dieser Punkt in einer Richtung z.B. x auf der selben Kante liegt, sich aber in y Richtung unterscheidet, oder andersherum, 
    // dann muss der Schnittpunkt betrachtet werden, obwohl er mit einem Konturpunkt zusammenfaellt, weil der vorhergehende Konturpunkt auf der gleichen Kante liegt (eventuell in anderem Voxel, aber auf der selben Kante)  


      // Es ist nicht noetig die Selbe Stelle der Kante zweimal zu betrachten. Diese Stelle hier wird vom vorhergehenden Konturpunkt ausgehend als Schnittpunkt erkannt werden und wird deshalb hier nicht beruecksichtigt
      if( ( Intersections.at(0).at(struct_index).at( point_index ).contour_point_voxel_coord.y != Intersections.at(0).at(struct_index).at( point_index ).intersections_raw_and_column.at( intersect_index ).y ) ||
          ( Intersections.at(0).at(struct_index).at( point_index ).intersections_raw_and_column.at( intersect_index ).x  != Intersections.at(0).at(struct_index).at( point_index ).contour_point_voxel_coord.x )  )return true;
      else{    


            unsigned int struct_index_other = struct_index;
            unsigned int point_index_other = point_index;
            unsigned int intersect_index_other = intersect_index;

            if( GetFirstDifferent( struct_index_other , point_index_other , Intersections.at(0).at(struct_index).at( point_index ).contour_point_voxel_coord ) ){
            
            
              
               if(  ( floor(Intersections.at(0).at(struct_index).at( point_index ).contour_point_voxel_coord.y / 1.0 )  != Intersections.at(0).at(struct_index).at( point_index ).contour_point_voxel_coord.y ) &&  ( floor(Intersections.at(0).at(struct_index).at( point_index ).contour_point_voxel_coord.x / 1.0 ) == Intersections.at(0).at(struct_index).at( point_index ).contour_point_voxel_coord.x )  ){
               
                   if( Intersections.at(0).at(struct_index_other).at( point_index_other ).contour_point_voxel_coord.x  == Intersections.at(0).at(struct_index).at( point_index ).contour_point_voxel_coord.x ){
                    return true;
                   } 
               
               }
               else if(  (  floor( Intersections.at(0).at(struct_index).at( point_index ).contour_point_voxel_coord.y / 1 ) ==  Intersections.at(0).at(struct_index).at( point_index ).contour_point_voxel_coord.y  ) && ( floor( Intersections.at(0).at(struct_index).at( point_index ).contour_point_voxel_coord.x / 1 ) != Intersections.at(0).at(struct_index).at( point_index ).contour_point_voxel_coord.x )  ){
               
                   if( Intersections.at(0).at(struct_index_other).at( point_index_other ).contour_point_voxel_coord.y == Intersections.at(0).at(struct_index).at( point_index ).contour_point_voxel_coord.y ){
                    return true;
                   }
      
               }
              
            
            }
            else assert( 0 );

      
      }
    
      return false;
    

    }


    bool Mask::GetFirstDifferent( unsigned int& struct_index , unsigned int& point_index , LegacyWorldCoordinate3D compare_point ){
            
      if( Intersections.at(0).at(struct_index).size() <= 1 ){
       std::cerr<<  "  Doesn't make sense to voxelize contour with less than two points !  "  <<std::endl;
       assert(0); // this should never happen. Doesn't make sense to voxelize contour with less than two points !       
      }
      
      bool docontinue = true;
      int counter = 0;

      while( docontinue  ){
      
        if( ( Intersections.at(0).at(struct_index).at( point_index ).contour_point_voxel_coord.x != compare_point.x ) || (  Intersections.at(0).at(struct_index).at( point_index ).contour_point_voxel_coord.y != compare_point.y ) ) return true;

        if(  point_index > 0 )  point_index--;
        else point_index = ( Intersections.at(0).at(struct_index).size() - 1 );
        counter++;
        if( counter > ( Intersections.at(0).at(struct_index).size() * 2 ) ) break;
                
      }    
      
      return false; 
                
    }


    void Mask::ExtendFieldOfIntersections( unsigned int index_x , unsigned int index_y , unsigned int index_z , unsigned int struct_index ){
    
        LegacyPolygonSequenceType strVector = _structure->getLegacyStructureVector();

        LegacyPolygonType& the_polygon_ref = strVector.at(struct_index);  

        PolygonInTermsOfIntersections& Polygon_Intersections_Input_Ref = Intersections.at(0).at( struct_index );
        
        LegacyDoseVoxelIndex3D aDoseIndex;
        aDoseIndex.x = index_x;
        aDoseIndex.y = index_y;
        aDoseIndex.z = index_z;
       
        //PolygonInfo polInf = PolygonInfo( the_polygon_ref , struct_index , brightness_outside , brightness_inside , MaskField.at( resolution_level ) , Polygon_Intersections_Input_Ref ,  aDoseIndex , GInf.at(resolution_level) );       
    
        VoxelIntersectionsVectorPointer pol_inf_pointer = new std::vector<IntersectionsThisVoxel>();
        FieldOfIntersections->PutData( index_x, index_y, index_z , pol_inf_pointer );
    
        IntersectionsThisVoxel intersection_info;
        intersection_info.SetPolygonNumber( struct_index );
    
        FieldOfIntersections->GetData( index_x , index_y, index_z )->push_back( intersection_info );
 
    }
       


    int Mask::GetRespectiveIntersections( unsigned int index_x , unsigned int index_y , unsigned int index_z , unsigned int struct_index ) const{

    
     if( FieldOfIntersections->GetData( index_x , index_y, index_z )!= NULL ){
    
       for( int i = 0 ; i < FieldOfIntersections->GetData( index_x , index_y, index_z )->size() ; i++ ){
          if( FieldOfIntersections->GetData( index_x , index_y, index_z )->at(i).GetPolygonNumber() == struct_index )return i;
       }
       
       return (-1);
    
     }
     else return (-1);
    
    }


    LegacyWorldCoordinate2D Mask::GetDoubleIndex2D( double x , double y, int resolution_level ){

      LegacyWorldCoordinate2D result;

      // zunaechst 
      //     x_world = imageOrientationRow.x*pixelSpacingRow*the_index.x+imageOrientationColumn.x*pixelSpacingColumn*the_index.y+imagePosition.x;
      //     y_world = imageOrientationRow.y*pixelSpacingRow*the_index.x+imageOrientationColumn.y*pixelSpacingColumn*the_index.y+imagePosition.y;
      //  mit x_world y_world  ( Welt Koordinaten  alles andere dcmtkrt Ausdruecke )

      // daraus folgt
      // Gleichung a :      (  ( x_world - imagePosition.x ) - ( imageOrientationColumn.x*pixelSpacingColumn*the_index.y  ) ) / ( imageOrientationRow.x*pixelSpacingRow ) = the_index.x ;
      // Gleichung b :    y_world = imageOrientationRow.y*pixelSpacingRow*   (    (  ( x_world - imagePosition.x ) - ( imageOrientationColumn.x*pixelSpacingColumn*the_index.y  ) ) / ( imageOrientationRow.x*pixelSpacingRow )  )      +imageOrientationColumn.y*pixelSpacingColumn*the_index.y+imagePosition.y;

      // Gleichung   b weiter aufgeloest nach the_index.y
      //   y_world - imagePosition.y -   imageOrientationRow.y*pixelSpacingRow*( x_world - imagePosition.x ) / ( imageOrientationRow.x*pixelSpacingRow )  =       - imageOrientationRow.y*pixelSpacingRow*( imageOrientationColumn.x*pixelSpacingColumn*the_index.y  )/ ( imageOrientationRow.x*pixelSpacingRow )     +imageOrientationColumn.y*pixelSpacingColumn*the_index.y 
      // Gelichung   b weiter umgeformt :
      // y_world - imagePosition.y -   imageOrientationRow.y*pixelSpacingRow*( x_world - imagePosition.x ) / ( imageOrientationRow.x*pixelSpacingRow )  =  the_index.y  *  (   imageOrientationColumn.y*pixelSpacingColumn     - imageOrientationRow.y*pixelSpacingRow*( imageOrientationColumn.x*pixelSpacingColumn  )/ ( imageOrientationRow.x*pixelSpacingRow )    ) 
      // es ergibt sich 

      // aus Gleichung  b :  the_index.y =   a / b
      // wobei  a =  y_world - imagePosition.y -   imageOrientationRow.y*pixelSpacingRow*( x_world - imagePosition.x ) / ( imageOrientationRow.x*pixelSpacingRow ) 
      // und    b =   (   imageOrientationColumn.y*pixelSpacingColumn     - imageOrientationRow.y*pixelSpacingRow*( imageOrientationColumn.x*pixelSpacingColumn  )/ ( imageOrientationRow.x*pixelSpacingRow )    ) 

      // Folglich erbibt sich aus Gleichung a :
      //  the_index.x  = ( ( x_world - imagePosition.x )  - ( imageOrientationColumn.x*pixelSpacingColumn* ( a / b )  ) ) / ( imageOrientationRow.x*pixelSpacingRow )  
      // Uebersichtlicher :
      //  the_index.x  = ( c  - ( d * ( a / b )  ) ) / e    
      // c = ( x_world - imagePosition.x )  
      // d = imageOrientationColumn.x*pixelSpacingColumn
      // e = ( imageOrientationRow.x*pixelSpacingRow )

      // in anderen Worten :  

      double a = y -  GInf.at(resolution_level).getImagePositionPatient()(1) - GInf.at(resolution_level).getImageOrientationRow()(1) * GInf.at(resolution_level).getPixelSpacingRow() * ( x - GInf.at(resolution_level).getImagePositionPatient()(0) ) / ( GInf.at(resolution_level).getImageOrientationRow()(0) * GInf.at(resolution_level).getPixelSpacingRow() ) ;
      double b = GInf.at(resolution_level).getImageOrientationColumn()(1) * GInf.at(resolution_level).getPixelSpacingColumn()  - GInf.at(resolution_level).getImageOrientationRow()(1) * GInf.at(resolution_level).getPixelSpacingRow() * ( GInf.at(resolution_level).getImageOrientationColumn()(0) * GInf.at(resolution_level).getPixelSpacingColumn()  ) / ( GInf.at(resolution_level).getImageOrientationRow()(0) * GInf.at(resolution_level).getPixelSpacingRow() ) ;  
      double c = ( x - GInf.at(resolution_level).getImagePositionPatient()(0) )  ;
      double d = GInf.at(resolution_level).getImageOrientationColumn()(0) * GInf.at(resolution_level).getPixelSpacingColumn();
      double e = ( GInf.at(resolution_level).getImageOrientationRow()(0) * GInf.at(resolution_level).getPixelSpacingRow() );

      //alt
      // folglich ergeben sich die voxel Koordinaten zu
      //result.x = ( ( c  - ( d * ( a / b )  ) ) / e ) ;
      //result.y = ( a / b ) ;

      // neu
      // folglich ergeben sich die voxel Koordinaten zu

      // Hier muss das resolution level beruecksichtigt werden. Aenderung vornehmen. Auch testen !

      // result.x = ( ( c  - ( d * ( a / b )  ) ) / e ) + 0.5;
      // result.y = ( a / b ) + 0.5;

      result.x = ( ( c  - ( d * ( a / b )  ) ) / e ) + GetWorldCoordinateOffsetThisResolutionLevel( resolution_level );
      result.y = ( a / b ) + GetWorldCoordinateOffsetThisResolutionLevel( resolution_level );

      return result;    
    
    }


    LegacyWorldCoordinate2D Mask::GetDoubleIndex2D( double x , double y ){

      return GetDoubleIndex2D( x , y, 0 );

    }
    

    double Mask::GetWorldCoordinateOffsetThisResolutionLevel( int resolution_level ){

      double world_coord_offset = 1;
      
      for( int i = 1 ; i <= resolution_level ; i++ )world_coord_offset *= 2;
      
      world_coord_offset = ( 0.5 / world_coord_offset ); 
      
      return world_coord_offset;

    }    


    double Mask::GetBlockLengthThisResolutionLevel( int resolution_level ){

      double length = 1;
      
      for( int i = 1 ; i <= resolution_level ; i++ )length *= 2;
      
      return length;

    }   


    LegacyWorldCoordinate1D Mask::GetZIndex( LegacyWorldCoordinate1D z ){
      // alt
      // return ( ( z  - GInf.at(0).getImagePositionPatient()(2) ) / GInf.at(0).getSliceThickness() );
      //neu
		LegacyWorldCoordinate1D result = ( ( z  - GInf.at(0).getImagePositionPatient()(2) ) / GInf.at(0).getSliceThickness() ) + 0.5;
      return result;
    }

    bool Mask::GetSomeBorderVoxelXYZForTest( LegacyDoseVoxelIndex3D& aDoseIndex ){ 

      correspoinding_structure_vector str_v;
     
      bool got_it = false;

      while( ( got_it == false ) && ( aDoseIndex.z < ( SliceStructureCorrespondency.size() ) ) ){
  
        str_v = SliceStructureCorrespondency.at( aDoseIndex.z );

        if( ( str_v.size() > 0 ) && ( GetSomeBorderVoxelXYForTest( aDoseIndex ) ) ){ 
        
          got_it = true;
        } 
        else aDoseIndex.z++;

      }

      return got_it;  

    }


    bool Mask::GetSomeBorderVoxelXYForTest( LegacyDoseVoxelIndex3D& aDoseIndex ){ 

      bool voxel_found = false;

      unsigned int z = aDoseIndex.z;
      int resolution_level = 0;

      for( unsigned int x = 0 ; x < MaskField.at( resolution_level )->GetDimX() ; x++ ){
        for( unsigned int y = 0 ; y < MaskField.at( resolution_level )->GetDimY() ; y++ ){

  
        
          if( MaskField.at( resolution_level )->GetData( x, y, z ) == brightness_border ){
            voxel_found = true;
            aDoseIndex.x = x;
            aDoseIndex.y = y;
            x = MaskField.at( resolution_level )->GetDimX();
            y = MaskField.at( resolution_level )->GetDimY();
          }
        }
      }

      return voxel_found ;   
    
    }


    bool Mask::DoubleCheckIsOnBorderForTest( LegacyDoseVoxelIndex3D& aDoseIndex ){ 
      int resolution_level = 0;
      if( MaskField.at( resolution_level )->GetData( aDoseIndex.x, aDoseIndex.y, aDoseIndex.z ) == brightness_border ) return true;
      else return false;
    }


    bool Mask::CheckFractionThisVoxelCorrectForTest( LegacyDoseVoxelIndex3D another_dose_index , float the_correct_result ){
      if( GetFractionInside( another_dose_index ) == the_correct_result ) return true; 
      else return false;
    }


    float Mask::GetFractionThisVoxelForTest( LegacyDoseVoxelIndex3D another_dose_index  ){
      return GetFractionInside( another_dose_index );
    }


    PolygonInfo* Mask::JustCreateAPolygonInfoForTest( LegacyDoseVoxelIndex3D aDoseIndex ){ 

      PolygonInfo* polInfTest = NULL;     

      if( do_detailed_subvox ){

        correspoinding_structure_vector str_v;
        bool got_it = false;                           

        while( ( got_it == false ) && ( aDoseIndex.z < ( SliceStructureCorrespondency.size() - 1 ) ) ){
          str_v = SliceStructureCorrespondency.at( aDoseIndex.z );
          if( str_v.size() > 0 ) got_it = true;
          else aDoseIndex.z++;
        }

        // Nur der erste Polygonzug des entsprechenden slices wird betrachtet, daher keine Schleife ueber alle Strukturen dieses Slices noetig. 
        // Die Teststruktur die hier verwendetw wird besteht in diesem Slice nur aus einem Polygonzug. 
        // for( unsigned int count = 0 ; count < str_v.size() ; count++ ){           

        if( got_it ){

          int struct_index = str_v.at( 0 );

          LegacyPolygonSequenceType pol_seq;

          PolygonInTermsOfIntersections& Polygon_Intersections_Input_Ref = Intersections.at(0).at( struct_index );

          LegacyPolygonSequenceType strVector = _structure->getLegacyStructureVector();
          LegacyPolygonType& the_polygon_ref = strVector.at(struct_index);     

          if( FieldOfIntersections ){     
              FieldOfIntersections->SetZero();
              GetIntersectionInformationThisSlice( aDoseIndex.z );
          }  
          int which_one = GetRespectiveIntersections( aDoseIndex.x , aDoseIndex.y , 0 , struct_index );  
    
          if( which_one != (-1) ){
       
            IntersectionsThisVoxel intersections_this_voxel =  FieldOfIntersections->GetData( aDoseIndex.x , aDoseIndex.y , 0 )->at( which_one );     
            polInfTest = new PolygonInfo( the_polygon_ref, struct_index , brightness_outside , brightness_inside ,MaskField.at( 0 ) , Polygon_Intersections_Input_Ref ,  aDoseIndex , GInf.at( 0 ) , intersections_this_voxel );
      
          }
        
        }
        else return NULL;

      }

      return polInfTest;

    }




    bool Mask::GetIntersectionsOfSliceForTest( unsigned int position , PolygonInTermsOfIntersections& Polygon_Intersections_Ref ){

      if( position < Intersections.at(0).size() ){

        Polygon_Intersections_Ref = Intersections.at(0).at( position );
        return true;  

      }
      else return false;

    }


    bool Mask::ReturnFirstWithIntersectionsForTest( unsigned int& position ){

      position = 0;
     
      while( position < Intersections.at(0).size() ){

        if( Intersections.at(0).at( position ).size() > 0 )return true;
        else position++;  

      }
      
      position = 0;
      return false;  

    }


    float Mask::GetFractionInside( const LegacyDoseVoxelIndex3D& aDoseIndex ) { 

      double fraction_total = 0; 

      correspoinding_structure_vector str_v;
      str_v = SliceStructureCorrespondency.at( aDoseIndex.z );

      // Hier werden alle Polygone untersucht, die in diesem Slice liegen.
      for( unsigned int count = 0 ; count < str_v.size() ; count++ ){           

        int struct_index = str_v.at( count );

        int which_one = GetRespectiveIntersections( aDoseIndex.x , aDoseIndex.y , 0 , struct_index );        

        //assert( which_one != (-1) ); // nein nicht assert, denn nicht jede Struktur die zu diesem Slice gehoert schneidet zwangslauefig dieses Voxel

        if( which_one != (-1) ){

        LegacyPolygonSequenceType pol_seq;

        assert( struct_index < Intersections.at(0).size() ); 

        const PolygonInTermsOfIntersections& Polygon_Intersections_Input_Ref = Intersections.at(0).at( struct_index );

        LegacyPolygonSequenceType strVector = _structure->getLegacyStructureVector();

        LegacyPolygonType& the_polygon_ref = strVector.at(struct_index);       

        const IntersectionsThisVoxel& intersections_this_voxel = FieldOfIntersections->GetData( aDoseIndex.x , aDoseIndex.y , 0  )->at(which_one) ;


        PolygonInfo polInf = PolygonInfo( the_polygon_ref , struct_index , brightness_outside , brightness_inside , MaskField.at( 0 ) , Polygon_Intersections_Input_Ref ,  aDoseIndex , GInf.at(0) , intersections_this_voxel );

        if( polInf.CreatePolygonSequence() ){

          pol_seq = polInf.GetResultingSequence();

          double fraction_this_polygon = 0;

          int numberOfContours = pol_seq.size();

          for(int i=0; i<numberOfContours ; i++){            

            Contour contour=Contour( pol_seq.at(i) );
            double area = contour.getArea();            

            fraction_this_polygon += area;                        

          //  if( ! (fraction_this_polygon >= 0) ) std::cout<<  " fraction_this_polygon : " << fraction_this_polygon  <<std::endl;  
          //  if( ! (fraction_this_polygon <= 1) ) std::cout<<  " fraction_this_polygon : " <<  fraction_this_polygon <<std::endl; 

            // Baustelle
            //assert( fraction_this_polygon >= 0 );  // that's essential since ResultingSequence is represented in Voxel coordinates
            //assert( fraction_this_polygon <= 1 );  // that's essential since ResultingSequence is represented in Voxel coordinates

            //double voxel_volume = GInf.at(resolution_level).getPixelSpacingRow() * GInf.at(resolution_level).getPixelSpacingColumn() * GInf.at(resolution_level).getSliceThickness();    

            //if( voxel_volume == 0 )assert(0); // this must never happen. Since depends on dataset : exception to be implemented !

            //fraction_this_polygon /= voxel_volume;     

          }    


          fraction_total += fraction_this_polygon;

        }
        
      }  
              

      } // Iteration ueber Polygone         

      return  fraction_total;   

    }      


 }//namespace
  }//namespace
}//namespace



