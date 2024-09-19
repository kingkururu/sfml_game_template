# Compiler and linker flags
CXX = clang++
CXXFLAGS = -std=c++17 -Wall \
            -I./src -I./src/game -I./src/game/constants -I./src/game/flags -I./src/game/core -I./src/game/physics -I./src/game/rendering -I./src/game/utils -I./src/game/scenes \
            -I./assets/sprites -I./assets/fonts -I./assets/sound \
            -I./libs/logging \
            -I/opt/homebrew/opt/spdlog/include -I/opt/homebrew/opt/fmt/include
LDFLAGS = -L/opt/homebrew/lib -L/opt/homebrew/opt/spdlog/lib -L/opt/homebrew/opt/fmt/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lspdlog -lfmt

# Source files
SRC = src/main.cpp \
      src/game/constants/constants.cpp \
      src/game/flags/flags.cpp \
      src/game/core/game.cpp \
      src/game/physics/physics.cpp \
      src/game/rendering/windows.cpp \
      src/game/utils/utils.cpp \
      src/game/scenes/scenes.cpp \
      assets/sprites/sprites.cpp \
      assets/fonts/fonts.cpp \
      assets/sound/sound.cpp \
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
