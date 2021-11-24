//
// Created by piotr on 24/11/2021.
//

#ifndef SPIRA__HISTOGRAM_H_
#define SPIRA__HISTOGRAM_H_

#include "map"
#include "plot.h"
#include <fstream>
#include <optional>
#include <vector>

class Histogram : public Plot {
public:
  Histogram(const std::string &plot_title);

  /// count value in
  void Fill(double value);

  /// populates specified bin with given value
  /// \param bin populated bin id
  /// \param value new bin value
  void AddBin(int bin, double value);

  void GenFile();


  std::optional<double> min_bin_;
  std::optional<double> max_bin_;
  std::optional<unsigned> bin_count_;
  std::string plot_label_;
  std::string x_axis_label_ = "";
  std::string y_axis_label_ = "";


private:
  void ClearScript();
  void AddHeader();
  void DeclareCanvas(unsigned canvas_vertical_offset = 10,
                     unsigned canvas_horizontal_offset = 10,
                     unsigned canvas_width = 1000,
                     unsigned canvas_height = 600);
  void DeclareHistogram();
  void AddHistogramSettings();
  void AddData();
  void AddHistogramDraw();
  void AddCanvasDraw();
  void AddCloseFile();


protected:
  std::string plot_title_;
  std::map<double, double> data_;
  Color color_ = Plot::Color::BLUE;

  std::vector<std::string> generated_script_;
};

#endif // SPIRA__HISTOGRAM_H_
