#include "PlayerAudio.h"

PlayerAudio::PlayerAudio()
{
    formatManager.registerBasicFormats();
}

void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    transportSource.getNextAudioBlock(bufferToFill);
}

void PlayerAudio::releaseResources()
{
    transportSource.releaseResources();
}

void PlayerAudio::loadFile(const juce::File& file)
{
    // Stop current playback and release old source
    transportSource.stop();
    transportSource.setSource(nullptr);
    readerSource.reset();

    if (auto* reader = formatManager.createReaderFor(file))
    {
        // Create a new reader source
        readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);

        // Attach it to the transport source
        transportSource.setSource(readerSource.get(), 0, nullptr, reader->sampleRate);

        // Start playback
        play();
    }
}

void PlayerAudio::play()
{
        transportSource.start();
}

void PlayerAudio::pause()
{
        transportSource.stop();
}

void PlayerAudio::goToStart()
{
        transportSource.setPosition(0.0);
}

void PlayerAudio::goToEnd()
{
        transportSource.setPosition(transportSource.getLengthInSeconds());
}

void PlayerAudio::setGain(float gain)
{
    transportSource.setGain(gain);
}

bool PlayerAudio::isPlaying() const
{
    return transportSource.isPlaying();
}