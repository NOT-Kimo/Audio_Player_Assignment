#include <JuceHeader.h>
#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
   
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

    positionSlider.setRange(0.0, 1.0, 0.001);
    positionSlider.setValue(0.0);
    positionSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    positionSlider.addListener(this);
    addAndMakeVisible(positionSlider);
    timeLabel.setText("00:00 / 00:00", juce::dontSendNotification);
    timeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(timeLabel);
    startTimer(33);

	speedSlider.setRange(0.25, 4.0, 0.01);
	speedSlider.setSkewFactorFromMidPoint(1.0);
	speedSlider.setValue(1.0);
	speedSlider.addListener(this);
    addAndMakeVisible(speedSlider);
}

PlayerGUI::~PlayerGUI() {
    stopTimer();
}


void PlayerGUI::resized()
{
    
    int buttonY = 115;
    loadButton.setBounds(20, buttonY, 100, 40);
    restartButton.setBounds(130, buttonY, 80, 40);
    gotostartButton.setBounds(220, buttonY, 50, 40);
    PlayPauseButton.setBounds(280, buttonY, 100, 40);
    gotoendButton.setBounds(390, buttonY, 50, 40);
    MuteButton.setBounds(450, buttonY, 80, 40);
    loopButton.setBounds(540, buttonY, 100, 40);
    volumeSlider.setBounds(20, 10, getWidth() - 40, 30);
    positionSlider.setBounds(20, 50, getWidth() - 40, 30);
    timeLabel.setBounds(20, 85, getWidth() - 40, 20);
	speedSlider.setBounds(20, 100, getWidth() - 40, 30);

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
void PlayerGUI::timerCallback()
{
    double currentPos = playerAudio.getPostion();
    double totalLength = playerAudio.getTotalLength();

    if (totalLength > 0.0)
    {
        
        if (!positionSlider.isMouseButtonDown())
        {
            isUserDragging = false;
            positionSlider.setValue(currentPos / totalLength, juce::dontSendNotification);
        }

        
        timeLabel.setText(formatTime(currentPos) + " / " + formatTime(totalLength),
            juce::dontSendNotification);
    }
}
juce::String PlayerGUI::formatTime(double seconds)
{
    int totalSeconds = static_cast<int>(seconds);
    int minutes = totalSeconds / 60;
    int secs = totalSeconds % 60;

    return juce::String::formatted("%02d:%02d", minutes, secs);
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
                    positionSlider.setValue(0.0, juce::dontSendNotification);
                   
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
        bool isMuted = playerAudio.mute(volumeSlider.getValue());
        MuteButton.setButtonText(isMuted ? " UNMUTE " : " MUTE ");
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
    else if (slider == &positionSlider)
    {
        // User is dragging or clicked on the slider
        isUserDragging = true;
        double totalLength = playerAudio.getTotalLength();
        double newPosition = positionSlider.getValue() * totalLength;
        playerAudio.setPosition(newPosition);
        timeLabel.setText(formatTime(newPosition) + " / " + formatTime(totalLength),
            juce::dontSendNotification);
    }
    else if (slider == &speedSlider)
    {
		playerAudio.setSpeed(speedSlider.getValue());
    }
}