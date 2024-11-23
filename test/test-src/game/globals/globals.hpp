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
#include <filesystem>

#include "../test-logging/log.hpp"
#include "../utils/utils.hpp"

namespace SpriteComponents {
    enum Direction { NONE, LEFT, RIGHT, UP, DOWN };

    Direction toDirection(const std::string& direction); // convert string from yaml to Direction
    sf::Color toSfColor(const std::string& color); // convert string from yaml to sf::Color
}

namespace MetaComponents{
    inline sf::Vector2i mouseClickedPosition_i {}; 
    inline sf::Vector2f mouseClickedPosition_f {}; 

    inline float globalTime {};
    inline float deltaTime {}; 
    inline float spacePressedElapsedTime{};

    extern sf::Clock clock;
    extern sf::View view;

    extern sf::FloatRect getViewBounds();
}

/* constant variables declared here */
namespace Constants { // not actually "constants" in terms of being fixed, but should never be altered after being read from the config.yaml file
    extern void initialize();

    // make random positions each time
    extern sf::Vector2f makeRandomPosition(); 
    extern void writeRandomTileMap(const std::filesystem::path filePath); 

    // load textures, fonts, music, and sound
    extern std::shared_ptr<sf::Uint8[]> createBitmask( const std::shared_ptr<sf::Texture>& texture, const sf::IntRect& rect );
    extern void printBitmaskDebug(const std::shared_ptr<sf::Uint8[]>& bitmask, unsigned int width, unsigned int height);
    extern void loadAssets(); 
    extern void readFromYaml(const std::filesystem::path configFile); 
    extern void makeRectsAndBitmasks(); 

    // Game display settings
    inline float WORLD_SCALE;
    inline unsigned short WORLD_WIDTH;
    inline unsigned short WORLD_HEIGHT;
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
    inline std::filesystem::path BACKGROUNDSPRITE_PATH;
    inline std::filesystem::path BACKGROUNDSPRITE_PATH2;
    inline sf::Vector2f BACKGROUND_POSITION;
    inline sf::Vector2f BACKGROUND_SCALE;
    inline SpriteComponents::Direction BACKGROUND_MOVING_DIRECTION;
    inline std::shared_ptr<sf::Texture> BACKGROUND_TEXTURE = std::make_shared<sf::Texture>();
    inline std::shared_ptr<sf::Texture> BACKGROUND_TEXTURE2 = std::make_shared<sf::Texture>();
  
    // Sprite paths and settings
    inline short SPRITE1_INDEXMAX;
    inline short SPRITE1_ANIMATIONROWS;  
    inline std::filesystem::path SPRITE1_PATH;
    inline sf::Vector2f SPRITE1_POSITION;
    inline sf::Vector2f SPRITE1_SCALE;
    inline sf::Vector2f SPRITE1_JUMP_ACCELERATION;
    inline float SPRITE1_SPEED;
    inline sf::Vector2f SPRITE1_ACCELERATION;
    inline std::shared_ptr<sf::Texture> SPRITE1_TEXTURE = std::make_shared<sf::Texture>();
    inline std::vector<sf::IntRect> SPRITE1_ANIMATIONRECTS;
    inline std::vector<std::shared_ptr<sf::Uint8[]>> SPRITE1_BITMASK;

    // Button settings
    inline short BUTTON1_INDEXMAX;
    inline std::filesystem::path BUTTON1_PATH;
    inline sf::Vector2f BUTTON1_POSITION;
    inline sf::Vector2f BUTTON1_SCALE;
    inline std::shared_ptr<sf::Texture> BUTTON1_TEXTURE = std::make_shared<sf::Texture>();
    inline std::vector<sf::IntRect> BUTTON1_ANIMATIONRECTS;
    inline std::vector<std::shared_ptr<sf::Uint8[]>> BUTTON1_BITMASK;

    // Tile settings
    inline sf::Vector2f TILEMAP_POSITION; 
    inline std::filesystem::path TILES_PATH;
    inline unsigned short TILES_ROWS;
    inline unsigned short TILES_COLUMNS;
    inline unsigned short TILES_NUM;
    //////////////////////////////////////////////
    inline unsigned short const TILES_NUMBER = 60; // const in tile making (need to fix later)
    inline std::array<bool, TILES_NUMBER> TILES_BOOLS; 
    //////////////////////////////////////////////

    inline sf::Vector2f TILES_SCALE;
    inline unsigned short TILE_WIDTH;
    inline unsigned short TILE_HEIGHT;
    inline std::shared_ptr<sf::Texture> TILES_TEXTURE = std::make_shared<sf::Texture>();
    inline std::vector<sf::IntRect> TILES_SINGLE_RECTS;
    inline std::vector<std::shared_ptr<sf::Uint8[]>> TILES_BITMASKS;

    // Tilemap settings
    inline size_t TILEMAP_WIDTH;
    inline size_t TILEMAP_HEIGHT;
    inline float TILEMAP_BOUNDARYOFFSET; 
    inline std::filesystem::path TILEMAP_FILEPATH;

    // Text settings
    inline unsigned short TEXT_SIZE;
    inline std::filesystem::path TEXT_PATH;
    inline std::string TEXT_MESSAGE;
    inline sf::Vector2f TEXT_POSITION;
    inline sf::Color TEXT_COLOR;
    inline std::shared_ptr<sf::Font> TEXT_FONT = std::make_shared<sf::Font>(); 

    // Music settings
    inline std::filesystem::path BACKGROUNDMUSIC_PATH;
    inline float BACKGROUNDMUSIC_VOLUME;
    inline std::unique_ptr<sf::Music> BACKGROUNDMUSIC_MUSIC = std::make_unique<sf::Music>(); 

    // Sound settings
    inline std::filesystem::path PLAYERJUMPSOUND_PATH;
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
            log_info("General game flags reset complete");
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
            log_info("Reset sceneEvents flags");
        }
    };

    struct GameSceneEvents1 {
        bool sceneEnd;
        bool sceneStart; 

        bool playerJumping;
        bool playerFalling; 
        GameSceneEvents1() : sceneEnd(false), sceneStart(true), playerJumping(false), playerFalling(false) {} 

        void resetFlags() {
            log_error("failed resetting scene Flags");
            std::memset(this, 0, sizeof(*this));
            log_info("Reset GameSceneEvents1 flags");
        }
    };

    inline GameSceneEvents1 gameScene1Flags; 
    inline SceneEvents gameSceneNextFlags;
}
