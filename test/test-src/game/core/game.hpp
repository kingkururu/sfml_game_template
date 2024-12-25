//
//  game.hpp
//  sfml game template 
//

/* This is the game.hpp file containing all the declarations for game.cpp's GameManager class. */

#pragma once

#include <iostream>
#include <stdexcept>

#include <SFML/Graphics.hpp>

#include "../camera/window.hpp"
#include "../scenes/scenes.hpp"
#include "../globals/globals.hpp"
#include "../test-logging/log.hpp"

class GameManager {
public:
    GameManager();
    void loadScenes(); 
    void runGame();
    void runScenesFlags();
    void resetFlags(); 
    
private:
    void countTime(); // countTime counts time regardless of the scene 
    void handleEventInput(); // handleEventInput taks input from device, such as keyboard, mouse, etc */

    GameWindow mainWindow;

    std::unique_ptr<introScene> introScreenScene; 
    std::unique_ptr<gamePlayScene> gameScene;
    std::unique_ptr<gamePlayScene2> gameSceneNext; 

};

