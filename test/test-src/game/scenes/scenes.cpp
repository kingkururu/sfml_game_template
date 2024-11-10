//
//  scenes.cpp
//  sfml game template
//
//

#include "scenes.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
// Base scene functions  
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

/* Scene constructure sets up window and sprite respawn times */
Scene::Scene( sf::RenderWindow& gameWindow ) : window(gameWindow), quadtree(0.0f, 0.0f, Constants::WORLD_WIDTH, Constants::WORLD_HEIGHT){ 
    MetaComponents::view = sf::View(Constants::VIEW_RECT); 
    log_info("scene made"); 
}

void Scene::runScene() {
    if (FlagSystem::flagEvents.gameEnd) return; // Early exit if game ended
    
    setTime();

    handleInput();

    respawnAssets();

    handleGameEvents();
    handleGameFlags();
    handleSceneFlags();

    updateDrawablesVisibility();

    update();
    draw();
}

void Scene::draw(){
    window.clear(sf::Color::Black);
    window.display(); 
 }

void Scene::moveViewPortWASD(){
    // move view port 
    if(FlagSystem::flagEvents.aPressed){
        MetaComponents::view.move(sf::Vector2f(-10, 0)); 
    }
    if(FlagSystem::flagEvents.dPressed){
        MetaComponents::view.move(sf::Vector2f(10, 0)); 
    }
    if(FlagSystem::flagEvents.sPressed){
        MetaComponents::view.move(sf::Vector2f(0, 10)); 
    }
    if(FlagSystem::flagEvents.wPressed){
        MetaComponents::view.move(sf::Vector2f(0, -10)); 
    }
}

/* Resets everything for scene to start again. The position, moveState, flagEvents, etc are all reset */
void Scene::restartScene() {
    // re-play background music

    // set sprite movestates to true
   
    // re-set sprite and text positions 

    // clear respawn time vectors or any other unecessary vectors 

    // re-set flagEvents
    sceneEvents.resetFlags(); 
}

/* Handles events from flagEvents; deals with gameEnd state */
void Scene::handleGameFlags(){
    // if flagEvents.gameEnd is true or some event ... do somthing 
    if(FlagSystem::flagEvents.gameEnd){
       
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
// Game Scene #1 down below 
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

/* Gets called once before the main game loop to handle cpu-heavy work only once at the beggining */
void gamePlayScene::createAssets() {
    try {
       // globalTimer.Reset();  

        // Initialize sprites and music here 
        background = std::make_unique<Background>(Constants::BACKGROUND_POSITION, Constants::BACKGROUND_SCALE, Constants::BACKGROUND_TEXTURE);

        player = std::make_unique<Player>(Constants::SPRITE1_POSITION, Constants::SPRITE1_SCALE, Constants::SPRITE1_TEXTURE, Constants::SPRITE1_SPEED, Constants::SPRITE1_ACCELERATION, 
                                          Constants::SPRITE1_ANIMATIONRECTS, Constants::SPRITE1_INDEXMAX, utils::convertToWeakPtrVector(Constants::SPRITE1_BITMASK));
        player->setRects(0); 

        backgroundMusic = std::make_unique<MusicClass>(std::move(Constants::BACKGROUNDMUSIC_MUSIC), Constants::BACKGROUNDMUSIC_VOLUME);
        if(backgroundMusic) backgroundMusic->returnMusic().play(); 
       
        button1 = std::make_unique<Button>(Constants::BUTTON1_POSITION, Constants::BUTTON1_SCALE, Constants::BUTTON1_TEXTURE, 
                                   Constants::BUTTON1_ANIMATIONRECTS, Constants::BUTTON1_INDEXMAX, 
                                   utils::convertToWeakPtrVector(Constants::BUTTON1_BITMASK));
        button1->setRects(0); 
        
        // Initialize individual Tiles in the array
        for (int i = 0; i < Constants::TILES_NUMBER; ++i) {
            tiles1.at(i) = std::make_shared<Tile>(Constants::TILES_SCALE, Constants::TILES_TEXTURE, Constants::TILES_SINGLE_RECTS[i], Constants::TILES_BITMASKS[i], Constants::TILES_BOOLS[i]); 
        }
        tileMap1 = std::make_unique<TileMap>(tiles1.data(), Constants::TILES_NUMBER, Constants::TILEMAP_WIDTH, Constants::TILEMAP_HEIGHT, Constants::TILE_WIDTH, Constants::TILE_HEIGHT, Constants::TILEMAP_FILEPATH, Constants::TILEMAP_POSITION); 

        playerJumpSound = std::make_unique<SoundClass>(Constants::PLAYERJUMP_SOUNDBUFF, Constants::PLAYERJUMPSOUND_VOLUME); 
          
        text1 = std::make_unique<TextClass>(Constants::TEXT_POSITION, Constants::TEXT_SIZE, Constants::TEXT_COLOR, Constants::TEXT_FONT, Constants::TEXT_MESSAGE);
        
        //globalTimer.End("initializing assets in scene 1"); 

        insertItemsInQuadtree(); 
    } 

    catch (const std::exception& e) {
         log_error("Exception in createAssets: " + std::string(e.what()));
    }
}

void gamePlayScene::insertItemsInQuadtree(){
    quadtree.insert((player));  
    quadtree.insert((button1)); 
}

/* Creates more sprites from exisitng textures; avoids heavy cpu work */
void gamePlayScene::respawnAssets(){
    // if certain respawn time variable is less than a certain value, respawn objects
} 

/* delet some sprites if their visibleState is false for memory management */
void gamePlayScene::deleteInvisibleSprites() {
    // Remove invisible sprites
   
   // Remove large-scale unenecessary texts 
}

/* Updating time from GameManager's deltatime; it updates sprite respawn times and also counts 
elapsed times from when bullets were spawned and when space button was pressed */
void gamePlayScene::setTime(){
    // count respawn time here 
    if (FlagSystem::flagEvents.spacePressed){
        MetaComponents::spacePressedElapsedTime += MetaComponents::deltaTime; 
    } else {
        MetaComponents::spacePressedElapsedTime = 0.0f; 
    }
} 

void gamePlayScene::handleInput() {
    handleMouseClick();
    handleSpaceKey(); 
    handleMovementKeys();
}

void gamePlayScene::handleMouseClick() {
    if (FlagSystem::flagEvents.mouseClicked) {
        if (button1->getVisibleState() && 
            physics::boundingBoxCollisionHelper(MetaComponents::mouseClickedPosition_i, *button1)) {
            log_info("button clicked");

            button1->setClickedBool(true);
            FlagSystem::gameScene1Flags.sceneEnd = true;
            FlagSystem::gameSceneNextFlags.sceneStart = true;
            FlagSystem::gameSceneNextFlags.sceneEnd = false;

            window.clear();
        }
    }
}

void gamePlayScene::handleSpaceKey() {
    if (FlagSystem::flagEvents.spacePressed) {
        if (player->getMoveState()) {
            physics::spriteMover(player, physics::jump, MetaComponents::spacePressedElapsedTime);
        }
    }
}

void gamePlayScene::handleMovementKeys() {
    sf::FloatRect background1Bounds = background->getViewBounds(background->returnSpritesShape());
    sf::FloatRect background2Bounds = background->getViewBounds(background->returnSpritesShape2());
    sf::FloatRect viewBounds = MetaComponents::getViewBounds();

    // Left movement
    if (FlagSystem::flagEvents.aPressed) {
        if (!physics::boundingBoxCollisionHelper(*tileMap1, *player) || player->getSpritePos().x > tileMap1->getTileMapPosition().x) {
            physics::spriteMover(player, physics::moveLeft);
        }
    }
    // Right movement
    if (FlagSystem::flagEvents.dPressed) {
        if (!physics::boundingBoxCollisionHelper(*tileMap1, *player) || player->getSpritePos().x + player->getRects().width < tileMap1->getTileMapPosition().x + tileMap1->getTileMapWidth() * tileMap1->getTileWidth()) {
            physics::spriteMover(player, physics::moveRight);
        }
    }
    // Down movement
    if (FlagSystem::flagEvents.sPressed) {
        if (!physics::boundingBoxCollisionHelper(*tileMap1, *player) || player->getSpritePos().y + player->getRects().height < tileMap1->getTileMapPosition().y + tileMap1->getTileMapHeight() * tileMap1->getTileHeight()) {
            physics::spriteMover(player, physics::moveDown);
        }
    }
    // Up movement
    if (FlagSystem::flagEvents.wPressed) {
        if (!physics::boundingBoxCollisionHelper(*tileMap1, *player) || player->getSpritePos().y > tileMap1->getTileMapPosition().y) {
            physics::spriteMover(player, physics::moveUp);
        }
        if (viewBounds.top > background1Bounds.top || viewBounds.top > background2Bounds.top) {
           // MetaComponents::view.move(sf::Vector2f(0, -1));
        }
    }
}

/* Keeps sprites inside screen bounds, checks for collisions, update scores, and sets flagEvents.gameEnd to true in an event of collision */
void gamePlayScene::handleGameEvents() { 
    if(player) physics::spriteMover(player, physics::moveRight); 

    // Example: Query for sprites within a certain area (e.g., 100x100 region)
    sf::FloatRect queryArea(0.0f, 0.0f, Constants::VIEW_SIZE_X, Constants::VIEW_SIZE_Y);
    auto nearbySprites = quadtree.query(queryArea);

    // Handle the nearby sprites (e.g., for collision detection or interaction)
    for (auto& sprite : nearbySprites) {
        // Example: Print out that a nearby sprite was found
        log_info("Found nearby sprite at position: " + std::to_string(sprite->returnSpritesShape().getPosition().x));
    }
    
} 

void gamePlayScene::handleSceneFlags(){
    // do something from its private flag
}

/* Updates sprite and text positions when their moveState is true and their pointers are not null. 
deleteInvisibleSprites is called to destroy invisible sprites for memory management */
void gamePlayScene::update() {
    try {
        // Remove invisible sprites
        deleteInvisibleSprites();

        if (button1) button1->changeAnimation();
        if (background) background->updateBackground(Constants::BACKGROUND_SPEED, Constants::BACKGROUND_MOVING_DIRECTION);

        if (player) {
            player->changeAnimation();
            updatePlayerAndView(); 
        }

        quadtree.update(); 

        // Set the view for the window
        window.setView(MetaComponents::view);
        
    } catch (const std::exception& e) {
        log_error("Exception in updateSprites: " + std::string(e.what()));
    }
}

void gamePlayScene::updatePlayerAndView() {
    if(MetaComponents::spacePressedElapsedTime) return; // don't change view when space clicked, i.e, jumped is true
    // Calculate the center of the view based on the player's position
    float viewCenterX = player->getSpritePos().x;
    float viewCenterY = player->getSpritePos().y;

    // Set the new view center to follow the player's position
    MetaComponents::view.setCenter(viewCenterX, viewCenterY - Constants::SPRITE_OUT_OF_BOUNDS_OFFSET);
}

void gamePlayScene::updateDrawablesVisibility(){
    try{
        if (button1){
            button1->setVisibleState(physics::boundingBoxCollisionHelper(MetaComponents::view, *button1)); 
        }
    }
    catch(const std::exception & e){
         log_error("Exception in updateDrawablesVisibility: " + std::string(e.what()));
    }
}

/* Draws only the visible sprite and texts */
void gamePlayScene::draw() {
    try {
        window.clear(sf::Color::Blue);

        if (background && background->getVisibleState()) {
            window.draw(*background); 
        }

        if (button1 && button1->getVisibleState()) {
            window.draw(*button1); 
        }
        if (tileMap1) window.draw(*tileMap1); 
        
        if (player && player->getVisibleState()) {
            window.draw(*player); 
        }

        window.display(); 
    } 
    
    catch (const std::exception& e) {
         log_error("Exception in draw: " + std::string(e.what()));
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
// Game Scene #2 from down below 
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

void gamePlayScene2::createAssets() {
 try {
        // Initialize sprites and music here 
        background = std::make_unique<Background>(Constants::BACKGROUND_POSITION, Constants::BACKGROUND_SCALE, Constants::BACKGROUND_TEXTURE2);
    } 

    catch (const std::exception& e) {
         log_error("Exception in createAssets: " + std::string(e.what()));
    }
}

void gamePlayScene2::handleInput() {
    moveViewPortWASD();
}

void gamePlayScene2::draw() {
    try {
        window.clear();

        if (background && background->getVisibleState()) {
            window.draw(*background); 
        }
    
        window.display(); 
    } 
    
    catch (const std::exception& e) {
        log_error("Exception in gamePlayScene2 draw: " + std::string(e.what()));
    }
}

void gamePlayScene2::update() {
    try {
        deleteInvisibleSprites();

        window.setView(MetaComponents::view); 
        
    }
    catch (const std::exception& e) {
        log_error("Exception in updateSprites: " + std::string(e.what()));
    }
}