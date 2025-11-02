cmake_minimum_required(VERSION 3.15)

project(GoldenAudioJUCE VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
