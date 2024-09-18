//
//  sprites.hpp
//  sfml game template
//
//  Created by Sunmyoung Yun on 2024/08
//
#pragma once

#ifndef sprites_hpp
#define sprites_hpp

#include <stdio.h>
#include <string> 
#include <iostream>
#include <stdexcept>
#include <map>
#include <SFML/Graphics.hpp>
#include "constants.hpp"
#include "flags.hpp" 

/* base class for all sprites; contains position, scale, and texture */
class Sprite{
public:
    explicit Sprite(sf::Vector2f position, sf::Vector2f scale, std::weak_ptr<sf::Texture> texture);

    virtual ~Sprite() = default;
    sf::Vector2f const getSpritePos() const { return position; };
    sf::Sprite& returnSpritesShape() const { return *spriteCreated; }
    bool const getVisibleState() const { return visibleState; }
    void setVisibleState(bool VisibleState){ visibleState = VisibleState; }
    float getRadius() const;
    //blank members for use in Animated class
    virtual sf::IntRect getRects() const { return sf::IntRect(); }
    virtual int getCurrIndex() const { return 0; }
    virtual std::shared_ptr<sf::Uint8[]> const getBitmask(size_t index) const { return nullptr; }
    //blank members for use in NonStatic class
    virtual sf::Vector2f getDirectionVector() const { return sf::Vector2f(); }
    virtual float getSpeed() const { return 0.0f; }
    virtual float getAcceleration() const { return 0.0f; }

protected:
    sf::Vector2f position {};
    sf::Vector2f scale {};
    std::weak_ptr<sf::Texture> texture;
    std::unique_ptr<sf::Sprite> spriteCreated;
    bool visibleState {};
    float radius{}; 
};

class Animated : public virtual Sprite {
public:
    explicit Animated( sf::Vector2f position, sf::Vector2f scale, std::weak_ptr<sf::Texture> texture, const std::vector<sf::IntRect> animationRects, unsigned const int indexMax,  const std::vector<std::weak_ptr<sf::Uint8[]>>& bitMask) 
        : Sprite(position, scale, texture), animationRects(animationRects), indexMax(indexMax), bitMask(bitMask) {}
    std::vector<sf::IntRect> getAnimationRects() const { return animationRects; } 
    void setAnimation(std::vector<sf::IntRect> AnimationRects) { animationRects = AnimationRects; } 
    using Sprite::getRects;
    sf::IntRect getRects() const;
    void setRects(int animNum); 
    using Sprite::getCurrIndex; 
    int getCurrIndex() const { return currentIndex; } 
    void changeAnimation(float deltaTime); 
    using Sprite::getBitmask; 
    std::shared_ptr<sf::Uint8[]> const getBitmask(size_t index) const ; 
    void setAnimChangeState(bool newState) { animChangeState = newState; }

private:
    std::vector<sf::IntRect> animationRects{}; 
    int currentIndex {};
    int indexMax {}; 
    float elapsedTime {};
    bool animChangeState = true; 
    std::vector<std::weak_ptr<sf::Uint8[]>> bitMask{}; 
};

class NonAnimated : public virtual Sprite {
    using Sprite::Sprite;
    ~NonAnimated() override{};
};


/* background class deriving from sprites; the background doesn't "actually move with physics", but moves constantly to the left */
class Background : public Sprite{
public:
   explicit Background(sf::Vector2f position, sf::Vector2f scale, std::weak_ptr<sf::Texture> texture);
    ~Background() override{};
    void updateBackground(float deltaTime, float backgroundSpeed); 
    sf::Sprite& returnSpritesShape2() { return *spriteCreated2; }
    bool getBackgroundMoveState() { return backgroundMoveState; } 
    void setBackgroundMoveState(bool newState) { backgroundMoveState = newState; }

private:
    std::unique_ptr<sf::Sprite> spriteCreated2;
    bool backgroundMoveState = true; 
};

/* static class deriving from sprites; refers to non-moving sprites */
class Static : public virtual Sprite{
public:
    using Sprite::Sprite;
    ~Static() override{};
};

/* NonStatic class deriving from sprites; refers to moving sprites */
class NonStatic : public virtual Sprite{
public:
   explicit NonStatic(sf::Vector2f position, sf::Vector2f scale, std::weak_ptr<sf::Texture> texture, float speed, float acceleration)
        : Sprite(position, scale, texture), speed(speed), acceleration(acceleration) {}
    ~NonStatic() override{}; 
    void updatePos(); 
    bool const getMoveState() const { return moveState; }
    void setMoveState(bool newState) { moveState = newState; }
    void changePosition(sf::Vector2f newPos) { position = newPos; }  
    using Sprite::getDirectionVector;
    virtual sf::Vector2f getDirectionVector() const override { return directionVector; }
    virtual void setDirectionVector( sf::Vector2f dir) {directionVector = dir; } 
    using Sprite::getSpeed;
    virtual float getSpeed() const override { return speed; }
    void setSpeed(float newSpeed) { speed = newSpeed; } 
    using Sprite::getAcceleration;
    virtual float getAcceleration() const override{ return acceleration; }
    void setAcceleration(float newAcc) { acceleration = newAcc; } 

protected:
    bool moveState = true;
    sf::Vector2f directionVector{}; 
    float speed {}; 
    float acceleration{}; 
};

/* player class deriving from NonStatic; refers to movable player */
class Player : public NonStatic, public Animated {
public:
   explicit Player(sf::Vector2f position, sf::Vector2f scale, std::weak_ptr<sf::Texture> texture,
                float speed, float acceleration,  
                const std::vector<sf::IntRect> animationRects, unsigned int indexMax, 
                const std::vector<std::weak_ptr<sf::Uint8[]>>& bitMask)
    : Sprite(position, scale, texture), 
      NonStatic(position, scale, texture, speed, acceleration), 
      Animated(position, scale, texture, animationRects, indexMax, bitMask) {}

   ~Player() override = default;
    void updatePlayer(sf::Vector2f newPos); 
};

/* obstacle class deriving from NonStatic; refers to movable obstacles */
class Obstacle : public NonStatic, public Animated {
public:
    explicit Obstacle(sf::Vector2f position, sf::Vector2f scale, std::weak_ptr<sf::Texture> texture, 
                      float speed, float acceleration,  
                      const std::vector<sf::IntRect> animationRects, unsigned int indexMax, 
                      const std::vector<std::weak_ptr<sf::Uint8[]>>& bitMask)
        : Sprite(position, scale, texture), 
          NonStatic(position, scale, texture, speed, acceleration), // Call NonStatic constructor
          Animated(position, scale, texture, animationRects, indexMax, bitMask) // Call Animated constructor
    {}
    ~Obstacle() override = default;
    sf::Vector2f getDirectionVector() const override { return directionVector; }
    using NonStatic::setDirectionVector;
    void setDirectionVector(float angle);

private:
};

/* bullet class deriving from NonStatic; refers to moving bullets */
class Bullet : public NonStatic, public Animated {
public:
   explicit Bullet(sf::Vector2f position, sf::Vector2f scale, std::weak_ptr<sf::Texture> texture, 
                    float speed, float acceleration,  
                    const std::vector<sf::IntRect> animationRects, unsigned int indexMax, 
                    const std::vector<std::weak_ptr<sf::Uint8[]>>& bitMask)
        : Sprite(position, scale, texture), 
          NonStatic(position, scale, texture, speed, acceleration), // Call NonStatic constructor
          Animated(position, scale, texture, animationRects, indexMax, bitMask) // Call Animated constructor
    {}
    ~Bullet() override = default;
    using NonStatic::setDirectionVector;
    void setDirectionVector(sf::Vector2i projectionPos);

private:
};



#endif /* sprites_hpp */
