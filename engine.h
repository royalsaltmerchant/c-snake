#ifndef ENGINE_H
#define ENGINE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
  SDL_Window* window;
  SDL_Renderer* renderer;
  TTF_Font* font;
} Engine;

void initEngine(Engine *engine, const char *title, int screenWidth, int screenHeight, const char *fontLocation, int fontSize);

void freeEngine(Engine *engine);

void clearRender(Engine *engine);

SDL_Rect createTextCentered(Engine *engine, int x, int y, SDL_Color color);

#endif
