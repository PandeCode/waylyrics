#include "lib.hpp"

int main(int argc, char *argv[]) {
  init();

  while (1) {

    auto line = getCurrentLine();
    if (line.has_value()) {
      std::cout << line.value() << std::endl;
    }

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);
  }

  return 0;
}
