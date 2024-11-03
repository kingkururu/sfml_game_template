//
//  globals.hpp
//  sfmlgame3
//
//  Created by Sunmyoung Yun on 2024/08
//

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream> 

#include "log.hpp" 

namespace SpriteComponents {
    enum Direction {
        NONE = 0, 
        LEFT = 1,
        RIGHT = -1,
        UP = -2,
        DOWN = 2
    };
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

    constexpr float ANIMATION_CHANGE_TIME = 0.3; 
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
