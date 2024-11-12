
#include "game/core/game.hpp"

int main(){
    Constants::initialize(); 

    GameManager game1; 
    game1.runGame();

    cleanup_logging(); 
}
