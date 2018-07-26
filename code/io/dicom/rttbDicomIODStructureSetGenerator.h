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

/* Changes in Architecture:
The DICOM specific classes will be removed and transfered to the corresponding IO classes.
This class should only provide general structure functionality.
*/

#ifndef __DICOM_IOD_STRUCTURE_SET_GENERATOR_H
#define __DICOM_IOD_STRUCTURE_SET_GENERATOR_H

#include <boost/shared_ptr.hpp>

#include "drtstrct.h"

#include "rttbBaseType.h"
#include "rttbStrVectorStructureSetGenerator.h"

namespace rttb
{
	namespace io
	{
		namespace dicom
		{

			/*! @class DicomIODStructureSetGenerator
			@brief Generate a structure set from a DRTStructureSetIOD pointer.
			*/
			class DicomIODStructureSetGenerator: public core::StrVectorStructureSetGenerator
			{
			public:
				using StructTypePointer = core::Structure::Pointer;
				using StructureSetPointer = core::StructureSet::Pointer;
				using DRTStrSetIODPtr = boost::shared_ptr<DRTStructureSetIOD>;

			private:
				DRTStrSetIODPtr _drtStrSetIOD;
				IDType _UID;



				/*!	Import Structure data from file.
				@exception InvalidParameterException Thrown if the imported header tags are not numerical.
				*/
				void readStrSet();
        /*!	Replaces the character 'á' ((int)-96) to ' ' ((int)32) in a string.
        */
        void correctSpacesInROIName(std::string& roiName);
      public:

				/*! @brief Structure Constructor
				Get the vector of structures from DRTStructureSetIOD object
				@exception NullPointerException Thrown if structureSet is nullptr
				*/
				DicomIODStructureSetGenerator(DRTStrSetIODPtr aDRTStructureSetIOD);

				/*! @brief Destructor
				*/
				~DicomIODStructureSetGenerator() override;

				/*! @brief generate structure set
				@return return shared pointer of StructureSet
				@exception InvalidParameterException throw if the imported header tags are not numerical.
				*/
				StructureSetPointer generateStructureSet() override;

			};
		}
	}
}

#endif
