#include <JuceHeader.h>
#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
   
    for (auto* btn : { &loadButton, &restartButton, &gotostartButton, &PlayPauseButton,
                   &gotoendButton, &MuteButton, &loopButton, &setAButton, &setBButton,
                   &clearABButton, &abLoopButton, &skipBackButton, &skipForwardButton })
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
	positionSlider.setSliderStyle(juce::Slider::LinearBar);
    addAndMakeVisible(positionSlider);

    timeLabel.setText("00:00 / 00:00", juce::dontSendNotification);
    timeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(timeLabel);
    startTimer(33);

    metadata.setText("No file loaded", juce::dontSendNotification);
    metadata.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(metadata);

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
    
    int buttonY = 180;
    loadButton.setBounds(30, buttonY, 100, 40);
    restartButton.setBounds(140, buttonY, 80, 40);
    gotostartButton.setBounds(230, buttonY, 50, 40);
    PlayPauseButton.setBounds(290, buttonY, 100, 40);
    gotoendButton.setBounds(400, buttonY, 50, 40);
    skipBackButton.setBounds(460, buttonY, 80, 40);
    skipForwardButton.setBounds(550, buttonY, 80, 40);
    MuteButton.setBounds(640, buttonY, 80, 40);
    loopButton.setBounds(730, buttonY, 100, 40);
    volumeSlider.setBounds(20, 10, getWidth() - 40, 30);
    positionSlider.setBounds(20, 50, getWidth() - 40, 30);
    timeLabel.setBounds(20, 80, getWidth() - 40, 20);
	speedSlider.setBounds(20, 100, 150, 30);
    metadata.setBounds(10, 130, getWidth() - 20, 30);
    setAButton.setBounds(30, 230, 60, 40);
    setBButton.setBounds(100, 230, 60, 40);
    clearABButton.setBounds(170, 230, 80, 40);
    abLoopButton.setBounds(260, 230, 100, 40);
    
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

void PlayerGUI::getMetadata(const juce::File& file)
{
    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();

    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));

    if (reader != nullptr)
    {
        juce::String title = reader->metadataValues.getValue("title", "");
        juce::String artist = reader->metadataValues.getValue("artist", "");

        double totalLength = reader->lengthInSamples / reader->sampleRate;
        juce::String durationStr = formatTime(totalLength);

        juce::String info;
        if (title.isNotEmpty() || artist.isNotEmpty())
        { info = title + " - " + artist + " (" + durationStr + ")"; }
        else 
        { info = file.getFileNameWithoutExtension() + " (" + durationStr + ")";}
        metadata.setText(info, juce::dontSendNotification);

    }
    else
    {
        metadata.setText(file.getFileNameWithoutExtension(), juce::dontSendNotification);
        
    }
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
                if (playerAudio.loadFile(files))
                {
                    positionSlider.setValue(0.0, juce::dontSendNotification);
                    getMetadata(files);
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
    else if (button == &setAButton)
    {
        double currentPos = playerAudio.getPostion();
        playerAudio.setPointA(currentPos);
    }
    else if (button == &setBButton)
    {
        double currentPos = playerAudio.getPostion();
        playerAudio.setPointB(currentPos);
    }
    else if (button == &clearABButton)
    {
        playerAudio.clearABPoints();
        isABLooping = false;
        abLoopButton.setButtonText("AB Loop: Off");
    }
    else if (button == &abLoopButton)
    {
        double pointA = playerAudio.getPointA();
        double pointB = playerAudio.getPointB();

        if (pointA >= 0.0 && pointB > pointA)
        {
            isABLooping = !isABLooping;
            playerAudio.setABLoop(isABLooping);
            abLoopButton.setButtonText(isABLooping ? "AB Loop: On" : "AB Loop: Off");
        }
    }
    else if (button == &skipBackButton)
    {
        playerAudio.skipBackward(10.0);
    }
    else if (button == &skipForwardButton)
    {
        playerAudio.skipForward(10.0);
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
