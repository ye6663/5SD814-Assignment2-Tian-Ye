# 5SD814 Assignment 2 - Arcade Game

**Course:** 5SD814 - Game Programming 1
**Student:** [Tian Ye] (tye050321@163.com)

## Description

This project transforms the asteroid field renderer from Assignment 1 into a complete, playable arcade space shooter game. The implementation includes:

### Core Features
- **Complete Gameplay Loop**: Main menu, playing, and game over states
- **Weapon System**: Player can fire bullets with finite lifetime
- **Asteroid Management**: Three-size asteroid system with fragmentation (large -> medium -> small)
- **Collision Detection**: Two-phase collision system (broad phase + narrow phase)
- **Event-Driven Architecture**: Decoupled systems reacting to game events

### Advanced Features
- **Resource Management**: TextureManager for loading all external assets
- **Hyperspace Jump**: Emergency teleport to random location (SHIFT key)
- **Laser Weapon**: Secondary weapon with line segment collision detection (ALT key)
- **Player Defense**: Regenerating shield system with visual feedback

### Technical Implementation
- Spatial grid for efficient collision detection
- Event system for decoupled game logic
- Render command queue with layer-based sorting
- Audio system with sound effects
- Score tracking and game state management

## How to Use

### Controls
- **W / UP ARROW**: Apply thrust
- **A / LEFT ARROW**: Rotate left
- **D / RIGHT ARROW**: Rotate right  
- **SPACE**: Fire bullets (primary weapon)
- **ALT**: Fire laser (secondary weapon)
- **SHIFT**: Hyperspace jump (emergency teleport)
- **F1**: Toggle debug information

### Gameplay
1. Start from the main menu and click "START GAME"
2. Destroy asteroids to earn points:
   - Large asteroids: 10 points
   - Medium asteroids: 5 points  
   - Small asteroids: 2 points
3. Use different weapons strategically:
   - **Bullets**: Fast, unlimited but single target
   - **Laser**: Penetrates multiple asteroids but has cooldown
4. Use hyperspace jump (SHIFT) to escape dangerous situations
5. Avoid collisions with asteroids - they damage your shields and eventually end the game
6. Your shield regenerates after not taking damage for 3 seconds
7. When game over occurs, you can restart or return to main menu

### Game Rules
- Player starts with 100 shield points
- Shield regenerates at 10 points per second after 3 seconds of no damage
- Bullets have limited range and lifetime
- Laser weapon has finite range and duration, requiring line segment collision detection
- Hyperspace jump teleports player to random location on screen
- Asteroids break into smaller pieces when destroyed
- Collision with asteroids when shield is depleted ends the game

## System Architecture

### Event System
The game uses a publish-subscribe event system where multiple systems react to game events:
- **AudioSystem**: Plays sound effects for collisions, shooting, etc.
- **ScoreSystem**: Tracks and updates player score
- **GameplaySystem**: Handles game rules and asteroid fragmentation

### Collision Pipeline
1. **Broad Phase**: Spatial grid identifies potential collisions
2. **Narrow Phase**: Precise collision detection between entities
3. **Event Publishing**: Collision events are published to interested systems

### Resource Management
All textures are loaded through the TextureManager using string identifiers from a manifest file.

## Building and Running

### Prerequisites
- Visual Studio Community 2022 (v17.14 or newer)
- Raylib library
- C++17 compatible compiler

### Build Instructions
1. Open the solution file in Visual Studio
2. Ensure all dependencies are properly linked
3. Build in Release or Debug mode
4. Run the executable

**Quick Start**: You can directly run the pre-built executable at `build/Assignment2.debug.exe` in the project root directory.

## Collaborators
- None (individual assignment)

