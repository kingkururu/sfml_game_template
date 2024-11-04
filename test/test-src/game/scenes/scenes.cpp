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
Scene::Scene( sf::RenderWindow& gameWindow ) : window(gameWindow) /* initialize other elements here */ { 
    MetaComponents::view = sf::View(Constants::VIEW_RECT); 
    log_info("scene made"); 
}

void Scene::runScene() {
    if (FlagSystem::flagEvents.gameEnd) return; // Early exit if game ended
    
    setTime();
    handleInput();
    respawnAssets();
    handleGameEvents();
    updateDrawablesVisibility();
    handleGameFlags();
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
        // Initialize sprites and music here 
        background = std::make_unique<Background>(Constants::BACKGROUND_POSITION, Constants::BACKGROUND_SCALE, Constants::BACKGROUND_TEXTURE);
        backgroundMusic = std::make_unique<MusicClass>(std::move(Constants::BACKGROUNDMUSIC_MUSIC), Constants::BACKGROUNDMUSIC_VOLUME);
        button1 = std::make_unique<Button>(Constants::BUTTON1_POSITION, Constants::BUTTON1_SCALE, Constants::BUTTON1_TEXTURE, 
                                   Constants::BUTTON1_ANIMATIONRECTS, Constants::BUTTON1_INDEXMAX, 
                                   utils::convertToWeakPtrVector(Constants::BUTTON1_BITMASK));
        button1->setRects(0); 
        //  if (backgroundMusic) backgroundMusic->returnMusic().play(); 
        
        //tileMap1 = std::make_unique<TileMap>(Constants::MAP_WIDTH, Constants::MAP_HEIGHT, Constants::TILE_WIDTH, Constants::TILE_HEIGHT, std::move(tile1)); 

        // Initialize individual Tiles in the array
        for (int i = 0; i < Constants::TILES_NUMBER; ++i) {
            tiles1.at(i) = std::make_unique<Tile>(Constants::TILES_SCALE, Constants::TILES_TEXTURE, Constants::TILES_SINGLE_RECTS[i], Constants::TILES_BITMASKS[i], Constants::TILES_BOOLS[i]); 
        }

        playerJumpSound = std::make_unique<SoundClass>(Constants::PLAYERJUMP_SOUNDBUFF, Constants::PLAYERJUMPSOUND_VOLUME); 
          
        text1 = std::make_unique<TextClass>(Constants::TEXT_POSITION, Constants::TEXT_SIZE, Constants::TEXT_COLOR, Constants::TEXT_FONT, Constants::TEXT_MESSAGE);
         
        } 

    catch (const std::exception& e) {
         log_error("Exception in createAssets: " + std::string(e.what()));
    }
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
} 

/* deals with inputs from device, let known by flagEvents. */
void gamePlayScene::handleInput() {
    moveViewPortWASD();

    if(FlagSystem::flagEvents.mouseClicked){
        if ( button1->getVisibleState() && 
            physics::boundingBoxCollisionHelper(MetaComponents::mouseClickedPosition, *button1)){
            log_info("button clicked"); 

            button1->setClickedBool(true);

            FlagSystem::gameScene1Flags.sceneEnd = true;
            FlagSystem::gameSceneNextFlags.sceneStart = true;
            FlagSystem::gameSceneNextFlags.sceneEnd = false;

            window.clear();
            return; // put here for anything that could be added below in the future
        }
    }
}

/* makes the view only stay inside background sprite  */ 
void gamePlayScene::moveViewPortWASD(){
    if (!background) return; 

    // Define boundaries based on the background sprite
    float bgLeft = background->returnSpritesShape().getGlobalBounds().left;
    float bgRight = bgLeft + background->returnSpritesShape().getGlobalBounds().width;
    float bgTop = background->returnSpritesShape().getGlobalBounds().top;
    float bgBottom = bgTop + background->returnSpritesShape().getGlobalBounds().height;

    float bgLeft2 = background->returnSpritesShape2().getGlobalBounds().left;
    float bgRight2 = bgLeft2 + background->returnSpritesShape2().getGlobalBounds().width;
    float bgTop2 = background->returnSpritesShape2().getGlobalBounds().top;
    float bgBottom2 = bgTop2 + background->returnSpritesShape2().getGlobalBounds().height;

    // Get the current view boundaries
    sf::FloatRect viewBounds(
        MetaComponents::view.getCenter().x - MetaComponents::view.getSize().x / 2,
        MetaComponents::view.getCenter().y - MetaComponents::view.getSize().y / 2,
        MetaComponents::view.getSize().x,
        MetaComponents::view.getSize().y
    );

    // Move viewport only if within background y-pos (doesn't get above or below)
    if(FlagSystem::flagEvents.aPressed && 
    (viewBounds.left > bgLeft || viewBounds.left > bgLeft2)){
        MetaComponents::view.move(sf::Vector2f(-10, 0)); 
    }
    if(FlagSystem::flagEvents.dPressed && 
    (viewBounds.left + viewBounds.width < bgRight || viewBounds.left + viewBounds.width < bgRight2 )){
        MetaComponents::view.move(sf::Vector2f(10, 0)); 
    }
    if(FlagSystem::flagEvents.sPressed && 
    (viewBounds.top + viewBounds.height < bgBottom || viewBounds.top + viewBounds.height < bgBottom2 )){
        MetaComponents::view.move(sf::Vector2f(0, 10)); 
    }
    if(FlagSystem::flagEvents.wPressed && 
    (viewBounds.top > bgTop || viewBounds.top > bgTop2 )){
        MetaComponents::view.move(sf::Vector2f(0, -10)); 
    }

}

/* Keeps sprites inside screen bounds, checks for collisions, update scores, and sets flagEvents.gameEnd to true in an event of collision */
void gamePlayScene::handleGameEvents() { 
    // increase score

    // sprite collisions bool collision = physics::checkCollisions(sprite1, sprites, collisiontype) 

   // if collision, modify a sceneEvent

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

        if(button1) button1->changeAnimation();
        if(background) background->updateBackground(Constants::BACKGROUND_SPEED, Constants::BACKGROUND_MOVING_DIRECTION);
                
        window.setView(MetaComponents::view);
        
    }
    catch (const std::exception& e) {
        log_error("Exception in updateSprites: " + std::string(e.what()));
    }
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
        window.clear();

        if (background && background->getVisibleState()) {
            window.draw(*background); 
        }
        if (button1 && button1->getVisibleState()) {
            window.draw(*button1); 
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