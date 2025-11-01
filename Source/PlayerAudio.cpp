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
	resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    
    // Handle normal looping
    if (isLooping && readerSource != nullptr)
    {
        auto currentPos = transportSource.getCurrentPosition();
        auto totalLength = transportSource.getLengthInSeconds();

        if (currentPos >= totalLength - 0.01 && transportSource.isPlaying())
        {
            transportSource.setPosition(0.0);
        }
    }
    
    if (readerSource == nullptr) {
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    else if (abLoopEnabled && pointA >= 0.0 && pointB > pointA && readerSource != nullptr)
    {
        auto currentPos = transportSource.getCurrentPosition();

        if (currentPos >= pointB && transportSource.isPlaying())
        {
            transportSource.setPosition(pointA);
        }
        else if (currentPos < pointA)
        {
            transportSource.setPosition(pointA);
        }
    }

    resampleSource.getNextAudioBlock(bufferToFill);
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
    if (isLooping && transportSource.getCurrentPosition() >= transportSource.getLengthInSeconds() - 0.01)
    {
        transportSource.setPosition(0.0);
    }
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
double PlayerAudio::getTotalLength() const
{
    return transportSource.getLengthInSeconds();
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
void PlayerAudio::setSpeed(double speed)
{
   resampleSource.setResamplingRatio (speed);
}
void PlayerAudio::setABLoop(bool enabled)
{
    abLoopEnabled = enabled;
}

void PlayerAudio::setPointA(double position)
{
    pointA = position;
}

void PlayerAudio::setPointB(double position)
{
    pointB = position;
}

void PlayerAudio::clearABPoints()
{
    pointA = -1.0;
    pointB = -1.0;
    abLoopEnabled = false;
}

void PlayerAudio::skipForward(double seconds)
{
    double currentPos = transportSource.getCurrentPosition();
    double totalLength = transportSource.getLengthInSeconds();
    double newPos = juce::jmin(currentPos + seconds, totalLength);
    transportSource.setPosition(newPos);
}

void PlayerAudio::skipBackward(double seconds)
{
    double currentPos = transportSource.getCurrentPosition();
    double newPos = juce::jmax(currentPos - seconds, 0.0);
    transportSource.setPosition(newPos);
}
