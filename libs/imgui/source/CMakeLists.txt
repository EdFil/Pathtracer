cmake_minimum_required(VERSION 3.0)
project(imgui VERSION 1.73.0 LANGUAGES CXX C)



add_library (imgui STATIC
    imgui.cpp
    imgui_draw.cpp
    imgui_widgets.cpp
    imgui_tables.cpp
    backends/imgui_impl_sdl.cpp
    backends/imgui_impl_opengl3.cpp
)

add_library(imgui::imgui ALIAS imgui)

message(STATUS "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories (imgui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_compile_features(imgui PRIVATE cxx_std_98)
target_compile_options(imgui PRIVATE -DIMGUI_IMPL_OPENGL_LOADER_GLAD)

target_link_libraries(imgui PRIVATE SDL2::Core glad)