//
//  globals.hpp
//  sfmlgame3
//
//

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream> 
#include <sstream>
#include <yaml-cpp/yaml.h>
#include <filesystem>

#include "log.hpp" 


namespace SpriteComponents {
    enum Direction { NONE, LEFT, RIGHT, UP, DOWN };

    Direction toDirection(const std::string& direction); // convert string from yaml to Direction
    sf::Color toSfColor(const std::string& color); // convert string from yaml to sf::Color
}

namespace MetaComponents{
    inline sf::Vector2i mouseClickedPosition {}; 

    inline float globalTime {};
    inline float deltaTime {}; 

    extern sf::Clock clock;
    extern sf::View view;
}

namespace Constants {
    extern void initialize();

    // make random positions each time
    extern sf::Vector2f makeRandomPosition(); 

    // load textures, fonts, music, and sound
    extern std::shared_ptr<sf::Uint8[]> createBitmask( const std::shared_ptr<sf::Texture>& texture, const sf::IntRect& rect, const float transparencey = 0.0f);
    extern void printBitmaskDebug(const std::shared_ptr<sf::Uint8[]>& bitmask, unsigned int width, unsigned int height);
    extern void loadAssets(); 
    extern void readFromYaml(std::filesystem::path configFile);  
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
    
}
