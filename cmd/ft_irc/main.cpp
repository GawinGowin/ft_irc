#include "presentation/entrypoint.hpp"
#include <iostream>

volatile sig_atomic_t g_signal = 0;

int main(int argc, char **argv) {
  try {
    entrypoint(argc, argv);
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return (1);
  }
}