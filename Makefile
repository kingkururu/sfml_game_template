# Default target (build the project)
all: install_deps $(TARGET)

# Compiler and linker flags (Homebrew paths are now customizable)
CXX = clang++
CXXFLAGS = -std=c++17 -Wall \
            -I./src -I./src/game -I./src/game/globals -I./src/game/core -I./src/game/physics -I./src/game/camera -I./src/game/utils -I./src/game/scenes \
            -I./assets/sprites -I./assets/fonts -I./assets/sound -I./assets/tiles \
            -I./libs/logging

# Homebrew paths, allow override via environment variables
HOMEBREW_PREFIX ?= /opt/homebrew
SPDLOG_INCLUDE ?= $(HOMEBREW_PREFIX)/opt/spdlog/include
FMT_INCLUDE ?= $(HOMEBREW_PREFIX)/opt/fmt/include
SFML_INCLUDE ?= $(HOMEBREW_PREFIX)/opt/sfml/include
SPDLOG_LIB ?= $(HOMEBREW_PREFIX)/opt/spdlog/lib
FMT_LIB ?= $(HOMEBREW_PREFIX)/opt/fmt/lib
SFML_LIB ?= $(HOMEBREW_PREFIX)/opt/sfml/lib

# Adding Homebrew include paths dynamically
CXXFLAGS += -I$(SPDLOG_INCLUDE) -I$(FMT_INCLUDE) -I$(SFML_INCLUDE)

# Library paths and linking
LDFLAGS = -L$(SPDLOG_LIB) -L$(FMT_LIB) -L$(SFML_LIB) -L$(HOMEBREW_PREFIX)/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lspdlog -lfmt

# Source files
SRC = src/main.cpp \
      src/game/globals/globals.cpp \
      src/game/core/game.cpp \
      src/game/physics/physics.cpp \
      src/game/camera/window.cpp \
      src/game/utils/utils.cpp \
      src/game/scenes/scenes.cpp \
      assets/sprites/sprites.cpp \
      assets/fonts/fonts.cpp \
      assets/sound/sound.cpp \
      assets/tiles/tiles.cpp \
      libs/logging/log.cpp 

# Object files
OBJ = $(SRC:.cpp=.o)

# Target executable
TARGET = run

# Homebrew check
HOMEBREW_CHECK := $(shell command -v brew 2>/dev/null)

.PHONY: check_homebrew install_deps build clean

# Check if Homebrew is installed
check_homebrew:
ifdef HOMEBREW_CHECK
	@echo "Homebrew is already installed."
else
	@echo "Homebrew not found. Installing Homebrew..."
	/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
endif

# Check and install required dependencies via Homebrew
install_deps: check_homebrew
	@brew list spdlog >/dev/null 2>&1 || (echo "Installing spdlog..."; brew install spdlog)
	@brew list fmt >/dev/null 2>&1 || (echo "Installing fmt..."; brew install fmt)
	@brew list sfml >/dev/null 2>&1 || (echo "Installing sfml..."; brew install sfml)

# Build target
all: install_deps $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

# Rule to build object files
src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

assets/%.o: assets/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

libs/logging/%.o: libs/logging/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(TARGET) $(OBJ)
