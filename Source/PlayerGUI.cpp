#include "PlayerGUI.h"

PlayerGUI::PlayerGUI(PlayerAudio& audio) : playerAudio(audio)
{
    // Add buttons and set this class as their listener
    for (auto* btn : { &loadButton, &playPauseButton, &goToStartButton, &goToEndButton })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }

    // Set more descriptive labels
    goToStartButton.setButtonText("Start");
    goToEndButton.setButtonText("End");

    // Configure and add the volume slider
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);

    // Set initial gain
    playerAudio.setGain((float)volumeSlider.getValue());

    // Start timer to update the play/pause button text
    startTimerHz(10);
}

PlayerGUI::~PlayerGUI() {}

void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

void PlayerGUI::resized()
{
    // Layout the GUI components
    int buttonWidth = 80;
    int buttonHeight = 40;
    int margin = 10;
    int yPos = 20;

    loadButton.setBounds(margin, yPos, 100, buttonHeight);
    goToStartButton.setBounds(loadButton.getRight() + margin, yPos, buttonWidth, buttonHeight);
    playPauseButton.setBounds(goToStartButton.getRight() + margin, yPos, buttonWidth, buttonHeight);
    goToEndButton.setBounds(playPauseButton.getRight() + margin, yPos, buttonWidth, buttonHeight);

    volumeSlider.setBounds(margin, yPos + buttonHeight + margin, getWidth() - (margin * 2), 30);
}

void PlayerGUI::timerCallback()
{
    // Periodically update the button text based on the transport's state
    updatePlayPauseButtonText();
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>(
            "Select an audio file...",
            juce::File{},
            "*.wav;*.mp3");

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile())
                {
                    playerAudio.loadFile(file);
                }
            });
    }
    else if (button == &playPauseButton)
    {
        if (playerAudio.isPlaying())
            playerAudio.pause();
        else
            playerAudio.play();
    }
    else if (button == &goToStartButton)
    {
        playerAudio.goToStart();
    }
    else if (button == &goToEndButton)
    {
        playerAudio.goToEnd();
    }
}

void PlayerGUI::updatePlayPauseButtonText()
{
    if (playerAudio.isPlaying())
    {
        playPauseButton.setButtonText("Pause");
    }
    else
    {
        playPauseButton.setButtonText("Play");
    }
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
    {
        playerAudio.setGain((float)slider->getValue());
    }
}