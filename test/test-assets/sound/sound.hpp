//
//  sound.hpp
//  sfmlgame3
//
//
#pragma once

#include <SFML/Audio.hpp>
#include <memory>
#include <string>
#include <stdexcept>
#include <iostream>

#include "../../test-logging/log.hpp"


class SoundClass{
public:
    explicit SoundClass(std::weak_ptr<sf::SoundBuffer> soundBuffer, float volume);
    sf::Sound& returnSound() { return *sound; }
    const sf::Sound& returnSound() const { return *sound; }
    ~SoundClass() = default; 
    void setVolume(float volume);
    float const getVolume() const { return volume; } 

protected:
    std::weak_ptr<sf::SoundBuffer> soundBuffer;
    std::unique_ptr<sf::Sound> sound;
    float volume = 100.0f; 
};

class MusicClass {
public:
    MusicClass(std::unique_ptr<sf::Music> musicLoad, float volume);
    sf::Music& returnMusic() { return *music; }
    void setVolume(float volume); 
    float const getVolume() { return volume; } 

private:
    std::unique_ptr<sf::Music> music;
    float volume{};
};

