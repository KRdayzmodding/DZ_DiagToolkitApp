# Standard routine
cmake_minimum_required(VERSION 3.16)
project(example_app
        VERSION 0.0.12 # CI_PROJECT_VERSION
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(BUILD_SHARED_LIBS "Build shared libs" OFF)
option(AUIB_FORCE_PRECOMPILED "AUI.Boot: force precompiled dependencies" OFF)
option(AUIB_NO_PRECOMPILED "AUI.Boot: disable precompiled dependencies" OFF)

# set(AUI_VERSION develop)
set(AUI_VERSION 12554c2b58bf4b44fcc134e6402e361b9c0a0b41)

# Use AUI.Boot
file(
        DOWNLOAD
        https://raw.githubusercontent.com/aui-framework/aui/${AUI_VERSION}/aui.boot.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/aui.boot.cmake)
include(${CMAKE_CURRENT_BINARY_DIR}/aui.boot.cmake)

# import AUI
auib_import(aui https://github.com/aui-framework/aui
        COMPONENTS core views curl json crypt updater
        VERSION ${AUI_VERSION})


# Create the executable. This function automatically links all sources from the src/ folder,
# creates CMake target and places the resulting executable to bin/ folder.
aui_executable(${PROJECT_NAME})

# Link required libs
aui_link(${PROJECT_NAME} PRIVATE aui::core aui::views aui::updater)

# Compile assets
aui_compile_assets(${PROJECT_NAME})

# Enable testing
aui_enable_tests(${PROJECT_NAME})

# Setup icon, display name, etc
aui_app(TARGET ${PROJECT_NAME}
        NAME "AUI App Example"
        VENDOR "AUI Project"
        ICON "assets/img/icon.svg"
)

# Force C++23 for this target (must be after all aui_* functions)
set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
)

# Add C++23 compile option for clang-cl (using -Xclang to pass to clang frontend)
target_compile_options(${PROJECT_NAME} PRIVATE 
-Xclang -std=c++23 -Wno-deprecated-this-capture)