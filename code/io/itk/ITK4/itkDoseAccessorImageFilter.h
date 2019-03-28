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

#ifndef __itkDoseAccessorImageFilter_h
#define __itkDoseAccessorImageFilter_h

#include "itkImageToImageFilter.h"

#include "rttbDoseAccessorInterface.h"
#include "rttbITKImageAccessor.h"

namespace itk
{
	/** \class DoseAccessorImageFilter
	 * \brief Perform a generic pixel-wise operation on the input image by setting its pixel values according to the dose accessor output.
	 *
	 * \ingroup IntensityImageFilters MultiThreaded
	 * \ingroup ITKImageIntensity
	 */

	typedef rttb::io::itk::ITKImageAccessor::ITKImageType RTTBDoseImageType;

	class ITK_EXPORT DoseAccessorImageFilter:
		public ImageToImageFilter< RTTBDoseImageType, RTTBDoseImageType >

	{
	public:
		/** Standard class typedefs. */
		typedef DoseAccessorImageFilter                          Self;
		typedef ImageToImageFilter< RTTBDoseImageType, RTTBDoseImageType > Superclass;
		typedef SmartPointer< Self >                            Pointer;
		typedef SmartPointer< const Self >                      ConstPointer;
		/** Method for creation through the object factory. */
		itkNewMacro(Self);

		/** Run-time type information (and related methods). */
		itkTypeMacro(DoseAccessorImageFilter, ImageToImageFilter);

		/** Some typedefs. */
		typedef RTTBDoseImageType                          InputImageType;
		typedef InputImageType::Pointer     InputImagePointer;
		typedef InputImageType::RegionType  InputImageRegionType;
		typedef InputImageType::PixelType   InputImagePixelType;
		typedef RTTBDoseImageType                         OutputImageType;
		typedef OutputImageType::Pointer    OutputImagePointer;
		typedef OutputImageType::RegionType OutputImageRegionType;
		typedef OutputImageType::PixelType  OutputImagePixelType;

		typedef rttb::core::DoseAccessorInterface DoseAccessorType;
		typedef rttb::core::DoseAccessorInterface::Pointer DoseAccessorPointer;

		/** Get the accessor pointer. */
		DoseAccessorPointer GetAccessor()
		{
			return m_Accessor;
		}

		/** Set the accessor pointer. */
		void SetAccessor(DoseAccessorPointer accessor)
		{
			if (m_Accessor != accessor)
			{
				m_Accessor = accessor;
				this->Modified();
			}
		}

		/** ImageDimension constants */
		itkStaticConstMacro(
		    InputImageDimension, unsigned int, InputImageType::ImageDimension);
		itkStaticConstMacro(
		    OutputImageDimension, unsigned int, OutputImageType::ImageDimension);

	protected:
		DoseAccessorImageFilter();
		~DoseAccessorImageFilter() override {}

		/** DoseAccessorImageFilter can be implemented as a multi threaded filter.
		 * Therefore, this implementation provides a ThreadedGenerateData() routine
		 * which is called for each processing thread. The output image data is
		 * allocated automatically by the superclass prior to calling
		 * ThreadedGenerateData().  ThreadedGenerateData can only write to the
		 * portion of the output image specified by the parameter
		 * "outputRegionForThread"
		 *
		 * \sa ImageToImageFilter::ThreadedGenerateData(),
		 *     ImageToImageFilter::GenerateData()  */
		void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
		                          ThreadIdType threadId) override;

	private:
		DoseAccessorImageFilter(const Self&);  //purposely not implemented
		void operator=(const Self&);          //purposely not implemented

		DoseAccessorPointer m_Accessor;
	};
} // end namespace itk


#endif
