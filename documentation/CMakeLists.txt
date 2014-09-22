MESSAGE (STATUS "processing RTToolbox documentation")

CONFIGURE_FILE(${RTToolbox_SOURCE_DIR}/documentation/doxygen.config.in
               ${RTToolbox_BINARY_DIR}/documentation/doxygen.config)

