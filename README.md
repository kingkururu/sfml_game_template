# Custom SFML Game Framework 

A lightweight custom game framework (~2000 LOC) built with C++ and SFML, designed for flexible 2D game development. This framework allows for code-driven development without relying on external engines or graphical editors.

## Features

- **Game Manager**: Manages game loop, scenes, and input handling
- **Scenes**: Handles specialized events for different game states
- **Sprites**: Extends sf::Sprite with static/non-static and animated/non-animated support
- **Fonts**: Customizes sf::Text for typography management
- **Sounds**: Extends sf::Sound and sf::Music for audio control
- **Tiles & Tilemap**: Loads tilemaps from text files with basic random map generator
- **Logging**: Toggle logging per state with macros, includes timer and multi-threading support
- **Physics**: AABB and Circle collision, pixel-perfect collision detection, raycast precollision detection, and quadtree implementation
- **Constants & Globals**: Configured via YAML files for easy management

## Technical Architecture

### Code Structure
- **Tiles and Tilemap Class**: Refactored for better tile management
- **Window Management**: Moved out of Game class, added logging system
- **Constants and Flags**: Merged into single Globals module with YAML configuration
- **Utils Integration**: Connected to logging system with togglable macros
- **Scenes System**: Access to major components excluding Game and main files

## Building & Running

### Prerequisites
- **Compiler**: Requires clang++ (or g++) with support for C++17
- **SFML**: Simple and Fast Multimedia Library for graphics, windowing, and audio
- **Homebrew (macOS)**: Setup includes auto-installation if missing

### Installation

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/kingkururu/sfml_game_template
   cd sfml_game_template (loaction will be different)
   ```

2. **Build the and Run the Test**:
   ```bash
   make test
   ```

3. **Clean the Build**:
   ```bash
   make clean
   ```
   
### Alternative Setup (macOS with Homebrew)

1. **Install SFML**:
   ```bash
   # Install Homebrew if not already installed
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   
   # Install SFML
   brew install sfml@2
   brew install .... (other missing libraries)
   ```

2. **Configure PATH** (add to ~/.zshrc or ~/.bash_profile):
   ```bash
   export PATH="/opt/homebrew/bin:$PATH"
   export PATH="/opt/homebrew/opt/sfml@2/bin:$PATH"
   export PATH="path to other libraries if any of them is missing)
   ```

## Project Structure

```
/sfml_game_template
│
├── src/                      # Source files
│   ├── main.cpp              # Main entry point of the game
│   └── game/                 # Core engine functionalities
│       ├── globals/          # Constants and flags
│       ├── core/             # Game loop and state management
│       ├── physics/          # Physics and collision detection
│       ├── camera/           # Window and view management
│       ├── utils/            # Utility functions
│       └── scenes/           # Scene management
│
├── assets/                   # Game assets
│   ├── fonts/                # Text files and sources
│   ├── sound/                # Sound effects
│   ├── tiles/                # Tiles and tilemaps
│   └── sprites/              # Sprite images
│
├── libs/                      # External libraries
│   └── logging/               # Logging system
│
├── Makefile                   # Build instructions
└── README.md                  # Project documentation
```

## How to Use the Template

### 1. Game Manager and Scene Management
- **Add Scenes**: Use GameManager to manage all your game scenes (IntroScene, GameplayScene, etc.)
- **Scene Inheritance**: Each scene should inherit from the base Scene class with virtual methods like update, draw, and handleEvents

### 2. Constants Management
- **Add New Constants**: Define constants in the Constants namespace (SCREEN_WIDTH, VIEW_SIZE_X, SCREEN_SCALE, etc.)
- **Accessing Constants**: Include Constants.h and refer to them as Constants::SCREEN_WIDTH

### 3. MetaComponents Setup
- **Global Variables**: Variables like mouseClickedPosition, globalTime, deltaTime, and clock are globally accessible
- **Using MetaComponents**: Access variables directly through MetaComponents for input, time, and window management

### 4. Sprites and 2D Components
- **Adding Sprites**: Use the Sprite class to manage game visuals
- **Collision Handling**: Use Physics::checkCollision(sprite1, sprite2, helperFunctionType) for collision detection

### 5. Physics and Movement
- **Physics Namespace**: Includes functions for movement, gravity, and basic physics interactions
- **Customizing Movement**: Adjust movement speeds, jump heights, or gravity by editing Physics functions

### 6. Logging (Using spdlog)
- **Debug Logging**: spdlog functions accessible throughout the project
- **Performance Monitoring**: Logging initialized in GameManager to monitor state changes and performance metrics

## Dependencies

- **SFML**: For graphics, windowing, and audio handling
- **spdlog**: Logging support for debug and error tracking
- **fmt**: Text formatting library for logging

## Key Learning Outcomes

- Custom game framework development
- Advanced C++ programming with modern features
- SFML library integration and optimization
- Multi-threaded application design
- Spatial partitioning algorithms (Quadtree)
- YAML configuration management
- Performance optimization techniques
- Modular architecture design

## Usage Notes

This template does not include a UI layer for customization. Changes must be made directly in the code to implement specific game functionality. The framework is designed entirely for code-driven development, allowing maximum flexibility for 2D game creation.
