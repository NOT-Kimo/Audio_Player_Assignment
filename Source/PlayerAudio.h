#pragma once
#include "JuceHeader.h"

class PlayerAudio  : public juce::Component
{
public:
    PlayerAudio();
    ~PlayerAudio() override;
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();
    bool loadFile(const juce::File& file);
    void play();
    void stop();
    void restart();
    void gostart();
    void goend();
    void setGain(float gain);
    void setPosition(double position);
    double getPostion() const;
    double getGain() const;
    double getTotalLength() const;
    bool mute(double last_gain);
    bool loop();
    bool isPlaying();


private:
	juce::AudioFormatManager formatManager;
	std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
	juce::AudioTransportSource transportSource;
    bool isLooping = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlayerAudio)
};
