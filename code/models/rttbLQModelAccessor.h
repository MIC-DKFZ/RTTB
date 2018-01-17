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
#ifndef __LQ_MODEL_ACCESSOR_H
#define __LQ_MODEL_ACCESSOR_H

#include "rttbAccessorWithGeoInfoBase.h"
#include "rttbDoseAccessorInterface.h"
#include "rttbBaseTypeModels.h"

namespace rttb
{
	namespace models
	{
		/*! @class LQModelAccessor
		@brief This class gives access to the LQ Model information in an image
		*/
		class LQModelAccessor: public core::AccessorWithGeoInfoBase
		{
		public:
			using DoseAccessorPointer = core::DoseAccessorInterface::DoseAccessorPointer;
		private:
			DoseAccessorPointer _dose;
			BioModelParamType _alpha;
      BioModelParamType _beta;
      unsigned int _nFractions;
      DoseAccessorPointer _alphaMap;
      DoseAccessorPointer _betaMap;
      double _doseScaling;
      bool _withAlphaBetaMaps;

			IDType _bioModelUID;

			LQModelAccessor();

			/*! @brief get all required data from the dose geometric info
			*/
			bool assembleGeometricInfo();


		public:
			~LQModelAccessor() override;

			/*! @brief Constructor.
			@pre dose must be a valid instance (and != nullptr)
			@exception InvalidDoseException if _dose is nullptr
			*/
			LQModelAccessor(DoseAccessorPointer dose, BioModelParamType alpha, BioModelParamType beta, unsigned int nFractions=1,
			                double doseScaling = 1.0);

            /*! @brief Constructor.
            @pre dose must be a valid instance (and != nullptr)
            @exception InvalidDoseException if dose is nullptr, if alphaMap is nullptr or if betaMap is nullptr
            */
            LQModelAccessor(DoseAccessorPointer dose, DoseAccessorPointer alphaMap, DoseAccessorPointer betaMap, unsigned int nFractions = 1,
                double doseScaling = 1.0);

			/*! @brief returns the LQ Model value for an id
			*/
			GenericValueType getValueAt(const VoxelGridID aID) const override;

			/*! @brief returns the LQ Model value for an index
			*/
			GenericValueType getValueAt(const VoxelGridIndex3D& aIndex) const override;

			const IDType getUID() const override
			{
				return _bioModelUID;
			};

		};
	}
}

#endif
