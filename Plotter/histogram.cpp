//
// Created by piotr on 24/11/2021.
//

#include "histogram.h"
Histogram::Histogram(const std::string &plot_title)
    : Plot(plot_title), plot_label_(plot_title) {}
void Histogram::Fill(double value) {
  if (data_.find(value) == data_.end())
    data_.insert({value, 1});
  else
    data_[value]++;
}
void Histogram::ClearScript() { generated_script_.clear(); }
void Histogram::AddHeader() {
  generated_script_.push_back("void " + plot_label_ + "(){");
}
void Histogram::DeclareCanvas(unsigned int canvas_vertical_offset,
                              unsigned int canvas_horizontal_offset,
                              unsigned int canvas_width,
                              unsigned int canvas_height) {

  generated_script_.push_back("auto c1 = new TCanvas(\"c1\",\"" + plot_label_ +
                              "\"," + std::to_string(canvas_vertical_offset) +
                              ',' + std::to_string(canvas_horizontal_offset) +
                              ',' + std::to_string(canvas_width) + ',' +
                              std::to_string(canvas_height) + ");");
}
void Histogram::DeclareHistogram() {

  if (min_bin_.has_value() and max_bin_.has_value() and bin_count_.has_value())
    generated_script_.push_back("auto histogram = new TH1I(\"" + plot_label_ +
                                "\",\"" + plot_title_ + ';' + x_axis_label_ +
                                ';' + y_axis_label_ + "\"," +
                                std::to_string((int)bin_count_.value()) + "," +
                                std::to_string(min_bin_.value()) + "," +
                                std::to_string(max_bin_.value()) + ");");

  else {

    generated_script_.push_back("auto histogram = new TH1I(\"" + plot_label_ +
                                "\",\"" + plot_title_ + ';' + x_axis_label_ +
                                ';' + y_axis_label_ + "\");");
  }
}
void Histogram::AddHistogramSettings() {

  generated_script_.emplace_back("histogram->SetStats(0);");
  switch (color_) {

  case Color::BLUE:
    generated_script_.emplace_back("histogram->SetFillColor(38);");
    break;
  }
}
void Histogram::AddData() {

  for (auto &d : data_) {

    for (int i = 0; i < d.second; i++)
      generated_script_.push_back("histogram->Fill(" + std::to_string(d.first) +
                                  ");");
  }
}
void Histogram::AddHistogramDraw() {
  generated_script_.emplace_back("histogram->Draw(\"B\");");
}
void Histogram::AddCanvasDraw() {
  generated_script_.emplace_back("c1->BuildLegend();");
  generated_script_.emplace_back("c1->Draw();");
}
void Histogram::AddCloseFile() { generated_script_.emplace_back("}"); }
void Histogram::GenFile() {
  ClearScript();
  AddHeader();
  DeclareCanvas();
  DeclareHistogram();
  AddHistogramSettings();
  AddData();
  AddHistogramDraw();
  AddCanvasDraw();
  AddCloseFile();

  std::fstream file(plot_label_ + ".C", std::ios::out);
  for (const auto &line : generated_script_) {
    file << line << "\n";
  }
  file.close();
}
