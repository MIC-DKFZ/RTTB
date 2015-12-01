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
#ifndef __PolygonInfo_H
#define __PolygonInfo_H

#include "rttbBaseType.h"
#include "rttbGeometricInfo.h"
#include "rttbStructure.h"
#include "rttbDoseIteratorInterface.h"
#include "rttbField_LEGACY.h"
#include "rttbMaskType_LEGACY.h"
#include "rttbContour_LEGACY.h"
#include "rttbIterativePolygonInTermsOfIntersections_LEGACY.h"
#include <math.h>
#include <deque>



namespace rttb
{
	namespace masks
	{
		namespace legacy
		{



			typedef std::vector< std::vector<  std::vector< IterativePolygonInTermsOfIntersections::WhichIntersection > > >
			edge_intersections_intersection_coord_type;
			typedef std::vector< std::vector< IterativePolygonInTermsOfIntersections::WhichIntersection > >
			intersection_coord_type;
			typedef std::vector< std::vector<  IterativePolygonInTermsOfIntersections::WhichIntersection > >::iterator
			intersection_coord_iterator_type;

			struct VoxelIntersectionIndex
			{

			public:

				unsigned int next_intersection;
				unsigned int intersection_douplication;

				VoxelIntersectionIndex()
				{
					next_intersection = 0;
					intersection_douplication = 0;
				}

				void reset()
				{
					next_intersection = 0;
					intersection_douplication = 0;
				}


			};


			struct IntersectionsThisVoxel
			{

			public:

				IntersectionsThisVoxel()
				{
					polygon_number = -1;
					corner_intersections_intersection_coord.resize(4);
					edge_intersections_intersection_coord.resize(4);
					edge_intersections_vox_coord.resize(4);
				}

				~IntersectionsThisVoxel() {};

				int GetPolygonNumber()
				{
					return polygon_number;
				}

				void SetPolygonNumber(int polygon_number_in)
				{
					polygon_number = polygon_number_in;
				}


				std::vector< std::vector<  std::vector< double > > > edge_intersections_vox_coord;
				intersection_coord_type corner_intersections_intersection_coord;
				edge_intersections_intersection_coord_type edge_intersections_intersection_coord;

			private:

				int polygon_number;

			};







			// Die folgende Klasse handhabt die konkrete Speicherung der Schnittpunkte.
			// Kennt sich also mit der Speicherung der Schnittpunkte der Konturen mit den Voxelkanten aus.
			// Information auf Struktur-Polygonebene.
			// Nimmt PolygonInfo den Zugriff auf diese Information ab.





			// PolygonInfo: Handhabung der Polygonschnipsel die ein Voxel anschneiden :
			// Information auf Voxelebene.
			// Speichert diese derart, dass die Schnittpunkte mit der Voxelkante in Uhrzeigersinn-Reihenfolge repraesentiert sind.
			// Berechnet daraus dann den Anteil des Voxels, der Innerhalb der betrachteten Kontur liegt.

			// PolygonInfo ist ein Objekt, das alle noetigen Informationen zur Berechnung des Polygonsets enthaelt und sinnvoll
			//strukturiert. Es berechnet dann anhand der Gesamtheit der schneidenden Polygonschnipsel den Innenanteil.

			class PolygonInfo
			{


			public:




				PolygonInfo() {};
				PolygonInfo(const LegacyPolygonType& the_polygon_in, int polygon_number_in,
				            field_content_type brightness_outside_in, field_content_type brightness_inside_in ,
				            FieldOfScalar<field_content_type>* MaskFieldReceived_in ,
				            const PolygonInTermsOfIntersections& Polygon_Intersections_In ,
				            const LegacyDoseVoxelIndex3D& aDoseIndex , core::GeometricInfo& GInfIn,
				            const IntersectionsThisVoxel& intersections_this_voxel_in);

				~PolygonInfo() {};
				LegacyPolygonSequenceType& GetResultingSequence()
				{
					return resulting_sequence;
				};
				bool CreatePolygonSequence();



				struct PolygonSnippet
				{

					PolygonSnippet()
					{
						why_here.x = 0;
						why_here.y = 0;
						point_of_interest_start.x = 0;
						point_of_interest_start.y = 0;
						point_of_interest_end.x = 0;
						point_of_interest_end.y = 0;
						is_edge = 0;
						characterize = unknown;
						angle_charact = (-1);
						i_have_been_processed = false;
					}


					//Wesentlicher Punkt:
					//Verantwortlich fuer die Position dieses Snipplets an dieser Stelle Position der Kante
					//(die Snipplets sollen anhand ihrer Schnittpositionen entlang der Kante eingeordnet werden und es soll
					//hier gekennzeichnet werden, welcher der beiden Schnittpunkte oder Beruehrpunkte fuer die Einordnung and dieser Stelle wesentlich war.
					//Ein Polygonsnippet kommt in den Kantenvektoren jeweils doppelt vor, denn es hat zwei Schnitt/Beruehrpunkte.
					//Einer der beiden begrenzenden Schnittpunkte/Beruehrpunkte mit der Voxlekante kann auch der Eckpunkt sein.
					//In vielen Faellen wird der Eckpunkt jedoch  selbst kein Beruehr/Schnittpunkt sein.
					LegacyWorldCoordinate2D why_here;

					// Anfangsschnittpunkt
					LegacyWorldCoordinate2D point_of_interest_start;

					// Endschnittpunkt
					LegacyWorldCoordinate2D point_of_interest_end;

					// Polygonstueckchen zwischen den Schnitt-/Beruehrpunkten
					std::vector<LegacyWorldCoordinate3D> snippet_intermediate_content;

					bool i_have_been_processed;

					// handelt es sich um eine Ecke?
					bool is_edge;

					//characterize ist ein Integer:
					//sagt, ob es sich hier

					//a) um einen wahren Schnittpunkt handelt
					// drinnen drausen Wechsel .
					//Sein Wert ist dann 0.
					//Drinnen Drausen Wechsel geschieht.

					//b) um einen  Beruehrpunkt vom Typ 1 :
					// Beide Snippets ganz auf der Kante kein Wechsel .
					//Kontur und Voxelkante beider angrenzenden Polygonsnipplets parallell und deckungsgleich, die Voxelkante wird von beiden zugehoerigen
					//Polygonsnippets nicht verlassen
					//Sein Wert ist dann 1
					//Ein Drinnen Drausen Wechsel findet hier nicht statt.

					//c) um einen Beruehrpunkt vom Typ 2:
					//Polygon laueft halb raus. Wechsel je nachdem was folgt.
					//Das Erste der beiden angrenzenden Polygonsnipplets endet direkt auf der Voxelkante und ist nicht parallel, liegt also nicht in seinder
					//ganzen Laenge auf der Kante. Es kommt von drinnen, aus dem Voxel heraus.
					//Das zweite der Angrenzenden Polygonsnippets verlaueft auf seiner angrenzenden Seite zunaechst auf der Kante.
					//Ein Wechsel von drausen nach drinnen findet nur statt, falls im Folgenden das an das zweite Snippet
					//angrenzende nach drausen weiterverlaueft.
					//Der Wert des integers ist 2

					//d) Um einen Beruehrpunkt vom Typ 3:
					//Polygon laueft halb rein.
					//Wechsel nach drausen, je nachdem was folgt.
					//Das zweite der beiden angrenzenden Polygonsnipplets endet direkt auf der Voxelkante und ist nicht parallel, liegt also nicht in seinder
					//ganzen Laenge auf der Kante. Es Fuehrt nach drinnen ins voxel hinein.
					//Das erste der Angrenzenden  Polygonsnippets verlaueft auf seiner angrenzenden Seite zunaechst auf der Kante.
					//Ein wechsel von drinnen nach drausen findet nur statt, falls das Polygon zuletzt von drausen auf die Voxlekante zugelaufen ist.
					//Der Wert des integers ist dann 3

					//e) Um einen Beruehrpunkt vom Typ 4:
					// Polygon laueft halb rein.
					//Das Erste der beiden angrenzenden Polygonsnipplets endet direkt auf der Voxelkante und ist nicht parallel, liegt also nicht in seinder
					//ganzen Laenge auf der Kante. Es kommt von draussen auf das Voxel zu.
					//Das zweite der Angrenzenden Polygonsnippets verlaueft auf seiner angrenzenden Seite zunaechst auf der Kante.
					// Ein Wechsel findet dann statt, falls das Polygon im Folgenden nach drinnen weiter verlaueft.
					//Der Wert des integers ist dann 4

					//f) Um einen Beruehrpunkt vom Typ 5:
					//Das Zweite der beiden angrenzenden Polygonsnipplets endet direkt auf der Voxelkante und ist nicht parallel, liegt also nicht in seinder
					//ganzen Laenge auf der Kante. Es Fuehrt nach drausen aus dem Voxel heraus.
					//Das erste der Angrenzenden  Polygonsnippets verlaueft auf seiner angrenzenden Seite zunaechst auf der Kante.
					//Ein Wechsel findet dann statt, wenn das Polygon zuvor aus dem inneren des Voxels heraus kam.
					//Der Wert des integers ist dann 5


					//g) Beruehrpunkte vom Typ 6 :
					// Kein Wechsel aber eventuell ein Innenbereich zu erkennen und als Polygon anzuhaengen.
					//(Beide angrenzenden Polygonsnipplets enden auf der Voxelkante und sind zur Voxelkante nicht paralell)
					// Der Wert des integers ist dann 6.
					// Falls der eine Schnipsel von drinnen kommt und der andere nach drinnen weiterlaueft, muss man einmal im Kreis rum fahren und
					// das Polygon, das dabei eintsteht anhaengen.

					//h) Beruehrpunkte vom Typ 7 :
					// Beruehrt, beide angrenzenden Polygonstuecke drausen

					// Ecke 8
					// Ecke darf nur dann gewaehlt werden, wenn die Ecke mit sicherheit kein Schittpunkt oder Beruherpunkt mit der Kontur ist
					// CheckCornersOutside ist davon abhaengig.

					// Unklar -1.

					CharacterizeIntersection characterize;

					float angle_charact;

				};

				typedef std::vector<PolygonSnippet> snippet_vector;
				typedef std::vector<snippet_vector> PolygonInfoVectorOfEdges;
				typedef snippet_vector::iterator snippet_vector_iterator;


				enum WhichSector { not_known = -1 , on_edge_twelve_o_clock = 0 , section_twelve_to_three_o_clock = 1 , on_edge_three_o_clock = 2 , section_three_to_six_o_clock = 3 , on_edge_six_o_clock = 4 , section_six_to_nine_o_clock = 5 , on_edge_nine_o_clock = 6, section_nine_to_twelve_o_clock = 7 };
				enum EdgeStatus { unclear = -1 , inside = 0 , edge_or_outside = 1 };
				EdgeStatus edge_status;

				struct SnippetIndex
				{
					unsigned int edge;
					unsigned int snip;
				};

				bool selfTest();
				IterativePolygonInTermsOfIntersections GetIterativePolygoneInTermsOfIntersections();

				// Constructor of PolygonInfo calls function SetIntersectionsAndResetIntersectionIndex
				// which calls function CreateUnifiedListOfRawAndColumnIntersections
				// success of this function is checked here. Calculation of the intersection points is checked as well in this process.
				bool TestCreateUnifiedListOfRawAndColumnIntersections(PolygonInTermsOfIntersections
				        Expected_Unified_Polygon_Intersections);
				bool CompareCalculatedAndExpectedResultingSequenceForTest(LegacyPolygonSequenceType&
				        the_resulting_sequence_ref);

				// Checks the member pol_inf_vec  wihch is a vector that consists of four vectors of PolygonSnippets
				// representing the voxel edges.
				bool CheckResultingPolygonInfoVectorOfEdgesForTestStructure(PolygonInfoVectorOfEdges
				        pol_inf_vec_expected);
				void ReferenceIterativePolygonForTest(IterativePolygonInTermsOfIntersections& it_poly_ref)
				{
					it_poly_ref = it_poly;
				}


			private:


				void CalcSnippetVectors();
				void SetCurrentMovingProcessed();
				void AppendEdgeToVector(std::vector<LegacyWorldCoordinate3D>& vector , int edge_internal);
				bool GetCurrentMovingSnippet(PolygonSnippet& snipp_ref);
				bool CurrentSnippetIsProcessed();
				void ShowResultingSequence();
				void ShowPolygonInTermsOfIntersections();
				void ShowSnippet(PolygonSnippet a_snippet);
				void DoCharacterize(WhichSector& sector_first_point_ref ,  WhichSector& sector_second_point_ref ,
				                    CharacterizeIntersection& charact_ref , std::vector<WhichSector> anti_outside_combination_vector ,
				                    WhichSector& inside_ref , WhichSector& preceding_edge_ref , WhichSector& following_edge_ref);
				void SetDoseIndex(const LegacyDoseVoxelIndex3D& aDoseIndex);
				void AppendEdges(PolygonInfoVectorOfEdges& pol_inf_vec_ref);
				void CheckEdgeIntersections();
				void DoAppendIntermediateEdgeUpToThisSnippet(PolygonSnippet& snipp_ref ,
				        LegacyWorldCoordinate2D why_here , int the_previous_edge , int the_current_edge);
				int EdgeDist(int edge_old , int the_current_edge);
				void IncrementEdge(int& edge_increment);
				bool GetCornerFromVoxelIntersections(unsigned int the_index, SnippetIndex edge_index ,
				                                     IterativePolygonInTermsOfIntersections::WhichIntersection& intersect_index);
				int GetNumberOfSnppetsThisEdge(SnippetIndex edge_index);
				bool SetToNextIntersectonAlongThisEdgeAndReturnDistanceBasedOnVoxelIntersections(
				    LegacyWorldCoordinate2D& closest_local_current_position_ref , int current_edge,
				    VoxelIntersectionIndex next_voxel_intersection);
				snippet_vector CreateSnippetVector(int number_of_snippets_this_edge , PolygonSnippet template_snip);
				snippet_vector SortClockwise(snippet_vector snip_vec_local);
				void InsertToSnippetVectorFront(snippet_vector snip_vec_local ,  SnippetIndex edge_index);
				void RemoveFromLocalSnippetVector(snippet_vector& snip_vec_local , GridIndexType which_one);
				void CheckEdegeTouchCharacteristicsAndAppendIntermediateContent(SnippetIndex edge_index);
				void WorkWithForwardAndBackwardEdgeSnippet(int which_one, float& angle_charact_a ,
				        float& angle_charact_b , PolygonSnippet& forward_snippet_ref ,
				        PolygonSnippet& backward_snippet_ref , PolygonSnippet& edge_snippet_ref ,
				        std::vector<PolygonSnippet>& snip_vec_local , int& erased);
				void WorkWithForwardEdgeSnippet(LegacyWorldCoordinate2D edge_coord,
				                                LegacyWorldCoordinate3D contour_point_one, int which_one, float& angle_charact_a ,
				                                float& angle_charact_b , PolygonSnippet& forward_snippet_ref ,
				                                PolygonSnippet& backward_snippet_ref , PolygonSnippet& edge_snippet_ref ,
				                                std::vector<PolygonSnippet>& snip_vec_local , int& erased);
				void WorkWithBackwardEdgeSnippet(LegacyWorldCoordinate2D edge_coord,
				                                 LegacyWorldCoordinate3D  contour_point_two , int which_one, float& angle_charact_a ,
				                                 float& angle_charact_b , PolygonSnippet& forward_snippet_ref ,
				                                 PolygonSnippet& backward_snippet_ref , PolygonSnippet& edge_snippet_ref ,
				                                 std::vector<PolygonSnippet>& snip_vec_local , int& erased);



				// The following function is just for test and easy debugging.
				void GetSectorsAndSetCharacteristicsDoubleCheck(SnippetIndex edge_index,
				        LegacyWorldCoordinate2D edge_coord , LegacyWorldCoordinate3D& contour_point_one ,
				        LegacyWorldCoordinate3D& contour_point_two);
				void GetSectors(LegacyWorldCoordinate2D edge_coord , LegacyWorldCoordinate3D& contour_point_one ,
				                LegacyWorldCoordinate3D& contour_point_two, WhichSector& sector_first_point_ref ,
				                WhichSector& sector_second_point_ref);
				void GetSectorFirstPoint(LegacyWorldCoordinate2D edge_coord ,
				                         LegacyWorldCoordinate3D& contour_point_one , WhichSector& sector_first_point_ref);

				// Anmerkung : Funktion verdoppelt die obige. Kann man loeschen und ihre Aufrufe durch GetSectorFirstPoint ersetzen,
				// wobei GetSectorFirstPoint dann in GetSectorThisPoint umbenannt werden sollte.
				void GetSectorSecondPoint(LegacyWorldCoordinate2D edge_coord ,
				                          LegacyWorldCoordinate3D& contour_point_two, WhichSector& sector_second_point_ref);
				CharacterizeIntersection GetCharacteristicFirstCorner(WhichSector& sector_first_point_ref ,
				        WhichSector& sector_second_point_ref);
				void GetAntiOutsideCombinationFirstCorner(std::vector<WhichSector>&
				        anti_outside_combination_vector_ref);
				void GetInsideFirstCorner(WhichSector& inside_ref);
				void GetPrecedingEdgeFirstCorner(WhichSector& preceding_edge_ref);
				void GetFollowingEdgeFirstCorner(WhichSector& following_edge_ref);
				bool BothOnEdge(WhichSector& sector_first_point_ref ,  WhichSector& sector_second_point_ref ,
				                WhichSector& preceding_edge_ref , WhichSector& following_edge_ref);
				bool FromEdgeIn(WhichSector& sector_first_point_ref ,  WhichSector& sector_second_point_ref ,
				                WhichSector& preceding_edge_ref , WhichSector& inside_ref);
				bool FromEdgeOut(WhichSector& sector_first_point_ref ,  WhichSector& sector_second_point_ref ,
				                 WhichSector& preceding_edge_ref ,  std::vector<WhichSector> second_sector_vector);
				bool OutToEdge(WhichSector& sector_first_point_ref ,  WhichSector& sector_second_point_ref ,
				               WhichSector& following_edge_ref ,  std::vector<WhichSector> second_sector_vector);
				bool InToEdge(WhichSector& sector_first_point_ref ,  WhichSector& sector_second_point_ref ,
				              WhichSector& following_edge_ref , WhichSector& inside_ref);
				bool BothIn(WhichSector& sector_first_point_ref ,  WhichSector& sector_second_point_ref ,
				            WhichSector& inside_ref);
				bool BothOut(WhichSector& sector_first_point_ref ,  WhichSector& sector_second_point_ref ,
				             std::vector<WhichSector> sector_vector);
				bool OneInOneOut(WhichSector& sector_first_point_ref ,  WhichSector& sector_second_point_ref ,
				                 WhichSector& inside_ref , std::vector<WhichSector> first_sector_vector);
				bool FirstSectorIsOneOfTheseAndSecondSectorIsOneOfThose(WhichSector& sector_first_point_ref ,
				        std::vector<WhichSector> first_sector_vector , WhichSector& sector_second_point_ref ,
				        std::vector<WhichSector> second_sector_vector);
				bool FirstSectorIsOneOfTheseAndSecondSectorIsNoneOfThose(WhichSector& sector_first_point_ref ,
				        std::vector<WhichSector> first_sector_vector , WhichSector& sector_second_point_ref ,
				        std::vector<WhichSector> second_sector_vector);
				bool FirstSectorIsNoneOfTheseAndSecondSectorIsNoneOfThose(WhichSector& sector_first_point_ref ,
				        std::vector<WhichSector> first_sector_vector , WhichSector& sector_second_point_ref ,
				        std::vector<WhichSector> second_sector_vector);
				CharacterizeIntersection GetCharacteristicSecondCorner(WhichSector& sector_first_point_ref ,
				        WhichSector& sector_second_point_ref);
				void GetAntiOutsideCombinationSecondCorner(std::vector<WhichSector>&
				        anti_outside_combination_vector_ref);
				void GetInsideSecondCorner(WhichSector& inside_ref);
				void GetPrecedingEdgeSecondCorner(WhichSector& preceding_edge_ref);
				void GetFollowingEdgeSecondCorner(WhichSector& following_edge_ref);
				CharacterizeIntersection GetCharacteristicThirdCorner(WhichSector& sector_first_point_ref ,
				        WhichSector& sector_second_point_ref);
				void GetAntiOutsideCombinationThirdCorner(std::vector<WhichSector>&
				        anti_outside_combination_vector_ref);
				void GetInsideThirdCorner(WhichSector& inside_ref);
				void GetPrecedingEdgeThirdCorner(WhichSector& preceding_edge_ref);
				void GetFollowingEdgeThirdCorner(WhichSector& following_edge_ref);
				CharacterizeIntersection GetCharacteristicFourthCorner(WhichSector& sector_first_point_ref ,
				        WhichSector& sector_second_point_ref);
				void GetAntiOutsideCombinationFourthCorner(std::vector<WhichSector>&
				        anti_outside_combination_vector_ref);
				void GetInsideFourthCorner(WhichSector& inside_ref);
				void GetPrecedingEdgeFourthCorner(WhichSector& preceding_edge_ref);
				void GetFollowingEdgeFourthCorner(WhichSector& following_edge_ref);
				bool AppendNextIntersectionAndCorrespoindingPolygonSnippets(snippet_vector& snip_vec_ref_local ,
				        snippet_vector& snip_vec_ref , int current_edge, VoxelIntersectionIndex& next_voxel_intersection);
				float SetToNextIntersectionAlongThisEdgeAndReturnDistance(LegacyWorldCoordinate2D&
				        closest_local_current_position_ref , float distance_current_position , int current_edge);
				bool IsTheSameEdgeButNoCorner(LegacyWorldCoordinate2D local_current_position ,
				                              LegacyWorldCoordinate2D current_position , int current_edge);
				bool AddSnippetsThisIntersection(snippet_vector& snip_vec_ref ,
				                                 LegacyWorldCoordinate2D closest_local_current_position);
				bool JustEdge(PolygonSnippet& snippet_ref);
				void Switch(PolygonSnippet& snippet_one_ref ,  PolygonSnippet& snippet_two_ref);
				void CopySnippet(PolygonSnippet& to_snippet, PolygonSnippet& snip_in);
				bool GoForwardAndCreatePolygonSnippet(PolygonSnippet& forward_snippet_ref);
				bool GoBackwardAndCreatePolygonSnippet(PolygonSnippet& backward_snippet_ref);
				bool CheckPolygonIsInsideForward(float& angle_charact_forward);
				void GetAngle(LegacyWorldCoordinate2D the_next_point ,  LegacyWorldCoordinate2D the_first_point ,
				              float& angle_charact);
				bool CheckPolygonIsInsideBackward(float& angle_charact_backward);
				bool CheckPointInVoxelRegardingDoseIndex(LegacyWorldCoordinate2D voxel_coordinate_to_be_checked);
				double GetDistanceNextEdge(int current_edge);
				float GetDistanceAlongEdge(LegacyDoseVoxelIndex3D edge_position ,
				                           LegacyWorldCoordinate2D local_position);
				float GetPeriodicDist(float distance_current_position_local , float distance_current_position);
				double GetClockwiseDist(double distance_refer_to , double distance_compare);
				void AppendNewPolygonToResultingSequence();
				void AppendVoxelContour();
				void SlicePositionUnifyAndCheckPeriodicSequence();
				int GetMaxNumberSnippets();
				void GoGetPoly(bool& do_continue_ref , bool& next_is_edge_ref, int& continue_counter_ref ,
				               int& max_num);
				void AppendToResultingSequence(PolygonSnippet& snipp_ref);
				void DoAppendForward(PolygonSnippet& snipp_ref , int which_polygon);
				void DoAppendBackward(PolygonSnippet& snipp_ref , int which_polygon);
				void DoInsertForward(PolygonSnippet& snipp_ref , int which_polygon);
				void DoInsertBackward(PolygonSnippet& snipp_ref , int which_polygon);
				void AddFirstForward(PolygonSnippet& snipp_ref , int which_polygon);
				void GoAlongSnippetToNextIntersectionAndAppendToResultingSequence(PolygonSnippet& snipp_ref ,
				        int& continue_counter_ref);
				bool IsCorresponding(PolygonSnippet& snipp_ref_compare , PolygonSnippet& snipp_ref);
				LegacyWorldCoordinate2D GetOtherEnd(PolygonSnippet& snipp_ref);
				int GetSnippetIndexDistance(SnippetIndex behind_snip_index , SnippetIndex front_snip_index);
				bool SnippetIndexIdentical(SnippetIndex first , SnippetIndex second);
				void FindCorrespondingOneAndSetMovingIndexAndSetContinueCounter(PolygonSnippet& snipp_ref ,
				        int& continue_counter_ref);
				bool TryGetPolygonsnippetEdge(int& continue_counter_ref , int the_previous_edge);
				bool IsOtherEdge(PolygonSnippet& snipp_ref , int the_previous_edge);
				void IncrementSnippetIndex(SnippetIndex& the_snippet_index);
				bool GetNextIsEdge(SnippetIndex  snip_index);
				bool DoContinue(int& max_num , int& continue_counter_ref);
				void CopySnippetIndex(SnippetIndex  snip_index_from , SnippetIndex&  snip_index_to);
				void ToZero(SnippetIndex  snip_index);
				void CheckInside(SnippetIndex current_moving_index , bool& it_is_inside_ref ,
				                 bool& inside_switch_ref);
				void SetEdgeStatus(CharacterizeIntersection characterize);
				bool SetCurrentStaticIndexKnownInsideOrOutside(SnippetIndex& current_static_index_ref ,
				        bool& nothing_to_do_ref);
				bool CheckCornersOutside(SnippetIndex&  current_static_index_ref);
				bool CheckCornersInside(SnippetIndex&  current_static_index_ref);
				bool CheckThereIsSuchNeighbour(Uint16 x , Uint16 y , Uint16 z , int shift_x , int shift_y  ,
				                               int shift_z , field_content_type brightness);
				bool CheckCenterSurroundingStructure(bool& inside);
				LegacyWorldCoordinate3D GetWorldCoordinate(LegacyWorldCoordinate3D actually_a_voxel_coordinate);
				void SetCurrentPosition(const LegacyDoseVoxelIndex3D& aDoseIndex);
				bool SnippetIndexIsIdentical(SnippetIndex first_index , SnippetIndex second_index);
				bool CheckPointOnEdgeOutside(SnippetIndex& current_static_index_ref , bool& inside);
				void ResetSnippetIndicees();
				void SetIndexIdentical(IterativePolygonInTermsOfIntersections::WhichIntersection&
				                       intersection_index_to_ref , IterativePolygonInTermsOfIntersections::WhichIntersection&
				                       intersection_index_from_ref);
				void SetCurrentPositionToEdge(int voxel_edge);
				bool SameSnippet(PolygonSnippet& pol_snip_a , PolygonSnippet& pol_snip_b);
				void GetInCombinationFirstCorner(std::vector<WhichSector>& in_combination_vector_ref);
				void GetInCombinationSecondCorner(std::vector<WhichSector>& in_combination_vector_ref);
				void GetInCombinationThirdCorner(std::vector<WhichSector>& in_combination_vector_ref);
				void GetInCombinationFourthCorner(std::vector<WhichSector>& in_combination_vector_ref);
				void IncrementVoxelIntersectionIndex(int current_edge, VoxelIntersectionIndex& vox_inters_index ,
				                                     bool& changed_location);

				// begin some functions and stuff for test
				bool CheckConstructurSuccessForTest();
				// end some functions and stuff for test

				PolygonInfoVectorOfEdges pol_inf_vec;
				LegacyPolygonSequenceType resulting_sequence;
				FieldOfScalar<field_content_type>* MaskFieldReceived;
				field_content_type brightness_outside;
				field_content_type brightness_inside;
				LegacyDoseVoxelIndex3D the_dose_index;
				LegacyPolygonType the_polygon;

				IterativePolygonInTermsOfIntersections it_poly;
				IntersectionsThisVoxel voxel_intersections;

				LegacyWorldCoordinate2D current_position;

				int current_edge;

				SnippetIndex current_static_index;
				SnippetIndex current_moving_index;
				SnippetIndex begin_index;

				core::GeometricInfo GInformation;

			};
		}

	}
}







#endif
