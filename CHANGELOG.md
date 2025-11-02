# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- Initial release of Golden Audio JUCE testing framework
- Template-based `GoldenAudioTestCase<ProcessorType>` class
- Template-based `GoldenAudioTestSuite<ProcessorType>` class
- MIDI sequence support (notes, chords, sequences)
- Parameter state management
- WAV file generation and comparison
- RMS-based audio comparison with configurable tolerance
- Header-only library design
- CMake integration with interface library pattern

### Features
- Support for synth and effect plugins
- Customizable processor setup via virtual methods
- Configurable sample rate, buffer size, and test duration
- Automatic WAV file creation and cleanup
- Comprehensive test result reporting

## [1.0.0] - 2025-11-02

### Added
- Initial public release
