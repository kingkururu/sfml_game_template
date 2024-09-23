//
//  scenes.cpp
//  sfml game template
//
//  Created by Sunmyoung Yun on 2024/09
//

#include "scenes.hpp"

/* Scene constructure sets up window and sprite respawn times */
Scene::Scene( sf::RenderWindow& gameWindow ) : window(gameWindow), scene1View(Constants::VIEW_RECT) /* initialize other elements here */ {
    initialScore = Constants::INITIAL_SCORE; 
}

/* runScene that takes in delta time and global time from GameManager class to execute scene logic */
void Scene::runScene(float deltaT, float globalT){
    deltaTime = deltaT;
    globalTime = globalT; 
    
    /* inside the while loop in game, only runs when gameEnd state is false */
    if (!flagEvents.gameEnd) {
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
        // Initialize sprites and music here 
         background = std::make_unique<Background>(Constants::BACKGROUND_POSITION, Constants::BACKGROUND_SCALE, Constants::BACKGROUND_TEXTURE);

        } 

    catch (const std::exception& e) {
        std::cerr << "Exception caught in createAssets: " << e.what() << std::endl;
    }
}

/* Creates more sprites from exisitng textures; avoids heavy cpu work */
void Scene::respawnAssets(){
    // if certain respawn time variable is less than a certain value, respawn objects
} 

/* Updating time from GameManager's deltatime; it updates sprite respawn times and also counts 
elapsed times from when bullets were spawned and when space button was pressed */
void Scene::setTime(){
    // count respawn time here 
} 

/* Updates mouse position from GameManager input */
void Scene::setMouseClickedPos(sf::Vector2i mousePos){
    mouseClickedPos = mousePos; 
} 

/* Draws only the visible sprite and texts */
void Scene::draw() {
    try {
        window.clear();
        // if pointer to a sprite or text exists, window.draw(sf::sprite); 

        if (background && background->getVisibleState()) {
            window.draw(background->returnSpritesShape());
            window.draw(background->returnSpritesShape2());
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
        // Update sprites if their getMoveState() is true by changeAnimation() and/or updatePos()
        // if (background && background->getBackgroundMoveState()) {
        //     background->updateBackground(deltaTime, Constants::BACKGROUND_SPEED);
        // } 

        // Remove invisible sprites
        deleteInvisibleSprites();

        window.setView(scene1View.getView()); 
    }

    catch (const std::exception& e) {
        std::cerr << "Exception in updateSprites: " << e.what() << std::endl;
    }
}

/* deals with inputs from device, let known by flagEvents. If player's MoveState is true and its pointer is not null, it updates player's position */
void Scene::handleInput() {
    // if player's getMoveState() is true, and flagEvents.somekeypressed is true, updatePlayer(sf::Vector2f newPosition)
    if(flagEvents.aPressed){
        if(scene1View)
            scene1View.getView().move(sf::Vector2f(-1, 0)); 
    }
    if(flagEvents.dPressed){
        if(scene1View)
            scene1View.getView().move(sf::Vector2f(1, 0)); 
    }
    if(flagEvents.sPressed){
        if(scene1View)
            scene1View.getView().move(sf::Vector2f(0, 1)); 
    }
    if(flagEvents.wPressed){
        if(scene1View)
            scene1View.getView().move(sf::Vector2f(0, -1)); 
    }
}

/* Keeps sprites inside screen bounds, checks for collisions, update scores, and sets flagEvents.gameEnd to true in an event of collision */
void Scene::handleGameEvents() { 
    // increase score

    // sprite collisions bool collision = physics::checkCollisions(sprite1, sprites, collisiontype) 
   
   // if collision, set a game event 
} 

/* Handles events from flagEvents; deals with gameEnd state */
void Scene::handleGameFlags(){
    // if flagEvents.gameEnd is true or some event ... do somthing 
    if(flagEvents.gameEnd){
       
    }
}

/* Resets everything for scene to start again. The position, moveState, flagEvents, etc are all reset */
void Scene::restart() {
    // re-play background music

    // set sprite movestates to true
   
    // re-set sprite and text positions 

    // clear respawn time vectors or any other unecessary vectors 

    // re-set flagEvents
    flagEvents.resetFlags(); 
}

/* deletes sprites if their visibleState is false for memory management */
void Scene::deleteInvisibleSprites() {
    // Remove invisible sprites
   
   // Remove large-scale unenecessary texts 
}