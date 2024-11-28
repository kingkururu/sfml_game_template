//
//  game.hpp
//  sfml game template 
//

#pragma once

#include <iostream>
#include <stdexcept>

#include <SFML/Graphics.hpp>

#include "scenes.hpp"
#include "globals.hpp"
#include "window.hpp"
#include "log.hpp" 


// can be manipulated for extra functionality in game
class GameManager {
public:
    GameManager();
    void loadScenes(); 
    void runGame();
    void runScenesFlags();

private:
    void countTime(); // countTime counts time regardless of the scene 
    void handleEventInput();

    GameWindow mainWindow;

    std::unique_ptr<Scene> scene1; 

};

