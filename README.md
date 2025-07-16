# Custom SFML Game Framework 🎮

A lightweight custom game framework (~2000 LOC) built with C++ and SFML, designed for flexible 2D game development. This framework allows for code-driven development without relying on external engines or graphical editors.

## 🎯 Features

- **Game Manager**: Manages game loop, scenes, and input handling
- **Scenes**: Handles specialized events for different game states
- **Sprites**: Extends sf::Sprite with static/non-static and animated/non-animated support
- **Fonts**: Customizes sf::Text for typography management
- **Sounds**: Extends sf::Sound and sf::Music for audio control
- **Tiles & Tilemap**: Loads tilemaps from text files with basic random map generator
- **Logging**: Toggle logging per state with macros, includes timer and multi-threading support
- **Physics**: AABB and Circle collision, pixel-perfect collision detection, raycast precollision detection, and quadtree implementation
- **Constants & Globals**: Configured via YAML files for easy management

## 🛠️ Technical Architecture

### Systems hierarchy
<img width="871" alt="Screenshot 2024-11-05 at 11 40 42 AM" src="https://github.com/user-attachments/assets/211ec582-6439-444b-9450-c1ea3210776f">
 
<img width="1248" alt="Screenshot 2024-10-27 at 8 06 23 PM" src="https://github.com/user-attachments/assets/f664171a-9f20-40c7-9a09-7108944b6f8d">

### Code Structure
- **Tiles and Tilemap Class**: Refactored for better tile management
- **Window Management**: Moved out of Game class, added logging system
- **Constants and Flags**: Merged into single Globals module with YAML configuration
- **Utils Integration**: Connected to logging system with togglable macros
- **Scenes System**: Access to major components excluding Game and main files

### Advanced Features
- **Custom Window Class**: Support for multiple views with YAML-based configuration
- **Logging System**: Color-coded console output (red, green, yellow) with file logging
- **Quadtree Algorithm**: Implemented for recursive search in spatial partitioning
- **Performance Optimization**: Separate thread execution for reduced overhead
- **Input Handling**: Extended helper methods for various input types including mouse positions and window bounds

## 🚀 Building & Running

### Prerequisites
- **Compiler**: Requires clang++ (or g++) with support for C++17
- **SFML**: Simple and Fast Multimedia Library for graphics, windowing, and audio
- **Homebrew (macOS)**: Setup includes auto-installation if missing

### Installation

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/kingkururu/sfml_game_template
   cd sfml_game_template
   ```

2. **Build the Project**:
   ```bash
   make
   ```
   This will:
   - Install Homebrew if missing
   - Install SFML, spdlog, and fmt via Homebrew
   - Compile and build the project executable

3. **Run the Game**:
   ```bash
   # Run the tester
   make test
   
   # Run the actual game
   make run
   ```

4. **Clean the Build**:
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

## 📁 Project Structure

```
/sfml_game_template
│
├── .vscode/                   # VSCode settings and configurations
│
├── src/                       # Source files
│   ├── main.cpp              # Main entry point of the game
│   └── game/                 # Core engine functionalities
│       ├── globals/          # Constants and flags
│       ├── core/             # Game loop and state management
│       ├── physics/          # Physics and collision detection
│       ├── camera/           # SFML window and view management
│       ├── utils/            # Utility functions
│       └── scenes/           # Scene management
│
├── assets/                    # Game assets
│   ├── fonts/                # Font files and sources
│   ├── sound/                # Sound effects
│   ├── tiles/                # Tiles and tilemaps
│   └── sprites/              # Sprite images
│
├── libs/                      # External libraries
│   └── logging/              # Logging system
│
├── test/                      # Test game (example project)
├── Makefile                   # Build instructions
└── README.md                  # Project documentation
```

## 🎮 How to Use the Template

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
- **Visibility Checks**: Template includes functions to set sprites visible if within bounds
- **Collision Handling**: Use Physics::checkCollision(sprite1, sprite2, helperFunctionType) for collision detection

### 5. Physics and Movement
- **Physics Namespace**: Includes functions for movement, gravity, and basic physics interactions
- **Customizing Movement**: Adjust movement speeds, jump heights, or gravity by editing Physics functions

### 6. Logging (Using spdlog)
- **Debug Logging**: spdlog functions accessible throughout the project
- **Performance Monitoring**: Logging initialized in GameManager to monitor state changes and performance metrics

## 🔧 Dependencies

- **SFML**: For graphics, windowing, and audio handling
- **spdlog**: Logging support for debug and error tracking
- **fmt**: Text formatting library for logging
- **catch2**: For testing purposes

## 🎯 Key Learning Outcomes

- Custom game framework development
- Advanced C++ programming with modern features
- SFML library integration and optimization
- Multi-threaded application design
- Spatial partitioning algorithms (Quadtree)
- YAML configuration management
- Performance optimization techniques
- Modular architecture design

## 🏆 Advanced Features

- **New Window Class**: Multiple view support with YAML configuration
- **New Globals Class**: Merged constants and flags with YAML reading
- **New Tiles Class**: Enhanced tile and tilemap management
- **New Logging Class**: Console and file logging with FMT, SPDLOG, and multi-threading
- **Physics Updates**: Quadtree implementation and enhanced helper methods

## 📋 Usage Notes

This template does not include a UI layer for customization. Changes must be made directly in the code to implement specific game functionality. The framework is designed entirely for code-driven development, allowing maximum flexibility for 2D game creation.

## 📄 License

This project is licensed under the MIT License.

---

*Note: Adjust file paths, include dependencies, and modify sections based on your environment, as some systems may require manual setup for Homebrew or custom paths.*
