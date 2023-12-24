cmake_minimum_required (VERSION 3.8)

# Set variables PROJECT_NAME and _VERSION
project("glrf" VERSION 1.0 LANGUAGES C CXX)

# ==== settings ====

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Enable linking of this library on windows
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1)

# Setting of directory variables for convenience
set(SUBMODULE_DIR "${PROJECT_SOURCE_DIR}/external")
set(GLAD_DIR "${PROJECT_SOURCE_DIR}/generated/glad")
set(GLRF_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(GLRF_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")
# set(GLRF_TEST_DIR "${PROJECT_SOURCE_DIR}/test")

# # Path to glad directory
# # set(GLAD_SOURCES_DIR "${SUBMODULE_DIR}/glad/")
set(GLAD_SOURCES_DIR "${PROJECT_SOURCE_DIR}/external/glad")
# # Path to glad cmake files
add_subdirectory("${GLAD_SOURCES_DIR}/cmake" glad_cmake)

# Check if the project is built by itself and not as a subdirectory
set(IS_STANDALONE "${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR}")
if (IS_STANDALONE)
	message(STATUS "IS_STANDALONE = TRUE")
else()
	message(STATUS "IS_STANDALONE = FALSE")
endif()

# ==== integrity checks ====

# Try to find Git
find_package(Git)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
	message(STATUS "Git found!")
	option(GIT_SUBMODULE "Check submodules during build" ON)
	# Update submodules
	if (GIT_SUBMODULE)
		message(STATUS "Updating submodules..")
		execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
			WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} RESULT_VARIABLE GIT_SUBMODULE_RESULT)
		if(NOT GIT_SUBMODULE_RESULT EQUAL "0")
			message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
		endif()
	endif()
else()
	message(FATAL_ERROR "Package 'Git' could not be found!")
endif()

# Check if all dependencies exist that the library needs to link to
if(NOT EXISTS "${SUBMODULE_DIR}")
	message(FATAL_ERROR "No submodules were downloaded! Please update submodules and try again.")
endif()
# if(NOT EXISTS "${SUBMODULE_DIR}/glad/CMakeLists.txt")
#     message(FATAL_ERROR "Submodule 'glad' was not downloaded! Please update submodules and try again.")
# endif()
if(NOT EXISTS "${SUBMODULE_DIR}/glfw/CMakeLists.txt")
    message(FATAL_ERROR "Submodule 'GLFW' was not downloaded! Please update submodules and try again.")
endif()
if(NOT EXISTS "${SUBMODULE_DIR}/glm/CMakeLists.txt")
    message(FATAL_ERROR "Submodule 'glm' was not downloaded! Please update submodules and try again.")
endif()
if(NOT EXISTS "${SUBMODULE_DIR}/stb/stb/stb_image.h")
    message(FATAL_ERROR "Submodule 'stb' was not downloaded! Please update submodules and try again.")
endif()
message(STATUS "All submodules needed to build the project found and updated!")

# Check if files from glad have not been created yet.
# If they were created previously, skip this step.
if(NOT EXISTS "${GLAD_DIR}/include/glad/glad.h")
	message(STATUS "Generating files with 'glad'..")
	# set(GLAD_SOURCES_DIR "${PROJECT_SOURCE_DIR}/external/glad/")
	# add_subdirectory("${GLAD_SOURCES_DIR}/cmake" glad_cmake)
	glad_add_library(glad_gl_core LOCATION "${GLAD_DIR}" API gl:core=3.3)
else()
	message(STATUS "Previous files of 'glad' are still available. Skipping recreation..")
endif()

# # Specify glad settings
# # glad_add_library(glad_gl_core LOCATION "${GLAD_DIR}" API gl:core=3.3)
# glad_add_library(glad_gl_core API gl:core=3.3)

# ==== load libraries / dependencies ====
# -- glad --
# add_subdirectory("${SUBMODULE_DIR}/glad/")


# Set variables GLAD_SOURCES and GLAD_HEADERS, containing all generated files from glad.
# [recommendation] --> Include the two variables in your target, in order to link glad correctly.
# if(NOT IS_STANDALONE)
# 	set(GLAD_SOURCES "${GLAD_DIR}/src" PARENT_SCOPE)
# 	set(GLAD_HEADERS "${GLAD_DIR}/include" PARENT_SCOPE)
# 	message(STATUS "glad info0" "${GLAD_SOURCES}" "${GLAD_HEADERS}")
# else ()
# 	set(GLAD_SOURCES "${GLAD_DIR}/src")
# 	set(GLAD_HEADERS "${GLAD_DIR}/include")
# endif()

# -- GLFW --
# Disable targets that are not needed
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("${SUBMODULE_DIR}/glfw")

# -- glm --
add_subdirectory("${SUBMODULE_DIR}/glm")

# ==== build project ====
# Get source files directly from directories without linking individual files
aux_source_directory(${GLRF_INCLUDE_DIR} GLRF_HEADERS)
aux_source_directory(${GLRF_SOURCE_DIR} GLRF_SOURCES)

# Build as STATIC library
add_library(${PROJECT_NAME} STATIC
	${GLRF_SOURCES}
)

target_include_directories(${PROJECT_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include/GLRF>
)

# Include the non-CMake submodule 'stb' (headers only).
# Note that there is another 'stb' folder inside the specified directory.
# This allows to access stb-internal files with #include <stb/?>,
# where ? is replaced with the filename.
target_include_directories(${PROJECT_NAME}
	PRIVATE
		${SUBMODULE_DIR}/stb
)

# Link library to all dependencies
# target_link_libraries(${PROJECT_NAME} $<BUILD_INTERFACE:glad>)
target_link_libraries(${PROJECT_NAME} $<BUILD_INTERFACE:glfw>)
target_link_libraries(${PROJECT_NAME} $<BUILD_INTERFACE:glm>)

# if(IS_STANDALONE)
# 	include(CTest)
# 	if(BUILD_TESTING)
# 		# ==== integrity checks ====
# 		if(NOT EXISTS "${SUBMODULE_DIR}/googletest/CMakeLists.txt")
# 			message(FATAL_ERROR "Submodule 'googletest' was not downloaded! Please update submodules and try again.")
# 		endif()
# 		message(STATUS "All submodules needed to test the project found and updated!")

# 		# ==== load libraries / dependencies ====
# 		add_subdirectory("${SUBMODULE_DIR}/googletest")

# 		# ==== test project ====
# 		message(STATUS "Setting up tests..")
# 		add_subdirectory(${GLRF_TEST_DIR})
# 	endif()
# endif()