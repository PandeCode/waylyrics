#include "lib.hpp"
#include <iostream>
#include <thread>

int main(int argc, char *argv[]) {
  init();

  while (1) {

    auto d = getCurrentLine();
    if (d.has_value()) {
      auto [line, pos, dur] = d.value();
      std::cout << pos << "/" << dur << ": " << line << std::endl;
    }

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);
  }

  return 0;
}
