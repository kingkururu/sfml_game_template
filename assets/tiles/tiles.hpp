//
//  tilemap.hpp
//  sfml game template
//
//  Created by Sunmyoung Yun on 2024/09
//

#pragma once

#ifndef tiles_hpp
#define tiles_hpp

#include <stdio.h>
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "constants.hpp"

class Tile {
public:
    // Constructor with position, scale, texture, and a texture rect (to support tilesets)
    explicit Tile(sf::Vector2f position, sf::Vector2f scale, std::weak_ptr<sf::Texture> texture, sf::IntRect textureRect, bool walkable = true); 

    ~Tile() = default;

    sf::Sprite& getTileSprite() { return *spriteCreated; }
    sf::Vector2f getPosition() const { return position; }
    void setPosition(sf::Vector2f newPos);
    
    bool isWalkable() const { return walkable; }
    void setWalkable(bool newWalkable) { walkable = newWalkable; }

private:
    sf::Vector2f position {};
    sf::Vector2f scale {};
    std::weak_ptr<sf::Texture> texture;
    sf::IntRect textureRect {};   // Texture portion for this tile
    std::unique_ptr<sf::Sprite> spriteCreated;
    bool walkable;                // Whether the tile is walkable (for pathfinding, collision, etc.)
};

// class TileMap : public sf::Drawable {
// public:
//     explicit TileMap(unsigned int width, unsigned int height, float tileWidth, float tileHeight)
//         : width(width), height(height), tileWidth(tileWidth), tileHeight(tileHeight) {}

//     // Add a tile to the map at the specified grid position (x, y)
//     void addTile(unsigned int x, unsigned int y, std::shared_ptr<Tile> tile);

//     // Access a tile at a specific grid position
//     std::shared_ptr<Tile> getTile(unsigned int x, unsigned int y) const;

//     // Load the entire tilemap
//     void loadTileMap(const std::shared_ptr<sf::Texture>& texture, const std::vector<int>& map, const sf::Vector2f& scale, sf::Vector2u tileSize);

// private:
//     unsigned int width;
//     unsigned int height;
//     float tileWidth;
//     float tileHeight;
//     std::vector<std::shared_ptr<Tile>> tiles;

//     // Override the draw function of sf::Drawable to draw all tiles
//     virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
// };

class TileMap : public sf::Drawable {
public:
    // Constructor now accepts a shared_ptr to a default tile, and initializes the map with it
    explicit TileMap(unsigned int width, unsigned int height, float tileWidth, float tileHeight, std::shared_ptr<Tile> defaultTile)
        : width(width), height(height), tileWidth(tileWidth), tileHeight(tileHeight), tiles(width * height) {

        // Initialize the tile map with the default tile at each position
        for (unsigned int y = 0; y < height; ++y) {
            for (unsigned int x = 0; x < width; ++x) {
                addTile(x, y, defaultTile);
            }
        }
    }

    // Add a tile to the map at the specified grid position (x, y)
    void addTile(unsigned int x, unsigned int y, std::shared_ptr<Tile> tile) {
        if (x < width && y < height) {
            tiles[y * width + x] = tile;
        } else {
            throw std::out_of_range("Tile position out of bounds");
        }
    }

    // Access a tile at a specific grid position
    std::shared_ptr<Tile> getTile(unsigned int x, unsigned int y) const {
        if (x < width && y < height) {
            return tiles[y * width + x];
        }
        throw std::out_of_range("Tile position out of bounds");
    }

    // Load the entire tilemap by modifying the existing tiles
    void loadTileMap(const std::vector<int>& map, const std::vector<std::shared_ptr<Tile>>& tileSet) {
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

private:
    unsigned int width;
    unsigned int height;
    float tileWidth;
    float tileHeight;
    std::vector<std::shared_ptr<Tile>> tiles;  // Store tiles as shared pointers

    // Override the draw function of sf::Drawable to draw all tiles
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        for (const auto& tile : tiles) {
            if (tile) {
                target.draw(tile->getTileSprite(), states);
            }
        }
    }
};


#endif /* tiles_hpp */
