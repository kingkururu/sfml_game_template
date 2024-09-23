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

/* Scene class for creating scenes to run inside GameManager class */
class Scene {
public:
  Scene( sf::RenderWindow& gameWindow );
    
  void runScene(float deltaTime, float globalTime);  
  void createAssets();
  void respawnAssets(); 
  void update();
  void draw();
  void handleInput();
  void handleGameEvents();
  void restart();
  void setTime(); 
  void deleteInvisibleSprites(); 
  void setMouseClickedPos(sf::Vector2i mouseClickedPos); 
  void handleGameFlags(); 
    
private:
  //initial score
  int initialScore{}; 

  // Sprites 
  std::unique_ptr<Background> background; 
    /*  
    */ 

  // Music and sounds  
    /*  
     ex)std::unique_ptr<MusicClass> backgroundMusic;
    */ 

  //Fonts and texts
       /*   
     ex) std::unique_ptr<TextClass> endingText;
    */ 

  //Times
  float deltaTime {}; 
  float globalTime {}; 

  /* respawn times for sprites  
     ex) float bulletRespTime {};  
    */ 

  /* times elapsed since bullet was spawned  
     ex) std::vector<float> bulletSpawnedTimes;  
    */ 

  /* time elapsed since space button was pressed
     ex) float spacePressedElapsedTime {}; 
    */ 

  // Other game components 
  sf::RenderWindow& window; // from game.hpp

  GameView scene1View; 
  SceneEvents scene1Events; 

  // Position in screen where mouse was clicked
  sf::Vector2i mouseClickedPos {}; 
};


#endif /* scenes_hpp */