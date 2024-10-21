//
//  sound.cpp
//  sfmlgame3
//
//  Created by Sunmyoung Yun on 2024/08
//

#include "sound.hpp"
/* Sound class constructor, sets the buffer and volume */
SoundClass::SoundClass(std::weak_ptr<sf::SoundBuffer> soundBuffer, float volume)
    : soundBuffer(soundBuffer), sound(std::make_unique<sf::Sound>()), volume(volume) {
    try {
        auto soundBuff = soundBuffer.lock();
        if (!soundBuff) {
            throw std::runtime_error("Failed loading sound buffer");
        }
        sound->setBuffer(*soundBuff);
        sound->setVolume(volume); 

        log_info("Sound initialized successfully");

    } catch (const std::exception& e) {
        log_error(e.what());  // Use spdlog to log the error
        soundBuffer.reset();
        sound.reset(); 
    }
}

/* Music class constructor, takes in and sets music pointer and volume */
MusicClass::MusicClass(std::unique_ptr<sf::Music> musicLoad, float volume)
    : music(std::move(musicLoad)), volume(volume) {
    try {
        if (!music) {
            throw std::runtime_error("Error loading music");
        }
        music->setVolume(volume);
        music->setLoop(true);  
        //music->play();

        log_info("Music initialized successfully, volume is " + std::to_string(volume));  // Log music volume using spdlog

    } catch (const std::exception& e) {
        log_error(e.what());  // Use spdlog to log the error
        music.reset();  
    }
}

/* Sets new volume for sound */
void SoundClass::setVolume(float newVolume) {
    volume = newVolume;

    if (sound) {
        sound->setVolume(volume); 
        log_info("Sound volume set to " + std::to_string(volume));  // Log volume change
    }
}

/* Sets new volume for music */
void MusicClass::setVolume(float newVolume) {
    volume = newVolume;

    if (music) {
        music->setVolume(volume); 
        log_info("Music volume set to " + std::to_string(volume));  // Log volume change
    }
}