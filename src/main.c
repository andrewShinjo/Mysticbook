#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include "./DrawLayer.h"

#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 600
#define DEFAULT_WINDOW_FLAGS SDL_WINDOW_RESIZABLE

/**
 * Called once before anything else.
 *
 * If SDL_APP_CONTINUE is returned, then the app runs.
 * If SDL_APP_FAILURE is returned, then the app calls SDL_AppQuit
 * and terminates with a failure exit code.
 * If SDL_APP_SUCCESS is returned, then the app calls SDL_AppQuit
 * and terminates with a success exit code.
 *
 * Assigning a pointer to appstate will make the pointer available to
 * later function calls in their appstate parameter.
 */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{
  // Initialize SDL3 video subsystem.

  bool init_failed = !SDL_Init(SDL_INIT_VIDEO);

  if(init_failed)
  {
    SDL_Log("SDL_Init failed: %s\n", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  // Create window.

  SDL_Window *window = SDL_CreateWindow(
    "Mysticbook",
    DEFAULT_WINDOW_WIDTH,
    DEFAULT_WINDOW_HEIGHT,
    DEFAULT_WINDOW_FLAGS
  );
  bool window_creation_failed = !window;

  if(window_creation_failed)
  {
    SDL_Log("SDL_CreateWindow failed: %s\n", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_RaiseWindow(window);

  // Initialize renderer.

  SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
  bool renderer_creation_failed = !renderer;

  if(renderer_creation_failed)
  {
    SDL_Log("SDL_CreateRenderer failed: %s\n", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  // Create DrawLayer
  DrawLayer *draw = draw_create(window, renderer);

  return SDL_APP_CONTINUE;
}

/**
 * Called over and over again.
 *
 * If SDL_APP_CONTINUE is returned, then the app continues.
 * If SDL_APP_FAILURE is returned, then the app calls SDL_AppQuit
 * and terminates with a failure exit code.
 * If SDL_APP_SUCCESS is returned, then the app calls SDL_AppQuit
 * and terminates with a success exit code.
 */
SDL_AppResult SDL_AppIterate(void *appstate)
{
  return SDL_APP_CONTINUE;
}

/**
 * Called whenever an SDL event arrives.
 *
 * If SDL_APP_CONTINUE is returned, then the app continues.
 * If SDL_APP_FAILURE is returned, then the app calls SDL_AppQuit
 * and terminates with a failure exit code.
 * If SDL_APP_SUCCESS is returned, then the app calls SDL_AppQuit
 * and terminates with a success exit code.
*/
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
  switch(event->type)
  {
    case SDL_EVENT_QUIT:
    {
      return SDL_APP_SUCCESS;
    }
    default: 
    {
      return SDL_APP_CONTINUE;
    }
  }
}

/**
 * Called once before terminating the app.
 * If you set appstate in the app, then free it here.
 */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{

}
