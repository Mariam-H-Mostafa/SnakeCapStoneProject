#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include <thread>
#include <future>

int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{800};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;
  Game game(kGridWidth, kGridHeight);
  game.Run(controller, renderer, kMsPerFrame);
  std::cout << "Game has terminated successfully!\n";
  std::vector<int> final_score(2);
  final_score = game.GetScore();
  std::cout << "Score: " << final_score[0]<< "\n";
  std::cout << "Score2: " << final_score[1]<< "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  return 0;
}