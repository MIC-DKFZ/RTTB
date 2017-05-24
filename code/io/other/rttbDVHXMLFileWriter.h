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
#ifndef __DVH_XML_FILE_WRITER_H
#define __DVH_XML_FILE_WRITER_H


#include "rttbDVH.h"
#include "../rttbDVHWriterInterface.h"
#include "rttbBaseType.h"

#include "RTTBOtherIOExports.h"

namespace rttb
{
	namespace io
	{
		namespace other
		{

			/*! @class DVHXMLFileWriter
			@brief Writes DVHs to xml files.
			*/
            class RTTBOtherIO_EXPORT DVHXMLFileWriter : public DVHWriterInterface
			{
			public:
				typedef core::DVH::DataDifferentialType DataDifferentialType;
				typedef core::DVH::DVHPointer DVHPointer;

			private:
				FileNameString _fileName;
				DVHType _dvhType;

			public:
				/*! @brief Constructor
				@param aFileName a xml file name to write 
                @param aDVHType: DIFFERENTIAL or CUMULATIVE.
				*/
				DVHXMLFileWriter(FileNameString aFileName, DVHType aDVHType);

				void setFileName(FileNameString aFileName);
				FileNameString getFileName() const;

				void setDVHType(DVHType aDVHType);
				DVHType getDVHType() const;

				/*! @brief Write aDvh to xml file with the name: _fileName
				@exception NullPointerException Thrown if _aDvh is NULL
				@exception InvalidParameterException Thrown if _fileName invalid: could not open;
				or if _dvhType invalid: only DIFFERENTIAL or CUMULATIVE is accepted!
				@exception Exception thrown if dvh init error
				*/
				void writeDVH(DVHPointer aDvh, bool normalized = false);
			};
		}
	}
}
#endif
