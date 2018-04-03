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

/* Changes in Architecture:
This class should be universally used independent of the origin of the Structures. All UID references are omitted.
*/
#ifndef __STRUCTURE_H
#define __STRUCTURE_H


#include <vector>
#include <cmath>

#include <boost/shared_ptr.hpp>

#include "rttbBaseType.h"

#include "RTTBCoreExports.h"


namespace rttb
{

	namespace core
	{

		/*! @class Structure
		@brief This is a class representing a RT Structure
		*/
		class RTTBCore_EXPORT Structure
		{
		public:
			using StructTypePointer = boost::shared_ptr<Structure>;
		private:
			/*! @brief WorldCoordinate3D in mm
			*/
			PolygonSequenceType _structureVector;


			/*! @brief Contour Geometric Type using DICOM-RT definition (3006,0042).
			* POINT: indicates that the contour is a single point, defining a specific location of significance.
			* OPEN_PLANAR: indicates that the last vertex shall not be connected to the first point, and that all points
			* in Contour Data (3006,0050) shall be coplanar.
			* OPEN_NONPLANAR: indicates that the last vertex shall not be connected to the first point, and that the points
			* in Contour Data(3006,0050) may be non-coplanar.
			* CLOSED_PLANAR: indicates that the last point shall be connected to the first point, where the first point is
			* not repeated in the Contour Data. All points in Contour Data (3006,0050) shall be coplanar.
			*/
			std::vector<ContourGeometricTypeString> _contourGeometricTypeVector;

			/*! @brief Structure UID*/
			IDType _strUID;

			/*! @brief Structure Label*/
			StructureLabel _label;


		public:
			/*! @brief Structure Standard Constructor
				uid will be randomly generated using boost::uuid library at first. To change the uid using setUID().
			*/
			Structure();

			/*! @brief Structure Constructor
				uid will be randomly generated using boost::uuid library at first. To change the uid using setUID().
			*/
			Structure(PolygonSequenceType strVector);

			Structure(const Structure& copy);

			~Structure();

			const PolygonSequenceType& getStructureVector() const;

			/*! @brief Get the number of end points (points that define the polygon) of all contours of the structure.
			*/
			int getNumberOfEndpoints() const;

			IDType getUID() const;

			void setUID(const IDType& aUID);

			void setLabel(const StructureLabel& aLabel);

			StructureLabel getLabel() const;

		};
	}//end namespace core
}//end namespace rttb

#endif
