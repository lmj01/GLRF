cmake_minimum_required (VERSION 3.8)

project("glrf" VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1)

set(SUBMODULE_DIR "${PROJECT_SOURCE_DIR}/external")
set(GLAD_DIR "${PROJECT_SOURCE_DIR}/generated/glad")
set(GLRF_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(GLRF_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")
set(IS_STANDALONE "${CMAKE_SOURCE_DIR} STREQUAL ${PROJECT_SOURCE_DIR}")

set(ENABLE_EXPORTS TRUE)

find_package(Git)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
	message(STATUS "Git found!")
	option(GIT_SUBMODULE "Check submodules during build" ON)
	if (GIT_SUBMODULE)
		message(STATUS "Updating submodules..")
		execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} RESULT_VARIABLE GIT_SUBMODULE_RESULT)
		if(NOT GIT_SUBMODULE_RESULT EQUAL "0")
			message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
		endif()
	endif()
else()
	message(FATAL_ERROR "Package 'Git' could not be found!")
endif()

if(NOT EXISTS "${SUBMODULE_DIR}/glad/CMakeLists.txt")
    message(FATAL_ERROR "Submodule 'glad' was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()
if(NOT EXISTS "${SUBMODULE_DIR}/glfw/CMakeLists.txt")
    message(FATAL_ERROR "Submodule 'GLFW' was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()
if(NOT EXISTS "${SUBMODULE_DIR}/glm/CMakeLists.txt")
    message(FATAL_ERROR "Submodule 'glm' was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()
if(NOT EXISTS "${SUBMODULE_DIR}/stb/stb/stb_image.h")
    message(FATAL_ERROR "Submodule 'stb' was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

message(STATUS "All submodules found and updated!")

if(NOT EXISTS "${GLAD_DIR}/include/glad/glad.h")
	find_package(Python)
	if (Python_FOUND)
		message(STATUS "Python found!")
		message(STATUS "Generating files with 'glad'..")
		execute_process(COMMAND pip install glad)
		execute_process(COMMAND glad --profile core --out-path ${GLAD_DIR} --generator c --no-loader)
		message(STATUS "Generated files with 'glad' in '${GLAD_DIR}'.")
	else()
		message(FATAL_ERROR "Package 'Python' could not be found!")
	endif()
else()
	message(STATUS "Previous files of 'glad' are still available. Skipping recreation..")
endif()

# Load libraries
add_subdirectory("${SUBMODULE_DIR}/glad")
if(${IS_STANDALONE})
	set(GLAD_SOURCES "${GLAD_DIR}/src" PARENT_SCOPE)
	set(GLAD_HEADERS "${GLAD_DIR}/include" PARENT_SCOPE)
endif()

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("${SUBMODULE_DIR}/glfw")

add_subdirectory("${SUBMODULE_DIR}/glm")

# Include sub-projects.
aux_source_directory(${GLRF_INCLUDE_DIR} GLRF_HEADERS)
aux_source_directory(${GLRF_SOURCE_DIR} GLRF_SOURCES)

add_library(${PROJECT_NAME} STATIC
	${GLRF_SOURCES}
)

target_include_directories(${PROJECT_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include/GLRF>
)

target_include_directories(${PROJECT_NAME}
	PRIVATE
		${SUBMODULE_DIR}/stb
)

target_link_libraries(${PROJECT_NAME} $<BUILD_INTERFACE:glad>)
target_link_libraries(${PROJECT_NAME} $<BUILD_INTERFACE:glfw>)
target_link_libraries(${PROJECT_NAME} $<BUILD_INTERFACE:glm>)

#include(GenerateExportHeader)
#GENERATE_EXPORT_HEADER(${PROJECT_NAME}
#	BASE_NAME ${PROJECT_NAME}
#	EXPORT_MACRO_NAME ${PROJECT_NAME}_EXPORTS
#	EXPORT_FILE_NAME ${PROJECT_NAME}_EXPORTS.h
#	STATIC_DEFINE ${PROJECT_NAME}_EXPORTS_BUILT_AS_STATIC
#)

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(FILES ${GLRF_HEADERS} DESTINATION include)

#install(TARGETS ${PROJECT_NAME}
#	EXPORT ${PROJECT_NAME}Targets
#	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
#)
#
#install(EXPORT ${PROJECT_NAME}Targets
#	FILE ${PROJECT_NAME}Targets.cmake
#	NAMESPACE ${PROJECT_NAME}::
#	DESTINATION "cmake"
#)