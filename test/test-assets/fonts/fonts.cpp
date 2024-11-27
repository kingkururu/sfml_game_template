//
//  fonts.cpp
//  sfml game template
//
//

#include "fonts.hpp"

// text class constructor, sets up color, size, font, position, text message 
TextClass::TextClass(sf::Vector2f position, unsigned int size, sf::Color color, std::weak_ptr<sf::Font> font, const std::string& testMessage)
    : position(position), size(size), color(color), font(font), text(std::make_unique<sf::Text>()) {

    try {  
        auto fontptr = font.lock();  
        if (!fontptr) {
            throw std::runtime_error("Font failed to load");
        }
        text->setFont(*fontptr);
        text->setCharacterSize(size);
        text->setFillColor(color);
        text->setPosition(position);
        text->setString(testMessage);
        
        log_info("text initialized successully");
    } 
    catch(const std::exception& e) {
        log_error(e.what());  // Log error using spdlog
        visibleState = false;
    }
}

// change message inside text 
void TextClass::updateText(const std::string& newText) {
    if (text) {
        text->setString(newText); 
    } else {
        log_warning("Text not initialized");  // Log warning using spdlog
    }
}