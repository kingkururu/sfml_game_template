//
//  sprites.cpp
//  sfml game template
//
//  Created by Sunmyoung Yun on 2024/09
//

#include "sprites.hpp"

/* sprite class constructor; takes in position, scale, texture */
Sprite::Sprite(sf::Vector2f position, sf::Vector2f scale, std::weak_ptr<sf::Texture> texture)
    : position(position), scale(scale), texture(texture), spriteCreated(std::make_unique<sf::Sprite>()), visibleState(true) {
    try {
        if (auto tex = texture.lock()) {  
            sf::Vector2u textureSize = tex->getSize(); 
            if (!textureSize.x || !textureSize.y) {
                throw std::runtime_error("Loaded texture has size 0");
            }

            spriteCreated->setTexture(*tex); 
            spriteCreated->setPosition(position);
            spriteCreated->setScale(scale);
        } else {
            throw std::runtime_error("Texture is no longer available");
        }
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        visibleState = false;
    }
}

/* background class constructor; takes in position, scale, texture */
Background::Background(sf::Vector2f position, sf::Vector2f scale, std::weak_ptr<sf::Texture> texture) : Static(position, scale, texture) {
    if (auto tex = texture.lock()) {
        spriteCreated2 = std::make_unique<sf::Sprite>(*tex);
        spriteCreated2->setScale(scale);
        spriteCreated2->setPosition(position.x + tex->getSize().x * scale.x, position.y);
    }
}

/* moves background to the left and fills the screen with the same image stuck next to the original image */
void Background::updateBackground(float deltaTime, float speed) {
        // Move both background sprites to the left
        spriteCreated->move(-speed * deltaTime, 0);
        spriteCreated2->move(-speed * deltaTime, 0);

        // Reposition sprites when they go off screen
        if (spriteCreated->getPosition().x + spriteCreated->getGlobalBounds().width < 0) {
            spriteCreated->setPosition(spriteCreated2->getPosition().x + spriteCreated2->getGlobalBounds().width, spriteCreated->getPosition().y);
        }
        if (spriteCreated2->getPosition().x + spriteCreated2->getGlobalBounds().width < 0) {
            spriteCreated2->setPosition(spriteCreated->getPosition().x + spriteCreated->getGlobalBounds().width, spriteCreated2->getPosition().y);
        }
}

/* sets cut-out rect for sprite animation */
void NonStatic::setRects(int animNum){
    try{
        if(animNum < 0 || animNum > indexMax){
            throw std::out_of_range("Animation index out of range. ");
        }

        spriteCreated->setTextureRect(animationRects[animNum]);    
    }
    
    catch(const std::exception& e){
        std::cerr << "error in setting texture: " << e.what() << "the indexmax is: "<< indexMax << "the current index is "<< animNum << std::endl;
    }
}

/* changes animation based on time */
void NonStatic::changeAnimation(float deltaTime) {
    try {
        if(animChangeState){
            elapsedTime += deltaTime;
            if (elapsedTime > Constants::ANIMATION_CHANGE_TIME) {
                ++currentIndex; 

                if (currentIndex >= indexMax) {
                    currentIndex = 0; 
                }
                setRects(currentIndex); 
                elapsedTime = 0.0;
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error in changing animation, current index: " << currentIndex << " - " << e.what() << std::endl;
    }
}

/* updates position to be assigned most recent position */
void NonStatic::updatePos() {
    try {
        if (position.y > Constants::SCREEN_HEIGHT + Constants::SPRITE_OUT_OF_BOUNDS_OFFSET 
        || position.x > Constants::SCREEN_WIDTH + Constants::SPRITE_OUT_OF_BOUNDS_OFFSET 
        || position.y < 0 - Constants::SPRITE_OUT_OF_BOUNDS_OFFSET
        || position.x < 0 - Constants::SPRITE_OUT_OF_BOUNDS_OFFSET) {
            setVisibleState(false);
        }

        spriteCreated->setPosition(position); 
    }
    catch (const std::exception& e) {
        std::cerr << "Error in updating position: " << e.what() << std::endl;
    }
}

/* returns animation rects vector */
sf::IntRect NonStatic::getRects() const{
    if (animationRects.empty()) {
        throw std::runtime_error("Animation rects are empty.");
    }
    return animationRects[currentIndex % animationRects.size()];
}

/* returns bitmask for a sprite */
std::shared_ptr<sf::Uint8[]> const NonStatic::getBitmask(size_t index) const {
    if (index >= bitMask.size()) 
        throw std::out_of_range("Index out of range.");
        
    return bitMask[index].lock();
}

/* specialized player position update method */
void Player::updatePlayer(sf::Vector2f newPos) {
    if (FlagEvents.sPressed && newPos.y > Constants::SCREEN_HEIGHT - Constants::SPRITE_OUT_OF_BOUNDS_OFFSET){
        newPos.y = Constants::SCREEN_HEIGHT - Constants::SPRITE_OUT_OF_BOUNDS_ADJUSTMENT; 
    }
    if (FlagEvents.wPressed && newPos.y < (Constants::SCREEN_HEIGHT + Constants::SPRITE_OUT_OF_BOUNDS_OFFSET) / 2 ){
        newPos.y = (Constants::SCREEN_HEIGHT + Constants::SPRITE_OUT_OF_BOUNDS_OFFSET) / 2 ; 
    }

    changePosition(newPos); 
}

/* calculates obstacles's direction vector when bullet is made */
void Obstacle::setDirectionVector(float angle){
    float angleRad = angle * (3.14f / 180.f);
    directionVector.x = std::cos(angleRad);
    directionVector.y = std::sin(angleRad);
}

/* sets obstacles's direction vector when obstacle is made */
void Bullet::setDirectionVector(sf::Vector2i projectionPos) {
    directionVector = static_cast<sf::Vector2f>(projectionPos) - position;
        
        // Calculate the length of the direction vector (distance to the target)
        float length = std::sqrt(directionVector.x * directionVector.x + directionVector.y * directionVector.y);
    
         if (length == 0) 
            return; 

        directionVector.x /= length;
        directionVector.y /= length;
}