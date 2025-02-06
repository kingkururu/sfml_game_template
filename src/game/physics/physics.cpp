//
//  physics.cpp
//  sfml game template
//
//

#include "physics.hpp"

namespace physics {

    Quadtree::Quadtree(float x, float y, float width, float height, size_t level, size_t maxObjects, size_t maxLevels)
        : maxObjects(maxObjects), maxLevels(maxLevels), level(level), bounds(x, y, width, height) {}

    void Quadtree::clear() {
        objects.clear();
        log_info("objects cleared.");
        nodes.clear();
        log_info("Quadtree cleared.");
    }

    std::vector<Sprite*> Quadtree::query(const sf::FloatRect& area) const {
        try {
            std::vector<Sprite*> result;
            if (!bounds.intersects(area)) {
                log_info("Area does not intersect with the quadtree bounds at level " + std::to_string(level));
                return result;
            }

            for (const auto& obj : objects) {
                if (area.intersects(obj->returnSpritesShape().getGlobalBounds())) {
                    result.push_back(obj);
                    // log_info("Sprite added to query result at level " + std::to_string(level));
                }
            }

            for (const auto& node : nodes) {
                auto nodeResult = node->query(area);
                result.insert(result.end(), nodeResult.begin(), nodeResult.end());
            }
            return result;

        } catch (const std::exception& e) {
            log_error("Error during query at level " + std::to_string(level) + ": " + std::string(e.what()));
            return std::vector<Sprite*>();
        }
    }

    bool Quadtree::contains(const sf::FloatRect& bounds) const {
        try {
            bool result = this->bounds.contains(bounds.left, bounds.top) &&
                this->bounds.contains(bounds.left + bounds.width, bounds.top + bounds.height);
            log_info(result ? "Bounds are contained in the quadtree at level " + std::to_string(level) :
                            "Bounds are not contained in the quadtree at level " + std::to_string(level));
            return result;
        } catch (const std::exception& e) {
            log_error("Error during contains check at level " + std::to_string(level) + ": " + std::string(e.what()));
            return false;
        }
    }

    void Quadtree::subdivide() {
        try {
            // Check if we've reached the max level
            if (level >= maxLevels) {
                log_info("Maximum level reached, cannot subdivide further.");
                return;
            }

            float halfWidth = bounds.width / 2;
            float halfHeight = bounds.height / 2;
            float x = bounds.left;
            float y = bounds.top;

            // Create four child nodes with smaller bounds and increment the level
            nodes.push_back(std::make_unique<Quadtree>(x, y, halfWidth, halfHeight, level + 1, maxObjects, maxLevels));
            nodes.push_back(std::make_unique<Quadtree>(x + halfWidth, y, halfWidth, halfHeight, level + 1, maxObjects, maxLevels));
            nodes.push_back(std::make_unique<Quadtree>(x, y + halfHeight, halfWidth, halfHeight, level + 1, maxObjects, maxLevels));
            nodes.push_back(std::make_unique<Quadtree>(x + halfWidth, y + halfHeight, halfWidth, halfHeight, level + 1, maxObjects, maxLevels));

            log_info("Quadtree subdivided into 4 child nodes at level " + std::to_string(level));

            // Redistribute the objects into the appropriate child nodes
            for (auto it = objects.begin(); it != objects.end(); ) {
                bool inserted = false;
                for (auto& node : nodes) {
                    if (node->bounds.intersects((*it)->returnSpritesShape().getGlobalBounds())) {
                        node->objects.push_back(*it);
                        it = objects.erase(it); // Remove object from the current node
                        inserted = true;
                        log_info("Sprite moved to child node at level " + std::to_string(node->level));
                        break;
                    }
                }
                if (!inserted) {
                    ++it;
                }
            }
        } catch (const std::exception& e) {
            log_error("Error during subdivision at level " + std::to_string(level) + ": " + std::string(e.what()));
        }
    }

    void Quadtree::update() {
        try {
            for (auto& sprite : objects) {
                // log_info("Updating quadtree at level " + std::to_string(level));

                if (sprite->getMoveState()) {
                    // Check which node the sprite was in
                    for (auto& node : nodes) {
                        if (node->contains(sprite->returnSpritesShape().getGlobalBounds())) {
                            // Remove sprite from the old node
                            node->objects.erase(std::remove(node->objects.begin(), node->objects.end(), sprite), node->objects.end());
                            log_info("Sprite removed from old node at level " + std::to_string(node->level));
                            break;
                        }
                    }

                    // Insert the sprite back into the quadtree
                    std::unique_ptr<Sprite> spritePtr(sprite);
                    insert(spritePtr);
                    log_info("Sprite updated and inserted into quadtree at level " + std::to_string(level));
                }
            }
        } catch (const std::exception& e) {
            log_error("Error during update at level " + std::to_string(level) + ": " + std::string(e.what()));
        }
    }

    // struct to hold raycast operation results that use vector of sprites
    RaycastResult cachedRaycastResult {}; 

    // falling objects 
    sf::Vector2f freeFall( float speed, sf::Vector2f originalPos){
        return { originalPos.x, originalPos.y += speed * MetaComponents::deltaTime * gravity };
    }

    // object moving in a direction vector
    sf::Vector2f follow( float speed, sf::Vector2f originalPos, sf::Vector2f acceleration, const sf::Vector2f& direction){
        sf::Vector2f movement(direction.x * speed * MetaComponents::deltaTime * acceleration.x, direction.y * speed * MetaComponents::deltaTime * acceleration.y);
        return originalPos + movement;
    }

    // moving x or y positions based on directions
    sf::Vector2f moveLeft( float speed, sf::Vector2f originalPos, sf::Vector2f acceleration){
        return { originalPos.x -= speed * MetaComponents::deltaTime * acceleration.x, originalPos.y };
    }
    sf::Vector2f moveRight( float speed, sf::Vector2f originalPos, sf::Vector2f acceleration){
        return { originalPos.x += speed * MetaComponents::deltaTime * acceleration.x, originalPos.y };
    }
    sf::Vector2f moveUp( float speed, sf::Vector2f originalPos, sf::Vector2f acceleration){
        return { originalPos.x, originalPos.y -= speed * MetaComponents::deltaTime * acceleration.y};
    }
    sf::Vector2f moveDown( float speed, sf::Vector2f originalPos, sf::Vector2f acceleration){
        return { originalPos.x, originalPos.y += speed * MetaComponents::deltaTime * acceleration.y};
    }

    // jumping sprites 
    sf::Vector2f jump(float& elapsedTime, float speed, sf::Vector2f originalPos, sf::Vector2f acceleration){
       if(elapsedTime <= 0.2f){
            return { originalPos.x, originalPos.y -= speed * MetaComponents::deltaTime * gravity * acceleration.y };
        } else if (elapsedTime >= 0.2 && elapsedTime <= 0.4f){
            return { originalPos.x, originalPos.y += speed * MetaComponents::deltaTime * gravity * acceleration.y }; 
        } else {
            FlagSystem::flagEvents.spacePressed = false; 
            elapsedTime = 0.0f; 
            return { originalPos.x, originalPos.y };
        }
    }

// collisions 
    // circle collision 
    bool circleCollision(sf::Vector2f pos1, float radius1, sf::Vector2f pos2, float radius2) {
        // Calculate the distance between the centers of the circles
        float dx = pos1.x - pos2.x;
        float dy = pos1.y - pos2.y;
        float distanceSquared = dx * dx + dy * dy;

        // Calculate the sum of the radii
        float radiusSum = radius1 + radius2;
        float radiusSumSquared = radiusSum * radiusSum;

        // Check if the distance squared is less than or equal to the sum of radii squared
        return distanceSquared <= radiusSumSquared;
    }

    // raycast collision 
    bool raycastPreCollision(const sf::Vector2f obj1position, const sf::Vector2f obj1direction, float obj1Speed, const sf::FloatRect obj1Bounds, sf::Vector2f obj1Acceleration, 
                                const sf::Vector2f obj2position, const sf::Vector2f obj2direction, float obj2Speed, const sf::FloatRect obj2Bounds, sf::Vector2f obj2Acceleration) {
            
        ++cachedRaycastResult.counter;
        std::cout << "calculating raycast collision time" << std::endl; 

        // Calculate the initial relative velocity (obj1 velocity minus obj2 velocity)
        sf::Vector2f relativeVelocity = obj1direction * obj1Speed - obj2direction * obj2Speed;
        sf::Vector2f relativePosition = obj1position - obj2position;

        // Relative acceleration as a scalar difference
        sf::Vector2f relativeAcceleration = obj1Acceleration - obj2Acceleration;

        // Calculate velocity dot products
        float velocityDot = relativeVelocity.x * relativeVelocity.x + relativeVelocity.y * relativeVelocity.y;
        float positionVelocityDot = relativePosition.x * relativeVelocity.x + relativePosition.y * relativeVelocity.y;

        // Avoid division by zero or invalid values
        if (velocityDot == 0  && (relativeAcceleration.x == 0 && relativeAcceleration.y == 0)) {
            std::cout << "No relative motion or acceleration; no collision possible." << std::endl;
            return false;
        }

        // Time of closest approach considering relative acceleration
        float timeToClosestApproach = 0.0f;
        // If there is no relative acceleration, calculate using relative velocity only
        if (relativeAcceleration.x == 0 && relativeAcceleration.y == 0) {
            // Standard case: objects have relative velocity, no relative acceleration
            if (velocityDot != 0) {
                timeToClosestApproach = -positionVelocityDot / velocityDot;
            } else {
                std::cout << "No relative velocity detected; no collision possible." << std::endl;
                return false;
            }
        } else {
            // Case with relative acceleration: solve the quadratic equation
            // Compute a quadratic form: at² + bt + c = 0, where:
            // a = 0.5 * relative acceleration · relative acceleration
            // b = relative velocity · relative acceleration
            // c = relative position · relative velocity
            
            float a = 0.5f * (relativeAcceleration.x * relativeAcceleration.x + relativeAcceleration.y * relativeAcceleration.y);
            float b = relativeVelocity.x * relativeAcceleration.x + relativeVelocity.y * relativeAcceleration.y;
            float c = relativePosition.x * relativeVelocity.x + relativePosition.y * relativeVelocity.y;

            // Solve the quadratic equation for time (if applicable)
            float discriminant = b * b - 4.0f * a * c;

            if (discriminant < 0) {
                std::cout << "No collision; discriminant < 0." << std::endl;
                return false;
            }

            // Calculate the two possible times of closest approach
            float sqrtDiscriminant = std::sqrt(discriminant);
            float time1 = (-b - sqrtDiscriminant) / (2.0f * a);
            float time2 = (-b + sqrtDiscriminant) / (2.0f * a);

            // Choose the smallest positive time
            timeToClosestApproach = std::min(time1, time2);
            
            if (timeToClosestApproach < 0) {
                std::cout << "Closest approach is in the past." << std::endl;
                return false;
            }
        }

        // Store the calculated time of closest approach
        cachedRaycastResult.collisionTimes.emplace_back(timeToClosestApproach);

        // Log the calculated time for debugging
        std::cout << "Calculated Time to Closest Approach: " << timeToClosestApproach << std::endl;

        return true;
    }

    bool boundingBoxCollision(const sf::Vector2f &position1, const sf::Vector2f &size1,
                                const sf::Vector2f &position2, const sf::Vector2f &size2) {

        float xOverlapStart = std::max(position1.x, position2.x);
        float yOverlapStart = std::max(position1.y, position2.y);
        float xOverlapEnd = std::min(position1.x + size1.x, position2.x + size2.x);
        float yOverlapEnd = std::min(position1.y + size1.y, position2.y + size2.y);

        return !(xOverlapStart >= xOverlapEnd || yOverlapStart >= yOverlapEnd); 
    }

    bool pixelPerfectCollision( const std::shared_ptr<sf::Uint8[]>& bitmask1, const sf::Vector2f& position1, const sf::Vector2f& size1,
                                const std::shared_ptr<sf::Uint8[]>& bitmask2, const sf::Vector2f& position2, const sf::Vector2f& size2) {

        // Helper function to get the pixel index in the bitmask
        auto getPixelIndex = [](const sf::Vector2f& size, int x, int y) -> int {
            return (y * static_cast<int>(size.x) + x) * 4; // Each pixel has 4 bytes (RGBA)
        };

        // Calculate the overlapping area between the two objects
        float left = std::max(position1.x, position2.x);
        float top = std::max(position1.y, position2.y);
        float right = std::min(position1.x + size1.x, position2.x + size2.x);
        float bottom = std::min(position1.y + size1.y, position2.y + size2.y);

        // Check AABB collision first
        if (left >= right || top >= bottom) return false; 

        // Check each pixel in the overlapping area
        for (int y = static_cast<int>(top); y < static_cast<int>(bottom); ++y) {
            for (int x = static_cast<int>(left); x < static_cast<int>(right); ++x) {
                // Calculate the position in each bitmask
                int x1 = x - static_cast<int>(position1.x);
                int y1 = y - static_cast<int>(position1.y);
                int x2 = x - static_cast<int>(position2.x);
                int y2 = y - static_cast<int>(position2.y);

                // Get the index of the pixel in each bitmask
                int index1 = getPixelIndex(size1, x1, y1);
                int index2 = getPixelIndex(size2, x2, y2);

                // Check if the pixels' values are non-zero (i.e., not transparent)
                if (bitmask1[index1] == 1 && bitmask2[index2] == 1) {
                // std::cout << "Collision detected at pixel (" << x << ", " << y << ")" << std::endl;
                    return true; // Collision detected
                }
            }
        }
        return false; 
    }
}