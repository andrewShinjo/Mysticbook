# Technical Context: Mysticbook

## Technologies Used
- SDL2 for window management and rendering
- SDL_ttf for text rendering
- Open Sans font (Regular variant)

## Implementation Details
- Window: 800x600 pixels, resizable
- Renderer: Accelerated with SDL_RENDERER_ACCELERATED flag
- Text Rendering:
  - Using TTF_RenderText_Blended() for anti-aliased rendering
  - Font size: 24pt
  - Color: Black (RGBA: 0, 0, 0, 255)
  - Centered on screen

## Dependencies
- SDL2
- SDL2_ttf
- Freetype (via SDL_ttf)
