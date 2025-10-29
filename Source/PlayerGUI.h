#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"
//==============================================================================
/*
*/
class PlayerGUI : public juce::Component,
                  public juce::Button::Listener,
                  public juce::Slider::Listener,
                  public juce::Timer

{
public:
    PlayerGUI();
    ~PlayerGUI() override;
	void resized() override;
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
	void getnextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
	void releaseResources();
    void timerCallback() override;

private:
	PlayerAudio playerAudio;
    
    juce::TextButton loadButton{ "Load" };
    juce::TextButton restartButton{ "Restart" };
    juce::TextButton gotostartButton{ "|<<" };
    juce::TextButton PlayPauseButton{ "Play > " };
    juce::TextButton gotoendButton{ ">>|" };
    juce::TextButton MuteButton{ "MUTE" };
    juce::Slider volumeSlider;
    juce::TextButton loopButton{ "Loop: Off" };
    juce::Slider positionSlider;
    juce::Label timeLabel;
	juce::Slider speedSlider;
	bool isLooping = false;
    bool isplaying = true;
    bool isUserDragging = false;
    juce::String formatTime(double seconds);

    
public:
    void sliderValueChanged(juce::Slider* slider) override;
    void buttonClicked(juce::Button* button) override;
    std::unique_ptr<juce::FileChooser> fileChooser;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlayerGUI)
};
