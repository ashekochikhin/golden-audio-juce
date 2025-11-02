#include "SimpleSynth.h"
#include <golden_audio/GoldenAudioTestBase.h>
#include <iostream>

/**
 * Example test case for SimpleSynth
 */
class SimpleSynthBasicTest : public GoldenAudio::GoldenAudioTestCase<SimpleSynth> {
public:
    SimpleSynthBasicTest() 
        : GoldenAudioTestCase("basic_note", "Test basic note playback") 
    {
        setDuration(2.0);
        setSampleRate(44100.0);
        setBufferSize(512);
        
        // Add a single note
        addNote(60, 0.8f, 0.1, 1.5);  // C4, starts at 0.1s, duration 1.5s
        
        // Set gain parameter
        addParameter("gain", 0.7f);
    }

protected:
    void setupProcessor(SimpleSynth& processor) override {
        processor.setPlayConfigDetails(0, 2, 44100, 512);
        processor.prepareToPlay(44100, 512);
    }
};

/**
 * Test with chord
 */
class SimpleSynthChordTest : public GoldenAudio::GoldenAudioTestCase<SimpleSynth> {
public:
    SimpleSynthChordTest() 
        : GoldenAudioTestCase("chord_test", "Test chord playback") 
    {
        setDuration(2.0);
        
        // C major chord (C-E-G)
        addChord({60, 64, 67}, 0.6f, 0.5, 1.0);
        
        addParameter("gain", 0.5f);
    }

protected:
    void setupProcessor(SimpleSynth& processor) override {
        processor.setPlayConfigDetails(0, 2, 44100, 512);
        processor.prepareToPlay(44100, 512);
    }
};

int main(int argc, char* argv[]) {
    std::cout << "Golden Audio JUCE - Simple Synth Example\n" << std::endl;
    
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <generate|test>" << std::endl;
        std::cout << "  generate - Generate golden audio files" << std::endl;
        std::cout << "  test     - Run regression tests" << std::endl;
        return 1;
    }
    
    std::string mode(argv[1]);
    
    // Create test suite
    GoldenAudio::GoldenAudioTestSuite<SimpleSynth> suite;
    suite.addTest(std::make_unique<SimpleSynthBasicTest>());
    suite.addTest(std::make_unique<SimpleSynthChordTest>());
    
    SimpleSynth processor;
    
    if (mode == "generate") {
        std::cout << "Generating golden audio files...\n" << std::endl;
        suite.generateAllGoldenAudio(processor, "golden_files");
        std::cout << "\n✓ Golden files generated in golden_files/" << std::endl;
    } 
    else if (mode == "test") {
        std::cout << "Running golden audio tests...\n" << std::endl;
        bool allPassed = suite.runAllTests(processor, "golden_files");
        suite.printResults();
        
        return allPassed ? 0 : 1;
    } 
    else {
        std::cout << "Unknown mode: " << mode << std::endl;
        return 1;
    }
    
    return 0;
}
