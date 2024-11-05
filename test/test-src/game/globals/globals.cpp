//
//  globals.cpp
//
//

#include "globals.hpp"  

namespace MetaComponents {
    sf::Clock clock;
    sf::View view; 
}   

namespace SpriteComponents {
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
            {"TRANSPARENT", sf::Color::Transparent}
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
        std::srand(static_cast<unsigned int>(std::time(nullptr)));

        init_logging();
        setup_signal_handlers();

        readFromYaml(std::filesystem::path("test/test-src/game/globals/config.yaml"));
        loadAssets();
        makeRectsAndBitmasks(); 
    }

    void readFromYaml(const std::filesystem::path configFile) {
        try{ 
            //globalTimer.Reset();

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

            // Load score settings
            INITIAL_SCORE = config["score"]["initial"].as<unsigned short>(); 

            // Load animation settings
            ANIMATION_CHANGE_TIME = config["animation"]["change_time"].as<float>();
            PASSTHROUGH_OFFSET = config["animation"]["passthrough_offset"].as<short>();

            // Load sprite and text settings
            SPRITE_OUT_OF_BOUNDS_OFFSET = config["sprite"]["out_of_bounds_offset"].as<unsigned short>();
            SPRITE_OUT_OF_BOUNDS_ADJUSTMENT = config["sprite"]["out_of_bounds_adjustment"].as<unsigned short>();
            PLAYER_Y_POS_BOUNDS_RUN = config["sprite"]["player_y_pos_bounds_run"].as<unsigned short>();

            // Load background settings
            BACKGROUND_SPEED = config["background"]["speed"].as<float>();
            BACKGROUNDSPRITE_PATH = config["background"]["textures"]["day_path"].as<std::string>();
            BACKGROUNDSPRITE_PATH2 = config["background"]["textures"]["night_path"].as<std::string>();
            BACKGROUND_POSITION = {config["background"]["position"]["x"].as<float>(),
                                config["background"]["position"]["y"].as<float>()};
            BACKGROUND_SCALE = {config["background"]["scale"]["x"].as<float>(),
                                config["background"]["scale"]["y"].as<float>()};
            BACKGROUND_MOVING_DIRECTION = SpriteComponents::toDirection(config["background"]["moving_direction"].as<std::string>());

            // Load sprite paths and settings
            SPRITE1_PATH = config["sprites"]["sprite1"]["path"].as<std::string>();
            SPRITE1_POSITION = {config["sprites"]["sprite1"]["position"]["x"].as<float>(),
                                config["sprites"]["sprite1"]["position"]["y"].as<float>()};
            SPRITE1_SCALE = {config["sprites"]["sprite1"]["scale"]["x"].as<float>(),
                            config["sprites"]["sprite1"]["scale"]["y"].as<float>()};

            // Load button settings
            BUTTON1_INDEXMAX = config["sprites"]["button1"]["index_max"].as<short>();
            BUTTON1_PATH = config["sprites"]["button1"]["path"].as<std::string>();
            BUTTON1_POSITION = {config["sprites"]["button1"]["position"]["x"].as<float>(),
                                config["sprites"]["button1"]["position"]["y"].as<float>()};
            BUTTON1_SCALE = {config["sprites"]["button1"]["scale"]["x"].as<float>(),
                            config["sprites"]["button1"]["scale"]["y"].as<float>()};

            // Load tile settings
            TILES_PATH = config["tiles"]["path"].as<std::string>();
            TILES_ROWS = config["tiles"]["rows"].as<unsigned short>();
            TILES_COLUMNS = config["tiles"]["columns"].as<unsigned short>();
            TILES_NUM = config["tiles"]["number"].as<unsigned short>();
            TILES_SCALE = {config["tiles"]["scale"]["x"].as<float>(),
                        config["tiles"]["scale"]["y"].as<float>()};
            TILE_WIDTH = config["tiles"]["tile_width"].as<unsigned short>();
            TILE_HEIGHT = config["tiles"]["tile_height"].as<unsigned short>();
            if (config["tiles"]["walkable"]) {
               // TILES_BOOLS.resize(TILES_NUM, false); 
                for (unsigned short i = 0; i < TILES_NUM; ++i) {
                    // Check if the index exists to avoid bad conversion
                    if (config["tiles"]["walkable"].size() > i) {
                        TILES_BOOLS[i] = config["tiles"]["walkable"][i].as<bool>();
                    } else {
                        TILES_BOOLS[i] = false; // Or set a default value
                    }
                }
            }

            // Load tilemap settings
            TILEMAP_WIDTH = config["tilemap"]["width"].as<size_t>();
            TILEMAP_HEIGHT = config["tilemap"]["height"].as<size_t>();
            TILEMAP_FILEPATH = config["tilemap"]["filepath"].as<std::string>();

            // Load text settings
            TEXT_SIZE = config["text"]["size"].as<unsigned short>();
            TEXT_PATH = config["text"]["font_path"].as<std::string>();
            TEXT_MESSAGE = config["text"]["message"].as<std::string>();
            TEXT_POSITION = {config["text"]["position"]["x"].as<float>(),
                            config["text"]["position"]["y"].as<float>()};
            TEXT_COLOR = SpriteComponents::toSfColor(config["text"]["color"].as<std::string>());

            // Load music settings
            BACKGROUNDMUSIC_PATH = config["music"]["background_music"]["path"].as<std::string>();
            BACKGROUNDMUSIC_VOLUME = config["music"]["background_music"]["volume"].as<float>();

            // Load sound settings
            PLAYERJUMPSOUND_PATH = config["sound"]["player_jump"]["path"].as<std::string>();
            PLAYERJUMPSOUND_VOLUME = config["sound"]["player_jump"]["volume"].as<float>();
            
            log_info("Succesfuly read yaml file");
            
            // globalTimer.End("read from yaml file"); 

        } 
        catch (const YAML::BadFile& e) {
            log_error("Failed to load config file: " + std::string(e.what()));
        } 
        catch (const YAML::Exception& e) {
            log_error("YAML parsing error: " + std::string(e.what()));
        }

    }

    void loadAssets(){  // load all sprites textures and stuff across scenes 
        if (!BACKGROUND_TEXTURE->loadFromFile(BACKGROUNDSPRITE_PATH)) log_warning("Failed to load background texture");

        if (!BACKGROUND_TEXTURE2->loadFromFile(BACKGROUNDSPRITE_PATH2)) log_warning("Failed to load background2 texture");
        
        if (!BUTTON1_TEXTURE->loadFromFile(BUTTON1_PATH)) log_warning("Failed to load button texture");

        if (!SPRITE1_TEXTURE->loadFromFile(SPRITE1_PATH)) log_warning("Failed to load sprite1 texture");

        if (!TILES_TEXTURE->loadFromFile(TILES_PATH)) log_warning("Failed to load tiles texture");

        if (!BACKGROUNDMUSIC_MUSIC->openFromFile(BACKGROUNDMUSIC_PATH)) log_warning("Failed to load background music");

        if (!PLAYERJUMP_SOUNDBUFF->loadFromFile(PLAYERJUMPSOUND_PATH)) log_warning("Failed to load player jump sound");

        if (!TEXT_FONT->loadFromFile(TEXT_PATH)) log_warning("Failed to load text font");
    }

    void makeRectsAndBitmasks(){
        BUTTON1_ANIMATIONRECTS.reserve(BUTTON1_INDEXMAX); 
        // make rects for animations     
        for(int i = 0; i < BUTTON1_INDEXMAX; ++i ){
            BUTTON1_ANIMATIONRECTS.emplace_back(sf::IntRect{ 170 * i, 0, 170, 170 }); 
        }

        BUTTON1_BITMASK.reserve(BUTTON1_INDEXMAX); 
        // make bitmasks
        for (const auto& rect : BUTTON1_ANIMATIONRECTS ) {
            BUTTON1_BITMASK.emplace_back(createBitmask(BUTTON1_TEXTURE, rect));
        }

        TILES_SINGLE_RECTS.reserve(TILES_NUMBER); 
        // Populate individual tile rectangles
        for (int row = 0; row < TILES_ROWS; ++row) {
            for (int col = 0; col < TILES_COLUMNS; ++col) {
                TILES_SINGLE_RECTS.emplace_back(sf::IntRect{col * TILE_WIDTH, row * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT});
            }
        }

        TILES_BITMASKS.reserve(TILES_NUMBER); 
        // make bitmasks for tiles 
        for (const auto& rect : TILES_SINGLE_RECTS ) {
            TILES_BITMASKS.emplace_back(createBitmask(TILES_TEXTURE, rect));
        }
        
        log_info("\tConstants initialized ");
    }

    void writeRandomTileMap(const std::filesystem::path filePath) {
        try{
            std::ofstream fileStream(filePath);
        
            if (!fileStream.is_open()) {
                throw std::runtime_error("Unable to open file: " + filePath.string());
            }

            for (unsigned short y = 0; y < TILEMAP_HEIGHT; ++y) {
                for (unsigned short x = 0; x < TILEMAP_WIDTH; ++x) {
                    // Generate a random tile index using rand()
                    unsigned int tileIndex = std::rand() % TILES_NUMBER;
                    fileStream << tileIndex;

                    if (x < TILEMAP_WIDTH - 1) { // Avoid adding extra space at the end of the line
                        fileStream << " ";
                    }
                }
                fileStream << std::endl; // New line after each row
            }
            
            fileStream.close();

            log_info("successfuly made a random tile map"); 
        }
        catch (const std::exception& e){
            log_warning("Error in writing random tile map: " + std::string(e.what()));
        }
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


