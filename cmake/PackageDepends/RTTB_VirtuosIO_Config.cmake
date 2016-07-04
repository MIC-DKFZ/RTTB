#-----------------------------------------------------------------------------
# Find Virtuos IO
#-----------------------------------------------------------------------------
FIND_PACKAGE(VirtuosIOCore)
IF(VirtuosIOCore_FOUND)
  INCLUDE(${VirtuosIOCore_USE_FILE})
ELSE(VirtuosIOCore_FOUND)
  MESSAGE(FATAL_ERROR
          "Cannot build without VirtuosIOCore.  Please set VirtuosIOCore_DIR.")
ENDIF(VirtuosIOCore_FOUND)

LIST(APPEND ALL_INCLUDE_DIRECTORIES ${VirtuosIOCore_INCLUDE_DIRS})
LIST(APPEND ALL_LIBRARIES VirtuosIO)

LINK_DIRECTORIES(${VirtuosIOCore_LIBRARY_DIRS})

# include path to gzip/gunzip
