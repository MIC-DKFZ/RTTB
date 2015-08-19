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
#ifndef __ITK_DOSE_ACCESSOR_H
#define __ITK_DOSE_ACCESSOR_H

#include <exception>

#include <boost/shared_ptr.hpp>

#include "rttbDoseAccessorWithGeoInfoBase.h"
#include "rttbGeometricInfo.h"
#include "rttbBaseType.h"

#include "itkImage.h"

namespace rttb
{
  namespace io
  {
    namespace itk
    {
      typedef ::itk::Image<DoseTypeGy, 3> ITKDoseImageType;
      typedef ::itk::ImageBase<3> ITKImageBaseType;
      /*! @class ITKImageDoseAccessor
      @brief This class gives access to dose information stored in an itk image
      @details _doseGridScaling is always 1.0. Thus, it is assumed that the values in the itkImage are absolute.
      */
      class ITKImageDoseAccessor: public core::DoseAccessorWithGeoInfoBase
      {
      private:
        /** @brief The dose as itkImage */
        ITKDoseImageType::ConstPointer _dose;

        IDType _doseUID;

        /** @brief The dosegridscaling
        *	@note is always 1.0
        */
        double _doseGridScaling;

        /*! @brief constructor
        	@exception InvalidDoseException if _dose is NULL
        */
        ITKImageDoseAccessor();

        /*! @brief get all required data from the itk image contained in _dose
        	@exception InvalidDoseException if PixelSpacing is 0 or size in any dimension is 0.
        */
        bool assembleGeometricInfo();


      public:
        ~ITKImageDoseAccessor();

        /*! @brief Constructor. Initialization with a itk image containing the dose
        @pre doseImage must be a valid instance (and not null)
        @note the doseImage pixels are dose (i.e. _doseGridScaling=1.0 is assumed always)
        */
        ITKImageDoseAccessor(ITKDoseImageType::ConstPointer doseImage);

        /*! @brief returns the dose for an id
        */
        DoseTypeGy getDoseAt(const VoxelGridID aID) const;

        /*! @brief returns the dose for an index
        */
        DoseTypeGy getDoseAt(const VoxelGridIndex3D& aIndex) const;

        const IDType getDoseUID() const
        {
          return _doseUID;
        };

      };
    }
  }
}

#endif
