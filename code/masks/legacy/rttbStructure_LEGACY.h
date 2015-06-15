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

/* Changes in Architecture:
	  This class should be universally used independent of the origin of the Structures. All UID references are omitted.
*/
#ifndef __STRUCTURE_LEGACY_H
#define __STRUCTURE_LEGACY_H


#include <vector>
#include <math.h>

#include "rttbBaseType.h"
#include "rttbBaseType_LEGACY.h"
#include "rttbStructure.h"


namespace rttb{

  namespace masks{
	  namespace legacy{

    /*! @class Structure
    @brief This is a class representing a RT Structure
    */
	class StructureLegacy
    {
    private:
		/*! @brief WorldCoordinate3D in mm
      */
      PolygonSequenceType _structureVector;

	  //legacy data structure for faster computation in original toolbox voxelization
	  LegacyPolygonSequenceType _legacyStructureVector;

      /*! @brief Contour Geometric Type using DICOM-RT definition (3006,0042).
      * POINT: indicates that the contour is a single point, defining a specific location of significance.
      * OPEN_PLANAR: indicates that the last vertex shall not be connected to the first point, and that all points 
      * in Contour Data (3006,0050) shall be coplanar.
      * OPEN_NONPLANAR: indicates that the last vertex shall not be connected to the first point, and that the points 
      * in Contour Data(3006,0050) may be non-coplanar.
      * CLOSED_PLANAR: indicates that the last point shall be connected to the first point, where the first point is 
      * not repeated in the Contour Data. All points in Contour Data (3006,0050) shall be coplanar.
      */

	  // wo und wie wird das Initialisier? Ist Vector die geeignete Datenstruktur?
	  std::vector<std::string> _contourGeometricTypeVector;

	  /*! @brief Structure UID*/
      IDType _strUID;

      /*! @brief Structure Label*/
      StructureLabel _label;

	  



    public:
      /** copy constructor
      *  @param copy Structure object to be copied
      */
	  StructureLegacy(const core::Structure& copy);


      /*! @brief Structure Destructor 
      */
	  ~StructureLegacy();

      /*! @brief Get the structure vector
      *  @Return Return a LegacyPolygonSequenceType
      */
	  const LegacyPolygonSequenceType& getLegacyStructureVector()const;

	  /*! @brief Get the structure vector
      *  @Return Return a PolygonSequenceType
      */
      const PolygonSequenceType& getStructureVector() const;


      /*! @brief Get the volume of this Structure. Calculation using volume = 1/3 * h* (S1+S2+(S1*S2)^0,5)
      @return Return the absolute volume in mm3
      */
      VolumeType getVolume() const;

      /*! @brief Structure operation: contain
      @return Return the contain factor of str2 in this structure
      @brief 0<value<1: this structure contains part of str2; -1<value<0: str2 contains part of this structure; value=0: there is no intersection between str2 and this structure
      */
      double contain(const StructureLegacy& str2) const;

      /*! @brief Tests if a point is inside the polygon defined by the given list of positions.
      ! Make sure that the point and the list of positions lies both in the z-plane
      */
      bool pointInStructure(const LegacyWorldCoordinate3D& aPoint) const;

      /*! @brief Get the number of end points of all contours of the structure.
      */
      int getNumberOfEndpoints() const;

	  /*! @brief Get structure uid.   
      */
      IDType getUID() const;

      /*! @brief Set structure uid.   
      */
      void setUID(const IDType aUID);

      void setLabel(const StructureLabel aLabel);

      StructureLabel getLabel() const;




    };
	  }
  }//end namespace masks
}//end namespace rttb

#endif
