#include "../core/Game.hpp"
#include <iostream>
#include <thread>
#include <mutex>

// Static member initialization
SDL_AudioDeviceID Game::audioDevice = 0;
std::mutex audioMutex;

// Audio initialization
void Game::initializeAudio() {
    if (audioDevice == 0) {
        SDL_AudioSpec desiredSpec;
        SDL_zero(desiredSpec);
        desiredSpec.freq = 44100;             // Audio frequency
        desiredSpec.format = AUDIO_S16SYS;    // Audio format
        desiredSpec.channels = 2;              // Stereo
        desiredSpec.samples = 4096;            // Buffer size

        // Open audio device
        audioDevice = SDL_OpenAudioDevice(nullptr, 0, &desiredSpec, nullptr, 0);
        if (audioDevice == 0) {
            std::cerr << "Failed to open audio device: " << SDL_GetError() << std::endl;
        }
    }
}

// Play WAV file
void Game::playWav(const char* filename) {
    std::lock_guard<std::mutex> lock(audioMutex);

    // Initialize audio if not done
    if (audioDevice == 0) {
        initializeAudio();
        if (audioDevice == 0) return;  // Exit if audio device failed to initialize
    }

    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8* wavBuffer;

    // Load the WAV file
    if (SDL_LoadWAV(filename, &wavSpec, &wavBuffer, &wavLength) == nullptr) {
        std::cerr << "Could not load WAV: " << SDL_GetError() << std::endl;
        return;
    }

    // Clear any existing audio in the queue
    SDL_ClearQueuedAudio(audioDevice);

    // Queue and play the new audio
    SDL_QueueAudio(audioDevice, wavBuffer, wavLength);
    SDL_PauseAudioDevice(audioDevice, 0); // Start playback

    // Use a separate thread for managing playback time
    std::thread([wavBuffer, wavLength]() {
        float duration = static_cast<float>(wavLength) / (44100 * 2 * (16 / 8));
        SDL_Delay(static_cast<int>(duration * 1000)); // Wait for the duration of the audio
        
        // Stop playback and clear the queue
        SDL_PauseAudioDevice(Game::audioDevice, 1);
        SDL_ClearQueuedAudio(Game::audioDevice);

        // Free the WAV buffer
        SDL_FreeWAV(wavBuffer);
    }).detach(); // Detach the thread to allow it to run independently
}

// Play WAV file in a separate thread
void Game::playWavInThread(const char* filename) {
    std::thread audioThread(&Game::playWav, this, filename);
    audioThread.detach(); // Detach to allow independent execution
}

// Cleanup audio resources
void Game::cleanupAudio() {
    if (audioDevice != 0) {
        SDL_CloseAudioDevice(audioDevice);
        audioDevice = 0; // Reset the audio device ID
    }
}