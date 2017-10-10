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
#ifndef __DVH_TXT_FILE_READER_H
#define __DVH_TXT_FILE_READER_H

#include "rttbBaseType.h"
#include "rttbDVHGeneratorInterface.h"

namespace rttb
{
	namespace io
	{
		namespace other
		{

			/*! @class DVHTxtFileReader
			@brief Reads DVH data from txt files.
            @deprecated Please use DVHXMLFileReader.
			*/
			class DVHTxtFileReader: public core::DVHGeneratorInterface
			{


			private:
				FileNameString _fileName;
				bool _resetFile;

				/*! @brief Create new DVH object using the info from dvh txt file
					@exception InvalidParameterException Thrown if _fileName invalid
				*/
				void createDVH();

				void calculateDataCumulative(const std::deque<DoseTypeGy>& dataCumulative, std::deque<DoseTypeGy>& dataDifferential, unsigned int numberOfBins) const;

				/*! @brief Load the DVH data either in a vector for differential data or in one for cumulative data
					@exception InvalidParameterException Thrown if splitting of a line in the file is invalid
				*/
				void loadData(const std::string& line, std::deque<DoseTypeGy>& data) const;

			public:
				/*! @brief Constructor.
				*/
				DVHTxtFileReader(FileNameString aFileName);

				~DVHTxtFileReader();

				/*! @brief Change file name.
				*/
				void resetFileName(FileNameString aFileName);

				/*! @brief Generate DVH, createDVH() will be called
					@return Return new shared pointer of DVH.
					@exception InvalidParameterException Thrown if _fileName invalid
				*/
				DVHPointer generateDVH() override;
			};
		}
	}
}

#endif
