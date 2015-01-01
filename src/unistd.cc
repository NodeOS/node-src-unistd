#include <signal.h>
#include <sys/ioctl.h>
#include <v8.h>
#include <node.h>
#include <unistd.h>
#include <nan.h>

using namespace v8;
using namespace node;

NAN_METHOD(GetForegroundProcessGroup) {
  NanScope();
  int fd = Handle<Integer>::Cast(args[0])->Value();
  pid_t res = tcgetpgrp(fd);
  NanReturnValue(NanNew<Integer>(res));
}

NAN_METHOD(SetForegroundProcessGroup) {
  NanScope();
  int fd = Handle<Integer>::Cast(args[0])->Value();
  pid_t pid = Handle<Integer>::Cast(args[1])->Value();
  int res = tcsetpgrp(fd,pid);
  NanReturnValue(NanNew<Integer>(res));
}

NAN_METHOD(IgnoreSig) {
  NanScope();
  sigset_t set;
  sigemptyset(&set);
  sigaddset(&set, SIGTTOU);
  // sigaddset(&set, SIGTTIN);
  sigaddset(&set, SIGTSTP);
  // sigaddset(&set, SIGCHLD);
  sigprocmask(SIG_BLOCK, &set, NULL);
  NanReturnUndefined();
}

NAN_METHOD(SetControllingTTY) {
  NanScope();
  int fd = args[0]->Int32Value();
  int rt = ioctl (fd, TIOCSCTTY, NULL);
  NanReturnValue(NanNew<Integer>(rt));
}

NAN_METHOD(VHangUp) {
  NanScope();
  int ret = vhangup();
  NanReturnValue(NanNew<Integer>(ret));
}

NAN_METHOD(SetGID) {
  NanScope();
  pid_t pid = Handle<Integer>::Cast(args[0])->Value();
  pid_t pgid = Handle<Integer>::Cast(args[1])->Value();
  int res = setpgid(pid,pgid);
  NanReturnValue(NanNew<Integer>(res));
}

NAN_METHOD(GetGID) {
  NanScope();
  pid_t pid = Handle<Integer>::Cast(args[0])->Value();
  int res = getpgid(pid);
  NanReturnValue(NanNew<Integer>(res));
}

NAN_METHOD(SetSID) {
  NanScope();
  int sid = setsid();
  NanReturnValue(NanNew<Integer>(sid));
}

NAN_METHOD(GetSID) {
  NanScope();
  pid_t pid = Handle<Integer>::Cast(args[0])->Value();
  int sid = getsid(pid);
  NanReturnValue(NanNew<Integer>(sid));
}

NAN_METHOD(Fork) {
  NanScope();
  int pid = fork();
  NanReturnValue(NanNew<Integer>(pid));
}

NAN_METHOD(Dup) {
  NanScope();
  int fd = args[0]->Int32Value();
  int dd = dup(fd);
  NanReturnValue(NanNew<Integer>(dd));
}

void init(Handle<Object> exports) {
  exports->Set(NanNew<String>("ignore"),
    NanNew<FunctionTemplate>(IgnoreSig)->GetFunction());
  exports->Set(NanNew<String>("getpgid"),
    NanNew<FunctionTemplate>(GetGID)->GetFunction());
  exports->Set(NanNew<String>("setpgid"),
    NanNew<FunctionTemplate>(SetGID)->GetFunction());
  exports->Set(NanNew<String>("tcgetpgrp"),
    NanNew<FunctionTemplate>(GetForegroundProcessGroup)->GetFunction());
  exports->Set(NanNew<String>("tcsetpgrp"),
    NanNew<FunctionTemplate>(SetForegroundProcessGroup)->GetFunction());
  exports->Set(NanNew<String>("dup"),
    NanNew<FunctionTemplate>(Dup)->GetFunction());
  exports->Set(NanNew<String>("fork"),
    NanNew<FunctionTemplate>(Fork)->GetFunction());
  exports->Set(NanNew<String>("setControllingTTY"),
    NanNew<FunctionTemplate>(SetControllingTTY)->GetFunction());
  exports->Set(NanNew<String>("vhangup"),
    NanNew<FunctionTemplate>(VHangUp)->GetFunction());
  exports->Set(NanNew<String>("getsid"),
    NanNew<FunctionTemplate>(GetSID)->GetFunction());
  exports->Set(NanNew<String>("setsid"),
    NanNew<FunctionTemplate>(SetSID)->GetFunction());
}

NODE_MODULE(binding, init)
