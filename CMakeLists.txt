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

# ExpertSystemShell app declaration
add_executable(${ESS_TARGET_NAME})

# Set build type to debug by default.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Subdirectories for the Cmake tree
add_subdirectory(src)
add_subdirectory(lib)

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
        nlohmann_json::nlohmann_json)

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
            "${ESS_EXECUTABLE_NAME}"
        RUNTIME_OUTPUT_DIRECTORY
            "${CMAKE_BINARY_DIR}"
        ARCHIVE_OUTPUT_DIRECTORY
            "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY
            "${CMAKE_BINARY_DIR}/lib")
