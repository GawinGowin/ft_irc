#include "entrypoint.hpp"
#include <iostream>

int main(int argc, char **argv) {
  try {
    entrypoint(argc, argv);
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return (1);
  }
}