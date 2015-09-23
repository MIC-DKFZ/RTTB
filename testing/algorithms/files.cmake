SET(CPP_FILES 
	DoseStatisticsTest.cpp
	DoseStatisticsCalculatorTest.cpp
	ArithmeticTest.cpp
	BinaryFunctorAccessorTest.cpp
	rttbAlgorithmsTests.cpp
	
	#include dummy accessor files
	../core/DummyDoseAccessor.cpp
	../core/DummyMaskAccessor.cpp
	../core/DummyMutableDoseAccessor.cpp
	
  )

SET(H_FILES 
	#include dummy accessor files
	../core/DummyDoseAccessor.h
	../core/DummyMaskAccessor.h
	../core/DummyMutableDoseAccessor.h
)
