# Compiler and linker flags (homebrew paths are now customizable)
CXX = clang++
CXXFLAGS = -std=c++17 -Wall \
            -I./src -I./src/game -I./src/game/constants -I./src/game/flags -I./src/game/core -I./src/game/physics -I./src/game/rendering -I./src/game/utils -I./src/game/scenes \
            -I./assets/sprites -I./assets/fonts -I./assets/sound -I./assets/tiles \
            -I./libs/logging

# Homebrew paths, allow override via environment variables
HOMEBREW_PREFIX ?= /opt/homebrew
SPDLOG_INCLUDE ?= $(HOMEBREW_PREFIX)/opt/spdlog/include
FMT_INCLUDE ?= $(HOMEBREW_PREFIX)/opt/fmt/include
SPDLOG_LIB ?= $(HOMEBREW_PREFIX)/opt/spdlog/lib
FMT_LIB ?= $(HOMEBREW_PREFIX)/opt/fmt/lib

# Adding homebrew include paths dynamically
CXXFLAGS += -I$(SPDLOG_INCLUDE) -I$(FMT_INCLUDE)

# Library paths and linking
LDFLAGS = -L$(SPDLOG_LIB) -L$(FMT_LIB) -L$(HOMEBREW_PREFIX)/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lspdlog -lfmt

# Source files
SRC = src/main.cpp \
      src/game/constants/constants.cpp \
      src/game/flags/flags.cpp \
      src/game/core/game.cpp \
      src/game/physics/physics.cpp \
      src/game/rendering/window.cpp \
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

# Build target
all: $(TARGET)

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
