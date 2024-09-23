//
//  game.hpp
//  sfml game template 
//
//  Created by Sunmyoung Yun on 2024/09

/* This is the game.hpp file containing all the declarations for game.cpp's GameManager class. */

#pragma once

#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <stdexcept>

#include <SFML/Graphics.hpp>

#include "scenes.hpp"
#include "constants.hpp"
#include "flags.hpp"
#include "window.hpp"

class GameManager {
public:
    GameManager();
    void runGame();

private:
    /* countTime counts time regardless of the scene 
       handleEventInput taks input from device, such as keyboard, mouse, etc */
    void countTime();
    void handleEventInput();

    /* Game components (private in GameManager) */
    GameWindow mainWindow;
   // sf::RenderWindow window;
    sf::View view; 

    sf::Clock clock;
    float globalTime {};
    float deltaTime {}; 
    
    /* Make more scene here. ex) name it openingScene, scene1, etc */
    std::unique_ptr<Scene> scene;
};

#endif // GAME_HPP
