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

#include "log.hpp"

/* constant variables declared here */
namespace Constants {

    // make random positions each time
    extern sf::Vector2f makeRandomPosition(); 

    // load textures, fonts, music, and sound
    extern void initialize(); 
    extern std::shared_ptr<sf::Uint8[]> createBitmask( const std::shared_ptr<sf::Texture>& texture, const sf::IntRect& rect );
    extern void printBitmaskDebug(const std::shared_ptr<sf::Uint8[]>& bitmask, unsigned int width, unsigned int height);

    // basic game components
    constexpr float SCREEN_SCALE = 0.5f;
    constexpr unsigned short SCREEN_WIDTH = 1920 * SCREEN_SCALE;
    constexpr unsigned short SCREEN_HEIGHT = 1080 * SCREEN_SCALE;

    constexpr unsigned short FRAME_LIMIT = 60;

    constexpr const char* GAME_TITLE = "SFML game template";

    constexpr float VIEW_SIZE_X = 1920.0f * SCREEN_SCALE;
    constexpr float VIEW_SIZE_Y = 1080.0f * SCREEN_SCALE;
    inline const sf::FloatRect VIEW_RECT = { 0.0f, 0.0f, VIEW_SIZE_X, VIEW_SIZE_Y };
    inline const sf::Vector2f VIEW_INITIAL_CENTER = { VIEW_SIZE_X / 2.0f, VIEW_SIZE_Y / 2.0f };

    // score components
    constexpr unsigned short INITIAL_SCORE = 0;

    // basic animation standards
    constexpr float ANIMATION_CHANGE_TIME = 0.1f;
    constexpr short PASSTHROUGH_OFFSET = 65; 

    // typical screen buffers for sprites and texts
    constexpr unsigned short SPRITE_OUT_OF_BOUNDS_OFFSET = 110;
    constexpr unsigned short SPRITE_OUT_OF_BOUNDS_ADJUSTMENT = 100;
    constexpr unsigned short PLAYER_Y_POS_BOUNDS_RUN = SCREEN_HEIGHT - SPRITE_OUT_OF_BOUNDS_ADJUSTMENT; 
    
    // background components 
    constexpr float BACKGROUND_SPEED = 35.0;
    constexpr const char* BACKGROUNDSPRITE_PATH = "assets/sprites/png/background_day.png";
    constexpr const char* BACKGROUNDSPRITE_PATH2 = "assets/sprites/png/background_night.png";   
    inline const sf::Vector2f BACKGROUND_POSITION = { 0.0f, 0.0f };
    inline const sf::Vector2f BACKGROUND_SCALE = { 1.0f, 1.0f };
    inline std::shared_ptr<sf::Texture> BACKGROUND_TEXTURE = std::make_shared<sf::Texture>();
    inline std::shared_ptr<sf::Texture> BACKGROUND_TEXTURE2 = std::make_shared<sf::Texture>();

    // sprite components
    constexpr const char* SPRITE1SPRITE_PATH = "assets/sprites/png/Static.png";
    inline const sf::Vector2f SPRITE1_POSITION = { 0.0f, 0.0f };
    inline const sf::Vector2f SPRITE1_SCALE = { 1.0f, 1.0f };
    inline std::shared_ptr<sf::Texture> SPRITE1_TEXTURE = std::make_shared<sf::Texture>();

    constexpr short BUTTON1_INDEXMAX = 6; 
    constexpr const char* BUTTON1_PATH = "assets/sprites/png/Static.png";
    inline const sf::Vector2f BUTTON1_POSITION = { 0.0f, 0.0f };
    inline const sf::Vector2f BUTTON1_SCALE = { 1.0f, 1.0f };
    inline std::shared_ptr<sf::Texture> BUTTON1_TEXTURE = std::make_shared<sf::Texture>();
    inline std::vector<sf::IntRect> BUTTON1_ANIMATIONRECTS;
    inline std::vector<std::shared_ptr<sf::Uint8[]>> BUTTON1_BITMASK;

    // typical tile components 
    constexpr const char* TILE1_PATH = "assets/sprites/png/Player.png";
    constexpr bool TILE1_BOOL = true; 
    inline const sf::Vector2f TILE1_POSITION = { 0.0f, 0.0f };
    inline const sf::Vector2f TILE1_SCALE = { 1.0f, 1.0f };
    inline std::shared_ptr<sf::Texture> TILE1_TEXTURE = std::make_shared<sf::Texture>();
    inline const sf::IntRect TILE1_RECT = { 1, 1, 2, 2 }; 

    // typical tile map componenets
    constexpr float MAP_WIDTH = 1.0f; 
    constexpr float MAP_HEIGHT = 1.0f;  
    constexpr unsigned short TILE_WIDTH = 20;
    constexpr unsigned short TILE_HEIGHT = 20;

    // typical text components
    constexpr unsigned short TEXT_SIZE = 40;
    constexpr const char* TEXT_PATH = "assets/fonts/ttf/font1.ttf";
    constexpr const char* TEXT_MESSAGE = "Some text here";
    inline const sf::Vector2f TEXT_POSITION = { 0.0f, 0.0f };
    inline const sf::Color TEXT_COLOR = sf::Color::Green;
    inline std::shared_ptr<sf::Font> TEXT_FONT = std::make_shared<sf::Font>(); 

    // typical music components 
    constexpr const char* BACKGROUNDMUSIC_PATH = "assets/sound/mp3/bgm.mp3";
    inline std::unique_ptr<sf::Music> BACKGROUNDMUSIC_MUSIC = std::make_unique<sf::Music>(); 
    constexpr float BACKGROUNDMUSIC_VOLUME = 100.0f;
   
    // typical sound components
    constexpr const char* PLAYERJUMPSOUND_PATH = "assets/sound/mp3,flac,wav/jump.wav";
    inline std::shared_ptr<sf::SoundBuffer> PLAYERJUMP_SOUNDBUFF = std::make_shared<sf::SoundBuffer>();
    constexpr float PLAYERJUMPSOUND_VOLUME = 100.0f; 
}
