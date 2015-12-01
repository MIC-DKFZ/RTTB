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
The DICOM specific classes will be removed and transfered to the corresponding IO classes.
This class should only provide general structure functionality.
*/

#ifndef __VIRTUOS_FILE_STRUCTURE_SET_GENERATOR_H
#define __VIRTUOS_FILE_STRUCTURE_SET_GENERATOR_H

#include <vector>

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

// Virtuos includes
#include "ncfile.h"
#include "voi_man.hxx"

#include "rttbBaseType.h"
#include "rttbStrVectorStructureSetGenerator.h"
#include "rttbStructure.h"


namespace rttb
{
	namespace io
	{
		namespace virtuos
		{

			/*! @class VirtuosFileStructureSetGenerator
			@brief Genereate a structure set from a corresponding Virtuos data.
			*/
			class VirtuosFileStructureSetGenerator: public core::StrVectorStructureSetGenerator
			{
			public:
				typedef core::StructureSet::StructTypePointer StructTypePointer;
				typedef StructureSetGeneratorInterface::StructureSetPointer StructureSetPointer;

			private:
				// Pointer to Virtuos image data cube
				Cubeinfo** _pPointerOnVirtuosCube;

				//Virtuos Patient Pointer
				VoiModel* _patient;

				IDType _UID;
				FileNameType _VDXFileName;
				FileNameType _CTXFileName;


				/*! open virtuos ctx cube
				@throw InvalidParameterException if ctx cube could not be opened or dose dimensions are invalid
				*/
				void initializeVirtuosCube(FileNameType aVirtuosCTXFileName);

				/*! import virtuos vdx contours from file
				@throw InvalidParameterException if one of the file names is empty.
				@throw InvalidParameterException if aVirtuosVDXFileName, does not end with '.vdx'.
				*/
				void importStructureSet(FileNameType aVirtuosVDXFileName, FileNameType aVirtuosCTXFileName);


			public:
				/*! @brief Constructor
				@param aVirtuosVDXFileName a Virtuos structure set .vdx file name
				@param aVirtuosCTXFileName a Virtuos CT .ctx file name

				*/
				VirtuosFileStructureSetGenerator(FileNameType aVirtuosVDXFileName,
				                                 FileNameType aVirtuosCTXFileName);

				/*! @brief Destructor
				Free Virtuos Cubeinfo.
				*/
				~VirtuosFileStructureSetGenerator();

				/*! @brief generate structure set
				@return return shared pointer of StructureSet
				@exception InvalidParameterException Thrown if loadFile and read failed
				@exception InvalidParameterException throw if structure data invalid.
				*/
				StructureSetPointer generateStructureSet();
			};
		}
	}
}

#endif
