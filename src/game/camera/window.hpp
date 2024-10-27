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
#include "log.hpp" 

class GameWindow{
public: 
    GameWindow( unsigned int screenWidth, unsigned int screenHeight, std::string gameTitle, unsigned int frameRate );
    sf::RenderWindow& getWindow() { return window; } 

    explicit operator bool() const {
        return window.getSize().x > 0 && window.getSize().y > 0;
    }

private:
    sf::RenderWindow window;
};

class GameView{
public:
    GameView(sf::FloatRect viewRect);
    sf::View& getView() { return view; } 
    void setSceneView(const sf::View& NewView) { view = NewView; }
    explicit operator bool() const {
        return view.getSize().x > 0 && view.getSize().y > 0; // Checks if the view has a valid size
    }

private:
    sf::View view; 

};

#endif /* window_hpp */
