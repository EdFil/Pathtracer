project(stb)
cmake_minimum_required(VERSION 3.0)

message("")
message("==== STB ====")

# Make library
add_library(stb INTERFACE)
add_library(stb::stb ALIAS stb)
target_sources(stb INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/stb_image.h")

# Include directotories
target_include_directories(stb SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Show files
message(STATUS "stb_image.h: v2.26")




