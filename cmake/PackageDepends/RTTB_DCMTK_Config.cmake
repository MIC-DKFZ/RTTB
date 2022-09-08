if(NOT WIN32 AND NOT APPLE)
  set(MISSING_LIBS_REQUIRED_BY_DCMTK tiff z)
endif()

foreach(dcmtk_component ${DCMTK_REQUIRED_COMPONENTS_BY_MODULE})
  set(dcmtk_component "DCMTK::${dcmtk_component}")
  list(APPEND _dcmtk_required_components_by_module ${dcmtk_component})
endforeach()

find_package(DCMTK COMPONENTS ${_dcmtk_required_components_by_module} REQUIRED)

foreach(dcmtk_component ${_dcmtk_required_components_by_module})
  list(APPEND ALL_LIBRARIES ${dcmtk_component})
endforeach()


list(APPEND ALL_INCLUDE_DIRECTORIES ${DCMTK_INCLUDE_DIRS})
list(APPEND ALL_LIBRARIES ${MISSING_LIBS_REQUIRED_BY_DCMTK})