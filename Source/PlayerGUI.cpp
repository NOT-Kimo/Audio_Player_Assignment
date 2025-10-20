#include <JuceHeader.h>
#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
    //volume slider
    for (auto* btn : { &loadButton, &restartButton ,&gotostartButton , &PlayPauseButton , &gotoendButton , &MuteButton, &loopButton })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }
    // Volume slider
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);

}

PlayerGUI::~PlayerGUI() {}


void PlayerGUI::resized()
{
    int y = 20;
    loadButton.setBounds(20, y, 100, 40);
    restartButton.setBounds(140, y, 80, 40);
    gotostartButton.setBounds(240, y, 50, 40);
    PlayPauseButton.setBounds(310, y, 100, 40);
    gotoendButton.setBounds(430, y, 50, 40);
    MuteButton.setBounds(500, y, 80, 40);
    loopButton.setBounds(600, y, 100, 40);

    volumeSlider.setBounds(10, 80, getWidth() - 20, 30);
}

void PlayerGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerGUI::getnextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    playerAudio.getNextAudioBlock(bufferToFill);
}

void PlayerGUI::releaseResources()
{
    playerAudio.releaseResources();
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        juce::FileChooser chooser("Select audio files...",
            juce::File{},
            "*.wav;*.mp3");

        fileChooser = std::make_unique<juce::FileChooser>(
            "Select an audio file...",
            juce::File{},
            "*.wav;*.mp3");
        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& chooser)
            {
                auto files = chooser.getResult();
                if (files.existsAsFile())
                {
                    playerAudio.loadFile(files);
                   
                }
            });
        PlayPauseButton.setButtonText("Pause || ");
    }
    else if (button == &restartButton)
    {
        playerAudio.restart();
        PlayPauseButton.setButtonText("Pause || ");
    }
    else if (button == &gotostartButton)
    {
        playerAudio.gostart();
        playerAudio.stop();
        PlayPauseButton.setButtonText("Play > ");
    }
    else if (button == &PlayPauseButton)
    {
        if (playerAudio.isPlaying())
        {
            playerAudio.stop();
            PlayPauseButton.setButtonText("Play > ");
        }
        else
        {
            playerAudio.play();
            PlayPauseButton.setButtonText("Pause || ");
        }
    }
    else if (button == &gotoendButton)
    {
        playerAudio.goend();
        playerAudio.stop();
        PlayPauseButton.setButtonText("Play > ");
    }
    else if (button == &MuteButton)
    {
        bool isMuted = playerAudio.mute();
        MuteButton.setButtonText(isMuted ? "Unmute" : "Mute");
    }
    else if (button == &loopButton)
    {
        isLooping = !isLooping;
        playerAudio.loop();
        loopButton.setButtonText(isLooping ? "Loop: On" : "Loop: Off");
        PlayPauseButton.setButtonText(isplaying ? "Pause || " : "Play > ");
    }
}
void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
    {
        playerAudio.setGain(volumeSlider.getValue());
    }
}