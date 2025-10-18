#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h" // Include the audio engine

class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::Timer
{
public:
    // The GUI needs a reference to the audio engine to control it
    PlayerGUI(PlayerAudio& audio);
    ~PlayerGUI() override;

    //================== Component Overrides ==================
    void paint(juce::Graphics& g) override;
    void resized() override;
    void timerCallback() override;

private:
    //================== Listener Overrides ==================
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

    //================== Helpers ==================
    void updatePlayPauseButtonText();

    // Reference to our audio engine
    PlayerAudio& playerAudio;

    // GUI elements
    juce::TextButton loadButton{ "Load File" };
    juce::TextButton playPauseButton{ "Play" };
    juce::TextButton stopButton{ "Pause" };
    juce::TextButton goToStartButton{ "<<" };
    juce::TextButton goToEndButton{ ">>" };
    juce::Slider volumeSlider;

    std::unique_ptr<juce::FileChooser> fileChooser;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};