#pragma once
#include <JuceHeader.h>

class PlayerAudio : public juce::AudioSource
{
public:
    PlayerAudio();

    //================== AudioSource Overrides ==================
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //================== Transport Controls ==================
    void loadFile(const juce::File& file);
    void play();
    void pause();
    void goToStart();
    void goToEnd();
    void setGain(float gain);
    bool isPlaying() const;

    //feature 3  mute button functions
    void SwitchMute();
    bool GetMuteState();
    //feature 4 repeat button functions
    bool GetRepeatState() const;
    void switchrepeat();

    //feture 5

    juce::String getTrackTitle() const;
    juce::String getTrackDuration() const;


    //feature 6 variables

    void setPlaybackSpeed(double speed);
    double getPlaybackSpeed();

private:

    //feature 3 mute button variables
    bool Muted = false;
    float PreviousVolume = 1.0f;


	//feature 4 repeat button variables
    bool rp = false;


    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;

    //feature 6 
    juce::ResamplingAudioSource resampleSource;

    juce::String trackTitle;
    juce::String trackDuration;
  
};