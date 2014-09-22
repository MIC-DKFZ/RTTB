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

#include "rttbStructure.h"
#include "rttbBaseType.h"
#include "rttbGeometricInfo.h"

namespace rttb{
	namespace testing{
		/*!@class CreateTestStructure
		@brief Create dummy structures for testing. 
		*/
		class CreateTestStructure{

		private:
			core::GeometricInfo _geoInfo;
			CreateTestStructure(){};

		public:
			~CreateTestStructure();

			CreateTestStructure(const core::GeometricInfo& aGeoInfo);

			PolygonType createPolygonLeftUpper(std::vector<VoxelGridIndex2D> aVoxelVector, GridIndexType sliceNumber);
			PolygonType createPolygonCenter(std::vector<VoxelGridIndex2D> aVoxelVector, GridIndexType sliceNumber);
			PolygonType createPolygonBetweenUpperLeftAndCenter(std::vector<VoxelGridIndex2D> aVoxelVector, GridIndexType sliceNumber);
			PolygonType createPolygonBetweenLowerRightAndCenter(std::vector<VoxelGridIndex2D> aVoxelVector, GridIndexType sliceNumber);
			PolygonType createPolygonIntermediatePoints(std::vector<VoxelGridIndex2D> aVoxelVector, GridIndexType sliceNumber);
			PolygonType createPolygonUpperCenter(std::vector<VoxelGridIndex2D> aVoxelVector, GridIndexType sliceNumber);
			PolygonType createPolygonLeftEdgeMiddle(std::vector<VoxelGridIndex2D> aVoxelVector, GridIndexType sliceNumber);

			PolygonType createPolygonCircle(std::vector<VoxelGridIndex2D> aVoxelVector, GridIndexType sliceNumber); 	 

			PolygonType createStructureSeveralSectionsInsideOneVoxelA(std::vector<VoxelGridIndex2D> aVoxelVector, GridIndexType sliceNumber);

			PolygonType createStructureSelfTouchingA(std::vector<VoxelGridIndex2D> aVoxelVector, GridIndexType sliceNumber);

			PolygonType createStructureSelfTouchingB(std::vector<VoxelGridIndex2D> aVoxelVector, GridIndexType sliceNumber);	 
			};
		}//testing
	}//rttb

