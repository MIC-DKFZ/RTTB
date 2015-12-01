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
#ifndef __CONTOUR_H
#define __CONTOUR_H


#include <vector>


#include "rttbBaseType.h"
#include "rttbBaseType_LEGACY.h"




namespace rttb
{

	namespace masks
	{
		namespace legacy
		{

			typedef size_t NumberOfEndpointsType;

			/*! @class Contour
			*	@brief This is a class representing a RT Contour. A RT contour is either a single point (for a point ROI) or more than
			*		   one point (representing an open or closed polygon).
			*/
			class Contour
			{

			private:
				/*! @brief WorldCoordinate3D in mm
				*/
				LegacyPolygonType _polygon;

				static double multiply(const LegacyWorldCoordinate3D& p1, const LegacyWorldCoordinate3D& p2,
				                       const LegacyWorldCoordinate3D& p0);

				static LegacyWorldCoordinate2D calcInterPoint2D(const LegacyWorldCoordinate2D& p1,
				        const LegacyWorldCoordinate2D& p2, const LegacyWorldCoordinate2D& q1,
				        const LegacyWorldCoordinate2D& q2);

			public:

				//Contour(int i);
				/*! @brief Contour Constructor
				@param aPolygon: a polygon used to generate the contour
				*/
				Contour(LegacyPolygonType aPolygon);

				/*! @brief Destructor
				*/
				~Contour();

				/*! @brief Calculate contour area using Formel 0.5*abs(x1*y2-y1*x2+x2*y3-y2*x3+...+xn*y1-yn*x1)
				@return Return a double value of contour area
				*/
				double getArea() const;

				/*! @brief Tests if a point is inside _polygon. If the point lies on an edge of the polygon, return true
				*! Make sure that the point and the list of positions lies both in the z-plane
				*/
				bool pointInPolygon(const LegacyWorldCoordinate3D& aPoint) const;

				/*! @brief Tests if a 2Dpoint is inside _polygon in 2D. If the point lies on an edge of the polygon, return true
				*
				*/
				bool point2DInPolygon(const LegacyWorldCoordinate2D& aPoint) const;

				/*! @brief Get the polygon
				*/
				const LegacyPolygonType& getPolygon() const;

				/*! @brief Tests if a point is inside the polygon defined by the given list of positions.
				! Make sure that the point and the list of positions lies both in the z-plane
				*/
				//bool pointInStructure(const WorldCoordinate3D& aPoint) const;

				/*! @brief Get the number of end points of the polygon*/
				NumberOfEndpointsType getNumberOfEndpoints() const;

				/*! @brief Get the minimal bounding box
				* @return Return Polygon{{minX,minY,z},{maxX,minY,z},{maxX,maxY,z},{minX,maxY,z}}
				*/
				LegacyPolygonType getMinimalBox() const;

				/*! @brief Get the intersection of this Contour and contour2
				! Make sure that the 2 Contours lies both in the z-plane
				* @return Return NULL if no intersection
				*/
				Contour* calcIntersection(Contour& contour2);

				/*! @brief Sort the points in _polygon in clockwise order*/
				void sortClockwise();

				/*! @brief Calculate the intersection between the two segments (p1,p2) and (q1,q2)
				*  Make sure that the points lies in the same z-plane
				*/
				static LegacyWorldCoordinate3D calcInterPoint(const LegacyWorldCoordinate3D& p1,
				        const LegacyWorldCoordinate3D& p2, const LegacyWorldCoordinate3D& q1,
				        const LegacyWorldCoordinate3D& q2);

				/*! @brief Tests if an intersection exists between two segments (p1, p2) and (q1, q2) */
				static bool intersect(const LegacyWorldCoordinate3D& p1, const LegacyWorldCoordinate3D& p2,
				                      const LegacyWorldCoordinate3D& q1, const LegacyWorldCoordinate3D& q2);

				std::string toString() const;

				/*! @brief If aPoint stored in _polygon
				*/
				bool contains(const LegacyWorldCoordinate3D& aPoint) const;

			};
		}
	}
}

#endif
