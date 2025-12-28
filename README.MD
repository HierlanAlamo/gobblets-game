# Gobblets Game - SDL2 Implementation

## Overview
A digital implementation of the strategic board game Gobblets, developed using SDL2 for graphics rendering. This project was created as a learning experience to deepen understanding of C++ and game development fundamentals.

## Key Gameplay Changes

### Movement System Update
Based on professor's suggestion, the piece movement system was modified:
- **Before**: Pieces were dragged visually across the screen
- **After**: Pieces remain stationary during drag and only "teleport" to their destination when released
- **Reason**: This prevents opponents from seeing the piece underneath when moving the piece on top
- **Implementation**: Visual feedback (piece color change to white) indicates when a piece is selected for movement

## Technical Implementation

### Development Philosophy
I used SDL2 for this project by personal choice because I wanted to try making the game in a more fundamental way to the extent possible, as I believe this improves my understanding of how the language works. I also drew all the sprites myself.

### Audio Limitation
**Note**: Background music works when running the executable natively but may not play within Docker containers due to missing audio drivers.

## Project Experience
I appreciate the project - it was really fun, and despite any bugs, here is the final product! :D