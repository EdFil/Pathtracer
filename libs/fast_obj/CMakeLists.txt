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
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/fast_obj.h")
  file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/fast_obj.h" VERSION_LINE REGEX "(.*)Version [0-9.]*")
  string(REGEX REPLACE "^(.*)Version ([0-9.]*)(.*)" "\\2" VERSION "${VERSION_LINE}")
  message(STATUS "VERSION: ${VERSION}")
endif()
message(STATUS "Includes: ${INCLUDE_DIR}")




