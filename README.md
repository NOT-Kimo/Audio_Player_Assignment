# Audio Player Assignment


This repository contains a feature-rich desktop audio player application built with the C++ JUCE framework. It provides a comprehensive set of controls for audio playback, playlist management, and advanced features like A-B looping and session persistence. The application supports running two independent player instances simultaneously.

## Features

*   **Dual Player Interface**: Toggle a second, fully independent audio player on the same screen.
*   **Core Playback Controls**: Play, pause, restart, and seek to the beginning or end of a track.
*   **Advanced Playback**:
    *   **Variable Speed**: Adjust playback speed from 0.25x to 4.0x.
    *   **Standard Loop**: Loop the entire track.
    *   **A-B Loop**: Define and loop a specific segment of the audio.
    *   **Quick Skip**: Jump forward or backward by 10-second intervals.
*   **Volume and Muting**: control the output volume and mute/unmute the audio.
*   **Playlist Management**:
    *   Load `.wav` and `.mp3` files.
    *   Navigate through the playlist with next and previous buttons.
    *   View tracks in a playlist box and select them for playback.
    *   Clear the entire playlist.
*   **Track Markers**:
    *   Add markers at specific timestamps in a track.
    *   Select a marker to jump directly to that position.
    *   Delete unwanted markers.
*   **Session Persistence**: The application automatically saves the last played file and its position, resuming your session when you reopen it.
*   **Sleep Timer**: Set a timer (1 to 60 minutes) to automatically stop playback.
*   **Metadata Display**: Shows track information like title, artist, and duration.
*   **Visual Feedback**: A position slider displays the current playback progress, and a time label shows the current and total duration.

## Architecture

The application is structured into several key components using the JUCE framework:

*   **`MainComponent`**: The top-level component that manages the layout and audio processing for the two player instances. It mixes the audio output from both players.
*   **`PlayerGUI`**: Encapsulates the entire user interface for a single audio player, including all buttons, sliders, labels, and list boxes. It listens for user interactions and communicates them to the `PlayerAudio` engine.
*   **`PlayerAudio`**: The core audio engine responsible for file loading, transport control (play/stop/seek), volume/speed adjustments, and looping logic. It uses `juce::AudioTransportSource` for playback control.
*   **`PlayerPlaylist`**: A helper class to manage the collection of audio files, including adding, removing, and navigating tracks.
*   **`Main`**: The application's entry point, which creates and manages the main application window.

## Getting Started

### Prerequisites
*   A C++ compiler and build environment (e.g., Visual Studio 2022, Xcode, or a Linux toolchain).
*   The [JUCE Framework](https://juce.com/get-juce/).

### Building from Source

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/not-kimo/audio_player_assignment.git
    cd audio_player_assignment
    ```

2.  **Set up JUCE:**
    The project is configured to find the JUCE modules in a directory parallel to the project directory (e.g., `../JUCE/modules`). Your directory structure should look like this:

    ```
    YourWorkspace/
    ├── JUCE/
    │   └── modules/
    └── audio_player_assignment/
        └── NewProject.jucer
    ```

    If your JUCE installation is elsewhere, open `NewProject.jucer` in the Projucer and update the module paths under the "Exporters" tab.

3.  **Generate Build Files:**
    *   Open `NewProject.jucer` with the JUCE Projucer application.
    *   Click the "Save Project" button or select your IDE from the "Exporters" tab and save. This will generate the necessary project files (e.g., a Visual Studio Solution) in the `Builds/` directory.

4.  **Compile and Run:**
    *   Navigate to the generated build directory (e.g., `Builds/VisualStudio2022/`).
    *   Open the solution file (e.g., `NewProject.sln`).
    *   Build and run the project from your IDE.


    ## 📄 License

This project is created for educational purposes as part of a university assignment.

---

**Course**: CS213
**Institution**: Faculty of Computers and Artificial Intelligence Cairo University
**Academic Year**: 2024
