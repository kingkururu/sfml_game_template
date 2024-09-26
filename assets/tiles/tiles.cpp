#include "tiles.hpp"

Tile::Tile(sf::Vector2f position, sf::Vector2f scale, std::weak_ptr<sf::Texture> texture, sf::IntRect textureRect, bool walkable)
          : position(position), scale(scale), texture(texture), textureRect(textureRect), walkable(walkable) {
    if (auto sharedTexture = texture.lock()) {
        spriteCreated = std::make_unique<sf::Sprite>(*sharedTexture, textureRect);
        spriteCreated->setPosition(position);
        spriteCreated->setScale(scale);
    }
}

void Tile::setPosition(sf::Vector2f newPos) { 
        position = newPos; 
        spriteCreated->setPosition(position);
}

// functions for tilemaps 
void TileMap::addTile(unsigned int x, unsigned int y, std::shared_ptr<Tile> tile) {
    if (x < width && y < height) {
        tiles[y * width + x] = tile;
    }
}

std::shared_ptr<Tile> TileMap::getTile(unsigned int x, unsigned int y) const {
    if (x < width && y < height) {
        return tiles[y * width + x];
    }
    return nullptr;
}

void TileMap::loadTileMap(const std::shared_ptr<sf::Texture>& texture, const std::vector<int>& map, const sf::Vector2f& scale, sf::Vector2u tileSize) {
    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            int tileNumber = map[y * width + x];

            // Calculate texture coordinates based on the tile number
            int tu = tileNumber % (texture->getSize().x / tileSize.x);
            int tv = tileNumber / (texture->getSize().x / tileSize.x);

            sf::IntRect textureRect(tu * tileSize.x, tv * tileSize.y, tileSize.x, tileSize.y);
            sf::Vector2f position(static_cast<float>(x) * tileWidth, static_cast<float>(y) * tileHeight);

            // Create and add the tile
            auto tile = std::make_shared<Tile>(position, scale, texture, textureRect);
            addTile(x, y, tile);
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