//
// Created by piotr on 24/11/2021.
//

#ifndef SPIRA_PLOTTER_PLOT_H_
#define SPIRA_PLOTTER_PLOT_H_
#include "SubProcess.h"
#include "string"
#include <assert.h>
#include <ciso646>
/// Purely virtual plot class, use as a template for future plot classes
class Plot {
public:
  Plot(const std::string &plot_title) : plot_title_(plot_title) {}
  enum class Color { BLUE = 38 };
  /// Run root program with
  void DrawPlot() {
    GenFile();
    SubProcess sub_process("root", {"-l", "-X", plot_title_ + ".C"}, "./");
  }

protected:
  virtual void GenFile() {
    assert(false && "this function should never be used");
  };
  std::string plot_title_;
};

#endif // SPIRA_PLOTTER_PLOT_H_
