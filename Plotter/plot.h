//
// Created by piotr on 24/11/2021.
//

#ifndef SPIRA_PLOTTER_PLOT_H_
#define SPIRA_PLOTTER_PLOT_H_
#include "string"


/// Purely virtual plot class, use as a template for future plot classes
class Plot {
public:
  enum class Color{
    BLUE = 38

  };
private:

  /// Run root program with
  virtual void DrawPlot() = delete;

protected:
  std::string plot_title;
};

#endif // SPIRA_PLOTTER_PLOT_H_
