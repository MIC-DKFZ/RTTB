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

#include "rttbContour_LEGACY.h"

#include <math.h>
#include <algorithm>
#include <map>




namespace rttb{
	namespace masks{
	  namespace legacy{


        
    Contour::Contour(LegacyPolygonType aPolygon)
    {
        _polygon=aPolygon;
    }

    /*Contour::Contour(int i){

    }*/

    Contour::~Contour(){
    }

    const LegacyPolygonType& Contour::getPolygon() const{
        return _polygon;
    }

 
		/*calculate contour area*/
		double Contour::getArea() const{

			    
      double area=0;
			size_t numberOfPoints=_polygon.size();
			for(int j1=0, j2=1; j1<numberOfPoints;j1++,j2++)
			{
				j2=j2 % numberOfPoints;
				LegacyWorldCoordinate3D p1=_polygon.at(j1);
				LegacyWorldCoordinate3D p2=_polygon.at(j2);
				area+=p1.y*p2.x - p1.x*p2.y;
			}
			area=std::fabs(area*0.5);
			//std::cout << "area: "<< area<<std::endl;
			return area;
				 
			
		}

    bool Contour::intersect(const LegacyWorldCoordinate3D& p1, const LegacyWorldCoordinate3D& p2, const LegacyWorldCoordinate3D& q1, const LegacyWorldCoordinate3D& q2){
        bool intersect=((std::max(p1.x,p2.x)>=std::min(q1.x,q2.x))&& 
                        (std::max(q1.x,q2.x)>=std::min(p1.x,p2.x))&& 
                        (std::max(p1.y,p2.y)>=std::min(q1.y,q2.y))&& 
                        (std::max(q1.y,q2.y)>=std::min(p1.y,p2.y))&& 
                        (multiply(q1,p2,p1)*multiply(p2,q2,p1)>=0)&& 
                        (multiply(p1,q2,q1)*multiply(q2,p2,q1)>=0));
        //if(intersect) std::cout << "intersect: "<<intersect << std::endl;
        return intersect;
    }


    double Contour::multiply(const LegacyWorldCoordinate3D& p1, const LegacyWorldCoordinate3D& p2, const LegacyWorldCoordinate3D& p0) 
    { 
        return((p1.x-p0.x)*(p2.y-p0.y)-(p2.x-p0.x)*(p1.y-p0.y)); 
    }

    //If intersect, calc interpoint. So no test if intersect or not.
    LegacyWorldCoordinate2D Contour::calcInterPoint2D(const LegacyWorldCoordinate2D& p1, const LegacyWorldCoordinate2D& p2, const LegacyWorldCoordinate2D& q1, const LegacyWorldCoordinate2D& q2){
        //std::cout << "p1:"<<p1.x<<","<<p1.y<<", p2:"<<p2.x<<","<<p2.y<<", q1:"<<q1.x<<","<<q1.y<<", q2:"<<q2.x<<","<<q2.y<<std::endl;
        LegacyWorldCoordinate2D p;
        double a=(p2.y-p1.y)/(p2.x-p1.x);
        double b=(q2.y-q1.y)/(q2.x-q1.x);
        //std::cout << "a:"<<a<<", b:"<<b<<std::endl;

        if(a!=b)
        {
        p.x=(WorldCoordinate)((p2.x*a-q2.x*b-p2.y+q2.y)/(a-b));
        p.y=(WorldCoordinate)((p2.y-p1.y)*(p.x-p2.x)/(p2.x-p1.x)+p2.y);
        }
        //parallel, intersect means in the same line
        else{
            //return the mittel point of intersect as interpoint 
            p.x=(WorldCoordinate)((std::min(std::max(p1.x,p2.x),std::max(q1.x,q2.x))+std::max(std::min(p1.x,p2.x),std::min(q1.x,q2.x)))/2);
            p.y=(WorldCoordinate)((std::min(std::max(p1.y,p2.y),std::max(q1.y,q2.y))+std::max(std::min(p1.y,p2.y),std::min(q1.y,q2.y)))/2);
            
        }

        //std::cout << "p.x: " << p.x << std::endl;
        //std::cout << "p.y: " << p.y << std::endl;
        return p;
    }

   LegacyWorldCoordinate3D Contour::calcInterPoint(const LegacyWorldCoordinate3D& p1, const LegacyWorldCoordinate3D& p2, const LegacyWorldCoordinate3D& q1, const LegacyWorldCoordinate3D& q2){
        LegacyWorldCoordinate2D p1_2={p1.x,p1.y};
        LegacyWorldCoordinate2D p2_2={p2.x,p2.y};
        LegacyWorldCoordinate2D q1_2={q1.x,q1.y};
        LegacyWorldCoordinate2D q2_2={q2.x,q2.y};
        LegacyWorldCoordinate2D p_2=calcInterPoint2D(p1_2,p2_2,q1_2,q2_2);
        LegacyWorldCoordinate3D p(p_2.x,p_2.y,p1.z);
        return p;
    }
    
    bool Contour::pointInPolygon(const LegacyWorldCoordinate3D& aPoint) const{

       
      if(_polygon.size()==0){
		    std::cerr << "_polygon is empty! "<< std::endl;
		    return false;
	    }

	    bool isZPlane=true;
	    if(_polygon[0].z != aPoint.z)
		    isZPlane=false;

	    bool inside   =   false;
	    if(!isZPlane)
	    {
		   // std::cerr << "Make sure that the point and the polygon lies both in the z-plane"<<std::endl;
        //isZPlane=true;
	    }
	    //else{
    	
		    LegacyWorldCoordinate3D pnt1, pnt2; 

		
		    bool flag1,flag2;   
            
		    pnt1 = _polygon[0];   
		    flag1 =( aPoint.y >= pnt1.y );  

		    GridIndexType i=0;
            
		    do{   
			    i++;
        		
			    if(i==_polygon.size())
				    pnt2=_polygon[0];
			    else
				    pnt2 = _polygon[i];  

			    //if the point lies on an edge of the polygon, return true
			    if((pnt2.y-aPoint.y)*(pnt1.x-pnt2.x)==(pnt2.x-aPoint.x)*(pnt1.y-pnt2.y) &&
				    (aPoint.x>=std::min(pnt1.x,pnt2.x)) && (aPoint.x <=std::max(pnt1.x,pnt2.x)) &&
				    (aPoint.y>=std::min(pnt1.y,pnt2.y)) && (aPoint.y <=std::max(pnt1.y,pnt2.y)))
				    return true;
                 
			    //else
			    flag2=( aPoint.y >= pnt2.y );   
			    if(flag1 != flag2)   
			    {   	
				    if(((pnt2.y-aPoint.y)*(pnt1.x-pnt2.x)>=(pnt2.x-aPoint.x)*(pnt1.y-pnt2.y))   
					    ==flag2)   
					    inside=!inside;     
			    }   
			    pnt1=pnt2;     
			    flag1=flag2;   
		    }   while(i<_polygon.size());

	    //}
    	
        
        return   inside;     
    }


    
    bool Contour::point2DInPolygon(const LegacyWorldCoordinate2D& aPoint) const{
      if(this->_polygon.size()>0){
        LegacyWorldCoordinate3D p;
		p.x = aPoint.x;
		p.y = aPoint.y;
		p.z = this->_polygon.at(0).z;
        return this->pointInPolygon(p);
      }
      else{
        std::cerr << "_polygon is empty! "<< std::endl;
		    return false;
      }
    }

    NumberOfEndpointsType Contour::getNumberOfEndpoints() const{
        
        return _polygon.size();
    }

		LegacyPolygonType Contour::getMinimalBox() const{
			LegacyPolygonType box;

			WorldCoordinate maxX;
			WorldCoordinate maxY;
			WorldCoordinate minX;
			WorldCoordinate minY;
			WorldCoordinate z;
			for(GridIndexType i=0;i<_polygon.size();i++){
				LegacyWorldCoordinate3D p=_polygon.at(i);
				z=p.z;
				if(i==0){
					minX=p.x;
					minY=p.y;
          maxX=p.x;
          maxY=p.y;
				}
				if(p.x<minX)
					minX=p.x;
				if(p.x>maxX)
					maxX=p.x;
				if(p.y<minY)
					minY=p.y;
				if(p.y>maxY)
					maxY=p.y;

			}

			LegacyWorldCoordinate3D pLU;
			pLU.x = minX; pLU.y = minY; pLU.z = z;
			LegacyWorldCoordinate3D pRU;
			pRU.x = maxX; pRU.y = minY; pRU.z = z;
			LegacyWorldCoordinate3D pRB;
			pRB.x = maxX; pRB.y = maxY; pRB.z = z;
			LegacyWorldCoordinate3D pLB;
			pLB.x = minX; pLB.y = maxY; pLB.z = z;
			box.push_back(pLU);
			box.push_back(pRU);
			box.push_back(pRB);
			box.push_back(pLB);
			return box;
		}

		Contour* Contour::calcIntersection(Contour& contour2){
			
			LegacyPolygonType box1=this->getMinimalBox();
			LegacyWorldCoordinate3D box1P1=box1.at(0);
			LegacyWorldCoordinate3D box1P2=box1.at(1);
			LegacyWorldCoordinate3D box1P3=box1.at(2);
			LegacyWorldCoordinate3D box1P4=box1.at(3);
			LegacyPolygonType box2=contour2.getMinimalBox();
			LegacyWorldCoordinate3D box2P1=box2.at(0);
			LegacyWorldCoordinate3D box2P2=box2.at(1);
			LegacyWorldCoordinate3D box2P3=box2.at(2);
			LegacyWorldCoordinate3D box2P4=box2.at(3);

			Contour box1Contour=Contour(box1);
			//if box1 and box2 have no intersection, that means no intersection between this contour and contour2
			if(!box1Contour.pointInPolygon(box2P1) && !box1Contour.pointInPolygon(box2P2) 
				&& !box1Contour.pointInPolygon(box2P3) && !box1Contour.pointInPolygon(box2P4) ){
				//std::cout << "No intersection!"<<std::endl;
				return NULL;
			}

			//Remove segments of this which not inside box2
			std::vector<LegacySegment3D> segments1InBox2;
			for(GridIndexType i=0;i<_polygon.size();i++){
				LegacyWorldCoordinate3D segmentL=_polygon.at(i);//segment left corner
				LegacyWorldCoordinate3D segmentR;//segment right corner
				if(i==_polygon.size()-1)
					segmentR=_polygon.at(0);
				else
					segmentR=_polygon.at(i+1);

				//if this segment not inside box2
				if(!(std::max(segmentL.x,segmentR.x)<box2P1.x || std::min(segmentL.x,segmentR.x)>box2P2.x
					|| (std::max(segmentL.y,segmentR.y)<box2P1.y) || (std::min(segmentL.y,segmentR.y)>box2P3.y))){
						LegacySegment3D segment;
						segment.begin=segmentL;
						segment.end=segmentR;
						segments1InBox2.push_back(segment);
				}
			}

			//Remove segments of contour2 which not inside box1
			std::vector<LegacySegment3D> segments2InBox1;
			LegacyPolygonType polygon2=contour2.getPolygon();
			for(GridIndexType i=0;i<polygon2.size();i++){
				LegacyWorldCoordinate3D segmentL=polygon2.at(i);//segment left corner
				LegacyWorldCoordinate3D segmentR;//segment right corner
				if(i==polygon2.size()-1)
					segmentR=polygon2.at(0);
				else
					segmentR=polygon2.at(i+1);

				//if this segment not inside box2
				if(!(std::max(segmentL.x,segmentR.x)<box1P1.x || std::min(segmentL.x,segmentR.x)>box1P2.x
					|| (std::max(segmentL.y,segmentR.y)<box1P1.y) || (std::min(segmentL.y,segmentR.y)>box1P3.y))){
						LegacySegment3D segment;
						segment.begin=segmentL;
						segment.end=segmentR;
						segments2InBox1.push_back(segment);
				}
			}

			//Get the points of this contour inside contour2 and all intersect points
			std::vector<LegacyWorldCoordinate3D> points1InContour2;//store points of this contour inside contour2 and all intersect points
			std::vector<LegacySegment3D>::iterator it;
			std::vector<LegacySegment3D>::iterator it2;
			for(it=segments1InBox2.begin();it!=segments1InBox2.end();it++){
				LegacySegment3D segment=*it;
				if(contour2.pointInPolygon(segment.begin))
					points1InContour2.push_back(segment.begin);
				
				//test: if segment has intersection with any segment in segments2InBox1
				for(it2=segments2InBox1.begin();it2!=segments2InBox1.end();it2++){
					if(Contour::intersect(segment.begin,segment.end,(*it2).begin,(*it2).end)){
						LegacyWorldCoordinate3D interP=Contour::calcInterPoint(segment.begin,segment.end,(*it2).begin,(*it2).end);
						if(Contour(points1InContour2).contains(interP))
							points1InContour2.push_back(interP);
					}
				}
			}
			//sort points1InContour2 clockwise
			Contour tempContour=Contour(points1InContour2);
			tempContour.sortClockwise();
			points1InContour2=tempContour.getPolygon();


			//Get the points of contour2 inside this contour and all intersect points
			std::vector<LegacyWorldCoordinate3D> points2InContour1;//store points of contour2 inside this contour and all intersect points
			for(it=segments2InBox1.begin();it!=segments2InBox1.end();it++){
				LegacySegment3D segment=*it;
				if(pointInPolygon(segment.begin))
					points2InContour1.push_back(segment.begin);
				
				//test: if segment has intersection with any segment in segments2InBox1
				for(it2=segments1InBox2.begin();it2!=segments1InBox2.end();it2++){
					if(Contour::intersect(segment.begin,segment.end,(*it2).begin,(*it2).end)){
						LegacyWorldCoordinate3D interP=Contour::calcInterPoint(segment.begin,segment.end,(*it2).begin,(*it2).end);
						if(Contour(points2InContour1).contains(interP))
							points2InContour1.push_back(interP);
					}
				}
			}
			//sort points1InContour2 clockwise
			tempContour=Contour(points2InContour1);
			tempContour.sortClockwise();
			points2InContour1=tempContour.getPolygon();


			LegacyPolygonType intersectionPolygon=points1InContour2;
			LegacyPolygonType::iterator itP;
			for(itP=points2InContour1.begin();itP!=points2InContour1.end();itP++){
				if(!Contour(intersectionPolygon).contains((*itP)))
					intersectionPolygon.push_back((*itP));

			}
			//sort intersectionPolygon
			Contour* intersection=new Contour(intersectionPolygon);
			intersection->sortClockwise();

			return intersection;
		}

		/*! @brief Sort the points in _polygon in clockwise order*/
		void Contour::sortClockwise(){
			size_t numberOfP=_polygon.size();
			if(numberOfP>=3){
				LegacyWorldCoordinate3D p1=_polygon.at(0);
				LegacyWorldCoordinate3D p2=_polygon.at(1);
				LegacyWorldCoordinate3D p3=_polygon.at(2);

				LegacyWorldCoordinate2D pInPolygon = {(p1.x+p2.x+p3.x)/3,(p1.y+p2.y+p3.y)/3};//get a point inside the polygon

				std::map<double, LegacyWorldCoordinate3D> pointsI;
				std::map<double, LegacyWorldCoordinate3D> pointsII;
				std::map<double, LegacyWorldCoordinate3D> pointsIII;
				std::map<double, LegacyWorldCoordinate3D> pointsIV;
				for(int i=0;i<numberOfP;i++){
					LegacyWorldCoordinate3D p=_polygon.at(i);
					double dx=p.x-pInPolygon.x;
					double dy=p.y-pInPolygon.y;
					double ds=sqrt(dx*dx+dy*dy);
					double sin=dy/ds;

					if(dx>=0 && dy>=0){
						pointsI.insert(std::pair<double, LegacyWorldCoordinate3D>(sin*(-1.0),p));
					}
					else if(dx>=0 && dy<0)
						pointsIV.insert(std::pair<double, LegacyWorldCoordinate3D>(sin*(-1.0),p));
					else if(dx<0 && dy<0)
						pointsIII.insert(std::pair<double, LegacyWorldCoordinate3D>(sin,p));
					else
						pointsII.insert(std::pair<double, LegacyWorldCoordinate3D>(sin,p));
				}

				LegacyPolygonType sorted;
				std::map<double, LegacyWorldCoordinate3D>::iterator it;
				for(it=pointsII.begin();it!=pointsII.end();it++)
					sorted.push_back((*it).second);
				for(it=pointsI.begin();it!=pointsI.end();it++)
					sorted.push_back((*it).second);
				for(it=pointsIV.begin();it!=pointsIV.end();it++)
					sorted.push_back((*it).second);
				for(it=pointsIII.begin();it!=pointsIII.end();it++)
					sorted.push_back((*it).second);

				_polygon=sorted;

			}

		}

		std::string Contour::toString() const{
			std::stringstream sstr;
			sstr<<"{";
			for(GridIndexType i=0;i<_polygon.size();i++)
				sstr<<_polygon.at(i).toString()<<", ";
			sstr<<"}";
			return sstr.str();
		}

		bool Contour::contains(const LegacyWorldCoordinate3D& aPoint) const{
			for(GridIndexType i=0;i<_polygon.size();i++){
				if(aPoint==_polygon.at(i))
					return true;
			}
			return false;
		}
}//end namespace legacy
	}//end namespace masks
}//end namespace rttb
