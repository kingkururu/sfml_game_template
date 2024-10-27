
## SFML Game Template

This repository provides a foundational template for building 2D games using C++ and SFML. It's designed for developers looking to implement essential game features like game loops, asset management, physics, and scene management. Built with Visual Studio Code, this setup should also work in other environments that support Makefile builds.



## Features

-   2D Game Template: Provides a structured layout to help you kickstart 2D games in C++.
-   Built-in Dependencies: Uses spdlog for logging and fmt for formatted text output.
-   Modular Design: Organizes code by game components, assets, and utilities.
-   Scene Management: Supports multiple scenes (e.g., main menu, gameplay).
-   Asset Management: Structure for managing sprites, sounds, fonts, and tiles.
-   Physics & Collision Detection: Includes a basic physics module to detect collisions.
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

3. Run the game 
```bash
  ./run
```
4. Clean the Build
Remove compiled files with:
```bash
  make clean
```

## File Structure
The codebase is organized for ease of modification and scalability:

```bash
  /sfml_game_template
│
├── .vscode                    # VSCode settings and configurations
│
├── src                        # Source files for game engine
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
│   └── sprites                # Sprite images
│
├── libs                       # External libraries
│   └── logging                # Logging (spdlog-based)
│
├── Makefile                   # Build instructions
└── README.md                  # Project documentation

```
This template includes assets and utilities to simplify your development process, with logging and debug information to help you track runtime behavior.



## Customization & Usage

-   Adjust Constants: Modify constants in globals.cpp for screen dimensions, physics parameters, or other game settings.
-   Add Scenes: Create new files under /scenes to implement unique game states (e.g., MenuScene, GameScene).
-   Asset Management: Add or modify sprites, sounds, and fonts in the respective folders under /assets.
## Potential Extensions
-   Additional Scenes: Extend functionality by adding new game scenes for different states or menus.
-   Networking: Integrate SFML networking components for multiplayer features.
-   Custom Physics: Extend physics capabilities for advanced collision or physics-based interactions.
## Dependencies

-   SFML: For graphics, windowing, and audio handling.
-   spdlog: Logging support for debug and error tracking.
-   fmt: Text formatting library for easier output management.

## License

This project is licensed under the MIT License.

##  

Note: Adjust file paths, include dependencies, and modify other sections based on your environment, as some systems may require manual setup for Homebrew or custom paths.
## Game Architecture Overview
<img width="976" alt="Screenshot 2024-10-26 at 8 25 33 PM" src="https://github.com/user-attachments/assets/5b57e765-6977-43a8-a479-05965d5e8a9c">
 
<img width="1232" alt="Screenshot 2024-10-27 at 2 39 21 PM" src="https://github.com/user-attachments/assets/17f4318e-7e0b-402e-916b-69122d6d4300">
 
