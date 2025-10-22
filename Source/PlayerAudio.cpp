#include <JuceHeader.h>
#include "PlayerAudio.h"

PlayerAudio::PlayerAudio()
{
	formatManager.registerBasicFormats();
}

PlayerAudio::~PlayerAudio()
{
	releaseResources();
}

void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    transportSource.getNextAudioBlock(bufferToFill);
    if (isLooping && !transportSource.isPlaying() && transportSource.getLengthInSeconds() > 0)
    {
        transportSource.setPosition(0.0);
        transportSource.start();
    }
}

void PlayerAudio::releaseResources()
{
    transportSource.releaseResources();
}


bool PlayerAudio::loadFile(const juce::File& file)
{
    auto* reader = formatManager.createReaderFor(file);
    if (reader != nullptr)
    {
        // ?? Disconnect old source first
        transportSource.stop();
        transportSource.setSource(nullptr);
        readerSource.reset();

        // Create new reader source
        readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);

        // Attach safely
        transportSource.setSource(readerSource.get(),
            0,
            nullptr,
            reader->sampleRate);
        transportSource.start();
        return true;
    }
    return false;

}
void PlayerAudio::play()
{
    transportSource.start();
}
void PlayerAudio::stop()
{
    transportSource.stop();
}
void PlayerAudio::restart()
{
    transportSource.setPosition(0.0);
    transportSource.start();
}
void PlayerAudio::gostart()
{
    transportSource.setPosition(0.0);
}
void PlayerAudio::goend()
{
    double len = transportSource.getLengthInSeconds();
    transportSource.setPosition(len);
}
void PlayerAudio::setGain(float gain)
{
    transportSource.setGain(gain);
}
void PlayerAudio::setPosition(double position)
{
    transportSource.setPosition(position);
}
double PlayerAudio::getPostion() const
{
    return transportSource.getCurrentPosition();
}
double PlayerAudio::getGain() const
{
    return transportSource.getGain();
}
bool PlayerAudio::mute(double last_gain)
{
    if (transportSource.getGain() > 0)
    {
        transportSource.setGain(0);
        return true;
    }
    else
    {
        transportSource.setGain(last_gain); 
        return false;
    }
}
bool PlayerAudio::loop()
{
    isLooping = !isLooping;
    return isLooping;
}
bool PlayerAudio::isPlaying()
{
    return transportSource.isPlaying();
}