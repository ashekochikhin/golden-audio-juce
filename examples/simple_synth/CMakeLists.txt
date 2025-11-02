cmake_minimum_required(VERSION 3.15)

project(SimpleSynthExample)

# This example requires JUCE to be available
# In a real project, you would add JUCE as a submodule or use FetchContent

# For this example, we assume JUCE is available via find_package or add_subdirectory
# find_package(JUCE CONFIG REQUIRED)
# OR
# add_subdirectory(path/to/JUCE)

# Uncomment when JUCE is available:
# juce_add_console_app(SimpleSynthExample
#     PRODUCT_NAME "SimpleSynthExample"
# )
# 
# target_sources(SimpleSynthExample PRIVATE
#     SimpleSynthTest.cpp
# )
# 
# target_compile_definitions(SimpleSynthExample PRIVATE
#     JUCE_USE_CURL=0
#     JUCE_WEB_BROWSER=0
#     JUCE_VST3_CAN_REPLACE_VST2=0
# )
# 
# target_link_libraries(SimpleSynthExample PRIVATE
#     golden_audio_juce
#     juce::juce_audio_basics
#     juce::juce_audio_processors
#     juce::juce_audio_formats
#     juce::juce_core
# )

message(STATUS "SimpleSynth example configured (requires JUCE to build)")
