FUNCTION(ORGANIZE_SOURCES)

  # this functions gets a filelist as input and looks
  # for corresponding h-files to add them to the project.
  
  # additionally files are grouped in source-groups.
  
  # No parameters explicitly declared here, because
  # we want to allow for variable argument lists, which
  # are later access by the keyword FOREACH(MYFILE ${ARGV})
  
  # output: after calling the macro, files that were found
  # correspondigly to the given files are stored in the
  # variable:
  # ${CORRESPONDING_H_FILES}
  # ${CORRESPONDING_TXX_FILES}
  
  # Globbed can be found in the variables
  # ${GLOBBED_TXX_FILES} (CURRENTLY COMMENTED OUT)
  # ${GLOBBED_DOX_FILES}

  cmake_parse_arguments(_ORG "" "" "HEADER;SOURCE;TXX;DOC" ${ARGN})

  SET(CORRESPONDING__H_FILES "" )
  SET(GLOBBED__H_FILES "" )

  IF(_ORG_HEADER)
    FOREACH(_file ${_ORG_SOURCE})
      STRING(REGEX REPLACE "(.*)\\.(txx|tpp|cpp|c|cxx)$" "\\1.h" H_FILE ${_file})
      IF(EXISTS ${H_FILE})
        LIST(APPEND CORRESPONDING__H_FILES "${H_FILE}")
      ENDIF()
    ENDFOREACH()
  ELSE()
    FILE(GLOB_RECURSE GLOBBED__H_FILES *.h)
  ENDIF()

  set(CORRESPONDING__H_FILES ${CORRESPONDING__H_FILES} PARENT_SCOPE)
  set(GLOBBED__H_FILES ${GLOBBED__H_FILES} PARENT_SCOPE)

  #_MACRO_APPEND_TO_LIST(_ORG_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/")
  SOURCE_GROUP("Source Files" FILES ${_ORG_SOURCE})
  
  #_MACRO_APPEND_TO_LIST(_ORG_TXX "${CMAKE_CURRENT_SOURCE_DIR}/")
  SOURCE_GROUP("Template Files" FILES ${_ORG_TXX})
 
  #_MACRO_APPEND_TO_LIST(_ORG_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/")
  SOURCE_GROUP("Header Files" FILES ${_ORG_HEADER} ${CORRESPONDING__H_FILES} ${GLOBBED__H_FILES})
   
  #_MACRO_APPEND_TO_LIST(_ORG_DOC "${CMAKE_CURRENT_SOURCE_DIR}/")
  SOURCE_GROUP("Doxygen Files" FILES ${_ORG_DOC})
    
ENDFUNCTION(ORGANIZE_SOURCES)
