#include <JuceHeader.h>
#include "PlayerPlaylist.h"

void PlayerPlaylist::addFiles(const juce::Array<juce::File>& newFiles)
{
    for (auto& file : newFiles)
    {
        if (file.existsAsFile())
        {
            files.add(file);
        }
    }

    if (currentIndex == -1 && files.size() > 0)
    {
        currentIndex = 0;
    }
}

void PlayerPlaylist::removeFile(int index)
{
    if (index >= 0 && index < files.size())
    {
        files.remove(index);

        if (currentIndex >= files.size())
        {
            currentIndex = files.size() - 1;
        }
    }
}

void PlayerPlaylist::clear()
{
    files.clear();
    currentIndex = -1;
}

int PlayerPlaylist::getNumFiles() const
{
    return files.size();
}

juce::File PlayerPlaylist::getFile(int index) const
{
    if (index >= 0 && index < files.size())
    {
        return files[index];
    }
    return {};
}

juce::File PlayerPlaylist::getCurrentFile() const
{
    if (currentIndex >= 0 && currentIndex < files.size())
    {
        return files[currentIndex];
    }
    return {};
}

bool PlayerPlaylist::next()
{
    if (files.isEmpty()) { return false; }

    currentIndex++;
    if (currentIndex >= files.size()) { currentIndex = 0; }
    return true;
}

bool PlayerPlaylist::previous()
{
    if (files.isEmpty()) { return false; }

    currentIndex--;
    if (currentIndex < 0) { currentIndex = files.size() - 1; }

    return true;
}

void PlayerPlaylist::setCurrentIndex(int i)
{
    if (i >= 0 && i < files.size()) { currentIndex = i; }
}


