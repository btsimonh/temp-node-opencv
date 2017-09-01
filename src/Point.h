// Template class for 2D points

#include "OpenCV.h"

class Point: public Nan::ObjectWrap {
private:
  static Nan::Persistent<FunctionTemplate> constructor;

public:
  cv::Point point;

  static void Init(Local<Object> target);
  static NAN_METHOD(New);

  Point() : Nan::ObjectWrap() {};

  static Local<Object> NewInstance();
  static Local<Object> NewInstance(const cv::Point &point);
  static Local<Object> NewInstance(const int &x, const int &y);

  static bool HasInstance(Local<Value> object);

  static cv::Point RawPoint(const int &argc, const Local<Value> argv[]);

  static NAN_PROPERTY_GETTER(PropertyGetter);
  static NAN_PROPERTY_SETTER(PropertySetter);
  static NAN_PROPERTY_ENUMERATOR(PropertyEnumerator);

  static NAN_METHOD(Dot);
};

