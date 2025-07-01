#pragma once

#include <type_traits>
#include <iostream>
#include <memory>
#include <vector>
#include <array>
#include <stdexcept>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <functional> 
#include <utility>

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
                if (nodes.empty()) { // If no child nodes exist, add the object to this node
                    objects.push_back(obj.get());
                    log_info("Sprite inserted into quadtree node.");
                } else { // Check which child node the object belongs to
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

    // moving object
    constexpr float gravity = 9.8f;
    sf::Vector2f freeFall(float speed, sf::Vector2f originalPo);
    sf::Vector2f jump(float& elapsedTime, float speed, sf::Vector2f originalPos, sf::Vector2f acceleration = {0.1f, 0.1f}); 
    sf::Vector2f jumpToSurface(float& elapsedTime, float speed, sf::Vector2f originalPos, sf::Vector2f acceleration = {0.1f, 0.1f}); 

    sf::Vector2f moveLeft(float speed, sf::Vector2f originalPos, sf::Vector2f acceleration = {1.0f, 1.0f}); 
    sf::Vector2f moveRight(float speed, sf::Vector2f originalPos, sf::Vector2f acceleration = {1.0f, 1.0f});
    sf::Vector2f moveUp(float speed, sf::Vector2f originalPos, sf::Vector2f acceleration = {1.0f, 1.0f});
    sf::Vector2f moveDown(float speed, sf::Vector2f originalPos, sf::Vector2f acceleration = {1.0f, 1.0f});

    sf::Vector2f followDirVec(float speed, sf::Vector2f originalPos, sf::Vector2f acceleration, const sf::Vector2f& direction); 
    sf::Vector2f followDirVecOpposite(float speed, sf::Vector2f originalPos, sf::Vector2f acceleration, const sf::Vector2f& direction); 

    template<typename SpriteType, typename MoveFunc, typename... Args>
    void spriteMover(std::unique_ptr<SpriteType>& sprite, const MoveFunc& moveFunc, Args&&... args) {
        float speed = sprite->getSpeed();
        sf::Vector2f originalPos = sprite->getSpritePos();

        if constexpr (std::is_invocable_v<MoveFunc, float&, float, sf::Vector2f, sf::Vector2f>) {
            // Handle time-based movement with acceleration
            auto&& argsTuple = std::forward_as_tuple(std::forward<Args>(args)...);
            float& elapsedTime = std::get<0>(argsTuple);
            sf::Vector2f acceleration = std::get<1>(argsTuple);
            sprite->changePosition(moveFunc(elapsedTime, speed, originalPos, acceleration));
        } else {
            // Handle standard movement
            sf::Vector2f acceleration = sprite->getAcceleration();
            sf::Vector2f direction = sprite->getDirectionVector();
            
            if constexpr (std::is_invocable_v<MoveFunc, float, sf::Vector2f, sf::Vector2f, sf::Vector2f&>) {
                sprite->changePosition(moveFunc(speed, originalPos, acceleration, direction));
            } else if constexpr (std::is_invocable_v<MoveFunc, float, sf::Vector2f, sf::Vector2f>) {
                sprite->changePosition(moveFunc(speed, originalPos, acceleration));
            } else if constexpr (std::is_invocable_v<MoveFunc, float, sf::Vector2f>) {
                sprite->changePosition(moveFunc(speed, originalPos));
            }
        }
        sprite->updatePos();
    }

    // collision methods
    bool circleCollision(const sf::Vector2f pos1, float radius1, const sf::Vector2f pos2, float radius2);
    // raycast pre-collision in 2D space
    struct RaycastResult {
        std::vector<float> collisionTimes;
        int counter; 
    };    
    extern RaycastResult cachedRaycastResult; 
    bool raycastPreCollision(const sf::Vector2f obj1position, const sf::Vector2f obj1direction, float obj1Speed, const sf::FloatRect obj1Bounds, sf::Vector2f obj1Acceleration, 
                             const sf::Vector2f obj2position, const sf::Vector2f obj2direction, float obj2Speed, const sf::FloatRect obj2Bounds, sf::Vector2f obj2Acceleration);
    bool boundingBoxCollision(const sf::Vector2f &position1, const sf::Vector2f& size1, const sf::Vector2f &position2, const sf::Vector2f& size2);
    bool pixelPerfectCollision( const std::shared_ptr<sf::Uint8[]> &bitmask1, const sf::Vector2f &position1, const sf::Vector2f &size1,
                                const std::shared_ptr<sf::Uint8[]> &bitmask2, const sf::Vector2f &position2, const sf::Vector2f &size2);  
   
    struct CollisionData {
        sf::Vector2f position;
        float radius;
        sf::Vector2f direction;
        float speed;
        sf::Vector2f acceleration;
        sf::Vector2f size;
        std::shared_ptr<sf::Uint8[]> bitmask; 
        sf::FloatRect bounds;
    };

    template<typename Sprite> 
    CollisionData extractCollisionData(Sprite&& sprite) {
        CollisionData data;
        data.bounds = sprite->returnSpritesShape().getGlobalBounds();
        if(!sprite->isCentered()) {
            data.position = sprite->getSpritePos();
        } else {
            data.position = sf::Vector2f{sprite->getSpritePos().x - data.bounds.width / 2, sprite->getSpritePos().y - data.bounds.height / 2};
        }
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

    template<typename ObjType1, typename ObjType2, typename... Args>
    bool collisionHelper(ObjType1&& obj1, ObjType2&& obj2, Args&&... args) {
        auto getSprite = [](auto&& obj) -> auto& {
            if constexpr (std::is_pointer_v<std::decay_t<decltype(obj)>>) return *obj;
            else return obj;
        };

        auto& sprite1 = getSprite(std::forward<ObjType1>(obj1));
        CollisionData data1 = extractCollisionData(sprite1);

        if constexpr (sizeof...(Args) == 0) { // Handle sprite vs. non-sprite (mouse, view, tilemap)
            if constexpr (std::is_same_v<std::decay_t<ObjType2>, sf::Vector2f>) { // mouse
                sf::Vector2f position2(static_cast<float>(obj2.x), static_cast<float>(obj2.y));
                sf::Vector2f size2(1.0f, 1.0f);
                return boundingBoxCollision(data1.position, data1.size, position2, size2);
            } else if constexpr (std::is_same_v<std::decay_t<ObjType2>, sf::View>) { // view
                sf::Vector2f viewCenter = obj2.getCenter();
                sf::Vector2f viewSize = obj2.getSize();
                sf::Vector2f position2(viewCenter.x - viewSize.x / 2, viewCenter.y - viewSize.y / 2);
                sf::Vector2f size2(viewSize.x, viewSize.y);
                return boundingBoxCollision(data1.position, data1.size, position2, size2);
            } else { // tilemap
                auto getTileMap = [](auto&& obj) -> auto& {
                    if constexpr (std::is_pointer_v<std::decay_t<decltype(obj)>> || std::is_same_v<std::decay_t<decltype(obj)>, std::unique_ptr<TileMap>>) return *obj;
                    else return obj; };
                auto& tileMap = getTileMap(obj2);

                if constexpr (std::is_same_v<std::decay_t<decltype(tileMap)>, TileMap>) { // only checks entire tilemap block as a single object
                    sf::Vector2f position2 = tileMap.getTileMapPosition();
                    sf::Vector2f size2(tileMap.getTileWidth() * static_cast<float>(tileMap.getTileMapWidth()),
                                       tileMap.getTileHeight() * static_cast<float>(tileMap.getTileMapHeight()));
                    return boundingBoxCollision(data1.position, data1.size, position2, size2);
                }
                return false;
            }
        } else { // Handle sprite vs. sprite with collision function and optional parameters
            if (!obj1) {
                log_warning("First object is missing in collision detection");
                return false;
            }
            if (!obj2) {
                log_warning("Second object is missing in collision detection");
                return false;
            }

            auto& sprite2 = getSprite(std::forward<ObjType2>(obj2));
            CollisionData data2 = extractCollisionData(sprite2);

            auto&& collisionFunc = std::get<0>(std::forward_as_tuple(std::forward<Args>(args)...));

            Quadtree* quadtree = nullptr;
            if constexpr (sizeof...(Args) >= 2) {
                quadtree = std::get<1>(std::forward_as_tuple(std::forward<Args>(args)...));
            }

            float timeElapsed = 0.0f;
            if constexpr (sizeof...(Args) >= 3) {
                timeElapsed = std::get<2>(std::forward_as_tuple(std::forward<Args>(args)...));
            }

            size_t counterIndex = 0;
            if constexpr (sizeof...(Args) >= 4) {
                counterIndex = std::get<3>(std::forward_as_tuple(std::forward<Args>(args)...));
            }

            auto collisionLambda = [&timeElapsed, counterIndex](const CollisionData& d1, const CollisionData& d2, auto&& func) {
                if constexpr (std::is_invocable_v<decltype(func), sf::Vector2f, float, sf::Vector2f, float>) {
                    return func(d1.position, d1.radius, d2.position, d2.radius);
                } else if constexpr (std::is_invocable_v<decltype(func), sf::Vector2f, sf::Vector2f, sf::Vector2f, sf::Vector2f>) {
                    return func(d1.position, d1.size, d2.position, d2.size);
                } else if constexpr (std::is_invocable_v<decltype(func), sf::Vector2f, sf::Vector2f, float, sf::FloatRect, sf::Vector2f>) {
                    if (!cachedRaycastResult.counter) {
                        return func(d1.position, d1.direction, d1.speed, d1.bounds, d1.acceleration,
                                    d2.position, d2.direction, d2.speed, d2.bounds, d2.acceleration);
                    } else if (timeElapsed > cachedRaycastResult.collisionTimes[counterIndex]) {
                        cachedRaycastResult.counter = 0;
                        return true;
                    }
                } else if constexpr (std::is_invocable_v<decltype(func), std::shared_ptr<sf::Uint8[]>, sf::Vector2f, sf::Vector2f,
                                                                        std::shared_ptr<sf::Uint8[]>, sf::Vector2f, sf::Vector2f>) {
                    return func(d1.bitmask, d1.position, d1.size, d2.bitmask, d2.position, d2.size);
                }
                return false;
            };

            if (quadtree) {
                auto potentialColliders1 = quadtree->query(sprite1->returnSpritesShape().getGlobalBounds());
                auto potentialColliders2 = quadtree->query(sprite2->returnSpritesShape().getGlobalBounds());

                if (potentialColliders1.empty() || potentialColliders2.empty()) return false;

                for (const auto& collider1 : potentialColliders1) {
                    for (const auto& collider2 : potentialColliders2) {
                        if (collider1 == collider2) continue;
                        if (collisionLambda(data1, data2, collisionFunc)) {
                            return true;
                        }
                    }
                }
                return false;
            } else {
                return collisionLambda(data1, data2, collisionFunc);
            }
        }
    }
}    