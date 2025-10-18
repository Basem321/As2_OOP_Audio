#include "MainComponent.h"

MainComponent::MainComponent()
    : playerGUI(playerAudio) // Initialize GUI with a reference to the audio engine
{
    addAndMakeVisible(playerGUI);
    setSize(500, 250);
    setAudioChannels(0, 2); // 0 inputs, 2 outputs
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    // Pass the call to our audio engine
    playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Pass the call to our audio engine
    playerAudio.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    // Pass the call to our audio engine
    playerAudio.releaseResources();
}

void MainComponent::paint(juce::Graphics& g)
{
    // This can be empty, as the child PlayerGUI component handles painting
}

void MainComponent::resized()
{
    // Make the PlayerGUI component fill the entire MainComponent
    playerGUI.setBounds(getLocalBounds());
}