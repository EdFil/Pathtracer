project(rapidjson)
cmake_minimum_required(VERSION 3.0)

message("")
message("==== Rapidjson ====")

# Glob sources
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

# Make library
add_library(rapidjson INTERFACE)
add_library(rapidjson::rapidjson ALIAS rapidjson)
target_sources(rapidjson INTERFACE ${SOURCES})

# Include directotories
target_include_directories(rapidjson SYSTEM INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Show files
message(STATUS "rapidjson: v1.10")




