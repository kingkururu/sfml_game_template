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

TileMap::TileMap(const std::vector<std::shared_ptr<Tile>>& tileTypesVector, unsigned int tileMapWidth, unsigned int tileMapHeight, float tileWidth, float tileHeight, const std::string& filePath) 
    : tileMapWidth(tileMapWidth), tileMapHeight(tileMapHeight), tileWidth(tileWidth), tileHeight(tileHeight) {

    try{
        tiles.reserve( tileMapWidth * tileMapHeight ); 

        std::ifstream fileStream(filePath);
        
        if (!fileStream.is_open()) {
            throw std::runtime_error("Unable to open file: " + filePath);
        }

        std::string line;
        while (std::getline(fileStream, line)) {
            std::istringstream lineStream(line);
            unsigned int tileIndex;

            for (unsigned int y = 0; y < tileMapHeight; ++y) {
                for (unsigned int x = 0; x < tileMapWidth; ++x) {
                    if (lineStream >> tileIndex) {
                        if (tileIndex < tileTypesVector.size()) {
                            tiles.push_back(std::make_shared<Tile>(*tileTypesVector[tileIndex]));
                        } else {
                            throw std::out_of_range("Tile index out of bounds: " + std::to_string(tileIndex));
                        }
                    } else {
                        throw std::runtime_error("Error reading tile index from file: " + filePath);
                    }
                }
            }
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
