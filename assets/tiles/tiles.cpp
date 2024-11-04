#include "tiles.hpp"

Tile::Tile(sf::Vector2f position, sf::Vector2f scale, std::weak_ptr<sf::Texture> texture, sf::IntRect textureRect, bool walkable)
          : position(position), scale(scale), texture(texture), textureRect(textureRect), walkable(walkable), spriteCreated(std::make_unique<sf::Sprite>()) {
    try{
        if (auto sharedTexture = texture.lock()) {
            sf::Vector2u textureSize = sharedTexture->getSize(); 
            if (!textureSize.x || !textureSize.y) {
                throw std::runtime_error("Loaded texture has size 0");
            }

            spriteCreated->setTexture(*sharedTexture);
            spriteCreated->setPosition(position);
            spriteCreated->setScale(scale);
            spriteCreated->setTextureRect(textureRect); 
            

        } else {
            throw std::runtime_error("Tile texture is not available");
        }
    }   
    catch (const std::exception& e){
        log_error(e.what());
    }
}

void Tile::setPosition(sf::Vector2f newPos) { 
        position = newPos; 
        spriteCreated->setPosition(position);
}

TileMap::TileMap(unsigned int width, unsigned int height, float tileWidth, float tileHeight, std::shared_ptr<Tile> defaultTile) 
    : width(width), height(height), tileWidth(tileWidth), tileHeight(tileHeight), tiles(width * height) {

  // Initialize the tile map with the default tile at each position
    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
        addTile(x, y, defaultTile);
        }
    }

    log_info("Tile map initialized successfully");

}

void TileMap::addTile(unsigned int x, unsigned int y, std::shared_ptr<Tile> tile) {
    if (x < width && y < height) {
        tiles[y * width + x] = tile;
    } else {
        throw std::out_of_range("Tile position out of bounds");
    }
}

std::shared_ptr<Tile> TileMap::getTile(unsigned int x, unsigned int y) const {
    if (x < width && y < height) {
        return tiles[y * width + x];
    }
    throw std::out_of_range("Tile position out of bounds");
}

void TileMap::loadTileMap(const std::vector<int>& map, const std::vector<std::shared_ptr<Tile>>& tileSet) {
    if (map.size() != width * height) {
        throw std::invalid_argument("Map size does not match TileMap dimensions");
    }

    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            int tileIndex = map[y * width + x];
            if (tileIndex >= 0 && static_cast<size_t>(tileIndex) < tileSet.size()) {
                // Replace the corresponding tile in the map with one from the tileSet
                addTile(x, y, tileSet[tileIndex]);
            } else {
                throw std::runtime_error("Invalid tile index in map");
            }
        }
    }
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (const auto& tile : tiles) {
        if (tile) {
            target.draw(tile->getTileSprite(), states);
        }
    }
}

