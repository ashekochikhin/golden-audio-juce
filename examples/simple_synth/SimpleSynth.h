#pragma once

#include <JuceHeader.h>

/**
 * Simple example synthesizer for demonstrating golden audio testing
 * Generates a basic sine wave with ADSR envelope
 */
class SimpleSynth : public juce::AudioProcessor {
public:
    SimpleSynth()
        : AudioProcessor(BusesProperties()
            .withOutput("Output", juce::AudioChannelSet::stereo(), true))
    {
        addParameter(gainParam = new juce::AudioParameterFloat(
            "gain", "Gain", 0.0f, 1.0f, 0.7f));
    }

    void prepareToPlay(double sampleRate, int samplesPerBlock) override {
        currentSampleRate = sampleRate;
        synth.setCurrentPlaybackSampleRate(sampleRate);
        
        for (int i = 0; i < 8; ++i) {
            synth.addVoice(new juce::SynthesiserVoice());
        }
        
        synth.addSound(new juce::SynthesiserSound());
    }

    void releaseResources() override {}

    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override {
        buffer.clear();
        
        // Simple sine wave synthesis
        for (const auto metadata : midiMessages) {
            auto message = metadata.getMessage();
            
            if (message.isNoteOn()) {
                currentNote = message.getNoteNumber();
                currentPhase = 0.0;
                isPlaying = true;
            } else if (message.isNoteOff()) {
                if (currentNote == message.getNoteNumber()) {
                    isPlaying = false;
                }
            }
        }
        
        if (isPlaying) {
            float frequency = juce::MidiMessage::getMidiNoteInHertz(currentNote);
            float phaseIncrement = frequency / currentSampleRate;
            float gain = gainParam->get();
            
            for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
                float sampleValue = std::sin(currentPhase * 2.0 * juce::MathConstants<double>::pi) * gain;
                
                for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
                    buffer.setSample(channel, sample, sampleValue);
                }
                
                currentPhase += phaseIncrement;
                if (currentPhase >= 1.0) currentPhase -= 1.0;
            }
        }
    }

    const juce::String getName() const override { return "SimpleSynth"; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}
    void getStateInformation(juce::MemoryBlock&) override {}
    void setStateInformation(const void*, int) override {}

    juce::AudioProcessorEditor* createEditor() override { return nullptr; }
    bool hasEditor() const override { return false; }

private:
    juce::AudioParameterFloat* gainParam;
    juce::Synthesiser synth;
    double currentSampleRate = 44100.0;
    double currentPhase = 0.0;
    int currentNote = 60;
    bool isPlaying = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleSynth)
};
