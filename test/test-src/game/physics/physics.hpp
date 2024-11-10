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

        // Handle different types of MoveFunc
        if constexpr (std::is_invocable_v<MoveFunc, float&, float, sf::Vector2f, float>){
            sprite->changePosition(moveFunc(elapsedTime, speed, originalPos)); 
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
    
    // template<typename ObjType1, typename ObjType2, typename CollisionType>
    // bool collisionHelper(const ObjType1& obj1, const ObjType2& obj2, const CollisionType& collisionFunc, Quadtree& quadtree, float timeElapsed = 0.0f, size_t counterIndex = 0){
        
    //     if(!obj1){
    //         log_warning("first object is missing in collision detection");
    //         return false; 
    //     } if(!obj2){
    //         log_warning("first object is missing in collision detection");
    //         return false; 
    //     }
        
    //     sf::Vector2f position1 {};
    //     float radius1 {};
    //     sf::Vector2f direction1 {};
    //     float speed1 {};
    //     sf::FloatRect bounds1 {};
    //     sf::Vector2f acceleration1 {}; 
    //     sf::Vector2f size1 {};
    //     std::shared_ptr<sf::Uint8[]> &bitmask1; 
       
    //     sf::Vector2f position2 {};
    //     float radius2 {};
    //     sf::Vector2f direction2 {};
    //     float speed2 {};
    //     sf::FloatRect bounds2 {};
    //     sf::Vector2f acceleration2 {}; 
    //     sf::Vector2f size2 {};
    //     std::shared_ptr<sf::Uint8[]> &bitmask2; 

    //     // obj1 is a sprite (unique_ptr<Sprite> or some other thing deriving from Sprite)
    //     if (1 is sprite ) {
    //         bounds1 = obj1.returnSpritesShape().getGlobalBounds();
    //         position1 = obj1.getSpritePos();
    //         radius1 =  obj1.getRadius(); 
    //         bitmask1 = obj1.getBitmask(obj1.getCurrIndex());
    //         direction1 = obj1.getDirectionVector();
    //         speed1 = obj1.getSpeed(); 
    //         bounds1 = obj1.returnSpritesShape().getGlobalBounds();
    //         acceleration1 = obj1.getAcceleration(); 

    //         if (animated) {
    //             sf::IntRect rect1 = obj1.getRects();
    //             position1(bounds1.left + rect1.left, bounds1.top + rect1.top);
    //             size1(static_cast<float>(rect1.width), static_cast<float>(rect1.height));
    //         }
    //     } // obj2 is mouse position (sf::Vector2f)
    //     else if( ) {
    //         position1(static_cast<float>(obj1.x), static_cast<float>(obj1.y));
    //         size1(1.0f, 1.0f);
    //     } // obj2 is view (sf::View)
    //     else if( ) {
    //         sf::Vector2f viewCenter = obj1.getCenter();
    //         sf::Vector2f viewSize = obj1.getSize();
    //         position1(viewCenter.x - viewSize.x / 2, viewCenter.y - viewSize.y / 2);
    //         size1(viewSize.x, viewSize.y);
    //     } // obj2 is tileMap (TileMap)
    //     else if( ) {
    //         position1(obj1.getTileMapPosition());
    //         size1( obj1.getTileWidth() * static_cast<float>(obj1.getTileMapWidth()),   // Total width in pixels
    //                obj1.getTileHeight() * static_cast<float>(obj1.getTileMapHeight())  // Total height in pixels
    //         );
    //     }

    //     // obj2 is a sprite (unique_ptr<Sprite> or some other thing deriving from Sprite)
    //     if ( ) {
    //         bounds2 = obj2.returnSpritesShape().getGlobalBounds();
    //         position2 = obj2.getSpritePos();
    //         radius2 =  obj2.getRadius(); 
    //         bitmask2 = obj2.getBitmask(obj2.getCurrIndex());
    //         direction2 = obj2.getDirectionVector();
    //         speed2 = obj2.getSpeed(); 
    //         bounds2 = obj2.returnSpritesShape().getGlobalBounds();
    //         acceleration2 = obj2.getAcceleration(); 

    //         if (animated) {
    //             sf::IntRect rect2 = obj2.getRects();
    //             position2(bounds2.left + rect2.left, bounds2.top + rect2.top);
    //             size2(static_cast<float>(rect2.width), static_cast<float>(rect2.height));
    //         }
    //     } // obj2 is mouse position (sf::Vector2f)
    //     else if( ) {
    //         position2(static_cast<float>(obj2.x), static_cast<float>(obj2.y));
    //         size2(1.0f, 1.0f);
    //     } // obj2 is view (sf::View)
    //     else if( ) {
    //         sf::Vector2f viewCenter = obj2.getCenter();
    //         sf::Vector2f viewSize = obj2.getSize();
    //         position2(viewCenter.x - viewSize.x / 2, viewCenter.y - viewSize.y / 2);
    //         size2(viewSize.x, viewSize.y);
    //     } // obj2 is tileMap (TileMap)
    //     else if( ) {
    //         position2(obj2.getTileMapPosition());
    //         size2( obj2.getTileWidth() * static_cast<float>(obj2.getTileMapWidth()),   // Total width in pixels
    //                obj2.getTileHeight() * static_cast<float>(obj2.getTileMapHeight())  // Total height in pixels
    //         );
    //     }

    //     // circle collision 
    //     if constexpr (std::is_invocable_v<CollisionType, sf::Vector2f, float, sf::Vector2f, float>) {
    //         CollisionType(position1, radius1, position2, radius2);
    //     } // bounding box collision
    //     else if constexpr (std::is_invocable_v<CollisionType, sf::Vector2f, sf::Vector2f, sf::Vector2f, sf::Vector2f>) {
    //         CollisionType(position1, size1, position2, size2);
    //     } // raycast pre-calculation 
    //     else if constexpr (std::is_invocable_v<CollisionType, sf::Vector2f, sf::Vector2f, float, sf::FloatRect, sf::Vector2f>) {
    //         if(!cachedRaycastResult.counter){
    //             CollisionType(position1, direction1, speed1, bounds1, acceleration1, position2, direction2, speed2, bounds2, acceleration2);
    //         } else if (timeElapsed > cachedRaycastResult.collisionTimes[counterIndex]){
    //             cachedRaycastResult.counter = 0; 
    //             return true;
    //         }
    //     } // pixel perfect collision
    //     else if constexpr (std::is_invocable_v<CollisionType, std::shared_ptr<sf::Uint8[]>, sf::Vector2f, sf::Vector2f>){
    //         CollisionType(bitmask1, position1, size1, bitmask2, position2, size2)
    //     }

    //     return false;
       
    //    }
    // }

template<typename ObjType1, typename ObjType2, typename CollisionType>
bool collisionHelper(ObjType1&& obj1, ObjType2&& obj2, const CollisionType& collisionFunc, Quadtree& quadtree, float timeElapsed = 0.0f, size_t counterIndex = 0) {
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
        if constexpr (std::is_pointer_v<std::decay_t<decltype(obj)>>) {
            return *obj; // Dereference unique_ptr or raw pointer
        } else {
            return obj; // Direct reference if it's an object
        }
    };

    // Retrieve references to obj1 and obj2
    auto& sprite1 = getSprite(std::forward<ObjType1>(obj1));
    auto& sprite2 = getSprite(std::forward<ObjType2>(obj2));

    // Query the quadtree for potential collisions
    sf::FloatRect bounds1 = sprite1->returnSpritesShape().getGlobalBounds();
    sf::FloatRect bounds2 = sprite2->returnSpritesShape().getGlobalBounds();
    auto potentialColliders1 = quadtree.query(bounds1);
    auto potentialColliders2 = quadtree.query(bounds2);

    // Only proceed if the quadtree query suggests a potential collision
    if (!potentialColliders1.empty() && !potentialColliders2.empty()) {
        for (const auto& collider1 : potentialColliders1) {
            for (const auto& collider2 : potentialColliders2) {
                if (collider1 == collider2) continue;  // Skip self-collision checks

                // Object properties retrieval for collision checks
                sf::Vector2f position1 = sprite1->getSpritePos();
                float radius1 = sprite1->getRadius();
                auto bitmask1 = sprite1->getBitmask(sprite1->getCurrIndex());
                sf::Vector2f direction1 = sprite1->getDirectionVector();
                float speed1 = sprite1->getSpeed();
                sf::Vector2f acceleration1 = sprite1->getAcceleration();
                
                sf::Vector2f position2 = sprite2->getSpritePos();
                float radius2 = sprite2->getRadius();
                auto bitmask2 = sprite2->getBitmask(sprite2->getCurrIndex());
                sf::Vector2f direction2 = sprite2->getDirectionVector();
                float speed2 = sprite2->getSpeed();
                sf::Vector2f acceleration2 = sprite2->getAcceleration();

                sf::Vector2f size1, size2;
                if (sprite1->isAnimated()) {
                    sf::IntRect rect1 = sprite1->getRects();
                    position1 = {bounds1.left + rect1.left, bounds1.top + rect1.top};
                    size1 = {static_cast<float>(rect1.width), static_cast<float>(rect1.height)};
                } else {
                    size1 = {bounds1.width, bounds1.height};
                }

                if (sprite2->isAnimated()) {
                    sf::IntRect rect2 = sprite2->getRects();
                    position2 = {bounds2.left + rect2.left, bounds2.top + rect2.top};
                    size2 = {static_cast<float>(rect2.width), static_cast<float>(rect2.height)};
                } else {
                    size2 = {bounds2.width, bounds2.height};
                }

                // Collision calculations based on function signature
                if constexpr (std::is_invocable_v<CollisionType, sf::Vector2f, float, sf::Vector2f, float>) {
                    return collisionFunc(position1, radius1, position2, radius2);
                } else if constexpr (std::is_invocable_v<CollisionType, sf::Vector2f, sf::Vector2f, sf::Vector2f, sf::Vector2f>) {
                    return collisionFunc(position1, size1, position2, size2);
                } else if constexpr (std::is_invocable_v<CollisionType, sf::Vector2f, sf::Vector2f, float, sf::FloatRect, sf::Vector2f>) {
                    if (!cachedRaycastResult.counter) {
                        return collisionFunc(position1, direction1, speed1, bounds1, acceleration1, position2, direction2, speed2, bounds2, acceleration2);
                    } else if (timeElapsed > cachedRaycastResult.collisionTimes[counterIndex]) {
                        cachedRaycastResult.counter = 0;
                        return true;
                    }
                } else if constexpr (std::is_invocable_v<CollisionType, std::shared_ptr<sf::Uint8[]>, sf::Vector2f, sf::Vector2f>) {
                    return collisionFunc(bitmask1, position1, size1, bitmask2, position2, size2);
                }
            }
        }
    }
    
    return false;  // No collision detected
}

}