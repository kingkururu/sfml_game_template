//
//  flags.hpp
//  sfml game template
//
//  Created by Sunmyoung Yun on 2024/09
//

#pragma once

#ifndef flags_hpp
#define flags_hpp

#include <stdio.h>
#include <cstring>
#include <vector>

#include "log.hpp"

/* flagEvents singleton definition for game states (visible globally) */
extern struct FlagEvents{
    // game states
    bool gameEnd; 

    // keyboard inputs
    bool wPressed;
    bool aPressed;
    bool sPressed;
    bool dPressed;
    bool bPressed;
    bool spacePressed; 
    bool mouseClicked;

    FlagEvents() : wPressed(false), aPressed(false), sPressed(false), dPressed(false), bPressed(false), spacePressed(false), mouseClicked(false) {}

    // resets every flag
    void resetFlags(){
        log_error("Failed resetting flagEvents");
        std::memset(this, 0, sizeof(*this));
        log_info("Flags have been successfully reset.");
    }

    // resets keyboard flags only 
    void flagKeyReleased(){
        wPressed = false;
        aPressed = false;
        sPressed = false;
        dPressed = false;
        bPressed = false;
    }
    
} flagEvents;

struct SceneEvents{
    bool sceneEnd;
    bool sceneStart; 
    /* add something more here */
    
    SceneEvents() : sceneEnd(false), sceneStart(true) {} 

    void resetFlags(){
        log_error("failed resetting scene Flags");
        std::memset(this, 0, sizeof(*this));
        log_info("Flags have been successfully reset.");
    }
};

struct GameSceneEvents1{
    bool sceneEnd;
    bool sceneStart; 
    /* add something more here */
    
    GameSceneEvents1() : sceneEnd(false), sceneStart(true) {} 

    void resetFlags(){
        log_error("failed resetting scene Flags");
        std::memset(this, 0, sizeof(*this));
        log_info("Flags have been successfully reset.");
    }
};

extern SceneEvents gameSceneNextFlags; // Declare the global instance
extern GameSceneEvents1 gameSceneEvents1Flags; // Declare the global instance

#endif /* flags_hpp */