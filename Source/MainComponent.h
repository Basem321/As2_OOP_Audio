#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"
#include "PlayerGUI.h"
#include "AudioMixer.h"

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
    PlayerAudio playerAudio1; 
    PlayerAudio playerAudio2; 
    AudioMixer audioMixer;    

    PlayerGUI playerGUI1;
    PlayerGUI playerGUI2;

    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};