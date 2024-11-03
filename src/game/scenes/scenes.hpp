//
//  scenes.hpp
//  sfml game template
//
//
#pragma once

#include <iostream>
#include <stdexcept>

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include "sprites.hpp"
#include "sound.hpp"
#include "fonts.hpp"

#include "globals.hpp"
#include "physics.hpp"
#include "utils.hpp"
#include "tiles.hpp"
#include "log.hpp"

/* Base scene class */
class Scene {
 public:
  Scene( sf::RenderWindow& gameWindow );
  virtual ~Scene() = default; 

  // base functions inside scene
  void runScene();  
  void restartScene();
  void handleGameFlags(); 

  // blank templates here
  virtual void createAssets(){}; 
  virtual void respawnAssets(){}; 

  virtual void deleteInvisibleSprites(){};  

  virtual void setTime(){}; 

  virtual void handleInput(){};

  virtual void handleGameEvents(){};
  virtual void handleSceneFlags(){}; 
  virtual void updateDrawablesVisibility(){}; 

  virtual void update(){};
  virtual void draw(); 
  virtual void moveViewPortWASD();

 protected:
  // Other game components 
  sf::RenderWindow& window; // from game.hpp

  FlagSystem::SceneEvents sceneEvents; // scene's own flag events
};
