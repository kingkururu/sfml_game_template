//
//  physics.hpp
//  sfml game template
//
//  Created by Sunmyoung Yun on 2024/09
//
#pragma once

#ifndef physics_hpp
#define physics_hpp

#include <type_traits>
#include <iostream>
#include <memory>
#include <vector>
#include <stdexcept>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <functional> 
#include "sprites.hpp"
#include "flags.hpp"


/* declarations for physics methods */
namespace physics{
    struct RaycastResult;
    extern RaycastResult cachedRaycastResult; 

    constexpr float gravity = 9.8f;
    constexpr float timeStep = 0.01667;

    // falling objects
    sf::Vector2f freeFall(float speed, sf::Vector2f originalPo);
    sf::Vector2f follow( float speed, sf::Vector2f originalPos, sf::Vector2f acceleration, const sf::Vector2f& direction); 

    // moving player
    sf::Vector2f moveLeft( float speed, sf::Vector2f originalPos, sf::Vector2f acceleration = {1.0f, 1.0f}); 
    sf::Vector2f moveRight( float speed, sf::Vector2f originalPos, sf::Vector2f acceleration = {1.0f, 1.0f});
    sf::Vector2f moveUp( float speed, sf::Vector2f originalPos, sf::Vector2f acceleration = {1.0f, 1.0f});
    sf::Vector2f moveDown( float speed, sf::Vector2f originalPos, sf::Vector2f acceleration = {1.0f, 1.0f});
    sf::Vector2f jump(float& elapsedTime, float speed, sf::Vector2f originalPos, float deltaTime ); 

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
            sprite->changePosition(moveFunc(elapsedTime, speed, originalPos, timeStep)); 
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
    
    // collision helpers
    bool circleCollisionHelper(const Sprite& sprite1, const Sprite& sprite2); 

    bool boundingBoxCollisionHelper(const Sprite& sprite1, const Sprite& sprite2); 
    bool boundingBoxCollisionHelper(sf::Vector2i mousePos, const Sprite& sprite2); 

    bool pixelPerfectCollisionHelper(const Sprite& sprite1, const Sprite& sprite2);
    bool raycastCollisionHelper(const Sprite& sprite1, const Sprite& sprite2, float currentTime, size_t index);

    // //mousePos vs sprite for button detection 
    // template<typename SpriteType1, typename CollisionFunc>
    // bool checkCollisions(const std::unique_ptr<SpriteType1>& first, const sf::Vector2i item2, const CollisionFunc& collisionFunc) {
    
    //     if(!first){
    //         log_error("First sprite pointer is empty.");
    //         throw std::runtime_error("first sprite pointer is empty.");
    //     }

    //     if (collisionFunc(*first, item2)) {
    //         item2->setVisibleState(false); 
    //         return true; // Collision detected
    //     }
    
    //     return false; // No collisions detected
    // }

    //sprite vs spritesvector collision check 
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
                item2->setVisibleState(false); 
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
                        item2->setVisibleState(false); 
                        item1->setVisibleState(false); 
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
                        item2->setVisibleState(false);
                        firstGroup[i]->setVisibleState(false);
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

}

#endif /* game_hpp */
