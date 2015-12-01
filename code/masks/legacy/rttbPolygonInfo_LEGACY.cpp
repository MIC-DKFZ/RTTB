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

#include <map>
#include <utility>
#include <assert.h>
#include <algorithm>
#include <list>

#include "rttbMask.h"
#include "rttbPolygonInfo_LEGACY.h"
#include "rttbNullPointerException.h"

namespace rttb
{
	namespace masks
	{
		namespace legacy
		{

			PolygonInfo::PolygonInfo(const LegacyPolygonType& the_polygon_in, int polygon_number_in,
			                         field_content_type brightness_outside_in , field_content_type brightness_inside_in ,
			                         FieldOfScalar<field_content_type>* MaskFieldReceived_in ,
			                         const PolygonInTermsOfIntersections& Polygon_Intersections_In ,
			                         const LegacyDoseVoxelIndex3D& aDoseIndex , core::GeometricInfo& GInfIn ,
			                         const IntersectionsThisVoxel& intersections_this_voxel_in): the_polygon(the_polygon_in) ,
				voxel_intersections(intersections_this_voxel_in)
			{

				if (MaskFieldReceived_in == NULL)
				{
					assert(0);
				}
				else
				{
					MaskFieldReceived = MaskFieldReceived_in;
				}

				brightness_outside = brightness_outside_in;

				brightness_inside = brightness_inside_in;

				SetDoseIndex(aDoseIndex);

				pol_inf_vec.clear();

				PolygonInfoVectorOfEdges& pol_inf_vec_ref = pol_inf_vec;

				GInformation = GInfIn;

				SetCurrentPosition(aDoseIndex);

				it_poly.SetIntersectionsAndResetIntersectionIndex(Polygon_Intersections_In);

				voxel_intersections.SetPolygonNumber(polygon_number_in);
			}

			void PolygonInfo::CalcSnippetVectors()
			{
				VoxelIntersectionIndex next_voxel_intersection;

				pol_inf_vec.resize(0);

				for (int voxel_edge = 0 ; voxel_edge < 4 ; voxel_edge++)
				{

					next_voxel_intersection.reset();

					current_edge = voxel_edge;

					snippet_vector snip_vec;

					snippet_vector& snip_vec_ref = snip_vec;

					SetCurrentPositionToEdge(voxel_edge);

					std::vector<PolygonSnippet> snip_vec_local;
					snippet_vector& snip_vec_ref_local = snip_vec_local;

					bool finished = false;

					while (finished == false)
					{
						finished = AppendNextIntersectionAndCorrespoindingPolygonSnippets(snip_vec_ref_local, snip_vec_ref ,
						           voxel_edge, next_voxel_intersection);
					}

					pol_inf_vec.push_back(snip_vec);
				}

				AppendEdges(pol_inf_vec);

				it_poly.ResetIntersectionIndex();
			}

			void PolygonInfo::ShowSnippet(PolygonSnippet a_snippet)
			{
				std::cout << " a_snippet.i_have_been_processed : " << a_snippet.i_have_been_processed  << std::endl;

				std::cout << " a_snippet.characterize : " << a_snippet.characterize  << std::endl;
				std::cout << " a_snippet.is_edge : " << a_snippet.is_edge  << std::endl;

				std::cout << " a_snippet.angle_charact : " << a_snippet.angle_charact  << std::endl;

				std::cout << " a_snippet.why_here.x : " << a_snippet.why_here.x  << std::endl;
				std::cout << " a_snippet.why_here.y : " << a_snippet.why_here.y  << std::endl;

				std::cout << " a_snippet.point_of_interest_start.x : " << a_snippet.point_of_interest_start.x  <<
				          std::endl;
				std::cout << " a_snippet.point_of_interest_start.y : " << a_snippet.point_of_interest_start.y  <<
				          std::endl;

				std::cout << " a_snippet.point_of_interest_end.x : " << a_snippet.point_of_interest_end.x  <<
				          std::endl;
				std::cout << " a_snippet.point_of_interest_end.y : " << a_snippet.point_of_interest_end.y  <<
				          std::endl;

				std::cout << " a_snippet.snippet_intermediate_content.size() : " <<
				          a_snippet.snippet_intermediate_content.size()  << std::endl;

				for (unsigned int counter = 0 ; counter < a_snippet.snippet_intermediate_content.size() ; counter++)
				{
					std::cout << " a_snippet.snippet_intermediate_content.at( counter ).x : " <<
					          a_snippet.snippet_intermediate_content.at(counter).x  << std::endl;
					std::cout << " a_snippet.snippet_intermediate_content.at( counter ).y : " <<
					          a_snippet.snippet_intermediate_content.at(counter).y  << std::endl;
				}
			}

			void PolygonInfo::SetDoseIndex(const LegacyDoseVoxelIndex3D& aDoseIndex)
			{
				the_dose_index = aDoseIndex;
			}

			bool PolygonInfo::selfTest()
			{
				return CheckConstructurSuccessForTest();
			}

			IterativePolygonInTermsOfIntersections PolygonInfo::GetIterativePolygoneInTermsOfIntersections()
			{
				return it_poly;
			}


			bool PolygonInfo::CheckConstructurSuccessForTest()
			{
				bool gone_well = true;

				if (MaskFieldReceived == NULL)
				{
					gone_well = false;
					std::cout << " Mask field must not be zero ! " << std::endl;
				}

				if (MaskFieldReceived->GetData(0, 0, 0) < 0)
				{
					gone_well = false;
					std::cout << " Mask field values must not be below zero ! " << std::endl;
				}

				if (brightness_outside != 0)
				{
					gone_well = false;
					std::cout << " Strange value for brightness_outside ! " << std::endl;
				}

				if (it_poly.CheckCurrentIndexInitForTest() == false)
				{
					gone_well = false;
					std::cout << " current_index_internal initialized incorrect ! " << std::endl;
				}

				return gone_well;
			}

			bool PolygonInfo::TestCreateUnifiedListOfRawAndColumnIntersections(PolygonInTermsOfIntersections
			        Expected_Unified_Polygon_Intersections)
			{
				return it_poly.ItIsAsExpected(Expected_Unified_Polygon_Intersections);
			}

			bool PolygonInfo::CheckResultingPolygonInfoVectorOfEdgesForTestStructure(
			    PolygonInfoVectorOfEdges pol_inf_vec_expected)
			{
				if (pol_inf_vec.size() != pol_inf_vec_expected.size())
				{
					std::cout << "  incompatible size  pol_inf_vec.size()  " <<  pol_inf_vec.size() << std::endl;
					std::cout << "  incompatible size  pol_inf_vec_expected.size()  " <<  pol_inf_vec_expected.size() <<
					          std::endl;
					return false;
				}

				for (GridIndexType counter = 0 ; counter < pol_inf_vec.size() ; counter++)
				{
					if (pol_inf_vec.at(counter).size() != pol_inf_vec_expected.at(counter).size())
					{
						std::cout << "  incompatible size  pol_inf_vec.at( counter ).size()  " <<  pol_inf_vec.at(
						              counter).size()  << std::endl;
						std::cout << "  incompatible size  pol_inf_vec_expected.at( counter ).size()  "  <<
						          pol_inf_vec_expected.at(counter).size()  << std::endl;
						return false;
					}

					for (GridIndexType internal_counter = 0 ; internal_counter < pol_inf_vec.at(counter).size() ;
					     internal_counter++)
					{
						PolygonSnippet pol_snip_a = pol_inf_vec.at(counter).at(internal_counter);
						PolygonSnippet pol_snip_b = pol_inf_vec_expected.at(counter).at(internal_counter);

						if (! SameSnippet(pol_snip_a , pol_snip_b))
						{
							std::cout << " Unexpected snippet das vorhandene ! " << std::endl;
							ShowSnippet(pol_snip_a);
							std::cout << " Unexpected snippet das erwartete ! " << std::endl;
							ShowSnippet(pol_snip_b);
							return false;
						}

					}

				}

				return true;
			}


			bool PolygonInfo::SameSnippet(PolygonSnippet& pol_snip_a , PolygonSnippet& pol_snip_b)
			{

				if (pol_snip_a.is_edge != pol_snip_b.is_edge)
				{
					return false;
				}

				if (pol_snip_a.characterize != pol_snip_b.characterize)
				{
					return false;
				}

				if (static_cast<float>(pol_snip_a.point_of_interest_start.x) != static_cast<float>
				    (pol_snip_b.point_of_interest_start.x))
				{
					return false;
				}

				if (static_cast<float>(pol_snip_a.point_of_interest_start.y) != static_cast<float>
				    (pol_snip_b.point_of_interest_start.y))
				{
					return false;
				}

				if (static_cast<float>(pol_snip_a.point_of_interest_end.y) != static_cast<float>
				    (pol_snip_b.point_of_interest_end.y))
				{
					return false;
				}

				if (static_cast<float>(pol_snip_a.why_here.x) != static_cast<float>(pol_snip_b.why_here.x))
				{
					return false;
				}

				if (static_cast<float>(pol_snip_a.why_here.y) != static_cast<float>(pol_snip_b.why_here.y))
				{
					return false;
				}

				if (pol_snip_a.snippet_intermediate_content.size() !=
				    pol_snip_b.snippet_intermediate_content.size())
				{
					return false;
				}

				for (GridIndexType counter = 0 ; counter < pol_snip_a.snippet_intermediate_content.size() ;
				     counter++)
				{
					if (static_cast<float>(pol_snip_a.snippet_intermediate_content.at(counter).x) != static_cast<float>
					    (pol_snip_b.snippet_intermediate_content.at(counter).x))
					{
						return false;
					}

					if (static_cast<float>(pol_snip_a.snippet_intermediate_content.at(counter).y) != static_cast<float>
					    (pol_snip_b.snippet_intermediate_content.at(counter).y))
					{
						return false;
					}

					if (static_cast<float>(pol_snip_a.snippet_intermediate_content.at(counter).z) != static_cast<float>
					    (pol_snip_b.snippet_intermediate_content.at(counter).z))
					{
						return false;
					}
				}

				return true;
			}


			bool PolygonInfo::CompareCalculatedAndExpectedResultingSequenceForTest(
			    LegacyPolygonSequenceType& expected_resulting_sequence_ref)
			{
				if (expected_resulting_sequence_ref.size() > resulting_sequence.size())
				{
					std::cout <<  " Size ! " << std::endl;
					std::cout <<  " expected_resulting_sequence_ref.size() "  << expected_resulting_sequence_ref.size()
					          << std::endl;
					std::cout <<  " resulting_sequence.size() "  <<  resulting_sequence.size() << std::endl;
					return false;
				}

				for (GridIndexType counter = 0 ; counter < expected_resulting_sequence_ref.size() ; counter++)
				{
					if (expected_resulting_sequence_ref.at(counter).size() != resulting_sequence.at(counter).size())
					{
						std::cout <<  " different size ! " << std::endl;
						std::cout <<  " expected_resulting_sequence_ref.at( counter ).size() "  <<
						          expected_resulting_sequence_ref.at(counter).size()  << std::endl;
						std::cout <<  " resulting_sequence.at( counter ).size() "  <<  resulting_sequence.at(
						              counter).size() << std::endl;
						return false;
					}

					for (GridIndexType counter_internal = 0 ;
					     counter_internal < expected_resulting_sequence_ref.at(counter).size() ; counter_internal++)
					{
						LegacyWorldCoordinate3D wc3d_a = expected_resulting_sequence_ref.at(counter).at(counter_internal);
						LegacyWorldCoordinate3D wc3d_b = resulting_sequence.at(counter).at(counter_internal);

						if (wc3d_a.x != wc3d_b.x)
						{
							std::cout << " unexpected x ! " << std::endl;
							return false;
						}

						if (wc3d_a.y != wc3d_b.y)
						{
							std::cout << " unexpected y ! " << std::endl;
							return false;
						}

						if (wc3d_a.z != wc3d_b.z)
						{
							std::cout << " unexpected z ! " << std::endl;
							return false;
						}
					}
				}

				return true;
			}

			void PolygonInfo::SetCurrentPositionToEdge(int voxel_edge)
			{
				switch (voxel_edge)
				{
					case 0 :
						current_position.x = static_cast<WorldCoordinate>(the_dose_index.x);
						current_position.y = static_cast<WorldCoordinate>(the_dose_index.y);
						break;

					case 1 :
						current_position.x = static_cast<WorldCoordinate>(the_dose_index.x + 1);
						current_position.y = static_cast<WorldCoordinate>(the_dose_index.y);
						break;

					case 2 :
						current_position.x = static_cast<WorldCoordinate>(the_dose_index.x + 1);
						current_position.y = static_cast<WorldCoordinate>(the_dose_index.y + 1);
						break;

					case 3 :
						current_position.x = static_cast<WorldCoordinate>(the_dose_index.x);
						current_position.y = static_cast<WorldCoordinate>(the_dose_index.y + 1);
						break;

					default :
						assert(0);
				}
			}

			void PolygonInfo::AppendEdges(PolygonInfoVectorOfEdges& pol_inf_vec_ref)
			{
				for (unsigned int voxel_edge = 0 ; voxel_edge < pol_inf_vec_ref.size() ; voxel_edge++)
				{

					assert(voxel_edge <= 3);

					PolygonSnippet pol_snip_front;
					pol_snip_front.is_edge = 1;
					pol_snip_front.characterize = corner;

					switch (voxel_edge)
					{
						case 0 :
							pol_snip_front.point_of_interest_start.x = static_cast<WorldCoordinate>(the_dose_index.x);
							pol_snip_front.point_of_interest_start.y = static_cast<WorldCoordinate>(the_dose_index.y);
							pol_snip_front.point_of_interest_end.x = static_cast<WorldCoordinate>(the_dose_index.x);
							pol_snip_front.point_of_interest_end.y = static_cast<WorldCoordinate>(the_dose_index.y);
							pol_snip_front.why_here.x = static_cast<WorldCoordinate>(the_dose_index.x);
							pol_snip_front.why_here.y = static_cast<WorldCoordinate>(the_dose_index.y);
							break;

						case 1 :
							pol_snip_front.point_of_interest_start.x = static_cast<WorldCoordinate>(the_dose_index.x + 1);
							pol_snip_front.point_of_interest_start.y = static_cast<WorldCoordinate>(the_dose_index.y);
							pol_snip_front.point_of_interest_end.x = static_cast<WorldCoordinate>(the_dose_index.x + 1);
							pol_snip_front.point_of_interest_end.y = static_cast<WorldCoordinate>(the_dose_index.y);
							pol_snip_front.why_here.x = static_cast<WorldCoordinate>(the_dose_index.x + 1);
							pol_snip_front.why_here.y = static_cast<WorldCoordinate>(the_dose_index.y);
							break;

						case 2 :
							pol_snip_front.point_of_interest_start.x = static_cast<WorldCoordinate>(the_dose_index.x + 1);
							pol_snip_front.point_of_interest_start.y = static_cast<WorldCoordinate>(the_dose_index.y + 1);
							pol_snip_front.point_of_interest_end.x = static_cast<WorldCoordinate>(the_dose_index.x + 1);
							pol_snip_front.point_of_interest_end.y = static_cast<WorldCoordinate>(the_dose_index.y + 1);
							pol_snip_front.why_here.x = static_cast<WorldCoordinate>(the_dose_index.x + 1);
							pol_snip_front.why_here.y = static_cast<WorldCoordinate>(the_dose_index.y + 1);
							break;

						case 3 :
							pol_snip_front.point_of_interest_start.x = static_cast<WorldCoordinate>(the_dose_index.x);
							pol_snip_front.point_of_interest_start.y = static_cast<WorldCoordinate>(the_dose_index.y + 1);
							pol_snip_front.point_of_interest_end.x = static_cast<WorldCoordinate>(the_dose_index.x);
							pol_snip_front.point_of_interest_end.y = static_cast<WorldCoordinate>(the_dose_index.y + 1);
							pol_snip_front.why_here.x = static_cast<WorldCoordinate>(the_dose_index.x);
							pol_snip_front.why_here.y = static_cast<WorldCoordinate>(the_dose_index.y + 1);
							break;

						default :
							assert(0);
					}

					snippet_vector_iterator iter;
					iter = pol_inf_vec_ref.at(voxel_edge).begin();
					pol_inf_vec_ref.at(voxel_edge).insert(iter , pol_snip_front);
				}

				CheckEdgeIntersections();
			}

			void PolygonInfo::CheckEdgeIntersections()
			{
				SnippetIndex edge_index;

				edge_index.edge = 0;
				edge_index.snip = 0;
				current_edge = 0;

				CheckEdegeTouchCharacteristicsAndAppendIntermediateContent(edge_index);

				edge_index.edge = 1;
				edge_index.snip = 0;
				current_edge = 1;

				CheckEdegeTouchCharacteristicsAndAppendIntermediateContent(edge_index);

				edge_index.edge = 2;
				edge_index.snip = 0;
				current_edge = 2;

				CheckEdegeTouchCharacteristicsAndAppendIntermediateContent(edge_index);

				edge_index.edge = 3;
				edge_index.snip = 0;
				current_edge = 3;
				CheckEdegeTouchCharacteristicsAndAppendIntermediateContent(edge_index);
			}


			bool PolygonInfo::GetCornerFromVoxelIntersections(unsigned int the_index, SnippetIndex edge_index ,
			        IterativePolygonInTermsOfIntersections::WhichIntersection& intersect_index)
			{
				if (voxel_intersections.corner_intersections_intersection_coord.at(edge_index.edge).size() >
				    the_index)
				{
					intersect_index = voxel_intersections.corner_intersections_intersection_coord.at(
					                      edge_index.edge).at(the_index);
					return true;
				}
				else
				{
					return false;
				}
			}

			int PolygonInfo::GetNumberOfSnppetsThisEdge(SnippetIndex edge_index)
			{
				return voxel_intersections.corner_intersections_intersection_coord.at(edge_index.edge).size();
			}


			bool PolygonInfo::SetToNextIntersectonAlongThisEdgeAndReturnDistanceBasedOnVoxelIntersections(
			    LegacyWorldCoordinate2D& closest_local_current_position_ref , int current_edge,
			    VoxelIntersectionIndex next_voxel_intersection)
			{
				if (voxel_intersections.edge_intersections_intersection_coord.at(current_edge).size() >
				    next_voxel_intersection.next_intersection)
				{
					if (voxel_intersections.edge_intersections_intersection_coord.at(current_edge).at(
					        next_voxel_intersection.next_intersection).size() >
					    next_voxel_intersection.intersection_douplication)
					{
						IterativePolygonInTermsOfIntersections::WhichIntersection an_intersection_index =
						    voxel_intersections.edge_intersections_intersection_coord.at(current_edge).at(
						        next_voxel_intersection.next_intersection).at(next_voxel_intersection.intersection_douplication);

						it_poly.SetCurrentIntersectionIndex(an_intersection_index);

						it_poly.ThisIntersection(closest_local_current_position_ref);

						if (!it_poly.CheckCurrentIntersectionIndexIdentical(an_intersection_index))
						{
							assert(0);
						}

						return true;
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}

			void PolygonInfo::CheckEdegeTouchCharacteristicsAndAppendIntermediateContent(
			    SnippetIndex edge_index)
			{

				LegacyWorldCoordinate2D edge_coord =  pol_inf_vec.at(edge_index.edge).at(edge_index.snip).why_here;

				IterativePolygonInTermsOfIntersections::WhichIntersection
				the_intersection_index_from_voxel_intersections;
				the_intersection_index_from_voxel_intersections.point_index = 0;
				the_intersection_index_from_voxel_intersections.intersection_index = 0;
				IterativePolygonInTermsOfIntersections::WhichIntersection&
				the_intersection_index_from_voxel_intersections_ref =
				    the_intersection_index_from_voxel_intersections;

				int number_of_snippets_this_edge = GetNumberOfSnppetsThisEdge(edge_index);

				std::vector<PolygonSnippet> snip_vec_local;


				if (number_of_snippets_this_edge > 0)
				{
					snip_vec_local = CreateSnippetVector(number_of_snippets_this_edge ,
					                                     pol_inf_vec.at(edge_index.edge).at(edge_index.snip));
				}

				int erased = 0;

				for (int which_one = 0 ; which_one < number_of_snippets_this_edge ; which_one++)
				{
					bool there_is_an_intersection = GetCornerFromVoxelIntersections(which_one, edge_index ,
					                                the_intersection_index_from_voxel_intersections_ref);

					if (there_is_an_intersection)
					{
						it_poly.SetCurrentIntersectionIndex(the_intersection_index_from_voxel_intersections_ref);

						LegacyWorldCoordinate3D contour_point_one = it_poly.GetRestpectivePologonPointInVoxelCoordinates(
						            the_intersection_index_from_voxel_intersections_ref);

						int back = 1;

						while ((contour_point_one.x == edge_coord.x) && (contour_point_one.y == edge_coord.y))
						{
							contour_point_one = it_poly.GetRestpectivePologonPointInVoxelCoordinatesPrevious(
							                        the_intersection_index_from_voxel_intersections_ref , back);

							back++;
						}

						int ahead = 1;

						LegacyWorldCoordinate3D  contour_point_two =
						    it_poly.GetRestpectivePologonPointInVoxelCoordinatesFurther(
						        the_intersection_index_from_voxel_intersections_ref , ahead);

						while ((contour_point_two.x == edge_coord.x) && (contour_point_two.y == edge_coord.y))
						{
							ahead++;

							contour_point_two = it_poly.GetRestpectivePologonPointInVoxelCoordinatesFurther(
							                        the_intersection_index_from_voxel_intersections_ref , ahead);
						}

						bool there_is_a_suitable_polygon_forward = false;
						bool there_is_a_suitable_polygon_backward = false;

						PolygonSnippet forward_snippet;
						PolygonSnippet& forward_snippet_ref = forward_snippet;
						PolygonSnippet backward_snippet;
						PolygonSnippet& backward_snippet_ref = backward_snippet;

						PolygonSnippet& edge_snippet_ref = snip_vec_local.at((which_one - erased));

						float angle_charact_a = 0;
						float angle_charact_b = 0;

						if (GoForwardAndCreatePolygonSnippet(forward_snippet_ref))
						{
							there_is_a_suitable_polygon_forward = true;
						}

						if (GoBackwardAndCreatePolygonSnippet(backward_snippet_ref))
						{
							there_is_a_suitable_polygon_backward = true;
						}

						if (there_is_a_suitable_polygon_forward)
						{
							angle_charact_a = forward_snippet_ref.angle_charact;
						}

						if (there_is_a_suitable_polygon_backward)
						{
							angle_charact_b = backward_snippet_ref.angle_charact;
						}

						if ((! there_is_a_suitable_polygon_forward) && (! there_is_a_suitable_polygon_backward))
						{
							edge_snippet_ref.characterize = corner;
						}

						if ((there_is_a_suitable_polygon_forward) && (there_is_a_suitable_polygon_backward))
						{
							WorkWithForwardAndBackwardEdgeSnippet(which_one, angle_charact_a , angle_charact_b ,
							                                      forward_snippet_ref , backward_snippet_ref , edge_snippet_ref , snip_vec_local , erased);
						}

						if ((there_is_a_suitable_polygon_forward) && (! there_is_a_suitable_polygon_backward))
						{
							WorkWithForwardEdgeSnippet(edge_coord, contour_point_one, which_one, angle_charact_a ,
							                           angle_charact_b , forward_snippet_ref , backward_snippet_ref , edge_snippet_ref , snip_vec_local ,
							                           erased);
						}

						if ((! there_is_a_suitable_polygon_forward) && (there_is_a_suitable_polygon_backward))
						{
							WorkWithBackwardEdgeSnippet(edge_coord, contour_point_two , which_one, angle_charact_a ,
							                            angle_charact_b , forward_snippet_ref , backward_snippet_ref , edge_snippet_ref , snip_vec_local ,
							                            erased);
						}
					}
				}

				if (snip_vec_local.size() > 1)
				{
					snip_vec_local = SortClockwise(snip_vec_local);
				}

				InsertToSnippetVectorFront(snip_vec_local, edge_index);
			}

			void PolygonInfo::WorkWithForwardAndBackwardEdgeSnippet(int which_one, float& angle_charact_a ,
			        float& angle_charact_b , PolygonSnippet& forward_snippet_ref ,
			        PolygonSnippet& backward_snippet_ref , PolygonSnippet& edge_snippet_ref ,
			        std::vector<PolygonSnippet>& snip_vec_local , int& erased)
			{
				if (angle_charact_a < angle_charact_b)
				{
					Switch(forward_snippet_ref , backward_snippet_ref);
					float keep_angle_charact_a = angle_charact_a;
					float keep_angle_charact_b = angle_charact_b;
					angle_charact_a  =  keep_angle_charact_b;
					angle_charact_b  =  keep_angle_charact_a;
				}

				if ((angle_charact_a > 0) && (angle_charact_a < 1.0))
				{
					if ((angle_charact_b > 0) && (angle_charact_b < 1.0))
					{
						forward_snippet_ref.characterize = inside_inside_touches;
						forward_snippet_ref.is_edge = true;
						forward_snippet_ref.why_here.x = forward_snippet_ref.point_of_interest_start.x;
						forward_snippet_ref.why_here.y = forward_snippet_ref.point_of_interest_start.y;
						CopySnippet(edge_snippet_ref , forward_snippet_ref);

						backward_snippet_ref.characterize = inside_inside_touches;
						backward_snippet_ref.why_here.x = backward_snippet_ref.point_of_interest_start.x;
						backward_snippet_ref.why_here.y = backward_snippet_ref.point_of_interest_start.y;
						backward_snippet_ref.is_edge = true;

						snippet_vector_iterator iter;

						iter = snip_vec_local.begin() + (which_one - erased) ;
						snip_vec_local.insert(iter , backward_snippet_ref);
					}
					else if (angle_charact_b == 0)
					{
						forward_snippet_ref.characterize = edge_to_inside;
						forward_snippet_ref.is_edge = true;
						forward_snippet_ref.why_here.x = forward_snippet_ref.point_of_interest_start.x;
						forward_snippet_ref.why_here.y = forward_snippet_ref.point_of_interest_start.y;
						CopySnippet(edge_snippet_ref , forward_snippet_ref);
					}
					else if (angle_charact_b == 1.0)
					{
						assert(0);
					}
					else if (angle_charact_b < 0)
					{
						forward_snippet_ref.characterize = real_intersection;
						forward_snippet_ref.is_edge = true;
						forward_snippet_ref.why_here.x = forward_snippet_ref.point_of_interest_start.x;
						forward_snippet_ref.why_here.y = forward_snippet_ref.point_of_interest_start.y;

						CopySnippet(edge_snippet_ref , forward_snippet_ref);
					}
				}
				else if (angle_charact_a < 0)
				{
					assert(angle_charact_b < 0);
					int position = (which_one - erased);

					RemoveFromLocalSnippetVector(snip_vec_local , position);

					erased++;
				}
				else if (angle_charact_a == 1.0)
				{
					if ((angle_charact_b > 0) && (angle_charact_b < 1.0))
					{
						backward_snippet_ref.is_edge = true;
						backward_snippet_ref.characterize = inside_to_edge;
						backward_snippet_ref.why_here.x = backward_snippet_ref.point_of_interest_start.x;
						backward_snippet_ref.why_here.y = backward_snippet_ref.point_of_interest_start.y;

						CopySnippet(edge_snippet_ref , backward_snippet_ref);
					}
					else if (angle_charact_b == 1.0)
					{
						int position = (which_one - erased);

						RemoveFromLocalSnippetVector(snip_vec_local , position);

						erased++;
					}
					else if (angle_charact_b == 0)
					{
						int position = (which_one - erased);

						RemoveFromLocalSnippetVector(snip_vec_local , position);

						erased++;
					}
					else if (angle_charact_b < 0)
					{
						forward_snippet_ref.characterize = outside_to_edge;
						forward_snippet_ref.is_edge = true;
						forward_snippet_ref.why_here.x = forward_snippet_ref.point_of_interest_start.x;
						forward_snippet_ref.why_here.y = forward_snippet_ref.point_of_interest_start.y;

						CopySnippet(edge_snippet_ref , forward_snippet_ref);
					}
				}
			}

			void PolygonInfo::WorkWithForwardEdgeSnippet(LegacyWorldCoordinate2D edge_coord,
			        LegacyWorldCoordinate3D contour_point_one, int which_one, float& angle_charact_a ,
			        float& angle_charact_b , PolygonSnippet& forward_snippet_ref ,
			        PolygonSnippet& backward_snippet_ref , PolygonSnippet& edge_snippet_ref ,
			        std::vector<PolygonSnippet>& snip_vec_local , int& erased)
			{
				float angle_charact = 0;
				float& angle_charact_ref = angle_charact;
				LegacyWorldCoordinate2D contour_point_one_2d;
				contour_point_one_2d.x = contour_point_one.x;
				contour_point_one_2d.y = contour_point_one.y;

				GetAngle(contour_point_one_2d , edge_coord , angle_charact_ref);

				if ((angle_charact_a > 0) && (angle_charact_a <  1.0))
				{
					if ((angle_charact_ref != 0) && (angle_charact_ref != 1.0))
					{
						forward_snippet_ref.characterize = real_intersection;
						forward_snippet_ref.is_edge = true;
						forward_snippet_ref.why_here.x = forward_snippet_ref.point_of_interest_start.x;
						forward_snippet_ref.why_here.y = forward_snippet_ref.point_of_interest_start.y;

						CopySnippet(edge_snippet_ref , forward_snippet_ref);
					}
					else if (angle_charact_ref == 0)
					{
						forward_snippet_ref.characterize = edge_to_inside;
						forward_snippet_ref.is_edge = true;
						forward_snippet_ref.why_here.x = forward_snippet_ref.point_of_interest_start.x;
						forward_snippet_ref.why_here.y = forward_snippet_ref.point_of_interest_start.y;

						CopySnippet(edge_snippet_ref , forward_snippet_ref);
					}
					else if (angle_charact_ref == 1.0)
					{
						forward_snippet_ref.characterize = inside_to_edge ;
						forward_snippet_ref.is_edge = true;
						forward_snippet_ref.why_here.x = forward_snippet_ref.point_of_interest_start.x;
						forward_snippet_ref.why_here.y = forward_snippet_ref.point_of_interest_start.y;

						CopySnippet(edge_snippet_ref , forward_snippet_ref);
					}
				}
				else if (angle_charact_a == 0)
				{
					int position = (which_one - erased);

					RemoveFromLocalSnippetVector(snip_vec_local , position);

					erased++;
				}
				else if (angle_charact_a == 1.0)
				{
					int position = (which_one - erased);

					RemoveFromLocalSnippetVector(snip_vec_local , position);

					erased++;
				}
				else if (angle_charact_a < 0)
				{
					int position = (which_one - erased);

					RemoveFromLocalSnippetVector(snip_vec_local , position);

					erased++;
				}
			}

			void PolygonInfo::WorkWithBackwardEdgeSnippet(LegacyWorldCoordinate2D edge_coord,
			        LegacyWorldCoordinate3D  contour_point_two , int which_one, float& angle_charact_a ,
			        float& angle_charact_b , PolygonSnippet& forward_snippet_ref ,
			        PolygonSnippet& backward_snippet_ref , PolygonSnippet& edge_snippet_ref ,
			        std::vector<PolygonSnippet>& snip_vec_local , int& erased)
			{
				float angle_charact = 0;
				float& angle_charact_ref = angle_charact;
				LegacyWorldCoordinate2D contour_point_two_2d;
				contour_point_two_2d.x = contour_point_two.x;
				contour_point_two_2d.y = contour_point_two.y;

				GetAngle(contour_point_two_2d , edge_coord , angle_charact_ref);

				if ((angle_charact_b > 0) && (angle_charact_b < 1.0))
				{
					if ((angle_charact_ref != 0) && (angle_charact_ref != 1.0))
					{
						backward_snippet_ref.characterize = real_intersection;
						backward_snippet_ref.is_edge = true;
						backward_snippet_ref.why_here.x = backward_snippet_ref.point_of_interest_start.x;
						backward_snippet_ref.why_here.y = backward_snippet_ref.point_of_interest_start.y;

						CopySnippet(edge_snippet_ref , backward_snippet_ref);
					}

					if ((angle_charact_ref == 1.0))
					{
						backward_snippet_ref.characterize = inside_to_edge;
						backward_snippet_ref.is_edge = true;
						backward_snippet_ref.why_here.x = backward_snippet_ref.point_of_interest_start.x;
						backward_snippet_ref.why_here.y = backward_snippet_ref.point_of_interest_start.y;

						CopySnippet(edge_snippet_ref , backward_snippet_ref);
					}

					if ((angle_charact_ref == 0))
					{
						backward_snippet_ref.characterize = edge_to_inside;
						backward_snippet_ref.is_edge = true;
						backward_snippet_ref.why_here.x = backward_snippet_ref.point_of_interest_start.x;
						backward_snippet_ref.why_here.y = backward_snippet_ref.point_of_interest_start.y;

						CopySnippet(edge_snippet_ref , backward_snippet_ref);
					}

				}
				else if (angle_charact_b == 0)
				{
					int position = (which_one - erased);

					RemoveFromLocalSnippetVector(snip_vec_local , position);

					erased++;
				}
				else if (angle_charact_b == 1.0)
				{
					int position = (which_one - erased);

					RemoveFromLocalSnippetVector(snip_vec_local , position);

					erased++;
				}
				else if (angle_charact_b < 0)
				{
					int position = (which_one - erased);

					RemoveFromLocalSnippetVector(snip_vec_local , position);

					erased++;
				}
			}

			PolygonInfo::snippet_vector PolygonInfo::CreateSnippetVector(int number_of_snippets_this_edge ,
			        PolygonSnippet template_snip)
			{
				snippet_vector vector_local;

				PolygonSnippet pol_snip;

				for (int i = 0 ; i < number_of_snippets_this_edge ; i++)
				{
					CopySnippet(pol_snip, template_snip);

					vector_local.push_back(pol_snip);
				}

				return vector_local;
			}

			PolygonInfo::snippet_vector PolygonInfo::SortClockwise(snippet_vector snip_vec_local)
			{
				snippet_vector snip_vec_local_internal;
				std::vector<bool> do_check;
				do_check.clear();

				for (GridIndexType i = 0 ; i < snip_vec_local.size() ; i++)
				{

					LegacyWorldCoordinate2D position_one = snip_vec_local.at(i).point_of_interest_end;

					double distance_one = GetDistanceAlongEdge(the_dose_index , position_one);

					LegacyWorldCoordinate2D position_for_reference = snip_vec_local.at(i).why_here;

					double distance_for_reference = GetDistanceAlongEdge(the_dose_index , position_for_reference);

					double rang_one = GetClockwiseDist(distance_for_reference , distance_one);

					bool got_it = false;

					for (GridIndexType j = 0 ; j < snip_vec_local_internal.size() ; j++)
					{

						if (do_check.at(j) == true)
						{
							LegacyWorldCoordinate2D position_two = snip_vec_local_internal.at(j).point_of_interest_end;

							double distance_two = GetDistanceAlongEdge(the_dose_index , position_two);

							double rang_two = GetClockwiseDist(distance_for_reference , distance_two);

							if (rang_one < rang_two)
							{
								snippet_vector_iterator iter;
								iter = snip_vec_local_internal.begin();
								iter += j;
								snip_vec_local_internal.insert(iter , snip_vec_local.at(i));
								got_it = true;

								bool check = true;
								do_check.push_back(check);

								if ((snip_vec_local.at(i).characterize == 6) && ((i + 1) < snip_vec_local.size()))
								{
									if (snip_vec_local.at(i + 1).characterize == 6)
									{
										snippet_vector_iterator iter;
										iter = snip_vec_local_internal.begin();
										iter += j;
										snip_vec_local_internal.insert(iter , snip_vec_local.at(i + 1));
										i++;
										got_it = true;

										bool check = false;
										do_check.push_back(check);
									}
								}

								j = snip_vec_local.size();
							}
						}
					}

					if (got_it == false)
					{
						snip_vec_local_internal.push_back(snip_vec_local.at(i));

						bool check = true;
						do_check.push_back(check);

						if ((snip_vec_local.at(i).characterize == 6) && ((i + 1) < snip_vec_local.size()))
						{
							if (snip_vec_local.at(i + 1).characterize == 6)
							{
								snip_vec_local_internal.push_back(snip_vec_local.at(i + 1));
								i++;
								got_it = true;

								bool check = false;
								do_check.push_back(check);
							}
						}
					}

				}

				return snip_vec_local_internal;
			}

			void PolygonInfo::InsertToSnippetVectorFront(snippet_vector snip_vec_local ,
			        SnippetIndex edge_index)
			{
				snippet_vector_iterator iter;

				for (GridIndexType i = 0 ; i < snip_vec_local.size() ; i++)
				{
					PolygonSnippet pol_snip_front = snip_vec_local.at(i);

					iter = pol_inf_vec.at(edge_index.edge).begin() + (i + 1);

					pol_inf_vec.at(edge_index.edge).insert(iter , pol_snip_front);
				}

				if (snip_vec_local.size() > 0)
				{
					iter = pol_inf_vec.at(edge_index.edge).begin();
					pol_inf_vec.at(edge_index.edge).erase(iter);
				}
			}

			void PolygonInfo::RemoveFromLocalSnippetVector(snippet_vector& snip_vec_local ,
			        GridIndexType which_one)
			{
				snippet_vector_iterator iter;

				if (snip_vec_local.size() > which_one)
				{
					iter = snip_vec_local.begin() + which_one;
					snip_vec_local.erase(iter);
				}
			}

			void PolygonInfo::GetSectorsAndSetCharacteristicsDoubleCheck(SnippetIndex edge_index,
			        LegacyWorldCoordinate2D edge_coord , LegacyWorldCoordinate3D& contour_point_one ,
			        LegacyWorldCoordinate3D& contour_point_two)
			{
				PolygonSnippet& a_snippet_ref = pol_inf_vec.at(edge_index.edge).at(edge_index.snip);

				WhichSector sector_first_point = not_known;
				WhichSector sector_second_point = not_known;

				WhichSector& sector_first_point_ref = sector_first_point;
				WhichSector& sector_second_point_ref = sector_second_point;

				GetSectors(edge_coord , contour_point_one , contour_point_two , sector_first_point_ref ,
				           sector_second_point_ref);

				if (edge_index.edge  == 0)
				{
					CharacterizeIntersection charact_double_check = GetCharacteristicFirstCorner(
					            sector_first_point_ref , sector_second_point_ref);

					if ((a_snippet_ref.characterize != charact_double_check) && (a_snippet_ref.characterize != corner))
					{
						std::cout <<
						          " Have double checked the characteristic of corner zero. It turned out not to be determined correctly ! "
						          << std::endl;
						assert(0);
					}
				}
				else if (edge_index.edge  == 1)
				{
					CharacterizeIntersection charact_double_check = GetCharacteristicSecondCorner(
					            sector_first_point_ref , sector_second_point_ref);

					if ((a_snippet_ref.characterize != charact_double_check) && (a_snippet_ref.characterize != corner))
					{
						std::cout <<
						          " Have double checked the characteristic of corner one. It turned out not to be determined correctly ! "
						          << std::endl;
						assert(0);
					}
				}
				else if (edge_index.edge  == 2)
				{
					CharacterizeIntersection charact_double_check = GetCharacteristicThirdCorner(
					            sector_first_point_ref , sector_second_point_ref);

					if ((a_snippet_ref.characterize != charact_double_check) && (a_snippet_ref.characterize != corner))
					{
						std::cout <<
						          " Have double checked the characteristic of corner two. It turned out not to be determined correctly ! "
						          << std::endl;
						assert(0);
					}
				}
				else if (edge_index.edge  == 3)
				{
					CharacterizeIntersection charact_double_check = GetCharacteristicFourthCorner(
					            sector_first_point_ref , sector_second_point_ref);

					if ((a_snippet_ref.characterize != charact_double_check) && (a_snippet_ref.characterize != corner))
					{
						std::cout <<
						          " Have double checked the characteristic of corner three. It turned out not to be determined correctly ! "
						          << std::endl;
						assert(0);
					}
				}
				else
				{
					assert(0);
				}
			}

			void PolygonInfo::GetSectors(LegacyWorldCoordinate2D edge_coord ,
			                             LegacyWorldCoordinate3D& contour_point_one , LegacyWorldCoordinate3D& contour_point_two,
			                             WhichSector& sector_first_point_ref ,  WhichSector& sector_second_point_ref)
			{
				sector_first_point_ref = not_known;
				sector_second_point_ref = not_known;

				if ((contour_point_one.x == edge_coord.x) && (contour_point_one.y == edge_coord.y))
				{
					assert(0);
				}

				if ((contour_point_two.x == edge_coord.x) && (contour_point_two.y == edge_coord.y))
				{
					assert(0);
				}

				GetSectorFirstPoint(edge_coord , contour_point_one , sector_first_point_ref);
				GetSectorSecondPoint(edge_coord , contour_point_two, sector_second_point_ref);

				if ((sector_first_point_ref == not_known) || (sector_second_point_ref == not_known))
				{
					assert(0);
				}
			}

			void PolygonInfo::GetSectorFirstPoint(LegacyWorldCoordinate2D edge_coord ,
			                                      LegacyWorldCoordinate3D& contour_point_one , WhichSector& sector_first_point_ref)
			{
				if ((contour_point_one.x == edge_coord.x) && (contour_point_one.y < edge_coord.y))
				{
					sector_first_point_ref = on_edge_twelve_o_clock;
					return;
				}

				if ((contour_point_one.x > edge_coord.x) && (contour_point_one.y < edge_coord.y))
				{
					sector_first_point_ref = section_twelve_to_three_o_clock;
					return;
				}

				if ((contour_point_one.x > edge_coord.x) && (contour_point_one.y == edge_coord.y))
				{
					sector_first_point_ref = on_edge_three_o_clock;
					return;
				}

				if ((contour_point_one.x > edge_coord.x) && (contour_point_one.y > edge_coord.y))
				{
					sector_first_point_ref = section_three_to_six_o_clock;
					return;
				}

				if ((contour_point_one.x == edge_coord.x) && (contour_point_one.y > edge_coord.y))
				{
					sector_first_point_ref = on_edge_six_o_clock;
					return;
				}

				if ((contour_point_one.x < edge_coord.x) && (contour_point_one.y > edge_coord.y))
				{
					sector_first_point_ref = section_six_to_nine_o_clock;
					return;
				}

				if ((contour_point_one.x < edge_coord.x) && (contour_point_one.y == edge_coord.y))
				{
					sector_first_point_ref = on_edge_nine_o_clock;
					return;
				}

				if ((contour_point_one.x < edge_coord.x) && (contour_point_one.y < edge_coord.y))
				{
					sector_first_point_ref = section_nine_to_twelve_o_clock;
					return;
				}
			}

			void PolygonInfo::GetSectorSecondPoint(LegacyWorldCoordinate2D edge_coord ,
			                                       LegacyWorldCoordinate3D& contour_point_two, WhichSector& sector_second_point_ref)
			{
				if ((contour_point_two.x == edge_coord.x) && (contour_point_two.y < edge_coord.y))
				{
					sector_second_point_ref = on_edge_twelve_o_clock;
					return;
				}

				if ((contour_point_two.x > edge_coord.x) && (contour_point_two.y < edge_coord.y))
				{
					sector_second_point_ref = section_twelve_to_three_o_clock;
					return;
				}

				if ((contour_point_two.x > edge_coord.x) && (contour_point_two.y == edge_coord.y))
				{
					sector_second_point_ref = on_edge_three_o_clock;
					return;
				}

				if ((contour_point_two.x > edge_coord.x) && (contour_point_two.y > edge_coord.y))
				{
					sector_second_point_ref = section_three_to_six_o_clock;
					return;
				}

				if ((contour_point_two.x == edge_coord.x) && (contour_point_two.y > edge_coord.y))
				{
					sector_second_point_ref = on_edge_six_o_clock;
					return;
				}

				if ((contour_point_two.x < edge_coord.x) && (contour_point_two.y > edge_coord.y))
				{
					sector_second_point_ref = section_six_to_nine_o_clock;
					return;
				}

				if ((contour_point_two.x < edge_coord.x) && (contour_point_two.y == edge_coord.y))
				{
					sector_second_point_ref = on_edge_nine_o_clock;
					return;
				}

				if ((contour_point_two.x < edge_coord.x) && (contour_point_two.y < edge_coord.y))
				{
					sector_second_point_ref = section_nine_to_twelve_o_clock;
					return;
				}
			}

			CharacterizeIntersection PolygonInfo::GetCharacteristicFirstCorner(WhichSector&
			        sector_first_point_ref ,  WhichSector& sector_second_point_ref)
			{
				CharacterizeIntersection charact = unknown;
				CharacterizeIntersection& charact_ref = charact;

				std::vector<WhichSector> in_combination_vector;
				std::vector<WhichSector>& in_combination_vector_ref = in_combination_vector;
				GetInCombinationFirstCorner(in_combination_vector_ref);

				WhichSector inside;
				WhichSector& inside_ref = inside;
				GetInsideFirstCorner(inside_ref);

				WhichSector preceding_edge;
				WhichSector& preceding_edge_ref = preceding_edge;
				GetPrecedingEdgeFirstCorner(preceding_edge_ref);

				WhichSector following_edge;
				WhichSector& following_edge_ref = following_edge;
				GetFollowingEdgeFirstCorner(following_edge_ref);

				DoCharacterize(sector_first_point_ref ,  sector_second_point_ref , charact_ref ,
				               in_combination_vector ,  inside_ref , preceding_edge_ref , following_edge_ref);

				if (charact == unknown)
				{
					assert(0);
				}

				return charact;
			}

			void PolygonInfo::DoCharacterize(WhichSector& sector_first_point_ref ,
			                                 WhichSector& sector_second_point_ref , CharacterizeIntersection& charact_ref ,
			                                 std::vector<WhichSector> in_combination_vector , WhichSector& inside_ref ,
			                                 WhichSector& preceding_edge_ref , WhichSector& following_edge_ref)
			{
				if (BothOnEdge(sector_first_point_ref ,  sector_second_point_ref , preceding_edge_ref ,
				               following_edge_ref))
				{
					charact_ref = edge_to_edge;
				}
				else if (FromEdgeIn(sector_first_point_ref ,  sector_second_point_ref , preceding_edge_ref ,
				                    inside_ref))
				{
					charact_ref = edge_to_inside;
				}
				else if (FromEdgeOut(sector_first_point_ref ,  sector_second_point_ref , preceding_edge_ref ,
				                     in_combination_vector))
				{
					charact_ref = edge_to_outside;
				}
				else if (OutToEdge(sector_first_point_ref ,  sector_second_point_ref , following_edge_ref ,
				                   in_combination_vector))
				{
					charact_ref = outside_to_edge;
				}
				else if (InToEdge(sector_first_point_ref ,  sector_second_point_ref ,  following_edge_ref ,
				                  inside_ref))
				{
					charact_ref = inside_to_edge;
				}
				else if (BothIn(sector_first_point_ref ,  sector_second_point_ref , inside_ref))
				{
					charact_ref = inside_inside_touches;
				}
				else if (BothOut(sector_first_point_ref ,  sector_second_point_ref ,  in_combination_vector))
				{
					charact_ref = outside_outside_touches;
				}
				else if (OneInOneOut(sector_first_point_ref ,  sector_second_point_ref , inside_ref ,
				                     in_combination_vector))
				{
					charact_ref = real_intersection;
				}
				else
				{
					assert(0);
				}
			}


			void PolygonInfo::GetInCombinationFirstCorner(std::vector<WhichSector>& in_combination_vector_ref)
			{

				WhichSector sec_three_to_six = section_three_to_six_o_clock;
				in_combination_vector_ref.push_back(sec_three_to_six);

				WhichSector sec_on_edge_six = on_edge_six_o_clock;
				in_combination_vector_ref.push_back(sec_on_edge_six);

				WhichSector sec_on_edge_three = on_edge_three_o_clock;
				in_combination_vector_ref.push_back(sec_on_edge_three);

			}


			void PolygonInfo::GetInsideFirstCorner(WhichSector& inside_ref)
			{
				inside_ref = section_three_to_six_o_clock;
			}


			void PolygonInfo::GetPrecedingEdgeFirstCorner(WhichSector& preceding_edge_ref)
			{
				preceding_edge_ref = on_edge_six_o_clock;
			}


			void PolygonInfo::GetFollowingEdgeFirstCorner(WhichSector& following_edge_ref)
			{
				following_edge_ref = on_edge_three_o_clock;
			}



			bool PolygonInfo::BothOnEdge(WhichSector& sector_first_point_ref ,
			                             WhichSector& sector_second_point_ref , WhichSector& preceding_edge_ref ,
			                             WhichSector& following_edge_ref)
			{

				std::vector<WhichSector> first_sector_vector;
				WhichSector sec_following_edge = following_edge_ref;
				first_sector_vector.push_back(sec_following_edge);

				std::vector<WhichSector> second_sector_vector;
				WhichSector sec_preceding = preceding_edge_ref;
				second_sector_vector.push_back(sec_preceding);

				bool it_is_a = FirstSectorIsOneOfTheseAndSecondSectorIsOneOfThose(sector_first_point_ref ,
				               first_sector_vector  , sector_second_point_ref , second_sector_vector);
				bool it_is_b = FirstSectorIsOneOfTheseAndSecondSectorIsOneOfThose(sector_second_point_ref ,
				               first_sector_vector  , sector_first_point_ref , second_sector_vector);

				if (it_is_a || it_is_b)
				{
					return true;
				}
				else
				{
					return false;
				}

			}


			bool PolygonInfo::FromEdgeIn(WhichSector& sector_first_point_ref ,
			                             WhichSector& sector_second_point_ref , WhichSector& preceding_edge_ref , WhichSector& inside_ref)
			{

				std::vector<WhichSector> first_sector_vector;
				WhichSector sec_preceding = preceding_edge_ref;
				first_sector_vector.push_back(sec_preceding);

				std::vector<WhichSector> second_sector_vector;
				WhichSector sec_inside = inside_ref;
				second_sector_vector.push_back(sec_inside);

				bool it_is_a = FirstSectorIsOneOfTheseAndSecondSectorIsOneOfThose(sector_first_point_ref ,
				               first_sector_vector  , sector_second_point_ref , second_sector_vector);
				bool it_is_b = FirstSectorIsOneOfTheseAndSecondSectorIsOneOfThose(sector_second_point_ref ,
				               first_sector_vector  , sector_first_point_ref , second_sector_vector);

				if (it_is_a || it_is_b)
				{
					return true;
				}
				else
				{
					return false;
				}

			}



			bool PolygonInfo::FromEdgeOut(WhichSector& sector_first_point_ref ,
			                              WhichSector& sector_second_point_ref , WhichSector& preceding_edge_ref ,
			                              std::vector<WhichSector> second_sector_vector)
			{

				std::vector<WhichSector> first_sector_vector;
				WhichSector sec_preceding = preceding_edge_ref;
				first_sector_vector.push_back(sec_preceding);

				/*std::vector<WhichSector> second_sector_vector;
				WhichSector sec_three_to_six = section_three_to_six_o_clock;
				second_sector_vector.push_back( sec_three_to_six );

				WhichSector sec_on_edge_six = on_edge_six_o_clock;
				second_sector_vector.push_back( sec_on_edge_six );

				WhichSector sec_on_edge_three = on_edge_three_o_clock;
				second_sector_vector.push_back( sec_on_edge_three );*/



				bool it_is_a = FirstSectorIsOneOfTheseAndSecondSectorIsNoneOfThose(sector_first_point_ref ,
				               first_sector_vector  , sector_second_point_ref , second_sector_vector);
				bool it_is_b = FirstSectorIsOneOfTheseAndSecondSectorIsNoneOfThose(sector_second_point_ref ,
				               first_sector_vector  , sector_first_point_ref , second_sector_vector);

				if (it_is_a || it_is_b)
				{
					return true;
				}
				else
				{
					return false;
				}

			}




			bool PolygonInfo::OutToEdge(WhichSector& sector_first_point_ref ,
			                            WhichSector& sector_second_point_ref , WhichSector& following_edge_ref ,
			                            std::vector<WhichSector> second_sector_vector)
			{

				std::vector<WhichSector> first_sector_vector;
				WhichSector sec_following = following_edge_ref;
				first_sector_vector.push_back(sec_following);

				/* std::vector<WhichSector> second_sector_vector;
				WhichSector sec_three_to_six = section_three_to_six_o_clock;
				second_sector_vector.push_back( sec_three_to_six );

				WhichSector sec_on_edge_six = on_edge_six_o_clock;
				second_sector_vector.push_back( sec_on_edge_six );

				WhichSector sec_on_edge_three = on_edge_three_o_clock;
				second_sector_vector.push_back( sec_on_edge_three ); */

				bool it_is_a = FirstSectorIsOneOfTheseAndSecondSectorIsNoneOfThose(sector_first_point_ref ,
				               first_sector_vector  , sector_second_point_ref , second_sector_vector);
				bool it_is_b = FirstSectorIsOneOfTheseAndSecondSectorIsNoneOfThose(sector_second_point_ref ,
				               first_sector_vector  , sector_first_point_ref , second_sector_vector);

				if (it_is_a || it_is_b)
				{
					return true;
				}
				else
				{
					return false;
				}

			}




			bool PolygonInfo::InToEdge(WhichSector& sector_first_point_ref ,
			                           WhichSector& sector_second_point_ref , WhichSector& following_edge_ref , WhichSector& inside_ref)
			{

				std::vector<WhichSector> first_sector_vector;
				WhichSector sec_following_edge = following_edge_ref;
				first_sector_vector.push_back(sec_following_edge);

				std::vector<WhichSector> second_sector_vector;
				WhichSector sec_inside = inside_ref;
				second_sector_vector.push_back(sec_inside);

				bool it_is_a = FirstSectorIsOneOfTheseAndSecondSectorIsOneOfThose(sector_first_point_ref ,
				               first_sector_vector  , sector_second_point_ref , second_sector_vector);
				bool it_is_b = FirstSectorIsOneOfTheseAndSecondSectorIsOneOfThose(sector_second_point_ref ,
				               first_sector_vector  , sector_first_point_ref , second_sector_vector);

				if (it_is_a || it_is_b)
				{
					return true;
				}
				else
				{
					return false;
				}

			}




			bool PolygonInfo::BothIn(WhichSector& sector_first_point_ref ,
			                         WhichSector& sector_second_point_ref , WhichSector& inside_ref)
			{

				std::vector<WhichSector> sector_vector;
				WhichSector sec_inside = inside_ref;
				sector_vector.push_back(sec_inside);

				bool it_is_a = FirstSectorIsOneOfTheseAndSecondSectorIsOneOfThose(sector_first_point_ref ,
				               sector_vector  , sector_second_point_ref , sector_vector);

				if (it_is_a)
				{
					return true;
				}
				else
				{
					return false;
				}

			}


			bool PolygonInfo::BothOut(WhichSector& sector_first_point_ref ,
			                          WhichSector& sector_second_point_ref , std::vector<WhichSector> sector_vector)
			{

				/*std::vector<WhichSector> sector_vector;
				WhichSector sec_three_to_six = section_three_to_six_o_clock;
				sector_vector.push_back( sec_three_to_six );

				WhichSector sec_on_edge_six = on_edge_six_o_clock;
				sector_vector.push_back( sec_on_edge_six );

				WhichSector sec_on_edge_three = on_edge_three_o_clock;
				sector_vector.push_back( sec_on_edge_three );*/

				bool it_is_a = FirstSectorIsNoneOfTheseAndSecondSectorIsNoneOfThose(sector_first_point_ref ,
				               sector_vector  , sector_second_point_ref , sector_vector);

				if (it_is_a)
				{
					return true;
				}
				else
				{
					return false;
				}


			}



			bool PolygonInfo::OneInOneOut(WhichSector& sector_first_point_ref ,
			                              WhichSector& sector_second_point_ref , WhichSector& inside_ref ,
			                              std::vector<WhichSector> first_sector_vector)
			{

				/*std::vector<WhichSector> first_sector_vector;
				WhichSector sec_three_to_six = section_three_to_six_o_clock;
				first_sector_vector.push_back( sec_three_to_six );

				WhichSector sec_on_edge_six = on_edge_six_o_clock;
				first_sector_vector.push_back( sec_on_edge_six );

				WhichSector sec_on_edge_three = on_edge_three_o_clock;
				first_sector_vector.push_back( sec_on_edge_three );*/

				std::vector<WhichSector> second_sector_vector;
				WhichSector sec_inside = inside_ref;
				second_sector_vector.push_back(sec_inside);

				bool it_is_a = FirstSectorIsOneOfTheseAndSecondSectorIsNoneOfThose(sector_first_point_ref  ,
				               second_sector_vector  , sector_second_point_ref ,  first_sector_vector);
				bool it_is_b = FirstSectorIsOneOfTheseAndSecondSectorIsNoneOfThose(sector_second_point_ref ,
				               second_sector_vector  , sector_first_point_ref  ,  first_sector_vector);

				if (it_is_a || it_is_b)
				{
					return true;
				}
				else
				{
					return false;
				}

			}




			bool PolygonInfo::FirstSectorIsOneOfTheseAndSecondSectorIsOneOfThose(WhichSector&
			        sector_first_point_ref , std::vector<WhichSector> first_sector_vector ,
			        WhichSector& sector_second_point_ref , std::vector<WhichSector> second_sector_vector)
			{

				bool first_one_of_these = false;
				bool second_one_of_those = false;

				for (unsigned int index = 0 ; index < first_sector_vector.size() ; index++)
				{
					if (first_sector_vector.at(index) == sector_first_point_ref)
					{
						first_one_of_these = true;
					}
				}

				for (unsigned int index = 0 ; index < second_sector_vector.size() ; index++)
				{
					if (second_sector_vector.at(index) == sector_second_point_ref)
					{
						second_one_of_those = true;
					}
				}

				if (first_one_of_these && second_one_of_those)
				{
					return true;
				}
				else
				{
					return false;
				}

			}




			bool PolygonInfo::FirstSectorIsOneOfTheseAndSecondSectorIsNoneOfThose(
			    WhichSector& sector_first_point_ref , std::vector<WhichSector> first_sector_vector ,
			    WhichSector& sector_second_point_ref , std::vector<WhichSector> second_sector_vector)
			{

				bool first_one_of_these = false;
				bool second_none_of_those = true;

				for (unsigned int index = 0 ; index < first_sector_vector.size() ; index++)
				{
					if (first_sector_vector.at(index) == sector_first_point_ref)
					{
						first_one_of_these = true;
					}
				}

				for (unsigned int index = 0 ; index < second_sector_vector.size() ; index++)
				{
					if (second_sector_vector.at(index) == sector_second_point_ref)
					{
						second_none_of_those = false;
					}
				}

				if (first_one_of_these && second_none_of_those)
				{
					return true;
				}
				else
				{
					return false;
				}

			}




			bool PolygonInfo::FirstSectorIsNoneOfTheseAndSecondSectorIsNoneOfThose(
			    WhichSector& sector_first_point_ref , std::vector<WhichSector> first_sector_vector ,
			    WhichSector& sector_second_point_ref , std::vector<WhichSector> second_sector_vector)
			{

				bool first_none_of_these = true;
				bool second_none_of_those = true;

				for (unsigned int index = 0 ; index < first_sector_vector.size() ; index++)
				{
					if (first_sector_vector.at(index) == sector_first_point_ref)
					{
						first_none_of_these = false;
					}
				}

				for (unsigned int index = 0 ; index < second_sector_vector.size() ; index++)
				{
					if (second_sector_vector.at(index) == sector_second_point_ref)
					{
						second_none_of_those = false;
					}
				}

				if (first_none_of_these && second_none_of_those)
				{
					return true;
				}
				else
				{
					return false;
				}

			}



			CharacterizeIntersection PolygonInfo::GetCharacteristicSecondCorner(WhichSector&
			        sector_first_point_ref ,  WhichSector& sector_second_point_ref)
			{

				CharacterizeIntersection charact = unknown;
				CharacterizeIntersection& charact_ref = charact;

				std::vector<WhichSector> in_combination_vector;
				std::vector<WhichSector>& in_combination_vector_ref = in_combination_vector;
				GetInCombinationSecondCorner(in_combination_vector_ref);

				WhichSector inside;
				WhichSector& inside_ref = inside;
				GetInsideSecondCorner(inside_ref);

				WhichSector preceding_edge;
				WhichSector& preceding_edge_ref = preceding_edge;
				GetPrecedingEdgeSecondCorner(preceding_edge_ref);

				WhichSector following_edge;
				WhichSector& following_edge_ref = following_edge;
				GetFollowingEdgeSecondCorner(following_edge_ref);

				DoCharacterize(sector_first_point_ref ,  sector_second_point_ref , charact_ref ,
				               in_combination_vector ,  inside_ref , preceding_edge_ref , following_edge_ref);

				if (charact == unknown)
				{
					assert(0);
				}

				return charact;

			}




			void PolygonInfo::GetInCombinationSecondCorner(std::vector<WhichSector>& in_combination_vector_ref)
			{

				WhichSector sec_six_to_nine = section_six_to_nine_o_clock;
				in_combination_vector_ref.push_back(sec_six_to_nine);

				WhichSector sec_on_edge_nine = on_edge_nine_o_clock;
				in_combination_vector_ref.push_back(sec_on_edge_nine);

				WhichSector sec_on_edge_six = on_edge_six_o_clock;
				in_combination_vector_ref.push_back(sec_on_edge_six);

			}



			void PolygonInfo::GetInsideSecondCorner(WhichSector& inside_ref)
			{
				inside_ref = section_six_to_nine_o_clock;
			}



			void PolygonInfo::GetPrecedingEdgeSecondCorner(WhichSector& preceding_edge_ref)
			{
				preceding_edge_ref = on_edge_nine_o_clock;
			}



			void PolygonInfo::GetFollowingEdgeSecondCorner(WhichSector& following_edge_ref)
			{
				following_edge_ref = on_edge_six_o_clock;
			}



			CharacterizeIntersection PolygonInfo::GetCharacteristicThirdCorner(WhichSector&
			        sector_first_point_ref ,  WhichSector& sector_second_point_ref)
			{

				CharacterizeIntersection charact = unknown;
				CharacterizeIntersection& charact_ref = charact;

				std::vector<WhichSector> in_combination_vector;
				std::vector<WhichSector>& in_combination_vector_ref = in_combination_vector;
				GetInCombinationThirdCorner(in_combination_vector_ref);

				WhichSector inside;
				WhichSector& inside_ref = inside;
				GetInsideThirdCorner(inside_ref);

				WhichSector preceding_edge;
				WhichSector& preceding_edge_ref = preceding_edge;
				GetPrecedingEdgeThirdCorner(preceding_edge_ref);

				WhichSector following_edge;
				WhichSector& following_edge_ref = following_edge;
				GetFollowingEdgeThirdCorner(following_edge_ref);

				DoCharacterize(sector_first_point_ref ,  sector_second_point_ref , charact_ref ,
				               in_combination_vector ,  inside_ref , preceding_edge_ref , following_edge_ref);

				if (charact == unknown)
				{
					assert(0);
				}

				return charact;

			}



			void PolygonInfo::GetInCombinationThirdCorner(std::vector<WhichSector>& in_combination_vector_ref)
			{

				WhichSector sec_nine_to_twelve = section_nine_to_twelve_o_clock;
				in_combination_vector_ref.push_back(sec_nine_to_twelve);

				WhichSector sec_on_edge_twelve = on_edge_twelve_o_clock;
				in_combination_vector_ref.push_back(sec_on_edge_twelve);

				WhichSector sec_on_edge_nine = on_edge_nine_o_clock;
				in_combination_vector_ref.push_back(sec_on_edge_nine);

			}


			void PolygonInfo::GetInsideThirdCorner(WhichSector& inside_ref)
			{
				inside_ref = section_nine_to_twelve_o_clock;
			}


			void PolygonInfo::GetPrecedingEdgeThirdCorner(WhichSector& preceding_edge_ref)
			{
				preceding_edge_ref = on_edge_twelve_o_clock;
			}


			void PolygonInfo::GetFollowingEdgeThirdCorner(WhichSector& following_edge_ref)
			{
				following_edge_ref = on_edge_nine_o_clock;
			}



			CharacterizeIntersection PolygonInfo::GetCharacteristicFourthCorner(WhichSector&
			        sector_first_point_ref ,  WhichSector& sector_second_point_ref)
			{

				CharacterizeIntersection charact = unknown;
				CharacterizeIntersection& charact_ref = charact;

				std::vector<WhichSector> in_combination_vector;
				std::vector<WhichSector>& in_combination_vector_ref = in_combination_vector;
				GetInCombinationFourthCorner(in_combination_vector_ref);

				WhichSector inside;
				WhichSector& inside_ref = inside;
				GetInsideFourthCorner(inside_ref);

				WhichSector preceding_edge;
				WhichSector& preceding_edge_ref = preceding_edge;
				GetPrecedingEdgeFourthCorner(preceding_edge_ref);

				WhichSector following_edge;
				WhichSector& following_edge_ref = following_edge;
				GetFollowingEdgeFourthCorner(following_edge_ref);

				DoCharacterize(sector_first_point_ref ,  sector_second_point_ref , charact_ref ,
				               in_combination_vector ,  inside_ref , preceding_edge_ref , following_edge_ref);

				if (charact == unknown)
				{
					assert(0);
				}

				return charact;

			}


			void PolygonInfo::GetInCombinationFourthCorner(std::vector<WhichSector>& in_combination_vector_ref)
			{

				WhichSector sec_twelve_to_three = section_twelve_to_three_o_clock;
				in_combination_vector_ref.push_back(sec_twelve_to_three);

				WhichSector sec_on_edge_three = on_edge_three_o_clock;
				in_combination_vector_ref.push_back(sec_on_edge_three);

				WhichSector sec_on_edge_twelve = on_edge_twelve_o_clock;
				in_combination_vector_ref.push_back(sec_on_edge_twelve);

			}


			void PolygonInfo::GetInsideFourthCorner(WhichSector& inside_ref)
			{
				inside_ref = section_twelve_to_three_o_clock;
			}


			void PolygonInfo::GetPrecedingEdgeFourthCorner(WhichSector& preceding_edge_ref)
			{
				preceding_edge_ref = on_edge_three_o_clock;
			}


			void PolygonInfo::GetFollowingEdgeFourthCorner(WhichSector& following_edge_ref)
			{
				following_edge_ref = on_edge_twelve_o_clock;
			}



			bool PolygonInfo::AppendNextIntersectionAndCorrespoindingPolygonSnippets(
			    snippet_vector& snip_vec_ref_local , snippet_vector& snip_vec_ref , int current_edge,
			    VoxelIntersectionIndex& next_voxel_intersection)
			{

				bool finished = false;
				bool im_done = false;

				LegacyWorldCoordinate2D closest_local_current_position;
				LegacyWorldCoordinate2D& closest_local_current_position_ref = closest_local_current_position;
				closest_local_current_position.x = current_position.x;
				closest_local_current_position.y = current_position.y;

				while (im_done == false)
				{

					closest_local_current_position.x = current_position.x;
					closest_local_current_position.y = current_position.y;

					// double distance_current_position = GetDistanceAlongEdge( the_dose_index , current_position  );

					// die folgende Funktion macht den naechstliegenden Schnittpunkt ausfindig gespeichert in closest_local_current_position_ref
					// the_intersection_index speichert die Information ueber seine Lage im Polygonzug
					// hier wird untersucht, ob es einen naechsten Schnittpunkt gibt

					//  float distance_next_intersection = SetToNextIntersectionAlongThisEdgeAndReturnDistance( closest_local_current_position_ref , distance_current_position  , current_edge );

					unsigned int the_index = 0;
					SnippetIndex edge_index;
					edge_index.edge = 0;
					edge_index.snip = 0;

					bool got_one = 0;

					got_one = SetToNextIntersectonAlongThisEdgeAndReturnDistanceBasedOnVoxelIntersections(
					              closest_local_current_position_ref, current_edge, next_voxel_intersection);


					//if( distance_next_intersection != ( 1000000 )  ){       // gibt es einen naechsten Schnittpunkt?
					if (got_one)
					{

						// Was der folgende Funktionsaufruf alles tut :
						// Hier in AddSnippetsThisIntersection wird untersucht, ob zu dem gefundenen Schnittpunkt
						// ein Polygon existiert das innerhalb des Voxels liegt, nur dann, wenn das der Fall ist, wird an snip_vec_ref angehaengt.
						// Falls es einen geeigneten Schnittpunkt gibt, all die zugehoerigen
						// Polygonsnippets anhaengen.
						// Auch wird durch die Funktion AddSnippetsThisIntersection untersucht, ob man schon ausserhalb der betrachteten Kante ist.

						if (AddSnippetsThisIntersection(snip_vec_ref_local , closest_local_current_position))
						{

							im_done = true;
							finished = false;
							// Falls Polygone gefunden im_done auf true und finished auf false setzen.
							// Das bedeutet, an dieser Kante wird weitergearbeitet. Vielleicht gibt es noch mehr Schnittpunkte.

						}

						// falls kein drinnen liegendes Polygonstueck gefunden wurde, welches im Bereich der betrachteten Kante liegt
						// bleibt im_done auf false und es wird weiter gesucht
						// die current_position wird dann auf den zuletzt gefundenen Schnittpunkt gesetzt, zu dem es kein Polygonstueck gab
						current_position.x = closest_local_current_position.x;
						current_position.y = closest_local_current_position.y;


					}
					else
					{

						// es gibt keinen weiteren Schnittpunkt mehr
						im_done = true;
						finished = true;

					}

					bool changed_loc = false;

					IncrementVoxelIntersectionIndex(current_edge, next_voxel_intersection , changed_loc);

					if (changed_loc)
					{

						if (snip_vec_ref_local.size() > 1)
						{
							snip_vec_ref_local = SortClockwise(snip_vec_ref_local);
						}

						for (GridIndexType counter = 0 ; counter < snip_vec_ref_local.size() ; counter++)
						{

							snip_vec_ref.push_back(snip_vec_ref_local.at(counter));

						}

						snip_vec_ref_local.resize(0);

					}

				}

				return finished;

			}


			void PolygonInfo::IncrementVoxelIntersectionIndex(int current_edge,
			        VoxelIntersectionIndex& vox_inters_index , bool& changed_location)
			{

				if (voxel_intersections.edge_intersections_intersection_coord.at(current_edge).size() >
				    vox_inters_index.next_intersection)
				{

					if (voxel_intersections.edge_intersections_intersection_coord.at(current_edge).at(
					        vox_inters_index.next_intersection).size() > (vox_inters_index.intersection_douplication + 1))
					{

						vox_inters_index.intersection_douplication++;

					}
					else
					{

						vox_inters_index.intersection_douplication = 0;
						vox_inters_index.next_intersection++;
						changed_location = true;

					}

				}

			}


			// Achtung, bei der verallgemeinerung, die Beruehrpunkte erlaubt, muss hier eine Veraenderung vor werden.
			// Es muessen auch naechste Punkte mit Abstand wie der bisherige erlaubt sien.
			// Anmerkung : ist insofern erledigt, dass diese Funktion hier nicht mehr verwendet wird.
			float PolygonInfo::SetToNextIntersectionAlongThisEdgeAndReturnDistance(
			    LegacyWorldCoordinate2D& closest_local_current_position_ref , float distance_current_position ,
			    int current_edge)
			{

				// Hier wird im Uhrzeigersinn vorwaerts gegangen um den Schnittpunkt zu finden, der dem aktuell betrachteten Schnittpunkt
				// am naechsten liegt. Im Uhrzeigersinn zurueck liegende Punkte werden dabei ignoriert.
				// Natuerlich ist jeder Punkt sich selbst der Naechste. Selbst wird daher ignoriert.
				// diese Funktion gibt -1000000 zurueck, falls kein Punkt gefunden werden konnte.
				// Die Distanz wird im Uhrzeigersinn der Voxelkante entlang gemessen.
				// Erkannt und gesucht wird nur was auf der betrachteten Voxelkante liegt.
				// Es wird der Abstand zu current_position, eindimensional entlang der
				// Voxelkante zurueckgegeben.

				float distance_current_position_local = 0;
				double distance_next_edge = 0;
				float min_local_distance = 1000000;
				float min_local_distance_along_edge = 1000000;

				LegacyWorldCoordinate2D local_current_position;
				local_current_position.x = current_position.x;
				local_current_position.y = current_position.y;



				float local_distance;
				local_distance = -1000000;

				it_poly.RememberPosition();

				it_poly.ResetIntersectionIndex();

				bool was_able_to_increment = true;


				while (was_able_to_increment == true)
				{

					// if( it_poly.CheckToBeRegarded() ){


					if (it_poly.ThisIntersection(local_current_position))
					{

						if (IsTheSameEdgeButNoCorner(local_current_position , current_position , current_edge))
						{



							distance_current_position_local = GetDistanceAlongEdge(the_dose_index , local_current_position);
							distance_next_edge = GetDistanceNextEdge(current_edge);


							if ((static_cast<float>(distance_current_position_local) > static_cast<float>
							     (distance_current_position))
							    && (static_cast<float>(distance_next_edge) > static_cast<float>
							        (distance_current_position_local)))     // betrachtet werden nur die Schnittpunkte, die im Uhrzeigersinn
							{
								// entlanng der Voxelkante weiter entfernt sind
								// echt groesser schliesst Detektion des betracteten Punktes selbst aus


								local_distance = GetPeriodicDist(distance_current_position_local , distance_current_position);


								if (local_distance < min_local_distance)
								{

									min_local_distance =  local_distance;
									min_local_distance_along_edge = distance_current_position_local;

									closest_local_current_position_ref.x = local_current_position.x;
									closest_local_current_position_ref.y = local_current_position.y;

									it_poly.RememberPosition();

								}

							}

						}

						// }  // check to be regarded

					}

					was_able_to_increment = it_poly.IncremtentIntersection();

				}

				it_poly.JumpToRememberedPosition();
				return min_local_distance_along_edge;

			}




			// Es ist noetig zu prufen, ob Ecke, wegen Rundungsfehlern. Hier muss auf genau die selbe Art festgestellt werden ob Ecke wie bei der unabhaengigen Betrachtung der Ecken.
			// nein, das ist doch nicht noetig. Diese Funktion wird nicht mehr verwendet
			bool PolygonInfo::IsTheSameEdgeButNoCorner(LegacyWorldCoordinate2D local_current_position ,
			        LegacyWorldCoordinate2D current_position , int current_edge)
			{

				LegacyWorldCoordinate2D corner_coord;
				corner_coord.x = static_cast<WorldCoordinate>(the_dose_index.x);
				corner_coord.y = static_cast<WorldCoordinate>(the_dose_index.y);

				switch (current_edge)
				{

					case 0 :

						if ((local_current_position.y == current_position.y)
						    && (local_current_position.x <= (the_dose_index.x + 1))
						    && (local_current_position.x >= the_dose_index.x) && (the_dose_index.y == local_current_position.y))
						{

							if ((static_cast<float>(corner_coord.x) == static_cast<float>(local_current_position.x))
							    && (static_cast<float>(corner_coord.y) == static_cast<float>(local_current_position.y)))
							{
								return false;
							}

							corner_coord.x += 1;

							if ((static_cast<float>(corner_coord.x) == static_cast<float>(local_current_position.x))
							    && (static_cast<float>(corner_coord.y) == static_cast<float>(local_current_position.y)))
							{
								return false;
							}

							return true;

						}
						else
						{
							return false;
						}

						break;

					case 1 :

						if ((local_current_position.x == current_position.x)
						    && (local_current_position.y <= (the_dose_index.y + 1))
						    && (local_current_position.y >= the_dose_index.y)
						    && ((the_dose_index.x + 1) == local_current_position.x))
						{
							corner_coord.x += 1;

							if ((static_cast<float>(corner_coord.x) == static_cast<float>(local_current_position.x))
							    && (static_cast<float>(corner_coord.y) == static_cast<float>(local_current_position.y)))
							{
								return false;
							}

							corner_coord.y += 1;

							if ((static_cast<float>(corner_coord.x) == static_cast<float>(local_current_position.x))
							    && (static_cast<float>(corner_coord.y) == static_cast<float>(local_current_position.y)))
							{
								return false;
							}

							return true;
						}
						else
						{
							return false;
						}

						break;

					case 2 :

						if ((local_current_position.y == current_position.y)
						    && (local_current_position.x <= (the_dose_index.x + 1))
						    && (local_current_position.x >= the_dose_index.x)
						    && ((the_dose_index.y + 1) == local_current_position.y))
						{
							corner_coord.x += 1;
							corner_coord.y += 1;

							if ((static_cast<float>(corner_coord.x) == static_cast<float>(local_current_position.x))
							    && (static_cast<float>(corner_coord.y) == static_cast<float>(local_current_position.y)))
							{
								return false;
							}

							corner_coord.x -= 1;

							if ((static_cast<float>(corner_coord.x) == static_cast<float>(local_current_position.x))
							    && (static_cast<float>(corner_coord.y) == static_cast<float>(local_current_position.y)))
							{
								return false;
							}

							return true;
						}
						else
						{
							return false;
						}

						break;

					case 3 :

						if ((local_current_position.x == current_position.x)
						    && (local_current_position.y <= (the_dose_index.y + 1))
						    && (local_current_position.y >= the_dose_index.y) && (the_dose_index.x == local_current_position.x))
						{
							corner_coord.y += 1;

							if ((static_cast<float>(corner_coord.x) == static_cast<float>(local_current_position.x))
							    && (static_cast<float>(corner_coord.y) == static_cast<float>(local_current_position.y)))
							{
								return false;
							}

							corner_coord.y -= 1;

							if ((static_cast<float>(corner_coord.x) == static_cast<float>(local_current_position.x))
							    && (static_cast<float>(corner_coord.y) == static_cast<float>(local_current_position.y)))
							{
								return false;
							}

							return true;
						}
						else
						{
							return false;
						}

						break;

					default :

						assert(0);

				}

				return false;
			}





			bool PolygonInfo::AddSnippetsThisIntersection(snippet_vector& snip_vec_ref ,
			        LegacyWorldCoordinate2D closest_local_current_position)
			{

				bool there_is_a_suitable_polygon_forward = false;
				bool there_is_a_suitable_polygon_backward = false;

				PolygonSnippet forward_snippet;
				PolygonSnippet& forward_snippet_ref = forward_snippet;
				PolygonSnippet backward_snippet;
				PolygonSnippet& backward_snippet_ref = backward_snippet;


				// ersten Punkt an beide Polygonschnipsel anhaengen

				// Dann beide Schnittpunktelisten untersuchen um festzustellen, welche Punkte zuletzt noch aufgenommen
				// werden muss in dieses Polygonstueck. Das geschieht in der Vorwaertsrichtung sowie in der rueckwaertigen Richtung
				// durch die folgende Funktion

				bool do_switch = false ;

				float angle_charact_a = 0;
				float angle_charact_b = 0;

				if (GoForwardAndCreatePolygonSnippet(forward_snippet_ref))
				{
					there_is_a_suitable_polygon_forward = true;
				}

				if (GoBackwardAndCreatePolygonSnippet(backward_snippet_ref))
				{
					there_is_a_suitable_polygon_backward = true;
				}

				if (there_is_a_suitable_polygon_forward)
				{
					angle_charact_a = forward_snippet_ref.angle_charact;
				}

				if (there_is_a_suitable_polygon_backward)
				{
					angle_charact_b = backward_snippet_ref.angle_charact;
				}


				if (there_is_a_suitable_polygon_forward)
				{
					if (there_is_a_suitable_polygon_backward)
					{


						if (angle_charact_a < angle_charact_b)
						{
							Switch(forward_snippet_ref , backward_snippet_ref);
						}

						//if(   (  JustEdge( forward_snippet_ref )  )    && (   JustEdge( backward_snippet_ref )   )  ){
						// nothing happens
						//}

						if ((JustEdge(forward_snippet_ref))    && (! JustEdge(backward_snippet_ref)))
						{
							backward_snippet_ref.characterize = inside_to_edge;
							backward_snippet.why_here = closest_local_current_position;
							snip_vec_ref.push_back(backward_snippet);
						}
						else if ((! JustEdge(forward_snippet_ref))  && (JustEdge(backward_snippet_ref)))
						{
							forward_snippet_ref.characterize = edge_to_inside;
							forward_snippet.why_here = closest_local_current_position;
							snip_vec_ref.push_back(forward_snippet);
						}
						else if ((! JustEdge(forward_snippet_ref))  && (! JustEdge(backward_snippet_ref)))
						{
							backward_snippet_ref.characterize = inside_inside_touches;
							backward_snippet.why_here = closest_local_current_position;
							snip_vec_ref.push_back(backward_snippet);
							forward_snippet_ref.characterize = inside_inside_touches;
							forward_snippet.why_here = closest_local_current_position;
							snip_vec_ref.push_back(forward_snippet);
						}

					}

				}


				if ((there_is_a_suitable_polygon_forward) && (! there_is_a_suitable_polygon_backward))
				{

					if (JustEdge(forward_snippet_ref))
					{

						// nothing happens

					}
					else
					{

						forward_snippet_ref.characterize = real_intersection;
						forward_snippet.why_here = closest_local_current_position;
						snip_vec_ref.push_back(forward_snippet);

					}

				}


				if ((! there_is_a_suitable_polygon_forward) && (there_is_a_suitable_polygon_backward))
				{

					if (JustEdge(backward_snippet_ref))
					{

					}
					else
					{

						backward_snippet_ref.characterize = real_intersection;
						backward_snippet_ref.why_here = closest_local_current_position;
						snip_vec_ref.push_back(backward_snippet);

					}

				}



				// Es ist zwar so, dass man das backward snippet, falls es nur ein snippet gibt nicht anhaengen muss, denn in diesem Fall
				// ist das backward snippet bereits zum forward snippet geworden (und wird als forward snippet angehaengt).
				// Falls es zwei snippets gibt wird nur das forward snippet angehaengt, und zwar das, das im Urzeigersinn richtig liegt.
				// Im Zweifelsfall tauschen.
				// Falls es nur ein forward snippet gibt wird es angehaengt, falls es im Urzeigersinn liegt.


				// Dann muessen die Polygonstuecke an den snip_vec_ref dran gehaengt werden
				// nur falls das Polygonstueck drin liegt anhaengen (GoForwardAndCreatePolygonSnippet GoBackwardAndCreatePolygonSnippet prueft das).

				if ((there_is_a_suitable_polygon_forward) || (there_is_a_suitable_polygon_backward))
				{
					return true;
				}
				else
				{
					return false;
				}

			}



			bool PolygonInfo::JustEdge(PolygonSnippet& snippet_ref)
			{

				bool just_edge = true;

				if ((current_edge == 0)  || (current_edge == 2))
				{
					for (unsigned int position = 0 ; position < snippet_ref.snippet_intermediate_content.size() ;
					     position++)
					{
						LegacyWorldCoordinate3D w_coord = snippet_ref.snippet_intermediate_content.at(position);

						if (static_cast<float>(w_coord.y) != static_cast<float>(snippet_ref.point_of_interest_start.y))
						{
							just_edge = false;
						}
					}

					if (static_cast<float>(snippet_ref.point_of_interest_start.y) != static_cast<float>
					    (snippet_ref.point_of_interest_end.y))
					{
						just_edge = false;
					}
				}
				else if ((current_edge == 1) || (current_edge == 3))
				{
					if (static_cast<float>(snippet_ref.point_of_interest_start.x) != static_cast<float>
					    (snippet_ref.point_of_interest_end.x))
					{
						just_edge = false;
					}

					for (unsigned int position = 0 ; position < snippet_ref.snippet_intermediate_content.size() ;
					     position++)
					{
						LegacyWorldCoordinate3D w_coord = snippet_ref.snippet_intermediate_content.at(position);

						if (static_cast<float>(w_coord.x) != static_cast<float>(snippet_ref.point_of_interest_start.x))
						{
							just_edge = false;
						}
					}
				}
				else
				{
					assert(0);
				}

				return just_edge;

			}



			void PolygonInfo::Switch(PolygonSnippet& snippet_one_ref ,  PolygonSnippet& snippet_two_ref)
			{

				PolygonSnippet keep_snippet;
				PolygonSnippet& keep_snippet_ref = keep_snippet;
				CopySnippet(keep_snippet_ref , snippet_one_ref);

				CopySnippet(snippet_one_ref, snippet_two_ref);
				CopySnippet(snippet_two_ref, keep_snippet_ref);

			}





			void PolygonInfo::CopySnippet(PolygonSnippet& to_snippet, PolygonSnippet& snip_in)
			{

				to_snippet.characterize = snip_in.characterize;
				to_snippet.is_edge = snip_in.is_edge;

				to_snippet.why_here.x = snip_in.why_here.x;
				to_snippet.why_here.y = snip_in.why_here.y;

				to_snippet.point_of_interest_start.x = snip_in.point_of_interest_start.x;
				to_snippet.point_of_interest_start.y = snip_in.point_of_interest_start.y;

				to_snippet.point_of_interest_end.x = snip_in.point_of_interest_end.x;
				to_snippet.point_of_interest_end.y = snip_in.point_of_interest_end.y;

				to_snippet.snippet_intermediate_content.clear();

				for (unsigned int counter = 0 ; counter < snip_in.snippet_intermediate_content.size() ; counter++)
				{
					to_snippet.snippet_intermediate_content.push_back(snip_in.snippet_intermediate_content.at(counter));
				}

			}




			bool PolygonInfo::GoForwardAndCreatePolygonSnippet(PolygonSnippet& forward_snippet_ref)
			{

				// hier muss als erstes geprueft werden, ob das Polygonsnippet ueberhaupt innerhalb des betrachteten Voxels liegt.
				// Falls nein kann man alles folgende ueberspringen.

				float& angle_charact_forward = forward_snippet_ref.angle_charact;



				bool is_inside_valid = CheckPolygonIsInsideForward(angle_charact_forward);

				if ((the_dose_index.x == 2) && (the_dose_index.y == 2))
				{
					it_poly.PrintIntersectionIndex();
				}

				if (is_inside_valid)
				{

					LegacyWorldCoordinate2D& point_of_interest_start_ref = forward_snippet_ref.point_of_interest_start;
					LegacyWorldCoordinate2D& point_of_interest_end_ref = forward_snippet_ref.point_of_interest_end;

					std::vector<LegacyWorldCoordinate3D>& snippet_intermediate_content_ref =
					    forward_snippet_ref.snippet_intermediate_content;
					it_poly.RunForwardToNextIntersection(point_of_interest_start_ref , point_of_interest_end_ref ,
					                                     snippet_intermediate_content_ref , the_dose_index);

				}

				return is_inside_valid;

			}



			bool PolygonInfo::GoBackwardAndCreatePolygonSnippet(PolygonSnippet& backward_snippet_ref)
			{

				// hier muss als erstes geprueft werden, ob das Polygonsnippet ueberhaupt innerhalb des betrachteten Voxels liegt.
				// Falls nein kann man alles folgende ueberspringen.

				float& angle_charact_backward = backward_snippet_ref.angle_charact;
				bool is_inside_valid = CheckPolygonIsInsideBackward(angle_charact_backward);

				if (is_inside_valid)
				{

					LegacyWorldCoordinate2D& point_of_interest_start_ref = backward_snippet_ref.point_of_interest_start;
					LegacyWorldCoordinate2D& point_of_interest_end_ref = backward_snippet_ref.point_of_interest_end;
					std::vector<LegacyWorldCoordinate3D>& snippet_intermediate_content_ref =
					    backward_snippet_ref.snippet_intermediate_content;

					it_poly.RunBackwardToNextIntersection(point_of_interest_start_ref , point_of_interest_end_ref ,
					                                      snippet_intermediate_content_ref);

				}

				return is_inside_valid;

			}



			// To do : Sollte man    it_poly.NextIntersection( the_next_point_ref ) besser als Funktions-Objekt-Dings uebergeben
			// und dadurch  CheckPolygonIsInsideForward() und CheckPolygonIsInsideBackward() in eine Funktion verpacken ?
			// Man kann dann eigentlich auch mit anderen "Forward" und "Backward" Funktionen so verfahren und hat insgesamt weniger Funtionen.
			// Das ist to do fuer spaeter.
			bool PolygonInfo::CheckPolygonIsInsideForward(float& angle_charact_forward)
			{

				bool its_inside = false;

				it_poly.RememberPosition();

				LegacyWorldCoordinate2D the_first_point;
				the_first_point.x = 0;
				the_first_point.y = 0;
				LegacyWorldCoordinate2D& the_first_point_ref =  the_first_point;

				LegacyWorldCoordinate2D the_next_point;
				the_next_point.x = 0;
				the_next_point.y = 0;
				LegacyWorldCoordinate2D& the_next_point_ref = the_next_point;

				if (it_poly.ThisIntersection(the_first_point_ref))
				{

					bool got_it = it_poly.NextPointOrIntersectionPeriodically(the_first_point_ref, the_next_point_ref,
					              the_dose_index);

					int maxNum = it_poly.MaximumNumberOfElements();

					int stop_counter = -1;

					while ((stop_counter < (maxNum + 2)))
					{

						stop_counter++;

						/*
						double close_first = ( the_next_point.x - the_first_point.x ) * ( the_next_point.x - the_first_point.x );
						close_first = sqrt( close_first );
						if( close_first > ( GInformation.getPixelSpacingRow() / 10000 ) ) stop_counter = maxNum;   // ( GInformation.getPixelSpacingRow() / 10000 ) weil durch Rundungsfehler manchmal nicht exakt gleich

						double close_second = ( the_next_point.y - the_first_point.y ) * ( the_next_point.y - the_first_point.y );
						close_second = sqrt( close_second );
						if( close_second > ( GInformation.getPixelSpacingColumn() /10000 ) ) stop_counter = maxNum;
						*/

						if ((static_cast<float>(the_next_point.x) != static_cast<float>(the_first_point.x))
						    || (static_cast<float>(the_next_point.x) != static_cast<float>(the_first_point.x)))
						{
							stop_counter = (maxNum + 2);
						}

						if ((static_cast<float>(the_next_point.y) != static_cast<float>(the_first_point.y))
						    || (static_cast<float>(the_next_point.y) != static_cast<float>(the_first_point.y)))
						{
							stop_counter = (maxNum + 2);
						}

						if (stop_counter < (maxNum + 2))
						{
							got_it = it_poly.NextPointOrIntersectionPeriodically(the_first_point_ref, the_next_point_ref,
							         the_dose_index);
						}

					}

					if (got_it == false)
					{
						assert(0);
					}


					if ((got_it) && ((static_cast<float>(the_next_point.x) != static_cast<float>(the_first_point.x))
					                 || (static_cast<float>(the_next_point.y) != static_cast<float>(the_first_point.y))))
					{

						LegacyWorldCoordinate2D an_intermediate_point;
						an_intermediate_point.x = ((the_first_point.x + the_next_point.x) * 0.5) ;
						an_intermediate_point.y = ((the_first_point.y + the_next_point.y) * 0.5) ;

						GetAngle(the_next_point , the_first_point , angle_charact_forward);

						if (CheckPointInVoxelRegardingDoseIndex(an_intermediate_point))
						{
							its_inside = true;
						}

					}
					else
					{
						its_inside = false;
					}


				}

				it_poly.JumpToRememberedPosition();

				return its_inside;

			}




			// Je kleiner der Winkel zwischen Kantenrichtung im Uhrzeigersinn und Strukturelement, desto groesser der Rueckgabewert. Der
			// Rueckgabewert ist der Cosisnus dieses Winkels und ist daher 1, falls der Winkel null ist - minus eins ist er falls der
			// Winkel 180 Grad gegen die Kante laueft. Da hier nur Strukturelemente betrachtet werden, die ins Voxel rein laufen,
			// gibt es Winkel gruesser als 180 Grad nicht.

			void PolygonInfo::GetAngle(LegacyWorldCoordinate2D the_next_point ,
			                           LegacyWorldCoordinate2D the_first_point , float& angle_charact)
			{

				double hypoten = (the_next_point.x - the_first_point.x) * (the_next_point.x - the_first_point.x)
				                 + (the_next_point.y - the_first_point.y)  * (the_next_point.y - the_first_point.y);

				assert(hypoten >
				       0);    // This should never happen except in case of a structure consisting of one point only. However, that's not allowed in voxelisation.

				if (current_edge == 0)
				{
					hypoten = sqrt(hypoten);
					double gekat = (the_next_point.x - the_first_point.x);
					angle_charact = static_cast<float>((gekat / hypoten));
				}

				if (current_edge == 1)
				{
					hypoten = sqrt(hypoten);
					double ankat = (the_next_point.y - the_first_point.y);
					angle_charact = static_cast<float>((ankat / hypoten));
				}

				if (current_edge == 2)
				{
					hypoten = sqrt(hypoten);
					double gekat = (the_first_point.x - the_next_point.x);
					angle_charact = static_cast<float>((gekat / hypoten));
				}

				if (current_edge == 3)
				{
					hypoten = sqrt(hypoten);
					double ankat = (the_first_point.y - the_next_point.y);
					angle_charact = static_cast<float>((ankat / hypoten));
				}

			}




			// To do : Kann man    it_poly.NextIntersection( the_next_point_ref ) nicht als Funktions-Objekts-Dings uebergeben
			// und dadurch  CheckPolygonIsInsideForward() und CheckPolygonIsInsideBackward() in eine Funktion verpacken ?
			// Man kann dann eigentlich auch mit anderen "Forward" und "Backward" Sachen so verfahren.
			// Mache das spaeter.

			bool PolygonInfo::CheckPolygonIsInsideBackward(float& angle_charact_backward)
			{

				bool its_inside = false;

				it_poly.RememberPosition();

				LegacyWorldCoordinate2D the_first_point;
				the_first_point.x = 0;
				the_first_point.y = 0;
				LegacyWorldCoordinate2D& the_first_point_ref = the_first_point;

				LegacyWorldCoordinate2D the_next_point;
				the_next_point.x = 0;
				the_next_point.y = 0;
				LegacyWorldCoordinate2D& the_next_point_ref = the_next_point;

				if (it_poly.ThisIntersection(the_first_point_ref))
				{

					bool got_it = it_poly.PreviousPointOrIntersectionPeriodically(the_first_point_ref ,
					              the_next_point_ref , the_dose_index);

					int stop_counter = -1;
					int maxNum = it_poly.MaximumNumberOfElements();

					while ((stop_counter < maxNum))
					{

						stop_counter++;

						/*double close_first = ( the_next_point.x - the_first_point.x ) * ( the_next_point.x - the_first_point.x );
						close_first = sqrt( close_first );
						if( close_first > ( GInformation.getPixelSpacingRow() / 10000 ) ) stop_counter = maxNum;   // ( GInformation.getPixelSpacingRow() / 10000 ) weil durch Rundungsfehler manchmal nicht exakt gleich

						double close_second = ( the_next_point.y - the_first_point.y ) * ( the_next_point.y - the_first_point.y );
						close_second = sqrt( close_second );
						if( close_second > ( GInformation.getPixelSpacingColumn() /10000 ) ) stop_counter = maxNum; */


						if ((static_cast<float>(the_next_point.x) != static_cast<float>(the_first_point.x))
						    || (static_cast<float>(the_next_point.x) != static_cast<float>(the_first_point.x)))
						{
							stop_counter = maxNum;
						}

						if ((static_cast<float>(the_next_point.y) != static_cast<float>(the_first_point.y))
						    || (static_cast<float>(the_next_point.y) != static_cast<float>(the_first_point.y)))
						{
							stop_counter = maxNum;
						}



						if (stop_counter < maxNum)
						{
							got_it = it_poly.PreviousPointOrIntersectionPeriodically(the_first_point_ref , the_next_point_ref ,
							         the_dose_index);
						}


					}

					if (got_it == false)
					{
						assert(0);
					}


					if ((got_it) && ((static_cast<float>(the_next_point.x) != static_cast<float>(the_first_point.x))
					                 || (static_cast<float>(the_next_point.y) != static_cast<float>(the_first_point.y))))
					{

						LegacyWorldCoordinate2D an_intermediate_point;
						an_intermediate_point.x = ((the_first_point.x + the_next_point.x) * 0.5);
						an_intermediate_point.y = ((the_first_point.y + the_next_point.y) * 0.5);

						GetAngle(the_next_point , the_first_point , angle_charact_backward);

						if (CheckPointInVoxelRegardingDoseIndex(an_intermediate_point))
						{
							its_inside = true;
						}

					}
					else
					{
						its_inside = false;
					}

				}
				else
				{
					assert(0);
				}

				it_poly.JumpToRememberedPosition();

				return its_inside;

			}



			bool PolygonInfo::CheckPointInVoxelRegardingDoseIndex(LegacyWorldCoordinate2D
			        voxel_coordinate_to_be_checked)
			{

				bool its_inside = false;

				if ((voxel_coordinate_to_be_checked.x  <= (the_dose_index.x + 1))
				    && (voxel_coordinate_to_be_checked.x   >= the_dose_index.x)
				    && (voxel_coordinate_to_be_checked.y   <= (the_dose_index.y + 1))
				    && (voxel_coordinate_to_be_checked.y   >=  the_dose_index.y))
				{
					its_inside = true;
				}


				return its_inside;

			}



			double PolygonInfo::GetDistanceNextEdge(int current_edge)
			{

				double return_distance;


				switch (current_edge)
				{


					case 0 :
						return_distance = GInformation.getPixelSpacingRow();
						break;


					case 1 :
						return_distance = GInformation.getPixelSpacingColumn();
						return_distance += GInformation.getPixelSpacingRow();
						break;

					case 2 :
						return_distance = GInformation.getPixelSpacingColumn();
						return_distance += 2 * GInformation.getPixelSpacingRow();
						break;

					case 3 :
						return_distance = 2 * GInformation.getPixelSpacingColumn();
						return_distance += 2 * GInformation.getPixelSpacingRow();
						break;


				}


				return return_distance;

			}



			float PolygonInfo::GetDistanceAlongEdge(LegacyDoseVoxelIndex3D edge_position ,
			                                        LegacyWorldCoordinate2D local_position)
			{

				WorldCoordinate return_distance;

				if (local_position.y == edge_position.y)
				{
					return_distance = (local_position.x - edge_position.x) * GInformation.getPixelSpacingRow();
				}
				else if (local_position.y == (edge_position.y + 1))
				{
					return_distance = GInformation.getPixelSpacingRow() - (local_position.x - edge_position.x) *
					                  GInformation.getPixelSpacingRow();
					return_distance += GInformation.getPixelSpacingColumn();
					return_distance += GInformation.getPixelSpacingRow();

				}
				else if (local_position.x == edge_position.x)
				{

					return_distance = 2 * GInformation.getPixelSpacingRow();
					return_distance += GInformation.getPixelSpacingColumn();
					return_distance += GInformation.getPixelSpacingColumn() - (local_position.y - edge_position.y) *
					                   GInformation.getPixelSpacingColumn();

				}
				else if (local_position.x == (edge_position.x + 1))
				{
					return_distance = GInformation.getPixelSpacingRow();
					return_distance += (local_position.y - edge_position.y) * GInformation.getPixelSpacingColumn();

				}
				else
				{
					// assert : Etwas ist schief gegangen. Wenn es sich wirklich um einen Schnittpunkt handelt,
					// dann muesste einer der obigen Faelle eingetreten sein.
					assert(0);
				}

				return return_distance;

			}



			// Returns 1D distance of two Points along edge.
			// Both Points are located on the edge of the voxel and the

			float PolygonInfo::GetPeriodicDist(float distance_current_position_local ,
			                                   float distance_current_position)
			{

				float distance_oneD_return;

				float max_distance = static_cast<float>(2 * GInformation.getPixelSpacingRow() + 2 *
				                                        GInformation.getPixelSpacingColumn());

				if (distance_current_position_local >= distance_current_position)
				{

					distance_oneD_return = distance_current_position_local - distance_current_position;

					float cross_edge_distance = (max_distance - distance_current_position_local) +
					                            distance_current_position;

					if (cross_edge_distance < distance_oneD_return)
					{
						distance_oneD_return = cross_edge_distance;
					}

				}
				else
				{

					distance_oneD_return = distance_current_position - distance_current_position_local;

					float cross_edge_distance = (max_distance - distance_current_position) +
					                            distance_current_position_local;

					if (cross_edge_distance < distance_oneD_return)
					{
						distance_oneD_return = cross_edge_distance;
					}

				}

				return distance_oneD_return;

			}



			double PolygonInfo::GetClockwiseDist(double distance_refer_to , double distance_compare)
			{

				double distance_oneD_return;

				double max_distance = static_cast<double>(2 * GInformation.getPixelSpacingRow() + 2 *
				                      GInformation.getPixelSpacingColumn());

				if (distance_compare >= distance_refer_to)
				{

					distance_oneD_return = distance_compare - distance_refer_to;

				}
				else
				{

					distance_oneD_return = (max_distance - distance_refer_to) + distance_compare;

				}

				return distance_oneD_return;

			}



			/*

			// Es gibt einen current_static_index, der besagt welches Polygon gerade erzeugt wird.

			// Und einen current_moving_index, der einmal ums Voxel laueft.
			// Auserdem einen begin_index, der sagt, wo man mit der Erzeugung des umschliessenden Polygons angefangen hat
			// und wider aufhoeren will.


			current_static_index klappert jetzt die Voxelkante ab.
			Fuer jeden zweiten drinnen-drausen-Wechsel (den gibt es bei echten Schnittpunkten und manchen Beruehrpunkten)
			wird nun also ein innen-geschlossenes-Polygon berechnet.
			Das geschieht indem fuer die entsprechende Position des current_static_index solange der current_moving_index
			verschoben wird, bis man im Kreis rum ist.
			Der current_moving_index wird dabei in doppleschritten verschoben:
			Erst der Kante entlang zum naechsten echten Schnittpunkt oder Beruehrpunkt der zweiten Art.
			Dann wird diesem Polygonsnippet zu sienem anderen
			Schnittpunkt/Beruehrpunkt gefolgt. Das Polygonsnippet und die Randpunkte werden dabei an das derzeit zu erzeugende
			Innenpolygon angehaengt.

			Im Idealfall weiss man von einer Voxelecke anhand der Maske, dass sie draussen liegt.

			Falls es keine Voxelecke gibt von der man eindeutig sagen kann dass sie draussen liegt und auf die man dann den  begin_index setzen kann,
			dann wird gesucht, ob es irgend eine Ecke gibt, die nicht Schnitt oder Beruehrpunkt ist. Per VoxelInStructure aus der Tooblox kann man sich in
			diesem Fall behelfen.

			Falls alle Ecken Beruehr oder Schnittpunkte sind, wird zunaechst geprueft, ob es nur Polygonsnippets gibt, die den Kanten entlang
			laufen. Falls das so ist, wird VoxelInStructure der Toolbox fuer das Zentrum des Voxels aufgerufen.
			Wenn das Zentrum drin ist, dann ist das ganze Voxel von der Kontur umschlossen.
			Falls nicht liegt das ganze Voxel ausserhalb.

			Falls alle Voxelecken Schnitt/Beruehrpunkte sind und falls es sehr wohl echte Schnitte gibt:
			Einen Punkt finden, der echter Schnittpunkt ist. Seine Nachbarschaft per  VoxelInStructure aus der Tooblox  untersuchen.


			*/

			// Anmerkung : Hier werden Aenderungen noetig sien, sobald Beruehrpunkte der Kontur mit sich selbst erlaubt sind.
			// Die Aenderung wird darin bestehen, dass mehrere snippets mit dem selben why_here Wert existieren, die gleichberechtigt
			// behandelt werden nach dem Winkel des sich nach innnen fortsetzenden Strukturelements geordnet sind. Diese werden dann
			// behandelt wei ueblich (also keine echte Aenderung des Programmablaufs nur der Art der Polygon - Snippets )

			// Polygon aufbauen:  resulting_sequence wird hier belegt
			bool PolygonInfo::CreatePolygonSequence()
			{

				CalcSnippetVectors();

				SnippetIndex& current_static_index_ref = current_static_index;

				// Es wird im idealfall in einer Ecke mit bekanntem drinnen /drausen begonnen. begin_index wird dort drauf gesetzt.
				// current_static_index wird solange eins weiter gesetzt bis man wieder am Anfang ankommt.
				// falls die Kontur gar nicht ins voxel rein laueft wird nothing_to_do auf true gesetzt

				bool nothing_to_do = false;
				bool& nothing_to_do_ref = nothing_to_do;


				// SetCurrentStaticIndexKnownInsideOrOutside gibt zum einen ueber its_inside die information darueber zurueck,
				// ob der Punkt auf den current_static_index gesetzt wurde im aussenbereich der Kontur liegt.
				// Zum anderen enthaelt nothing_to_do_ref nach Aufruf von SetCurrentStaticIndexKnownInsideOrOutside die
				// Information darueber, ob das Voxel ueberhaupt angeschnitten ist. Ist  nothing_to_do_ref true, so ist das Voxel
				// gar nicht angeschnitten. Ist dann auch noch its_inside true, dann liegt das ganze Voxel komplett im innern der Struktur.
				// Ansonsten liegt das ganze Voxel komplett ausserhalb der Struktur.
				bool its_inside = SetCurrentStaticIndexKnownInsideOrOutside(current_static_index_ref ,
				                  nothing_to_do_ref);   // Falls es eine Ecke gibt, die draussen liegt wird hier false zurueckgegeben.
				bool just_switched_inside = false;

				if (its_inside)
				{
					just_switched_inside = true;
				}


				/*    std::cout<<  "  the_dose_index.x  : "  <<  the_dose_index.x  <<std::endl;
				    std::cout<<  "  the_dose_index.y  : "  <<  the_dose_index.y  <<std::endl;
				    std::cout<<  "  the_dose_index.z  : "  <<  the_dose_index.z  <<std::endl;

				    std::cout<< "  nothing_to_do : "  << nothing_to_do  <<std::endl;
				    std::cout<< "  its_inside : "  << its_inside  <<std::endl;  */


				/*std::cout<< "   and here are all the snippets!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  " <<std::endl;

				for( int counter = 0 ; counter < 4 ; counter++ ){

				  for( int counter_internal = 0 ; counter_internal < pol_inf_vec.at(counter).size() ; counter_internal++ ){

				    PolygonSnippet a_snippet = pol_inf_vec.at( counter ).at( counter_internal );

				    ShowSnippet( a_snippet );

				  }

				}*/


				if (nothing_to_do == false)
				{



					CopySnippetIndex(current_static_index , begin_index);
					CopySnippetIndex(current_static_index , current_moving_index);

					int max_num = GetMaxNumberSnippets();
					int& max_num_ref = max_num;

					// auch wenn der status "falsch" gesetzt wurde, funktioniert doch das umschalten dennoch
					// es ist also egal, ob man den ersten Schnitt als hinein, oder als hinauslaufend definiert, denn die Struktur ist ohnehin zyklisch
					edge_status = edge_or_outside;

					bool& its_inside_ref = its_inside;
					bool next_is_edge = true;
					bool& next_is_edge_ref = next_is_edge;
					bool& just_switched_inside_ref = just_switched_inside;

					bool its_first_current_static = true;

					while ((! SnippetIndexIdentical(begin_index , current_static_index))
					       || (its_first_current_static == true))
					{


						its_first_current_static = false;

						CopySnippetIndex(current_static_index , current_moving_index);

						CheckInside(current_moving_index , its_inside_ref , just_switched_inside_ref);

						// Jedes zweite weitersetzen erzeugt ein innenpolygon. Eventuell jedes zweite ein Ausenbereichs polygon, das ein Stueck Ausenbereich
						// des betrachteten Voxels umschliesst.

						next_is_edge = GetNextIsEdge(current_moving_index);

						if (! next_is_edge)                   // hier soll ein Snippet uebersprungen werden, weil das heran und das weglaufende vertreten sind. Verfolgt werden soll aber nur das weiterfuerende. Das heranfuehrende wird das snippet sein ueber das der Kreis am Ende geschlossen wird. Muss hier zunaechst nicht betrachtet werden.
						{
							IncrementSnippetIndex(current_static_index);
							CopySnippetIndex(current_static_index , current_moving_index);
						}

						if ((((its_inside) && (just_switched_inside_ref)) || (next_is_edge == false))
						    && (! CurrentSnippetIsProcessed()))
						{

							AppendNewPolygonToResultingSequence();

							bool do_continue = true;
							bool& do_continue_ref = do_continue;

							int continue_counter = 0;
							int& continue_counter_ref = continue_counter;


							while (do_continue)
							{


								// Baustelle was mit  ( !CurrentSnippetIsProcessed() ) und assert !
								GoGetPoly(do_continue_ref , next_is_edge_ref, continue_counter_ref , max_num_ref);



							} // do_continue



						} // its_inside

						IncrementSnippetIndex(current_static_index);

					}

				}
				else
				{

					if (its_inside)
					{

						AppendVoxelContour();

					}
				}


				//if(  ( the_dose_index.x == 3 ) && ( the_dose_index.y == 2 )  )  ShowResultingSequence();

				SlicePositionUnifyAndCheckPeriodicSequence();


				// if(  ( the_dose_index.x == 4 ) && ( the_dose_index.y == 3 )  )  ShowResultingSequence();
				//  ShowPolygonInTermsOfIntersections();


				// Baustelle : soll false zureuckgeben, falls was schief geht.
				return true;

			}




			void PolygonInfo::AppendNewPolygonToResultingSequence()
			{

				LegacyPolygonType new_polygon;
				resulting_sequence.push_back(new_polygon);

			}




			void PolygonInfo::AppendVoxelContour()
			{

				AppendNewPolygonToResultingSequence();

				LegacyWorldCoordinate3D w_3_d;
				w_3_d.x = the_dose_index.x;
				w_3_d.y = the_dose_index.y;
				w_3_d.z = the_dose_index.z;

				resulting_sequence.at(resulting_sequence.size() - 1).push_back(w_3_d);

				w_3_d.x = the_dose_index.x + 1;
				w_3_d.y = the_dose_index.y;
				w_3_d.z = the_dose_index.z;

				resulting_sequence.at(resulting_sequence.size() - 1).push_back(w_3_d);

				w_3_d.x = the_dose_index.x + 1;
				w_3_d.y = the_dose_index.y + 1;
				w_3_d.z = the_dose_index.z;

				resulting_sequence.at(resulting_sequence.size() - 1).push_back(w_3_d);


				w_3_d.x = the_dose_index.x;
				w_3_d.y = the_dose_index.y + 1;
				w_3_d.z = the_dose_index.z;

				resulting_sequence.at(resulting_sequence.size() - 1).push_back(w_3_d);

				w_3_d.x = the_dose_index.x;
				w_3_d.y = the_dose_index.y;
				w_3_d.z = the_dose_index.z;

				resulting_sequence.at(resulting_sequence.size() - 1).push_back(w_3_d);

			}




			void PolygonInfo::SlicePositionUnifyAndCheckPeriodicSequence()
			{


				for (unsigned int first_index = 0 ; first_index < resulting_sequence.size() ; first_index++)
				{

					WorldCoordinate the_z_value = 0;

					for (unsigned int second_index = 0 ; second_index < resulting_sequence.at(first_index).size() ;
					     second_index++)
					{


						LegacyWorldCoordinate3D the_world_coordinate_3D = resulting_sequence.at(first_index).at(
						            second_index);

						if (static_cast<float>(the_world_coordinate_3D.z) != 0)
						{
							the_z_value = the_world_coordinate_3D.z;
							second_index = resulting_sequence.at(first_index).size();
						}

					}

					for (unsigned int second_index = 0 ; second_index < resulting_sequence.at(first_index).size() ;
					     second_index++)
					{

						LegacyWorldCoordinate3D& the_world_coordinate_3D_ref = resulting_sequence.at(first_index).at(
						            second_index);
						the_world_coordinate_3D_ref.z = the_z_value;

					}


					if (resulting_sequence.at(first_index).size() > 0)
					{
						if (static_cast<float>(resulting_sequence.at(first_index).at(resulting_sequence.at(
						                           first_index).size() - 1).x) !=  static_cast<float>(resulting_sequence.at(first_index).at(0).x))
						{
							assert(0);    // this must never happen. The countour must be of periodic nature.
						}

						if (static_cast<float>(resulting_sequence.at(first_index).at(resulting_sequence.at(
						                           first_index).size() - 1).y) !=  static_cast<float>(resulting_sequence.at(first_index).at(0).y))
						{
							assert(0);    // this must never happen. The countour must be of periodic nature.
						}

						if (static_cast<float>(resulting_sequence.at(first_index).at(resulting_sequence.at(
						                           first_index).size() - 1).z) !=  static_cast<float>(resulting_sequence.at(first_index).at(0).z))
						{
							assert(0);    // this must never happen. The countour must be of periodic nature.
						}
					}

				}

			}




			int PolygonInfo::GetMaxNumberSnippets()
			{

				int nr = 0;

				for (int ind = 0 ; ind != 4 ; ind++)
				{

					nr += pol_inf_vec.at(ind).size();

				}

				return nr;

			}



			// Hier werden Aenderungen noetig sein, sobald Beruehrpunkte der Kontur mit sich selbst erlaubt sind.
			// Ist erledigt. Die Beruehrpunkte sind jetzt erlaubt
			void PolygonInfo::GoGetPoly(bool& do_continue_ref , bool& next_is_edge_ref,
			                            int& continue_counter_ref , int& max_num_ref)
			{


				PolygonSnippet snipp;
				PolygonSnippet& snipp_ref = snipp;

				if (next_is_edge_ref)
				{

					bool interupt = false;

					GetCurrentMovingSnippet(snipp_ref);

					LegacyWorldCoordinate2D why_here = snipp_ref.why_here;

					int the_previous_edge = current_moving_index.edge;
					continue_counter_ref++;
					IncrementSnippetIndex(current_moving_index);

					if (do_continue_ref)
					{
						do_continue_ref = DoContinue(max_num_ref , continue_counter_ref);
					}

					if (do_continue_ref == false)
					{
						interupt = true;
					}

					while (! interupt)
					{

						if (TryGetPolygonsnippetEdge(continue_counter_ref ,
						                             the_previous_edge))       // holt sich das Schnipsel, das zum moving index gehoert falls es Konturinhalt hat. Falls nicht wird der moving index weiter gesetzt.
						{
							interupt = true;
							next_is_edge_ref = false;

						}


						if (do_continue_ref)
						{
							do_continue_ref = DoContinue(max_num_ref , continue_counter_ref);
						}

						if (do_continue_ref == false)
						{

							interupt = true;
						}


					}

					GetCurrentMovingSnippet(snipp_ref);

					DoAppendIntermediateEdgeUpToThisSnippet(snipp_ref , why_here , the_previous_edge ,
					                                        current_moving_index.edge);

				}
				else
				{

					if (GetCurrentMovingSnippet(snipp_ref))
					{

						LegacyWorldCoordinate2D why_here = snipp_ref.why_here;

						SetCurrentMovingProcessed();

						GoAlongSnippetToNextIntersectionAndAppendToResultingSequence(snipp_ref ,  continue_counter_ref);

						SetCurrentMovingProcessed();

						if (do_continue_ref)
						{

							next_is_edge_ref = GetNextIsEdge(current_moving_index);

							if (! next_is_edge_ref)
							{
								continue_counter_ref++;
								IncrementSnippetIndex(current_moving_index);
							}

						}

						if (do_continue_ref)
						{
							do_continue_ref = DoContinue(max_num_ref , continue_counter_ref);
						}


					}
					else
					{
						assert(0);    //this should never happen
					}


				}


			}



			bool PolygonInfo::CurrentSnippetIsProcessed()
			{
				return pol_inf_vec.at(current_static_index.edge).at(
				           current_static_index.snip).i_have_been_processed;
			}


			void PolygonInfo::ShowPolygonInTermsOfIntersections()
			{

				it_poly.ShowSelf();

			}


			void PolygonInfo::ShowResultingSequence()
			{

				for (GridIndexType counter = 0 ; counter < resulting_sequence.size() ; counter++)
				{

					std::cout << " And this is countour number : " <<  counter  << std::endl;

					for (GridIndexType counter_internal = 0 ; counter_internal < resulting_sequence.at(counter).size() ;
					     counter_internal++)
					{

						LegacyWorldCoordinate3D wc3d_b = resulting_sequence.at(counter).at(counter_internal);

						std::cout << " wc3d_b.x : " <<  wc3d_b.x  << std::endl;
						std::cout << " wc3d_b.y : " <<  wc3d_b.y  << std::endl;
						std::cout << " wc3d_b.z : " <<  wc3d_b.z  << std::endl;


					}

				}


			}


			bool PolygonInfo::GetCurrentMovingSnippet(PolygonSnippet& snipp_ref)
			{

				if ((current_moving_index.edge < 4)
				    && (current_moving_index.snip < pol_inf_vec.at(current_moving_index.edge).size())
				    && (current_moving_index.snip >= 0) && (current_moving_index.edge >= 0))
				{
					snipp_ref = pol_inf_vec.at(current_moving_index.edge).at(current_moving_index.snip);

					return true;
				}
				else
				{
					return false;
				}

			}


			void PolygonInfo::SetCurrentMovingProcessed()
			{
				if ((current_moving_index.edge < 4)
				    && (current_moving_index.snip < pol_inf_vec.at(current_moving_index.edge).size())
				    && (current_moving_index.snip >= 0) && (current_moving_index.edge >= 0))
				{
					pol_inf_vec.at(current_moving_index.edge).at(current_moving_index.snip).i_have_been_processed =
					    true;
				}
			}


			int PolygonInfo::EdgeDist(int edge_old , int the_current_edge)
			{

				int edge_internal = edge_old;

				int counter = 0;

				while (edge_internal !=  the_current_edge)
				{
					counter++;
					IncrementEdge(edge_internal);
				}

				return counter;

			}


			void PolygonInfo::IncrementEdge(int& edge_increment)
			{

				if (edge_increment == 3)
				{
					edge_increment = 0;
				}
				else
				{
					edge_increment++;
				}

			}


			void PolygonInfo::DoAppendIntermediateEdgeUpToThisSnippet(PolygonSnippet& snipp_ref ,
			        LegacyWorldCoordinate2D why_here , int the_previous_edge , int the_current_edge)
			{

				PolygonSnippet snipp_internal;
				PolygonSnippet& snipp_internal_ref = snipp_internal;

				snipp_internal_ref.why_here = why_here;
				snipp_internal_ref.point_of_interest_start = why_here;
				snipp_internal_ref.point_of_interest_end = snipp_ref.why_here;

				std::vector<LegacyWorldCoordinate3D>& inter_content_ref =
				    snipp_internal.snippet_intermediate_content;

				int edge_internal = the_previous_edge;

				while (EdgeDist(edge_internal , the_current_edge) > 0)
				{
					edge_internal++;

					if (edge_internal == 4)
					{
						edge_internal = 0;
					}

					AppendEdgeToVector(inter_content_ref , edge_internal);
				}

				if (snipp_internal_ref.snippet_intermediate_content.size() > 0)
				{
					if ((snipp_internal_ref.snippet_intermediate_content.at(0).x ==
					     snipp_internal_ref.point_of_interest_end.x)
					    && (snipp_internal_ref.snippet_intermediate_content.at(0).y ==
					        snipp_internal_ref.point_of_interest_end.y))
					{
						snipp_internal_ref.snippet_intermediate_content.clear();
					}
				}

				if (snipp_internal_ref.snippet_intermediate_content.size() > 0)
				{
					std::vector<LegacyWorldCoordinate3D>::iterator iter;

					while ((snipp_internal_ref.snippet_intermediate_content.at(
					            snipp_internal_ref.snippet_intermediate_content.size() - 1).x ==
					        snipp_internal_ref.point_of_interest_end.x)
					       && (snipp_internal_ref.snippet_intermediate_content.at(
					               snipp_internal_ref.snippet_intermediate_content.size() - 1).y ==
					           snipp_internal_ref.point_of_interest_end.y))
					{
						iter = snipp_internal_ref.snippet_intermediate_content.begin();
						iter += (snipp_internal_ref.snippet_intermediate_content.size() - 1);
						snipp_internal_ref.snippet_intermediate_content.erase(iter);
					}
				}

				AppendToResultingSequence(snipp_internal_ref);


			}




			void PolygonInfo::AppendEdgeToVector(std::vector<LegacyWorldCoordinate3D>& vector ,
			                                     int edge_internal)
			{

				LegacyWorldCoordinate3D world_coordinate;

				switch (edge_internal)
				{

					case 0 :

						world_coordinate.x = the_dose_index.x;
						world_coordinate.y = the_dose_index.y;
						world_coordinate.z = 0;
						break;


					case 1 :

						world_coordinate.x = the_dose_index.x + 1;
						world_coordinate.y = the_dose_index.y;
						world_coordinate.z = 0;
						break;

					case 2 :

						world_coordinate.x = the_dose_index.x + 1;
						world_coordinate.y = the_dose_index.y + 1;
						world_coordinate.z = 0;
						break;

					case 3 :

						world_coordinate.x = the_dose_index.x;
						world_coordinate.y = the_dose_index.y + 1;
						world_coordinate.z = 0;
						break;

					default :

						assert(0);

				}

				vector.push_back(world_coordinate);


			}



			void PolygonInfo::AppendToResultingSequence(PolygonSnippet& snipp_ref)
			{

				int which_polygon = (resulting_sequence.size() - 1);

				if (resulting_sequence.at(which_polygon).size() > 0)
				{

					LegacyWorldCoordinate3D the_world_coordinate_3D = resulting_sequence.at(which_polygon).at(
					            resulting_sequence.at(which_polygon).size() - 1);

					if ((snipp_ref.point_of_interest_start.x == the_world_coordinate_3D.x)
					    && (snipp_ref.point_of_interest_start.y == the_world_coordinate_3D.y))
					{

						DoAppendForward(snipp_ref ,  which_polygon);

					}
					else if ((snipp_ref.point_of_interest_end.x == the_world_coordinate_3D.x)
					         && (snipp_ref.point_of_interest_end.y == the_world_coordinate_3D.y))
					{

						DoAppendBackward(snipp_ref ,  which_polygon);

					}
					else
					{

						the_world_coordinate_3D = resulting_sequence.at(which_polygon).at(0);

						if ((snipp_ref.point_of_interest_start.x == the_world_coordinate_3D.x)
						    && (snipp_ref.point_of_interest_start.y == the_world_coordinate_3D.y))
						{

							DoInsertForward(snipp_ref ,  which_polygon);

						}
						else if ((snipp_ref.point_of_interest_end.x == the_world_coordinate_3D.x)
						         && (snipp_ref.point_of_interest_end.y == the_world_coordinate_3D.y))
						{

							DoInsertBackward(snipp_ref ,  which_polygon);

						}
						else
						{
							assert(0);   // this should never happen
						}

					}

				}
				else
				{
					AddFirstForward(snipp_ref ,  which_polygon);
				}

			}



			void PolygonInfo::DoAppendForward(PolygonSnippet& snipp_ref , int which_polygon)
			{

				for (unsigned int counter = 0 ; counter < snipp_ref.snippet_intermediate_content.size() ; counter++)
				{
					resulting_sequence.at(which_polygon).push_back(snipp_ref.snippet_intermediate_content.at(counter));
				}

				LegacyWorldCoordinate3D coord_3_d;
				coord_3_d.x = snipp_ref.point_of_interest_end.x;
				coord_3_d.y = snipp_ref.point_of_interest_end.y;
				coord_3_d.z = 0;

				resulting_sequence.at(which_polygon).push_back(coord_3_d);

			}



			void PolygonInfo::DoAppendBackward(PolygonSnippet& snipp_ref , int which_polygon)
			{

				int index = 0;

				for (unsigned int counter = 0 ; counter < snipp_ref.snippet_intermediate_content.size() ; counter++)
				{

					index = snipp_ref.snippet_intermediate_content.size() - counter - 1;

					resulting_sequence.at(which_polygon).push_back(snipp_ref.snippet_intermediate_content.at(index));

				}

				LegacyWorldCoordinate3D coord_3_d;
				coord_3_d.x = snipp_ref.point_of_interest_start.x;
				coord_3_d.y = snipp_ref.point_of_interest_start.y;
				coord_3_d.z = 0;

				resulting_sequence.at(which_polygon).push_back(coord_3_d);

			}


			void PolygonInfo::DoInsertForward(PolygonSnippet& snipp_ref , int which_polygon)
			{

				LegacyPolygonTypeIterator it;

				for (unsigned int counter = 0 ; counter < snipp_ref.snippet_intermediate_content.size() ; counter++)
				{
					it = resulting_sequence.at(which_polygon).begin();
					resulting_sequence.at(which_polygon).insert(it ,
					        snipp_ref.snippet_intermediate_content.at(counter));
				}

				it = resulting_sequence.at(which_polygon).begin();

				LegacyWorldCoordinate3D coord_3_d;
				coord_3_d.x = snipp_ref.point_of_interest_end.x;
				coord_3_d.y = snipp_ref.point_of_interest_end.y;
				coord_3_d.z = 0;

				resulting_sequence.at(which_polygon).insert(it , coord_3_d);

			}


			void PolygonInfo::DoInsertBackward(PolygonSnippet& snipp_ref , int which_polygon)
			{

				int index = 0;

				LegacyPolygonTypeIterator it;

				for (unsigned int counter = 0 ; counter < snipp_ref.snippet_intermediate_content.size() ; counter++)
				{

					index = snipp_ref.snippet_intermediate_content.size() - counter - 1;

					it = resulting_sequence.at(which_polygon).begin();

					resulting_sequence.at(which_polygon).insert(it , snipp_ref.snippet_intermediate_content.at(index));

				}

				it = resulting_sequence.at(which_polygon).begin();

				LegacyWorldCoordinate3D coord_3_d;
				coord_3_d.x = snipp_ref.point_of_interest_start.x;
				coord_3_d.y = snipp_ref.point_of_interest_start.y;
				coord_3_d.z = 0;

				resulting_sequence.at(which_polygon).insert(it , coord_3_d);

			}


			void PolygonInfo::AddFirstForward(PolygonSnippet& snipp_ref , int which_polygon)
			{

				LegacyWorldCoordinate3D coord_3_d;
				coord_3_d.x = snipp_ref.point_of_interest_start.x;
				coord_3_d.y = snipp_ref.point_of_interest_start.y;
				coord_3_d.z = 0;

				resulting_sequence.at(which_polygon).push_back(coord_3_d);

				DoAppendForward(snipp_ref , which_polygon);

			}




			void PolygonInfo::GoAlongSnippetToNextIntersectionAndAppendToResultingSequence(
			    PolygonSnippet& snipp_ref ,  int& continue_counter_ref)
			{

				AppendToResultingSequence(snipp_ref);
				FindCorrespondingOneAndSetMovingIndexAndSetContinueCounter(snipp_ref , continue_counter_ref);

			}





			bool PolygonInfo::IsCorresponding(PolygonSnippet& snipp_ref_compare , PolygonSnippet& snipp_ref)
			{


				if ((static_cast<float>(snipp_ref_compare.why_here.x) == static_cast<float>
				     (snipp_ref.point_of_interest_end.x))
				    && (static_cast<float>(snipp_ref_compare.why_here.y) == static_cast<float>
				        (snipp_ref.point_of_interest_end.y)))
				{

					if (!((static_cast<float>(snipp_ref_compare.point_of_interest_end.x) == static_cast<float>
					       (snipp_ref.why_here.x))
					      || (static_cast<float>(snipp_ref_compare.point_of_interest_start.x) == static_cast<float>
					          (snipp_ref.why_here.x)))
					    && ((static_cast<float>(snipp_ref_compare.point_of_interest_end.y) == static_cast<float>
					         (snipp_ref.why_here.y))
					        || (static_cast<float>(snipp_ref_compare.point_of_interest_start.y) == static_cast<float>
					            (snipp_ref.why_here.y))))
					{
						return false;
					}

					if (snipp_ref_compare.snippet_intermediate_content.size() !=
					    snipp_ref.snippet_intermediate_content.size())
					{
						return false;
					}

					for (GridIndexType counter = 0 ; counter < snipp_ref_compare.snippet_intermediate_content.size() ;
					     counter++)
					{

						if (static_cast<float>(snipp_ref_compare.snippet_intermediate_content.at(
						                           counter).x) != static_cast<float>(snipp_ref.snippet_intermediate_content.at(
						                                       snipp_ref.snippet_intermediate_content.size() - (counter + 1)).x))
						{
							return false;
						}

						if (static_cast<float>(snipp_ref_compare.snippet_intermediate_content.at(
						                           counter).y) != static_cast<float>(snipp_ref.snippet_intermediate_content.at(
						                                       snipp_ref.snippet_intermediate_content.size() - (counter + 1)).y))
						{
							return false;
						}

					}

					return true;

				}
				else
				{
					return false;
				}

			}



			LegacyWorldCoordinate2D PolygonInfo::GetOtherEnd(PolygonSnippet& snipp_ref)
			{

				if ((snipp_ref.why_here.x == snipp_ref.point_of_interest_start.x)
				    && (snipp_ref.why_here.y == snipp_ref.point_of_interest_start.y))
				{
					return snipp_ref.point_of_interest_end;
				}
				else if ((snipp_ref.why_here.x == snipp_ref.point_of_interest_end.x)
				         && (snipp_ref.why_here.y == snipp_ref.point_of_interest_end.y))
				{
					return snipp_ref.point_of_interest_start;
				}
				else
				{
					assert(0);    // this should never happen
				}

				return LegacyWorldCoordinate2D();
			}


			int PolygonInfo::GetSnippetIndexDistance(SnippetIndex behind_snip_index ,
			        SnippetIndex front_snip_index)
			{

				int dist = 0;

				while (!SnippetIndexIdentical(behind_snip_index , front_snip_index))
				{

					dist++;
					IncrementSnippetIndex(behind_snip_index);

				}

				return dist;
			}


			// returns true in case indicees are identical
			bool PolygonInfo::SnippetIndexIdentical(SnippetIndex first , SnippetIndex second)
			{
				if ((first.edge  ==  second.edge) && (first.snip  ==  second.snip))
				{
					return true;
				}
				else
				{
					return false;
				}
			}



			void PolygonInfo::FindCorrespondingOneAndSetMovingIndexAndSetContinueCounter(
			    PolygonSnippet& snipp_ref , int& continue_counter_ref)
			{


				SnippetIndex another_snip_index;
				another_snip_index.edge = current_moving_index.edge;
				another_snip_index.snip = current_moving_index.snip;

				IncrementSnippetIndex(another_snip_index);

				bool there_is_one = false;

				while ((!there_is_one) && ((another_snip_index.edge != current_moving_index.edge)
				                           || (another_snip_index.snip != current_moving_index.snip)))
				{

					PolygonSnippet& snipp_ref_compare = pol_inf_vec.at(another_snip_index.edge).at(
					                                        another_snip_index.snip) ;

					if (IsCorresponding(snipp_ref_compare , snipp_ref))
					{

						continue_counter_ref += GetSnippetIndexDistance(current_moving_index , another_snip_index);
						current_moving_index.edge = another_snip_index.edge;
						current_moving_index.snip = another_snip_index.snip;
						SetCurrentMovingProcessed();
						there_is_one = true;

					}

					IncrementSnippetIndex(another_snip_index);

				}


				assert(there_is_one);

			}



			bool PolygonInfo::IsOtherEdge(PolygonSnippet& snipp_ref  , int the_previous_edge)
			{

				if ((the_previous_edge == 0) || (the_previous_edge == 2))
				{

					if (static_cast<float>(snipp_ref.point_of_interest_start.y)  !=  static_cast<float>
					    (snipp_ref.point_of_interest_end.y))
					{
						return true;
					}

				}
				else if ((the_previous_edge == 1) || (the_previous_edge == 3))
				{

					if (static_cast<float>(snipp_ref.point_of_interest_start.x)  != static_cast<float>
					    (snipp_ref.point_of_interest_end.x))
					{
						return true;
					}

				}
				else
				{
					assert(0);
				}

				return false;

			}



			bool PolygonInfo::TryGetPolygonsnippetEdge(int& continue_counter_ref , int the_previous_edge)
			{

				if (pol_inf_vec.at(current_moving_index.edge).at(
				        current_moving_index.snip).snippet_intermediate_content.size() > 0)
				{

					//    ShowSnippet(pol_inf_vec.at( current_moving_index.edge ).at( current_moving_index.snip ));

					return true;

				}
				else if (IsOtherEdge(pol_inf_vec.at(current_moving_index.edge).at(current_moving_index.snip) ,
				                     the_previous_edge))
				{

					return true;

				}
				else
				{

					continue_counter_ref++;
					IncrementSnippetIndex(current_moving_index);
					return false;

				}

			}


			void PolygonInfo::IncrementSnippetIndex(SnippetIndex& the_snippet_index)
			{

				the_snippet_index.snip += 1;

				if (the_snippet_index.snip >= pol_inf_vec.at(the_snippet_index.edge).size())
				{

					the_snippet_index.snip = 0;
					the_snippet_index.edge++;

					if (the_snippet_index.edge > 3)
					{
						the_snippet_index.edge = 0;
					}

				}

			}


			bool PolygonInfo::GetNextIsEdge(SnippetIndex  snip_index)
			{

				CharacterizeIntersection characterize = pol_inf_vec.at(snip_index.edge).at(
				        snip_index.snip).characterize;

				if (characterize == inside_inside_touches)
				{
					return false;
				}
				else
				{
					return true;
				}

			}



			bool PolygonInfo::DoContinue(int& max_num , int& continue_counter_ref)
			{

				if (max_num > (continue_counter_ref))
				{
					return true;
				}
				else if (max_num == (continue_counter_ref))
				{
					return false;
				}
				else
				{
					//std::cout<< " the_dose_index.x : " << the_dose_index.x <<std::endl;
					//std::cout<< " the_dose_index.y : " << the_dose_index.y <<std::endl;
					//std::cout<< " the_dose_index.z : " << the_dose_index.z <<std::endl;
					assert(0);    // this should never happen, since countour must not intersect with itself
				}

				return false;
			}


			void PolygonInfo::CopySnippetIndex(SnippetIndex  snip_index_from , SnippetIndex&  snip_index_to)
			{

				snip_index_to.edge = snip_index_from.edge;
				snip_index_to.snip = snip_index_from.snip;

			}



			void PolygonInfo::ToZero(SnippetIndex  snip_index)
			{
				snip_index.edge = 0;
				snip_index.snip = 0;
			}




			// Der Rueckgabewert besagt, ob das was nach dem Schnittpunkt kommt drinnen oder drausen liegt. Also ob das folgende Segment
			// innerhalb der VOI liegt.
			// Ob drinen oder draussen wird anhand des Polygonsnippets in position current_moving_index und des
			// bisherigen edge_status entschieden. edge_status wird dann entsprechend neu gesetzt.

			// Der edge_status gibt an ob sich die Kontur an dieser Stelle innerhalb des Voxels oder ausserhalb des Voxels beziehungsweise
			// auf dessen Kante befindet. Zwischen ausserhalb und Kante wird nicht unterschieden.

			// Bitte beachten, dass Kontur drausen (also der edge_status), nichts darueber aussagt, ob das folgende Punkt der Voxelkante
			// ausserhalb der Kontur (VOI) liegt.
			// Im Gegensatz zum Rueckgabewert dieser Funktion it_is_inside (der Tatsaechlich die Information enthaelt, ob das folgende Kantensegment
			// innerhalb der VOI liegt) sagt einem der edge_status nichts darueber aus, ob das folgende Segment innerhalb der VOI liegt, sonder lediglich
			// wie der Polygonzug an dieser Stelle relativ zu diesem Voxel liegt. Naemlich in seinem Inneren bzw. auf der Kante oder im
			// Aussenbereich (Kante und Aussenbereich wird nicht unterschieden).

			void PolygonInfo::CheckInside(SnippetIndex current_moving_index , bool& it_is_inside_ref ,
			                              bool& just_switched_inside_ref)
			{

				EdgeStatus previous_edge_status = edge_status;

				// Hier den edge status setzen

				CharacterizeIntersection characterize = pol_inf_vec.at(current_moving_index.edge).at(
				        current_moving_index.snip).characterize;

				SetEdgeStatus(characterize);

				if (previous_edge_status !=  edge_status)
				{
					if (it_is_inside_ref)
					{
						it_is_inside_ref = false;
					}
					else
					{
						it_is_inside_ref = true;
						just_switched_inside_ref = true;
					}
				}
				else
				{
					just_switched_inside_ref = false;
				}

			}




			void PolygonInfo::SetEdgeStatus(CharacterizeIntersection characterize)
			{


				switch (characterize)
				{

					case real_intersection :
						if (edge_status == inside)
						{
							edge_status = edge_or_outside;
						}
						else
						{
							edge_status = inside;
						}

						break;

					case edge_to_edge :
						break;

					case inside_to_edge :
						if (edge_status == inside)
						{
							edge_status = edge_or_outside;
						}
						else
						{
							edge_status = inside;
						}

						break;

					case edge_to_inside :
						if (edge_status == inside)
						{
							edge_status = edge_or_outside;
						}
						else
						{
							edge_status = inside;
						}

						break;

					case outside_to_edge :
						break;

					case edge_to_outside :
						break;

					case inside_inside_touches :
						break;

					case outside_outside_touches :
						break;

					case corner :
						// nichts passiert
						break;

					default:
						assert(0);   // Fall, der nicht bedacht wurde und daher nicht auftreten darf.

				}

			}




			/*
			void PolygonInfo::SetEdgeStatus( CharacterizeIntersection characterize ){


			switch ( characterize ){

			case real_intersection :
			if( edge_status == inside ) edge_status = edge_or_outside;
			else edge_status = inside;
			break;

			case edge_to_edge :
			if( edge_status == inside )assert(0);
			// wird nochmal explizit gesetzt, denn edge_status koennte unclear sein
			edge_status = edge_or_outside;
			break;

			case inside_to_edge :
			if( edge_status == edge_or_outside )assert(0);
			edge_status = edge_or_outside;
			break;

			case edge_to_inside :
			if( edge_status == inside )assert(0);
			edge_status = inside;
			break;

			case outside_to_edge :
			if( edge_status == inside )assert(0);
			// wird nochmal explizit gesetzt, denn edge_status koennte unclear sein
			edge_status = edge_or_outside;
			break;

			case edge_to_outside :
			if( edge_status == inside )assert(0);
			// wird nochmal explizit gesetzt, denn edge_status koennte unclear sein
			edge_status = edge_or_outside;
			break;

			case inside_inside_touches :
			if( edge_status == edge_or_outside )assert(0);
			// wird nochmal explizit gesetzt, denn edge_status koennte unclear sein
			edge_status = inside;
			break;

			case outside_outside_touches :
			if( edge_status == inside )assert(0);
			// wird nochmal explizit gesetzt, denn edge_status koennte unclear sein
			edge_status = edge_or_outside;
			break;

			case corner :
			// nichts passiert
			break;

			default:
			assert( 0 ); // Fall, der nicht bedacht wurde und daher nicht auftreten darf.

			}

			}
			*/


			// Gibt eine Ecke zurueck und die Information darueber, ob diese Ecke "drin liegt".
			// Falls sich eine solche Ecke nicht findet ...      CheckCenterSurroundingStructure  .... CheckPointOnEdgeOutside
			bool PolygonInfo::SetCurrentStaticIndexKnownInsideOrOutside(SnippetIndex& current_static_index_ref ,
			        bool& nothing_to_do_ref)
			{

				bool got_it = false;


				/* std::cout<<  "  It is here !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  "  <<std::endl;

				 for( int counter = 0 ; counter < 4 ; counter++ ){

				 for( int counter_internal = 0 ; counter_internal < pol_inf_vec.at(counter).size() ; counter_internal++ ){

				 PolygonSnippet a_snippet = pol_inf_vec.at( counter ).at( counter_internal );

				 ShowSnippet( a_snippet );

				 }

				 }*/


				if (CheckCornersOutside(current_static_index_ref))
				{

					// if( ( the_dose_index.z != 4 ) && ( the_dose_index.z != 20 ) )std::cout<< "  the_dose_index.z in CheckCornersOutside and its really outside : " << the_dose_index.z <<std::endl;

					return false; // so it is outside
				}
				/*else if( CheckCornersInside( current_static_index_ref ) ){

				  return true; // so it is inside

				}*/
				else  // es gibt also keine Ecke, die ganz drausen liegt
				{

					bool completely_inside = 0;
					bool& completely_inside_ref = completely_inside;

					// CheckCenterSurroundingStructure gibt true zurueck, falls die Kontur lediglich den Voxelrand beruehrt.

					// Anmerkung : Ist es in Ordnung, dass current_static_index_ref unveraendert bleibt, falls CheckCornersOutside( current_static_index_ref )
					// false zurueckliefert und   CheckCenterSurroundingStructure( completely_inside_ref ) zeigt, dass das Voxel gar nicht
					// angeschnitten ist?
					// Ist es unpassend, dass der Rueckgabewert diese Funktion ( SetCurrentStaticIndexKnownInsideOrOutside ) unterschiedliche
					// Bedeutung hat je nach dem, ob das Voxel angeschnitten wird (dann ob Punkt den current_static_index_ref referenziert drin oder draussen )
					// oder ob das Voxel nicht angeschnitten wird (dann information ueber den Punkt in der Mitte)
					// Ich denke das muss man andern. Soll umgebaut werden.


					if (CheckCenterSurroundingStructure(completely_inside_ref))
					{

						//     if( ( the_dose_index.z != 4 ) && ( the_dose_index.z != 20 ) )std::cout<< "  the_dose_index.z Center is surounding structure : " << the_dose_index.z <<std::endl;

						nothing_to_do_ref = true;
						return completely_inside;
					}

				}


				bool inside = false;
				bool& inside_ref = inside;


				got_it = CheckPointOnEdgeOutside(current_static_index_ref  , inside_ref);


				if (got_it)
				{
					return inside;
				}
				else
				{
					assert(0);    // this must not happen
				}

				return false;
			}


			// Gibt, falls es eine Ecke gibt, die drausen liegt true zurueck und setzt current_static_index_ref gegebenenfalls auf die entsprechende Ecke.
			bool PolygonInfo::CheckCornersOutside(SnippetIndex&  current_static_index_ref)
			{



				Uint16 x = the_dose_index.x;
				Uint16 y = the_dose_index.y;
				Uint16 z = the_dose_index.z;


				if (MaskFieldReceived->GetData(x, y, z) == brightness_outside)
				{
					assert(0);    // this should never happen since we are dealing with a voxel that is touched by the structure.
				}

				//if( MaskFieldReceived->GetData( x, y, z ) == brightness_inside ) assert( 0 ); // this should never happen since we are dealing with a voxel that is touched by the structure.


				if ((CheckThereIsSuchNeighbour(x , y , z , -1 , -1 , 0, brightness_outside)))
				{
					current_static_index_ref.edge = 0;
					current_static_index_ref.snip = 0;
					return true;
				}
				else if ((CheckThereIsSuchNeighbour(x , y , z , 1 , -1 , 0 , brightness_outside)))
				{
					current_static_index_ref.edge = 1;
					current_static_index_ref.snip = 0;
					return true;
				}
				else if ((CheckThereIsSuchNeighbour(x , y , z , 1 , 1 , 0, brightness_outside)))
				{
					current_static_index_ref.edge = 2;
					current_static_index_ref.snip = 0;
					return true;
				}
				else if ((CheckThereIsSuchNeighbour(x , y , z , -1 , 1 , 0, brightness_outside)))
				{
					current_static_index_ref.edge = 3;
					current_static_index_ref.snip = 0;
					return true;
				}

				return false;

			}



			// Gibt, falls es eine Ecke gibt, die drinnen liegt true zurueck und setzt current_static_index_ref gegebenenfalls auf die entsprechende Ecke.
			bool PolygonInfo::CheckCornersInside(SnippetIndex&  current_static_index_ref)
			{



				Uint16 x = the_dose_index.x;
				Uint16 y = the_dose_index.y;
				Uint16 z = the_dose_index.z;


				if (MaskFieldReceived->GetData(x, y, z) == brightness_outside)
				{
					assert(0);    // this should never happen since we are dealing with a voxel that is touched by the structure.
				}

				//if( MaskFieldReceived->GetData( x, y, z ) == brightness_inside ) assert( 0 );

				if ((CheckThereIsSuchNeighbour(x , y , z , -1 , -1 , 0, brightness_inside)))
				{
					current_static_index_ref.edge = 0;
					current_static_index_ref.snip = 0;
					return true;
				}
				else if ((CheckThereIsSuchNeighbour(x , y , z , 1 , -1 , 0, brightness_inside)))
				{
					current_static_index_ref.edge = 1;
					current_static_index_ref.snip = 0;
					return true;
				}
				else if ((CheckThereIsSuchNeighbour(x , y , z , 1 , 1 , 0, brightness_inside)))
				{
					current_static_index_ref.edge = 2;
					current_static_index_ref.snip = 0;
					return true;
				}
				else if ((CheckThereIsSuchNeighbour(x , y , z , -1 , 1 , 0, brightness_inside)))
				{
					current_static_index_ref.edge = 3;
					current_static_index_ref.snip = 0;
					return true;
				}

				return false;

			}


			bool PolygonInfo::CheckThereIsSuchNeighbour(Uint16 x , Uint16 y , Uint16 z , int shift_x ,
			        int shift_y  , int shift_z, field_content_type brightness)
			{

				Uint16 x_intern = x + shift_x;
				Uint16 y_intern = y;

				if ((x_intern < MaskFieldReceived->GetDimX()) && (x_intern >= 0))
				{
					if (MaskFieldReceived->GetData(x_intern, y_intern, z) == brightness)
					{
						return true;
					}
				}

				x_intern = x + shift_x;
				y_intern = y + shift_y;

				if ((x_intern < MaskFieldReceived->GetDimX()) && (x_intern >= 0)
				    && (y_intern < MaskFieldReceived->GetDimY()) && (y_intern >= 0))
				{
					if (MaskFieldReceived->GetData(x_intern, y_intern, z) == brightness)
					{
						return true;
					}
				}

				x_intern = x;
				y_intern = y + shift_y;

				if ((y_intern < MaskFieldReceived->GetDimY()) && (y_intern >= 0))
				{
					if (MaskFieldReceived->GetData(x_intern, y_intern, z) == brightness)
					{
						return true;
					}
				}

				return false;

			}




			// Gibt true zurueck, falls das Voxel von der Kontur gar nicht angeschnitten wird.
			bool PolygonInfo::CheckCenterSurroundingStructure(bool& inside)
			{

				bool no_intersections = true;

				SnippetIndex a_snippet_index;
				a_snippet_index.snip = 0;
				a_snippet_index.edge = 0;

				PolygonSnippet a_snippet;

				for (a_snippet_index.edge = 0 ; a_snippet_index.edge < 4 ; a_snippet_index.edge++)
				{
					for (a_snippet_index.snip = 0 ;
					     a_snippet_index.snip < (pol_inf_vec.at(a_snippet_index.edge).size()) ; a_snippet_index.snip++)
					{

						a_snippet = pol_inf_vec.at(a_snippet_index.edge).at(a_snippet_index.snip);

						if (!((a_snippet.characterize == outside_outside_touches)
						      || (a_snippet.characterize == edge_to_edge) || (a_snippet.characterize == corner)
						      || (a_snippet.characterize == outside_to_edge)  || (a_snippet.characterize == edge_to_outside)))
						{
							no_intersections = false;
						}

					}
				}

				if (no_intersections)
				{

					LegacyWorldCoordinate3D a_voxel_coordinate;
					a_voxel_coordinate.x = (the_dose_index.x + 0.5);
					a_voxel_coordinate.y = (the_dose_index.y + 0.5);
					a_voxel_coordinate.z = the_dose_index.z;


					LegacyWorldCoordinate3D a_world_coordinate = GetWorldCoordinate(a_voxel_coordinate);

					Contour contour(the_polygon);
					inside = contour.pointInPolygon(a_world_coordinate);


				}

				return  no_intersections;

			}



			// Die folgende Funktionalitaet ist in DoseVoxel im Prinziep bereits vorhanden. Daher Lanlan fragen, ob man sie dort
			// zugaenglich machen kann, damit wir sie hier nicht verdoppeln muessen. Allerdings ist der Input hier  .
			LegacyWorldCoordinate3D PolygonInfo::GetWorldCoordinate(LegacyWorldCoordinate3D voxel_coordinate)
			{

				LegacyWorldCoordinate3D world_coordinate;
				LegacyWorldCoordinate3D spacing;
				spacing.x = GInformation.getPixelSpacingRow();
				spacing.y = GInformation.getPixelSpacingColumn();
				spacing.z = GInformation.getSliceThickness();
				OrientationMatrix orientation = GInformation.getOrientationMatrix();
				LegacyWorldCoordinate3D posPatient =  GInformation.getImagePositionPatient();

				world_coordinate.x = (orientation(0, 0) * spacing.x * (voxel_coordinate.x - 0.5) + orientation(0,
				                      1) * spacing.y * (voxel_coordinate.y - 0.5) + posPatient.x);
				world_coordinate.y = (orientation(1, 0) * spacing.x * (voxel_coordinate.x - 0.5) + orientation(1,
				                      1) * spacing.y * (voxel_coordinate.y - 0.5) + posPatient.y);
				world_coordinate.z = (posPatient.z + spacing.z * (voxel_coordinate.z - 0.5));

				return world_coordinate;

			}


			void PolygonInfo::SetCurrentPosition(const LegacyDoseVoxelIndex3D& aDoseIndex)
			{
				current_position.x = static_cast<WorldCoordinate>(aDoseIndex.x);
				current_position.y = static_cast<WorldCoordinate>(aDoseIndex.y);
			}



			bool PolygonInfo::SnippetIndexIsIdentical(SnippetIndex first_index , SnippetIndex second_index)
			{
				if ((first_index.edge == second_index.edge) && (first_index.snip == second_index.snip))
				{
					return true;
				}
				else
				{
					return false;
				}
			}



			// Anpassung fuer beruehrende Strukturen noetig. Dann muss sichergestellt werdedn, dass current_static_index_ref nicht ein
			// snippet repraesentiert das von einem anderen Snippet gefolgt wird fuer das .why_here identisch ist.
			bool PolygonInfo::CheckPointOnEdgeOutside(SnippetIndex& current_static_index_ref , bool& inside)
			{

				bool got_it = false;

				SnippetIndex a_snippet_index;
				a_snippet_index.snip = 0;
				a_snippet_index.edge = 0;

				PolygonSnippet a_snippet;
				PolygonSnippet next_snippet;

				for (a_snippet_index.edge = 0 ; a_snippet_index.edge < 4 ; a_snippet_index.edge++)
				{

					for (a_snippet_index.snip = 0 ;
					     a_snippet_index.snip < (pol_inf_vec.at(a_snippet_index.edge).size()) ; a_snippet_index.snip++)
					{

						a_snippet = pol_inf_vec.at(a_snippet_index.edge).at(a_snippet_index.snip);

						if ((a_snippet.characterize == real_intersection) || (a_snippet.characterize == edge_to_inside)
						    || (a_snippet.characterize == edge_to_outside)
						    || (a_snippet.characterize == inside_inside_touches))
						{

							int at_begin = 0;

							while ((got_it == false) && (at_begin < 2))
							{

								// folgenden Teil auslagern als Funkton IncrementSnippetIndex()
								if (pol_inf_vec.at(a_snippet_index.edge).size() > (a_snippet_index.snip + 1))
								{
									a_snippet_index.snip += 1;
									next_snippet = pol_inf_vec.at(a_snippet_index.edge).at(a_snippet_index.snip);
								}
								else if (a_snippet_index.edge < 3)
								{
									a_snippet_index.edge += 1;
									a_snippet_index.snip = 0;
									next_snippet = pol_inf_vec.at(a_snippet_index.edge).at(a_snippet_index.snip);
								}
								else
								{
									a_snippet_index.edge = 0;
									a_snippet_index.snip = 0;
									next_snippet = pol_inf_vec.at(a_snippet_index.edge).at(a_snippet_index.snip);
									at_begin++;
								}

								// ende auszulagern als Funkton IncrementSnippetIndex()

								if ((static_cast<float>(a_snippet.why_here.x) != static_cast<float>(next_snippet.why_here.x))
								    || (static_cast<float>(a_snippet.why_here.y) != static_cast<float>(next_snippet.why_here.y)))
								{

									LegacyWorldCoordinate3D a_voxel_coordinate;
									a_voxel_coordinate.x = ((a_snippet.why_here.x + next_snippet.why_here.x) * 0.5);
									a_voxel_coordinate.y = ((a_snippet.why_here.y + next_snippet.why_here.y) * 0.5);
									a_voxel_coordinate.z = the_dose_index.z;

									LegacyWorldCoordinate3D a_world_coordinate = GetWorldCoordinate(a_voxel_coordinate);

									/*
									std::cout<< " a_voxel_coordinate.x  : " << a_voxel_coordinate.x  <<std::endl;
									std::cout<< " a_voxel_coordinate.y  : " << a_voxel_coordinate.y <<std::endl;
									std::cout<< " a_voxel_coordinate.z  : " << a_voxel_coordinate.z <<std::endl;

									std::cout<<  " GInformation.getNumColumns(): "  <<  GInformation.getNumColumns()<<std::endl;
									std::cout<<  " GInformation.getNumRows(): "  <<  GInformation.getNumRows()   <<std::endl;
									std::cout<<  " GInformation.numberOfFrames : "  <<  GInformation.numberOfFrames <<std::endl;

									*/

									Contour contour(the_polygon);
									inside = contour.pointInPolygon(a_world_coordinate);



									//              std::cout<< "  inside : " << inside <<std::endl;

									current_static_index_ref.snip = a_snippet_index.snip;
									current_static_index_ref.edge = a_snippet_index.edge;

									got_it = true;

								}

							}


						}


					}
				}

				if (got_it == false)
				{

					for (a_snippet_index.edge = 0 ; a_snippet_index.edge < 4 ; a_snippet_index.edge++)
					{

						for (a_snippet_index.snip = 0 ;
						     a_snippet_index.snip < (pol_inf_vec.at(a_snippet_index.edge).size()) ; a_snippet_index.snip++)
						{

							a_snippet = pol_inf_vec.at(a_snippet_index.edge).at(a_snippet_index.snip);

							if ((a_snippet.characterize == inside_to_edge)  || (a_snippet.characterize == outside_to_edge))
							{

								int at_end = 0;

								while ((got_it == false) && (at_end < 2))
								{

									// Folgender Bereich is auszulagern als Funkton DecrementSnippetIndex()
									if (a_snippet_index.snip > 0)
									{
										a_snippet_index.snip -= 1;
										next_snippet = pol_inf_vec.at(a_snippet_index.edge).at(a_snippet_index.snip);
									}
									else if (a_snippet_index.edge > 0)
									{
										a_snippet_index.edge -= 1;
										a_snippet_index.snip = pol_inf_vec.at(a_snippet_index.edge).size() - 1;
										next_snippet = pol_inf_vec.at(a_snippet_index.edge).at(a_snippet_index.snip);
									}
									else
									{
										a_snippet_index.edge = 3;
										a_snippet_index.snip = pol_inf_vec.at(a_snippet_index.edge).size() - 1;
										next_snippet = pol_inf_vec.at(a_snippet_index.edge).at(a_snippet_index.snip);
										at_end++;
									}

									// ende auszulagern als Funkton DecrementSnippetIndex()


									if ((static_cast<float>(a_snippet.why_here.x) != static_cast<float>(next_snippet.why_here.x))
									    || (static_cast<float>(a_snippet.why_here.y) != static_cast<float>(next_snippet.why_here.y)))
									{

										LegacyWorldCoordinate3D a_voxel_coordinate;
										a_voxel_coordinate.x = ((a_snippet.why_here.x + next_snippet.why_here.x) * 0.5);
										a_voxel_coordinate.y = ((a_snippet.why_here.y + next_snippet.why_here.y) * 0.5);
										a_voxel_coordinate.z = the_dose_index.z;


										LegacyWorldCoordinate3D a_world_coordinate = GetWorldCoordinate(a_voxel_coordinate);

										Contour contour(the_polygon);
										inside = contour.pointInPolygon(a_world_coordinate);


										// folgenden Teil auslagern als Funkton IncrementSnippetIndex()
										if (pol_inf_vec.at(a_snippet_index.edge).size() > (a_snippet_index.snip + 1))
										{
											a_snippet_index.snip += 1;
											next_snippet = pol_inf_vec.at(a_snippet_index.edge).at(a_snippet_index.snip);
										}
										else if (a_snippet_index.edge < 3)
										{
											a_snippet_index.edge += 1;
											a_snippet_index.snip = 0;
											next_snippet = pol_inf_vec.at(a_snippet_index.edge).at(a_snippet_index.snip);
										}
										else
										{
											a_snippet_index.edge = 0;
											a_snippet_index.snip = 0;
											next_snippet = pol_inf_vec.at(a_snippet_index.edge).at(a_snippet_index.snip);

										}

										// ende auszulagern als Funkton IncrementSnippetIndex()

										current_static_index_ref.snip = a_snippet_index.snip;
										current_static_index_ref.edge = a_snippet_index.edge;

										got_it = true;

									}

								}
							}

						}
					}

				}

				return got_it;

			}


			void PolygonInfo::ResetSnippetIndicees()
			{

				current_static_index.edge = 0;
				current_static_index.snip = 0;
				current_moving_index.edge = 0;
				current_moving_index.snip = 0;
				CopySnippetIndex(current_static_index , begin_index);

			}


			void PolygonInfo::SetIndexIdentical(IterativePolygonInTermsOfIntersections::WhichIntersection&
			                                    intersection_index_to_ref , IterativePolygonInTermsOfIntersections::WhichIntersection&
			                                    intersection_index_from_ref)
			{

				intersection_index_to_ref.point_index = intersection_index_from_ref.point_index;
				intersection_index_to_ref.intersection_index = intersection_index_from_ref.intersection_index;
				intersection_index_to_ref.column_raw_or_unified = intersection_index_from_ref.column_raw_or_unified;

			}








		}//namespace
	}//namespace
}//namespace



