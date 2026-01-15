# AGENTS.md

This file contains guidelines and commands for agentic coding agents working on the Space-Wars project.

## Project Overview

Space-Wars is a C++ SDL2-based game project that follows CMake build system conventions. The project uses C++17 standard and cross-platform SDL2 for graphics and input handling.

## Build Commands

### Basic Build
```bash
# Configure the build
cmake -B build

# Build the project
cmake --build build

# Alternative: Build with specific configuration
cmake --build build --config Release
cmake --build build --config Debug
```

### Clean Build
```bash
# Remove build directory completely
rm -rf build dist

# Rebuild from scratch
cmake -B build && cmake --build build
```

### Run Executable
```bash
# Run the built application
./dist/SpaceWars-0.1.0-Darwin  # on macOS
./dist/SpaceWars-0.1.0-Windows.exe  # on Windows
./dist/SpaceWars-0.1.0-Linux  # on Linux
```

## Testing

Currently this project doesn't have a formal test suite. When adding tests:
- Use GoogleTest framework (already integrated via MCP dependencies if needed)
- Place test files in `test/` directory
- Add test targets to CMakeLists.txt

### Future Test Commands
```bash
# Run all tests
cmake --build build --target test

# Run specific test
ctest --test-dir build -R "test_name"
```

## Code Style Guidelines

### C++ Standards and Conventions

#### Language Standard
- Use C++17 standard (enforced by CMake)
- Follow modern C++ best practices
- Use RAII principles consistently
- Prefer smart pointers over raw pointers

#### Naming Conventions
```cpp
// Classes: PascalCase
class SpaceShip {
public:
    // Methods: camelCase
    void updatePosition();
    
    // Member variables: snake_ with trailing underscore
    float x_position_;
    int health_points_;
    
private:
    // Private members: snake_ with trailing underscore
    bool is_active_;
};

// Functions: camelCase
void initializeGame();
void renderFrame();

// Constants: UPPER_SNAKE_CASE
const int MAX_PLAYERS = 4;
const float SCREEN_WIDTH = 800.0f;

// Variables: snake_case
int current_score;
float player_velocity;
```

#### File Organization
- Headers: `.h` extension
- Source files: `.cpp` extension
- One class per file when possible
- Header guards format: `PROJECT_NAME_FILENAME_H`
```cpp
#ifndef SPACE_WARS_SPACESHIP_H
#define SPACE_WARS_SPACESHIP_H

// Content

#endif // SPACE_WARS_SPACESHIP_H
```

### Imports and Includes

#### Include Order
1. System headers (angle brackets)
2. SDL headers
3. Project headers (quotes)
4. Local headers (relative path)

```cpp
#include <iostream>
#include <memory>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>

#include "space_ship.h"
#include "game_state.h"

#include "utils/helpers.h"
```

#### Forward Declarations
Use forward declarations instead of includes where possible to reduce compilation dependencies:
```cpp
// In header file
class SpaceShip;  // Forward declaration
class GameState;

class GameManager {
    SpaceShip* player_;  // Pointer/reference uses forward declaration
    GameState* state_;
};
```

### Error Handling

#### Exceptions
- Use standard exceptions (`std::runtime_error`, `std::invalid_argument`)
- Create custom exception classes for game-specific errors
- Always catch exceptions by const reference

```cpp
try {
    initializeSDL();
} catch (const std::runtime_error& e) {
    std::cerr << "SDL initialization failed: " << e.what() << std::endl;
    return EXIT_FAILURE;
}
```

#### Resource Management
- Use RAII for resource cleanup
- Prefer smart pointers for memory management
- Handle SDL resources properly

```cpp
class SDLWindow {
private:
    SDL_Window* window_;
    
public:
    SDLWindow(const char* title, int width, int height) 
        : window_(SDL_CreateWindow(title, 0, 0, width, height, 0)) {
        if (!window_) {
            throw std::runtime_error("Failed to create SDL window");
        }
    }
    
    ~SDLWindow() {
        if (window_) {
            SDL_DestroyWindow(window_);
        }
    }
    
    // Delete copy constructor and assignment
    SDLWindow(const SDLWindow&) = delete;
    SDLWindow& operator=(const SDLWindow&) = delete;
    
    // Allow move
    SDLWindow(SDLWindow&& other) noexcept : window_(other.window_) {
        other.window_ = nullptr;
    }
    
    SDL_Window* get() const { return window_; }
};
```

### Formatting Guidelines

#### Indentation and Spacing
- Use 4 spaces for indentation (no tabs)
- Maximum line length: 100 characters
- Space around operators: `a = b + c`
- No space after function name: `func(param)` not `func (param)`

#### Braces
- Use Allman style for functions and classes
- Use K&R style for control structures

```cpp
class MyClass
{
public:
    void myFunction()
    {
        if (condition)
        {
            // K&R style for control structures
            doSomething();
        }
        else
        {
            doSomethingElse();
        }
    }
};
```

### Comments and Documentation

#### Doxygen Style
Use Doxygen comments for public APIs:

```cpp
/**
 * @brief Updates the spaceship position based on current velocity
 * @param delta_time Time elapsed since last frame in seconds
 * @return true if position was updated, false if no change
 */
bool updatePosition(float delta_time);
```

#### Implementation Comments
- Use `//` for single-line comments
- Use `/* */` for multi-line comments
- Comment non-obvious logic
- Avoid commenting obvious code

```cpp
// Calculate collision using AABB algorithm
bool checkCollision(const GameObject& a, const GameObject& b) {
    return a.x < b.x + b.width &&
           a.x + a.width > b.x &&
           a.y < b.y + b.height &&
           a.y + a.height > b.y;
}
```

## CMake Guidelines

### Target Naming
- Executable: `${PROJECT_NAME}-${PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}`
- Libraries: Use descriptive names with namespace prefix
- Tests: `${PROJECT_NAME}_test_${test_name}`

### Dependency Management
- Use `find_package()` for system dependencies (SDL2, etc.)
- Use `FetchContent` for third-party libraries
- Link libraries with `PRIVATE`/`PUBLIC`/`INTERFACE` appropriately

### Build Options
- Use CMake options for optional features: `option(BUILD_FEATURE "Description" ON)`
- Support both Debug and Release configurations
- Set appropriate compiler warnings for each platform

## Memory Management

### Guidelines
- Prefer stack allocation over heap allocation
- Use `std::unique_ptr` for exclusive ownership
- Use `std::shared_ptr` for shared ownership
- Use `std::make_unique` and `std::make_shared` for exception safety

```cpp
// Good: Using smart pointers
auto entity = std::make_unique<SpaceShip>(x, y);
auto renderer = std::make_shared<SDLRenderer>(window);

// Avoid: Raw pointers with manual deletion
SpaceShip* entity = new SpaceShip(x, y);  // Bad
delete entity;  // Error-prone
```

## Performance Considerations

### Frame Rate
- Target 60 FPS (16.67ms per frame)
- Use delta time for frame-independent movement
- Profile performance regularly

### Resource Loading
- Load resources asynchronously when possible
- Cache frequently used resources
- Use efficient data structures (std::vector for contiguous memory)

## Platform Specifics

### Cross-Platform Development
- Use CMake for build system portability
- Avoid platform-specific code when possible
- Use preprocessor macros for platform differences:
```cpp
#ifdef _WIN32
    // Windows-specific code
#elif __APPLE__
    // macOS-specific code
#elif __linux__
    // Linux-specific code
#endif
```

### SDL2 Integration
- Initialize SDL properly with error checking
- Clean up SDL resources on shutdown
- Handle SDL events in main game loop

## Integration with MCP

When adding MCP (Model Context Protocol) functionality:
- Use the simplified MCP server implementation in `mcp/` directory
- Run MCP server in separate thread to avoid blocking game loop
- Handle MCP errors gracefully without crashing the game

## Git Workflow

### Commit Messages
- Use present tense: "Add feature" not "Added feature"
- Keep first line under 50 characters
- Include detailed explanation if needed

### Branching
- Use feature branches for new development
- Keep main branch stable at all times
- Use descriptive branch names: `feature/space-ship-physics`

## Debugging

### Logging
- Use `std::cout` for simple debugging
- Consider adding a proper logging framework for larger projects
- Log important events and errors

### Debug Builds
- Enable debug symbols in Debug configuration
- Use compiler-specific debug flags
- Test with sanitizers when available