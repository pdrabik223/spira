//
// Created by piotr on 24/11/2021.
//
#include "SubProcess.h"
#include "histogram.h"
#include <iostream>
int main() {
  //  try {
  //
  Histogram test("test");
  test.min_bin_ = 0;
  test.max_bin_ = 100;
  test.bin_count_ = 100;

  for (int i = 0; i < 100; i++) {
    test.Fill(i);
  }
  test.DrawPlot();
}