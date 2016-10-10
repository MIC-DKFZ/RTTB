add_library(${PROJECT_NAME} STATIC parseXML.cpp parseXML.h XMLGenerator.cpp XMLGenerator.h CmdLineParserBase.cpp CmdLineParserBase.h)

# MSVC automatically links to boost
if(NOT MSVC)
	target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
endif()

# Enable C++11 support in Linux
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)