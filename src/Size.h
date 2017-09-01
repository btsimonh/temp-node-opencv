// Template class for 2D points

#include "OpenCV.h"

class Size : public Nan::ObjectWrap {
private:
  static Nan::Persistent<FunctionTemplate> constructor;

public:
  cv::Size size;

  static void Init(Local<Object> target);
  static NAN_METHOD(New);

  Size() : Nan::ObjectWrap() {};

  static Local<Object> NewInstance();
  static Local<Object> NewInstance(const cv::Size &size);
  static Local<Object> NewInstance(const int &width, const int &height);

  static bool HasInstance(Local<Value> object);

  static cv::Size RawSize(const int &argc, const Local<Value> argv[]);

  static void PropertyGetter(Local<Name> property, const PropertyCallbackInfo<Value> &info);
  static void PropertySetter(Local<Name> property, Local<Value> value, const PropertyCallbackInfo<Value> &info);
  static void PropertyEnumerator(const PropertyCallbackInfo<Array> &info);

  static NAN_METHOD(Area);

  static NAN_METHOD(ToString);
};
