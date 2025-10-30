#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h" 

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

    //================== Helpers ==================
    // This function now updates the play/pause button's image based on the audio state
    void updatePlayPauseButton();

    void updateTrackInfo();

    PlayerAudio& playerAudio;

    // GUI elements
    juce::TextButton loadButton{ "Load File" };

    juce::ImageButton playPauseButton;
    juce::ImageButton goToStartButton;
    juce::ImageButton goToEndButton;

    //feature 3 mute button
    juce::TextButton muteButton{ "mute" };
    //feature 4 repeat button
    juce::TextButton repeatButton{ "repeat" };

    juce::Slider volumeSlider;

	//feature 5 metadata label
    juce::Label trackInfoLabel; 


    //feature 6 
    juce::Slider speedSlider;
    juce::Label speedLabel;


    std::unique_ptr<juce::FileChooser> fileChooser;

    juce::Image playImage;
    juce::Image pauseImage;
    juce::Image toStartImage;
    juce::Image toEndImage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};