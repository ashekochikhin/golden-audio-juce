#pragma once

#include <JuceHeader.h>
#include <memory>
#include <vector>
#include <string>

namespace GoldenAudio {

/**
 * MIDI note event for test sequences
 */
struct MidiNote {
    int noteNumber;
    float velocity;
    double startTime;
    double duration;
};

/**
 * Parameter state for test configuration
 */
struct ParameterState {
    std::string parameterName;
    float value;
};

/**
 * Base class for golden audio test cases
 * Template parameter ProcessorType should be derived from juce::AudioProcessor
 */
template<typename ProcessorType>
class GoldenAudioTestCase {
public:
    GoldenAudioTestCase(const std::string& name, const std::string& description)
        : testName(name), testDescription(description) {
    }
    
    virtual ~GoldenAudioTestCase() = default;
    
    // Test configuration
    void setDuration(double seconds) { testDurationSeconds = seconds; }
    void setSampleRate(double rate) { sampleRate = rate; }
    void setBufferSize(int size) { bufferSize = size; }
    void setTolerance(float tolerance) { comparisonTolerance = tolerance; }
    
    // Parameter setup
    void addParameter(const std::string& name, float value) {
        parameterStates.push_back({name, value});
    }
    
    void resetAllParameters() {
        parameterStates.clear();
    }
    
    // MIDI sequence setup
    void addNote(int noteNumber, float velocity, double startTime, double duration) {
        midiSequence.push_back({noteNumber, velocity, startTime, duration});
    }
    
    void addChord(const std::vector<int>& noteNumbers, float velocity, double startTime, double duration) {
        for (int noteNumber : noteNumbers) {
            addNote(noteNumber, velocity, startTime, duration);
        }
    }
    
    void addSequence(const std::vector<MidiNote>& notes) {
        midiSequence.insert(midiSequence.end(), notes.begin(), notes.end());
    }
    
    // Test execution
    bool generateGoldenAudio(ProcessorType& processor, const std::string& outputPath) {
        try {
            setupProcessor(processor);
            applyParametersToProcessor(processor);
            
            int totalSamples = static_cast<int>(testDurationSeconds * sampleRate);
            int totalBuffers = (totalSamples + bufferSize - 1) / bufferSize;
            
            juce::AudioBuffer<float> outputBuffer(getNumOutputChannels(), totalSamples);
            outputBuffer.clear();
            
            juce::MidiBuffer midiBuffer = createMidiBuffer();
            
            int sampleOffset = 0;
            for (int bufferIndex = 0; bufferIndex < totalBuffers; ++bufferIndex) {
                int currentBufferSize = std::min(bufferSize, totalSamples - sampleOffset);
                
                juce::AudioBuffer<float> tempBuffer(getNumOutputChannels(), currentBufferSize);
                tempBuffer.clear();
                
                juce::MidiBuffer currentMidiBuffer;
                extractMidiForBuffer(midiBuffer, currentMidiBuffer, sampleOffset, currentBufferSize);
                
                processor.processBlock(tempBuffer, currentMidiBuffer);
                
                for (int channel = 0; channel < getNumOutputChannels(); ++channel) {
                    outputBuffer.copyFrom(channel, sampleOffset, tempBuffer, channel, 0, currentBufferSize);
                }
                
                sampleOffset += currentBufferSize;
            }
            
            return saveToWavFile(outputBuffer, outputPath);
        } catch (const std::exception& e) {
            DBG("Exception in generateGoldenAudio: " << e.what());
            return false;
        }
    }
    
    bool compareWithGoldenAudio(ProcessorType& processor, const std::string& goldenPath) {
        std::string tempPath = goldenPath + ".temp.wav";
        if (!generateGoldenAudio(processor, tempPath)) {
            return false;
        }
        
        juce::AudioFormatManager formatManager;
        formatManager.registerBasicFormats();
        
        juce::File goldenFile(goldenPath);
        juce::File tempFile(tempPath);
        
        if (!goldenFile.exists()) {
            DBG("Golden file does not exist: " << goldenPath);
            tempFile.deleteFile();
            return false;
        }
        
        std::unique_ptr<juce::AudioFormatReader> goldenReader(formatManager.createReaderFor(goldenFile));
        std::unique_ptr<juce::AudioFormatReader> tempReader(formatManager.createReaderFor(tempFile));
        
        if (!goldenReader || !tempReader) {
            tempFile.deleteFile();
            return false;
        }
        
        if (goldenReader->sampleRate != tempReader->sampleRate ||
            goldenReader->numChannels != tempReader->numChannels ||
            goldenReader->lengthInSamples != tempReader->lengthInSamples) {
            DBG("Audio format mismatch in test: " << testName);
            tempFile.deleteFile();
            return false;
        }
        
        int numSamples = static_cast<int>(goldenReader->lengthInSamples);
        juce::AudioBuffer<float> goldenBuffer(static_cast<int>(goldenReader->numChannels), numSamples);
        juce::AudioBuffer<float> tempBuffer(static_cast<int>(tempReader->numChannels), numSamples);
        
        goldenReader->read(&goldenBuffer, 0, numSamples, 0, true, true);
        tempReader->read(&tempBuffer, 0, numSamples, 0, true, true);
        
        float rmsDifference = calculateRMSDifference(goldenBuffer, tempBuffer);
        
        tempFile.deleteFile();
        
        bool passed = rmsDifference <= comparisonTolerance;
        
        if (!passed) {
            DBG("Test failed: " << testName << " - RMS difference: " << rmsDifference 
                << " (tolerance: " << comparisonTolerance << ")");
        }
        
        return passed;
    }
    
    const std::string& getName() const { return testName; }
    const std::string& getDescription() const { return testDescription; }
    
protected:
    // Virtual methods for customization
    virtual void setupProcessor(ProcessorType& processor) {
        processor.setPlayConfigDetails(getNumInputChannels(), getNumOutputChannels(), sampleRate, bufferSize);
        processor.prepareToPlay(sampleRate, bufferSize);
    }
    
    virtual void applyParametersToProcessor(ProcessorType& processor) {
        for (const auto& paramState : parameterStates) {
            applyParameter(processor, paramState.parameterName, paramState.value);
        }
    }
    
    virtual void applyParameter(ProcessorType& processor, const std::string& paramName, float value) {
        // Default implementation - override for project-specific parameter access
        auto& params = processor.getParameters();
        for (auto* param : params) {
            if (param->getName(100) == paramName) {
                param->setValueNotifyingHost(value);
                return;
            }
        }
    }
    
    virtual int getNumInputChannels() const { return 0; }  // Override for non-synth processors
    virtual int getNumOutputChannels() const { return 2; } // Stereo by default
    
private:
    std::string testName;
    std::string testDescription;
    double testDurationSeconds = 5.0;
    double sampleRate = 44100.0;
    int bufferSize = 512;
    float comparisonTolerance = 0.001f;
    
    std::vector<ParameterState> parameterStates;
    std::vector<MidiNote> midiSequence;
    
    juce::MidiBuffer createMidiBuffer() {
        juce::MidiBuffer buffer;
        
        for (const auto& note : midiSequence) {
            int startSample = static_cast<int>(note.startTime * sampleRate);
            juce::MidiMessage noteOnMessage = juce::MidiMessage::noteOn(1, note.noteNumber, note.velocity);
            buffer.addEvent(noteOnMessage, startSample);
            
            int endSample = static_cast<int>((note.startTime + note.duration) * sampleRate);
            juce::MidiMessage noteOffMessage = juce::MidiMessage::noteOff(1, note.noteNumber, 0.0f);
            buffer.addEvent(noteOffMessage, endSample);
        }
        
        return buffer;
    }
    
    void extractMidiForBuffer(const juce::MidiBuffer& sourceMidi, juce::MidiBuffer& destMidi, 
                              int startSample, int numSamples) {
        int endSample = startSample + numSamples;
        
        for (const auto& midiIterator : sourceMidi) {
            int eventSample = midiIterator.samplePosition;
            if (eventSample >= startSample && eventSample < endSample) {
                int relativeSample = eventSample - startSample;
                destMidi.addEvent(midiIterator.getMessage(), relativeSample);
            }
        }
    }
    
    bool saveToWavFile(const juce::AudioBuffer<float>& buffer, const std::string& outputPath) {
        juce::File outputFile(outputPath);
        outputFile.getParentDirectory().createDirectory();
        
        if (outputFile.existsAsFile()) {
            outputFile.deleteFile();
        }
        
        juce::WavAudioFormat wavFormat;
        std::unique_ptr<juce::AudioFormatWriter> writer(
            wavFormat.createWriterFor(
                new juce::FileOutputStream(outputFile),
                sampleRate,
                static_cast<unsigned int>(buffer.getNumChannels()),
                16,
                {},
                0
            )
        );
        
        if (writer != nullptr) {
            writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
            return true;
        }
        
        return false;
    }
    
    float calculateRMSDifference(const juce::AudioBuffer<float>& buffer1, 
                                 const juce::AudioBuffer<float>& buffer2) {
        if (buffer1.getNumChannels() != buffer2.getNumChannels() ||
            buffer1.getNumSamples() != buffer2.getNumSamples()) {
            return std::numeric_limits<float>::max();
        }
        
        float sumSquaredDifference = 0.0f;
        int totalSamples = 0;
        
        for (int channel = 0; channel < buffer1.getNumChannels(); ++channel) {
            const float* data1 = buffer1.getReadPointer(channel);
            const float* data2 = buffer2.getReadPointer(channel);
            
            for (int sample = 0; sample < buffer1.getNumSamples(); ++sample) {
                float difference = data1[sample] - data2[sample];
                sumSquaredDifference += difference * difference;
                totalSamples++;
            }
        }
        
        if (totalSamples == 0) {
            return 0.0f;
        }
        
        return std::sqrt(sumSquaredDifference / totalSamples);
    }
};

/**
 * Test suite manager
 */
template<typename ProcessorType>
class GoldenAudioTestSuite {
public:
    void addTest(std::unique_ptr<GoldenAudioTestCase<ProcessorType>> testCase) {
        testCases.push_back(std::move(testCase));
    }
    
    void generateAllGoldenAudio(ProcessorType& processor, const std::string& outputDirectory) {
        juce::File outputDir(outputDirectory);
        outputDir.createDirectory();
        
        for (auto& testCase : testCases) {
            std::string outputPath = outputDirectory + "/" + testCase->getName() + ".wav";
            
            std::cout << "Generating golden audio for: " << testCase->getName() << std::endl;
            
            if (testCase->generateGoldenAudio(processor, outputPath)) {
                std::cout << "✓ Generated: " << testCase->getName() << std::endl;
            } else {
                std::cout << "✗ Failed: " << testCase->getName() << std::endl;
            }
        }
    }
    
    bool runAllTests(ProcessorType& processor, const std::string& goldenDirectory) {
        bool allPassed = true;
        passedTests = 0;
        failedTests = 0;
        
        for (auto& testCase : testCases) {
            std::string goldenPath = goldenDirectory + "/" + testCase->getName() + ".wav";
            
            if (testCase->compareWithGoldenAudio(processor, goldenPath)) {
                passedTests++;
            } else {
                failedTests++;
                allPassed = false;
            }
        }
        
        return allPassed;
    }
    
    void printResults() const {
        std::cout << "\n=== Test Results ===" << std::endl;
        std::cout << "Passed: " << passedTests << std::endl;
        std::cout << "Failed: " << failedTests << std::endl;
        std::cout << "Total: " << (passedTests + failedTests) << std::endl;
    }
    
    int getPassedCount() const { return passedTests; }
    int getFailedCount() const { return failedTests; }
    
private:
    std::vector<std::unique_ptr<GoldenAudioTestCase<ProcessorType>>> testCases;
    int passedTests = 0;
    int failedTests = 0;
};

} // namespace GoldenAudio
