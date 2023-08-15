#include "controller.h"
#include <iostream>
#include "SDL.h"
#include "snake.h"
#include "mutex"

void Controller::ChangeDirection(Snake &snake, Snake::Direction input,
                                 Snake::Direction opposite) const {
  if (snake.direction != opposite || snake.size == 1) snake.direction = input;
  return;
}

void Controller::HandleInput(bool &running, Snake &snake, Snake &snake_2) const {
  std::mutex mutex;
  mutex.lock();
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
    } else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_UP:
          ChangeDirection(snake, Snake::Direction::kUp,
                          Snake::Direction::kDown);
          break;

        case SDLK_DOWN:
          ChangeDirection(snake, Snake::Direction::kDown,
                          Snake::Direction::kUp);
          break;

        case SDLK_LEFT:
          ChangeDirection(snake, Snake::Direction::kLeft,
                          Snake::Direction::kRight);
          break;

        case SDLK_RIGHT:
          ChangeDirection(snake, Snake::Direction::kRight,
                          Snake::Direction::kLeft);
          break;
        case SDLK_i:
          ChangeDirection(snake_2, Snake::Direction::kUp,
                          Snake::Direction::kDown);
          break;
        case SDLK_k:
          ChangeDirection(snake_2, Snake::Direction::kDown,
                          Snake::Direction::kUp);
          break;
        case SDLK_l:
          ChangeDirection(snake_2, Snake::Direction::kRight,
                          Snake::Direction::kLeft);
          break;
        case SDLK_j:
          ChangeDirection(snake_2, Snake::Direction::kLeft,
                          Snake::Direction::kRight);
          break;
      }
    }
  }
  mutex.unlock();
}