//
// Created by piotr on 24/11/2021.
//

#include "histogram.h"
Histogram::Histogram(const std::string &plot_title)
    : plot_title_(plot_title), plot_label_(plot_title) {}
void Histogram::Fill(double value) {
  if (data_.find(value) == data_.end())
    data_.insert({value, 1});
  else
    data_[value]++;
}
void Histogram::ClearScript() { generated_script_.clear(); }
void Histogram::AddHeader() {
  generated_script_.push_back("void" + plot_label_ + "(){");
}
void Histogram::DeclareCanvas(unsigned int canvas_vertical_offset,
                              unsigned int canvas_horizontal_offset,
                              unsigned int canvas_width,
                              unsigned int canvas_height) {

  generated_script_.push_back("auto c1 = new TCanvas(\"c1\"," + plot_label_ +
                              ',' + std::to_string(canvas_vertical_offset) +
                              ',' + std::to_string(canvas_horizontal_offset) +
                              ',' + std::to_string(canvas_width) + ',' +
                              std::to_string(canvas_height) + ");");
}
void Histogram::DeclareHistogram() {



  generated_script_.push_back("auto histogram = new TH1I(" + plot_label_ + ',' +
                              plot_title_ + ';' + x_axis_label_ + ';' +
                              y_axis_label_ + "\");");

}
