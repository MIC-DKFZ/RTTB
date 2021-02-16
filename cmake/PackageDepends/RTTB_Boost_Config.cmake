IF(NOT DEFINED RTTB_USE_MITK_BOOST)
  OPTION(RTTB_USE_MITK_BOOST "RTTB should use a boost which is available in the MITK superbuild external projects structure." OFF)
  MARK_AS_ADVANCED(RTTB_USE_MITK_BOOST)
ENDIF(NOT DEFINED RTTB_USE_MITK_BOOST)

IF(BUILD_SHARED_LIBS OR RTTB_USE_MITK_BOOST)
  SET(Boost_USE_STATIC_LIBS OFF)
  ADD_DEFINITIONS(-DBOOST_ALL_DYN_LINK)
ELSE(BUILD_SHARED_LIBS OR RTTB_USE_MITK_BOOST)
  SET(Boost_USE_STATIC_LIBS ON)
ENDIF(BUILD_SHARED_LIBS OR RTTB_USE_MITK_BOOST)

SET(BOOST_MIN_VERSION "1.64.0")

SET(RTTB_Boost_REQUIRED_COMPONENTS_BY_MODULE ${Boost_REQUIRED_COMPONENTS_BY_MODULE} ${RTTB_Boost_ADDITIONAL_COMPONENT})

FIND_PACKAGE(Boost ${BOOST_MIN_VERSION} REQUIRED COMPONENTS ${RTTB_Boost_REQUIRED_COMPONENTS_BY_MODULE} QUIET)

if(Boost_REQUIRED_COMPONENTS_BY_MODULE)
  foreach(boost_component ${Boost_REQUIRED_COMPONENTS_BY_MODULE})
    list(APPEND ALL_LIBRARIES "Boost::${boost_component}")
  endforeach()
endif()

IF(BUILD_SHARED_LIBS OR RTTB_USE_MITK_BOOST)
  IF(MSVC)
     list(APPEND ALL_LIBRARIES "Boost::dynamic_linking" "bcrypt")
  ENDIF(MSVC)
ENDIF(BUILD_SHARED_LIBS OR RTTB_USE_MITK_BOOST)
