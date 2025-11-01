#pragma once
#include "JuceHeader.h"

class PlayerAudio : public juce::Component
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
	void setSpeed(double speed);
    void setABLoop(bool enabled);
    bool isABLoopEnabled() const { return abLoopEnabled; }
    void setPointA(double position);
    void setPointB(double position);
    double getPointA() const { return pointA; }
    double getPointB() const { return pointB; }
    void clearABPoints();
    void skipForward(double seconds);
    void skipBackward(double seconds);


private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
	juce::ResamplingAudioSource resampleSource{ &transportSource, false, 2 };


    bool isLooping = false;
    bool abLoopEnabled = false;
    double pointA = -1.0;
    double pointB = -1.0;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};