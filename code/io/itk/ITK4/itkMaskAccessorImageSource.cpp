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

#include "itkMaskAccessorImageSource.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkProgressReporter.h"
#include "itkExceptionObject.h"

#include "rttbGeometricInfo.h"

namespace itk
{
	/**
	* Constructor
	*/
	MaskAccessorImageSource
    ::MaskAccessorImageSource() 
	= default;

	void
	MaskAccessorImageSource
	::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
	                       ThreadIdType threadId)
	{
		ProgressReporter progress(this, threadId,
		                          outputRegionForThread.GetNumberOfPixels());

		using OutputImageRegionIteratorType = ImageRegionIterator<OutputImageType>;

		OutputImagePointer outputPtr = dynamic_cast< OutputImageType* >(ProcessObject::GetOutput(0));
		OutputImageRegionIteratorType outputItr;

		if (outputPtr)
		{
			outputItr = OutputImageRegionIteratorType(outputPtr, outputRegionForThread);

      for (; !(outputItr.IsAtEnd()); ++outputItr)
			{
        OutputImageType::IndexType index = outputItr.GetIndex();
        rttb::VoxelGridIndex3D maskIndex(index[0], index[1], index[2]);
        rttb::VoxelGridID id = 0;

        if (m_Accessor->getGeometricInfo().convert(maskIndex, id))
        {
          auto finding = m_idMap.find(id);
          if (finding != m_idMap.end())
          {
            // Set the current pixel
            outputItr.Set(m_MaskedVoxels->at(finding->second).getRelevantVolumeFraction());
          }
        }
        else
        {
          if (m_FailsOnInvalidIDs)
          {
            itkExceptionMacro(<<"invalid Mask index. Index:"<<index);
          }
          else
          {
            outputItr.Set(m_InvalidMaskValue);
          }
        }

				progress.CompletedPixel();
			}
		}
	}

  void
    MaskAccessorImageSource
    ::BeforeThreadedGenerateData()
  {

    OutputImagePointer outputPtr = dynamic_cast< OutputImageType* >(ProcessObject::GetOutput(0));
    outputPtr->FillBuffer(0.0);

    //The id map approach and working with relevant voxel vector is a workarround till task T22067 is solved and and can be used again.
    m_MaskedVoxels = m_Accessor->getRelevantVoxelVector();

    m_idMap.clear();

    for (rttb::core::MaskAccessorInterface::MaskVoxelList::size_type pos = 0; pos < m_MaskedVoxels->size(); ++pos)
    {
      m_idMap.insert(std::make_pair((*m_MaskedVoxels)[pos].getVoxelGridID(), pos));
    }
  }
} // end namespace itk

