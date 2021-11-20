project(fast_obj)
cmake_minimum_required(VERSION 3.0)

message("")
message("==== FAST OBJ ====")

# Make library
add_library(${PROJECT_NAME} INTERFACE)
target_sources(${PROJECT_NAME} INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/fast_obj.h")

# Include directories
target_include_directories(${PROJECT_NAME} SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Show Info
message(STATUS "Version: 1.1")
message(STATUS "Includes: ${INCLUDE_DIR}")




