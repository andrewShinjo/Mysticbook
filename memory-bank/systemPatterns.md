# System Patterns: Mysticbook

## Architecture
- Simple SDL2 application with main event loop
- Single-window design with centralized text rendering

## Key Patterns
1. **Main Loop Pattern**:
   - Event polling
   - Screen clearing
   - Text rendering
   - Frame presentation

2. **Resource Management**:
   - Font loading on startup
   - Proper cleanup on exit
   - Texture creation/destruction per frame

3. **Rendering Flow**:
   - Create text surface
   - Convert to texture
   - Render texture
   - Clean up resources
