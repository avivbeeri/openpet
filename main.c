#include <SDL2/SDL.h>
//Using SDL and standard IO
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "vpet.h"
#include "sprites.h"

//Screen dimension constants
const int SCALE = 8;
const int GAME_WIDTH = 32;
const int GAME_HEIGHT = 32;
const int SCREEN_WIDTH = GAME_WIDTH * SCALE;
const int SCREEN_HEIGHT = GAME_HEIGHT * SCALE;

void RenderWeirdGradient(void *pixels)
{
  int PITCH = GAME_WIDTH;
  uint32_t *pixel = (uint32_t *)pixels;
  for (int x = 0; x < GAME_WIDTH; x++) {
    *pixel = 0xFF;
    pixel += 1 + PITCH;
  }
}

void set_alpha_pixel(void *pixels, uint8_t x, uint8_t y, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) 
{
  uint8_t PITCH = GAME_WIDTH;
  uint32_t *pixel = (uint32_t *)pixels;
  pixel += y * PITCH + x;
  *pixel = (alpha << 8*3) | (blue << 8*2) | (green << 8*1) | red;
}

void set_pixel(void *pixels, uint8_t x, uint8_t y, uint8_t red, uint8_t green, uint8_t blue) 
{
  set_alpha_pixel(pixels, x, y, red, green, blue, 0xFF);
}

void ClearDisplay(void *pixels)
{
  for (int x = 0; x < GAME_WIDTH; x++) {
    for (int y = 0; y < GAME_HEIGHT; y++) {
      set_pixel(pixels, x, y, 0xB5, 0xC0, 0xAC);
    }
  }
}


void draw_sprite(void *pixels, uint8_t *sprite, uint8_t startX, uint8_t startY)
{
  for (int x = startX; x < GAME_WIDTH; x++) {
    for (int y = startX; y < GAME_HEIGHT; y++) {
      uint8_t *pixel = &sprite[y * 16 + x];
      if (*pixel != 0) {
        set_pixel(pixels, x, y, 0xFF, 0xFF, 0xFF);
      }
    }
  }
}

bool HandleEvent(SDL_Event *Event)
{
  bool ShouldQuit = false;
  switch (Event->type)
  {
    case SDL_QUIT:
      {
        printf("SDL_QUIT\n");
        ShouldQuit = true;
      } break;
    case SDL_WINDOWEVENT:
      {
        switch(Event->window.event)
        {
          case SDL_WINDOWEVENT_RESIZED:
            {
            } break;
          case SDL_WINDOWEVENT_EXPOSED:
            {
            } break;
        }
      } break;
  }
  return(ShouldQuit);
}

int main(int argc, char* args[]) 
{

  //The window we'll be rendering to
  SDL_Window* window = NULL;
  
  // Create a "Renderer" for our window.
  SDL_Renderer *renderer = NULL;

  //Initialize SDL
  if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
  {
    printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    goto CLEANUP;
  }

  window = SDL_CreateWindow("Digimon", 
      SDL_WINDOWPOS_UNDEFINED, 
      SDL_WINDOWPOS_UNDEFINED, 
      SCREEN_WIDTH, 
      SCREEN_HEIGHT, 
      SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE 
  );
  if( window == NULL )
  {
    printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
    goto CLEANUP;
  }
  

  renderer = SDL_CreateRenderer(window, -1, 0);
  if( renderer == NULL )
  {
    printf( "Renderer could not be created! SDL_Error: %s\n", SDL_GetError() );
    goto CLEANUP;
  }
  SDL_Texture *texture = NULL;

  texture = SDL_CreateTexture(renderer,
      SDL_PIXELFORMAT_ARGB8888,
      SDL_TEXTUREACCESS_STREAMING,
      GAME_WIDTH,
      GAME_HEIGHT);
  
  if( texture == NULL )
  {
    printf( "Texture could not be created! SDL_Error: %s\n", SDL_GetError() );
    goto CLEANUP;
  }
  void *bitmap_memory = malloc(GAME_WIDTH * GAME_HEIGHT * 4);
  if( bitmap_memory == NULL )
  {
    printf( "Video memory could not be created! SDL_Error: %s\n", SDL_GetError() );
    goto CLEANUP;
  }
  SDL_Rect screenRect;
  screenRect.x = 0;
  screenRect.y = 0;
  screenRect.w = SCREEN_WIDTH;
  screenRect.h = SCREEN_HEIGHT;

  VPET vpet;

  // EVENT LOOP
  for(;;)
  {
    SDL_Event Event;
    while (SDL_PollEvent(&Event))
    {
      if (HandleEvent(&Event))
      {
        goto CLEANUP;
      }
    }
    // DRAW STEP
    ClearDisplay(bitmap_memory);
    set_pixel(bitmap_memory, 16, 16, 0x00, 0x00, 0x00);
    draw_sprite(bitmap_memory, botamon_sprite, 8, 8);

    // FLIP BUFFER
    if (SDL_UpdateTexture(texture,
          0,
          bitmap_memory,
          GAME_WIDTH * 4))
    {
      // TODO: Do something about this error!
    }
    SDL_RenderCopy(renderer,
        texture,
        0,
        &screenRect);
    SDL_RenderPresent(renderer);
  }

  CLEANUP:
  free(bitmap_memory);
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  //Quit SDL subsystems
  SDL_Quit();

  return 0;
}
