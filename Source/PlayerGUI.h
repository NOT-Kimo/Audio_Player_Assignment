#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"
#include "PlayerPlaylist.h"
//==============================================================================
/*
*/
class PlayerGUI : public juce::Component,
                  public juce::Button::Listener,
                  public juce::Slider::Listener,
                  public juce::Timer,
                  public juce::ListBoxModel

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
    PlayerPlaylist playlist;
    
    juce::TextButton loadButton{ "Load" };
    juce::TextButton restartButton{ "Restart" };
    juce::TextButton gotostartButton{ "|<<" };
    juce::TextButton PlayPauseButton{ "Play > " };
    juce::TextButton gotoendButton{ ">>|" };
    juce::TextButton MuteButton{ "MUTE" };
    juce::Label volumelabel;
    juce::Slider volumeSlider;
    juce::TextButton loopButton{ "Loop: Off" };
    juce::Slider positionSlider;
    juce::Label timeLabel;
    juce::Label speedLabel;
	juce::Slider speedSlider;
    juce::Label metadata;
	bool isLooping = false;
    bool isABLooping = false;
    bool isplaying = true;
    bool isUserDragging = false;
    juce::String formatTime(double seconds);
    void getMetadata(const juce::File& file);
    juce::TextButton setAButton{ "Set A" };
    juce::TextButton setBButton{ "Set B" };
    juce::TextButton clearABButton{ "Clear AB" };
    juce::TextButton abLoopButton{ "AB Loop: Off" };
    juce::TextButton skipBackButton{ "Backward 10s" };
    juce::TextButton skipForwardButton{ "Forward 10s" };
    juce::Label playlistLabel;
    juce::ListBox playlistBox{ "Playlist", this };
    juce::TextButton nextButton{ "Next" };
    juce::TextButton prevButton{ "Previous" };
    juce::TextButton clearplaylistButton{ "Clear" };
    juce::TextButton sleepTimerButton{ "Sleep Timer" };
    juce::ComboBox sleepTimerCombo;
    bool sleepTimerActive = false;
    juce::Time sleepTimerEndTime;
    juce::TextButton exitButton{ "X" };

    std::unique_ptr<juce::PropertiesFile> settings;
    juce::File lastLoadedFile;
    void saveLastSession();
    void loadLastSession();

    struct Marker {
        juce::String label;
        double timeInSeconds;
    };
    juce::Array<Marker> markers;

    class MarkerListModel : public juce::ListBoxModel
    {
    public:
        MarkerListModel() : owner(nullptr) {}
        explicit MarkerListModel(class PlayerGUI* o) : owner(o) {}

        int getNumRows() override;
        void paintListBoxItem(int rowNumber, juce::Graphics& g,
                              int width, int height, bool rowIsSelected) override;
        void selectedRowsChanged(int lastRowSelected) override;

    private:
        PlayerGUI* owner;
    };

    juce::Label markersLabel;
    MarkerListModel markerListModel;
    juce::ListBox markerListBox{ "Markers", nullptr };
    juce::TextButton addMarkerButton{ "Add Marker" };
    juce::TextButton deleteMarkerButton{ "Delete Marker" };
    void updateMarkerList();


public:
    void sliderValueChanged(juce::Slider* slider) override;
    void buttonClicked(juce::Button* button) override;
    std::unique_ptr<juce::FileChooser> fileChooser;
    int getNumRows() override;
    void paintListBoxItem(int rowNumber, juce::Graphics& g,
        int width, int height, bool rowIsSelected) override;
    void PlayerGUI::selectedRowsChanged(int lastRowSelected) override;
    void updatePlayPauseButton();


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlayerGUI)
};