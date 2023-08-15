#include "game.h"
#include <iostream>
#include "SDL.h"
#include "mutex"
#include <vector>
#include <algorithm>
#include <thread>
#include <future>


Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height), snake_2(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
  PlaceFood();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake, snake_2);
    Update();
    renderer.Render(snake, snake_2, food);
   
    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, score_2, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);

    std::cout <<"Collision is" << collision<<"\n";  
    auto hit = std::async(std::launch::async, &Game::Detect_Collision, this);
    hit.wait();
    collision = hit.get();
    if (collision == true)
    {
      std::cout <<"Collision is" << collision<<"\n";
      SDL_Window *screen = SDL_CreateWindow("COLLISION", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, 150, 150, SDL_WINDOW_SHOWN);
      SDL_Delay (5);
      SDL_DestroyWindow(screen);
      collision = false;
    }
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)&&!snake_2.SnakeCell(x,y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::Update() {
  std::mutex mutex2;
  mutex2.lock();
  if (!snake.alive || !snake_2.alive) return;

  snake.Update();
  snake_2.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  int new_2_x = static_cast<int>(snake_2.head_x);
  int new_2_y = static_cast<int>(snake_2.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }
  // Check if there's food over here
  if (food.x == new_2_x && food.y == new_2_y) {
    score_2++;
    PlaceFood();
    // Grow snake and increase speed.
    snake_2.GrowBody();
    snake_2.speed +=0.02;
  }
  mutex2.unlock();
}

std::vector<int> Game::GetScore() const { 
  return {score, score_2};
}
int Game::GetSize() const { return snake.size; }

inline bool operator==(SDL_Point const &a, SDL_Point const &b)
{
    return a.x == b.x && a.y == b.y;
}
inline bool operator<(SDL_Point const &a, SDL_Point const &b)
{
  return a.x < b.x && a.y < b.y;
}

bool Game::Detect_Collision() {
    std::sort(snake.body.begin(),snake.body.end());
    std::sort(snake_2.body.begin(),snake_2.body.end());
    if (snake.body == snake_2.body){
          return true;
    }                  
}