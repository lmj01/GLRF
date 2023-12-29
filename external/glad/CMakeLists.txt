project(gladZip LANGUAGES C)

file ( GLOB all_files "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c" )
set(GLAD_INCLUDE_DIR include)
set(GLAD_SOURCE_DIR ${all_files})

include_directories ( ${GLAD_INCLUDE_DIR} )

# Build as STATIC library
add_library(glad 
	${GLAD_SOURCE_DIR}
)
