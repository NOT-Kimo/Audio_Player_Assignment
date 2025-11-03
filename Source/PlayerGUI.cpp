#include <JuceHeader.h>
#include "PlayerGUI.h"
#include "PlayerPlaylist.h"


PlayerGUI::PlayerGUI()
{
   
    for (auto* btn : { &loadButton, &restartButton, &gotostartButton, &PlayPauseButton,
                   &gotoendButton, &MuteButton, &loopButton, &setAButton, &setBButton,
                   &clearABButton, &abLoopButton, &skipBackButton, &skipForwardButton,
                   &nextButton, &prevButton })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }
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
    
    positionSlider.setColour(juce::Slider::trackColourId, juce::Colour(0xff1e90ff));
    positionSlider.setColour(juce::Slider::backgroundColourId, juce::Colour());
    positionSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xff1e90ff));

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

    addAndMakeVisible(playlistBox);
    playlistBox.setModel(this);

    sleepTimerButton.addListener(this);
    addAndMakeVisible(sleepTimerButton);
    sleepTimerCombo.addItem("Off", 1);
    sleepTimerCombo.addItem("1 minute", 2);
    sleepTimerCombo.addItem("5 minutes", 3);
    sleepTimerCombo.addItem("10 minutes", 4);
    sleepTimerCombo.addItem("15 minutes", 5);
    sleepTimerCombo.addItem("30 minutes", 6);
    sleepTimerCombo.addItem("45 minutes", 7);
    sleepTimerCombo.addItem("60 minutes", 8);
    sleepTimerCombo.setSelectedId(1);
    addAndMakeVisible(sleepTimerCombo);
}

PlayerGUI::~PlayerGUI() {
    stopTimer();
}


void PlayerGUI::resized()
{
    int buttonY = 180;
    volumeSlider.setBounds(20, 10, getWidth() - 40, 30);

    positionSlider.setBounds(20, 50, getWidth() - 40, 30);
    timeLabel.setBounds(20, 80, getWidth() - 40, 20);

    speedSlider.setBounds(20, 100, 150, 30);

    metadata.setBounds(10, 130, getWidth() - 20, 30);

    loadButton.setBounds(30, buttonY, 100, 40);
    restartButton.setBounds(140, buttonY, 80, 40);
    gotostartButton.setBounds(230, buttonY, 50, 40);
    PlayPauseButton.setBounds(290, buttonY, 100, 40);
    gotoendButton.setBounds(400, buttonY, 50, 40);
    skipBackButton.setBounds(460, buttonY, 80, 40);
    skipForwardButton.setBounds(550, buttonY, 80, 40);
    MuteButton.setBounds(640, buttonY, 80, 40);
    loopButton.setBounds(730, buttonY, 100, 40);

    setAButton.setBounds(30, 230, 60, 40);
    setBButton.setBounds(100, 230, 60, 40);
    clearABButton.setBounds(170, 230, 80, 40);
    abLoopButton.setBounds(260, 230, 100, 40);

    playlistBox.setBounds(850, 100 , getWidth() - 40, 150);
    prevButton.setBounds(930, 260, 100 , 30);
    nextButton.setBounds(1050, 260, 100 , 30);

    sleepTimerButton.setBounds(600, 250, 100, 40);
    sleepTimerCombo.setBounds(710, 250, 120, 40);

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

    if (totalLength > 0.0){
        if (!positionSlider.isMouseButtonDown())
        {
            isUserDragging = false;
            positionSlider.setValue(currentPos / totalLength, juce::dontSendNotification);
        }

        timeLabel.setText(formatTime(currentPos) + " / " + formatTime(totalLength),
            juce::dontSendNotification);



        if (currentPos >= totalLength - 0.05 )
        {
            if (isLooping)
            {
                playerAudio.setPosition(0.0);
                playerAudio.play();
                updatePlayPauseButton();
            }
            else if (playlist.getNumFiles() > 1)
            {
                if (playlist.next())
                {
                    auto file = playlist.getCurrentFile();
                    if (file.existsAsFile())
                    {
                        playerAudio.loadFile(file);
                        playerAudio.play();
                        getMetadata(file);
                        updatePlayPauseButton();
                        playlistBox.selectRow(playlist.getCurrentIndex());
                    }
                }
            }
        }
    }
    
    if (sleepTimerActive)
    {
        if (juce::Time::getCurrentTime() >= sleepTimerEndTime)
        {
            playerAudio.stop();
            sleepTimerActive = false;
            sleepTimerCombo.setSelectedId(1);
            sleepTimerButton.setButtonText("Sleep Timer");
            updatePlayPauseButton();
        }
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
                    playlist.addFiles({ files });
                    playlistBox.updateContent(); 
                    playlistBox.selectRow(playlist.getNumFiles() - 1);
                }
            });
        updatePlayPauseButton();
    }
    else if (button == &restartButton)
    {
        playerAudio.restart();
        updatePlayPauseButton();
    }
    else if (button == &gotostartButton)
    {
        playerAudio.gostart();
        playerAudio.stop();
        updatePlayPauseButton();
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
        updatePlayPauseButton();
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
        updatePlayPauseButton();
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
    else if (button == &nextButton)
    {
        if (playlist.next())
        {
            auto file = playlist.getCurrentFile();
            if (file.existsAsFile())
            {
                playerAudio.loadFile(file);
                playerAudio.play();
                getMetadata(file);
                updatePlayPauseButton();
                playlistBox.selectRow(playlist.getCurrentIndex());
            }
        }
    }
    else if (button == &prevButton)
    {
        if (playlist.previous())
        {
            auto file = playlist.getCurrentFile();
            if (file.existsAsFile())
            {
                playerAudio.loadFile(file);
                playerAudio.play();
                getMetadata(file);
                updatePlayPauseButton();
                playlistBox.selectRow(playlist.getCurrentIndex());
            }
        }
    }
    else if (button == &sleepTimerButton)
    {
        int selectedId = sleepTimerCombo.getSelectedId();

        if (selectedId == 1 || sleepTimerActive)
        {
            sleepTimerActive = false;
            sleepTimerButton.setButtonText("Sleep Timer");
        }
        else
        {
            int minutes = 0;
            switch (selectedId)
            {
            case 2: minutes = 1; break;
            case 3: minutes = 5; break;
            case 4: minutes = 10; break;
            case 5: minutes = 15; break;
            case 6: minutes = 30; break;
            case 7: minutes = 45; break;
            case 8: minutes = 60; break;
            }

            if (minutes > 0)
            {
                sleepTimerActive = true;
                sleepTimerEndTime = juce::Time::getCurrentTime() + juce::RelativeTime::minutes(minutes);
                sleepTimerButton.setButtonText("Timer: " + juce::String(minutes) + "m");
            }
        }
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
void PlayerGUI::updatePlayPauseButton()
{
    if (playerAudio.isPlaying())
        PlayPauseButton.setButtonText("Pause || ");
    else
        PlayPauseButton.setButtonText("Play > ");
}


int PlayerGUI::getNumRows()
{
    return playlist.getNumFiles();
}

void PlayerGUI::paintListBoxItem(int rowNumber, juce::Graphics& g,
    int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
        g.fillAll(juce::Colours::lightblue);

    auto file = playlist.getFile(rowNumber);
    g.setColour(juce::Colours::white);
    g.drawText(file.getFileName(), 5, 0, width - 10, height, juce::Justification::centredLeft);
}

void PlayerGUI::selectedRowsChanged(int lastRowSelected)
{
    playlist.setCurrentIndex(lastRowSelected);
    auto file = playlist.getCurrentFile();
    if (file.existsAsFile())
    {
        playerAudio.loadFile(file);
        playerAudio.play();
        getMetadata(file);
        PlayPauseButton.setButtonText("Pause || ");
    }
}

