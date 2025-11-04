#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"

class AudioMixer : public juce::AudioSource
{
public:
    AudioMixer(PlayerAudio& player1, PlayerAudio& player2);
    ~AudioMixer() override = default;

   
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

   
    void setPlayer1Volume(float volume);
    void setPlayer2Volume(float volume);

private:
    PlayerAudio& player1;
    PlayerAudio& player2;

    juce::MixerAudioSource mixerSource;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioMixer)
};
