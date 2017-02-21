#include <signal.h>
#include <sys/ioctl.h>
#include <v8.h>
#include <node.h>
#include <unistd.h>
#include <nan.h>

using namespace v8;
using namespace node;


NAN_METHOD(GetForegroundProcessGroup) {
  Nan::HandleScope scope;

  int fd = Handle<Integer>::Cast(info[0])->Value();
  pid_t res = tcgetpgrp(fd);

  info.GetReturnValue().Set(Nan::New<Integer>(res));
}

NAN_METHOD(SetForegroundProcessGroup) {
  Nan::HandleScope scope;

  int fd = Handle<Integer>::Cast(info[0])->Value();
  pid_t pid = Handle<Integer>::Cast(info[1])->Value();
  int res = tcsetpgrp(fd,pid);

  info.GetReturnValue().Set(Nan::New<Integer>(res));
}

NAN_METHOD(IgnoreSig) {
  Nan::HandleScope scope;

  sigset_t set;
  sigemptyset(&set);
  sigaddset(&set, SIGTTOU);
  // sigaddset(&set, SIGTTIN);
  sigaddset(&set, SIGTSTP);
  // sigaddset(&set, SIGCHLD);
  sigprocmask(SIG_BLOCK, &set, NULL);

  return;
}

NAN_METHOD(SetControllingTTY) {
  Nan::HandleScope scope;

  int fd = info[0]->Int32Value();
  int rt = ioctl (fd, TIOCSCTTY, NULL);

  info.GetReturnValue().Set(Nan::New<Integer>(rt));
}

NAN_METHOD(VHangUp) {
  Nan::HandleScope scope;

  // `vhangup` is a Linux-specific function, simply do a no-op and return
  // success in other platforms
  #if __linux__
    int ret = vhangup();
  #else
    int ret = 0;
  #endif

  info.GetReturnValue().Set(Nan::New<Integer>(ret));
}

NAN_METHOD(SetGID) {
  Nan::HandleScope scope;

  pid_t pid = Handle<Integer>::Cast(info[0])->Value();
  pid_t pgid = Handle<Integer>::Cast(info[1])->Value();
  int res = setpgid(pid,pgid);

  info.GetReturnValue().Set(Nan::New<Integer>(res));
}

NAN_METHOD(GetGID) {
  Nan::HandleScope scope;

  pid_t pid = Handle<Integer>::Cast(info[0])->Value();
  int res = getpgid(pid);

  info.GetReturnValue().Set(Nan::New<Integer>(res));
}

NAN_METHOD(SetSID) {
  Nan::HandleScope scope;

  int sid = setsid();

  info.GetReturnValue().Set(Nan::New<Integer>(sid));
}

NAN_METHOD(GetSID) {
  Nan::HandleScope scope;

  pid_t pid = Handle<Integer>::Cast(info[0])->Value();
  int sid = getsid(pid);

  info.GetReturnValue().Set(Nan::New<Integer>(sid));
}

NAN_METHOD(Fork) {
  Nan::HandleScope scope;

  int pid = fork();

  info.GetReturnValue().Set(Nan::New<Integer>(pid));
}

NAN_METHOD(Dup) {
  Nan::HandleScope scope;

  int fd = info[0]->Int32Value();
  int dd = dup(fd);

  info.GetReturnValue().Set(Nan::New<Integer>(dd));
}

NAN_METHOD(Dup2) {
  Nan::HandleScope scope;

  int oldfd = info[0]->Int32Value();
  int newfd = info[0]->Int32Value();

  int dd = dup2(oldfd, newfd);

  info.GetReturnValue().Set(Nan::New<Integer>(dd));
}


void init(Handle<Object> exports) {
  exports->Set(Nan::New<String>("ignore").ToLocalChecked(),
    Nan::New<FunctionTemplate>(IgnoreSig)->GetFunction());
  exports->Set(Nan::New<String>("getpgid").ToLocalChecked(),
    Nan::New<FunctionTemplate>(GetGID)->GetFunction());
  exports->Set(Nan::New<String>("setpgid").ToLocalChecked(),
    Nan::New<FunctionTemplate>(SetGID)->GetFunction());
  exports->Set(Nan::New<String>("tcgetpgrp").ToLocalChecked(),
    Nan::New<FunctionTemplate>(GetForegroundProcessGroup)->GetFunction());
  exports->Set(Nan::New<String>("tcsetpgrp").ToLocalChecked(),
    Nan::New<FunctionTemplate>(SetForegroundProcessGroup)->GetFunction());
  exports->Set(Nan::New<String>("dup").ToLocalChecked(),
    Nan::New<FunctionTemplate>(Dup)->GetFunction());
  exports->Set(Nan::New<String>("dup2").ToLocalChecked(),
    Nan::New<FunctionTemplate>(Dup2)->GetFunction());
  exports->Set(Nan::New<String>("fork").ToLocalChecked(),
    Nan::New<FunctionTemplate>(Fork)->GetFunction());
  exports->Set(Nan::New<String>("setControllingTTY").ToLocalChecked(),
    Nan::New<FunctionTemplate>(SetControllingTTY)->GetFunction());
  exports->Set(Nan::New<String>("vhangup").ToLocalChecked(),
    Nan::New<FunctionTemplate>(VHangUp)->GetFunction());
  exports->Set(Nan::New<String>("getsid").ToLocalChecked(),
    Nan::New<FunctionTemplate>(GetSID)->GetFunction());
  exports->Set(Nan::New<String>("setsid").ToLocalChecked(),
    Nan::New<FunctionTemplate>(SetSID)->GetFunction());
}

NODE_MODULE(binding, init)
