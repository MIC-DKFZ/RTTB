SET(CPP_FILES 
	DoseStatisticsTest.cpp
	DoseStatisticsCalculatorTest.cpp
	ArithmeticTest.cpp
	BinaryFunctorAccessorTest.cpp
	rttbAlgorithmsTests.cpp
	../io/other/CompareDoseStatistic.cpp
	../../code/io/other/rttbDoseStatisticsXMLReader.cpp
	
	#include dummy accessor files
	../core/DummyDoseAccessor.cpp
	../core/DummyMaskAccessor.cpp
	../core/DummyMutableDoseAccessor.cpp
	
  )

SET(H_FILES 
	../io/other/CompareDoseStatistic.h
	../../code/io/other/rttbDoseStatisticsXMLReader.h
	
	#include dummy accessor files
	../core/DummyDoseAccessor.h
	../core/DummyMaskAccessor.h
	../core/DummyMutableDoseAccessor.h
)
