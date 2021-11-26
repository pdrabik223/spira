//
// Created by piotr on 24/11/2021.
//
#include "histogram.h"
#include "SubProcess.h"
#include <iostream>
int main() {
  try {

    Histogram test("test");
    test.min_bin_ = 0;
    test.max_bin_ = 100*100;
    test.bin_count_ = 100;

    for(int i =0;i<100;i++){
      test.Fill(i*i);
    }

    test.GenFile();

    SubProcess s("root", {"-l", "-X", "../test.cpp"}, "./");
    std::cout << "=== [ SPIRA ] ===" << std::endl
              << "Write 'q' to quit" << std::endl;
    char x;
    do {
      std::cout << ">>> ";
      std::cin >> x;
      std::cout << std::endl;
    } while (x != 'q');

  } catch (const CustomError &e) {
    std::cout << "Runtime error: " << e.what() << std::endl;
    return 1;
  }
}