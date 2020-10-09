# CMake configuration
cmake_minimum_required(VERSION 3.15)

# Variable declarations
set(PROJECT_NAME
        ExpertSystemShell)
set(TARGET_NAME
        ApplicationESS)
set(EXECUTABLE_NAME
        ess)

# Project declaration
project(${PROJECT_NAME}
    DESCRIPTION
        "A graphical expert system shell for use on modern systems."
    LANGUAGES
        CXX)

# ExpertSystemShell app declaration
add_executable(${TARGET_NAME})

# Subdirectories for Cmake
add_subdirectory(src)

# App include directories
#target_include_directories()

# App minimum compiler requirements
target_compile_features(${TARGET_NAME}
    PRIVATE
        cxx_std_17)

# App compilation settings
target_compile_options(${TARGET_NAME}
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
#target_link_options()

# App output directories
set_target_properties(${TARGET_NAME}
    PROPERTIES
        CXX_STANDARD
            17
        CXX_STANDARD_REQUIRED
            ON
        CXX_EXTENSIONS
            OFF
        OUTPUT_NAME
            "${EXECUTABLE_NAME}"
        RUNTIME_OUTPUT_DIRECTORY
            "${CMAKE_BINARY_DIR}"
        ARCHIVE_OUTPUT_DIRECTORY
            "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY
            "${CMAKE_BINARY_DIR}/lib")
