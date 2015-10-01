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

#include "rttbStructure_LEGACY.h"

#include "rttbNullPointerException.h"
#include "rttbInvalidParameterException.h"
#include "rttbContour_LEGACY.h"

//#include <math.h>
//#include <algorithm>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <iostream>
#include <sstream>



namespace rttb{
	namespace masks{
	namespace legacy{
	
		StructureLegacy::StructureLegacy(const core::Structure& copy) {
			this->_structureVector=copy.getStructureVector();
			this->_legacyStructureVector.clear();
			this->_strUID=copy.getUID();
			this->_label=copy.getLabel();

			//initialize legacy structureVector
			LegacyWorldCoordinate3D p;
			if (!(_structureVector.size() == _legacyStructureVector.size())){ //needs to be converted
				_legacyStructureVector.clear();
				PolygonSequenceType::const_iterator itVV=_structureVector.begin();
				for(;itVV!=_structureVector.end();itVV++){
					PolygonType v1=*itVV;
					rttb::masks::legacy::LegacyPolygonType legacyPolygon;
					for(PolygonType::const_iterator itPP=(*itVV).begin();itPP!=(*itVV).end();itPP++){
						p.x = itPP->x();
						p.y = itPP->y();
						p.z = itPP->z();
						legacyPolygon.push_back(p);
					}
					_legacyStructureVector.push_back(legacyPolygon);
				}
			}
		}

		StructureLegacy::~StructureLegacy(){
		}

		const PolygonSequenceType& StructureLegacy::getStructureVector() const{
			return _structureVector;
		}


		const LegacyPolygonSequenceType& StructureLegacy::getLegacyStructureVector() const{

			return _legacyStructureVector;
		}

	VolumeType StructureLegacy::getVolume() const{

			/*check precondition: contourGeometricType must be closed planar! */
      for(int i=0;i<this->_contourGeometricTypeVector.size();i++){
        std::cout << _contourGeometricTypeVector[i]<<std::endl;
        if(_contourGeometricTypeVector[i]!="POINT" && _contourGeometricTypeVector[i]!="CLOSED_PLANAR")
          throw std::invalid_argument("Error: getVolume() can only be called for closed planar structure! ");
      }

      /*std::cout <<"GetVolume: structureVector 0 vector 0 Point:"<<_structureVector.at(0).at(0)->x<<std::endl;
      std::cout <<"structureVector size:"<<_structureVector.size()<<std::endl;
      std::cout <<"structureVector 0 vector size:"<<_structureVector.at(0).size()<<std::endl;*/

      int numberOfContours=this->_legacyStructureVector.size();
      std::cout <<"numberOfContours:"<<numberOfContours<<std::endl;
      double volume=0;
      for(int i=0; i<numberOfContours-1 ; i++)
      {
        Contour contour1=Contour(_legacyStructureVector.at(i));
        double area1=contour1.getArea();
        Contour contour2=Contour(_legacyStructureVector.at(i+1));
        double area2=contour2.getArea();

        LegacyWorldCoordinate3D p2=_legacyStructureVector.at(i+1).at(0);
        LegacyWorldCoordinate3D p1=_legacyStructureVector.at(i).at(0);
        double height=p2.z-p1.z;

        if(height==0){
          std::cout << "same slice!"<<std::endl;

        }


        //Using volume = 1/3 * h* (S1+S2+(S1*S2)^0,5)
        volume+=(height*(area1+area2+pow(area1*area2, 0.5)))/3;


        //std::cout << pow(area1*area2, 0.5) << std::endl;

        //std::cout << "area1:"<<area1<<", area2: "<<area2 << ", height: "<<height <<", volume: "<<volume << std::endl;
      }
	  volume=std::fabs(volume);
      //std::cout << "volume: "<<volume<<std::endl;

      return volume;
		}


		double StructureLegacy::contain(const StructureLegacy& str2) const{
			int count=0;
			double factor=0;
			//str2 bigger than this structure
			if(this->getVolume() < str2.getVolume()){
				LegacyPolygonSequenceType::const_iterator itVV;			
				for(itVV=this->_legacyStructureVector.begin();itVV!=_legacyStructureVector.end();itVV++){
					LegacyPolygonType::iterator itV;
					LegacyPolygonType vectorW=*itVV;
					for(itV=vectorW.begin();itV!=vectorW.end();itV++){
						LegacyWorldCoordinate3D p=*itV;
						if(str2.pointInStructure(p)){
							count++;
							//std::cout << "p: "<<p(0)<<","<<p(1)<<","<<p(2)<<std::endl;
						}
						/*else
						std::cout << "p: "<<p(0)<<","<<p(1)<<","<<p(2)<<std::endl;*/
					}
				}
				factor=-(count*1.0/this->getNumberOfEndpoints());
			}
			//this structure bigger than str2
			else{
				LegacyPolygonSequenceType::const_iterator itVV;
				const LegacyPolygonSequenceType& str2Vector = str2.getLegacyStructureVector();
				for(itVV=str2Vector.begin();itVV!=str2Vector.end();itVV++){
					LegacyPolygonType::iterator itV;
					LegacyPolygonType vectorW=*itVV;
					for(itV=vectorW.begin();itV!=vectorW.end();itV++){
						LegacyWorldCoordinate3D p=*itV;
						if(this->pointInStructure(p)){
							count++;
						}
						/*else
						std::cout << "p: "<<p(0)<<","<<p(1)<<","<<p(2)<<std::endl;*/
					}
				}
				factor=count*1.0/str2.getNumberOfEndpoints();
			}
			return factor;
		}

		bool StructureLegacy::pointInStructure(const LegacyWorldCoordinate3D& aPoint) const{
			LegacyPolygonSequenceType::const_iterator itVV;
			size_t size=_structureVector.size();

			//if between the fist and last polygon of the structure
			if(aPoint.z >= _legacyStructureVector.at(0).at(0).z && aPoint.z <=_legacyStructureVector.at(size-1).at(0).z){

				int polygonNr=0;
				//get correspoinding slice
				for(int i=0;i<size;i++){
					LegacyPolygonType roi=_legacyStructureVector.at(i);
					LegacyPolygonType roiNext;

					//if not the last polygon
					if(i!=size-1){

						roiNext=_legacyStructureVector.at(i+1);

						LegacyWorldCoordinate3D p1=roi[0];
						LegacyWorldCoordinate3D p2=roiNext[0];

						//if roi and roiNext in the same slice
						if(p1.z==p2.z){
							if(aPoint.z==p1.z){
								polygonNr=i;
								break;
							}
						}
						else{
							//if aPoint between roi i and roi i+1
							if(aPoint.z>= p1.z && aPoint.z<p2.z){
								polygonNr=i;
								break;
							}
						}
					}
					else
						polygonNr=i;
				}


				for(int i=polygonNr;i<size;i++){
					LegacyPolygonType roi=_legacyStructureVector.at(i);

					//get aPoint's projection on the slice
					LegacyWorldCoordinate3D p(aPoint.x,aPoint.y, roi.at(0).z);
					  Contour contour=Contour(roi);
					  if(contour.pointInPolygon(p))
						return true;
				}
			}

			return false;
		}

		int StructureLegacy::getNumberOfEndpoints() const{
			int count=0;
			PolygonSequenceType::const_iterator itVV;
			for(itVV=_structureVector.begin();itVV!=_structureVector.end();itVV++){
				count+=(int)(*itVV).size();
			}
			return count;
		}

		IDType StructureLegacy::getUID() const{

			return _strUID;
		}

		void StructureLegacy::setUID(const IDType aUID){
			_strUID=aUID;
		}

		void StructureLegacy::setLabel(const StructureLabel aLabel){
			_label=aLabel;
		}

		StructureLabel StructureLegacy::getLabel() const{
			return _label;
		}


}//end namespace legacy
	}//end namespace masks
}//end namespace rttb
