# Golden Audio - JUCE Testing Framework

A template-based C++ library for golden audio testing in JUCE projects. Generate reference audio files and automatically detect regressions in your audio plugins.

## Features

- 🎵 **Template-based design** - Works with any `juce::AudioProcessor`-derived class
- 🎹 **MIDI sequence support** - Define complex note patterns and automation
- 📊 **RMS-based comparison** - Configurable tolerance for regression detection
- 🔧 **Easy integration** - Header-only core, minimal dependencies
- 🎯 **Flexible parameter control** - Type-safe parameter management
- 📁 **WAV file generation** - Standard 16-bit stereo output

## Quick Start

### 1. Add to your project

```cmake
# CMakeLists.txt
include(FetchContent)

FetchContent_Declare(
    golden_audio_juce
    GIT_REPOSITORY https://github.com/yourusername/golden-audio-juce.git
    GIT_TAG main
)
FetchContent_MakeAvailable(golden_audio_juce)

target_link_libraries(YourTestTarget PRIVATE golden_audio_juce)
```

### 2. Create a test case

```cpp
#include <golden_audio/GoldenAudioTestBase.h>

class MyPluginTest : public GoldenAudio::GoldenAudioTestCase<MyAudioProcessor> {
public:
    MyPluginTest() : GoldenAudioTestCase("basic_test", "Test basic audio generation") {
        // Configure test
        setDuration(2.0);  // 2 seconds
        setSampleRate(44100.0);
        
        // Add parameters
        addParameter("gain", 0.8f);
        addParameter("filter_cutoff", 0.5f);
        
        // Add MIDI notes (for synths)
        addNote(60, 0.8f, 0.1, 1.5);  // C4, velocity 0.8, starts at 0.1s, duration 1.5s
    }
    
protected:
    void setupProcessor(MyAudioProcessor& processor) override {
        // Custom processor setup
        processor.setPlayConfigDetails(2, 2, 44100, 512);  // Stereo in/out
        processor.prepareToPlay(44100, 512);
    }
};
```

### 3. Generate golden files

```cpp
MyAudioProcessor processor;
MyPluginTest test;

// Generate reference audio
test.generateGoldenAudio(processor, "golden_files/basic_test.wav");
```

### 4. Run regression tests

```cpp
MyAudioProcessor processor;
MyPluginTest test;

// Compare against golden file
bool passed = test.compareWithGoldenAudio(processor, "golden_files/basic_test.wav");
```

## API Reference

### Test Configuration

```cpp
void setDuration(double seconds);           // Set test duration
void setSampleRate(double rate);            // Set sample rate (default: 44100)
void setBufferSize(int size);               // Set buffer size (default: 512)
void setTolerance(float tolerance);         // Set RMS tolerance (default: 0.001)
```

### Parameters

```cpp
void addParameter(const std::string& name, float value);
void resetAllParameters();
```

### MIDI Sequences

```cpp
void addNote(int noteNumber, float velocity, double startTime, double duration);
void addChord(const std::vector<int>& notes, float velocity, double startTime, double duration);
void addSequence(const std::vector<MidiNote>& notes);
```

### Test Suite Management

```cpp
GoldenAudio::GoldenAudioTestSuite<MyProcessor> suite;

// Add tests
suite.addTest(std::make_unique<MyTest1>());
suite.addTest(std::make_unique<MyTest2>());

// Generate all golden files
suite.generateAllGoldenAudio(processor, "golden_files/");

// Run all tests
bool allPassed = suite.runAllTests(processor, "golden_files/");
suite.printResults();
```

## Advanced Usage

### Custom Parameter Application

```cpp
class MyTest : public GoldenAudio::GoldenAudioTestCase<MyProcessor> {
protected:
    void applyParameter(MyProcessor& processor, const std::string& name, float value) override {
        // Custom parameter access logic
        if (auto* param = processor.getParameterByName(name)) {
            param->setValue(value);
        }
    }
};
```

### Effect Plugins (Non-Synth)

```cpp
class MyEffectTest : public GoldenAudio::GoldenAudioTestCase<MyEffectProcessor> {
protected:
    int getNumInputChannels() const override { return 2; }   // Stereo input
    int getNumOutputChannels() const override { return 2; }  // Stereo output
    
    // No MIDI notes needed for effects
};
```

## Building Examples

```bash
cd golden-audio-juce
cmake -B build -S .
cmake --build build

# Run example
./build/examples/basic_synth/BasicSynthExample
```

## Requirements

- CMake 3.15+
- JUCE 7.0+
- C++17 or later

## License

MIT License - see LICENSE file for details

## Contributing

Contributions welcome! Please open an issue or PR.

1. Fork the repository
2. Create your feature branch
3. Add tests for new features
4. Submit a pull request
