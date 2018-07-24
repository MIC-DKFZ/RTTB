// -----------------------------------------------------------------------
// RTToolbox - DKFZ radiotherapy quantitative evaluation library
//
// Copyright (c) German Cancer Research Center (DKFZ),
// Software development for Integrated Diagnostics and Therapy (SIDT).
// ALL RIGHTS RESERVED.
// See rttbCopyright.txt or
// http://www.dkfz.de/en/sidt/projects/rttb/copyright.html [^]
//
// This software is distributed WITHOUT ANY WARRANTY; without even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE. See the above copyright notices for more information.
//
//------------------------------------------------------------------------

#ifndef __REGISTRATION_TEST_H
#define __REGISTRATION_TEST_H

#include "mapRegistration.h"

namespace map
{
	namespace core
	{
		/*! @class TestRegistration
		@brief Simple implementation of MatchPoint Registration class with direct access to mapping.
		*/
		template<unsigned int VMovingDimensions, unsigned int VTargetDimensions>
		class RegistrationTest: public
			Registration<VMovingDimensions, VTargetDimensions>
		{
		public:
			typedef RegistrationTest<VMovingDimensions, VTargetDimensions> Self;
			typedef RegistrationBase Superclass;
			typedef itk::SmartPointer<Self> Pointer;
			typedef itk::SmartPointer<const Self> ConstPointer;

			typedef typename Registration<VMovingDimensions, VTargetDimensions>::TargetPointType
			TargetPointType;
			typedef typename Registration<VMovingDimensions, VTargetDimensions>::MovingPointType
			MovingPointType;

			itkTypeMacro(RegistrationTest, Registration);
			itkNewMacro(Self);

			bool _limitedTarget;
			double* _translation;

			RegistrationTest() {};

			~RegistrationTest()
			{
			};


			virtual bool mapPointInverse(const TargetPointType& inPoint, MovingPointType& outPoint) const
			{
				for (unsigned int i = 0; i < VTargetDimensions; i++)
				{
					outPoint[i] = inPoint[i] + _translation[i];
				}

				return true;
			};

			virtual bool hasLimitedTargetRepresentation() const
			{
				return _limitedTarget;
			}


		private:
			RegistrationTest(const Self& source);  //purposely not implemented
			void operator=(const Self&);  //purposely not implemented
		};
	}
}

#endif
