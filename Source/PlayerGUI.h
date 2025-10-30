#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h" // Include the audio engine

// This file was modified to use ImageButtons instead of TextButtons.

class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::Timer
{
public:
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
    

    //feature 3 mute button
    juce::TextButton muteButton{ "mute" };
    //feature 4 repeat button
    juce::TextButton repeatButton{ "repeat" };
    //================== Helpers ==================
    // This function now updates the play/pause button's image based on the audio state
    void updatePlayPauseButton();

    // Reference to our audio engine
    PlayerAudio& playerAudio;

    // GUI elements
    juce::TextButton loadButton{ "Load File" };

    // MODIFIED: Switched from TextButton to ImageButton
    juce::ImageButton playPauseButton;
    juce::ImageButton goToStartButton;
    juce::ImageButton goToEndButton;

    juce::Slider volumeSlider;

    std::unique_ptr<juce::FileChooser> fileChooser;

    //feature 6

    juce::Slider speedSlider;
    juce::Label speedLabel;

    // ADDED: Member variables to hold the loaded button images
    juce::Image playImage;
    juce::Image pauseImage;
    juce::Image toStartImage;
    juce::Image toEndImage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};