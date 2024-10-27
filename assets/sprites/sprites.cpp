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

            log_info("Sprite initialized successfully");

        } else {
            throw std::runtime_error("Texture is no longer available");
        }
    }
    catch (const std::exception& e) {
        log_error(e.what());
        visibleState = false;
    }
}

float Sprite::getRadius() const {
    if (!spriteCreated) {
        log_warning("\tUnable to get sprite's radius because sprite doesn't exist");
        return 0.0f; 
    }

    // Get the size of the sprite
    sf::FloatRect bounds = spriteCreated->getGlobalBounds();
    
    // Calculate the width and height of the bounds
    float width = bounds.width;
    float height = bounds.height;
    
    // Calculate the diagonal length
    float diagonal = std::sqrt(std::pow(width, 2) + std::pow(height, 2));
    
    // Radius is half the diagonal
    return diagonal / 2.0f;
}

/* background class constructor; takes in position, scale, texture */
Background::Background(sf::Vector2f position, sf::Vector2f scale, std::weak_ptr<sf::Texture> texture) : Sprite(position, scale, texture) {
    if (auto tex = texture.lock()) {
        spriteCreated2 = std::make_unique<sf::Sprite>(*tex);
        spriteCreated2->setScale(scale);
        spriteCreated2->setPosition(position.x + tex->getSize().x * scale.x, position.y);
        log_info("Background created at position (" + std::to_string(position.x) + ", " + std::to_string(position.y) + ")");    
    }
}

/* moves background to the left and fills the screen with the same image stuck next to the original image */
void Background::updateBackground(float speed) {
        // Move both background sprites to the left
        spriteCreated->move(-speed * MetaComponents::deltaTime, 0);
        spriteCreated2->move(-speed * MetaComponents::deltaTime, 0);

        // Reposition sprites when they go off screen
        if (spriteCreated->getPosition().x + spriteCreated->getGlobalBounds().width < 0) {
            spriteCreated->setPosition(spriteCreated2->getPosition().x + spriteCreated2->getGlobalBounds().width, spriteCreated->getPosition().y);
            log_info("Background sprite 1 repositioned.");
        }
        if (spriteCreated2->getPosition().x + spriteCreated2->getGlobalBounds().width < 0) {
            spriteCreated2->setPosition(spriteCreated->getPosition().x + spriteCreated->getGlobalBounds().width, spriteCreated2->getPosition().y);
            log_info("Background sprite 2 repositioned.");
        }
}

/* sets cut-out rect for sprite animation */
void Animated::setRects(int animNum){
    try {
        if (animNum < 0 || animNum > indexMax) {
            throw std::out_of_range("Animation index out of range.");
        }
        spriteCreated->setTextureRect(animationRects[animNum]);    
        
    }
    catch (const std::exception& e) {
        log_error("Error in setting texture: " + std::string(e.what()) + " | Index Max: " + std::to_string(indexMax) + " | Current Index: " + std::to_string(animNum));
    }
}

/* changes animation based on time */
void Animated::changeAnimation() {
    try {
        if (animChangeState) {
            elapsedTime += MetaComponents::deltaTime;
            if (elapsedTime > Constants::ANIMATION_CHANGE_TIME) {
                ++currentIndex;
                if (currentIndex >= indexMax) {
                    currentIndex = 0;
                }
                setRects(currentIndex);
                elapsedTime = 0.0f;
                
            }
        }
    }
    catch (const std::exception& e) {
        log_error("Error in changing animation: " + std::string(e.what()) + " | Current Index: " + std::to_string(currentIndex));
    }
}

float Animated::getRadius() const {
    if (!spriteCreated) {
        return 0.0f;  
        log_warning("\tUnable to get sprite's radius because sprite doesn't exist"); 
    }
    sf::IntRect rect = getRects();  // Retrieve the sf::IntRect for the sprite
    
    // Calculate the width and height of the rect
    float width = static_cast<float>(rect.width);
    float height = static_cast<float>(rect.height);
    
    // Calculate the diagonal length
    float diagonal = std::sqrt(std::pow(width, 2) + std::pow(height, 2));
    
    // Radius is half the diagonal
    return diagonal / 2.0f;
}

/* updates position to be assigned most recent position */
void Sprite::updateVisibility() {
    try {
        if (position.y > Constants::SCREEN_HEIGHT + Constants::SPRITE_OUT_OF_BOUNDS_OFFSET ||
            position.x > Constants::SCREEN_WIDTH + Constants::SPRITE_OUT_OF_BOUNDS_OFFSET ||
            position.y < 0 - Constants::SPRITE_OUT_OF_BOUNDS_OFFSET ||
            position.x < 0 - Constants::SPRITE_OUT_OF_BOUNDS_OFFSET) {
                // do something else 
            setVisibleState(false);
            log_info("Sprite moved out of bounds and is no longer visible.");
        }
        log_info("Sprite position updated to (" + std::to_string(position.x) + ", " + std::to_string(position.y) + ")");
    }
    catch (const std::exception& e) {
        log_error("Error in updating position: " + std::string(e.what()));
    }
}

/* returns animation rects vector */
sf::IntRect Animated::getRects() const {
    try {
        if (animationRects.empty()) {
            throw std::runtime_error("Animation rects are empty.");
        }
       // log_info("Returning animation rect for index " + std::to_string(currentIndex % animationRects.size()));
        return animationRects[currentIndex % animationRects.size()];
    } 
    catch (const std::exception& e) {
        log_error("Error in getRects: " + std::string(e.what()));
        throw;  // Rethrow the exception for further handling if necessary
    }
}

/* returns bitmask for a sprite */
std::shared_ptr<sf::Uint8[]> const Animated::getBitmask(size_t index) const {
    try {
        if (index >= bitMask.size()) {
            throw std::out_of_range("Index out of range.");
        }
        log_info("Returning bitmask for index " + std::to_string(index));
        return bitMask[index].lock();
    } 
    catch (const std::exception& e) {
        log_error("Error in getBitmask: " + std::string(e.what()) + " | Requested index: " + std::to_string(index));
        throw;  // Rethrow the exception for further handling if necessary
    }
}

/* specialized player position update method */
void Player::updatePlayer(sf::Vector2f newPos) {
    changePosition(position); 
    log_info("Player position updated to (" + std::to_string(newPos.x) + ", " + std::to_string(newPos.y) + ")");
}

/* calculates obstacle's direction vector when bullet is made */
void Obstacle::setDirectionVector(float angle) {
    float angleRad = angle * (3.14f / 180.f);
    directionVector.x = std::cos(angleRad);
    directionVector.y = std::sin(angleRad);
    log_info("Obstacle direction vector set based on angle " + std::to_string(angle));
}

/* sets bullet's direction vector */
void Bullet::setDirectionVector(sf::Vector2i projectionPos) {
    directionVector = static_cast<sf::Vector2f>(projectionPos) - position;
    
    // Calculate the length of the direction vector (distance to the target)
    float length = std::sqrt(directionVector.x * directionVector.x + directionVector.y * directionVector.y);

    if (length != 0) {
        directionVector.x /= length;
        directionVector.y /= length;
    }
    log_info("Bullet direction vector calculated.");
}

void Sprite3D::draw(sf::RenderTarget& target, sf::RenderStates states) {
    // Apply simple perspective scaling based on z-depth
    float perspectiveScale = 1.0f / (1.0f + zdepth / 500.0f);  // Example of perspective scaling

    sf::Transform transform;
    transform.scale(perspectiveScale, perspectiveScale);
    states.transform *= transform;

    Sprite::draw(target, states);  // Draw the sprite with perspective
} 