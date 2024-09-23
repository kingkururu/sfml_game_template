//
//  physics.hpp
//  sfml game template
//
//  Created by Sunmyoung Yun on 2024/09
//
#pragma once

#ifndef window_hpp
#define window_hpp

#include <SFML/Graphics.hpp>

class GameWindow{
public:
    GameWindow( unsigned int screenWidth, unsigned int screenHeight, std::string gameTitle, unsigned int frameRate );
    sf::RenderWindow& getWindow() { return window; } 

private:
    sf::RenderWindow window;
};

class GameView{
public:
    GameView(sf::FloatRect viewRect);
    sf::View& getView() { return view; } 

private:
    sf::View view; 

};

#endif /* window_hpp */
