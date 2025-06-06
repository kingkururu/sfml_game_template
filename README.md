
## SFML Game Template

This repository provides a foundational template for building 2D games using C++ and SFML. It's designed for basic game features like game loops, asset management, physics, and scene management. Built with Visual Studio Code, this setup should also work in other environments that support Makefile builds.



## Features

Game Manager: Manages game loop, scenes, and input.


Scenes: Handles specialized events for different states.


Sprites:
Extends sf::Sprite
- Static vs. Non-Static
- Animated vs. Non-Animated


Fonts: Customizes sf::Text for typography.


Sounds: Extends sf::Sound and sf::Music for audio control.


Tiles & Tilemap:
- Loads tilemaps from text files
- Includes very basic random map generator


Logging: Toggle logging per state with macros.
- includes a timer (thanks to cherno: https://gist.github.com/TheCherno/b2c71c9291a4a1a29c889e76173c8d14) 
- uses multi-threading 


Physics:
- AABB and Circle collision
- Pixel-perfect collision detection
- Raycast precollision detection
- Quadtree implementation 



Constants & Globals: Configured via YAML files.


Note: This template does not include a UI layer for customization. Changes must be made directly in the code to implement specific game functionality.







## Prerequisites

-   Compiler: Requires clang++ (or g++) with support for C++17
-   SFML: Simple and Fast Multimedia Library for graphics, windowing, and audio.
-   Homebrew (macOS): Setup includes steps to auto-install Homebrew and dependencies if missing.

## Installation & Setup

1. Clone the Repository

```bash
  git clone <https://github.com/kingkururu/sfml_game_template>
  cd sfml_game_template
```

2. Build the Project
Open Visual Studio Code, navigate to the project directory, and run:
```bash
  make
```
This will:

-   Install Homebrew if missing.
-   Install sfml, spdlog, and fmt via Homebrew.
-   Compile and build the project executable.

3.1 Run the game ( run the tester )
```bash
  make test
```
3.2 Run the game ( run the actual )
```bash
  make run
```
4. Clean the Build
Remove compiled files with:
```bash
  make clean
```

## File Structure

```bash
  /sfml_game_template
│
├── .vscode                    # VSCode settings and configurations
│
├── src                        # Source files
│   ├── main.cpp               # Main entry point of the game
│   └── game                   # Core engine functionalities
│       ├── globals            # Constants and flags
│       ├── core               # Game loop and state management
│       ├── physics            # Physics and collision detection
│       ├── camera             # SFML window and view management
│       ├── utils              # Utility functions
│       └── scenes             # Scene management
│
├── assets                     # Game assets
│   ├── fonts                  # Font files and sources
│   ├── sound                  # Sound effects
│   ├── tiles                  # Tiles and tilemaps 
│   └── sprites                # Sprite images
│
├── libs                       # External libraries
│   └── logging                # Logging
|
├── test                       # Test game (example project)
├── Makefile                   # Build instructions
└── README.md                  # Project documentation

```

## Dependencies
-   SFML: For graphics, windowing, and audio handling.
-   spdlog: Logging support for debug and error tracking.
-   fmt: Text formatting library for logging
-   catch2: For testing purpooses

## License

This project is licensed under the MIT License.

##  

Note: Adjust file paths, include dependencies, and modify other sections based on your environment, as some systems may require manual setup for Homebrew or custom paths.
## Game Architecture Overview
<img width="871" alt="Screenshot 2024-11-05 at 11 40 42 AM" src="https://github.com/user-attachments/assets/211ec582-6439-444b-9450-c1ea3210776f">
 
<img width="1248" alt="Screenshot 2024-10-27 at 8 06 23 PM" src="https://github.com/user-attachments/assets/f664171a-9f20-40c7-9a09-7108944b6f8d">
 
## How to Use the 2D Game Template
This guide explains how to set up and start building your 2D game using the provided game template. Follow these steps to add scenes, manage constants, and integrate other game elements.

1. Game Manager and Scene Management
- Add Scenes: Use GameManager to manage all your game scenes (like IntroScene, GameplayScene, etc.). Each scene should inherit from the base Scene class, which includes virtual methods like update, draw, and handleEvents.

2. Constants Management
- Add New Constants: Define constants (like screen dimensions, game view settings, and scaling values) in the Constants namespace. This includes things like SCREEN_WIDTH, VIEW_SIZE_X, SCREEN_SCALE, etc.
- Accessing Constants: To access these constants in other files, simply include the Constants.h file and refer to them as Constants::SCREEN_WIDTH, for example.

3. MetaComponents Setup
- Global Variables: Variables in the MetaComponents namespace—like mouseClickedPosition, globalTime, deltaTime, and clock—are meant to be global and accessible from any file without duplication.
- Using MetaComponents: Access variables directly through MetaComponents to manage input, time, and the main window. For example, MetaComponents::globalTime provides the time since the game started, useful for animations or timed events.

4. Sprites and 2D Components
- Adding Sprites: Use the Sprite manage your game visuals
- Visibility and Out-of-Bounds: The template includes functions to set sprites visible if within bounds. Leverage these checks to manage performance, hiding sprites when they’re completely offscreen; note that this only works inside a set view, where the entire window is the view.
- Collision Handling: Collision detection functions in the Physics namespace help handle sprite collisions and movement. Use Physics::checkCollision(sprite1, sprite2, helperFunctionType) for basic collision checks.

5. Physics and Movement
- Physics Namespace: The Physics namespace includes functions for handling movement, gravity, and basic physics interactions. These functions work well for platformers or games needing simple physics.
- Customizing Movement: Adjust movement speeds, jump heights, or gravity as needed by editing functions in Physics. You can also log debug information if using spdlog by adding logging calls in this namespace.

6. Logging (Using spdlog)
- Debug Logging: Logging can help debug scenes and check variable states. spdlog functions are accessible throughout the project; logging is initialized in GameManager or any key game class to monitor state changes or performance metrics.

7. Build and Run
- Compilation: Use make with the provided Makefile to compile the project

Feel free to modify these sections according to your game’s needs, adding more scenes, adjusting constants, or extending components as needed.
