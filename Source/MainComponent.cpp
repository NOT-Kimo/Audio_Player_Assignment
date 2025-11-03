#include "MainComponent.h"
MainComponent::MainComponent()
{
	addAndMakeVisible(player1);
	addAndMakeVisible(player2);

	addPlayerButton.addListener(this);
	addAndMakeVisible(addPlayerButton);
    player2.setVisible(false);

    setSize(1280, 1000);
    setAudioChannels(0, 2);

}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
	player2.prepareToPlay(samplesPerBlockExpected, sampleRate);

}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
	
	bufferToFill.clearActiveBufferRegion();
    juce::AudioBuffer<float> tempBuffer(bufferToFill.buffer->getNumChannels(), bufferToFill.numSamples);

    juce::AudioSourceChannelInfo tempInfo(&tempBuffer, 0, bufferToFill.numSamples);
    player1.getnextAudioBlock(tempInfo);

    for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
    {
        bufferToFill.buffer->addFrom(channel, bufferToFill.startSample, tempBuffer, channel, 0, bufferToFill.numSamples);
    }

    tempBuffer.clear();
    player2.getnextAudioBlock(tempInfo);

    for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
    {
        bufferToFill.buffer->addFrom(channel, bufferToFill.startSample, tempBuffer, channel, 0, bufferToFill.numSamples);
    }
    
	

}

void MainComponent::releaseResources()
{
    player1.releaseResources();
	player2.releaseResources();

}

void MainComponent::resized()
{
    int buttony = 320;
    addPlayerButton.setBounds(20, buttony, 150, 30);

    player1.setBounds(20, 20, getWidth() - 40, 300);
	player2.setBounds(20, buttony + 50, getWidth() - 40, 300);
}

void MainComponent::buttonClicked(juce::Button* button)
{
    if(button == &addPlayerButton)
    {
		bool isvisible = player2.isVisible();
        player2.setVisible(!isvisible);
        addPlayerButton.setButtonText(isvisible ? "Show Player 2" : "Hide Player 2");
    }
    else 
    {
        player1.buttonClicked(button);
        player2.buttonClicked(button);
    }
}

void MainComponent::sliderValueChanged(juce::Slider* slider)
{
	player1.sliderValueChanged(slider);
	player2.sliderValueChanged(slider);
}
