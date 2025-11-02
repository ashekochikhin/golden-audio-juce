cmake_minimum_required(VERSION 3.15)

project(GoldenAudioJUCE VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Option to build examples
option(GOLDEN_AUDIO_BUILD_EXAMPLES "Build example projects" ON)

# Header-only library
add_library(golden_audio_juce INTERFACE)

target_include_directories(golden_audio_juce INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# JUCE dependency - users must provide JUCE
# This library requires juce_audio_processors and juce_audio_formats modules
if(TARGET juce::juce_audio_processors)
    target_link_libraries(golden_audio_juce INTERFACE 
        juce::juce_audio_processors
        juce::juce_audio_formats
    )
else()
    message(STATUS "JUCE not found - golden_audio_juce requires JUCE to be added before this library")
    message(STATUS "Make sure to call add_subdirectory(JUCE) or find_package(JUCE) before adding golden_audio_juce")
endif()

# Installation
install(TARGETS golden_audio_juce
    EXPORT GoldenAudioJUCETargets
)

install(DIRECTORY include/
    DESTINATION include
)

install(EXPORT GoldenAudioJUCETargets
    FILE GoldenAudioJUCETargets.cmake
    NAMESPACE GoldenAudio::
    DESTINATION lib/cmake/GoldenAudioJUCE
)

# Create config file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/GoldenAudioJUCEConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_file(cmake/GoldenAudioJUCEConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/GoldenAudioJUCEConfig.cmake"
    @ONLY
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/GoldenAudioJUCEConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/GoldenAudioJUCEConfigVersion.cmake"
    DESTINATION lib/cmake/GoldenAudioJUCE
)

# Build examples if requested
if(GOLDEN_AUDIO_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
