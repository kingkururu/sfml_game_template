//
//  scenes.hpp
//
//

#pragma once

#include <iostream>
#include <stdexcept>

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <array>

#include "../test-assets/sprites/sprites.hpp"  
#include "../test-assets/sound/sound.hpp"      
#include "../test-assets/fonts/fonts.hpp"      
#include "../test-assets/tiles/tiles.hpp"      

#include "../globals/globals.hpp"              
#include "../physics/physics.hpp"             
#include "../utils/utils.hpp"                 

#include "../test-logging/log.hpp"              

/* Base scene class */
class Scene {
 public:
  Scene( sf::RenderWindow& gameWindow );
  virtual ~Scene() = default; 

  // base functions inside scene
  void runScene();  
  virtual void createAssets(){}; 

 protected:
  sf::RenderWindow& window; // from game.hpp
  FlagSystem::SceneEvents sceneEvents; // scene's own flag events

  // blank templates here
  virtual void deleteInvisibleSprites(){};  

  virtual void setTime(){}; 

  virtual void handleInput(){};

  virtual void respawnAssets(){}; 

  virtual void handleGameEvents(){};
  virtual void handleSceneFlags(){}; 
  virtual void updateDrawablesVisibility(){}; 

  virtual void update(){};
  virtual void draw(); 
  virtual void moveViewPortWASD();

  void restartScene();
  void handleGameFlags(); 
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
 
  void createAssets() override; 

 private:
  void handleInput() override; 
  void handleMouseClick(); 
  void handleSpaceKey();
  void handleMovementKeys(); 

  void respawnAssets() override; 
  void deleteInvisibleSprites() override;

  void setTime() override;

  void handleGameEvents() override; 
  void handleSceneFlags() override; 

  void update() override; 
  void updateDrawablesVisibility() override; 

  void draw() override; 

  std::unique_ptr<Background> background; 
  std::unique_ptr<Player> player; 

  std::array<std::shared_ptr<Tile>, Constants::TILES_NUMBER> tiles1;   
  std::unique_ptr<TileMap> tileMap1; 

  std::unique_ptr<Button> button1;  

  std::unique_ptr<MusicClass> backgroundMusic;
  std::unique_ptr<SoundClass> playerJumpSound; 

  std::unique_ptr<TextClass> text1; 
};

class gamePlayScene2 : public virtual Scene{
public:
  using Scene::Scene; 
  ~gamePlayScene2() override = default; 
 
  void createAssets() override; 

 private:
  void handleInput() override; 

  void draw() override; 
  void update() override; 

  std::unique_ptr<Background> background; 
};

