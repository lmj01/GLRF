cmake_minimum_required (VERSION 3.8)

project("glrf" VERSION 1.0 LANGUAGES C CXX)

find_package(Git)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
	message(STATUS "Git found!")
	option(GIT_SUBMODULE "Check submodules during build" ON)
	if (GIT_SUBMODULE)
		message(STATUS "Updating submodules..")
		execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} RESULT_VARIABLE GIT_SUBMODULE_RESULT)
		if(NOT GIT_SUBMODULE_RESULT EQUAL "0")
			message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
		endif()
	endif()
else()
	message(FATAL_ERROR "Package 'Git' could not be found!")
endif()

find_package(Python)
if (Python_FOUND)
	message(STATUS "Python found!")
	# TODO - add python script execution to generate files for 'glad'
	execute_process(COMMAND pip install glad)
	execute_process(COMMAND glad --profile core --out-path "${CMAKE_CURRENT_SOURCE_DIR}/lib/glad" --generator c --no-loader)
else()
	message(FATAL_ERROR "Package 'Python' could not be found!")
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/external/glad/CMakeLists.txt")
    message(FATAL_ERROR "Submodule 'glad' was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/external/glfw/CMakeLists.txt")
    message(FATAL_ERROR "Submodule 'GLFW' was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/external/glm/CMakeLists.txt")
    message(FATAL_ERROR "Submodule 'glm' was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/external/stb/stb_image.h")
    message(FATAL_ERROR "Submodule 'stb' was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()


message(STATUS "All submodules found and updated!")

# Load libraries
add_subdirectory("${PROJECT_SOURCE_DIR}/external/glad")
set(GLAD_SOURCES "${PROJECT_SOURCE_DIR}/lib/glad/src")
set(GLAD_HEADERS "${PROJECT_SOURCE_DIR}/lib/glad/include")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("${PROJECT_SOURCE_DIR}/external/glfw")

add_subdirectory("${PROJECT_SOURCE_DIR}/external/glm")

# Include sub-projects.
add_library(${PROJECT_NAME} SHARED "Library.cpp")

set_target_properties(${PROJECT_NAME} PROPERTIES
	VERSION ${PROJECT_VERSION}
)

target_include_directories(${PROJECT_NAME}
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/include
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME} glad)
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} glm)