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

#ifndef __RT_BASE_TYPE_H
#define __RT_BASE_TYPE_H

#include <string>
#include <vector>
#include <exception>
#include <map>
#include <iostream>
#include <sstream>

#include "rttbBaseType.h"


namespace rttb{
   namespace masks{
	  namespace legacy{

    typedef double LegacyWorldCoordinate1D;

    struct LegacyWorldCoordinate2D{
      LegacyWorldCoordinate1D x;
      LegacyWorldCoordinate1D y;

      std::string toString() const{
        std::stringstream sstr;
        sstr<<"("<<x<<","<<y<<")";
        return sstr.str();
      }

      bool operator==(const LegacyWorldCoordinate2D& p) const
      {
        return(x==p.x && y==p.y );
      }

      /** assigment operator
      *  @param copy LegacyWorldCoordinate2D object to be copied
      */
      LegacyWorldCoordinate2D& operator=(const LegacyWorldCoordinate2D& copy){
        if (this != &copy)
        {
          x=copy.x;
          y=copy.y;
        }
        return *this;
      }

	};

    typedef struct LegacyWorldCoordinate3D{
      LegacyWorldCoordinate1D x;
      LegacyWorldCoordinate1D y;
      LegacyWorldCoordinate1D z;

      std::string toString() const{
        std::stringstream sstr;
        sstr<<"("<<x<<","<<y<<","<<z<<")";
        return sstr.str();
      }

	  LegacyWorldCoordinate3D(){
		  this->x = 0;
		  this->y = 0;
		  this->z = 0;
	  }

	  LegacyWorldCoordinate3D( LegacyWorldCoordinate1D a, LegacyWorldCoordinate1D b, LegacyWorldCoordinate1D c){
		  this->x = a;
		  this->y = b;
		  this->z = c;
	  }

	  LegacyWorldCoordinate3D(rttb::WorldCoordinate3D aCoordinate){
		  this->x = aCoordinate.x();
		  this->y = aCoordinate.y();
		  this->z = aCoordinate.z();
	  }

      bool operator==(const LegacyWorldCoordinate3D& p) const
      {
        return(x==p.x && y==p.y && z==p.z);
      }

      /** assigment operator
      *  @param copy LegacyWorldCoordinate3D object to be copied
      */
      LegacyWorldCoordinate3D& operator=(const LegacyWorldCoordinate3D& copy){
        if (this != &copy)
        {
          x=copy.x;
          y=copy.y;
          z=copy.z;
        }
        return *this;
      }
	  //vector cross product (not included in boost.ublas)
		LegacyWorldCoordinate3D cross(LegacyWorldCoordinate3D aVector) const{
			LegacyWorldCoordinate3D result;
			LegacyWorldCoordinate1D x = this->x;
			LegacyWorldCoordinate1D y = this->y;
			LegacyWorldCoordinate1D z = this->z;

			result.x = y*aVector.z-z*aVector.y;
			result.y = z*aVector.x-x*aVector.z;
			result.z = x*aVector.y-y*aVector.x;

			return result;
		}

	}LegacyImageOrientation;





    typedef int LegacyDoseVoxelIndex1D;


    struct LegacyDoseVoxelIndex3D{
      LegacyDoseVoxelIndex1D x;
      LegacyDoseVoxelIndex1D y;
      LegacyDoseVoxelIndex1D z;

      std::string toString() const{
        std::stringstream sstr;
        sstr<<"("<<x<<","<<y<<","<<z<<")";
        return sstr.str();
      }

      bool operator<(const LegacyDoseVoxelIndex3D& index) const
      {
        if(z<index.z)
          return true;   
        else if(z>index.z)
          return false;
        else{
          if(y<index.y)
            return true;
          else if(y>index.y)
            return false;
          else{
            if(x<index.x)
              return true;
            else
              return false;
          }
        }
      }

	};

    typedef struct{
      LegacyDoseVoxelIndex1D x;
      LegacyDoseVoxelIndex1D y;

      std::string toString() const{
        std::stringstream sstr;
        sstr<<"("<<x<<","<<y<<")";
        return sstr.str();
      }


	}LegacyDoseVoxelIndex2D;


    typedef unsigned short LegacyUnsignedIndex1D;

    typedef struct{
      LegacyUnsignedIndex1D x;
      LegacyUnsignedIndex1D y;
    }LegacyUnsignedIndex2D;

    typedef struct{
      LegacyUnsignedIndex1D x;
      LegacyUnsignedIndex1D y;
      LegacyUnsignedIndex1D z;
    }LegacyUnsignedIndex3D;

    typedef std::vector<LegacyWorldCoordinate3D> LegacyPolygonType;

    typedef std::vector<LegacyPolygonType> LegacyPolygonSequenceType;


    typedef struct{

      LegacyWorldCoordinate1D x_begin;

      LegacyWorldCoordinate1D x_end;

      LegacyWorldCoordinate1D y_begin;

      LegacyWorldCoordinate1D y_end;

      LegacyDoseVoxelIndex2D index_begin;

      LegacyDoseVoxelIndex2D index_end; 	

      void Init(){
        x_begin = -1000000;
        x_end = -1000000;
        y_begin = -1000000;
        y_end = -1000000;		
        index_begin.x = 0;
        index_begin.y = 0;			        
        index_end.x = 0; 	
        index_end.y = 0; 			               

      }

	}LegacyArea2D;

    typedef struct{
      LegacyWorldCoordinate2D begin;
      LegacyWorldCoordinate2D end;
    }LegacySegment2D;

    typedef struct{
      LegacyWorldCoordinate3D begin;
      LegacyWorldCoordinate3D end;
    }LegacySegment3D;

	 /*! @brief area_vector specifies an image region. So its a vector of 2d areas. That means it is a 3D area.
    */
    typedef std::vector<LegacyArea2D> area_vector; 
    typedef std::vector< std::vector<LegacyArea2D> > vector_of_area_vectors;

}

  }

}

#endif

