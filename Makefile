# Compiler and linker settings
CXX := clang++

# Compiler flags
CXXFLAGS := -std=c++17 -Wall \
            -I./src -I./src/game -I./src/game/globals -I./src/game/core -I./src/game/physics -I./src/game/camera -I./src/game/utils -I./src/game/scenes \
            -I./assets/sprites -I./assets/fonts -I./assets/sound -I./assets/tiles \
            -I./libs/logging

# Homebrew paths, allow override via environment variables
HOMEBREW_PREFIX ?= /opt/homebrew
SPDLOG_INCLUDE ?= $(HOMEBREW_PREFIX)/opt/spdlog/include
FMT_INCLUDE ?= $(HOMEBREW_PREFIX)/opt/fmt/include
SFML_INCLUDE ?= $(HOMEBREW_PREFIX)/opt/sfml/include
CATCH2_INCLUDE ?= $(HOMEBREW_PREFIX)/opt/catch2/include
SPDLOG_LIB ?= $(HOMEBREW_PREFIX)/opt/spdlog/lib
FMT_LIB ?= $(HOMEBREW_PREFIX)/opt/fmt/lib
SFML_LIB ?= $(HOMEBREW_PREFIX)/opt/sfml/lib
YAML_INCLUDE ?= $(HOMEBREW_PREFIX)/Cellar/yaml-cpp/0.8.0/include

# Include paths for Homebrew libraries
BREW_INCLUDE_FLAGS := -I$(SPDLOG_INCLUDE) -I$(FMT_INCLUDE) -I$(SFML_INCLUDE) -I$(CATCH2_INCLUDE) -I$(YAML_INCLUDE)
CXXFLAGS += $(BREW_INCLUDE_FLAGS)
# CXXFLAGS += -I$(HOMEBREW_PREFIX)/opt/catch2/include

TEST_CXXFLAGS := -std=c++17 -Wall \
                 -I./test/test-src \
                 -I./test/test-src/game/core -I./test/test-src/game/camera \
                 -I./test/test-src/game/globals -I./test/test-src/game/physics \
                 -I./test/test-src/game/scenes -I./test/test-src/game/utils \
                 -I./test/test-assets -I./test/test-assets/fonts \
                 -I./test/test-assets/sound -I./test/test-assets/tiles \
                 -I./test/test-assets/sprites \
                 -I./test/test-logging \
                 -I./test/test-testing \
                 -I$(SPDLOG_INCLUDE) -I$(FMT_INCLUDE) -I$(SFML_INCLUDE) -I$(CATCH2_INCLUDE) -I$(YAML_INCLUDE) \
                 -DTESTING

# Library paths and linking
# LDFLAGS = -L$(SPDLOG_LIB) -L$(FMT_LIB) -L$(SFML_LIB) -L$(HOMEBREW_PREFIX)/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lspdlog -lfmt -lyaml-cpp
LDFLAGS = -L$(SPDLOG_LIB) -L$(FMT_LIB) -L$(SFML_LIB) -L$(HOMEBREW_PREFIX)/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lspdlog -lfmt -lyaml-cpp -lCatch2

# Build directories
BUILD_DIR := build
TEST_BUILD_DIR := test_build

# Main application source and object files
SRC := src/main.cpp \
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

OBJ := $(SRC:%.cpp=$(BUILD_DIR)/%.o)

# Test source and object files
TEST_SRC := test/test-src/testMain.cpp \
            test/test-src/game/globals/globals.cpp \
            test/test-src/game/core/game.cpp \
            test/test-src/game/physics/physics.cpp \
            test/test-src/game/camera/window.cpp \
            test/test-src/game/utils/utils.cpp \
            test/test-src/game/scenes/scenes.cpp \
            test/test-assets/sprites/sprites.cpp \
            test/test-assets/fonts/fonts.cpp \
            test/test-assets/sound/sound.cpp \
            test/test-assets/tiles/tiles.cpp \
            test/test-logging/log.cpp \
            test/test-testing/testing.cpp

TEST_OBJ := $(TEST_SRC:%.cpp=$(TEST_BUILD_DIR)/%.o)

# New target to copy YAML config file
COPY_CONFIG:
	@mkdir -p $(TEST_BUILD_DIR)/config
	cp test/test-src/game/globals/config.yaml $(TEST_BUILD_DIR)/config/

# Target executables
TARGET := sfml_game
TEST_TARGET := sfml_game_test

.PHONY: all install_deps build clean test run

# Default target (build the main application)
all: $(TARGET)

# Check and install required dependencies via Homebrew
install_deps:
	@brew list spdlog >/dev/null 2>&1 || (echo "Installing spdlog..."; brew install spdlog)
	@brew list fmt >/dev/null 2>&1 || (echo "Installing fmt..."; brew install fmt)
	@brew list sfml >/dev/null 2>&1 || (echo "Installing sfml..."; brew install sfml)
	@brew list yaml-cpp >/dev/null 2>&1 || (echo "Installing yaml-cpp..."; brew install yaml-cpp) 
	@brew list catch2 >/dev/null 2>&1 || (echo "Installing catch2..."; brew install catch2)

# Main application build target
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ) $(LDFLAGS)

# Test build target
$(TEST_TARGET): $(TEST_OBJ)
	$(CXX) $(TEST_CXXFLAGS) -o $@ $(TEST_OBJ) $(LDFLAGS)

# Rule to build main object files
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to build test object files
$(TEST_BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(TEST_CXXFLAGS) -c $< -o $@

# Clean up all build artifacts
clean:
	rm -rf $(BUILD_DIR) $(TEST_BUILD_DIR) $(TARGET) $(TEST_TARGET)

# Run the application
run: $(TARGET) COPY_CONFIG
	./$(TARGET)

# Run tests
test: $(TEST_TARGET) COPY_CONFIG
	./$(TEST_TARGET)
