#include "AudioMixer.h"

AudioMixer::AudioMixer(PlayerAudio& p1, PlayerAudio& p2)
    : player1(p1), player2(p2)
{
   
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
}

void AudioMixer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void AudioMixer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

void AudioMixer::releaseResources()
{
    mixerSource.releaseResources();
    player1.releaseResources();
    player2.releaseResources();
}

void AudioMixer::setPlayer1Volume(float volume)
{
   
    player1.getTransportSource().setGain(volume);
}

void AudioMixer::setPlayer2Volume(float volume)
{
   
    player2.getTransportSource().setGain(volume);
}
