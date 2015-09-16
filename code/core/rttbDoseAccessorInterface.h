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
#ifndef __DOSE_ACCESSOR_INTERFACE_NEW_H
#define __DOSE_ACCESSOR_INTERFACE_NEW_H

#include <boost/smart_ptr/shared_ptr.hpp>

#include "rttbBaseType.h"
#include "rttbGeometricInfo.h"
#include "rttbIndexConversionInterface.h"

namespace rttb
{
  namespace core
  {

    /*! @class IndexConversionInterface
    	@brief This class represents the conversion of 3D grid indices to 1D grid IDs.
    */
    class DoseAccessorInterface: public IndexConversionInterface
    {
    public:
      typedef boost::shared_ptr<DoseAccessorInterface> DoseAccessorPointer;
    private:
      DoseAccessorInterface(const DoseAccessorInterface&); //not implemented on purpose -> non-copyable
      DoseAccessorInterface& operator=(const
                                       DoseAccessorInterface&);//not implemented on purpose -> non-copyable

    public:
      DoseAccessorInterface() {};
      virtual ~DoseAccessorInterface() {};

      /*! @brief test if given ID is inside current dose grid
      */
      bool validID(const VoxelGridID aID) const
      {
        return this->getGeometricInfo().validID(aID);
      };


      /*! @brief test if given index is inside current dose grid
      */
      bool validIndex(const VoxelGridIndex3D& aIndex) const
      {
        return this->getGeometricInfo().validIndex(aIndex);
      };

      virtual const core::GeometricInfo& getGeometricInfo() const = 0;

      virtual GridSizeType getGridSize() const
      {
        return this->getGeometricInfo().getNumberOfVoxels();
      };

      virtual DoseTypeGy getDoseAt(const VoxelGridID aID) const = 0;

      virtual DoseTypeGy getDoseAt(const VoxelGridIndex3D& aIndex) const = 0;

      /*! @brief is true if dose is on a homogeneous grid
      	@remarks Inhomogeneous grids are not supported at the moment, but if they will be supported in the future
      	the interface does not need to change.
      */
      virtual bool isGridHomogeneous() const
      {
        return true;
      }

      virtual const IDType getDoseUID() const = 0;

    };
  }
}

#endif
