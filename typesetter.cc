#include <napi.h>
#include <linux/uinput.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <set>
#include <iostream>

class Typesetter
{
public:

  Typesetter()
  {
    struct uinput_setup usetup;
    fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

    ioctl(fd, UI_SET_EVBIT, EV_KEY);
    int codes[] = { KEY_SPACE, KEY_LEFT, KEY_C };
    initKeys(codes);

    memset(&usetup, 0, sizeof(usetup));
    usetup.id.bustype = BUS_USB;
    usetup.id.vendor = 0x1234;
    usetup.id.product = 0x5678;
    strcpy(usetup.name, "Typesetter");

    ioctl(fd, UI_DEV_SETUP, &usetup);
    ioctl(fd, UI_DEV_CREATE);
    std::cout << "====  Typesetter  ====" << std::endl;
  }

  ~Typesetter() {
    ioctl(fd, UI_DEV_DESTROY);
    close(fd);
    std::cout << "====  Destroy  ====" << std::endl;
  }

  void type(const Napi::Int32Array& codes)
  {
    for (size_t i = 0; i < codes.ElementLength(); ++i)
    {
      emit(EV_KEY, codes[i], 1);
      emit(EV_SYN, SYN_REPORT, 0);
      emit(EV_KEY, codes[i], 0);
      emit(EV_SYN, SYN_REPORT, 0);
    }
  }

private:
  int fd;

  void initKeys(const int codes[])
  {
    std::set<int> set = {};
    for (size_t i = 0; i < 4; ++i) {
      if (set.find(codes[i]) == set.end()) {
        set.insert(codes[i]);
        ioctl(fd, UI_SET_KEYBIT, codes[i]);
      }
    }
  }

  void emit(int type, int code, int val)
  {
    struct input_event ie;
    ie.type = type;
    ie.code = code;
    ie.value = val;
    ie.time.tv_sec = 0;
    ie.time.tv_usec = 0;
    write(fd, &ie, sizeof(ie));
  }
};

Typesetter typesetter;

void Type(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return;
  }

  Napi::Int32Array keyCodes = info[0].As<Napi::Int32Array>();
  typesetter.type(keyCodes);
}

void InitTypesetter(const Napi::CallbackInfo& info) {}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "type"), Napi::Function::New(env, Type));
  exports.Set(Napi::String::New(env, "init"), Napi::Function::New(env, InitTypesetter));
  return exports;
}

NODE_API_MODULE(addon, Init)
