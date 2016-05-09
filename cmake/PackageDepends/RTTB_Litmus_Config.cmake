#-----------------------------------------------------------------------------
# Litmus is built automatically. Just set include dir and link directories
#-----------------------------------------------------------------------------

FIND_PACKAGE(Litmus)

IF ((BUILD_RTToolbox_io_Tester AND BUILD_IO_ITK) OR (BUILD_RTToolbox_interpolation_Tester AND BUILD_InterpolationMatchPointTransformation))
	SET(Litmus_ITK "LitmusITK")
ENDIF()

LIST(APPEND ALL_INCLUDE_DIRECTORIES ${Litmus_INCLUDE_DIRS})
LIST(APPEND ALL_LIBRARIES LitmusCommon ${Litmus_ITK})

LINK_DIRECTORIES(${Litmus_LIBRARY_DIRS}) 
