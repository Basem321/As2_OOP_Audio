#include "MainComponent.h"

MainComponent::MainComponent():
    playerAudio1(),
    playerAudio2(),
    playerGUI1(playerAudio1),
    playerGUI2(playerAudio2),
    audioMixer(playerAudio1, playerAudio2)
    
{
    addAndMakeVisible(playerGUI1);
    addAndMakeVisible(playerGUI2);

    setAudioChannels(0, 2); // 0 inputs, 2 outputs
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    // Pass the call to our audio engine
    audioMixer.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Pass the call to our audio engine
    audioMixer.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    // Pass the call to our audio engine
    audioMixer.releaseResources();
}

void MainComponent::paint(juce::Graphics& g)
{
    // This can be empty, as the child PlayerGUI component handles painting
}

void MainComponent::resized()
{
    auto bounds = getLocalBounds();
    playerGUI1.setBounds(bounds.removeFromLeft(bounds.getWidth() / 2));
    playerGUI2.setBounds(bounds);
}