//
//  main.cpp
//  sfml game template
//
//  Created by Sunmyoung Yun on 2024/09
//

/* This is the main function where the GameManager gets called to instantiate a game and run the game. */

#include "game.hpp"

// can add more games here, but must modify GameManger first
int main(){
    GameManager game1;

    game1.runGame();
    
    return 0; 
}
