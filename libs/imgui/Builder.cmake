# Source builds disabled on Linux because of a bug
# if(${USE_PREBUILT_LIBRARIES} OR UNIX AND NOT APPLE)
#   message(STATUS "=== IMGUI (Prebuilt) ===")
#   include("${CMAKE_CURRENT_LIST_DIR}/prebuilt/Prebuilt.cmake")
# else()
  message(STATUS "=== IMGUI (Source) ===")
  add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/source")
# endif()

message(STATUS "")
