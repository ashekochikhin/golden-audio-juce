# Simple Synth Example

This example demonstrates how to use the Golden Audio JUCE testing framework with a basic synthesizer.

## SimpleSynth

A minimal sine wave synthesizer that:
- Responds to MIDI note on/off messages
- Has a single `gain` parameter (0.0 - 1.0)
- Outputs stereo audio

## Test Cases

1. **SimpleSynthBasicTest** - Tests a single note (C4) with 0.7 gain
2. **SimpleSynthChordTest** - Tests a C major chord with 0.5 gain

## Building

1. Ensure JUCE is available in your project
2. Add golden-audio-juce to your CMake project
3. Build the example:

```bash
cmake -B build -S .
cmake --build build
```

## Running

### Generate golden audio files:
```bash
./build/SimpleSynthExample generate
```

This creates WAV files in `golden_files/`:
- `basic_note.wav`
- `chord_test.wav`

### Run regression tests:
```bash
./build/SimpleSynthExample test
```

Compares newly generated audio against the golden files and reports results.

## Integration

You can use this example as a template for your own JUCE plugin tests:

1. Replace `SimpleSynth` with your `AudioProcessor` class
2. Create test cases inheriting from `GoldenAudioTestCase<YourProcessor>`
3. Configure test parameters, MIDI sequences, and processor state
4. Generate golden files once
5. Run tests in CI/CD to catch regressions
