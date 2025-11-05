#pragma once

#if ! JUCE_MSVC
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wredundant-decls"
#endif

#include <JuceHeader.h>
#include "PlayerAudio.h"

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
    void loadFiles(const juce::Array<juce::File>& files); // feature 8
    void play();
    void pause();
    void goToStart();
    void goToEnd();
    void setGain(float gain);
    bool isPlaying() const;
    juce::AudioTransportSource& getTransportSource();
    juce::ResamplingAudioSource& getResamplingSource();

    // feature 8 
    void playNext();
    void playPrevious();
    void playTrack(int index);

    //feature 3  mute button functions
    void SwitchMute();
    bool GetMuteState();
    //feature 4 repeat button functions
    bool GetRepeatState() const;
    void switchrepeat();

    //feature 5 metadata functions
    juce::String getTrackTitle() const;
    juce::String getTrackDuration() const;


    //feature 6 variables

    void setPlaybackSpeed(double speed);
    double getPlaybackSpeed();


    // feature 8
    juce::StringArray getTrackTitles() const;
    int getNumTracks() const;
    int getCurrentTrackIndex() const;
    bool isFinished() const;

    // feature 9
    double getCurrentPosition() const;
    double getTotalLength() const;
    void setPosition(double newPosition);
    //feature 10 loop section functions
    void setLoopSection(double startTime, double endTime);
    double getLoopStartTime() const;
    double getLoopEndTime() const;

private:
    void loadInternal(const juce::File& file); // feature 8 
    std::unique_ptr<juce::AudioFormatReader> audioReader;
    // feature 8
    juce::Array<juce::File> playlist;
    int currentTrackIndex = -1;

    //feature 3 mute button variables
    bool Muted = false;
    float PreviousVolume = 1.0f;
    //feature 4 repeat button variables
    bool rp = false;
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;

    //feature 5 variables
    juce::String trackTitle;
    juce::String trackDuration;
    //feature 10 loop section variables
    double loopStartTime = 0.0;
    double loopEndTime = 0.0;

    //feature 6 functions
    juce::ResamplingAudioSource resampleSource;


    juce::CriticalSection audioCallbackLock;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};

#if ! JUCE_MSVC
#pragma GCC diagnostic pop
#endif
