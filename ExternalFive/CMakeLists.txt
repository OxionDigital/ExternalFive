cmake_minimum_required(VERSION 3.20)
project(mtacheat CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_subdirectory(third_party/minhook)

set(IMGUI third_party/imgui)
add_library(imgui STATIC
    ${IMGUI}/imgui.cpp ${IMGUI}/imgui_draw.cpp ${IMGUI}/imgui_tables.cpp
    ${IMGUI}/imgui_widgets.cpp ${IMGUI}/backends/imgui_impl_win32.cpp
    ${IMGUI}/backends/imgui_impl_dx9.cpp)
target_include_directories(imgui PUBLIC ${IMGUI} ${IMGUI}/backends)

add_library(mtacheat SHARED
    src/dllmain.cpp
    src/Cheats.cpp
    src/Overlay.cpp
    src/ui/Menu.cpp
    src/ui/Preview.cpp
    src/features/Visuals.cpp
    src/features/Aimbot.cpp
    src/features/LuaExec.cpp)

target_include_directories(mtacheat PRIVATE src)
target_link_libraries(mtacheat PRIVATE imgui minhook d3d9 winmm)
target_compile_definitions(mtacheat PRIVATE _CRT_SECURE_NO_WARNINGS NOMINMAX)
set_target_properties(mtacheat PROPERTIES PREFIX "" SUFFIX ".dll")