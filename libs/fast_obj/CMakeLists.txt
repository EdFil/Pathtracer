project(fast_obj)
cmake_minimum_required(VERSION 3.0)

message("")
message("==== FAST OBJ ====")

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/fast_obj.h")

# Make library
add_library(${PROJECT_NAME} INTERFACE)
target_sources(${PROJECT_NAME} INTERFACE PUBLIC ${HEADERS})

# Include directories
target_include_directories(${PROJECT_NAME} SYSTEM INTERFACE ${INCLUDE_DIR})
#target_compile_definitions(fast_obj INTERFACE -DFAST_OBJ_IMPLEMENTATION)

# Show Info
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/fast_obj.h")
  file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/fast_obj.h" VERSION_LINE REGEX "(.*)Version [0-9.]*")
  string(REGEX REPLACE "^(.*)Version ([0-9.]*)(.*)" "\\2" VERSION "${VERSION_LINE}")
  message(STATUS "VERSION: ${VERSION}")
endif()
message(STATUS "Includes: ${INCLUDE_DIR}")




