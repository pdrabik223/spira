//
// Created by M.Kojro on 11/25/2021.
//

#ifndef SPIRA_PLOTTER_ERROR_H_
#define SPIRA_PLOTTER_ERROR_H_

#include <string>
#include <optional>

struct ErrorArgs {
  ErrorArgs(std::string msg) : message(std::move(msg)) {} // NOLINT(google-explicit-constructor)
  ErrorArgs(const char *msg) : message(msg) {} // NOLINT(google-explicit-constructor)

  std::string message;
};

class CustomError : public std::exception {
 public:
  CustomError(ErrorArgs error) : error_(std::move(error)) {} // NOLINT(google-explicit-constructor)
  [[nodiscard]] const char *what() const override { return error_.message.c_str(); }
 private:
  ErrorArgs error_;
};

template<class T>
class Error {
 public:

  static Error<T> error(const std::string &err_msg) { return Error<T>(std::nullopt, err_msg); }
  static Error<T> succes(const T &value) { return Error<T>(value, ""); }

  Error(std::optional<T> value, std::string err_msg) :
      error_message_(std::move(err_msg)),
      value_(std::move(value)) {}

  const std::string &GetError() { return error_message_; };
  const T &GetValue() { return value_.value(); }

  bool IsGood() const { return value_.has_value(); }
  bool IsError() const { return !IsGood(); }

  const T &SuccessOrThrow() {
    if (IsGood())
      return GetValue();
    throw CustomError(GetError());
  }

 private:
  std::optional<T> value_;
  std::string error_message_;
};

template<>
class Error<void> {
 public:

  static Error<void> error(const std::string &err_msg) { return {false, err_msg}; }
  static Error<void> success() { return {true, ""}; }

  Error(bool success, std::string err_msg) :
      error_message_(std::move(err_msg)), success_(success) {}

  const std::string &GetError() { return error_message_; };

  [[nodiscard]] bool IsGood() const { return success_; }
  [[nodiscard]] bool IsError() const { return !success_; }

  operator bool() const { return IsGood(); } // NOLINT(google-explicit-constructor)

  void SuccessOrThrow() {
    if (IsGood())
      return;
    throw CustomError(GetError());
  }

 private:
  bool success_ = false;
  std::string error_message_;
};


#endif //SPIRA_PLOTTER_ERROR_H_
