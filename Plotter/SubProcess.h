//
// Created by M.Kojro on 11/24/2021.
//

#ifndef SPIRA_PLOTTER_SUBPROCESS_H_
#define SPIRA_PLOTTER_SUBPROCESS_H_

#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__))
#define MK_PLATFORM_WINDOWS true
#include <windows.h>
#else
#define MK_PLATFORM_WINDOWS false;
#endif

#include <string>
#include <utility>
#include <vector>
#include <sstream>
#include <optional>

template<class T>
class Error {
 public:
//  template<class Tout>
  static Error<T> error(const std::string &err_msg) { return Error<T>(std::nullopt, err_msg); }
//  template<class Tout>
  static Error<T> succes(const T &value) { return Error<T>(value, ""); }

  Error(std::optional<T> value, std::string err_msg) :
      error_message_(std::move(err_msg)),
      value_(std::move(value)) {}

  const std::string &GetError() { return error_message_; };
  const T &GetValue() { return value_.value(); }

  bool IsGood() const { return value_.has_value(); }
  bool IsError() const { return !IsGood(); }

 private:
  std::optional<T> value_;
  std::string error_message_;
};
template<>
class Error<void> {
 public:
//  template<class Tout>
  static Error<void> error(const std::string &err_msg) { return {false, err_msg}; }
//  template<class Tout>
  static Error<void> succes() { return {true, ""}; }

  Error(bool succes, std::string err_msg) :
      error_message_(std::move(err_msg)), succes_(succes) {}

  const std::string &GetError() { return error_message_; };
//  GetValue() { return value_.value(); }

  [[nodiscard]] bool IsGood() const { return succes_; }
  [[nodiscard]] bool IsError() const { return !succes_; }

 private:
  bool succes_ = false;
  std::string error_message_;
};

class SubProcess {
 public:
  SubProcess(std::string program,
             std::vector<std::string> args,
             std::string path);

  ~SubProcess();

  Error<std::string> ReadChildOutput();
  Error<void> SendChildInput(const std::string &input);

 protected:
  Error<void> RunProc(bool wait = false);
  Error<void> CreatePipes();
 private:

#if MK_PLATFORM_WINDOWS

  HANDLE child_prc_;
  HANDLE child_thread_;

  HANDLE child_std_out_writer_;
  HANDLE child_std_out_reader_;
  HANDLE child_std_in_writer_;
  HANDLE child_std_in_reader_;

#endif

  std::string program_;
  std::vector<std::string> args_;
  std::string path_;
};

#endif //SPIRA_PLOTTER_SUBPROCESS_H_
