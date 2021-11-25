//
// Created by M.Kojro on 11/24/2021.
//

#include "SubProcess.h"
SubProcess::SubProcess(std::string program,
                       std::vector<std::string> args,
                       std::string path) : program_(std::move(program)),
                                           args_(std::move(args)),
                                           path_(std::move(path)) {
  CreatePipes();
  RunProc();
}

SubProcess::~SubProcess() {
#if MK_PLATFORM_WINDOWS
  TerminateProcess(child_prc_, 1);
  CloseHandle(child_prc_);
  CloseHandle(child_thread_);
  CloseHandle(child_std_out_reader_);
  CloseHandle(child_std_out_writer_);
#endif
}

#if MK_PLATFORM_WINDOWS
Error<void> SubProcess::RunProc(bool wait) {
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
  if (CreateProcess(NULL, cmd, NULL, NULL, true, 0, NULL, NULL, &si, &pi))
    return Error<void>::error("Creating process failed");
  delete[] cmd;

  child_prc_ = pi.hProcess;
  child_thread_ = pi.hThread;

  if (wait)
    WaitForSingleObject(child_prc_, INFINITE);

  return Error<void>::succes();
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
  return Error<void>::succes();
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

  return Error<void>::succes();
#endif
}