SET(CPP_FILES 
	DoseStatisticsTest.cpp
	ArithmeticTest.cpp
	BinaryFunctorDoseAccessorTest.cpp
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
