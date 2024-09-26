//
//  fonts.hpp
//  sfml game template
//
//  Created by Sunmyoung Yun on 2024/00
//
#pragma once

#ifndef fonts_hpp
#define fonts_hpp

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <iostream> 
#include <stdexcept>
#include "constants.hpp"

/* text class declaration */
class TextClass : public sf::Drawable {
public:
    explicit TextClass(sf::Vector2f position, unsigned int size, sf::Color color, std::weak_ptr<sf::Font> font, const std::string& testMessage);
    sf::Text& getText() { return *text; }
   
    const sf::Text& getText() const { return *text; } // now unnecessary 

    ~TextClass() = default;
    bool const getVisibleState() const { return visibleState; }
    void setVisibleState(bool VisibleState){ visibleState = VisibleState; }
    void updateText(const std::string& newText); 
    unsigned int getSize() const { return size; }
    void setSize(int newSize){ text->setCharacterSize(newSize); }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override { if (visibleState && text) target.draw(*text, states); }

private:
    sf::Vector2f position {};
    unsigned int size {};
    sf::Color color {};
    std::weak_ptr<sf::Font> font; 
    std::unique_ptr<sf::Text> text;
    bool visibleState = true;
};


#endif /* fonts_hpp */
