#include "engine.h"

void initEngine(Engine *engine, const char *title, int screenWidth, int screenHeight, const char *fontLocation, int fontSize) {
  // SDL
  SDL_Init(SDL_INIT_VIDEO);
  engine->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
  engine->renderer = SDL_CreateRenderer(engine->window, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawColor(engine->renderer, 0, 0, 0, 255);
  SDL_RenderClear(engine->renderer);
  SDL_RenderPresent(engine->renderer);
  // Font
  TTF_Init();
  engine->font = TTF_OpenFont(fontLocation, fontSize);
  if (engine->font == NULL) {
    fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
    exit(1);  // Terminate the program with a non-zero exit status
  }
}

void freeEngine(Engine *engine) {
  SDL_DestroyRenderer(engine->renderer);
  SDL_DestroyWindow(engine->window);

  TTF_CloseFont(engine->font);
  TTF_Quit();

  SDL_Quit();
}

void clearRender(Engine *engine) {
  SDL_SetRenderDrawColor(engine->renderer, 0, 0, 0, 255);
  SDL_RenderClear(engine->renderer);
}

SDL_Rect createTextCentered(Engine *engine, int x, int y, SDL_Color color) {
  SDL_Surface *surface = TTF_RenderText_Solid(engine->font, "Press any key to start", color);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(engine->renderer, surface);

  SDL_Rect text;
  text.x = (x - surface->w) / 2;
  text.y = (y - surface->h) / 2;
  text.w = surface->w;
  text.h = surface->h;

  SDL_RenderCopy(engine->renderer, texture, NULL, &text);

  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);

  return text;
}