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

    // feature 8
    addAndMakeVisible(playlistBox);
    playlistBox.setModel(this);
    addAndMakeVisible(previousButton);
    previousButton.addListener(this);
    addAndMakeVisible(nextButton);
    nextButton.addListener(this);
}

PlayerGUI::~PlayerGUI() {}

void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

void PlayerGUI::resized()
{
    int buttonWidth = 60; 
    int buttonHeight = 40;
    int margin = 10;
    int topRowY = 10; 

    // --- Top row of controls ---
    loadButton.setBounds(margin, topRowY, 100, buttonHeight);
    previousButton.setBounds(loadButton.getRight() + margin, topRowY, buttonWidth, buttonHeight); 
    goToStartButton.setBounds(previousButton.getRight() + margin, topRowY, buttonWidth, buttonHeight); 
    playPauseButton.setBounds(goToStartButton.getRight() + margin, topRowY, buttonWidth, buttonHeight); 
    goToEndButton.setBounds(playPauseButton.getRight() + margin, topRowY, buttonWidth, buttonHeight); 
    nextButton.setBounds(goToEndButton.getRight() + margin, topRowY, buttonWidth, buttonHeight); 
    muteButton.setBounds(nextButton.getRight() + margin, topRowY, 80, buttonHeight); 
    repeatButton.setBounds(muteButton.getRight() + margin, topRowY, 100, buttonHeight); 

    int secondRowY = topRowY + buttonHeight + margin; 
    volumeSlider.setBounds(margin, secondRowY, getWidth() - (margin * 2), 30); 

    int thirdRowY = secondRowY + 4 * margin; 
    speedSlider.setBounds(margin, thirdRowY, getWidth() - (margin * 2), 30); 

    int playlistY = thirdRowY + 20* margin; 
    int trackInfoHeight = 25; 
    trackInfoLabel.setBounds(margin, getHeight() - trackInfoHeight - 5, getWidth() - (margin * 2), trackInfoHeight); 
    playlistBox.setBounds(margin, playlistY, getWidth() - (margin * 2), getHeight() - playlistY - trackInfoHeight - 15); 
}

void PlayerGUI::timerCallback()
{
    updatePlayPauseButton();
    // feature 8 
    if (playerAudio.getNumTracks() > 0)
    {
        if (playerAudio.isFinished() && !playerAudio.GetRepeatState())
        {
            playerAudio.playNext();
        }

        if (playlistBox.getSelectedRow() != playerAudio.getCurrentTrackIndex())
        {
            playlistBox.selectRow(playerAudio.getCurrentTrackIndex());
            updateTrackInfo();
        }
    }
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>(
            "Select audio files...", // feature 8
            juce::File{},
            "*.wav;*.mp3");

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles | juce::FileBrowserComponent::canSelectMultipleItems, // feature 8
            [this](const juce::FileChooser& fc)
            {
                auto files = fc.getResults(); // feature 8
                if (!files.isEmpty()) // feature 8
                {
                    playerAudio.loadFiles(files); // feature 8
                    playlistBox.updateContent(); // feature 8
                    playlistBox.selectRow(0); // feature 8
                    updateTrackInfo();
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
    else if (button == &previousButton) // feature 8
    {
        playerAudio.playPrevious();
    }
    else if (button == &nextButton) // feature 8
    {
        playerAudio.playNext();
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

// feature 8
int PlayerGUI::getNumRows()
{
    return playerAudio.getNumTracks();
}

// feature 8
void PlayerGUI::paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
    {
        g.fillAll(juce::Colours::lightblue);
    }

    g.setColour(juce::Colours::black);
    g.setFont(14.0f);

    auto trackTitles = playerAudio.getTrackTitles();
    if (rowNumber < trackTitles.size())
    {
        // Highlight the current playing track
        if (rowNumber == playerAudio.getCurrentTrackIndex())
        {
            g.setColour(juce::Colours::orange);
            g.setFont(juce::Font(14.0f, juce::Font::bold));
        }

        g.drawText(trackTitles[rowNumber], 5, 0, width - 10, height, juce::Justification::centredLeft, true);
    }
}

// feature 8 
void PlayerGUI::listBoxItemDoubleClicked(int row, const juce::MouseEvent&)
{
    playerAudio.playTrack(row);
}