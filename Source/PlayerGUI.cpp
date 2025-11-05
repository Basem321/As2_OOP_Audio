#include "PlayerGUI.h"
#include "BinaryData.h"

PlayerGUI::PlayerGUI(PlayerAudio& audio) : playerAudio(audio)
{
    playImage = juce::ImageCache::getFromMemory(BinaryData::start_png, BinaryData::start_pngSize);
    pauseImage = juce::ImageCache::getFromMemory(BinaryData::Pause_png, BinaryData::Pause_pngSize);
    toStartImage = juce::ImageCache::getFromMemory(BinaryData::GoToStart_png, BinaryData::GoToStart_pngSize);
    toEndImage = juce::ImageCache::getFromMemory(BinaryData::GoToEnd_png, BinaryData::GoToEnd_pngSize);

    //                                         normal state,              hover state,                   clicked state
    playPauseButton.setImages(true, true, true, playImage, 1.0f, juce::Colours::transparentBlack, playImage, 0.8f, juce::Colours::transparentBlack, playImage, 0.5f, juce::Colours::transparentBlack);
    goToStartButton.setImages(true, true, true, toStartImage, 1.0f, juce::Colours::transparentBlack, toStartImage, 0.8f, juce::Colours::transparentBlack, toStartImage, 0.5f, juce::Colours::transparentBlack);
    goToEndButton.setImages(true, true, true, toEndImage, 1.0f, juce::Colours::transparentBlack, toEndImage, 0.8f, juce::Colours::transparentBlack, toEndImage, 0.5f, juce::Colours::transparentBlack);

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
    volumeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
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

    //feature 9
    //position slider
    positionSlider.setSliderStyle(juce::Slider::LinearBar);
    positionSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    positionSlider.setRange(0.0, 1.0, 0.0);
    positionSlider.addListener(this);
    addAndMakeVisible(positionSlider);

    //Time Label
    timeLabel.setText("0:00 / 0:00", juce::dontSendNotification);
    timeLabel.setJustificationType(juce::Justification::centred);
    timeLabel.setFont(12.0f);
    addAndMakeVisible(timeLabel);

    //feature 10 loop section sliders
    loopStartSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    loopStartSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    loopStartSlider.setRange(0.0, 100.0, 0.1);
    loopStartSlider.setValue(0.0);
    loopStartSlider.addListener(this);
    addAndMakeVisible(loopStartSlider);

    loopStartLabel.setText("Loop Start:", juce::dontSendNotification);
    loopStartLabel.setJustificationType(juce::Justification::centredRight);
    loopStartLabel.attachToComponent(&loopStartSlider, true);
    addAndMakeVisible(loopStartLabel);

    loopEndSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    loopEndSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    loopEndSlider.setRange(0.0, 100.0, 0.1);
    loopEndSlider.setValue(10.0);
    loopEndSlider.addListener(this);
    addAndMakeVisible(loopEndSlider);

    loopEndLabel.setText("Loop End:", juce::dontSendNotification);
    loopEndLabel.setJustificationType(juce::Justification::centredRight);
    loopEndLabel.attachToComponent(&loopEndSlider, true);
    addAndMakeVisible(loopEndLabel);

    // innovation feature
    addAndMakeVisible(themeButton);
    themeButton.addListener(this);

    currentTheme = Default;
    applyTheme();
}


PlayerGUI::~PlayerGUI() {}

void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(bgColour); // Use the theme's background color
}

void PlayerGUI::applyTheme()
{
    switch (currentTheme)
    {
    case Girls:
        bgColour = juce::Colour(0xFFFDEEF4); // Light Pink
        boxBgColour = juce::Colour(0xFFFADADD); // Medium Pink
        accentColour = juce::Colour(0xFFE1306C); // Strong Pink/Magenta
        sliderTrackColour = juce::Colour(0xFFF8C8DC); // Lighter Pink
        buttonColour = juce::Colour(0xFFF4ABC4); // Soft Pink
        textColour = juce::Colour(0xFF5E2A3A); // Dark Purple/Brown
        break;

    case Boys:
        bgColour = juce::Colour(0xFF1A2639); // Dark Blue
        boxBgColour = juce::Colour(0xFF283747); // Dark Slate Blue
        accentColour = juce::Colour(0xFF3498DB); // Bright Blue
        sliderTrackColour = juce::Colour(0xFF5D6D7E); // Greyish Blue
        buttonColour = juce::Colour(0xFF2E86C1); // Medium Blue
        textColour = juce::Colour(0xFFEAECEE); // Light Grey/White
        break;

    case Default:
    default:
        bgColour = juce::Colour(0xFF1F1F2B); // Very Dark Blue
        boxBgColour = juce::Colour(0xFF282C34); // Dark Slate Grey
        accentColour = juce::Colour(0xFF00D8FF); // Bright Cyan
        sliderTrackColour = juce::Colour(0xFF3A4A5C); // Dark Blue-Grey
        buttonColour = juce::Colour(0xFF3A3A4A); // Charcoal
        textColour = juce::Colour(0xFFAAAAAA); // Light Grey
        break;
    }

    playPauseButton.setImages(true, true, true, playImage, 1.0f, accentColour, playImage, 0.8f, accentColour, playImage, 0.5f, accentColour);
    goToStartButton.setImages(true, true, true, toStartImage, 1.0f, textColour, toStartImage, 0.8f, textColour, toStartImage, 0.5f, textColour);
    goToEndButton.setImages(true, true, true, toEndImage, 1.0f, textColour, toEndImage, 0.8f, textColour, toEndImage, 0.5f, textColour);

    loadButton.setColour(juce::TextButton::buttonColourId, buttonColour);
    loadButton.setColour(juce::TextButton::textColourOffId, textColour);

    // Mute button state depends on audio, so we just set the 'unmuted' state colors
    if (!playerAudio.GetMuteState()) {
        muteButton.setColour(juce::TextButton::buttonColourId, buttonColour);
        muteButton.setColour(juce::TextButton::textColourOffId, textColour);
    }
    else // Ensure muted state color is also updated
    {
        muteButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
        muteButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    }


    if (!playerAudio.GetRepeatState()) {
        repeatButton.setColour(juce::TextButton::buttonColourId, buttonColour);
        repeatButton.setColour(juce::TextButton::textColourOffId, textColour);
    }
    else 
    {
        repeatButton.setColour(juce::TextButton::buttonColourId, accentColour);
        repeatButton.setColour(juce::TextButton::textColourOffId, bgColour);
    }


    previousButton.setColour(juce::TextButton::buttonColourId, buttonColour);
    previousButton.setColour(juce::TextButton::textColourOffId, textColour);

    nextButton.setColour(juce::TextButton::buttonColourId, buttonColour);
    nextButton.setColour(juce::TextButton::textColourOffId, textColour);

    themeButton.setColour(juce::TextButton::buttonColourId, buttonColour);
    themeButton.setColour(juce::TextButton::textColourOffId, textColour);

    // Sliders
    volumeSlider.setColour(juce::Slider::thumbColourId, accentColour);
    volumeSlider.setColour(juce::Slider::trackColourId, accentColour);
    volumeSlider.setColour(juce::Slider::backgroundColourId, sliderTrackColour);

    speedSlider.setColour(juce::Slider::thumbColourId, accentColour);
    speedSlider.setColour(juce::Slider::trackColourId, accentColour);
    speedSlider.setColour(juce::Slider::backgroundColourId, sliderTrackColour);
    speedSlider.setColour(juce::Slider::textBoxTextColourId, textColour);

    positionSlider.setColour(juce::Slider::trackColourId, accentColour);

    loopStartSlider.setColour(juce::Slider::thumbColourId, accentColour);
    loopStartSlider.setColour(juce::Slider::trackColourId, sliderTrackColour);
    loopStartSlider.setColour(juce::Slider::textBoxTextColourId, textColour);
    loopStartSlider.setColour(juce::Slider::backgroundColourId, sliderTrackColour.darker(0.1f));


    loopEndSlider.setColour(juce::Slider::thumbColourId, accentColour);
    loopEndSlider.setColour(juce::Slider::trackColourId, sliderTrackColour);
    loopEndSlider.setColour(juce::Slider::textBoxTextColourId, textColour);
    loopEndSlider.setColour(juce::Slider::backgroundColourId, sliderTrackColour.darker(0.1f));


    // Labels
    trackInfoLabel.setColour(juce::Label::textColourId, textColour);
    speedLabel.setColour(juce::Label::textColourId, textColour);
    timeLabel.setColour(juce::Label::textColourId, textColour);
    loopStartLabel.setColour(juce::Label::textColourId, textColour);
    loopEndLabel.setColour(juce::Label::textColourId, textColour);

    // ListBox
    playlistBox.setColour(juce::ListBox::backgroundColourId, boxBgColour);
    playlistBox.setColour(juce::ListBox::outlineColourId, sliderTrackColour);
    playlistBox.updateContent(); 

    repaint();
}

void PlayerGUI::resized()
{
    int buttonWidth = 40; //changed from 60 to 40 cuz the theme buttons take more space
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
    themeButton.setBounds(repeatButton.getRight() + margin, topRowY, 110, buttonHeight);

    int secondRowY = topRowY + buttonHeight + margin;
    volumeSlider.setBounds(margin, secondRowY, getWidth() - (margin * 2), 30);

    int thirdRowY = secondRowY + 4 * margin;
    speedSlider.setBounds(margin, thirdRowY, getWidth() - (margin * 2), 30);

    int playlistY = thirdRowY + 20 * margin; // This will be updated later
    int trackInfoHeight = 25;
    trackInfoLabel.setBounds(margin, getHeight() - trackInfoHeight - 5, getWidth() - (margin * 2), trackInfoHeight);
    // This playlistBox.setBounds is overridden later, but we'll leave it for now

    int sliderHeight = 30;

    int fourthRowY = thirdRowY + sliderHeight + margin;
    int timeLabelWidth = 80;


    timeLabel.setBounds(margin, fourthRowY, timeLabelWidth, sliderHeight);

    int positionSliderX = timeLabel.getRight() + margin;

    int positionSliderWidth = getWidth() - positionSliderX - margin;

    positionSlider.setBounds(positionSliderX, fourthRowY, positionSliderWidth, sliderHeight);

    //feature 10 loop section sliders
    int fifthRowY = fourthRowY + sliderHeight + margin;
    int labelWidth = 80;

    loopStartSlider.setBounds(margin + labelWidth, fifthRowY,
        getWidth() - (margin * 2) - labelWidth, sliderHeight);

    int sixthRowY = fifthRowY + sliderHeight + margin;
    loopEndSlider.setBounds(margin + labelWidth, sixthRowY,
        getWidth() - (margin * 2) - labelWidth, sliderHeight);

    // I will change the playlist box position ya shbab 
    playlistY = sixthRowY + sliderHeight + margin; 
    playlistBox.setBounds(margin, playlistY, getWidth() - (margin * 2),
        getHeight() - playlistY - trackInfoHeight - 15);

}
//feature 9

void PlayerGUI::updatePositionSlider()
{
    double currentPosition = playerAudio.getCurrentPosition();
    double totalLength = playerAudio.getTotalLength();

    if (totalLength > 0.0)
    {

        positionSlider.setRange(0.0, totalLength, 0.0);

        positionSlider.setValue(currentPosition, juce::dontSendNotification);


        int currentMinutes = static_cast<int>(currentPosition) / 60;
        int currentSeconds = static_cast<int>(currentPosition) % 60;
        juce::String currentStr = juce::String(currentMinutes) + ":" + juce::String(currentSeconds).paddedLeft('0', 2);

        int totalMinutes = static_cast<int>(totalLength) / 60;
        int totalSeconds = static_cast<int>(totalLength) % 60;
        juce::String totalStr = juce::String(totalMinutes) + ":" + juce::String(totalSeconds).paddedLeft('0', 2);

        timeLabel.setText(currentStr + " / " + totalStr, juce::dontSendNotification);
    }
    else
    {

        positionSlider.setRange(0.0, 1.0, 0.0);
        positionSlider.setValue(0.0, juce::dontSendNotification);
        timeLabel.setText("0:00 / 0:00", juce::dontSendNotification);
    }
}


void PlayerGUI::timerCallback()
{

    updatePositionSlider();
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
    // Local color definitions removed, using member variables now

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
            muteButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
            muteButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
        }
        else {
            muteButton.setButtonText("Unmuted");
            muteButton.setColour(juce::TextButton::buttonColourId, buttonColour);
            muteButton.setColour(juce::TextButton::textColourOffId, textColour); 
        }
    }
    else if (button == &repeatButton) {
        playerAudio.switchrepeat();

        if (playerAudio.GetRepeatState()) {
            repeatButton.setButtonText("is being repeated");
            repeatButton.setColour(juce::TextButton::buttonColourId, accentColour);
            repeatButton.setColour(juce::TextButton::textColourOffId, bgColour);
        }
        else {
            repeatButton.setButtonText("click to repeat");
            repeatButton.setColour(juce::TextButton::buttonColourId, buttonColour); 
            repeatButton.setColour(juce::TextButton::textColourOffId, textColour); 
        }
    }
    else if (button == &themeButton) 
    {
        if (currentTheme == Default)
        {
            currentTheme = Girls;
            themeButton.setButtonText("Theme: Girls");
        }
        else if (currentTheme == Girls)
        {
            currentTheme = Boys;
            themeButton.setButtonText("Theme: Boys");
        }
        else 
        {
            currentTheme = Default;
            themeButton.setButtonText("Theme: Default");
        }
        applyTheme();
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
    //feature 10 update loop slider ranges عشان لما يحمل تراك جديد ال لوب سلايدر يتحدثوا لوحدهم
    double totalLength = playerAudio.getTotalLength();
    if (totalLength > 0)
    {
        loopStartSlider.setRange(0.0, totalLength, 0.1);
        loopEndSlider.setRange(0.0, totalLength, 0.1);
        loopEndSlider.setValue(totalLength);
        playerAudio.setLoopSection(0.0, totalLength);
    }
}

void PlayerGUI::updatePlayPauseButton()
{
    const auto& currentImage = playPauseButton.getNormalImage();

    if (playerAudio.isPlaying() && currentImage != pauseImage)
    {
        playPauseButton.setImages(true, true, true,
            pauseImage, 1.0f, accentColour,
            pauseImage, 0.8f, accentColour,
            pauseImage, 0.5f, accentColour);
    }
    else if (!playerAudio.isPlaying() && currentImage != playImage)
    {
        playPauseButton.setImages(true, true, true,
            playImage, 1.0f, accentColour,
            playImage, 0.8f, accentColour,
            playImage, 0.5f, accentColour);
    }
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
    {
        playerAudio.setGain((float)slider->getValue());
    }

    else if (slider == &speedSlider)
    {
        playerAudio.setPlaybackSpeed((float)slider->getValue());
    }

    else if (slider == &positionSlider)
    {

        if (slider->isMouseButtonDown())
        {
            playerAudio.setPosition(slider->getValue());
        }
    }
    //feature 10
    else if (slider == &loopStartSlider || slider == &loopEndSlider)
    {
        // Ensure start is before end
        if (loopStartSlider.getValue() >= loopEndSlider.getValue())
        {
            if (slider == &loopStartSlider)
                loopStartSlider.setValue(loopEndSlider.getValue() - 0.1, juce::dontSendNotification);
            else
                loopEndSlider.setValue(loopStartSlider.getValue() + 0.1, juce::dontSendNotification);
        }

        playerAudio.setLoopSection(loopStartSlider.getValue(), loopEndSlider.getValue());

        // Update button text if repeat is on
        if (playerAudio.GetRepeatState())
        {
            repeatButton.setButtonText("Loop: " + juce::String(loopStartSlider.getValue(), 1) +
                "s - " + juce::String(loopEndSlider.getValue(), 1) + "s");
        }
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
        g.fillAll(accentColour);
    }

    auto trackTitles = playerAudio.getTrackTitles();
    if (rowNumber >= trackTitles.size())
        return;

    if (rowNumber == playerAudio.getCurrentTrackIndex())
    {
        g.setColour(rowIsSelected ? bgColour : accentColour);
        g.setFont(juce::Font(14.0f, juce::Font::bold));
    }
    else
    {
        g.setColour(rowIsSelected ? bgColour : textColour);
        g.setFont(14.0f);
    }

    g.drawText(trackTitles[rowNumber], 5, 0, width - 10, height, juce::Justification::centredLeft, true);
}

// feature 8 
void PlayerGUI::listBoxItemDoubleClicked(int row, const juce::MouseEvent&)
{
    playerAudio.playTrack(row);
}