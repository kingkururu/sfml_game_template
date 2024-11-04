#include "tiles.hpp"

Tile::Tile(sf::Vector2f scale, std::weak_ptr<sf::Texture> texture, sf::IntRect textureRect, std::weak_ptr<sf::Uint8[]> bitmask, bool walkable)
          : scale(scale), texture(texture), textureRect(textureRect), bitmask(bitmask), walkable(walkable) {
    try{
        tileSprite = std::make_shared<sf::Sprite>();

        if (auto sharedTexture = texture.lock()) {
            sf::Vector2u textureSize = sharedTexture->getSize(); 
            if (!textureSize.x || !textureSize.y) {
                throw std::runtime_error("Loaded texture has size 0");
            }

            tileSprite->setTexture(*sharedTexture);
            tileSprite->setScale(scale);
            tileSprite->setTextureRect(textureRect); 
            
            log_info("Tile initialized successfully");

        } else {
            throw std::runtime_error("Tile texture is not available");
        }
    }   
    catch (const std::exception& e){
        log_error(e.what());
    }
}

TileMap::TileMap(std::shared_ptr<Tile>* tileTypesArray, unsigned int tileTypesNumber, unsigned int tileMapWidth, unsigned int tileMapHeight, float tileWidth, float tileHeight, const std::string& filePath) 
    : tileMapWidth(tileMapWidth), tileMapHeight(tileMapHeight), tileWidth(tileWidth), tileHeight(tileHeight) {

    try{
        tiles.reserve( tileMapWidth * tileMapHeight ); 

        std::ifstream fileStream(filePath);
        
        if (!fileStream.is_open()) {
            throw std::runtime_error("Unable to open file: " + filePath);
        }

        std::string line;
        unsigned int currentY = 0; // Track the current row

        while (std::getline(fileStream, line) && currentY < tileMapHeight) {
            std::istringstream lineStream(line);
            std::string tileIndexStr;
            unsigned int currentX = 0; // Track the current column

            while (lineStream >> tileIndexStr && currentX < tileMapWidth) {
                unsigned int tileIndex = std::stoul(tileIndexStr); // Convert to unsigned int
                if (tileIndex < tileTypesNumber) {
                    // Create a copy of the Tile object and set its position
                    auto tile = std::make_shared<Tile>(*tileTypesArray[tileIndex].get());
                    tile->getTileSprite().setPosition(currentX * tileWidth, currentY * tileHeight); // Set the tile's position
                    tiles.emplace_back(tile);
                } else {
                    throw std::out_of_range("Tile index out of bounds: " + std::to_string(tileIndex));
                }
                currentX++; // Increment column index
            }
            currentY++; // Increment row index
        }

        fileStream.close();

        log_info("Tile map initialized successfully");
    }
    catch (const std::exception& e) {
        log_error("Error in updating position: " + std::string(e.what()));
    }
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (const auto& tile : tiles) {
        if (tile) {
            target.draw(tile->getTileSprite(), states);
        }
    }
}
