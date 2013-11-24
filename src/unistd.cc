#include <signal.h>
#include <sys/ioctl.h>
#include <v8.h>
#include <node.h>
#include <unistd.h>

using namespace v8;
using namespace node;

namespace NodeOS
{

  static Handle<Value> GetForegroundProcessGroup(const Arguments& args){
    HandleScope scope;
    int fd = Handle<Integer>::Cast(args[0])->Value();
    pid_t res = tcgetpgrp(fd);
    return scope.Close(Integer::New(res));
  }
  
  static Handle<Value> SetForegroundProcessGroup(const Arguments& args){
    HandleScope scope;
    int fd = Handle<Integer>::Cast(args[0])->Value();
    pid_t pid = Handle<Integer>::Cast(args[1])->Value();
    int res = tcsetpgrp(fd,pid);
    return scope.Close(Integer::New(res));
  }
  
  static Handle<Value> IgnoreSig(const Arguments& args) {
    HandleScope scope;
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGTTOU);
    // sigaddset(&set, SIGTTIN);
    sigaddset(&set, SIGTSTP);
    // sigaddset(&set, SIGCHLD);
    sigprocmask(SIG_BLOCK, &set, NULL);
    return scope.Close(Undefined());
  }
    
  static Handle<Value> SetControllingTTY(const Arguments& args) {
    HandleScope scope;
    int fd = args[0]->Int32Value();
    int rt = ioctl (fd, TIOCSCTTY, NULL);
    return scope.Close(Integer::New(rt));
  }

  static Handle<Value> VHangUp(const Arguments& args) {
    HandleScope scope;
    int ret = vhangup();
    return scope.Close(Integer::New(ret));
  }
  
  static Handle<Value> SetGID(const Arguments& args) {
    HandleScope scope;
    pid_t pid = Handle<Integer>::Cast(args[0])->Value();
    pid_t pgid = Handle<Integer>::Cast(args[1])->Value(); 
    int res = setpgid(pid,pgid);
    return scope.Close(Integer::New(res));
  }

  static Handle<Value> GetGID(const Arguments& args) {
    HandleScope scope;
    pid_t pid = Handle<Integer>::Cast(args[0])->Value();
    int res = getpgid(pid);
    return scope.Close(Integer::New(res));
  }
  
  static Handle<Value> SetSID(const Arguments& args) {
    HandleScope scope;
    int sid = setsid();
    return scope.Close(Integer::New(sid));
  }
  
  static Handle<Value> GetSID(const Arguments& args) {
    HandleScope scope;
    pid_t pid = Handle<Integer>::Cast(args[0])->Value();
    int sid = getsid(pid);
    return scope.Close(Integer::New(sid));
  }
  
  static Handle<Value> Fork(const Arguments& args) {
    HandleScope scope;
    int pid = fork();
    return scope.Close(Integer::New(pid));
  }
  
  static Handle<Value> Dup(const Arguments& args) {
    HandleScope scope;
    int fd = args[0]->Int32Value();
    int dd = dup(fd);
    return scope.Close(Integer::New(dd));
  }
  
  static void init(Handle<Object> target) {
    target->Set(String::NewSymbol("ignore"),
      FunctionTemplate::New(IgnoreSig)->GetFunction());
    target->Set(String::NewSymbol("getpgid"),
      FunctionTemplate::New(GetGID)->GetFunction());
    target->Set(String::NewSymbol("setpgid"),
      FunctionTemplate::New(SetGID)->GetFunction());
    target->Set(String::NewSymbol("tcgetpgrp"),
      FunctionTemplate::New(GetForegroundProcessGroup)->GetFunction());
    target->Set(String::NewSymbol("tcsetpgrp"),
      FunctionTemplate::New(SetForegroundProcessGroup)->GetFunction());
    target->Set(String::NewSymbol("dup"),
      FunctionTemplate::New(Dup)->GetFunction());
    target->Set(String::NewSymbol("fork"),
      FunctionTemplate::New(Fork)->GetFunction());
    target->Set(String::NewSymbol("setControllingTTY"),
      FunctionTemplate::New(SetControllingTTY)->GetFunction());
    target->Set(String::NewSymbol("vhangup"),
      FunctionTemplate::New(VHangUp)->GetFunction());
    target->Set(String::NewSymbol("getsid"),
      FunctionTemplate::New(GetSID)->GetFunction());
    target->Set(String::NewSymbol("setsid"),
      FunctionTemplate::New(SetSID)->GetFunction());
  };

}

NODE_MODULE(binding, NodeOS::init)
