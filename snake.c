// gcc -o snake snake.c -lSDL2 -lSDL2_ttf

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define CELL_SIZE 20

typedef struct {
  int x;
  int y;
} Position;

SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font* font;
Position snake[(SCREEN_WIDTH / 20) * (SCREEN_HEIGHT / 20)];  // Set snake length to maximum cells
int snakeLength;
Position food;
Position velocity;
int gameState;

void init() {
  // SDL
  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
  // Font
  TTF_Init();
  font = TTF_OpenFont("./OpenSans-Regular.ttf", 24);  // Replace "path/to/font.ttf" with the actual path to your font file and the desired font size
  if (font == NULL) {
    printf("Failed to load font: %s\n", TTF_GetError());
    // Handle the error condition
  }
  // Time
  srand(time(NULL));  // seed random number generator
  // Game
  gameState = 2;  // Set to starting game state
}

void initSnake() {
  snake[0].x = SCREEN_WIDTH / 2;
  snake[0].y = SCREEN_HEIGHT / 2;
  snakeLength = 3;
  food.x = rand() % (SCREEN_WIDTH / CELL_SIZE) * CELL_SIZE;
  food.y = rand() % (SCREEN_HEIGHT / CELL_SIZE) * CELL_SIZE;
  velocity.x = CELL_SIZE;
  velocity.y = 0;
}

void handleInput() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      gameState = 0;
      return;
    }

    if (event.type == SDL_KEYDOWN) {
      // Start screen
      if (gameState == 2) {
        initSnake();
        gameState = 1;
        return;
      }
      // Snake directions
      switch (event.key.keysym.sym) {
        case SDLK_UP:
          if (velocity.y != CELL_SIZE) {
            velocity.x = 0;
            velocity.y = -CELL_SIZE;
          }
          break;
        case SDLK_DOWN:
          if (velocity.y != -CELL_SIZE) {
            velocity.x = 0;
            velocity.y = CELL_SIZE;
          }
          break;
        case SDLK_LEFT:
          if (velocity.x != CELL_SIZE) {
            velocity.x = -CELL_SIZE;
            velocity.y = 0;
          }
          break;
        case SDLK_RIGHT:
          if (velocity.x != -CELL_SIZE) {
            velocity.x = CELL_SIZE;
            velocity.y = 0;
          }
          break;
      }
    }
  }
}

void update() {
  // Make snake go
  snake[0].x += velocity.x;
  snake[0].y += velocity.y;

  // Collision with self
  for (int i = 1; i < snakeLength; i++) {
    if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
      gameState = 2;  // Restart game
      return;
    }
  }

  // Wrap around screen
  if (snake[0].x >= SCREEN_WIDTH) {
    snake[0].x = 0;
  } else if (snake[0].x < 0) {
    snake[0].x = SCREEN_WIDTH - CELL_SIZE;
  }
  if (snake[0].y >= SCREEN_HEIGHT) {
    snake[0].y = 0;
  } else if (snake[0].y < 0) {
    snake[0].y = SCREEN_HEIGHT - CELL_SIZE;
  }

  // Food
  if (snake[0].x == food.x && snake[0].y == food.y) {
    food.x = rand() % (SCREEN_WIDTH / CELL_SIZE) * CELL_SIZE;
    food.y = rand() % (SCREEN_HEIGHT / CELL_SIZE) * CELL_SIZE;

    snakeLength++;
  }

  for (int i = snakeLength - 1; i > 0; i--) {
    snake[i].x = snake[i - 1].x;
    snake[i].y = snake[i - 1].y;
  }
}

void render() {
  // Don't render if quit
  if (gameState == 0) return;

  // Reset
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  switch (gameState) {
    case 1: {
      // Render Snake and food
      update();  // Update their positions first

      // Snake
      SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
      for (int i = 0; i < snakeLength; i++) {
        SDL_Rect snakeRect = {snake[i].x, snake[i].y, CELL_SIZE, CELL_SIZE};
        SDL_RenderFillRect(renderer, &snakeRect);
      }

      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
      SDL_Rect foodRect = {food.x, food.y, CELL_SIZE, CELL_SIZE};
      SDL_RenderFillRect(renderer, &foodRect);

      SDL_RenderPresent(renderer);
      break;
    }
    case 2: {
      // Render the start screen
      SDL_Color textColor = {255, 255, 255, 255};
      SDL_Surface* startScreenSurface = TTF_RenderText_Solid(font, "Press any key to start", textColor);
      SDL_Texture* startScreenTexture = SDL_CreateTextureFromSurface(renderer, startScreenSurface);

      SDL_Rect startScreenRect;
      startScreenRect.x = (SCREEN_WIDTH - startScreenSurface->w) / 2;
      startScreenRect.y = (SCREEN_HEIGHT - startScreenSurface->h) / 2;
      startScreenRect.w = startScreenSurface->w;
      startScreenRect.h = startScreenSurface->h;

      SDL_RenderCopy(renderer, startScreenTexture, NULL, &startScreenRect);

      SDL_FreeSurface(startScreenSurface);
      SDL_DestroyTexture(startScreenTexture);
      // render
      SDL_RenderPresent(renderer);
      break;
    }
  }
}

void cleanUp() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  TTF_CloseFont(font);
  TTF_Quit();
  
  SDL_Quit();
}

int main() {
  init();

  while (gameState != 0) {
    handleInput();
    render();
    SDL_Delay(90);
  }

  cleanUp();
  return 0;
}