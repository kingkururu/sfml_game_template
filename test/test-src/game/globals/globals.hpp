//
//  globals.hpp
//
//

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream> 
#include <sstream>
#include <fstream> 
#include <yaml-cpp/yaml.h>

#include "log.hpp" 

namespace SpriteComponents {
    enum Direction { NONE, LEFT, RIGHT, UP, DOWN };

    Direction toDirection(const std::string& direction); // Declaration only
}

namespace MetaComponents{
    inline sf::Vector2i mouseClickedPosition {}; 

    inline float globalTime {};
    inline float deltaTime {}; 

    extern sf::Clock clock;
    extern sf::View view;
}

/* constant variables declared here */
namespace Constants {
    extern void initialize();

    // make random positions each time
    extern sf::Vector2f makeRandomPosition(); 
    extern void writeRandomTileMap(const std::string& filePath); 

    // load textures, fonts, music, and sound
    extern std::shared_ptr<sf::Uint8[]> createBitmask( const std::shared_ptr<sf::Texture>& texture, const sf::IntRect& rect );
    extern void printBitmaskDebug(const std::shared_ptr<sf::Uint8[]>& bitmask, unsigned int width, unsigned int height);
    extern void loadAssets(); 
    extern void readFromYaml(const std::string& configFile); 
    extern void makeRectsAndBitmasks(); 

    // Game display settings
    inline float SCREEN_SCALE;
    inline unsigned short SCREEN_WIDTH;
    inline unsigned short SCREEN_HEIGHT;
    inline unsigned short FRAME_LIMIT;
    inline std::string GAME_TITLE;
    inline sf::Vector2f VIEW_INITIAL_CENTER;
    inline float VIEW_SIZE_X;
    inline float VIEW_SIZE_Y;
    inline sf::FloatRect VIEW_RECT;

    // Score settings
    inline unsigned short INITIAL_SCORE;

    // Animation settings
    inline float ANIMATION_CHANGE_TIME;
    inline short PASSTHROUGH_OFFSET;

    // Sprite and text settings
    inline unsigned short SPRITE_OUT_OF_BOUNDS_OFFSET;
    inline unsigned short SPRITE_OUT_OF_BOUNDS_ADJUSTMENT;
    inline unsigned short PLAYER_Y_POS_BOUNDS_RUN;

    // Background settings
    inline float BACKGROUND_SPEED;
    inline std::string BACKGROUNDSPRITE_PATH;
    inline std::string BACKGROUNDSPRITE_PATH2;
    inline sf::Vector2f BACKGROUND_POSITION;
    inline sf::Vector2f BACKGROUND_SCALE;
    inline SpriteComponents::Direction BACKGROUND_MOVING_DIRECTION;
    inline std::shared_ptr<sf::Texture> BACKGROUND_TEXTURE = std::make_shared<sf::Texture>();
    inline std::shared_ptr<sf::Texture> BACKGROUND_TEXTURE2 = std::make_shared<sf::Texture>();
  
    // Sprite paths and settings
    inline std::string SPRITE1_PATH;
    inline sf::Vector2f SPRITE1_POSITION;
    inline sf::Vector2f SPRITE1_SCALE;
    inline std::shared_ptr<sf::Texture> SPRITE1_TEXTURE = std::make_shared<sf::Texture>();

    // Button settings
    inline short BUTTON1_INDEXMAX;
    inline std::string BUTTON1_PATH;
    inline sf::Vector2f BUTTON1_POSITION;
    inline sf::Vector2f BUTTON1_SCALE;
    inline std::shared_ptr<sf::Texture> BUTTON1_TEXTURE = std::make_shared<sf::Texture>();
    inline std::vector<sf::IntRect> BUTTON1_ANIMATIONRECTS;
    inline std::vector<std::shared_ptr<sf::Uint8[]>> BUTTON1_BITMASK;

    // Tile settings
    inline std::string TILES_PATH;
    inline unsigned short TILES_ROWS;
    inline unsigned short TILES_COLUMNS;
    inline unsigned short TILES_NUM;
    //////////////////////////////////////////////
    constexpr unsigned short TILES_NUMBER = 60; // for const in tile making
    inline std::array<bool, TILES_NUMBER> TILES_BOOLS {}; 

    inline sf::Vector2f TILES_SCALE;
    inline unsigned short TILE_WIDTH;
    inline unsigned short TILE_HEIGHT;
    inline std::shared_ptr<sf::Texture> TILES_TEXTURE = std::make_shared<sf::Texture>();
    inline std::vector<sf::IntRect> TILES_SINGLE_RECTS;
    inline std::vector<std::shared_ptr<sf::Uint8[]>> TILES_BITMASKS;

    // Tilemap settings
    inline size_t TILEMAP_WIDTH;
    inline size_t TILEMAP_HEIGHT;
    inline std::string TILEMAP_FILEPATH;

    // Text settings
    inline unsigned short TEXT_SIZE;
    inline std::string TEXT_PATH;
    inline std::string TEXT_MESSAGE;
    inline sf::Vector2f TEXT_POSITION;
    inline sf::Color TEXT_COLOR;
    inline std::shared_ptr<sf::Font> TEXT_FONT = std::make_shared<sf::Font>(); 

    // Music settings
    inline std::string BACKGROUNDMUSIC_PATH;
    inline float BACKGROUNDMUSIC_VOLUME;
    inline std::unique_ptr<sf::Music> BACKGROUNDMUSIC_MUSIC = std::make_unique<sf::Music>(); 

    // Sound settings
    inline std::string PLAYERJUMPSOUND_PATH;
    inline float PLAYERJUMPSOUND_VOLUME;
    inline std::shared_ptr<sf::SoundBuffer> PLAYERJUMP_SOUNDBUFF = std::make_shared<sf::SoundBuffer>();
}

// New namespace for flag events
namespace FlagSystem {
    
    struct FlagEvents {
        // game states
        bool gameEnd; 

        // keyboard inputs
        bool wPressed;
        bool aPressed;
        bool sPressed;
        bool dPressed;
        bool bPressed;
        bool spacePressed; 
        bool mouseClicked;

        FlagEvents() : wPressed(false), aPressed(false), sPressed(false), dPressed(false), bPressed(false), spacePressed(false), mouseClicked(false) {}

        // resets every flag
        void resetFlags() {
            gameEnd = wPressed = aPressed = sPressed = dPressed = bPressed = spacePressed = mouseClicked = false;
            log_info("Flags have been successfully reset.");
        }


        // resets keyboard flags only 
        void flagKeyReleased() {
            wPressed = false;
            aPressed = false;
            sPressed = false;
            dPressed = false;
            bPressed = false;
        }
    };

    inline FlagEvents flagEvents; 

    struct SceneEvents {
        bool sceneEnd;
        bool sceneStart; 

        SceneEvents() : sceneEnd(false), sceneStart(false) {} 

        void resetFlags() {
            log_error("failed resetting scene Flags");
            std::memset(this, 0, sizeof(*this));
            log_info("Flags have been successfully reset.");
        }
    };

    struct GameSceneEvents1 {
        bool sceneEnd;
        bool sceneStart; 

        GameSceneEvents1() : sceneEnd(false), sceneStart(true) {} 

        void resetFlags() {
            log_error("failed resetting scene Flags");
            std::memset(this, 0, sizeof(*this));
            log_info("Flags have been successfully reset.");
        }
    };

    inline GameSceneEvents1 gameScene1Flags; 
    inline SceneEvents gameSceneNextFlags;
}
