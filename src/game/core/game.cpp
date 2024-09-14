//
//  game.cpp
//  sfml game template
//
//  Created by Sunmyoung Yun on 2024/09
//

#include "game.hpp"

/* GameManager constructor sets up the window, intitializes constant variables, calls the random function, and makes scenes */
GameManager::GameManager()
    : window(sf::VideoMode(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT), Constants::GAME_TITLE, sf::Style::Titlebar | sf::Style::Close) {
    Constants::initialize();
    window.setFramerateLimit(Constants::FRAME_LIMIT);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    scene = std::make_unique<Scene>(window);
 //   init_logging();
    log_info("\tGame initialized");
}

/* runGame calls to createAssets from scenes and loops until window is closed to run scene events */
void GameManager::runGame() {
    try {
        scene->createAssets();

        while (window.isOpen()) {
            countTime();
            handleEventInput();
            scene->runScene(deltaTime, globalTime); 
        }
        
    } catch (const std::exception& e) {
        std::cerr << "exception in runGame: " << e.what() << std::endl;
    }
}

/* countTime counts global time and delta time for scenes to later use in runScene */
void GameManager::countTime() {
    sf::Time frameTime = clock.restart();
    deltaTime = frameTime.asSeconds(); 
    globalTime += deltaTime;
}

/* handleEventInput takes in keyboard and mouse input. It modifies FlagEvents and calls setMouseClickedPos in scene to 
pass in the position in screen where mouse was clicked */
void GameManager::handleEventInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
                case sf::Keyboard::A:
                    FlagEvents.aPressed = true;
                    break;
                case sf::Keyboard::S:
                    FlagEvents.sPressed = true;
                    break;
                case sf::Keyboard::W:
                    FlagEvents.wPressed = true;
                    break;
                case sf::Keyboard::D:
                    FlagEvents.dPressed = true;
                    break;
                case sf::Keyboard::B:
                    FlagEvents.bPressed = true;
                    break;
                case sf::Keyboard::Space:
                    FlagEvents.spacePressed = true;
                    break;
                default:
                    break;
            }
        }
        if (event.type == sf::Event::KeyReleased) {
            FlagEvents.flagKeyReleased(); 
        }
        if (event.type == sf::Event::MouseButtonPressed) {
            FlagEvents.mouseClicked = true;
            scene->setMouseClickedPos(sf::Mouse::getPosition(window)); 
        }
        if (event.type == sf::Event::MouseButtonReleased) {
            FlagEvents.mouseClicked = false;
        }
    }
}
