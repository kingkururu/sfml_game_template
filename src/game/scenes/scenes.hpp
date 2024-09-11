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
    void spawnBullets(); 
    void handleGameFlags(); 
    
private:
    /* Includes entities such as sprites, sounds, texts, and other game components */
    // Sprites
    std::unique_ptr<Background> background;
    std::unique_ptr<Player> playerSprite;
    std::vector<std::unique_ptr<Bullet>> bullets;
    std::vector<std::unique_ptr<Obstacle>> bushes;
    std::vector<std::unique_ptr<Obstacle>> slimes;

    // Music and sounds
    std::unique_ptr<MusicClass> backgroundMusic;
    std::unique_ptr<SoundClass> playerJumpSound;

    // Fonts and texts
    std::unique_ptr<TextClass> endingText;

    // other game components 
    sf::RenderWindow& window; 

    //initial score
    unsigned int score = Constants::INITIAL_SCORE;
   
    //delta time and global time passed on from GameManager class
    float deltaTime {}; 
    float globalTime {}; 

    //position in screen where mouse was clicked
    sf::Vector2i mouseClickedPos {}; 

    // respawn times for sprites 
    float slimeRespTime {};
    float bushRespTime {}; 
    float bulletRespTime {}; 

    // times elapsed since bullet was spawned 
    std::vector<float> bulletSpawnedTimes; 
    
    // time elapsed since space button was pressed 
    float spacePressedElapsedTime {}; 
};


#endif /* scenes_hpp */