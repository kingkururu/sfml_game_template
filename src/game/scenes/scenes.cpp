//
//  scenes.cpp
//  sfml game template
//
//  Created by Sunmyoung Yun on 2024/09
//

#include "scenes.hpp"

/* Scene constructure sets up window and sprite respawn times */
Scene::Scene( sf::RenderWindow& gameWindow ) : window(gameWindow), slimeRespTime(Constants::OBSTACLE_INITIAL_RESPAWN_TIME), bushRespTime(Constants::BUSH_INITIAL_RESPAWN_TIME), bulletRespTime(Constants::BULLET_RESPAWN_TIME){}

/* runScene that takes in delta time and global time from GameManager class to execute scene logic */
void Scene::runScene(float deltaT, float globalT){
    deltaTime = deltaT;
    globalTime = globalT; 
    
    /* inside the while loop in game, only runs when gameEnd state is false */
    if (!FlagEvents.gameEnd) {
        setTime();
        handleInput();
        respawnAssets(); 
        handleGameEvents(); 
    }
    /* inside the while loop in game, always runs */
    handleGameFlags(); 
    update();
    draw();
}

/* Gets called once before the main game loop to handle cpu-heavy work only once at the beggining */
void Scene::createAssets() {
    try {
        // Initialize sprites
        background = std::make_unique<Background>(Constants::BACKGROUND_POSITION, Constants::BACKGROUND_SCALE, Constants::BACKGROUND_TEXTURE);
        playerSprite = std::make_unique<Player>(Constants::PLAYER_POSITION, Constants::PLAYER_SCALE, Constants::PLAYER_TEXTURE, Constants::PLAYERSPRITE_RECTS, Constants::PLAYERANIM_MAX_INDEX, utils::convertToWeakPtrVector(Constants::PLAYER_BITMASKS));
        playerSprite->setRects(0);
        bushes.push_back(std::make_unique<Obstacle>(Constants::BUSH_POSITION, Constants::BUSH_SCALE, Constants::BUSH_TEXTURE, Constants::BUSHSPRITES_RECTS, Constants::BUSHANIM_MAX_INDEX, utils::convertToWeakPtrVector(Constants::BUSH_BITMASKS)));
        slimes.push_back(std::make_unique<Obstacle>(Constants::makeObstaclePosition(), Constants::OBSTACLE_SCALE, Constants::OBSTACLE_TEXTURE, Constants::OBSTACLESPRITE_RECTS, Constants::OBSTACLEANIM_MAX_INDEX, utils::convertToWeakPtrVector(Constants::OBSTACLE_BITMASKS)));
        slimes[0]->setRects(0);
        slimes[0]->setDirectionVector(Constants::OBSTACLE_FALL_ANGLE);

        // Initialize sounds and music
        playerJumpSound = std::make_unique<SoundClass>(Constants::PLAYERJUMP_SOUNDBUFF, Constants::PLAYERJUMPSOUND_VOLUME);
        backgroundMusic = std::make_unique<MusicClass>(std::move(Constants::BACKGROUNDMUSIC_MUSIC), Constants::BACKGROUNDMUSIC_VOLUME);
        if(backgroundMusic)
            backgroundMusic->returnMusic().play();

        // Initialize text
        endingText = std::make_unique<TextClass>(Constants::TEXT_POSITION, Constants::TEXT_SIZE, Constants::TEXT_COLOR, Constants::TEXT_FONT, Constants::TEXT_MESSAGE);
        } 

    catch (const std::exception& e) {
        std::cerr << "Exception caught in createAssets: " << e.what() << std::endl;
    }
}

/* Creates more sprites from exisitng textures; avoids heavy cpu work */
void Scene::respawnAssets(){
    if(slimeRespTime <= 0){
        float newSlimeInterval = Constants::OBSTACLE_INITIAL_RESPAWN_TIME - (globalTime * Constants::OBSTACLE_INTERVAL_DECREMENT);
        slimes.push_back(std::make_unique<Obstacle>(Constants::makeObstaclePosition(), Constants::OBSTACLE_SCALE, Constants::OBSTACLE_TEXTURE, Constants::OBSTACLESPRITE_RECTS, Constants::OBSTACLEANIM_MAX_INDEX, utils::convertToWeakPtrVector(Constants::OBSTACLE_BITMASKS)));
        slimes[slimes.size() - 1]->setRects(0);
        slimes[slimes.size() - 1]->setDirectionVector(Constants::OBSTACLE_FALL_ANGLE);

        slimeRespTime = std::max(newSlimeInterval, Constants::OBSTACLE_INITIAL_RESPAWN_TIME);
    }
    if(bushRespTime <= 0){
        float newBushInterval = Constants::BUSH_INITIAL_RESPAWN_TIME - (globalTime * Constants::BUSH_INTERVAL_DECREMENT);
        bushes.push_back(std::make_unique<Obstacle>(Constants::BUSH_POSITION, Constants::BUSH_SCALE, Constants::BUSH_TEXTURE, Constants::BUSHSPRITES_RECTS, Constants::BUSHANIM_MAX_INDEX, utils::convertToWeakPtrVector(Constants::BUSH_BITMASKS)));
        bushRespTime = std::max(newBushInterval, Constants::BUSH_INITIAL_RESPAWN_TIME);
    }
} 

/* Specialized spawnBullets to re-spawn bullets */
void Scene::spawnBullets(){
    if (bulletRespTime <= 0){
        bullets.push_back(std::make_unique<Bullet>(playerSprite->getSpritePos() + Constants::BULLET_POS_OFFSET, Constants::BULLET_SCALE, Constants::BULLET_TEXTURE, Constants::BULLETSPRITES_RECTS, Constants::BULLETANIM_MAX_INDEX, utils::convertToWeakPtrVector(Constants::BULLET_BITMASKS)));
        bullets[bullets.size() - 1]->setDirectionVector(mouseClickedPos);
        bulletRespTime = Constants::BULLET_RESPAWN_TIME; 
        bulletSpawnedTimes.push_back(deltaTime); 
    }
} 

/* Updating time from GameManager's deltatime; it updates sprite respawn times and also counts 
elapsed times from when bullets were spawned and when space button was pressed */
void Scene::setTime(){
    slimeRespTime -= deltaTime; 
    bulletRespTime -= deltaTime; 
    bushRespTime -= deltaTime; 
    
    for (auto& time : bulletSpawnedTimes){
        time += deltaTime; 
    }

    if (FlagEvents.spacePressed){
        spacePressedElapsedTime += deltaTime; 
    } else {
        spacePressedElapsedTime = 0.0f; 
    }
} 

/* Updates mouse position from GameManager input */
void Scene::setMouseClickedPos(sf::Vector2i mousePos){
    mouseClickedPos = mousePos; 
} 

/* Draws only the visible sprite and texts */
void Scene::draw() {
    try {
        window.clear();

        if (background && background->getVisibleState()) {
            window.draw(background->returnSpritesShape());
            window.draw(background->returnSpritesShape2());
        }
        if (playerSprite && playerSprite->getVisibleState()) {
            window.draw(playerSprite->returnSpritesShape());
        }
        for (const auto& bullet : bullets) {
            if (bullet && bullet->getVisibleState()) {
                window.draw(bullet->returnSpritesShape());
            }
        }
        for (const auto& bush : bushes) {
            if (bush && bush->getVisibleState()) {
                window.draw(bush->returnSpritesShape());
            }
        }
        for (const auto& slime : slimes) {
            if (slime && slime->getVisibleState()) {
                window.draw(slime->returnSpritesShape());
            }
        }
        if (endingText && endingText->getVisibleState()) {
            window.draw(endingText->getText());
        }
        
        window.display(); 
    } 
    
    catch (const std::exception& e) {
        std::cerr << "Exception in draw: " << e.what() << std::endl;
    }
}

/* Updates sprite and text positions when their moveState is true and their pointers are not null. 
deleteInvisibleSprites is called to destroy invisible sprites for memory management */
void Scene::update() {
    try {
       // Update background if it should move
        if (background && background->getBackgroundMoveState()) {
            background->updateBackground(deltaTime, Constants::BACKGROUND_SPEED);
        } 
        // Update player if it should move
        if (playerSprite && playerSprite->getMoveState()) {
            playerSprite->changeAnimation(deltaTime);
            playerSprite->updatePos();
        } 
        // Update slimes
        for (auto& slime : slimes) {
            if (slime) { 
                if (slime->getMoveState()) {
                    slime->changePosition(physics::follow(deltaTime, Constants::OBSTACLE_SPEED, slime->getSpritePos(), Constants::OBSTACLE_ACCELERATION, slime->getDirectionVector()));
                    slime->changeAnimation(deltaTime);
                    slime->updatePos();
                }
            } else {
                std::cerr << "Error: slime pointer is null" << std::endl;
            }
        }
        // Update bushes
        for (auto& bush : bushes) {
            if (bush) { 
                if (bush->getMoveState()) {
                    bush->changePosition(physics::moveLeft(deltaTime, Constants::BUSH_SPEED, bush->getSpritePos(), Constants::BUSH_ACCELERATION));
                    bush->updatePos();
                }
            } else {
                std::cerr << "Error: bush pointer is null" << std::endl;
            }
        }
        // Update bullets
        for (auto& bullet : bullets) {
            if (bullet) { 
                if (bullet->getMoveState()) {
                    bullet->changePosition(physics::follow(deltaTime, Constants::BULLET_SPEED, bullet->getSpritePos(), Constants::BULLET_ACCELERATION, bullet->getDirectionVector()));
                    bullet->updatePos();
                }
            } else {
                std::cerr << "Error: bullet pointer is null" << std::endl;
            }
        }

        // Remove invisible sprites
        deleteInvisibleSprites();
    }

    catch (const std::exception& e) {
        std::cerr << "Exception in updateSprites: " << e.what() << std::endl;
    }
}

/* deals with inputs from device, let known by FlagEvents. If player's MoveState is true and its pointer is not null, it updates player's position */
void Scene::handleInput() {
    if(playerSprite && playerSprite->getMoveState()){
        if(FlagEvents.sPressed){
            playerSprite->updatePlayer(physics::moveDown(deltaTime, Constants::PLAYER_SPEED, playerSprite->getSpritePos())); 
        }
        if(FlagEvents.wPressed){
            playerSprite->updatePlayer(physics::moveUp(deltaTime, Constants::PLAYER_SPEED, playerSprite->getSpritePos())); 
        }
        if(FlagEvents.mouseClicked){
            spawnBullets(); 
        }
        if(FlagEvents.spacePressed){
            playerSprite->updatePlayer(physics::jump(spacePressedElapsedTime, deltaTime, Constants::PLAYER_JUMP_SPEED, playerSprite->getSpritePos())); 
        }
    }

    if(FlagEvents.bPressed && backgroundMusic){
        backgroundMusic->returnMusic().stop(); 
        restart();
    }
}

/* Keeps sprites inside screen bounds, checks for collisions, update scores, and sets FlagEvents.gameEnd to true in an event of collision */
void Scene::handleGameEvents() { 
    // increase score
    for (auto it = slimes.begin(); it != slimes.end(); ) {
        if ((*it) && (*it)->getSpritePos().x < playerSprite->getSpritePos().x - Constants::PASSTHROUGH_OFFSET){
            ++score; 
            (*it)->setVisibleState(false); 
            it = slimes.erase(it);
        } else {
            ++it; 
        }
    }
    for (auto it = bushes.begin(); it != bushes.end(); ) {
        if ((*it) && (*it)->getSpritePos().x < playerSprite->getSpritePos().x - Constants::PASSTHROUGH_OFFSET){
            ++score; 
            (*it)->setVisibleState(false); 
            it = bushes.erase(it);
        } else {
            ++it; 
        }
    }

    // player vs bush collision 
    bool bushCollision = physics::checkCollisions( playerSprite, bushes, physics::pixelPerfectCollisionHelper);  
    bool slimeCollision = physics::checkCollisions( playerSprite, slimes, physics::pixelPerfectCollisionHelper); 
    bool bulletCollision = physics::checkCollisions( bullets, slimes, physics::raycastCollisionHelper, bulletSpawnedTimes);

    if(bulletCollision){
        ++score; 
    }

    if(bushCollision || slimeCollision){
        std::cout << "Ending Game" << std::endl;
        FlagEvents.gameEnd = true; 
  
        if(backgroundMusic)
            backgroundMusic->returnMusic().pause(); 
    }

    if(endingText)
        endingText->updateText("current score: " + std::to_string(score)); 
} 

/* Handles events from FlagEvents; deals with gameEnd state */
void Scene::handleGameFlags(){
    if(FlagEvents.gameEnd){
        if(background)
            background->setBackgroundMoveState(false); 

        if(playerSprite)
            playerSprite->setMoveState(false);

       for (auto& slime : slimes) {
            if(slime){
                slime->setMoveState(false);
            }
        }

        for (auto& bullet : bullets) {
            if(bullet)
                bullet->setMoveState(false);
        }

        for (auto& bush : bushes) {
            if(bush)
                bush->setMoveState(false);
        }

        if(endingText)
            endingText->updateText("Player dead!\nFinal score: " + std::to_string(score)); 
    }
}

/* Resets everything for scene to start again. The position, moveState, FlagEvents, etc are all reset */
void Scene::restart() {
    if(backgroundMusic)
        backgroundMusic->returnMusic().play();

    if (background)
        background->setBackgroundMoveState(true); 

    if(playerSprite){
        playerSprite->setMoveState(true);
        playerSprite->changePosition(Constants::PLAYER_POSITION);
        std::cout << "set player pos at" << Constants::PLAYER_POSITION.x << "and " << Constants::PLAYER_POSITION.y << std::endl; 
    }

    for (auto& slime : slimes) {
        if(slime){
            slime->setMoveState(true);
            slime->changePosition(Constants::makeObstaclePosition()); 
            std::cout << "set slime pos at" << slime->getSpritePos().x << "and " << slime->getSpritePos().y << std::endl; 
        }
    }

    for (auto& bullet : bullets) {
        if(bullet){
            bullet->setMoveState(true);
            bullet->changePosition(Constants::BULLET_POSITION); 
            std::cout << "set bullet pos at" << bullet->getSpritePos().x << "and " << bullet->getSpritePos().y << std::endl; 

        }
    }

    for (auto& bush : bushes) {
        if(bush){
            bush->setMoveState(true);
            bush->changePosition(Constants::BUSH_POSITION);
            std::cout << "set bush pos at" << bush->getSpritePos().x << "and " << bush->getSpritePos().y << std::endl; 

        }
    }

    FlagEvents.resetFlags(); 

    score = 0;

    if(endingText)
        endingText->updateText("current score: " + std::to_string(score)); 

    bullets.clear();
    bulletSpawnedTimes.clear(); 
    slimes.clear(); 
    bushes.clear(); 
}

/* deletes sprites if their visibleState is false for memory management */
void Scene::deleteInvisibleSprites() {
    // Remove invisible slimes
    auto slimeIt = std::remove_if(slimes.begin(), slimes.end(),
                                  [](const std::unique_ptr<Obstacle>& slime) { 
                                    if(slime){
                                        return !slime->getVisibleState();
                                    } 
                                    return false;
                                    });
    slimes.erase(slimeIt, slimes.end());

    // Remove invisible bushes
    auto bushIt = std::remove_if(bushes.begin(), bushes.end(),
                                 [](const std::unique_ptr<Obstacle>& bush) { 
                                    if(bush){
                                        return !bush->getVisibleState();
                                    } 
                                    return false;
                                    });
    bushes.erase(bushIt, bushes.end());

    // Remove invisible bullets
    std::vector<size_t> bulletsToRemove;
    auto bulletIt = std::remove_if(bullets.begin(), bullets.end(),
                                   [&bulletsToRemove, this](const std::unique_ptr<Bullet>& bullet) {
                                       if (bullet && !bullet->getVisibleState()) {
                                           bulletsToRemove.push_back(bullets.size() - 1); // Store index
                                           return true;
                                       }
                                       return false;
                                   });

    bullets.erase(bulletIt, bullets.end());

    // Remove corresponding times
    for (const auto& index : bulletsToRemove) {
        if (index < bulletSpawnedTimes.size()) {
            bulletSpawnedTimes.erase(bulletSpawnedTimes.begin() + index);
        }
    }
}