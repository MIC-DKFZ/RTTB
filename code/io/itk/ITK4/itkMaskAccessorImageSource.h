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

#ifndef __itkMaskAccessorImageSource_h
#define __itkMaskAccessorImageSource_h

#include "itkGenerateImageSource.h"

#include "rttbMaskAccessorInterface.h"
#include "rttbITKImageMaskAccessor.h"

namespace itk
{
	/** \class MaskAccessorImageSource
	 * \brief Converts a given mask accessor into an itk image.
	 *
	 */

  typedef rttb::io::itk::ITKImageMaskAccessor::ITKMaskImageType RTTBMaskImageType;

	class ITK_EXPORT MaskAccessorImageSource:
    public GenerateImageSource< RTTBMaskImageType>

	{
	public:
		/** Standard class typedefs. */
		typedef MaskAccessorImageSource                          Self;
    typedef GenerateImageSource< RTTBMaskImageType > Superclass;
		typedef SmartPointer< Self >                            Pointer;
		typedef SmartPointer< const Self >                      ConstPointer;
		/** Method for creation through the object factory. */
		itkNewMacro(Self);

		/** Run-time type information (and related methods). */
		itkTypeMacro(MaskAccessorImageSource, ImageToImageFilter);

		/** Some typedefs. */
    typedef RTTBMaskImageType           OutputImageType;
		typedef OutputImageType::Pointer    OutputImagePointer;
		typedef OutputImageType::RegionType OutputImageRegionType;
		typedef OutputImageType::PixelType  OutputImagePixelType;

		typedef rttb::core::MaskAccessorInterface AccessorType;
    typedef rttb::core::MaskAccessorInterface::Pointer AccessorPointer;

		/** Get the accessor pointer. */
    itkGetConstMacro(Accessor, AccessorPointer);

		/** Set the accessor pointer. */
    void SetAccessor(AccessorPointer accessor)
		{
			if (m_Accessor != accessor)
			{
				m_Accessor = accessor;
				this->Modified();
			}
		}

    itkSetMacro(FailsOnInvalidIDs, bool);
    itkGetConstMacro(FailsOnInvalidIDs, bool);

    itkSetMacro(InvalidMaskValue, OutputImageType::ValueType);
    itkGetConstMacro(InvalidMaskValue, OutputImageType::ValueType);

		/** ImageDimension constants */
		itkStaticConstMacro(
		    OutputImageDimension, unsigned int, OutputImageType::ImageDimension);

	protected:
		MaskAccessorImageSource();
		~MaskAccessorImageSource() override {}

		void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
		                          ThreadIdType threadId) override;

    void BeforeThreadedGenerateData() override;

	private:
		MaskAccessorImageSource(const Self&);  //purposely not implemented
		void operator=(const Self&);          //purposely not implemented

    typedef std::map <rttb::VoxelGridID, rttb::core::MaskAccessorInterface::MaskVoxelList::size_type> VoxelMapType;
    VoxelMapType m_idMap;
    rttb::core::MaskAccessorInterface::MaskVoxelListPointer m_MaskedVoxels;

    AccessorPointer m_Accessor;
    bool m_FailsOnInvalidIDs{true};
    OutputImageType::ValueType m_InvalidMaskValue{0};
	};
} // end namespace itk


#endif
