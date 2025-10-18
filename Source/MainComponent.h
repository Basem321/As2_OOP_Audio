#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"
#include "PlayerGUI.h"

class MainComponent : public juce::AudioAppComponent
{
public:
    MainComponent();
    ~MainComponent() override;

    //================== AudioAppComponent Overrides ==================
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //================== Component Overrides ==================
    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    PlayerAudio playerAudio; // Owns the audio engine
    PlayerGUI playerGUI;     // Owns the GUI, gives it a reference to the engine

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};