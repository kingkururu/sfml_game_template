//
//  constants.hpp
//  sfmlgame3
//
//  Created by Sunmyoung Yun on 2024/08
//


#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream> 
#include <unordered_map>

/* constant variables declared here */
namespace Constants {
    // make random positions each time
    extern sf::Vector2f makeRandomPosition(); 

    // load textures, fonts, music, and sound
    extern void initialize(); 
    extern std::shared_ptr<sf::Uint8[]> createBitmask( const std::shared_ptr<sf::Texture>& texture, const sf::IntRect& rect );
    extern void printBitmaskDebug(const std::shared_ptr<sf::Uint8[]>& bitmask, unsigned int width, unsigned int height);
    
    // basic game components
    constexpr unsigned short SCREEN_WIDTH = 1920 * .5;
    constexpr unsigned short SCREEN_HEIGHT = 1080 * .5;
    constexpr unsigned short FRAME_LIMIT = 60;
    constexpr const char* GAME_TITLE = "SFML game template";

    // score components
    constexpr unsigned short INITIAL_SCORE = 0;

    // basic animation standards
    constexpr float ANIMATION_CHANGE_TIME = 0.1f;
    constexpr short PASSTHROUGH_OFFSET = 65; 

    // typical screen buffers for sprites and texts
    /*
    constexpr unsigned short SPRITE_OUT_OF_BOUNDS_OFFSET = 110;
    constexpr unsigned short SPRITE_OUT_OF_BOUNDS_ADJUSTMENT = 100;
    constexpr unsigned short PLAYER_Y_POS_BOUNDS_RUN = SCREEN_HEIGHT - SPRITE_OUT_OF_BOUNDS_ADJUSTMENT; 
    */
    
    // typical sprite components (non-static) / animated
    /*
    extern const sf::Vector2f OBSTACLE_POSITION;
    extern const sf::Vector2f OBSTACLE_SCALE;
    extern std::vector<sf::IntRect> OBSTACLESPRITE_RECTS;
    constexpr const char* OBSTACLESPRITE_PATH = "src/assets/sprites/png/Obstacle.png";
    extern std::shared_ptr<sf::Texture> OBSTACLE_TEXTURE;
    constexpr unsigned short OBSTACLEANIM_MAX_INDEX = 5; 
    constexpr float OBSTACLE_SPEED = 200.0f;
    constexpr float OBSTACLE_ACCELERATION = 1.6;
    constexpr float OBSTACLE_INITIAL_RESPAWN_TIME = 3.0; 
    constexpr float OBSTACLE_INTERVAL_DECREMENT = 0.08; 
    constexpr short OBSTACLE_FALL_ANGLE = 150; 
    extern std::vector<std::shared_ptr<sf::Uint8[]>> OBSTACLE_BITMASKS;
    */

    // typical text components
    /*
    extern const sf::Vector2f TEXT_POSITION;
    constexpr unsigned short TEXT_SIZE = 40;
    extern const sf::Color TEXT_COLOR;
    constexpr const char* TEXT_PATH = "src/assets/fonts/ttf/font1.ttf";
    extern std::shared_ptr<sf::Font> TEXT_FONT; 
    constexpr const char* TEXT_MESSAGE = "current score: ";
    */

    // typical music components 
    /*
    constexpr const char* BACKGROUNDMUSIC_PATH = "src/assets/sound/mp3/bgm.mp3";
    extern std::unique_ptr<sf::Music> BACKGROUNDMUSIC_MUSIC;
    constexpr float BACKGROUNDMUSIC_VOLUME = 100.0f; 
    */
   
    // typical sound components
    /* 
    constexpr const char* PLAYERJUMPSOUND_PATH = "src/assets/sound/mp3,flac,wav/jump.wav";
    extern std::shared_ptr<sf::SoundBuffer> PLAYERJUMP_SOUNDBUFF;
    constexpr float PLAYERJUMPSOUND_VOLUME = 100.0f; 
    */

}
