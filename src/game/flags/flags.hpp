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
        std::memset(this, 0, sizeof(*this));
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

// private to scene 
struct SceneEvents{
    bool sceneEnd;
    bool sceneStart; 
    /* add something more here */
    
    SceneEvents() : sceneEnd(false), sceneStart(true) {} 

    void resetFlags(){
        std::memset(this, 0, sizeof(*this));
    }
};

#endif /* flags_hpp */