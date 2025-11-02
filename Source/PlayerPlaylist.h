#pragma once
#include <JuceHeader.h>

class PlayerPlaylist
{
public:
    PlayerPlaylist() = default;
    ~PlayerPlaylist() = default;

    void addFiles(const juce::Array<juce::File>& newFiles);
    void removeFile(int index);
    void clear();
    int getNumFiles() const;
    juce::File getFile(int index) const;
    juce::File getCurrentFile() const;
    bool next();
    bool previous();
    int getCurrentIndex() const { return currentIndex; }
    void setCurrentIndex(int newIndex);

private:
    juce::Array<juce::File> files;
    int currentIndex = -1;
};