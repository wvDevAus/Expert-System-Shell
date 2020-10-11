# CMake configuration
cmake_minimum_required(VERSION 3.15)

# Variable declarations
set(ESS_PROJECT_NAME
        ExpertSystemShell)
set(ESS_TARGET_NAME
        ApplicationESS)
set(ESS_EXECUTABLE_NAME
        ess)
set(SANITIZER_OS
        "Darwin,Linux")
set(SANITIZER_FLAGS
        "-fsanitize=address,undefined,leak")

# Project declaration
project(${ESS_PROJECT_NAME}
    DESCRIPTION
        "A graphical expert system shell for use on modern systems."
    LANGUAGES
        CXX)

# Set build type to debug by default.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Project build type configuration
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo;MinSizeRel" CACHE STRING "Configs" FORCE)
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS  ${CMAKE_CONFIGURATION_TYPES})
set(${PROJ_NAME}_PATH_INSTALL ${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_INSTALL_PREFIX "")

# Enable CMake's Qt integrations
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# ExpertSystemShell app declaration
add_executable(${ESS_TARGET_NAME})

# Subdirectories for the Cmake tree
add_subdirectory(src)
add_subdirectory(lib)

# Get the Qt5 Package
set(Qt5_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/qt5-build/qtbase/lib/cmake/Qt5)
find_package(Qt5
    COMPONENTS
        Core Gui Widgets REQUIRED)

# App source code directory
target_include_directories(${ESS_TARGET_NAME}
    PRIVATE
        src/)

# App minimum compiler requirements
target_compile_features(${ESS_TARGET_NAME}
    PRIVATE
        cxx_std_17)

# App compilation settings
target_compile_options(${ESS_TARGET_NAME}
    PRIVATE
        # Clang
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:
            -Weverything -fcolor-diagnostics
            -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded
            -Wno-deprecated-declarations -Wno-exit-time-destructors
            -Wno-switch-enum -Wno-weak-vtables -Wno-global-constructors>
        # GCC
        $<$<CXX_COMPILER_ID:GNU>:
            -Wall -Wextra -Wpedantic -fdiagnostics-color=always>
        # Visual Studio
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
        # Enable the clang sanitizer.
        $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:Clang>,$<PLATFORM_ID:${SANITIZER_OS}>>:${SANITIZER_FLAGS}>)

# App pre-processor settings
#target_compile_definitions()

# App linking settings
target_link_options(${ESS_TARGET_NAME}
    PRIVATE
        $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:Clang>,$<PLATFORM_ID:${SANITIZER_OS}>>:${SANITIZER_FLAGS}>)

# App linking for libraries
target_link_libraries(${ESS_TARGET_NAME}
    PRIVATE
        nlohmann_json::nlohmann_json
        Qt5::Core
        Qt5::Widgets)

# App output directories
set_target_properties(${ESS_TARGET_NAME}
    PROPERTIES
        CXX_STANDARD
            17
        CXX_STANDARD_REQUIRED
            ON
        CXX_EXTENSIONS
            OFF
        OUTPUT_NAME
            ${ESS_EXECUTABLE_NAME}
        RUNTIME_OUTPUT_DIRECTORY
            ${CMAKE_CURRENT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}
        ARCHIVE_OUTPUT_DIRECTORY
            ${CMAKE_CURRENT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/lib
        LIBRARY_OUTPUT_DIRECTORY
            ${CMAKE_CURRENT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/lib)

# Copy the required dll's for the output executable
add_custom_command(TARGET ${ESS_TARGET_NAME}
    POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E
            copy_if_different
                $<TARGET_FILE:Qt5::Core>
                $<TARGET_FILE_DIR:${ESS_TARGET_NAME}>
        COMMAND ${CMAKE_COMMAND} -E
            copy_if_different
                $<TARGET_FILE:Qt5::Gui>
                $<TARGET_FILE_DIR:${ESS_TARGET_NAME}>
        COMMAND ${CMAKE_COMMAND} -E
            copy_if_different
                $<TARGET_FILE:Qt5::Widgets>
                $<TARGET_FILE_DIR:${ESS_TARGET_NAME}>
        COMMAND ${CMAKE_COMMAND} -E
            copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/res/runtime
                $<TARGET_FILE_DIR:${ESS_TARGET_NAME}>/res)

# Don't forget to copy the platform plugin applicable to the build type
if(WIN32)
    # Windows
    add_custom_command(TARGET ${ESS_TARGET_NAME}
        POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E
                copy
                    ${CMAKE_CURRENT_SOURCE_DIR}/lib/qt5-build/qtbase/plugins/platforms/qwindows$<$<CONFIG:Debug>:d>.dll
                    $<TARGET_FILE_DIR:${ESS_TARGET_NAME}>/platforms/qwindows$<$<CONFIG:Debug>:d>.dll)
else()
    # Other platforms
endif()
