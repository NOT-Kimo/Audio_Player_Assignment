#pragma once

#include <JuceHeader.h>
#include <vector>
#include "PlayerGUI.h"

class MainComponent : public juce::AudioAppComponent,
                      public juce::Button::Listener,
                      public juce::Slider::Listener
{
public:
    MainComponent();
    ~MainComponent() override;

    // AudioAppComponent
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void resized() override;

    // Button::Listener
    void buttonClicked(juce::Button* button) override;

    // Slider::Listener
    void sliderValueChanged(juce::Slider* slider) override;

private:
    PlayerGUI player1;
	PlayerGUI player2;

	juce::TextButton addPlayerButton{ "Add Player2" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
