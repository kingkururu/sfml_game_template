//
//  globals.cpp
//  sfmlgame3
//
//  Created by Sunmyoung Yun on 2024/08
//

#include "globals.hpp"

namespace MetaComponents {
    sf::Clock clock;
    sf::View view; 
}   

/* constant variables defined here */
namespace Constants {
    // make random position function (random from upper right corner)
    sf::Vector2f makeRandomPosition(){
        float xPos = static_cast<float>(SCREEN_WIDTH - std::rand() % static_cast<int>(SCREEN_WIDTH / 2));
        float yPos = 0.0f;
        return sf::Vector2f{ xPos, yPos }; 
    }
    
    //initializer function
    void initialize() {
        init_logging();
        
        setup_signal_handlers();

        log_info("\tConstants initialized ");

    }

    std::shared_ptr<sf::Uint8[]> createBitmask( const std::shared_ptr<sf::Texture>& texture, const sf::IntRect& rect) {
        if (!texture) {
            log_warning("\tfailed to create bitmask ( texture is empty )");
            return nullptr;
        }

        // Ensure the rect is within the bounds of the texture
        sf::Vector2u textureSize = texture->getSize();
        if (rect.left < 0 || rect.top < 0 || 
            rect.left + rect.width > static_cast<int>(textureSize.x) || 
            rect.top + rect.height > static_cast<int>(textureSize.y)) {
            log_warning("\tfailed to create bitmask ( rect is out of bounds)");
            return nullptr;
        }

        sf::Image image = texture->copyToImage();
        unsigned int width = rect.width;
        unsigned int height = rect.height;

        unsigned int bitmaskSize = (width * height) / 8 + ((width * height) % 8 != 0); // rounding up
        std::shared_ptr<sf::Uint8[]> bitmask(new sf::Uint8[bitmaskSize](), std::default_delete<sf::Uint8[]>());

         for (unsigned int y = 0; y < height; ++y) {
            for (unsigned int x = 0; x < width; ++x) {
                sf::Color pixelColor = image.getPixel(x, y);
                unsigned int bitIndex = y * width + x;
                unsigned int byteIndex = bitIndex / 8;
                unsigned int bitPosition = bitIndex % 8;

                if (pixelColor.a > 128) {
                    bitmask[byteIndex] |= (1 << bitPosition);
                }
            }
        }

        return bitmask; 
    }

    void printBitmaskDebug(const std::shared_ptr<sf::Uint8[]>& bitmask, unsigned int width, unsigned int height) {
        unsigned int bitmaskSize = (width * height + 7) / 8;
        
        std::stringstream bitmaskStream; // Use a stringstream to accumulate the bitmask output

        for (unsigned int i = 0; i < bitmaskSize; ++i) {
            for (int bit = 7; bit >= 0; --bit) { // Print bits from high to low
                bitmaskStream << ((bitmask[i] & (1 << bit)) ? '1' : '0');
            }
            if ((i + 1) % (width / 8) == 0) { // New line after each row
                bitmaskStream << std::endl;
            }
        }
        
        // Log the accumulated bitmask
        log_info(bitmaskStream.str());
    }
}


