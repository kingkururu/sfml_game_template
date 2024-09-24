//
//  scenes.hpp
//  sfml game template
//
//  Created by Sunmyoung Yun on 2024/09
//
#pragma once

#ifndef scenes_hpp
#define scenes_hpp

#include <iostream>
#include <stdexcept>

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include "sprites.hpp"
#include "sound.hpp"
#include "fonts.hpp"
#include "constants.hpp"
#include "flags.hpp"
#include "physics.hpp"
#include "utils.hpp"
#include "window.hpp"

/* Base scene class */
class Scene {
 public:
  Scene( sf::RenderWindow& gameWindow );
  virtual ~Scene() = default; 

  // base functions inside scene
  void runScene(float deltaTime, float globalTime);  
  void restartScene();
  void handleGameFlags(); 
  void setMouseClickedPos(sf::Vector2i mouseClickedPos); 

  // blank templates here
  virtual void createAssets(){}; 
  virtual void respawnAssets(){}; 

  virtual void deleteInvisibleSprites(){};  

  virtual void setTime(){}; 

  virtual void handleInput(){};

  virtual void handleGameEvents(){};
  virtual void handleSceneFlags(){}; 

  virtual void update(){};
  virtual void draw(){}; 
    
 protected:
  // Times - copied from game.cpp
  float deltaTime {}; 
  float globalTime {}; 

  // Other game components 
  sf::RenderWindow& window; // from game.hpp

  GameView sceneView; 
  SceneEvents sceneEvents; 

  // Position in screen where mouse was clicked
  sf::Vector2i mouseClickedPos {}; 
};

/* costumize scenes by adding something */
class introScene : public virtual Scene{
  public:
    using Scene::Scene; 
    ~introScene() override = default; 
    
  private:

};

/* Main game play class, scene instance for creating scenes to run inside GameManager class */
class gamePlayScene : public virtual Scene{
 public:
  using Scene::Scene; 
  ~gamePlayScene() override = default; 
 
  using Scene::createAssets;
  void createAssets() override; 
  using Scene::respawnAssets;
  void respawnAssets() override; 

  using Scene::deleteInvisibleSprites;
  void deleteInvisibleSprites() override;

  using Scene::setTime;
  void setTime() override;

  using Scene::handleInput;
  void handleInput() override; 

  using Scene::handleGameEvents;
  void handleGameEvents() override; 
  using Scene::handleSceneFlags;
  void handleSceneFlags() override; 

  using Scene::update; 
  void update() override; 
  using Scene::draw; 
  void draw() override; 

 private:
  std::unique_ptr<Background> background; 

};

#endif /* scenes_hpp */