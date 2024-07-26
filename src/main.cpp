
#include "app/lve_app.hpp"
#include <cstdlib>
#include <exception>
#include <iostream>

int main(void){
  lve::FirstApp app{};

  try{
    app.run();
  } catch(const std::exception &e){
    std::cerr << e.what() << "\n";
    return EXIT_FAILURE;
  }
  return 0;
}
