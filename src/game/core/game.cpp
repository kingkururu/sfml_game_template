//
//  game.cpp
//  sfml game template
//
//  Created by Sunmyoung Yun on 2024/09
//

#include "game.hpp"

/* GameManager constructor sets up the window, intitializes constant variables, calls the random function, and makes scenes */
GameManager::GameManager()
    : mainWindow(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT, Constants::GAME_TITLE, Constants::FRAME_LIMIT) {

    Constants::initialize();
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // make scenes here
    introScreenScene = std::make_unique<introScene>(mainWindow.getWindow()); 
    gameScene = std::make_unique<gamePlayScene>(mainWindow.getWindow());
    gameSceneNext = std::make_unique<gamePlayScene>(mainWindow.getWindow());

    log_info("\tGame initialized");
}

/* runGame calls to createAssets from scenes and loops until window is closed to run scene events */
void GameManager::runGame() {
    auto logger = spdlog::get("error_logger"); 
    if (!logger) {
        spdlog::error("Logger not initialized.");
        return;
    }

    try {     
    loadScenes(); 

        while (mainWindow.getWindow().isOpen()) {
            countTime();
            handleEventInput();
            runScenesFlags(); 
        }
        log_info("\tGame Ended\n"); 
            
    } catch (const std::exception& e) {
        logger->error("Exception in runGame: {}", e.what());
    }
}

void GameManager::runScenesFlags(){
    log_info("running scenes start");
    if(!gameSceneEvents1Flags.sceneEnd){ 
            log_info("running scene 1");

        gameScene->runScene(deltaTime, globalTime);
    }
    if(gameSceneEvents1Flags.sceneEnd){
        gameSceneNext->runScene(deltaTime, globalTime);
    }
}

void GameManager::loadScenes(){
    introScreenScene->createAssets(); 
    gameScene->createAssets();
    gameSceneNext->createAssets(); 
}

/* countTime counts global time and delta time for scenes to later use in runScene */
void GameManager::countTime() {
    sf::Time frameTime = clock.restart();
    deltaTime = frameTime.asSeconds(); 
    globalTime += deltaTime;
}

/* handleEventInput takes in keyboard and mouse input. It modifies flagEvents and calls setMouseClickedPos in scene to 
pass in the position in screen where mouse was clicked */
void GameManager::handleEventInput() {
    sf::Event event;
    while (mainWindow.getWindow().pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            mainWindow.getWindow().close();
        }
        if (event.type == sf::Event::Resized){ 
            sf::FloatRect visibleArea(0.0f, 0.0f, event.size.width, event.size.height);
            mainWindow.getWindow().setView(sf::View(visibleArea)); 
        }
        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
                case sf::Keyboard::A:
                    flagEvents.aPressed = true;
                    break;
                case sf::Keyboard::S:
                    flagEvents.sPressed = true;
                    break;
                case sf::Keyboard::W:
                    flagEvents.wPressed = true;
                    break;
                case sf::Keyboard::D:
                    flagEvents.dPressed = true;
                    break;
                case sf::Keyboard::B:
                    flagEvents.bPressed = true;
                    break;
                case sf::Keyboard::Space:
                    flagEvents.spacePressed = true;
                    break;
                default:
                    break;
            }
        }
        if (event.type == sf::Event::KeyReleased) {
            flagEvents.flagKeyReleased(); 
        }
        if (event.type == sf::Event::MouseButtonPressed) {
            flagEvents.mouseClicked = true;
            gameScene->setMouseClickedPos(sf::Mouse::getPosition(mainWindow.getWindow())); 
        }
        if (event.type == sf::Event::MouseButtonReleased) {
            flagEvents.mouseClicked = false;
        }
    }
}

