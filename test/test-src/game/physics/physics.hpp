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

    class Quadtree {
     public:
        Quadtree(float x, float y, float width, float height, size_t level = 0, size_t maxObjects = 10, size_t maxLevels = 5);
        ~Quadtree(){ clear(); };

        void clear();

        template<typename SpriteType> void insert(std::unique_ptr<SpriteType>& obj) { 
            try {
                // If no child nodes exist, add the object to this node
                if (nodes.empty()) {
                    objects.push_back(obj.get());
                    log_info("Sprite inserted into quadtree node.");
                } else {
                    // Check which child node the object belongs to
                    for (auto& node : nodes) {
                        if (node->bounds.contains(obj->returnSpritesShape().getPosition())) {
                            node->insert(obj);
                            log_info("Sprite inserted into child node.");
                            return;
                        }
                    }
                }
            } catch (const std::exception& e) {
                log_error("Error during insert: " + std::string(e.what()));
            }
        }

        std::vector<Sprite*> query(const sf::FloatRect& area) const;
        void subdivide();
        bool contains(const sf::FloatRect& bounds) const;
        void update(); 

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
    sf::Vector2f jumpToSurface(float& elapsedTime, float speed, sf::Vector2f originalPos, sf::Vector2f acceleration = {0.1f, 0.1f}); 

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
    void spriteMover(std::unique_ptr<SpriteType>& sprite, const MoveFunc& moveFunc, float& elapsedTime, sf::Vector2f acceleration) {
        float speed = sprite->getSpeed(); 
        sf::Vector2f originalPos = sprite->getSpritePos(); 

        // Handle different types of MoveFunc
        if constexpr (std::is_invocable_v<MoveFunc, float&, float, sf::Vector2f, sf::Vector2f>){
            sprite->changePosition(moveFunc(elapsedTime, speed, originalPos, acceleration)); 
        }
        sprite->updatePos();  // Update sprite's position after applying the move function
    }

    //circle-shaped sprite collision
    bool circleCollision(const sf::Vector2f pos1, float radius1, const sf::Vector2f pos2, float radius2);
    //raycast pre-collision
    bool raycastPreCollision(const sf::Vector2f obj1position, const sf::Vector2f obj1direction, float obj1Speed, const sf::FloatRect obj1Bounds, sf::Vector2f obj1Acceleration, 
                            const sf::Vector2f obj2position, const sf::Vector2f obj2direction, float obj2Speed, const sf::FloatRect obj2Bounds, sf::Vector2f obj2Acceleration);
    //axis aligned bounding box collision
    bool boundingBoxCollision(const sf::Vector2f &position1, const sf::Vector2f& size1, const sf::Vector2f &position2, const sf::Vector2f& size2);
    //pixel perfect collision
    bool pixelPerfectCollision( const std::shared_ptr<sf::Uint8[]> &bitmask1, const sf::Vector2f &position1, const sf::Vector2f &size1,
                                const std::shared_ptr<sf::Uint8[]> &bitmask2, const sf::Vector2f &position2, const sf::Vector2f &size2);  
    
     struct CollisionData {
        sf::Vector2f position;
        float radius;
        sf::Vector2f direction;
        float speed;
        sf::Vector2f acceleration;
        sf::Vector2f size;
        std::shared_ptr<sf::Uint8[]> bitmask; // for bitmask-based collision
        sf::FloatRect bounds;
    };

    template<typename Sprite> 
    CollisionData extractCollisionData(Sprite&& sprite) {
        CollisionData data;
        data.bounds = sprite->returnSpritesShape().getGlobalBounds();
        data.position = sprite->getSpritePos();
        data.radius = sprite->getRadius();
        data.direction = sprite->getDirectionVector();
        data.speed = sprite->getSpeed();
        data.acceleration = sprite->getAcceleration();

        if (sprite->isAnimated()) {
            sf::IntRect rect = sprite->getRects();
            data.position = {data.bounds.left, data.bounds.top};
            data.size = {static_cast<float>(rect.width), static_cast<float>(rect.height)};
        } else {
            data.size = {data.bounds.width, data.bounds.height};
        }

        data.bitmask = sprite->getBitmask(sprite->getCurrIndex());
        return data;
    }

    template<typename ObjType1, typename ObjType2>
    bool collisionHelper(ObjType1&& obj1, ObjType2&& obj2) { // for sprive vs. non-sprite
        auto getSprite = [](auto&& obj1) -> auto& {
            if constexpr (std::is_pointer_v<std::decay_t<decltype(obj1)>>) {
                return *obj1; // Dereference unique_ptr or raw pointer
            } else {
                return obj1; // Direct reference if it's an object
            }
        };

        // Retrieve references to obj1 and obj2
        auto& sprite1 = getSprite(std::forward<ObjType1>(obj1));
        CollisionData data1 = extractCollisionData(sprite1);

        if constexpr (std::is_same_v<std::decay_t<ObjType2>, sf::Vector2f>) { // mouse 
            sf::Vector2f position2(static_cast<float>(obj2.x), static_cast<float>(obj2.y));
            sf::Vector2f size2(1.0f, 1.0f);

            return boundingBoxCollision(data1.position, data1.size, position2, size2);
        } 
        else if constexpr (std::is_same_v<std::decay_t<ObjType2>, sf::View>) { // view
            sf::Vector2f viewCenter = obj2.getCenter();
            sf::Vector2f viewSize = obj2.getSize();
            sf::Vector2f position2(viewCenter.x - viewSize.x / 2, viewCenter.y - viewSize.y / 2);
            sf::Vector2f size2(viewSize.x, viewSize.y);

            return boundingBoxCollision(data1.position, data1.size, position2, size2);
        } 
        else { // tilemap 
            auto getTileMap = [](auto&& obj2) -> auto& {
                if constexpr (std::is_pointer_v<std::decay_t<decltype(obj2)>> || std::is_same_v<std::decay_t<decltype(obj2)>, std::unique_ptr<TileMap>>) {
                    return *obj2; 
                } else {
                    return obj2; 
                }
            };
            auto& tileMap = getTileMap(obj2);

            if constexpr (std::is_same_v<std::decay_t<decltype(tileMap)>, TileMap>) {
                sf::Vector2f position2 = tileMap.getTileMapPosition();
                sf::Vector2f size2(
                    tileMap.getTileWidth() * static_cast<float>(tileMap.getTileMapWidth()),
                    tileMap.getTileHeight() * static_cast<float>(tileMap.getTileMapHeight())
                );
                return boundingBoxCollision(data1.position, data1.size, position2, size2);
            }
        }
        return false; // Default case
    }

    template<typename ObjType1, typename ObjType2, typename CollisionType> // for sprite vs. sprite
    bool collisionHelper(ObjType1&& obj1, ObjType2&& obj2, const CollisionType& collisionFunc, Quadtree* quadtree = nullptr, float timeElapsed = 0.0f, size_t counterIndex = 0) { // for sprive vs. sprite
        // Check if obj1 and obj2 are valid pointers
        if (!obj1) {
            log_warning("First object is missing in collision detection");
            return false;
        }
        if (!obj2) {
            log_warning("Second object is missing in collision detection");
            return false;
        }

        // Helper function for handling unique_ptr<DerivedSprite> or Sprite directly
        auto getSprite = [](auto&& obj) -> auto& {
            if constexpr (std::is_pointer_v<std::decay_t<decltype(obj)>>) return *obj; // Dereference unique_ptr or raw pointer
            else return obj; // Direct reference if it's an object
        };

        // Retrieve references to obj1 and obj2
        auto& sprite1 = getSprite(std::forward<ObjType1>(obj1));
        auto& sprite2 = getSprite(std::forward<ObjType2>(obj2));
        CollisionData data1 = extractCollisionData(sprite1);
        CollisionData data2 = extractCollisionData(sprite2);

        auto collisionLambda = [&timeElapsed, counterIndex](const CollisionData& data1, const CollisionData& data2, 
                                                                                  CollisionType collisionFunc) {
            if constexpr (std::is_invocable_v<CollisionType, sf::Vector2f, float, sf::Vector2f, float>) { // circle collision
                return collisionFunc(data1.position, data1.radius, data2.position, data2.radius);
            } else if constexpr (std::is_invocable_v<CollisionType, sf::Vector2f, sf::Vector2f, sf::Vector2f, sf::Vector2f>) { // bounding box collision
                return collisionFunc(data1.position, data1.size, data2.position, data2.size);
            } else if constexpr (std::is_invocable_v<CollisionType, sf::Vector2f, sf::Vector2f, float, sf::FloatRect, sf::Vector2f>) { // raycast pre-collision
                if (!cachedRaycastResult.counter) {
                    return collisionFunc(data1.position, data1.direction, data1.speed, data1.bounds, data1.acceleration, data2.position, data2.direction, data2.speed, data2.bounds, data2.acceleration);
                } else if (timeElapsed > cachedRaycastResult.collisionTimes[counterIndex]) {
                    cachedRaycastResult.counter = 0;
                    return true;
                }
            } else if constexpr (std::is_invocable_v<CollisionType, std::shared_ptr<sf::Uint8[]>, sf::Vector2f, sf::Vector2f, std::shared_ptr<sf::Uint8[]>, sf::Vector2f, sf::Vector2f>) { // pixel perfect collision
                return collisionFunc(data1.bitmask, data1.position, data1.size, data2.bitmask, data2.position, data2.size);
            }
            return false; // No collision detected
        };

         if(quadtree){
            auto potentialColliders1 = quadtree->query(sprite1->returnSpritesShape().getGlobalBounds());
            auto potentialColliders2 = quadtree->query(sprite2->returnSpritesShape().getGlobalBounds());

            if (potentialColliders1.empty() || potentialColliders2.empty()) return false;

            for (const auto& collider1 : potentialColliders1) {
                for (const auto& collider2 : potentialColliders2) {
                    if (collider1 == collider2) continue;  // Skip self-collision checks
                    
                    return collisionLambda(data1, data2, collisionFunc);
                }
            }
        } else {
            return collisionLambda(data1, data2, collisionFunc);
        }
        return false; // default 
    }
}