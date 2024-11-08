//
//  physics.hpp
//  sfml game template
//
//
#pragma once

#include <type_traits>
#include <iostream>
#include <memory>
#include <vector>
#include <stdexcept>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <functional> 
#include <utility>

#include "../globals/globals.hpp" 
#include "../../test-assets/sprites/sprites.hpp" 
#include "../../test-assets/tiles/tiles.hpp" 

namespace physics{

    // don't use this it's not fixed rn
    class Quadtree {
     public:
        Quadtree(float x, float y, float width, float height, size_t level = 0, size_t maxObjects = 10, size_t maxLevels = 5);
        ~Quadtree(){ clear(); };

        void clear();

        // Modify insert to accept unique_ptr but store raw pointers
        template<typename SpriteType> void insert(std::unique_ptr<SpriteType>& obj) {
            // If no child nodes exist, add the object to this node
            if (nodes.empty()) {
                objects.push_back(obj.get());  // Store the raw pointer (no ownership)
            } else {
                // Check which child node the object belongs to
                for (auto& node : nodes) {
                    if (node->bounds.contains(obj->returnSpritesShape().getPosition())) {
                        node->insert(obj);  // Recursively insert into the right child node
                        return;
                    }
                }
            }
        }

        std::vector<Sprite*> query(const sf::FloatRect& area) const;
        void subdivide();
        bool contains(const sf::FloatRect& bounds) const;

     private:
        size_t maxObjects;
        size_t maxLevels;
        size_t level;
        sf::FloatRect bounds;

    std::vector<Sprite*> objects;
        std::vector<std::unique_ptr<Quadtree>> nodes;
    };

    struct RaycastResult {
        std::vector<float> collisionTimes;
        int counter; 
    };    
    extern RaycastResult cachedRaycastResult; 

    constexpr float gravity = 9.8f;

    // falling objects
    sf::Vector2f freeFall(float speed, sf::Vector2f originalPo);
    sf::Vector2f follow( float speed, sf::Vector2f originalPos, sf::Vector2f acceleration, const sf::Vector2f& direction); 

    // moving player
    sf::Vector2f moveLeft( float speed, sf::Vector2f originalPos, sf::Vector2f acceleration = {1.0f, 1.0f}); 
    sf::Vector2f moveRight( float speed, sf::Vector2f originalPos, sf::Vector2f acceleration = {1.0f, 1.0f});
    sf::Vector2f moveUp( float speed, sf::Vector2f originalPos, sf::Vector2f acceleration = {1.0f, 1.0f});
    sf::Vector2f moveDown( float speed, sf::Vector2f originalPos, sf::Vector2f acceleration = {1.0f, 1.0f});
    sf::Vector2f jump(float& elapsedTime, float speed, sf::Vector2f originalPos, sf::Vector2f acceleration = {0.1f, 0.1f}); 

    template<typename SpriteType, typename MoveFunc>
    void spriteMover(std::unique_ptr<SpriteType>& sprite, const MoveFunc& moveFunc) {
        float speed = sprite->getSpeed(); 
        sf::Vector2f originalPos = sprite->getSpritePos(); 
        sf::Vector2f acceleration = sprite->getAcceleration(); 
        sf::Vector2f direction = sprite->getDirectionVector(); 

        // Handle different types of MoveFunc
        if constexpr (std::is_invocable_v<MoveFunc, float, sf::Vector2f, sf::Vector2f, sf::Vector2f&>){
            sprite->changePosition(moveFunc( speed, originalPos, acceleration, direction)); 
        } else if constexpr (std::is_invocable_v<MoveFunc, float, sf::Vector2f, sf::Vector2f>){
            sprite->changePosition(moveFunc( speed, originalPos, acceleration)); 
        } else if constexpr (std::is_invocable_v<MoveFunc, float, sf::Vector2f>){
            sprite->changePosition(moveFunc( speed, originalPos)); 
        }

        sprite->updatePos();  // Update sprite's position after applying the move function
    }
   
    template<typename SpriteType, typename MoveFunc>
    void spriteMover(std::unique_ptr<SpriteType>& sprite, const MoveFunc& moveFunc, float& elapsedTime) {
        float speed = sprite->getSpeed(); 
        sf::Vector2f originalPos = sprite->getSpritePos(); 
        sf::Vector2f acceleration = sprite->getAcceleration();
        
        // Handle different types of MoveFunc
        if constexpr (std::is_invocable_v<MoveFunc, float&, float, sf::Vector2f, sf::Vector2f>){
            sprite->changePosition(moveFunc(MetaComponents::spacePressedElapsedTime, speed, originalPos, acceleration)); 
        }
        sprite->updatePos();  // Update sprite's position after applying the move function
    }

    //circle-shaped sprite collision
    bool circleCollision(const sf::Vector2f pos1, float radius1, const sf::Vector2f pos2, float radius2);
    //raycast pre-collision
    bool raycastPreCollision(const sf::Vector2f obj1position, const sf::Vector2f obj1direction, float obj1Speed, const sf::FloatRect obj1Bounds, sf::Vector2f obj1Acceleration, 
                            const sf::Vector2f obj2position, const sf::Vector2f obj2direction, float obj2Speed, const sf::FloatRect obj2Bounds, sf::Vector2f obj2Acceleration);
    //axis aligned bounding box collision
    bool boundingBoxCollision(const sf::Vector2f &position1, const sf::Vector2f &size1, const sf::Vector2f &position2, const sf::Vector2f &size2);
    //pixel perfect collision
    bool pixelPerfectCollision( const std::shared_ptr<sf::Uint8[]> &bitmask1, const sf::Vector2f &position1, const sf::Vector2f &size1,
                                const std::shared_ptr<sf::Uint8[]> &bitmask2, const sf::Vector2f &position2, const sf::Vector2f &size2);  
    
    // collision helpers ( sprite vs. sprite )
    bool circleCollisionHelper(const Sprite& sprite1, const Sprite& sprite2); 

    // for non-sprite entities, call helper directly rather than checkcCollisions 
    bool boundingBoxCollisionHelper(const Sprite& sprite1, const Sprite& sprite2);      // for sprite vs. sprite
    bool boundingBoxCollisionHelper(sf::Vector2i mousePos, const Sprite& sprite2);      // for mouse vs. sprite
    bool boundingBoxCollisionHelper(sf::View view, const Sprite& sprite2);              // for view vs. sprite
    bool boundingBoxCollisionHelper(const TileMap& tileMap, const Sprite& sprite);      // for tileMap (entire) vs. sprite

    bool pixelPerfectCollisionHelper(const Sprite& sprite1, const Sprite& sprite2);
    bool raycastCollisionHelper(const Sprite& sprite1, const Sprite& sprite2, float currentTime, size_t index);

    //sprite vs spritesvector collision check; call physics::checkCollisions(sprite, sprite, collisionHelper)
    template<typename SpriteType1, typename SpriteType2, typename CollisionFunc>
    bool checkCollisions(const std::unique_ptr<SpriteType1>& first, const std::vector<std::unique_ptr<SpriteType2>>& Group, const CollisionFunc& collisionFunc) {
    
        if(!first){
            log_error("First sprite pointer is empty.");
            throw std::runtime_error("first sprite pointer is empty.");
        }

        for (const auto& item2 : Group) {
            if(!item2){
                log_error("Second sprite pointer is empty.");
                throw std::runtime_error("second sprite pointer is empty.");
            }

            if (collisionFunc(*first, *item2)) {
                return true; // Collision detected
            }
        }
        return false; // No collisions detected
    }

    //spritesvec vs spritesvector collision check 
    template<typename SpriteType1, typename SpriteType2, typename CollisionFunc>
    bool checkCollisions(const std::vector<std::unique_ptr<SpriteType1>>& firstGroup, 
                        const std::vector<std::unique_ptr<SpriteType2>>& secondGroup,
                        const CollisionFunc& collisionFunc, std::vector<float> firstGroupSpawnedTimes = {}) {
        
        // Check if the sizes of firstGroup and firstGroupSpawnedTimes match
        if (!firstGroupSpawnedTimes.empty() && firstGroupSpawnedTimes.size() != firstGroup.size()) {
            log_error("First group sprite vec size and spawned time size are not equal.");
            throw std::runtime_error("first group sprite vec size and spawned time size is not equal");
        }

        // If collisionFunc takes two parameters (SpriteType1&, SpriteType2&)
        if constexpr (std::is_invocable_v<CollisionFunc, SpriteType1&, SpriteType2&>) {
            std::cout << "CollisionFunc matches signature: bool(SpriteType1&, SpriteType2&)" << std::endl;
            for (const auto& item1 : firstGroup) {
                if (!item1) {
                    log_error("one or more of the first sprite pointer is empty");
                    throw std::runtime_error("first sprite pointer is empty.");
                }
                for (const auto& item2 : secondGroup) {
                    if (!item2) {
                        log_error("one or more of the second sprite pointer is empty");
                        throw std::runtime_error("second sprite pointer is empty.");
                    }
                    if (collisionFunc(*item1, *item2)) {
                        return true; // Collision detected
                    }
                }
            }
        } 
        // If collisionFunc takes four parameters (SpriteType1&, SpriteType2&, float, size_t)
        else if constexpr (std::is_invocable_v<CollisionFunc, SpriteType1&, SpriteType2&, float, size_t>) {
            std::cout << "CollisionFunc matches signature: bool(SpriteType1&, SpriteType2&, float, size_t)" << std::endl;
            for (size_t i = 0; i < firstGroup.size(); ++i) {
                if (!firstGroup[i]) {
                    log_error("one or more of the first sprite pointer is empty");
                    throw std::runtime_error("first sprite pointer is empty.");
                }
                for (const auto& item2 : secondGroup) {
                    if (!item2) {
                        log_error("one or more of the second sprite pointer is empty");
                        throw std::runtime_error("second sprite pointer is empty.");
                    }
                    if (collisionFunc(*firstGroup[i], *item2, firstGroupSpawnedTimes[i], i)) {
                        return true; // Collision detected
                    }
                }
            }
        } 
        else {
            std::cout << "Unknown CollisionFunc type" << std::endl;
        }
        return false;
    }

template<typename SpriteType1, typename SpriteType2, typename CollisionFunc>
bool checkCollisionsWithQuadtree(const std::vector<std::unique_ptr<SpriteType1>>& firstGroup,
                                 const std::vector<std::unique_ptr<SpriteType2>>& secondGroup,
                                 const CollisionFunc& collisionFunc, 
                                 Quadtree& quadtree,  // Now comes before the defaulted parameter
                                 std::vector<float> firstGroupSpawnedTimes = {}) {  // Defaulted parameter comes last
    
    if (!firstGroup.empty() && firstGroup.size() != firstGroupSpawnedTimes.size()) {
        log_error("First group sprite vec size and spawned time size are not equal.");
        throw std::runtime_error("First group sprite vec size and spawned time size is not equal.");
    }

    // Check collisions for SpriteType1 and SpriteType2
    for (const auto& item1 : firstGroup) {
        if (!item1) {
            log_error("One or more of the first sprite pointer is empty");
            throw std::runtime_error("First sprite pointer is empty.");
        }

        // Get the potential colliders from the quadtree for each sprite in firstGroup
        auto potentialColliders = quadtree.query(item1->getBounds());

        for (const auto& item2 : potentialColliders) {
            if (!item2) {
                log_error("One or more of the second sprite pointer is empty");
                throw std::runtime_error("Second sprite pointer is empty.");
            }

            // Check for collision between item1 and item2
            if (collisionFunc(*item1, *item2)) {
                item2->setVisibleState(false);
                item1->setVisibleState(false);
                return true; // Collision detected
            }
        }
    }

    return false; // No collisions detected
}
}