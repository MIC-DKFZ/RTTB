#ifndef __TESTREGISTRATION_H
#define __TESTREGISTRATION_H

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

			itkTypeMacro(RegistrationTest, Registration);
			itkNewMacro(Self);

			bool _limitedTarget;
			double* translation;

			RegistrationTest() {};

			~RegistrationTest()
			{
			};


			virtual bool mapPointInverse(const TargetPointType& inPoint, MovingPointType& outPoint) const
			{
				for (unsigned int i = 0; i < VTargetDimensions; i++)
				{
					outPoint[i] = inPoint[i] + translation[i];
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