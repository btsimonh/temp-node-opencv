// Template class for 2D points

#include "OpenCV.h"

class Rect : public Nan::ObjectWrap {
private:
  static Nan::Persistent<FunctionTemplate> constructor;

public:
  cv::Rect rect;

  static void Init(Local<Object> target);
  static NAN_METHOD(New);

  Rect() : Nan::ObjectWrap() {};

  static Local<Object> NewInstance();
  static Local<Object> NewInstance(const cv::Rect &rect);
  static Local<Object> NewInstance(const cv::Point &point, const cv::Size &size);
  static Local<Object> NewInstance(const cv::Point &point1, const cv::Point &point2);
  static Local<Object> NewInstance(const int &x, const int &y, const int &width, const int &height);

  static bool HasInstance(Local<Value> object);

  static cv::Rect RawRect(const int &argc, const Local<Value> argv[]);

  static void PropertyGetter(Local<Name> property, const PropertyCallbackInfo<Value> &info);
  static void PropertySetter(Local<Name> property, Local<Value> value, const PropertyCallbackInfo<Value> &info);
  static void PropertyEnumerator(const PropertyCallbackInfo<Array> &info);

  static NAN_METHOD(TopLeft);
  static NAN_METHOD(BottomRight);

  static NAN_METHOD(Size);
  static NAN_METHOD(Area);

  static NAN_METHOD(Contains);

  static NAN_METHOD(ToString);
};
