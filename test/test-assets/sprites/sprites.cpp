//
//  sprites.cpp
//  sfml game template
//
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
        spriteCreated = std::make_unique<sf::Sprite>(*tex);
        spriteCreated->setScale(scale);
        spriteCreated->setPosition(position.x, position.y); 

        // initially position background sprite2 to the right side (off screen)
        spriteCreated2 = std::make_unique<sf::Sprite>(*tex);
        spriteCreated2->setScale(scale);
        spriteCreated2->setPosition(position.x + tex->getSize().x * scale.x, position.y);

        // initially position background sprite2 to the down side (off screen)
        spriteCreated3 = std::make_unique<sf::Sprite>(*tex);
        spriteCreated3->setScale(scale);
        spriteCreated3->setPosition(position.x, position.y + tex->getSize().y * scale.y);

        // initially position background sprite2 to the down side (off screen)
        spriteCreated4 = std::make_unique<sf::Sprite>(*tex);
        spriteCreated4->setScale(scale);
        spriteCreated4->setPosition(position.x, position.y);

        log_info("Background created");    
    }
}
 
 // Updates the background to move in a specified direction; need to fix it later cus the forth sprite doesn't wrap properly 
void Background::updateBackground(float speed, SpriteComponents::Direction primaryDirection, SpriteComponents::Direction secondaryDirection) {
    // Get the global bounds of the view
    sf::FloatRect viewBounds(
        MetaComponents::view.getCenter().x - MetaComponents::view.getSize().x / 2,
        MetaComponents::view.getCenter().y - MetaComponents::view.getSize().y / 2,
        MetaComponents::view.getSize().x,
        MetaComponents::view.getSize().y
    );
        // Calculate the current movement offset based on speed, deltaTime
    float offsetX = 0; // horizontal offset
    float offsetY = 0; // vertical offset

    // Set the offsets based on direction
    if (primaryDirection == SpriteComponents::Direction::RIGHT) {
        offsetX = -speed * MetaComponents::deltaTime;
    } else if (primaryDirection == SpriteComponents::Direction::LEFT) {
        offsetX = speed * MetaComponents::deltaTime;
    } else if (primaryDirection == SpriteComponents::Direction::DOWN) {
        offsetY = -speed * MetaComponents::deltaTime;
    } else if (primaryDirection == SpriteComponents::Direction::UP) {
        offsetY = speed * MetaComponents::deltaTime;
    }

    // Calculate offset for secondary direction (if provided)
    if (secondaryDirection == SpriteComponents::Direction::RIGHT) {
        offsetX = -speed * MetaComponents::deltaTime;
    } else if (secondaryDirection == SpriteComponents::Direction::LEFT) {
        offsetX = speed * MetaComponents::deltaTime;
    } else if (secondaryDirection == SpriteComponents::Direction::DOWN) {
        offsetY = -speed * MetaComponents::deltaTime;
    } else if (secondaryDirection == SpriteComponents::Direction::UP) {
        offsetY = speed * MetaComponents::deltaTime;
    }

    // Move all background sprites by the calculated offsets
    spriteCreated->move(offsetX, offsetY);
    spriteCreated2->move(offsetX, offsetY);
    spriteCreated3->move(offsetX, offsetY);

    float width = spriteCreated->getGlobalBounds().width; 
    float height = spriteCreated->getGlobalBounds().height; 
    sf::Vector2f position1 = spriteCreated->getPosition(); 
    sf::Vector2f position2 = spriteCreated2->getPosition(); 
    sf::Vector2f position3 = spriteCreated3->getPosition(); 
    
    // horizontal wrapping
    if (position1.x + width < viewBounds.left) spriteCreated->setPosition(position2.x + width, position1.y);
    if (position2.x + width < viewBounds.left) spriteCreated2->setPosition(position1.x + width, position2.y);
    if (position1.x > viewBounds.left + viewBounds.width) spriteCreated->setPosition(position2.x - width, position1.y);
    if (position2.x > viewBounds.left + viewBounds.width) spriteCreated2->setPosition(position1.x - width, position2.y);

    // vertical wrapping 
    if (position1.y + height < viewBounds.top) spriteCreated->setPosition(position1.x, position3.y + height);
    if (position3.y + height < viewBounds.top) spriteCreated3->setPosition(position3.x, position1.y + height);
    if (position1.y > viewBounds.top + viewBounds.height) spriteCreated->setPosition(position1.x, position3.y - height);
    if (position3.y > viewBounds.top + viewBounds.height) spriteCreated3->setPosition(position3.x, position1.y - height);

}

sf::FloatRect const Background::getViewBounds(sf::Sprite& spriteNum) const {
    return {
        spriteNum.getGlobalBounds().left, 
        spriteNum.getGlobalBounds().width, 
        spriteNum.getGlobalBounds().top, 
        spriteNum.getGlobalBounds().height
    }; 
}

void Background::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (visibleState) {
        if (spriteCreated) {
            target.draw(*spriteCreated, states);
        }
        if (spriteCreated2) {
            target.draw(*spriteCreated2, states);
        }
        if (spriteCreated3) {
            target.draw(*spriteCreated3, states);
        }
        // if (spriteCreated4) {
        //     target.draw(*spriteCreated4, states);
        // }
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
        if (position.y > Constants::WORLD_HEIGHT + Constants::SPRITE_OUT_OF_BOUNDS_OFFSET ||
            position.x > Constants::WORLD_WIDTH + Constants::SPRITE_OUT_OF_BOUNDS_OFFSET ||
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
        // log_info("Returning bitmask for index " + std::to_string(index));
        return bitMask[index].lock();
    } 
    catch (const std::exception& e) {
        log_error("Error in getBitmask: " + std::string(e.what()) + " | Requested index: " + std::to_string(index));
        throw;  // Rethrow the exception for further handling if necessary
    }
}

/* specialized player position update method */
void Player::updatePlayer(sf::Vector2f newPos) {
    
    changePosition(newPos); 
    updatePos();

    log_info("Player position updated to (" + std::to_string(newPos.x) + ", " + std::to_string(newPos.y) + ")");
}

void Player::changeAnimation() {
    try {
        // Toggle firstTurnInstance based on previous turn
        firstTurnInstance = (prevTurnBool == firstTurnInstance) ? false : true;

        if (animChangeState) {
            elapsedTime += MetaComponents::deltaTime;

            // Change animation only if elapsed time exceeds threshold
            if (elapsedTime > Constants::ANIMATION_CHANGE_TIME) {
                // Update animation index based on 'A' key press
                if (FlagSystem::flagEvents.aPressed) {
                    prevTurnBool = false;
                    currentIndex = 6 + (currentIndex - 6 + 1) % 6; // Range 6 to 11
                } else {
                    prevTurnBool = true;
                    currentIndex = (currentIndex + 1) % 6; // Range 0 to 5
                }

                // Apply new animation frame
                setRects(currentIndex);

                // Reset elapsed time after animation change
                elapsedTime = 0.0f;
            }
        }
    } catch (const std::exception& e) {
        log_error("Error in changing animation: " + std::string(e.what()) + " | Current Index: " + std::to_string(currentIndex));
    }
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
