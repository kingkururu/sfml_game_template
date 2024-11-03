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
