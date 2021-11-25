//
// Created by M.Kojro on 11/24/2021.
//

#include "SubProcess.h"
SubProcess::SubProcess(std::string program,
                       std::vector<std::string> args,
                       std::string path,
                       bool auto_start) : program_(std::move(program)),
                                          args_(std::move(args)),
                                          path_(std::move(path)) {
#if not MK_PLATFORM_WINDOWS
  static_assert(false && "SubProcess is not supported on this platform");
#endif
  CreatePipes().SuccessOrThrow();
  RunChildProcess().SuccessOrThrow();
}

SubProcess::~SubProcess() {
#if MK_PLATFORM_WINDOWS
  KillChildProcess().SuccessOrThrow();
  CloseWindowsHandle(child_prc_).SuccessOrThrow();
  CloseWindowsHandle(child_thread_).SuccessOrThrow();
  CloseWindowsHandle(child_std_out_reader_).SuccessOrThrow();
  CloseWindowsHandle(child_std_out_writer_).SuccessOrThrow();
#endif
}

#if MK_PLATFORM_WINDOWS
Error<void> SubProcess::RunChildProcess(bool wait) {
  STARTUPINFO si;
  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  si.hStdError = child_std_out_writer_;
  si.hStdOutput = child_std_out_writer_;
  si.hStdInput = child_std_out_reader_;
  si.dwFlags |= STARTF_USESTDHANDLES;

  PROCESS_INFORMATION pi;
  ZeroMemory(&pi, sizeof(pi));

  std::stringstream ss;
  ss << program_ << " ";
  for (auto &arg: args_) {
    ss << arg << " ";
  }

  size_t str_size = ss.str().size() + 1;
  char *cmd = new char[str_size];
  strcpy_s(cmd, str_size, ss.str().c_str());
  if (!CreateProcess(NULL, cmd, NULL, NULL, true, 0, NULL, NULL, &si, &pi))
    return Error<void>::error("Creating process failed");
  delete[] cmd;

  child_prc_ = pi.hProcess;
  child_thread_ = pi.hThread;

  if (wait)
    WaitForSingleObject(child_prc_, INFINITE);

  return Error<void>::success();
}
#endif

Error<std::string> SubProcess::ReadChildOutput() {
  char buff[1024];
  int readc = 0;
  if (!ReadFile(child_std_out_reader_, buff, 1024, (LPDWORD) &readc, NULL))
    return Error<std::string>::error("Reading from pipe failed");
  buff[readc] = '\0';
  return Error<std::string>::succes(buff);
}

Error<void> SubProcess::SendChildInput(const std::string &input) {
  int readc = 0;
  if (!WriteFile(child_std_in_writer_, input.c_str(), input.size() + 1, (LPDWORD) readc, NULL)) {
    return Error<void>::error("Could not write to pipe");
  }
  return Error<void>::success();
}

Error<void> SubProcess::CreatePipes() {
#if MK_PLATFORM_WINDOWS
  SECURITY_ATTRIBUTES saAttr;
  saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
  saAttr.bInheritHandle = true;
  saAttr.lpSecurityDescriptor = NULL;

  if (!CreatePipe(&child_std_out_reader_, &child_std_out_writer_, &saAttr, 0))
    return Error<void>::error("Could not create a pipe");

  if (!SetHandleInformation(child_std_out_reader_, HANDLE_FLAG_INHERIT, 0))
    return Error<void>::error("Could not create a pipe");;

  if (!CreatePipe(&child_std_in_reader_, &child_std_in_writer_, &saAttr, 0))
    return Error<void>::error("Could not create a pipe");;

  if (!SetHandleInformation(child_std_in_writer_, HANDLE_FLAG_INHERIT, 0))
    return Error<void>::error("Could not create a pipe");;

  return Error<void>::success();
#endif
}
Error<void> SubProcess::KillChildProcess(int code) {
  if (!TerminateProcess(child_prc_, code))
    return Error<void>::error("Child process could not be terminated");
  return Error<void>::success();
}
void SubProcess::SetArgs(std::vector<std::string> args) {
  args_ = std::move(args);
}
Error<void> SubProcess::CloseWindowsHandle(HANDLE handle) {
  if (!CloseHandle(handle))
    return Error<void>::error("Windows handle could not be closed");
  return Error<void>::success();
}
