#include "PlayerAudio.h"

PlayerAudio::PlayerAudio(): resampleSource(&transportSource, false, 2)
{
    formatManager.registerBasicFormats();
   
}

void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
   
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
   
    resampleSource.getNextAudioBlock(bufferToFill);
}

void PlayerAudio::releaseResources()
{
   
    resampleSource.releaseResources();
}

void PlayerAudio::loadFile(const juce::File& file)
{
    // Stop current playback and release old source
    transportSource.stop();
    transportSource.setSource(nullptr);
    readerSource.reset();

    trackTitle.clear();
    trackDuration.clear();

    if (auto* reader = formatManager.createReaderFor(file))
    {
        // Create a new reader source
        readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);

        // Attach it to the transport source
        transportSource.setSource(readerSource.get(), 0, nullptr, reader->sampleRate);

        // --- Metadata Extraction ---
		// get durationnnnnn
        auto durationSeconds = transportSource.getLengthInSeconds();
        if (durationSeconds > 0)
        {
            int minutes = static_cast<int>(durationSeconds) / 60;
            int seconds = static_cast<int>(durationSeconds) % 60;
            trackDuration = juce::String(minutes) + ":" + juce::String(seconds).paddedLeft('0', 2);
        }

        // get titleee and artistsssssss
        const juce::StringPairArray& metadata = reader->metadataValues;
        juce::String title = metadata.getValue("title", {});
        juce::String artist = metadata.getValue("artist", {});

        if (artist.isNotEmpty() && title.isNotEmpty())
            trackTitle = artist + " - " + title;
        else if (title.isNotEmpty())
            trackTitle = title;
        else if (artist.isNotEmpty())
            trackTitle = artist;

        // if there is no data
        if (trackTitle.isEmpty())
            trackTitle = file.getFileNameWithoutExtension();

        play();
		readerSource->setLooping(rp);

   
       
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

//feature 3

void PlayerAudio::SwitchMute()
{
    if (Muted) {

        transportSource.setGain(PreviousVolume);
        Muted = false;
    }
    else
    {
        PreviousVolume = transportSource.getGain();
        transportSource.setGain(0.0f);
        Muted = true;
    }
}
void PlayerAudio::switchrepeat()
{
    rp = !rp;

    if (readerSource)
    {
        readerSource->setLooping(rp);
    }
}

//ft 6
void PlayerAudio::setPlaybackSpeed(double speed)
{
    resampleSource.setResamplingRatio(speed);
}

double PlayerAudio::getPlaybackSpeed()
{
    return resampleSource.getResamplingRatio();
}
//ft 6


bool PlayerAudio::GetRepeatState() const
{
    return rp;
}

bool PlayerAudio::GetMuteState()
{

    return Muted;
}

juce::String PlayerAudio::getTrackTitle() const
{
    return trackTitle;
}

juce::String PlayerAudio::getTrackDuration() const
{
    return trackDuration;
}