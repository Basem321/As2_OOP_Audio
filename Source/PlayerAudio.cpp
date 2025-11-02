#include "PlayerAudio.h"

PlayerAudio::PlayerAudio() :resampleSource(&transportSource, false, 2)
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

// feature 8 
void PlayerAudio::loadInternal(const juce::File& file)
{
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

        // get artisttttttttt
        const juce::StringPairArray& metadata = reader->metadataValues;
        juce::String title = metadata.getValue("title", {});
        juce::String artist = metadata.getValue("artist", {});

        if (artist.isNotEmpty() && title.isNotEmpty())
            trackTitle = artist + " - " + title;
        else if (title.isNotEmpty())
            trackTitle = title;
        else if (artist.isNotEmpty())
            trackTitle = artist;

        // 3. If no metadata was found
        if (trackTitle.isEmpty())
            trackTitle = file.getFileNameWithoutExtension();

        play();
        readerSource->setLooping(rp);
    }
}

void PlayerAudio::loadFile(const juce::File& file)
{
    playlist.clear(); // feature 8
    playlist.add(file); // feature 8
    currentTrackIndex = 0; // feature 8
    loadInternal(file); // feature 8
}

// feature 8
void PlayerAudio::loadFiles(const juce::Array<juce::File>& files)
{
    playlist = files;
    currentTrackIndex = 0;

    if (playlist.size() > 0)
    {
        loadInternal(playlist.getUnchecked(0));
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

// feature 8
void PlayerAudio::playNext()
{
    if (playlist.size() > 0)
    {
        currentTrackIndex = (currentTrackIndex + 1) % playlist.size();
        loadInternal(playlist.getUnchecked(currentTrackIndex));
    }
}

// feature 8
void PlayerAudio::playPrevious()
{
    if (playlist.size() > 0)
    {
        currentTrackIndex = (currentTrackIndex - 1 + playlist.size()) % playlist.size();
        loadInternal(playlist.getUnchecked(currentTrackIndex));
    }
}

// feature 8
void PlayerAudio::playTrack(int index)
{
    if (index >= 0 && index < playlist.size())
    {
        currentTrackIndex = index;
        loadInternal(playlist.getUnchecked(currentTrackIndex));
    }
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

//ft 6
void PlayerAudio::setPlaybackSpeed(double speed)
{
    resampleSource.setResamplingRatio(speed);
}

double PlayerAudio::getPlaybackSpeed()
{
    return resampleSource.getResamplingRatio();
}

// feature 8
juce::StringArray PlayerAudio::getTrackTitles() const
{
    juce::StringArray titles;
    for (const auto& file : playlist)
    {
        titles.add(file.getFileNameWithoutExtension());
    }
    return titles;
}

// feature 8
int PlayerAudio::getNumTracks() const
{
    return playlist.size();
}

// feature 8
int PlayerAudio::getCurrentTrackIndex() const
{
    return currentTrackIndex;
}

// feature 8
bool PlayerAudio::isFinished() const
{
    if (transportSource.getLengthInSeconds() > 0)
    {
        return transportSource.getCurrentPosition() >= transportSource.getLengthInSeconds() - 0.01;
    }
    return false;
}