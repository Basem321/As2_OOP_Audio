#include "PlayerGUI.h"
#include "BinaryData.h"

PlayerGUI::PlayerGUI(PlayerAudio& audio) : playerAudio(audio)
{
    // Load images
    playImage = juce::ImageCache::getFromMemory(BinaryData::start_png, BinaryData::start_pngSize);
    pauseImage = juce::ImageCache::getFromMemory(BinaryData::Pause_png, BinaryData::Pause_pngSize);
    toStartImage = juce::ImageCache::getFromMemory(BinaryData::GoToStart_png, BinaryData::GoToStart_pngSize);
    toEndImage = juce::ImageCache::getFromMemory(BinaryData::GoToEnd_png, BinaryData::GoToEnd_pngSize);


    // Setup image buttons
    playPauseButton.setImages(true, true, true, playImage, 1.0f, {}, playImage, 0.8f, {}, playImage, 0.5f, {});
    goToStartButton.setImages(true, true, true, toStartImage, 1.0f, {}, toStartImage, 0.8f, {}, toStartImage, 0.5f, {});
    goToEndButton.setImages(true, true, true, toEndImage, 1.0f, {}, toEndImage, 0.8f, {}, toEndImage, 0.5f, {});

    addAndMakeVisible(loadButton);
    loadButton.addListener(this);

    addAndMakeVisible(playPauseButton);
    playPauseButton.addListener(this);

    addAndMakeVisible(goToStartButton);
    goToStartButton.addListener(this);

    addAndMakeVisible(goToEndButton);
    goToEndButton.addListener(this);

    muteButton.setButtonText("Unmuted");
    muteButton.addListener(this);
    addAndMakeVisible(muteButton);

    repeatButton.setButtonText("click to repeat");
    repeatButton.addListener(this);
    addAndMakeVisible(repeatButton);

    // Configure and add the volume slider
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);

	//feature 5 metadata label
    trackInfoLabel.setText("No file loaded", juce::dontSendNotification);
    trackInfoLabel.setJustificationType(juce::Justification::centred);
    trackInfoLabel.setFont(14.0f);
    addAndMakeVisible(trackInfoLabel);

    // Set initial gain
    playerAudio.setGain((float)volumeSlider.getValue());

    startTimerHz(10);

    //feature 6
    speedSlider.setRange(0.25, 2.0, 0.05);
    speedSlider.setValue(1.0);
    speedSlider.setTextValueSuffix(" x");
    speedSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    speedLabel.setJustificationType(juce::Justification::centredRight);
    speedSlider.addListener(this);
    addAndMakeVisible(speedSlider);



    speedLabel.attachToComponent(&speedSlider, true);
    addAndMakeVisible(speedLabel);
}

PlayerGUI::~PlayerGUI() {}

void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

void PlayerGUI::resized()
{
    int buttonWidth = 80;
    int buttonHeight = 40;
    int margin = 10;
    int yPos = 20;

    // --- Top row of controls ---
    loadButton.setBounds(margin, yPos, 100, buttonHeight);
    goToStartButton.setBounds(loadButton.getRight() + margin, yPos, buttonWidth, buttonHeight);
    playPauseButton.setBounds(goToStartButton.getRight() + margin, yPos, buttonWidth, buttonHeight);
    goToEndButton.setBounds(playPauseButton.getRight() + margin, yPos, buttonWidth, buttonHeight);
    muteButton.setBounds(goToEndButton.getRight() + margin, yPos, buttonWidth, buttonHeight);
    repeatButton.setBounds(muteButton.getRight() + margin, yPos, 100, buttonHeight);

    volumeSlider.setBounds(margin, yPos + buttonHeight + margin, getWidth() - (margin * 2), 30);

	// metadata label
    trackInfoLabel.setBounds(margin, 450, getWidth() - (margin * 2), 25);


    speedSlider.setBounds(margin, 60, getWidth() - (margin * 2) - 80, 150);
}

void PlayerGUI::timerCallback()
{
    updatePlayPauseButton();
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
                    updateTrackInfo(); // for update metadata by basem
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

    else if (button == &muteButton) {
        playerAudio.SwitchMute();

        if (playerAudio.GetMuteState()) {
            muteButton.setButtonText("Muted");
        }
        else {
            muteButton.setButtonText("Unmuted");
        }
    }
    else if (button == &repeatButton) {
        playerAudio.switchrepeat();

        if (playerAudio.GetRepeatState()) {
            repeatButton.setButtonText("is being repeated");
        }
        else {
            repeatButton.setButtonText("click to repeat");
        }
    }
}
// feature 5
void PlayerGUI::updateTrackInfo()
{
    juce::String title = playerAudio.getTrackTitle();
    juce::String duration = playerAudio.getTrackDuration();

    if (duration.isNotEmpty())
        trackInfoLabel.setText(title + " (" + duration + ")", juce::dontSendNotification);
    else
        trackInfoLabel.setText(title, juce::dontSendNotification);
}

void PlayerGUI::updatePlayPauseButton()
{
    const auto& currentImage = playPauseButton.getNormalImage();

    if (playerAudio.isPlaying() && currentImage != pauseImage)
    {
        playPauseButton.setImages(true, true, true, pauseImage, 1.0f, {}, pauseImage, 0.8f, {}, pauseImage, 0.5f, {});
    }
    else if (!playerAudio.isPlaying() && currentImage != playImage)
    {
        playPauseButton.setImages(true, true, true, playImage, 1.0f, {}, playImage, 0.8f, {}, playImage, 0.5f, {});
    }
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
    {
        playerAudio.setGain((float)slider->getValue());
    }

    if (slider == &speedSlider)
    {
        playerAudio.setPlaybackSpeed((float)slider->getValue());
    }
}