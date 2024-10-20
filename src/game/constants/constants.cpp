//
//  constants.cpp
//  sfmlgame3
//
//  Created by Sunmyoung Yun on 2024/08
//

#include "constants.hpp"

/* constant variables defined here */
namespace Constants {

    // make random position function (random from upper right corner)
    sf::Vector2f makeRandomPosition(){
        float xPos = static_cast<float>(SCREEN_WIDTH - std::rand() % static_cast<int>(SCREEN_WIDTH / 2));
        float yPos = 0.0f;
        return sf::Vector2f{ xPos, yPos }; 
    }

    // declare sprite, text, sound, music components here ( ones without constexpr keywords )
    const sf::FloatRect VIEW_RECT = { 0.0f, 0.0f, 500.0f, 500.0f }; 

    const sf::Vector2f BACKGROUND_POSITION = { 0.0f, 0.0f };
    const sf::Vector2f BACKGROUND_SCALE = { 0.5f, 0.5f };
    const sf::Vector2f VIEW_INITIAL_CENTER = { 300.0f, 300.f }; 
    std::shared_ptr<sf::Texture> BACKGROUND_TEXTURE = std::make_shared<sf::Texture>();
    
    const sf::Vector2f SPRITE1_POSITION = { 0.0f, 0.0f };
    const sf::Vector2f SPRITE1_SCALE = { 0.5f, 0.5f };
    std::shared_ptr<sf::Texture> SPRITE1_TEXTURE = std::make_shared<sf::Texture>();

    const sf::Vector2f TILE1_POSITION = { 0.0f, 0.0f };
    const sf::Vector2f TILE1_SCALE = { 0.5f, 0.5f };
    std::shared_ptr<sf::Texture> TILE1_TEXTURE = std::make_shared<sf::Texture>();
    const sf::IntRect TILE1_RECT = { 1, 1, 2, 2 }; 
    
    std::unique_ptr<sf::Music> BACKGROUNDMUSIC_MUSIC = std::make_unique<sf::Music>(); 
    std::shared_ptr<sf::SoundBuffer> PLAYERJUMP_SOUNDBUFF = std::make_shared<sf::SoundBuffer>();

    const sf::Vector2f TEXT_POSITION = { 0.0f, 0.0f };
    const sf::Color TEXT_COLOR = sf::Color::Green;
    std::shared_ptr<sf::Font> TEXT_FONT = std::make_shared<sf::Font>(); 


    //initializer function
    void initialize() {
        init_logging();
        
        setup_signal_handlers();

        log_info("\tConstants initialized ");

        if (!BACKGROUND_TEXTURE->loadFromFile(BACKGROUNDSPRITE_PATH)) {
            log_warning("Failed to load background texture");
        }

        if (!SPRITE1_TEXTURE->loadFromFile(SPRITE1SPRITE_PATH)) {
            log_warning("Failed to load sprite1 texture");
        }

        if (!TILE1_TEXTURE->loadFromFile(TILE1_PATH)) {
            log_warning("Failed to load tile1 texture");
        }

        if (!BACKGROUNDMUSIC_MUSIC->openFromFile(BACKGROUNDMUSIC_PATH)) {
            log_warning("Failed to load background music");
        }

        if (!PLAYERJUMP_SOUNDBUFF->loadFromFile(PLAYERJUMPSOUND_PATH)) {
            log_warning("Failed to load player jump sound");
        }

        if (!TEXT_FONT->loadFromFile(TEXT_PATH)) {
            log_warning("Failed to load text font");
        }

        /* 
        if (! (some texture or font or sound) -> loadFromFile (some_PATH)) {
            log_warning("\Failed to load ", );
            std::cerr << "Failed to load ... from file: " << some_PATH << std::endl;
        }
         if (! (some music ) -> openFromFile (some_PATH)) {
            std::cerr << "Failed to load ... from file: " << some_PATH << std::endl;
        }
        */
     
        //make rects for animations     
        /*
        for(int i = 0; i < 6; ++i ){
            sprite name_RECTS.emplace_back(sf::IntRect{ 200 * i, 0, 200, 200 }); 
        }
        */

        // make bitmasks
        /*
        for (const auto& rect : some_RECTS ) {
            some_BITMASKS.emplace_back(createBitmask(some_TEXTURE, rect));
        }
        */

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
        
        // Create or get a logger (assuming 'info_logger' is your logger)
        auto logger = spdlog::get("info_logger");
        if (!logger) {
            log_error("Logger not initialized.");
            return;
        }
        
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
        logger->info(bitmaskStream.str());
    }
}


