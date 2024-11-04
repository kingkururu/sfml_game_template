//
//  tilemap.hpp
//
//

#pragma once

#include <stdio.h>
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "log.hpp"
#include <fstream>
#include <sstream>

class Tile {
public:
    // Constructor with position, scale, texture, and a texture rect (to support tilesets)
    explicit Tile(sf::Vector2f scale, std::weak_ptr<sf::Texture> texture, sf::IntRect textureRect, std::weak_ptr<sf::Uint8[]> bitmask, bool walkable = true); 

    ~Tile() = default;
    
    sf::Sprite& getTileSprite() const { return *tileSprite; } 

    sf::IntRect const getTextureRect() const { return textureRect; }
    std::weak_ptr<sf::Uint8[]>  const getBitMask() const { return bitmask; }
    
    // sf::Vector2f getPosition() const { return position; }
    void setPosition(sf::Vector2f newPos);
    
    bool getWalkable() const { return walkable; }
    void setWalkable(bool newWalkable) { walkable = newWalkable; }

private:
    // sf::Vector2f position {};
    std::unique_ptr<sf::Sprite> tileSprite {};
    sf::Vector2f scale {};
    std::weak_ptr<sf::Texture> texture;
    sf::IntRect textureRect {};   // Texture portion for this tile
    std::weak_ptr<sf::Uint8[]> bitmask; 
    bool walkable {};
};

// class TileMap : public sf::Drawable {
// public:
//     // Constructor now accepts a shared_ptr to a default tile, and initializes the map with it
//     explicit TileMap(unsigned int width, unsigned int height, float tileWidth, float tileHeight, std::shared_ptr<Tile> defaultTile);
//     ~TileMap() = default;

//     // Add a tile to the map at the specified grid position (x, y)
//     void addTile(unsigned int x, unsigned int y, std::shared_ptr<Tile> tile); 

//     // Access a tile at a specific grid position
//     std::shared_ptr<Tile> getTile(unsigned int x, unsigned int y) const; 

//     // Load the entire tilemap by modifying the existing tiles
//     void loadTileMap(const std::vector<int>& map, const std::vector<std::shared_ptr<Tile>>& tileSet);
    
//     // New method to load tile map data from a file
//     void loadTileMapFromFile(const std::string& filename, const std::vector<std::shared_ptr<Tile>>& tileSet);
    
//     float const getTileWidth() const { return tileWidth; }
//     float const getTileHeight() const { return tileHeight; }

// private:
//     unsigned int width;
//     unsigned int height;
//     float tileWidth;
//     float tileHeight;
//     std::vector<std::shared_ptr<Tile>> tiles;  // Store tiles as shared pointers

//     // Override the draw function of sf::Drawable to draw all tiles
//     virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
// };


