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

/* flagEvents singleton definition for game states */
extern struct flagEvents{
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

    flagEvents() : wPressed(false), aPressed(false), sPressed(false), dPressed(false), bPressed(false), spacePressed(false), mouseClicked(false) {}

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
    
} FlagEvents;



#endif /* flags_hpp */