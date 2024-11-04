#include "tiles.hpp"

Tile::Tile(sf::Vector2f scale, std::weak_ptr<sf::Texture> texture, sf::IntRect textureRect, std::weak_ptr<sf::Uint8[]> bitmask, bool walkable)
          : scale(scale), texture(texture), textureRect(textureRect), bitmask(bitmask), walkable(walkable) {
    try{
        tileSprite = std::make_unique<sf::Sprite>();

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

// void Tile::setPosition(sf::Vector2f newPos) { 
//     position = newPos; 
//     spriteCreated->setPosition(position);
// }

// TileMap::TileMap(unsigned int width, unsigned int height, float tileWidth, float tileHeight, std::shared_ptr<Tile> defaultTile) 
//     : width(width), height(height), tileWidth(tileWidth), tileHeight(tileHeight), tiles(width * height) {

//   // Initialize the tile map with the default tile at each position
//     for (unsigned int y = 0; y < height; ++y) {
//         for (unsigned int x = 0; x < width; ++x) {
//         addTile(x, y, defaultTile);
//         }
//     }

//     log_info("Tile map initialized successfully");

// }

// void TileMap::addTile(unsigned int x, unsigned int y, std::shared_ptr<Tile> tile) {
//     if (x < width && y < height) {
//         tiles[y * width + x] = tile;
//     } else {
//         throw std::out_of_range("Tile position out of bounds");
//     }
// }

// std::shared_ptr<Tile> TileMap::getTile(unsigned int x, unsigned int y) const {
//     if (x < width && y < height) {
//         return tiles[y * width + x];
//     }
//     throw std::out_of_range("Tile position out of bounds");
// }

// void TileMap::loadTileMap(const std::vector<int>& map, const std::vector<std::shared_ptr<Tile>>& tileSet) {
//     if (map.size() != width * height) {
//         throw std::invalid_argument("Map size does not match TileMap dimensions");
//     }

//     for (unsigned int y = 0; y < height; ++y) {
//         for (unsigned int x = 0; x < width; ++x) {
//             int tileIndex = map[y * width + x];
//             if (tileIndex >= 0 && static_cast<size_t>(tileIndex) < tileSet.size()) {
//                 // Replace the corresponding tile in the map with one from the tileSet
//                 addTile(x, y, tileSet[tileIndex]);
//             } else {
//                 throw std::runtime_error("Invalid tile index in map");
//             }
//         }
//     }
// }

// void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
//     for (const auto& tile : tiles) {
//         if (tile) {
//             target.draw(tile->getTileSprite(), states);
//         }
//     }
// }

// void TileMap::loadTileMapFromFile(const std::string& filename, const std::vector<std::shared_ptr<Tile>>& tileSet) {
//     std::ifstream file(filename);
//     if (!file.is_open()) {
//         throw std::runtime_error("Unable to open file: " + filename);
//     }

//     std::vector<int> map;
//     std::string line;
//     unsigned int row = 0;

//     while (std::getline(file, line) && row < height) {
//         std::istringstream ss(line);
//         int tileIndex;
//         unsigned int col = 0;

//         while (ss >> tileIndex && col < width) {
//             map.push_back(tileIndex);
//             ++col;
//         }

//         if (col != width) {
//             throw std::runtime_error("Invalid number of columns in row " + std::to_string(row) + " of the tile map file.");
//         }
//         ++row;
//     }

//     if (map.size() != width * height) {
//         throw std::runtime_error("Map size does not match TileMap dimensions");
//     }

//     // Load the tile map with the parsed data
//     loadTileMap(map, tileSet);
// }