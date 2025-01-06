//
//  globals.cpp
//  sfmlgame3
//
//

#include "globals.hpp"

namespace MetaComponents {     // single instance throughout the game
    sf::Clock clock;
    sf::View view; 
}   

namespace SpriteComponents {
    // direction for moving background sprite
    Direction toDirection(const std::string& direction) {
        static const std::unordered_map<std::string, Direction> directionMap = {
            {"LEFT", Direction::LEFT},
            {"RIGHT", Direction::RIGHT},
            {"UP", Direction::UP},
            {"DOWN", Direction::DOWN}
        };

        auto it = directionMap.find(direction);
        return (it != directionMap.end()) ? it->second : Direction::NONE;
    }

    // color for text colors
    sf::Color toSfColor(const std::string& color) {
        static const std::unordered_map<std::string, sf::Color> colorMap = {
            {"RED", sf::Color::Red},
            {"GREEN", sf::Color::Green},
            {"BLUE", sf::Color::Blue},
            {"YELLOW", sf::Color::Yellow},
            {"MAGENTA", sf::Color::Magenta},
            {"CYAN", sf::Color::Cyan},
            {"WHITE", sf::Color::White},
            {"BLACK", sf::Color::Black},
            {"TRANSPARENT", sf::Color::Transparent}, 
            {"CUSTOMCOLOR_LIGHTCORAL", sf::Color(240, 128, 128)} // add colors like this 
        };

        auto it = colorMap.find(color);
        return (it != colorMap.end()) ? it->second : sf::Color::Black; // Default to Black if not found
    }
}

/* constant variables defined here */
namespace Constants {
    // make random position function (random from upper right corner)
    sf::Vector2f makeRandomPosition(){
        float xPos = static_cast<float>(SCREEN_WIDTH - std::rand() % static_cast<int>(SCREEN_WIDTH / 2));
        float yPos = 0.0f;
        return sf::Vector2f{ xPos, yPos }; 
    }
    
    void initialize(){
        // for random function
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        
        readFromYaml(std::filesystem::path("src/game/globals/config.yaml"));
        loadAssets();
        makeRectsAndBitmasks(); 
    }

    void readFromYaml(const std::filesystem::path configFile) {
        try{ 
            YAML::Node config = YAML::LoadFile(configFile);

            // Load game display settings
            SCREEN_SCALE = config["screen"]["scale"].as<float>();
            SCREEN_WIDTH = config["screen"]["width"].as<unsigned short>();
            SCREEN_HEIGHT = config["screen"]["height"].as<unsigned short>();
            FRAME_LIMIT = config["screen"]["frame_limit"].as<unsigned short>();
            GAME_TITLE = config["screen"]["title"].as<std::string>();
            VIEW_SIZE_X = config["screen"]["view"]["size_x"].as<float>();
            VIEW_SIZE_Y = config["screen"]["view"]["size_y"].as<float>();
            VIEW_INITIAL_CENTER = {config["screen"]["view"]["initial_center"]["x"].as<float>(),
                                config["screen"]["view"]["initial_center"]["y"].as<float>()};
            VIEW_RECT = { 0.0f, 0.0f, VIEW_SIZE_X, VIEW_SIZE_Y };

            // Load animation settings
            ANIMATION_CHANGE_TIME = config["animation"]["change_time"].as<float>();

            log_info("Succesfuly read yaml file");
        } 
        catch (const YAML::BadFile& e) {
            log_error("Failed to load config file: " + std::string(e.what()));
        } 
        catch (const YAML::Exception& e) {
            log_error("YAML parsing error: " + std::string(e.what()));
        }
    }

    void loadAssets(){ 
        // load textures and sound buffers here
        log_info("\tSuccessfully loaded assets");
    }

    void makeRectsAndBitmasks(){
        //call createBitmask(texture, rect) for sprites
        log_info("\tSuccessfully created rects and bitmasks");
    }

    std::shared_ptr<sf::Uint8[]> createBitmask( const std::shared_ptr<sf::Texture>& texture, const sf::IntRect& rect, const float transparency) {
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
                sf::Color pixelColor = image.getPixel(rect.left + x, rect.top + y);
                unsigned int bitIndex = y * width + x;
                unsigned int byteIndex = bitIndex / 8;
                unsigned int bitPosition = bitIndex % 8;

                // Use transparency threshold if provided, otherwise default to alpha > 128
                if ((transparency > 0.0f && pixelColor.a >= static_cast<sf::Uint8>(transparency * 255)) || 
                    (transparency <= 0.0f && pixelColor.a > 128)) {
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


