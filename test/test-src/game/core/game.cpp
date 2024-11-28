
#include "game.hpp" 

// GameManager constructor sets up the window, intitializes constant variables, calls the random function, and makes scenes 
GameManager::GameManager()
    : mainWindow(Constants::VIEW_SIZE_X, Constants::VIEW_SIZE_Y, Constants::GAME_TITLE, Constants::FRAME_LIMIT) {
    introScreenScene = std::make_unique<introScene>(mainWindow.getWindow());
    gameScene = std::make_unique<gamePlayScene>(mainWindow.getWindow());
    gameSceneNext = std::make_unique<gamePlayScene2>(mainWindow.getWindow()); 

    log_info("\tGame initialized");
}

// runGame calls to createAssets from scenes and loops until window is closed to run scene events 
void GameManager::runGame() {
    try {     
        loadScenes(); 

        while (mainWindow.getWindow().isOpen()) {
            countTime();
            handleEventInput();
            runScenesFlags(); 
        }
        log_info("\tGame Ended\n"); 
            
    } catch (const std::exception& e) {
        log_error("Exception in runGame: " + std::string(e.what())); 
        mainWindow.getWindow().close(); 
    }
}

void GameManager::runScenesFlags(){
    if(!FlagSystem::flagEvents.gameEnd){
        if(FlagSystem::gameScene1Flags.sceneStart && !FlagSystem::gameScene1Flags.sceneEnd){ 
            gameScene->runScene();
        }

        if(FlagSystem::gameSceneNextFlags.sceneStart  && !FlagSystem::gameSceneNextFlags.sceneEnd){
            gameSceneNext->runScene();
        }
    }
}

void GameManager::loadScenes(){
    introScreenScene->createAssets(); 
    gameScene->createAssets();
    gameSceneNext->createAssets(); 
}

// countTime counts global time and delta time for scenes to later use in runScene 
void GameManager::countTime() {
    sf::Time frameTime = MetaComponents::clock.restart();
    MetaComponents::deltaTime = frameTime.asSeconds(); 
    MetaComponents::globalTime += MetaComponents::deltaTime;
}

/* handleEventInput takes in keyboard and mouse input. It modifies flagEvents and calls setMouseClickedPos in scene to 
pass in the position in screen where mouse was clicked */
void GameManager::handleEventInput() {
    sf::Event event;
    while (mainWindow.getWindow().pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            log_info("Window close event detected.");
            FlagSystem::flagEvents.gameEnd = true;
            mainWindow.getWindow().close();
            return; 
        }
        if (event.type == sf::Event::Resized){ 
            float aspectRatio = static_cast<float>(event.size.width) / event.size.height;
            sf::FloatRect visibleArea(0.0f, 0.0f, Constants::VIEW_SIZE_X, Constants::VIEW_SIZE_X / aspectRatio);
            MetaComponents::view = sf::View(visibleArea); 
        }
        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
                case sf::Keyboard::A:
                    FlagSystem::flagEvents.aPressed = true;
                    break;
                case sf::Keyboard::S:
                    FlagSystem::flagEvents.sPressed = true;
                    break;
                case sf::Keyboard::W:
                    FlagSystem::flagEvents.wPressed = true;
                    break;
                case sf::Keyboard::D:
                    FlagSystem::flagEvents.dPressed = true;
                    break;
                case sf::Keyboard::B:
                    FlagSystem::flagEvents.bPressed = true;
                    break;
                case sf::Keyboard::Space:
                    FlagSystem::flagEvents.spacePressed = true;
                    break;
                default:
                    break;
            }
        }
        if (event.type == sf::Event::KeyReleased) {
            FlagSystem::flagEvents.flagKeyReleased(); 
        }
        if (event.type == sf::Event::MouseButtonPressed) {
            FlagSystem::flagEvents.mouseClicked = true;
            sf::Vector2f worldPos = mainWindow.getWindow().mapPixelToCoords(sf::Mouse::getPosition(mainWindow.getWindow()), MetaComponents::view);
            MetaComponents::mouseClickedPosition_i = static_cast<sf::Vector2i>(worldPos);
            MetaComponents::mouseClickedPosition_f = worldPos; 
        }
        if (event.type == sf::Event::MouseButtonReleased) {
            FlagSystem::flagEvents.mouseClicked = false;
        }

    }
}

