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

// extern struct timeComponents{
//     float deltaTime;
//     float globalTime;
//     std::vector<float> sceneTimes;

//     // Constructor (corrected name)
//     timeComponents() : deltaTime(0.0f), globalTime(0.0f), sceneTimes() {
//         sceneTimes.push_back(0.0f);  // Correct way to add initial value
//     }

//     // Function to reset time (manual reset instead of memset)
//     void resetTime() {
//         deltaTime = 0.0f;
//         globalTime = 0.0f;
//         sceneTimes.clear();         // Clear the vector
//         sceneTimes.push_back(0.0f); // Add an initial value back if needed
//     }
    
// } TimeComponents;


#endif /* flags_hpp */