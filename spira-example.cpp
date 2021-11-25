//
// Created by piotr on 24/11/2021.
//
#include "histogram.h"
#include "SubProcess.h"
#include <iostream>
int main() {

  Histogram test("test");
  SubProcess s("root", {"-l", "-e", ".demo"}, "./");
  std::cout << "=== [ SPIRA ] ===" << std::endl
            << "Write 'q' to quit" << std::endl;
  char x;
  do {
    std::cout << ">>> ";
    std::cin >> x;
    std::cout << std::endl;
  } while (x != 'q');
}