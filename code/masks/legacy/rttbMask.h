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
#ifndef __RT_MASK_H
#define __RT_MASK_H

#include "rttbBaseType_LEGACY.h"
#include "rttbBaseType.h"
#include "rttbStructure_LEGACY.h"
#include "DoseVoxel_LEGACY.h"
#include "DoseIteratorInterface_LEGACY.h"
#include "rttbField_LEGACY.h"
#include "rttbMaskType_LEGACY.h"
#include "rttbContour_LEGACY.h"
#include "rttbPolygonInfo_LEGACY.h"
#include "rttbIterativePolygonInTermsOfIntersections_LEGACY.h"
#include <math.h>
#include <deque>

#include <RTTBLegacyMaskExports.h>

namespace rttb
{
	namespace masks
	{
		namespace legacy
		{

			/*! @class Mask
			*   @brief This class represents the Mask calculated using a dose iterator and a StructureLegacy/DoseVoxelVector.
			           The mask is first calculated with voxel accurracy. Those voxels that are touched by the structure are then further
			           regarded using PolygonInfo and IterativePolygonInTermsOfIntersections to find out the fraction that is
			           inside the structure.
			           The voxelization is done on several resultution levels, but the calculation on subvoxel accurracy,
			           that means the calculation of the fraction of the voxel that is inside the structure, is done solely on the finest resolution level.
			           And solely for those voxels that are touched by the structure - typically most voxels are not.
			            This multi resolution approach is much faster as compared to doing it on just one level,
			           because a voxel that is not touched by the structure and determinded to be inside the structure on a
			              croase resolution level does on all finer resolution level solely correspond to voxels that are inside, too. The same
			              goes for a voxel that was determined to be completely outside: All the voxels that are contained in this voxel
			              on each finer resolution level are colpletely outside, too. So the creation of the mask without subvoxel accurracy is
			              done on the croasest resolution level completely and for each voxel with clear affiliation inside/outside all the
			              respective voxels are set for all the finer resolution levels right away as they are determined on a croase level.
			              It is cumputational effort to create several masks
			              and it is effort to set the inside/outside affiliation of a voxel on all levels instead of just one. However, it saves
			              effort, because on each finer resolution level, only view voxels
			              need to be considered: Those that are close to the contour and were thus touched by the contour on the previous level
			               that was to croase to tell the inside/outside affiliation. Since there is a factor four in the number of voxels between two levels,
			                the multi resouluton calculation is fast, because view voxels need to be regarded.
			               Field.h has an efficient way of finding those voxels with unclear affiliation and of setting affiliation throughout resoltuion levels in blocks.
			               After reaching the finest resolution those voxels that are still in the border region
			               and thus just touched by the structure are then further examined with subvoxel accurracy, based on
			               PolygonInfo and IterativePolygonInTermsOfIntersections.


			*/
            class RTTBLegacyMask_EXPORT Mask
			{

			public:


				/*! @brief Constructor with a DoseIterator and a StructureLegacy
				*  The mask is calculated with sub voxel fraction info.
				   @pre The Structure must not intersect itself. It may touch itself.
				   @param aDoseIter is a pointer to a DoseIteratorInterface. It is supposed to contain the information about
				          the dose value associated with each voxel.
				   @param aStructure is a Pointer to the StructureLegacy which holds the contour the mask shall be based on.
				*  @exception RTTBNullPointerException thrown if aDoseIter or aStructure is NULL
				*/
				Mask(DoseIteratorInterface* aDoseIter  , StructureLegacy* aStructure);

				/*! @brief Constructor with a DoseIterator and a vector of DoseVoxel inside a structure (with voxel proportion information)
				    @param aDoseIter is a pointer to a DoseIteratorInterface. It is supposed to contain the information about
				           the dose value associated with each voxel.
				    @param aDoseVoxelVector is a reference to a vector that holds values of the type DoseVoxel. Using this parameter it is possible
				           to accept a given "voxelization" created by Mevis for example.
				*  @exception RTTBNullPointerException thrown if aDoseIter is NULL
				*/
				//      Mask( DoseIteratorInterface* aDoseIter  , const std::vector<legacy::DoseVoxel>& aDoseVoxelVector );

				/*! @brief Constructor with a DoseIterator and a StructureLegacy
				*  The mask is calculated with sub voxel fraction info.
				   @pre The Structure must not intersect itself. It may touch itself.
				   @param aGeoInfo is a pointer to a GeometricInfo of the dose.
				   @param aStructure is a Pointer to the StructureLegacy which holds the contour the mask shall be based on.
				*  @exception RTTBNullPointerException thrown if aDoseGeoInfo or aStructure is NULL
				*/
				Mask(core::GeometricInfo* aDoseGeoInfo  , StructureLegacy* aStructure);


				/*! @brief Destructor
				*/
				~Mask();

				/*! @brief Old Code!!! It is unnecessary now. The user should check the geometric information of the new dose iterator, if changed,
				    generate new mask using Mask( core::GeometricInfo* aDoseGeoInfo  , StructureLegacy* aStructure)!

				    *Old brief: Reset dose. If the geometric information has not changed, the _doseVoxelInStructure does not need to be recalculated, in case
				           other dose values are introduced for the same structure and geometry. Only doseData needs to be recalculated.
				    @param aDoseIter is a pointer to a DoseIteratorInterface. It is supposed to contain the information about
				           the dose value associated with each voxel.
				*  @exception RTTBNullPointerException Thrown if aDoseIter or aStructure is NULL

				*/
				void resetDose(DoseIteratorInterface* aDoseIterator);



				/*! @brief Get the dose voxels which lie inside the structure
				*  @return Vector of DoseVoxel which are inside the structure (with voxel proportion information)
				*/
				const std::vector<DoseVoxel>& getDoseVoxelInStructure() const;

				/*! @brief Set the vector of DoseVoxel which are inside the structure (with voxel proportion information)
				* @param  aDoseVoxelVector Vector of RTDoseVoxels inside the structure to be set.
				* @return
				*/
				void setDoseVoxelInStructure(std::vector<DoseVoxel> aDoseVoxelVector);

				/*! @brief Get the dose iterator
				* @return DoseIteratorInterface& returns the dose Iterator.
				*/
				const DoseIteratorInterface& getDoseIterator() const;

				/*! @brief Get the structure
				* @return StructureLegacy& Returns the structure.
				*/
				const StructureLegacy& getRTStructure() const;

				/*! @brief Get absolute dose data of the voxels which are inside the structure.
				    @return Returns the dose values.
				*/
				//const std::vector<DoseTypeGy> getDoseData() const;


				/*new...................*/

				/*! @brief Calculates 2D voxel coordinates from 2D world coordinates.
				    @param x voxel coordinate x
				    @param y voxel coordinate y
				    @return Returns 2D world coordinates.
				*/
				LegacyWorldCoordinate2D GetDoubleIndex2D(double x , double y);
				/*! @brief Calculates 2D voxel coordinates from 2D world coordinates taking the current resolution level under consideration.
				    @param x voxel coordinate x
				    @param y voxel coordinate y
				    @param resolution_level current resolution
				    @return Returns 2D voxel coordinates.
				*/
				LegacyWorldCoordinate2D GetDoubleIndex2D(double x , double y, int resolution_level);

				/*! @brief The offset between world and voxel coordinate system depends on the current resolution. This function calculates this offset.
				    @param resolution_level current resolution
				    @return Returns the offset between world and voxel coordinate system.
				*/
				double GetWorldCoordinateOffsetThisResolutionLevel(int resolution_level);

				// Functions and other stuff needed for unit tests:
				/*! @brief Needed for unit tests. Provides the Information about the Intersections of a specific polygon,
				           that is part of the structure, with the edges of the voxels.
				    @param position Index des polygon of interest. The structure consists of an entity of plygons.
				    @param Polygon_Intersections_Ref Refernce to an object of the Type PolygonInTermsOfIntersection. After running the function, this Reference holds the Information about
				           the intersections of the polygon of interest (specified by position) with the voxel edges.
				    @return This function returns false and does not do its job, in case "position" holds an unreasonalble value, e.g. refers to a
				            polygon that does not exist.
				*/
				bool GetIntersectionsOfSliceForTest(unsigned int position ,
				                                    PolygonInTermsOfIntersections& Polygon_Intersections_Ref);
				/*! @brief Needed for unit tests. Returns true in case there are any intersections of the structure with the voxel edges.
				                                  A reference to an integer is set to the index of the first polygon that intersects with voxel edges
				                                  in case there are intersections. Otherways the reference is set to zero and the function returns false.
				    @param position Some reference to some integer which is modified by this function.
				                    In case there are any intersection this reference is set to the index of the first intersecting polygon.
				                    Otherways it is set to zero.
				    @return  Returns false in case there are no intersections and true in case there are intersections of the contour with the voxel edges.
				*/
				bool ReturnFirstWithIntersectionsForTest(unsigned int& position);
				/*! @brief Needed for unit tests. This function is made to test the function GoIntersectColumn which is private and therefore not
				                                  accessible for the testing unit. This function called TestGoIntersectColumnByComparisonWithExpectedResult
				                                  is provided with two 3D Points and with a vector called expected_intersections_ref that holds the information about
				                                  those intersections that are to be expected. These values are set by the one who wrote the unit test. Now these expected
				                                  values are compared with those that are calculated by the function GoIntersectColumn and in case there is any deviation
				                                  between acutal an expected result this function here returns false. Otherwise it returns true.

				    @param firstPoint The first one of two points that specify a line which is to be checked for intersections with voxel edges.
				    @param secondPoint The second one of two points that specify a line which is to be checked for intersections with voxel edges.
				    @param column_intersections_ref After running this function this reference to a vector of objects of the type LegacyWorldCoordinate2D
				                                    holds the information about the calculated intersections.
				    @param expected_intersections_ref This is a reference to a vector of objects of the type LegacyWorldCoordinate2D which holds the
				                                      information about the expected intersections. These intersections are set by the programmer
				                                      who writes the unit test and who knows the correct intersections.
				    @return This funciton returns true in case the function GoIntersectColumn turned out to work properly. Otherwise it returns false.
				*/
				bool TestGoIntersectColumnByComparisonWithExpectedResult(LegacyWorldCoordinate3D firstPoint ,
				        LegacyWorldCoordinate3D secondPoint ,
				        std::vector<LegacyWorldCoordinate2D>& column_intersections_ref,
				        std::vector<LegacyWorldCoordinate2D>& expected_intersections_ref);
				/*! @brief Needed for unit tests. See TestGoIntersectColumnByComparisonWithExpectedResult for details since this function here is very similar.
				           The only difference is that this funciton is made to help with unit testing with respect to the funcion GoIntersectRaw
				           instead of GoIntersectColumn - despite that its just the same.
				*/
				bool TestGoIntersectRawByComparisonWithExpectedResult(LegacyWorldCoordinate3D firstPoint ,
				        LegacyWorldCoordinate3D secondPoint , std::vector<LegacyWorldCoordinate2D>& raw_intersections_ref,
				        std::vector<LegacyWorldCoordinate2D>& expected_intersections_ref);
				/*! @brief Needed for unit tests. Checks whether two vectors of points that are passed to this function are identical.
				    @param calculated_intersections_ref First vector of points to be compared with the second one.
				    @param expected_intersections_ref Second vector of points to be compared with the first one.
				    @return Returns true in case the vectors are identical.
				*/
				bool IdenticalIntersectionsForTest(std::vector<LegacyWorldCoordinate2D>&
				                                   calculated_intersections_ref , std::vector<LegacyWorldCoordinate2D>& expected_intersections_ref);
				/*! @brief Needed for unit tests. An object of the type PolygonInfo can only be created in an environment that has all the
				           incredients that are necessary. Since in Mask all the incredients are available and since there is no other environment, where this is the case,
				           since PolygonInfos are sofar not created anywhere else, this function was created, just in order to create and return a
				           PolygonInfo object for
				           test purposes. So this function creates an object of the type PolygonInfo and retunrns a pointer to it.
				           The PolygonInfo is created either for a voxel with cordinates aDoseIndex, given as a parameter
				           or alternatively for a voxel with greater
				           z-position in case there is no contour in the slice of the given location. In case there is no contour in any
				           slice with greater z-position either, the PolygonInfo can not be created. In that case a nullpointer is returned.
				    @param aDoseIndex Position for which the PolygonInfo object is to be created.
				    @return Returns an object of the type PolygonInfo in case a polygon of the structure can be found. Otherways it returns a NULL
				            pointer.
				*/
				PolygonInfo* JustCreateAPolygonInfoForTest(LegacyDoseVoxelIndex3D aDoseIndex);
				/*! @brief Needed for unit tests. Checks for a specific voxel, whether the correct fraction of this voxel was recognized to be
				           enclosed by the structure.
				    @param another_dose_index parameter of type LegacyDoseVoxelIndex3D which specifies the voxel coordinate of interest.
				    @param the_correct_result is a float that specifies the expected result.
				    @return Returns true in case Mask did well and the calculated fraction meets the expectation.
				*/
				bool CheckFractionThisVoxelCorrectForTest(LegacyDoseVoxelIndex3D another_dose_index ,
				        float the_correct_result);
				/*! @brief Needed for unit tests in order to access the fraction of a specific voxel that was determined to be enclosed by the structure.
				    @param  aDoseIndex is a reference to a LegacyDoseVoxelIndex3D which specifies the voxel position of interest.
				    @return Returns a float that holds the informaton about the portion of the voxel which is enclosed by the structure.
				*/
				float GetFractionThisVoxelForTest(LegacyDoseVoxelIndex3D another_dose_index);
				/*! @brief Needed for unit tests. Returns true in case the voxel specified by aDoseIndex is touched by the structure.
				    @param  aDoseIndex is a reference to a LegacyDoseVoxelIndex3D which specifies the voxel position of interest.
				    @return Returns true in case the voxel specified by aDoseIndex is touched by the structure.
				*/
				bool DoubleCheckIsOnBorderForTest(LegacyDoseVoxelIndex3D& aDoseIndex);
				/*! @brief Needed for unit tests. The parameter aDoseIndex is set to a voxel that is touched by the structure.
				    @param aDoseIndex is a reference to a LegacyDoseVoxelIndex3D which is modified by this function. If things go well,
				           it is set to a voxelposition touched by the structure. In that case the function returns true.
				    @return Returns false in case no voxel is touched by the structure. Otherways returns true.
				*/
				bool GetSomeBorderVoxelXYZForTest(LegacyDoseVoxelIndex3D& aDoseIndex);
				// End functions and other stuff needed for unit tests



			protected:

				typedef std::vector<IntersectionsThisVoxel>* VoxelIntersectionsVectorPointer;

				// @brief withSubVoxelFraction If true, the mask will be calculated with sub voxel fraction info; otherwise, without
				//  sub voxel fraction info, that means getDoseVoxelInStructure() returns the DoseVoxel with getProportionInStr()= 1.
				//
				//bool _withSubVoxelFraction;

				/// the dose iterator
				DoseIteratorInterface* _doseIter;
				/// the structure
				StructureLegacy* _structure;

				/// vector of the RTDoseVoxels inside the structure
				std::vector<DoseVoxel> _doseVoxelInStructure;

				/// vector of the dose values corresponding to _doseVoxelInStructure, it has the same size as _doseVoxelInStructure
				std::vector<DoseTypeGy> _doseData;

				/// geometric information about the dose distribution
				std::vector< core::GeometricInfo > GInf;

				//VoxelIntersectionPointsMap voxel_intersectionPoints;

				/// Once they are determined to be inside voxels get this brightness.
				field_content_type brightness_inside;
				/// Once they are determined to be touched by the structure voxels get this brightness.
				field_content_type brightness_border;
				/// Once they are determined to be outside voxels get this brightness.
				field_content_type brightness_outside;
				/// Voxels that are yet not determinded to be inside the structure, outside the structure or touched by the structure are marked with this brightness.
				field_content_type brightness_unknown;

				/// Calculates the mask.
				void Calc();
				/// Just for test. Doublechecks and does assert(0) in case something has gone wrong.
				void TestVoxelMaskUppward();

				/*! @brief This function is made for assertation. The voxelization is done on several resultution levels as explained above.
				           There are situations throughout the algorithm where it is known, that a specific affiliation of one voxel on one resolution
				           level does determine its affiliation on other resolution levels. The job of this function is to check that and to
				           assert, it terminates the program deliberately if something has happened, that definitely must never happen.
				    @param resolution_level_in Integer that species the current resolution level.
				    @param index_internal Parameter of the type LegacyUnsignedIndex3D which specifies the position in the voxel coordinate system
				                          on the current resolution level that is to be checked throughout all finer levels.
				    @param brightness Integer that specifies the brightness that is expected throughout all the finer resolution levels.
				*/
				void CheckUppwardBlockSameIntensity(int resolution_level_in , LegacyUnsignedIndex3D index_internal ,
				                                    int brightness);
				/*! @brief This function sets a brightness of a voxel throughout resolution levels, since the Voxelisierung is done on
				           several resultution levels as explained above.
				    @param resolution_level_in Integer that species the current resolution level.
				    @param index_internal Parameter of the type LegacyUnsignedIndex3D which specifies the position in the voxel coordinate system
				                          on the current resolution level that is to be set at first and then throughout all finer levels
				                          adapted to the finer voxel coordinate sysetem.
				    @param brightness Integer that specifies the brightness that is to be set on the current and throughout all the finer resolution levels.
				*/
				void SetUppwardBlockThisIntensity(int resolution_level_in , LegacyUnsignedIndex3D index_internal ,
				                                  int brightness);
				/*! @brief This function is made for assertation. The voxelization is done on several resultution levels as explained above.
				           There are situations where a specific affiliation of one voxel on one resolution
				           level does determine affiliation of at least one voxel on any finer resolution
				           level to the same intensity, within a specific block of voxels that corresponds to the voxel in position index_internal
				           on the croase level.
				           The job of this function is to check whether
				            there is any such voxel on every finer resolution level, within the respective block.
				            So this function terminates the program deliberately if something has happened, that definitely must never happen.
				    @param resolution_level_in Integer that species the current resolution level.
				    @param index_internal Parameter of the type LegacyUnsignedIndex3D which specifies the position in the voxel coordinate system
				                          on the current resolution level that is to be checked throughout all finer levels.
				    @param brightness Integer that specifies the brightness that is expected throughout all the finer resolution levels.
				*/
				void CheckUppwardOneOutOfThisBlockHasSameIntensity(int resolution_level_in ,
				        LegacyUnsignedIndex3D index_internal , int brightness);
				/*! @brief Calculates for some resolution level the number of voxels of the finest resolution level, that fit into this
				           voxel one dimensionally. So this is a number of fine voxels that fit into this croase voxel along the length of this croase voxel.
				    @param  resolution_level Integer that specifies the resolution level of the croase voxel. Whereas zero is the finest level, one the next
				            croase level with a total number of voxels of just one quater of the next finer level .... and so on ....
				    @return Returns a double that holds the information about the block length. Actually it should be an integer, but a double works, too ...
				*/
				double GetBlockLengthThisResolutionLevel(int resolution_level);
				/*! @brief  Checks the structure, so checks all polygons of the structure, on intersections with themselves.
				    @return Returns true, if the structure does not contain self intersecting polygons and thus is fine.
				*/
				bool ContourFine();
				/*! @brief Checks two polygons for intersection with one another. Checks one polygon for intersection with itself, in case
				            struct_index_a  and struct_index_b  are the same.
				    @param struct_index_a Integer that specifies the index of the first polygon that is to be checked for intersections.
				    @param struct_index_b Integer that specifies the index of the second polygon that is to be checked for intersections.
				    @return Returns true if the polygons do intersect (or if the polygon does intersect in case of a check for self intersection).
				*/
				bool DoesIntersect(int struct_index_a , int struct_index_b) const;
				/*! @brief Checks two lines for intersection.
				    @param firstPoint Paramter of type LegacyWorldCoordinate3D that specifies the beginning of the first line.
				    @param secondPoint Paramter of type LegacyWorldCoordinate3D that specifies the end of the first line.
				    @param thirdPoint Paramter of type LegacyWorldCoordinate3D that specifies the beginning of the second line.
				    @param fourthPoint Paramter of type LegacyWorldCoordinate3D that specifies the end of the second line.
				    @return Returns true in case there is an intersection.
				*/
				bool HasIntersectionsThisPair(const LegacyWorldCoordinate3D& firstPoint ,
				                              const LegacyWorldCoordinate3D& secondPoint,  const LegacyWorldCoordinate3D& thirdPoint ,
				                              const LegacyWorldCoordinate3D& fourthPoint) const;

				/*! Holds the masks for different resolution levels.
				To be initialized in Constructor by an initialization function.
				*/
				std::vector< rttb::masks::legacy::FieldOfScalar<field_content_type>* > MaskField;

				/*! Two dimensional field.
				Content of one element holds the information about the intersections of the structure with
				the voxel edges of the corresponding voxel. The object, that holds this intersecton information
				for the voxel in position x,y within the slice, is located in the position x,y within FieldOfIntersections.
				FieldOfIntersections is created for just one slice - the slice which Mask is currently working on.
				*/
				rttb::masks::legacy::FieldOfPointer<VoxelIntersectionsVectorPointer>* FieldOfIntersections;

				bool do_detailed_subvox; /// If do_detailed_subvox is true, the Mask is calculated with sub-voxel accurracy.

				/*! To be initialized in Constructor by an initialization function.
				  Holds the information about the structure and its Intersections with the voxel edges.
				*/
				std::vector<StructureInTermsOfIntersections>
				Intersections; ///Holds the information about the structure and its Intersections with the voxel edges.

				/*! @brief Fills _doseVoxelInStructure with DoseVoxel objects. Each of these DoseVoxel objects carries the information about the
				           portion of the voxel that is enclosed by the structure. The DoseVoxel objects are created based on the Mask
				           with the finest resolution. Based on this Mask field it is clear that the voxel is completely inside the structure or
				           completely outside in most of the cases.
				           SetDoseVoxelInStructureVector calls GetFractionInside() and thus determines the fraction
				           of the voxel that is enclosed by the structure in case the voxel is touched by the structure and thus
				           not yet determined to be completely inside or outside.
				*/
				void SetDoseVoxelInStructureVector();


				/*! @brief Old stuff.
				*/
				void SeperateRegions();

				/*! @brief  Old stuff.
				*/
				void SeperateRegionsQuick();

				/*! @brief Old Stuff.
				*/
				void ConnectArea(field_content_type brightness_from , field_content_type brightness_to ,
				                 LegacyUnsignedIndex3D start_index, LegacyDoseVoxelIndex2D index_begin,
				                 LegacyDoseVoxelIndex2D index_end);   // To be called by SetDoseVoxelInStructureVector

				/*! @brief To be initialized by an init function that is called by Constructor. Holds the information about how the polygons correspond to the slices.
				*/
				std::vector<correspoinding_structure_vector> SliceStructureCorrespondency;


				/*! @brief To be initialized by an init fuction which is called by Constructor. Contains the information about which area of the image is actually to be regarded. A large part of the image may be far off from any polygon.
				*/
				vector_of_area_vectors MarkInterestingArea;


				/*! @brief Is called by the constructor and calls other initialization fucntions.
				*/
				void Init();


				/*! @brief Searches backward in Intersections until a point is reached that differs from compare_point.
				           point_index is set to the index of this point.
				    @param struct_index Reference to an integer that specifies the polygon of interest.
				    @param point_index Reference to an integer that is modified by this function.
				           First it specifies the index of the point where the function stearts searching.
				           It is decremented during the search and in the end it points to the first point within the search direction
				           that turned out to be different from compare_point.
				    @param compare_point Parameter of the type LegacyWorldCoordinate3D. Specifies a point. This function searches for a point
				           which differs from compare_point.
				    @return Returns true in case such a point was found and false in case it was not.
				*/
				bool GetFirstDifferent(unsigned int& struct_index , unsigned int& point_index ,
				                       LegacyWorldCoordinate3D compare_point);



				/*! @brief This function determines whether the intersection  with struct_index,  point_index and intersect_index
				           is to be set to the FieldOfIntersections. It is to be set in case the intersection with the voxeledge is either
				           not a point of the polygon, or in case it is ( that means the point of the polygon is on the edge of a voxel )
				           and at the same time the previous point of the polygon has one identical coordinate in a way so that it is located on
				           the same edge, too (eventually alongside another voxel, so the same edge here means "same column seperating voxels").
				    @param struct_index Index of the polygon.
				    @param point_index  Index of the point within the polygon (last one between the intersection that is currently being processed).
				    @param intersect_index  Index of the intersection that is currently being processed.
				    @param index_x X-position of the voxel. The element of FieldOfIntersectons that belongs to this voxel is calculated right now.
				    @param index_y Y-position of the voxel. The element of FieldOfIntersectons that belongs to this voxel is calculated right now.
				    @return Retruns true in case the intersection with struct_index,  point_index and intersect_index is to be set to the
				            FieldOfIntersections.
				*/
				bool IsItToBeRegarded(unsigned int struct_index , unsigned int point_index ,
				                      unsigned int intersect_index , int index_x , int index_y);



				/*! @brief This function calls SetThisIntersection with the respective parameters and therefore takes part in positioning the intersection characterized by the
				           parameter intersection within the FieldOfIntersections.
				    @param  index_x X-position of the voxel.
				    @param  index_y Y-position of the voxel.
				    @param  intersection Specifies the intersection that is currently regarded (similar to an index, specifies location where the intersection can be found within the object Intersections).
				    @param  coord2D Voxel coordinate of the intersection.
				    @param  edge_number Denotes the edge which the intersection is on.
				    @param  voxel_intersections The element within FieldOfIntersections that corresponds to this voxel and polygon.
				*/
				void SetThisIntersectionToCornerOrEdge(unsigned int index_x , unsigned int index_y ,
				                                       IterativePolygonInTermsOfIntersections::WhichIntersection intersection ,
				                                       LegacyWorldCoordinate2D coord2D, unsigned int edge_number,
				                                       IntersectionsThisVoxel& voxel_intersections);


				/*! @brief This function positions an intersection within voxel_intersections. Practically voxel_intersections is an element within FieldOfIntersections (generally it can be any reference to an IntersectionsThisVoxel ).
				           The voxel edge the intersection is placed to is given by edge_number.
				    @param corner_of_interest The corner at the beginning of the respective edge. Beginning means the point of the edge that is first passed while surrounding the voxel clockwise.
				    @param corner_next The corner at the end of the respective edge. That means the point along the edge that is passed last while surrounding the voxel clockwise.
				    @param corner_fixed Its the coordinate, which is the same all along the edge.
				    @param intersection Denotes the index of the regarded intersection in Intersections.
				    @param coord_of_interest The coordinate which is interesting, because it is responsible for the position of the intersection along the edge, while the other coordinate is just the same for the intersection as well as for all the points that belong to the edge.
				    @param coord_fixed This coordinate should equal to corner_fixed. The function asserts for that. Its the coordinate that all the points on the edge and the intersection point have in common.
				    @param voxel_intersections Practically its an element of FieldOfIntersections (although generally it can be any element of IntersectionsThisVoxel).
				    @param edge_number Number of the edge that is regarded. The edges are counted colckwise starting from upper left.
				*/
				void SetThisIntersection(unsigned int corner_of_interest, unsigned int corner_next ,
				                         unsigned int corner_fixed ,  IterativePolygonInTermsOfIntersections::WhichIntersection intersection
				                         , double coord_of_interest , double coord_fixed, IntersectionsThisVoxel& voxel_intersections ,
				                         unsigned int edge_number);


				/*! @brief This function checks whether value_to_compare is in between small_value_to_compare_with and big_value_to_compare_with. Practically value_to_compare is a coordinate of an intersection point. Practically big_value_to_compare_with is the end point of an edge.
				           In case value_to_compare is in between, the index of the intersection  (that characterizes its location within Intersections (Intersections is a global variabele, a vector of StructureInTermsOfIntersections) ) and its coordinates are appended to the corresponding edge within voxel_intersections.
				    @param value_to_compare This value is to be compared with other values and inserted into voxel_intersections at the right place.
				    @param small_value_to_compare_with First value to compare with.
				    @param big_value_to_compare_with Second value to compare with.
				    @param edge_number Number of the edge. Edges are counted clockwise, starting from upper left.
				    @param intersection Index of the intersection within Intersections.
				    @param voxel_intersections Object of type IntersectionsThisVoxel that holds the information about all the intersections of the structure with this currently regarded voxel as well as the information where to find these intersection within Intersections
				    @return Retruns true in case insertation was successful.
				*/
				bool CompareIfBetweenAppend(double value_to_compare , double small_value_to_compare_with ,
				                            double big_value_to_compare_with, unsigned int edge_number,
				                            IterativePolygonInTermsOfIntersections::WhichIntersection intersection ,
				                            IntersectionsThisVoxel& voxel_intersections);


				/*! @brief This function checks whether the intersection point is already known.
				           In case it is the index of the intersection within Intersections is appended to the already existing vector of points of the respective edge that all represent this specific intersection point (in case the structure goes through this point several times touching itself, otherways ist just one)
				           (Intersections is a global variable, a vector of StructureInTermsOfIntersections which contains the information about all the intersections with all the voxel edges on all resolutions).
				           In case it does not exist yet a new vector of indices as well as coordinates is created and inserted in the respective position.
				           In case the same intersection point will be found again later on it will be appended then.
				    @param value_to_compare Coordinate of the point to be inserted.
				    @param edge_number Number of the edge. The edges are counted clockwise, starting from upper left.
				    @param intersection Index of the intersectoin within Intersections.
				    @param voxel_intersections An object representing the information about the intersections of the structure with this voxel, including the information about coordinates of the intersection points as well as indicees that represent the information about where to find them within Intersections.
				    @return Returns true in case of a successful insertation.
				*/
				bool CheckIfIndenticalOrBetweenRespectiveVectorElementsAndDoInsert(double value_to_compare ,
				        unsigned int edge_number, IterativePolygonInTermsOfIntersections::WhichIntersection intersection ,
				        IntersectionsThisVoxel& voxel_intersections);


				/*! @brief See CompareIfBetweenAppend - the only difference is, that this function inserts the intersection at the beginning of the edge and that practically small_value_to_compare_with is the first point within the respective edge while big_value_to_compare_with is either the first intersection that has aleready been found, or the last point within the edge.
				    @return Returns true in case the intersection was inserted into  voxel_intersections.
				*/
				bool CompareIfBetweenInsertAtBeginning(double value_to_compare ,
				                                       double small_value_to_compare_with , double big_value_to_compare_with, unsigned int edge_number,
				                                       IterativePolygonInTermsOfIntersections::WhichIntersection intersection ,
				                                       IntersectionsThisVoxel& voxel_intersections);


				/*! @brief  Old stuff.
				*/
				void SetThisIntersectionXIncreasing(unsigned int index_x , unsigned int index_y ,
				                                    IterativePolygonInTermsOfIntersections::WhichIntersection intersection ,
				                                    LegacyWorldCoordinate2D coord2D , IntersectionsThisVoxel& voxel_intersections ,
				                                    unsigned int edge_number);
				/*! @brief Old stuff.
				*/
				void SetThisIntersectionYIncreasing(unsigned int index_x , unsigned int index_y ,
				                                    IterativePolygonInTermsOfIntersections::WhichIntersection intersection ,
				                                    LegacyWorldCoordinate2D coord2D , IntersectionsThisVoxel& voxel_intersections);
				/*! @brief Old stuff.
				*/
				void SetThisIntersectionXDecreasing(unsigned int index_x , unsigned int index_y ,
				                                    IterativePolygonInTermsOfIntersections::WhichIntersection intersection ,
				                                    LegacyWorldCoordinate2D coord2D , IntersectionsThisVoxel& voxel_intersections);
				/*! @brief Old stuff.
				*/
				void SetThisIntersectionYDecreasing(unsigned int index_x , unsigned int index_y ,
				                                    IterativePolygonInTermsOfIntersections::WhichIntersection intersection ,
				                                    LegacyWorldCoordinate2D coord2D , IntersectionsThisVoxel& voxel_intersections);


				/*! @brief This function determines the area that contains voxels that need to be regarded.
				           For example voxels within x-coordinate lower than the contour pont with lowest x-coordinate do not need to be regarded, since they can
				           not be enclosed by the contor. Of course the same goes for other coordintates and ...
				*/
				void InitInterestingArea();
				/*! @brief Initializes MaskField which contains the masks fields for different resolution levels and the corresponding geometric infromation stored in GInf.
				           Different sizes are initialized, representing the different resolutions. Calls InitIntersections.
				*/
				void InitMultiResolution();
				/*! @brief Initializes Intersections which is a vector of StructureInTermsOfIntersections and holds the information about the intersections between voxel grid and structure for different resolution levels.
				           This function just initializes, but yet does not calculate the intersections, so here Intersectons is created, but remains empty.
				*/
				void InitIntersections();

				/*! @brief  This function is called to calculate the intersections between structure and voxel edges and thus to fill
				            Intersections with intersecton values, which is a vector of StructureInTermsOfIntersections and holds the information about the intersections between voxel grid and structure for different resolution levels.
				    @param  resolution_level Specifies for which voxel grid the intersections are to be calculated. In case it is zero it is done for the
				                             finest resolution and the values are filled into the first element of Intersections.
				*/
				void GetIntersections(int resolution_level);

				/*! @brief  This function fills intersections_raw_and_column_ref with intersection values. It operates on a segment of a polygon consisting of a line that connects a first and a second point.
				            The intersections of this polygon segment with the voxel edges on the resolution_level are already determined and stored in
				            raw_intersections_ref, which contains the intersections with raw-eges as well as
				            column_intersections_ref which holds the intersections with the voxel edges regarding columns.
				            In intersections_raw_and_column_ref they are sorted with respect to their distance
				            from firstPoint and in the end intersections_raw_and_column_ref contains all the intersections - colum and raw intersections.
				    @param  firstPoint Starting point of the segment of the polygon.
				    @param  raw_intersections_ref Contains the raw intersections sorted with resepect to their distance from firstPoint.
				    @param  column_intersections_ref Contains the column intersections sorted with resepect to their distance from firstPoint.
				    @param  intersections_raw_and_column_ref After running this function intersections_raw_and_column_ref contains all intersections sorted with resepect to their distance from firstPoint.
				    @param  resolution_level The resolution level. Zero is finest.
				*/
				void UnifyRawAndColumn(LegacyWorldCoordinate3D firstPoint,
				                       std::vector<LegacyWorldCoordinate2D>& raw_intersections_ref ,
				                       std::vector<LegacyWorldCoordinate2D>& column_intersections_ref ,
				                       std::vector<LegacyWorldCoordinate2D>& intersections_raw_and_column_ref, int resolution_level);


				/*! @brief  This function calculates the intersections between a line and the horizontal voxel edges (voxel edges in raw direction).
				            The line starts with firstPoint and ends with secondPoint.
				    @param  firstPoint Point to start the line.
				    @param  secondPoint point to end the line.
				    @param  raw_intersections_ref The intersections are placed here.
				    @param  resolution_level The current resolution.
				*/
				void GoIntersectRaw(LegacyWorldCoordinate3D firstPoint , LegacyWorldCoordinate3D secondPoint ,
				                    std::vector<LegacyWorldCoordinate2D>& raw_intersections_ref , int resolution_level);


				/*! @brief  This function calculates the intersections between a line and the vertical voxel edges (voxel edges in column direction).
				            The line starts with firstPoint and ends with secondPoint
				    @param  firstPoint Point to start the line.
				    @param  secondPoint point to end the line.
				    @param  column_intersections_ref The intersections are placed here.
				    @param  resolution_level The current resolution.
				*/
				void GoIntersectColumn(LegacyWorldCoordinate3D firstPoint , LegacyWorldCoordinate3D secondPoint ,
				                       std::vector<LegacyWorldCoordinate2D>& column_intersections_ref, int resolution_level);


				/*! @brief This function calculates the intersections between a line and the horizontal voxel edges (voxel edges in raw direction).
				            The line starts with firstPoint and ends with secondPoint.
				            This function is called in case the x-position of firstPoint and secondPoint are identical.
				    @param  firstPoint Point to start the line.
				    @param  secondPoint point to end the line.
				    @param  raw_intersections_ref The intersections are placed here.
				    @param  resolution_level The current resolution.
				*/
				void GoGetRawIntersectionsAlongThisColumn(LegacyWorldCoordinate3D firstPoint ,
				        LegacyWorldCoordinate3D secondPoint , std::vector<LegacyWorldCoordinate2D>& raw_intersections_ref,
				        int resolution_level);


				/*! @brief  This function calculates the intersections between a line and the vertical voxel edges (voxel edges in column direction).
				            The line starts with firstPoint and ends with secondPoint. This function is called only in case the y-position of first and of
				            second point are identical.
				    @param  firstPoint Point to start the line.
				    @param  secondPoint point to end the line.
				    @param  column_intersections_ref The intersections are placed here.
				    @param  resolution_level The current resolution.
				*/
				void GoGetColumnIntersectionsAlongThisRaw(LegacyWorldCoordinate3D firstPoint ,
				        LegacyWorldCoordinate3D secondPoint ,
				        std::vector<LegacyWorldCoordinate2D>& column_intersections_ref, int resolution_level);


				/*! @brief This function prints out all the intersections with cout. Normally its not called, but sometimes its needed for debugging.
				*/
				void ShowIntersections();


				/*! @brief Based on the global variable Intersections that holds the information about the intersections betweent the voxel edges and
				           the polygons this function determines all voxels that are touched by the structure on  resolution_level. So this operation
				             is carried out for the mask field that belongs to  resolution_level. For  resolution_level equal to zero its the finest
				             resolution and so the resolution of the mask field is the resolution of the dose distribution.
				*/
				void MarkVoxelsTouchedByStructure(int resolution_level);


				/*! @brief Sets FieldOfIntersections for the current slice. This is done based on Intersections which stores the information about the
				           polygons and their intersections.
				    @param  index_z Integer that represents the index of the slice which is to be regarded.
				*/
				void GetIntersectionInformationThisSlice(unsigned int index_z);


				/*! @brief The intersection with voxel coordinates coord2D is inserted into
				           the FieldOfIntersectons in position  of voxel (index_x , index_y , index_z) and for the polygon with index struct_index
				           and for the side of the voxel denoted by voxel_side.
				    @param index_x X-position of the voxel.
				    @param index_y Y-position of the voxel.
				    @param index_z Z-position of the voxel.
				    @param struct_index Index of the polygon that is intersecting here.
				    @param point_index Index of the last polygon point before the intersection.
				    @param intersect_index Index of the intersection. By the way also represents the information about how many intersections with voxel edges are in between this intersection with voxel edge and the last polygon point.
				    @param coord2D Intersection in voxel coordinates.
				    @param voxel_side Side of the voxel which is intersected here.
				*/
				void AddToFieldOfIntersections(unsigned int index_x , unsigned int index_y , unsigned int index_z ,
				                               unsigned int struct_index , unsigned int point_index , unsigned int intersect_index ,
				                               LegacyWorldCoordinate2D coord2D, unsigned int voxel_side);


				/*! @brief  A new element of the type VoxelIntersectionsVectorPointer is inserted into the FieldOfIntersections and a first element
				            of type IntersectionsThisVoxel is inserted.
				    @param  index_x X-position of the voxel of interest, which needs specification of its intersections.
				    @param  index_y Y-position of the voxel of interest, which needs specification of its intersections.
				    @param  index_z Z-position of the voxel of interest, which needs specification of its intersections.
				    @param  struct_index Specifies the specific polygon and its respective PolygonInTermsOfIntersections which intersects here.
				*/
				void ExtendFieldOfIntersections(unsigned int index_x , unsigned int index_y , unsigned int index_z ,
				                                unsigned int struct_index);


				/*! @brief In case FieldOfIntersections does already have an element of type VoxelIntersectionsVectorPointer in the position that
				           corresponds to the voxel in index_x , index_y , index_z,
				           the index of the IntersectionsThisVoxel element that corresponds to the polygon with index struct_index is returned.
				    @param  index_x X-position of the voxel of interest.
				    @param  index_y Y-position of the voxel of interest.
				    @param  index_z Z-position of the voxel of interest.
				    @param  struct_index Index of the polygon of interest.
				    @return Returns the index of the IntersectionsThisVoxel element that corresponds to the polygon with index struct_index.
				*/
				int GetRespectiveIntersections(unsigned int index_x , unsigned int index_y , unsigned int index_z ,
				                               unsigned int struct_index) const;


				/*! @brief  This function calculates and returns the fraction of the voxel with index aDoseIndex that is enclosed by the strucure.
				            This fraction is between zero and one.
				    @param  aDoseIndex Index of the voxel of interest.
				    @return Returns a number between zero and one that specifies the fraction of the voxel that is enclosed by the structure.
				*/
				float GetFractionInside(const LegacyDoseVoxelIndex3D& aDoseIndex) ;


				/*! @brief  Gets a number of voxels that are known to have oldcolor via indexList. Takes a voxel from this list. Lets call it THEVOXEL.
				            Checks the neighbours of THEVOXEL. Each neighbour that does have oldcolor is appended to the list for further
				            investigation. Afterwards THEVOXEL is set to newcolor and taken from the list. Now the next voxel of the list is regarded ...
				            ... so it is now THEVOXEL ... and so on ... until the list is empty.
				    @param  indexList List of indicees that are known to have oldcolor.
				    @param  oldcolor The old color.
				    @param  newcolor The new color.
				    @param  resolution_level  The resolution level which is currently regarded.
				*/
				void FloodFill4(UnsignedIndexList indexList , int oldcolor, int newcolor, int resolution_level);


				/*! @brief  Old stuff.
				*/
				void setInsideVoxelPreviousVersion(int resolution_level);


				/*! @brief Goes through MaskField over all resolution levels and sets all the mask voxels to the brightness that characterizes
				           those voxel that are not known to be outside, inside or border. Those voxels that do already have the brightness that
				           characterizes them as border are not changed here.
				*/
				void SetContentUnknown();


				/*! @brief This function calls FloodFill4 for those voxels that are known to be inside or outside although yet not denoted so in the
				            mask field. FloodFill4 will first investigate the neighbourhood of these voxels and then set them to the correct value.
				    @param  resolution_level The resolution that is currently regarded.
				    @param  indexListInside Voxels known to be inside, but yet not denoted so in the mask field.
				    @param  indexListOutside Voxels known to be outside, but yet not denoted so in the mask field.
				*/
				void setInsideVoxel(int resolution_level, std::vector< UnsignedIndexList > indexListInside,
				                    std::vector< UnsignedIndexList > indexListOutside);


				/*! @brief This function is just for debugging. It prints out the number of voxels with specific brightness.
				    @param resolution_level The resolution_level of interest.
				*/
				void CountThisColor(int resolution_level);


				/*! @brief First this function determines the voxels that are yet not denoted inside or outside in the mask field,
				           but are directly in touch with voxels which are already known to be inside/outside and thus their affiliation is clear.
				           This is done for the resolution resolution_level. GetBorderRegion of Field.h is called to do this job,
				           if the resolution is not croasest. In case the resolution is croasest, this step is omitted.
				           In case of the croasest resolution this step is not necessary and thus omitted since
				           on the croasest resolution no voxel is yet known to be inside or outside.
				           Afterwards setInsideVoxel is called by
				           this function.
				    @param resolution_level The resolution level which is supposed to be regarded here.
				*/
				void setInsideVoxelMultiRes(int resolution_level);


				/*! @brief Transforms world coordinate in voxel coordinate.
				    @param z The world coordinate to be tranformed.
				    @return Returns the resulting voxel coordinate.
				*/
				LegacyWorldCoordinate1D GetZIndex(LegacyWorldCoordinate1D z);


				/*! @brief Provides _doseData with the absolute dose data of the voxels which are inside the structure.
				*/
				void calcDoseData();


				// private stuff that is just for test:
				/*! @brief Needed for unit tests. The parameter aDoseIndex is set to a voxel that is touched by
				           the structure and which is located in the same slice that is given by the z-position of aDoseIndex, if possible.
				    @param aDoseIndex is a reference to a LegacyDoseVoxelIndex3D which is modified by this function in x and y, but not in z.
				           If things go well, aDoseIndex is set to a voxelposition within the slice in position z that is touched
				           by the structure. In that case the function returns true.
				    @return Returns false in case no voxel is touched by the structure within this slice. Otherways returns true.
				*/
				bool GetSomeBorderVoxelXYForTest(LegacyDoseVoxelIndex3D& aDoseIndex);
				// end private stuff just for test


			private:
				void clearMaskField();


			};
		}

	}
}







#endif