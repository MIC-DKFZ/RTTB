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

#include "DoseVoxel_LEGACY.h"
#include "rttbInvalidParameterException.h"
#include "rttbContour_LEGACY.h"

using namespace std;

namespace rttb{
	namespace masks{
namespace legacy{

		DoseVoxel::DoseVoxel(){
			//default 1
			_proportionInStr=1;
		}

		DoseVoxel::DoseVoxel(const VoxelGridIndex3D &aDoseIndex, const core::GeometricInfo* aGeometricInfo){
			if(aDoseIndex(0)<0 || aDoseIndex(1)<0 || aDoseIndex(2)<0){
				std::cout << aDoseIndex.toString()<<std::endl;
				throw rttb::core::InvalidParameterException("Invalid dose index!");
			}
			else if(aGeometricInfo->getPixelSpacingRow()<=0 || aGeometricInfo->getPixelSpacingColumn()<=0 || aGeometricInfo->getSliceThickness() <=0)
				throw rttb::core::InvalidParameterException("Pixel spacing and slice thickness must not be zero! ");
			else{
				_voxelIndex3D.x=aDoseIndex(0);
				_voxelIndex3D.y=aDoseIndex(1);
				_voxelIndex3D.z=aDoseIndex(2);
				_geoInfo=aGeometricInfo;

				WorldCoordinate3D currentWorld(0);
				aGeometricInfo->indexToWorldCoordinate(aDoseIndex,currentWorld);
				_x=currentWorld(0);
				_y=currentWorld(1);
				_z=currentWorld(2);
				//std::cout << "zIndex:"<<aDoseIndex(2)<< ", "<< getSliceThickness()<< ", _z:"<<_z<<std::endl;
				_deltaX=aGeometricInfo->getPixelSpacingRow();
				_deltaY=aGeometricInfo->getPixelSpacingColumn();
				_deltaZ=aGeometricInfo->getSliceThickness();
			}

			//default 1
			_proportionInStr=1;
		}

		DoseVoxel::DoseVoxel(const LegacyDoseVoxelIndex3D &aDoseIndex, const rttb::core::GeometricInfo* aGeometricInfo){
			if(aDoseIndex.x<0 || aDoseIndex.y<0 || aDoseIndex.z<0){
				std::cout << aDoseIndex.toString()<<std::endl;
				throw rttb::core::InvalidParameterException("Invalid dose index!");
			}
			else if(aGeometricInfo->getPixelSpacingRow()<=0 || aGeometricInfo->getPixelSpacingColumn()<=0 || aGeometricInfo->getSliceThickness() <=0)
				throw rttb::core::InvalidParameterException("Pixel spacing and slice thickness must not be zero! ");
			else{
				_voxelIndex3D=aDoseIndex;
				_geoInfo=aGeometricInfo;

    // old
				//_x=aGeometricInfo->imageOrientationRow.x*aGeometricInfo->pixelSpacingRow*(aDoseIndex.x)+aGeometricInfo->imageOrientationColumn.x*aGeometricInfo->pixelSpacingColumn*(aDoseIndex.y)+aGeometricInfo->imagePositionPatient.x;
				//_y=aGeometricInfo->imageOrientationRow.y*aGeometricInfo->pixelSpacingRow*(aDoseIndex.x)+aGeometricInfo->imageOrientationColumn.y*aGeometricInfo->pixelSpacingColumn*(aDoseIndex.y)+aGeometricInfo->imagePositionPatient.y;
				//_z=aGeometricInfo->imagePositionPatient.z + aGeometricInfo->sliceThickness *(aDoseIndex.z);

        //Martina
				WorldCoordinate3D currentWorld(0);
				VoxelGridIndex3D currentIndex(aDoseIndex.x,aDoseIndex.y,aDoseIndex.z);
				aGeometricInfo->indexToWorldCoordinate(currentIndex,currentWorld);
				_x=currentWorld(0);
				_y=currentWorld(1);
				_z=currentWorld(2);

        //Lanlan
        //_x=aGeometricInfo->imageOrientationRow.x*aGeometricInfo->pixelSpacingRow*aDoseIndex.x+aGeometricInfo->imageOrientationColumn.x*aGeometricInfo->pixelSpacingColumn*aDoseIndex.y+aGeometricInfo->imagePositionPatient.x-0.5*aGeometricInfo->pixelSpacingRow;
				//_y=aGeometricInfo->imageOrientationRow.y*aGeometricInfo->pixelSpacingRow*aDoseIndex.x+aGeometricInfo->imageOrientationColumn.y*aGeometricInfo->pixelSpacingColumn*aDoseIndex.y+aGeometricInfo->imagePositionPatient.y-0.5*aGeometricInfo->pixelSpacingColumn;

				//_z=aGeometricInfo->imagePositionPatient.z + aGeometricInfo->sliceThickness *(aDoseIndex.z)-0.5*aGeometricInfo->sliceThickness;
				
				//std::cout << "zIndex:"<<aDoseIndex.z<< ", "<< sliceThickness<< ", _z:"<<_z<<std::endl;
				_deltaX=aGeometricInfo->getPixelSpacingRow();
				_deltaY=aGeometricInfo->getPixelSpacingColumn();
				_deltaZ=aGeometricInfo->getSliceThickness();
    
			}

			//default 1
			_proportionInStr=1;
		}


		bool DoseVoxel::operator==(const DoseVoxel& doseVoxel) const{
			return (_x==doseVoxel._x)&& (_y==doseVoxel._y)&& (_z==doseVoxel._z)
				&& (_deltaX==doseVoxel._deltaX) && (_deltaY==doseVoxel._deltaY)&& (_deltaZ==_deltaZ);
		}

		bool DoseVoxel::operator<(const DoseVoxel& doseVoxel) const{
			WorldCoordinate3D leftP=doseVoxel.getLeftUpperPoint();
			WorldCoordinate3D deltaP=doseVoxel.getDelta();
			if(_z<leftP(2))
				return true;
			else if(_z==leftP(2)){
				if(_y<leftP(1))
					return true;
				else if(_y==leftP(1)){
					if(_x<leftP(0))
						return true;
					else 
						return false;
				}
				else
					return false;
			}
			else
				return false;

		}

        /*! @brief Transform dose index to world coordinate
             *
             */
void DoseVoxel::setDoseVoxel(const VoxelGridIndex3D &aDoseIndex, const core::GeometricInfo* aGeometricInfo) {
    WorldCoordinate3D aWorldCoordinate(0);      
    if(aDoseIndex(0)<0 || aDoseIndex(1)<0 || aDoseIndex(2)<0)
        throw rttb::core::InvalidParameterException("Invalid dose index!");
    else{
		_voxelIndex3D.x=aDoseIndex(0);
		_voxelIndex3D.y=aDoseIndex(1);
		_voxelIndex3D.z=aDoseIndex(2);
		_geoInfo=aGeometricInfo;

		if(aGeometricInfo->indexToWorldCoordinate(aDoseIndex , aWorldCoordinate)){
			_x = aWorldCoordinate(0);
			_y = aWorldCoordinate(1);
			_z = aWorldCoordinate(2);
		}
		else{
			//something went wrong!
		}

 /*   //Martina
    _x=aGeometricInfo->getImageOrientationRow()(0)*aGeometricInfo->getPixelSpacingRow()*(aDoseIndex(0)-0.5)+aGeometricInfo->getImageOrientationColumn()(0)*aGeometricInfo->getPixelSpacingColumn()*(aDoseIndex(1)-0.5)+aGeometricInfo->getImagePositionPatient()(0);
    _y=aGeometricInfo->getImageOrientationRow()(1)*aGeometricInfo->getPixelSpacingRow()*(aDoseIndex(0)-0.5)+aGeometricInfo->getImageOrientationColumn()(1)*aGeometricInfo->getPixelSpacingColumn()*(aDoseIndex(1)-0.5)+aGeometricInfo->getImagePositionPatient()(1);
    _z=aGeometricInfo->getImagePositionPatient()(2) + aGeometricInfo->getSliceThickness() *(aDoseIndex(2)-0.5);
*/
    //Lanlan
    //_x=aGeometricInfo->getImageOrientationRow()(0)*aGeometricInfo->getPixelSpacingRow()*aDoseIndex(0)+aGeometricInfo->getImageOrientationColumn()(0)*aGeometricInfo->getPixelSpacingColumn()*aDoseIndex(1)+aGeometricInfo->getImagePositionPatient()(0)-0.5*aGeometricInfo->getPixelSpacingRow();
    //_y=aGeometricInfo->getImageOrientationRow()(1)*aGeometricInfo->getPixelSpacingRow()*aDoseIndex(0)+aGeometricInfo->getImageOrientationColumn()(1)*aGeometricInfo->getPixelSpacingColumn()*aDoseIndex(1)+aGeometricInfo->getImagePositionPatient()(1)-0.5*aGeometricInfo->getPixelSpacingColumn();

    //_z=aGeometricInfo->getImagePositionPatient()(2) + aGeometricInfo->getSliceThickness() *(aDoseIndex(2))-0.5*aGeometricInfo->getSliceThickness();

    //std::cout << "zIndex:"<<aDoseIndex(2)<< ", "<< getSliceThickness()<< ", _z:"<<_z<<std::endl;
    _deltaX=aGeometricInfo->getPixelSpacingRow();
    _deltaY=aGeometricInfo->getPixelSpacingColumn();
    _deltaZ=aGeometricInfo->getSliceThickness();
    }

    //default 1
    _proportionInStr=1;
    
}
void DoseVoxel::setDoseVoxel(const LegacyDoseVoxelIndex3D &aDoseIndex, const rttb::core::GeometricInfo* aGeometricInfo) {
           
    if(aDoseIndex.x<0 || aDoseIndex.y<0 || aDoseIndex.z<0)
		throw rttb::core::InvalidParameterException("Invalid dose index!");
    else{
		_voxelIndex3D=aDoseIndex;
		_geoInfo=aGeometricInfo;


		//Martina
		WorldCoordinate3D aWorldCoordinate(0);
		VoxelGridIndex3D currentIndex(aDoseIndex.x,aDoseIndex.y,aDoseIndex.z);
		if(aGeometricInfo->indexToWorldCoordinate(currentIndex , aWorldCoordinate)){
			_x = aWorldCoordinate(0);
			_y = aWorldCoordinate(1);
			_z = aWorldCoordinate(2);
		}

    //Lanlan
    //_x=aGeometricInfo->imageOrientationRow.x*aGeometricInfo->pixelSpacingRow*aDoseIndex.x+aGeometricInfo->imageOrientationColumn.x*aGeometricInfo->pixelSpacingColumn*aDoseIndex.y+aGeometricInfo->imagePositionPatient.x-0.5*aGeometricInfo->pixelSpacingRow;
    //_y=aGeometricInfo->imageOrientationRow.y*aGeometricInfo->pixelSpacingRow*aDoseIndex.x+aGeometricInfo->imageOrientationColumn.y*aGeometricInfo->pixelSpacingColumn*aDoseIndex.y+aGeometricInfo->imagePositionPatient.y-0.5*aGeometricInfo->pixelSpacingColumn;

    //_z=aGeometricInfo->imagePositionPatient.z + aGeometricInfo->sliceThickness *(aDoseIndex.z)-0.5*aGeometricInfo->sliceThickness;

    //std::cout << "zIndex:"<<aDoseIndex.z<< ", "<< sliceThickness<< ", _z:"<<_z<<std::endl;
    _deltaX=aGeometricInfo->getPixelSpacingRow();
    _deltaY=aGeometricInfo->getPixelSpacingColumn();
    _deltaZ=aGeometricInfo->getSliceThickness();
    }

    //default 1
    _proportionInStr=1;
    

}
/*! @brief Set dose voxel with the left upper point and delta
             *
             */
void DoseVoxel::setDoseVoxel(WorldCoordinate3D aWorldCoordinate, WorldCoordinate3D aVoxelDelta){
	_x=aWorldCoordinate(0);
	_y=aWorldCoordinate(1);
	_z=aWorldCoordinate(2);
	_deltaX=aVoxelDelta(0);
	_deltaY=aVoxelDelta(1);
	_deltaZ=aVoxelDelta(2);

}
void DoseVoxel::setDoseVoxel(LegacyWorldCoordinate3D aWorldCoordinate, LegacyWorldCoordinate3D aVoxelDelta){
	_x=aWorldCoordinate.x;
	_y=aWorldCoordinate.y;
	_z=aWorldCoordinate.z;
	_deltaX=aVoxelDelta.x;
	_deltaY=aVoxelDelta.y;
	_deltaZ=aVoxelDelta.z;

}

/*! @brief Tests if a point is inside the voxel
             *
             */
bool DoseVoxel::pointInVoxel(const WorldCoordinate3D& aPoint) const {
	if ((aPoint(0) <=(_x+_deltaX) && aPoint(0) >=_x ) && 
	    (aPoint(1) <=(_y+_deltaY) && aPoint(1) >=_y ) && 
	    (aPoint(2) <=(_z+_deltaZ) && aPoint(2) >=_z ))
		return true;
	else 
		return false;
}

bool DoseVoxel::pointInVoxel(const LegacyWorldCoordinate3D& aPoint) const {
	if ((aPoint.x <=(_x+_deltaX) && aPoint.x >=_x ) && 
	    (aPoint.y <=(_y+_deltaY) && aPoint.y >=_y ) && 
	    (aPoint.z <=(_z+_deltaZ) && aPoint.z >=_z ))
		return true;
	else 
		return false;
}

/*! @brief Calculates how many relative volume of the voxel inside the structure.
 *  @return Return the relative volume. >0: voxel in structure, =0: voxel not in structure, <0: error
 *  @pre aRTStructure must not be NULL
 */
double DoseVoxel::voxelInStructure(const StructureLegacy& aRTStructure) const{
    
    //bool voxelInStr=false;
	const PolygonSequenceType& strVector=aRTStructure.getStructureVector();
	
    int size=strVector.size();

    if(size==0){
        std::cerr << "Error: structure vector of aRTStructure is empty! "<<std::endl;
        return -1;
    }

    

    double voxelInStructure=0;

    /*roiNumber[i]=j means _z+i*_deltaZ lies between the j-th and (j+1)-th ROI*/
    /*int roiNumber[5]={0,0,0,0,0};
    for(int i=0;i<size;i++){
        vector<WorldCoordinate3D*> roi=strVector[i];
        vector<WorldCoordinate3D*> roiNext;
        if(i==size-1)
            roiNext=strVector[0];
        else
            roiNext=strVector[i+1];

        WorldCoordinate3D* p1=roi[0];
        WorldCoordinate3D* p2=roiNext[0];


        if(_z>= p1->z && _z<p2->z)
            roiNumber[0]=i;
        else if(_z+_deltaZ/5 >= p1->z && _z+_deltaZ/5 <p2->z)
            roiNumber[1]=i;
        else if(_z+2*_deltaZ/5 >= p1->z && _z+2*_deltaZ/5 <p2->z)
            roiNumber[2]=i;
        else if(_z+3*_deltaZ/5 >= p1->z && _z+3*_deltaZ/5 <p2->z)
            roiNumber[3]=i;
        else if(_z+4*_deltaZ/5 >= p1->z && _z+4*_deltaZ/5 <p2->z)
            roiNumber[4]=i;

    }

    //get 5^3=125 points from the voxel
    for(int i=0; i<5; i++){
        //std::cout <<"------------"<<i<<"----------"<<std::endl;
        vector<WorldCoordinate3D*> voxelPoints;
        //std::cout << "----------------------------Voxel Points: "<<std::endl;
        //std::cout <<"_x:"<<this->_x<<std::endl;
        for(int j=0;j<5; j++){
            for(int k=0;k<5;k++){
                WorldCoordinate3D* p=new WorldCoordinate3D;
                p->x=(_x+k*_deltaX/5);
                p->y=(_y+j*_deltaY/5);
                p->z=(_z+i*_deltaZ/5);
                //std::cout << "("<<p(0)<<","<<p(1)<<","<<p(2)<<") ";
                voxelPoints.push_back(p);
            }
        }

        //for(int l=0;l<voxelPoints.size();l++){
        int l=0;
            WorldCoordinate3D* voxelPoint=voxelPoints[l];
            //std::cout <<"voxelPoint:"<<voxelPoint->x<<","<<voxelPoint->y<<","<<voxelPoint->z<<std::endl;
            
            vector<WorldCoordinate3D*> roi=strVector[roiNumber[i]];
            if(this->pointInPolygon(roi,voxelPoint))
                voxelPointInStructure++;
        //}

        //~
        vector<WorldCoordinate3D*>::iterator it;
        for(it=voxelPoints.begin();it!=voxelPoints.end();it++){
            delete (*it);
        }


    }*/
    

    /*PolygonType roi=strVector[i];
    if(roi.size()==0){
        std::cerr << "Error: corresponding roi of aRTStructure is empty! "<<std::endl;
        return -1;
    }

    WorldCoordinate3D p={_x,_y, roi[0](2)};
    if(this->pointInPolygon(roi,p))
        voxelPointInStructure++;*/

    //return voxelPointInStructure/125;
    //return voxelPointInStructure/1;

    std::vector<int> sliceV=this->getZIntersectPolygonIndexVector(aRTStructure);

    for(int i=0;i<sliceV.size();i++){
      voxelInStructure+=voxelInStructureAtIndex(aRTStructure,sliceV.at(i));
    }

	return voxelInStructure;
}

 /*! @brief Calculates how many relative volume of the voxel inside the structure, with the corresponding polygon index number. 
 *         It will be tested if the dose voxel in any Polygon with the same z coord..
 *  @param sliceNumber: fist index number of the polygon stored in strVector (ranked by z coord. of the polygon) if the z coord. 
 *                      of the dose voxel between the last Polygon and this Polygon!!! 
 *          
 *  @return Return the relative volume. >0: voxel in structure, =0: voxel not in structure, <0: error
 *  @pre aRTStructure must not be NULL
*/
double DoseVoxel::voxelInStructure(const StructureLegacy& aRTStructure, int sliceNumber) const{


    
    const LegacyPolygonSequenceType& strVector=aRTStructure.getLegacyStructureVector();
	int size=strVector.size();

    if(size==0){
        std::cerr << "Error: structure vector of aRTStructure is empty! "<<std::endl;
        return -1;
    }

    if(sliceNumber<-1 || sliceNumber>size){
        std::cerr << "Error: sliceNumber must between -1 and "<<size<<std::endl;
        return -1;
    }

	//out of structure and _z bigger than z coordi.of the last polygon: voxel not in structure.
	if(sliceNumber==size) return 0;

	//out of structure and _z smaller than z coordi.of the first polygon: 
	if(sliceNumber==-1){
		//if _z+_deltaZ smaller than z coordi.of the first polygon: voxel not in structure
		if(_z+_deltaZ<strVector.at(0).at(0).z)
			return 0;
		//else test from the first polygon
		else sliceNumber=0;
	}

    int voxelPointInStructure=0;

    
    double interArea=0;
    double voxelArea=_deltaX*_deltaY;
    

	
    for(int i=sliceNumber; i<size;i++){
		  LegacyPolygonType roi=strVector[i];
		  Contour contour(roi);

		  //test, if any of the 4 points of this dose voxel lie in the roi  
		  LegacyWorldCoordinate1D zPlane=roi.at(0).z;
		  LegacyWorldCoordinate3D p;
		  p.x = _x+_deltaX/4;
		  p.y = _y+_deltaY/4;
		  p.z = zPlane;
  		
		  if(contour.pointInPolygon(p)) voxelPointInStructure++;
		  p.x =_x+_deltaX*3/4;
		  if(contour.pointInPolygon(p)) voxelPointInStructure++;
		  p.y =_y+_deltaY*3/4;
		  if(contour.pointInPolygon(p)) voxelPointInStructure++;
		  p.z =_x+_deltaX/4;
		  if(contour.pointInPolygon(p)) voxelPointInStructure++;

      /*p(0)=_x;
      p(1)=_y;
  		
		  if(contour.pointInPolygon(p)) voxelPointInStructure++;
		  p(0)=_x+_deltaX;
		  if(contour.pointInPolygon(p)) voxelPointInStructure++;
		  p(1)=_y+_deltaY;
		  if(contour.pointInPolygon(p)) voxelPointInStructure++;
		  p(0)=_x;
		  if(contour.pointInPolygon(p)) voxelPointInStructure++;*/


      /*get intersection*/

      /*if(voxelPointInStructure=4)
        return 1;
      else{
        PolygonType voxelPolygon;
        WorldCoordinate3D p1={_x,_y,zPlane};
        WorldCoordinate3D p2={_x,_y+_deltaY,zPlane};
        WorldCoordinate3D p3={_x+_deltaX,_y+_deltaY,zPlane};
        WorldCoordinate3D p4={_x+_deltaX,_y,zPlane};
        voxelPolygon.push_back(p1);
        voxelPolygon.push_back(p2);
        voxelPolygon.push_back(p3);
        voxelPolygon.push_back(p4);
        Contour voxelContour=Contour(voxelPolygon);
        
        Contour* interContour=voxelContour.calcIntersection(contour);
      
        if(interContour)
          interArea+=interContour->getArea();
      }*/
      
    


		//if the next contour not in the same slice, break
		  if(i<size-1){
			  if(strVector.at(i+1).at(0).z!=roi.at(0).z)
				  break;
		  }

	
    }

	//std::cout << voxelPointInStructure/4.0<<std::endl;

    
    //return voxelPointInStructure/125;
  return voxelPointInStructure/4.0;
  
  //return interArea/voxelArea;
}

double DoseVoxel::voxelInStructureAtIndex(const StructureLegacy& aRTStructure, int polygonIndex) const{

    
  const LegacyPolygonSequenceType& strVector=aRTStructure.getLegacyStructureVector();
  int size=strVector.size();

  if(size==0){
      std::cerr << "Error: structure vector of aRTStructure is empty! "<<std::endl;
      return -1;
  }

  if(polygonIndex<-1 || polygonIndex>size){
      std::cerr << "Error: polygonIndex must between -1 and "<<size<<std::endl;
      return -1;
  }

	//out of structure and _z bigger than z coordi.of the last polygon: voxel not in structure.
	if(polygonIndex==size) return 0;

	//out of structure and _z smaller than z coordi.of the first polygon: 
	if(polygonIndex==-1){
			return 0;
	}

    
  int voxelPointInStructure=0;

    
    
  double interArea=0;
  double voxelArea=_deltaX*_deltaY;
    
  LegacyPolygonType roi=strVector[polygonIndex];
  Contour contour=Contour(roi);

  //test, if any of the 4 points of this dose voxel lie in the roi  
  LegacyWorldCoordinate1D zPlane=roi.at(0).z;
  LegacyWorldCoordinate3D p;
  p.x = _x+_deltaX/4;
  p.y = _y+_deltaY/4;
  p.z = zPlane;
	
  if(contour.pointInPolygon(p)) voxelPointInStructure++;
  p.x =_x+_deltaX*3/4;
  if(contour.pointInPolygon(p)) voxelPointInStructure++;
  p.y =_y+_deltaY*3/4;
  if(contour.pointInPolygon(p)) voxelPointInStructure++;
  p.z =_x+_deltaX/4;
  if(contour.pointInPolygon(p)) voxelPointInStructure++;

  /*p(0)=_x;
  p(1)=_y;
	
  if(contour.pointInPolygon(p)) voxelPointInStructure++;
  p(0)=_x+_deltaX;
  if(contour.pointInPolygon(p)) voxelPointInStructure++;
  p(1)=_y+_deltaY;
  if(contour.pointInPolygon(p)) voxelPointInStructure++;
  p(0)=_x;
  if(contour.pointInPolygon(p)) voxelPointInStructure++;*/

      /*get intersection*/

      /*if(voxelPointInStructure=4)
        return 1;
      else{
        PolygonType voxelPolygon;
        WorldCoordinate3D p1={_x,_y,zPlane};
        WorldCoordinate3D p2={_x,_y+_deltaY,zPlane};
        WorldCoordinate3D p3={_x+_deltaX,_y+_deltaY,zPlane};
        WorldCoordinate3D p4={_x+_deltaX,_y,zPlane};
        voxelPolygon.push_back(p1);
        voxelPolygon.push_back(p2);
        voxelPolygon.push_back(p3);
        voxelPolygon.push_back(p4);
        Contour voxelLegacyContour=Contour(voxelPolygon);
        
        Contour* interLegacyContour=voxelLegacyContour.calcIntersection(contour);
      
        if(interLegacyContour)
          interArea+=interLegacyContour->getArea();
      }*/
      
    

	//std::cout << voxelPointInStructure/4.0<<std::endl;

    
    //return voxelPointInStructure/125;
  return voxelPointInStructure/4.0;
  
  //return interArea/voxelArea;
}

int DoseVoxel::getCorrespondingSlice(const StructureLegacy& aRTStructure) const{

    const LegacyPolygonSequenceType& strVector=aRTStructure.getLegacyStructureVector();//strVector is ranked by _z of the contour
	  int size=strVector.size();

    if(size==0){
        throw std::out_of_range( "Error: structure vector of aRTStructure is empty! ");
    }

	//out of structure and _z+_deltaZ smaller than the first structure slice
	if((_z+_deltaZ)<strVector.at(0).at(0).z)
		return -1;
    
    for(int i=0;i<size;i++){

		//if the last polygon       
      if(i==size-1){
        //if top of the voxel >= last polygon
        
	    //Reduce copying of values, since boost_vector constructors are expensive
        //Anmerkung fuer Lanlan zwecks mergen : Eine Kontur, die direkt in einem slice liegt sollte nicht in beiden angrenzenden Slices voxelisiert werden.
        if(_z<=strVector[i][0].z && _z+_deltaZ > strVector[i][0].z)
          return i;
        else
				  return size;//out of structure and _z bigger than the last structure slice
       }
		//else
        else{

			//if roi and roiNext in the same slice
			    if(strVector[i][0].z==strVector[i+1][0].z){
            //if voxel intersect the slice
				    if(_z<=strVector[i][0].z && _z+_deltaZ>=strVector[i][0].z)
					    return i;
			    }
			    else{
            if(_z+_deltaZ>= strVector[i][0].z && _z<strVector[i+1][0].z){
					    if(_z+_deltaZ<=strVector[i+1][0].z)
					        //std::cout << "_z:"<<_z<<", p1(2):"<<p1->z<<", p2(2):"<<p2->z<<std::endl;
					         return i;//if top of the voxel <roiNext, set i
              else
                   return i+1;//if top of the voxel >=roiNext, set i+1
            }
    			
          }
        }

    }
    return -1;//out of structure and _z smaller than the first structure slice

}

std::vector<int> DoseVoxel::getZIntersectPolygonIndexVector(const StructureLegacy& aRTStructure) const{


    std::vector<int> sliceNumberVector;

    const LegacyPolygonSequenceType& strVector=aRTStructure.getLegacyStructureVector();//strVector is ranked by _z of the contour
	  int size=strVector.size();

    if(size==0){
        throw std::out_of_range( "Error: structure vector of aRTStructure is empty! ");
    }

	
    int firstSlice=this->getCorrespondingSlice(aRTStructure);
    if(firstSlice>=0 && firstSlice<size){
      sliceNumberVector.push_back(firstSlice);

      LegacyWorldCoordinate1D zCoord=strVector.at(firstSlice).at(0).z;

      //add all polygon index with the same z
      for(int i=0;i<size;i++)
        if( strVector.at(i).at(0).z==zCoord && i!=firstSlice ){
          sliceNumberVector.push_back(i);
        }
    }
    

    return sliceNumberVector;

}

WorldCoordinate3D DoseVoxel::getLeftUpperPoint() const{
	WorldCoordinate3D p;
	p(0) = _x;
	p(1) = _y;
	p(2) = _z;
	return p;
}

WorldCoordinate3D DoseVoxel::getDelta() const{
	WorldCoordinate3D p;
	p(0) = _deltaX;
	p(1) = _deltaY;
	p(2) = _deltaZ;
	return p;
}
LegacyWorldCoordinate3D DoseVoxel::getLeftUpperPointLegacy() const{
	LegacyWorldCoordinate3D p(_x,_y,_z);
	return p;
}

LegacyWorldCoordinate3D DoseVoxel::getDeltaLegacy() const{
	LegacyWorldCoordinate3D p(_deltaX,_deltaY, _deltaZ);
	return p;
}
std::vector<WorldCoordinate3D> DoseVoxel::calcIntersectionPoints(WorldCoordinate3D firstPoint, WorldCoordinate3D secondPoint){

	//4 Segment of the dose voxel:[(_x,_y),(_x+_deltaX,_y)],[(_x+_deltaX,_y),(_x+_deltaX,_y+_deltaY)],[(_x+_deltaX,_y+_deltaY),(_x,_y+_deltaY)],[(_x,_y+_deltaY),(_x,_y)]

	//segment: (y-first(1))*(secont(0)-first(0))=(x-first(0))*(secont(1)-first(1))
	int a=secondPoint(0)-firstPoint(0);
	int b=secondPoint(1)-firstPoint(1);

	std::vector<WorldCoordinate3D> intersectionPoints;

	if(a==0){
		if(firstPoint(0)>=_x && firstPoint(0)<=(_x+_deltaX)){
			if(min(firstPoint(1),secondPoint(1))<=_y && max(firstPoint(1), secondPoint(1))>=_y){
				WorldCoordinate3D p;
				p(0) = firstPoint(0);
				p(1) = _y;
				p(2) = _z;
				intersectionPoints.push_back(p);
			}
			if(min(firstPoint(1),secondPoint(1))<=(_y+_deltaY) && max(firstPoint(1), secondPoint(1))>=(_y+_deltaY)){
				WorldCoordinate3D p;
				p(0) = firstPoint(0);
				p(1) = _y+_deltaY;
				p(2) = _z;
				intersectionPoints.push_back(p);
			}

		}

	}
	else if(b==0){
		if(firstPoint(1)>=_y && firstPoint(1)<=(_y+_deltaY)){
			if(min(firstPoint(0),secondPoint(0))<=_x && max(firstPoint(0), secondPoint(0))>=_x){
				WorldCoordinate3D p;
				p(0) = _x;
				p(1) = firstPoint(1);
				p(2) = _z;
				intersectionPoints.push_back(p);
			}
			if(min(firstPoint(0),secondPoint(0))<=(_x+_deltaX) && max(firstPoint(0), secondPoint(0))>=(_x+_deltaX)){
				WorldCoordinate3D p;
				p(0) = _x+_deltaX;
				p(1) = firstPoint(1);
				p(2) = _z;
				intersectionPoints.push_back(p);
			}

		}

	}
	else{
		//1.[(_x,_y),(_x+_deltaX,_y)]
		double interX=(_y-firstPoint(1))*a/b+firstPoint(0);
		if(interX>=_x && interX<=(_x+_deltaX)){
			WorldCoordinate3D p;
			p(0) = interX;
			p(1) = _y;
			p(2) = _z;
			intersectionPoints.push_back(p);
		}

		//2.[(_x+_deltaX,_y),(_x+_deltaX,_y+_deltaY)]
		double interY=(_x+_deltaX-firstPoint(0))*b/a+firstPoint(1);
		if(interY>=_y && interY<=(_y+_deltaY)){
			WorldCoordinate3D p;
			p(0) = _x+_deltaX;
			p(1) = interY;
			p(2) = _z;
			intersectionPoints.push_back(p);
		}

		//3. [(_x+_deltaX,_y+_deltaY),(_x,_y+_deltaY)]
		interX=(_y+_deltaY-firstPoint(1))*a/b+firstPoint(0);
		if(interX>=_x && interX<=(_x+_deltaX)){
			WorldCoordinate3D p;
			p(0) = interX;
			p(1) = _y+_deltaY;
			p(2) = _z;
			intersectionPoints.push_back(p);
		}

		//4.[(_x,_y+_deltaY),(_x,_y)]
		interY=(_x-firstPoint(0))*b/a+firstPoint(1);
		if(interY>=_y && interY<=(_y+_deltaY)){
			WorldCoordinate3D p;
			p(0) = _x;
			p(1) = interY;
			p(2) = _z;
			intersectionPoints.push_back(p);
		}

	}
	return intersectionPoints;
}

std::vector<LegacyWorldCoordinate3D> DoseVoxel::calcIntersectionPoints(LegacyWorldCoordinate3D firstPoint, LegacyWorldCoordinate3D secondPoint){

	//4 Segment of the dose voxel:[(_x,_y),(_x+_deltaX,_y)],[(_x+_deltaX,_y),(_x+_deltaX,_y+_deltaY)],[(_x+_deltaX,_y+_deltaY),(_x,_y+_deltaY)],[(_x,_y+_deltaY),(_x,_y)]

	//segment: (y-first.y)*(secont.x-first.x)=(x-first.x)*(secont.y-first.y)
	int a=secondPoint.x-firstPoint.x;
	int b=secondPoint.y-firstPoint.y;

	std::vector<LegacyWorldCoordinate3D> intersectionPoints;

	if(a==0){
		if(firstPoint.x>=_x && firstPoint.x<=(_x+_deltaX)){
			if(min(firstPoint.y,secondPoint.y)<=_y && max(firstPoint.y, secondPoint.y)>=_y){
				LegacyWorldCoordinate3D p(firstPoint.x,_y,_z);
				intersectionPoints.push_back(p);
			}
			if(min(firstPoint.y,secondPoint.y)<=(_y+_deltaY) && max(firstPoint.y, secondPoint.y)>=(_y+_deltaY)){
				LegacyWorldCoordinate3D p(firstPoint.x,_y+_deltaY,_z);
				intersectionPoints.push_back(p);
			}

		}

	}
	else if(b==0){
		if(firstPoint.y>=_y && firstPoint.y<=(_y+_deltaY)){
			if(min(firstPoint.x,secondPoint.x)<=_x && max(firstPoint.x, secondPoint.x)>=_x){
				LegacyWorldCoordinate3D p(_x,firstPoint.y,_z);
				intersectionPoints.push_back(p);
			}
			if(min(firstPoint.x,secondPoint.x)<=(_x+_deltaX) && max(firstPoint.x, secondPoint.x)>=(_x+_deltaX)){
				LegacyWorldCoordinate3D p(_x+_deltaX,firstPoint.y,_z);
				intersectionPoints.push_back(p);
			}

		}

	}
	else{
		//1.[(_x,_y),(_x+_deltaX,_y)]
		double interX=(_y-firstPoint.y)*a/b+firstPoint.x;
		if(interX>=_x && interX<=(_x+_deltaX)){
			LegacyWorldCoordinate3D p(interX,_y,_z);
			intersectionPoints.push_back(p);
		}

		//2.[(_x+_deltaX,_y),(_x+_deltaX,_y+_deltaY)]
		double interY=(_x+_deltaX-firstPoint.x)*b/a+firstPoint.y;
		if(interY>=_y && interY<=(_y+_deltaY)){
			LegacyWorldCoordinate3D p(_x+_deltaX,interY,_z);
			intersectionPoints.push_back(p);
		}

		//3. [(_x+_deltaX,_y+_deltaY),(_x,_y+_deltaY)]
		interX=(_y+_deltaY-firstPoint.y)*a/b+firstPoint.x;
		if(interX>=_x && interX<=(_x+_deltaX)){
			LegacyWorldCoordinate3D p(interX,_y+_deltaY,_z);
			intersectionPoints.push_back(p);
		}

		//4.[(_x,_y+_deltaY),(_x,_y)]
		interY=(_x-firstPoint.x)*b/a+firstPoint.y;
		if(interY>=_y && interY<=(_y+_deltaY)){
			LegacyWorldCoordinate3D p(_x,interY,_z);
			intersectionPoints.push_back(p);
		}

	}
	return intersectionPoints;
}

/*! @brief Get the voxel index*/
const LegacyDoseVoxelIndex3D& DoseVoxel::getVoxelIndex3D() const{
	return _voxelIndex3D;
}

/*! @brief Get the geometric information*/
const core::GeometricInfo* DoseVoxel::getGeometricInfo() const{
	return _geoInfo;
}

void DoseVoxel::setProportionInStr(FractionType aProportionInStr){
	_proportionInStr=aProportionInStr;
}

FractionType DoseVoxel::getProportionInStr() const{
	return _proportionInStr;
}
}
	}//end namespace core
}//end namespace rttb

