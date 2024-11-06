//
//  tilemap.hpp
//
//

#pragma once

#include <stdio.h>
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <sstream>

#include "../../test-logging/log.hpp"


class Tile {
public:
    // Constructor with position, scale, texture, and a texture rect (to support tilesets)
    explicit Tile(sf::Vector2f scale, std::weak_ptr<sf::Texture> texture, sf::IntRect textureRect, std::weak_ptr<sf::Uint8[]> bitmask, bool walkable = true); 
    
    sf::Sprite& getTileSprite() const { return *tileSprite; } 

    sf::IntRect const getTextureRect() const { return textureRect; }
    std::weak_ptr<sf::Uint8[]>  const getBitMask() const { return bitmask; }
 
    bool getWalkable() const { return walkable; }
    void setWalkable(bool newWalkable) { walkable = newWalkable; }
    
    // making copies for use in tilemap
    virtual std::unique_ptr<Tile> clone() const {
        return std::unique_ptr<Tile>(new Tile(*this)); // Create a new instance with the same properties
    }
    virtual ~Tile() {} 
    Tile(const Tile& other); 

private:
    sf::Vector2f position {};
    std::unique_ptr<sf::Sprite> tileSprite {};
    sf::Vector2f scale {};
    std::weak_ptr<sf::Texture> texture;
    sf::IntRect textureRect {};   // Texture portion for this tile
    std::weak_ptr<sf::Uint8[]> bitmask; 
    bool walkable {};
};

class TileMap : public sf::Drawable {
public:
    // Constructor now accepts a shared_ptr to a default tile, and initializes the map with it
    explicit TileMap(std::shared_ptr<Tile>* tileTypesArray, unsigned int tileTypesNumber, size_t tileMapWidth, size_t tileMapHeight, float tileWidth, float tileHeight, std::filesystem::path filePath, sf::Vector2f tileMapPosition);
    ~TileMap() = default;
    
    // Add a tile to the map at the specified grid position (x, y)
    void addTile(unsigned int x, unsigned int y, std::unique_ptr<Tile> tile); 
    float const getTileWidth() const { return tileWidth; }
    float const getTileHeight() const { return tileHeight; }
    size_t const getTileMapWidth() const { return tileMapWidth; }
    size_t const getTileMapHeight() const { return tileMapHeight; }
    sf::Vector2f const getTileMapPosition() const { return tileMapPosition; }

private:
    unsigned int tilesArraySize {};
    size_t tileMapWidth{};
    size_t tileMapHeight{}; 

    float tileWidth {};
    float tileHeight {};

    std::vector<std::unique_ptr<Tile>> tiles; 
    sf::Vector2f tileMapPosition; 

    // Override the draw function of sf::Drawable to draw all tiles
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
