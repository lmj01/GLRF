cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (GLRF "App.cpp")

# TODO: Add tests and install targets if needed.
target_include_directories(GLRF PRIVATE "${PROJECT_SOURCE_DIR}/include")