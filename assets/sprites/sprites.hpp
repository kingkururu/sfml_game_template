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
#include "log.hpp"
#include "globals.hpp"

/* base class for all sprites; contains position, scale, and texture */
class Sprite : public sf::Drawable {
public:
    explicit Sprite(sf::Vector2f position, sf::Vector2f scale, std::weak_ptr<sf::Texture> texture);

    virtual ~Sprite() = default;
    sf::Vector2f const getSpritePos() const { return position; };
    sf::Sprite& returnSpritesShape() const { return *spriteCreated; } 
    bool const getVisibleState() const { return visibleState; }
    void setVisibleState(bool VisibleState){ visibleState = VisibleState; }

    // base template for retreaving radius (based on sprite size) 
    virtual float getRadius() const;

    // blank members for use in Animated class
    virtual sf::IntRect getRects() const { return sf::IntRect(); }
    virtual int getCurrIndex() const { return 0; }
    virtual std::shared_ptr<sf::Uint8[]> const getBitmask(size_t index) const { return nullptr; }

    // blank members for use in NonStatic class
    virtual sf::Vector2f getDirectionVector() const { return sf::Vector2f(); }
    virtual float getSpeed() const { return 0.0f; }
    virtual sf::Vector2f getAcceleration() const { return sf::Vector2f(); }

    // draws sprite using window.draw(*sprite)
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override { if (visibleState && spriteCreated) target.draw(*spriteCreated, states); }
    virtual void updateVisibility(); 

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
    
    void setAnimChangeState(bool newState) { animChangeState = newState; }
    void changeAnimation(); 
    void setRects(int animNum); 

    using Sprite::getRadius;
    float getRadius() const override; 
    using Sprite::getRects;
    sf::IntRect getRects() const override;
    using Sprite::getCurrIndex; 
    int getCurrIndex() const override { return currentIndex; } 
    using Sprite::getBitmask; 
    std::shared_ptr<sf::Uint8[]> const getBitmask(size_t index) const override; 

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
    void updateBackground(float backgroundSpeed); 
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
   explicit NonStatic(sf::Vector2f position, sf::Vector2f scale, std::weak_ptr<sf::Texture> texture, float speed, sf::Vector2f acceleration)
        : Sprite(position, scale, texture), speed(speed), acceleration(acceleration) {}
    ~NonStatic() override{}; 

    bool const getMoveState() const { return moveState; }
    void setMoveState(bool newState) { moveState = newState; }
    void changePosition(sf::Vector2f newPos) { position = newPos; }  
    void setSpeed(float newSpeed) { speed = newSpeed; } 
    void setAcceleration( sf::Vector2f newAcc) { acceleration = newAcc; } 

    virtual void setDirectionVector( sf::Vector2f dir) {directionVector = dir; } 

    using Sprite::getDirectionVector;
    virtual sf::Vector2f getDirectionVector() const override { return directionVector; }
    using Sprite::getSpeed;
    virtual float getSpeed() const override { return speed; }
    using Sprite::getAcceleration;
    virtual sf::Vector2f getAcceleration() const override{ return acceleration; }

protected:
    bool moveState = true;
    sf::Vector2f directionVector{}; 
    float speed {}; 
    sf::Vector2f acceleration{}; 
};

/* player class deriving from NonStatic; refers to movable player */
class Player : public NonStatic, public Animated {
public:
   explicit Player(sf::Vector2f position, sf::Vector2f scale, std::weak_ptr<sf::Texture> texture,
                float speed, sf::Vector2f acceleration,  
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
                      float speed, sf::Vector2f acceleration,  
                      const std::vector<sf::IntRect> animationRects, unsigned int indexMax, 
                      const std::vector<std::weak_ptr<sf::Uint8[]>>& bitMask)
        : Sprite(position, scale, texture), 
          NonStatic(position, scale, texture, speed, acceleration), // Call NonStatic constructor
          Animated(position, scale, texture, animationRects, indexMax, bitMask) // Call Animated constructor
    {}
    ~Obstacle() override = default;
    
    using Sprite::getDirectionVector;
    sf::Vector2f getDirectionVector() const override { return directionVector; }
    using NonStatic::setDirectionVector;
    void setDirectionVector(float angle);

private:
};

/* example use of animated, nonstatic class: bullet deriving from NonStatic; refers to moving bullets */
class Bullet : public NonStatic, public Animated {
public:
   explicit Bullet(sf::Vector2f position, sf::Vector2f scale, std::weak_ptr<sf::Texture> texture, 
                    float speed, sf::Vector2f acceleration,  
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

class Button : public Animated {
public:
    explicit Button(sf::Vector2f position, sf::Vector2f scale, std::weak_ptr<sf::Texture> texture, 
                      const std::vector<sf::IntRect> animationRects, unsigned int indexMax, 
                      const std::vector<std::weak_ptr<sf::Uint8[]>>& bitMask)
        : Sprite(position, scale, texture),
          Animated(position, scale, texture, animationRects, indexMax, bitMask) // Call Animated constructor
    {}
    ~Button() override = default;

    void setClickedBool(bool click) { clicked = click; }
    bool getClickedBool() const { return clicked; }
    
private:
    bool clicked {}; 
}; 

class Sprite3D : public Sprite {
public:
    explicit Sprite3D(sf::Vector2f position, sf::Vector2f scale, std::weak_ptr<sf::Texture> texture, float zdepth)
        : Sprite(position, scale, texture), zdepth(zdepth) {}

    void setZ(float newZ) { zdepth = newZ; }
    float getZ() const { return zdepth; }

    using Sprite::draw;
    void draw(sf::RenderTarget& target, sf::RenderStates states); 

private:
    float zdepth;  // Depth position
};


#endif /* sprites_hpp */
