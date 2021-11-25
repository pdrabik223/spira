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
#include <iostream>

#include "Error.h"

class SubProcess {
 public:
  SubProcess(std::string program,
             std::vector<std::string> args,
             std::string path,
             bool auto_start = true);

  ~SubProcess();

  void SetArgs(std::vector<std::string> args);
  Error<void> RunChildProcess(bool wait = false);
  Error<void> KillChildProcess(int code = 1);

  Error<std::string> ReadChildOutput();
  Error<void> SendChildInput(const std::string &input);

 protected:
  Error<void> CreatePipes();
 private:

#if MK_PLATFORM_WINDOWS

  Error<void> CloseWindowsHandle(HANDLE handle);

  HANDLE child_prc_{};
  HANDLE child_thread_{};

  HANDLE child_std_out_writer_{};
  HANDLE child_std_out_reader_{};
  HANDLE child_std_in_writer_{};
  HANDLE child_std_in_reader_{};

#endif

  std::string program_;
  std::vector<std::string> args_;
  std::string path_;
};

#endif //SPIRA_PLOTTER_SUBPROCESS_H_
