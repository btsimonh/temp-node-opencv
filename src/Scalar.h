// Template class for 2D points

#include "OpenCV.h"

#define SCALAR_FROM_ARGS(NAME, IND) \
  if (info.Length() > IND && Scalar::HasInstance(info[IND])) { \
    NAME = UNWRAP_ARG(Scalar, IND); \
  }

class Scalar : public Nan::ObjectWrap {
private:
  static Nan::Persistent<FunctionTemplate> constructor;

public:
  cv::Scalar scalar;

  static void Init(Local<Object> target);
  static NAN_METHOD(New);

  Scalar() : Nan::ObjectWrap() {};

  static Local<Object> NewInstance();
  static Local<Object> NewInstance(const cv::Scalar &scalar);
  static Local<Object> NewInstance(const double &v0, const double &v1 = 0, const double &v2 = 0, const double &v3 = 0);

  static bool HasInstance(Local<Value> object);

  static cv::Scalar RawScalar(const int &argc, const Local<Value> argv[]);

  static NAN_INDEX_GETTER(IndexGetter);
  static NAN_INDEX_SETTER(IndexSetter);
  static NAN_INDEX_QUERY(IndexQuery);
  static NAN_INDEX_ENUMERATOR(IndexEnumerator);

  static NAN_METHOD(Mul);
  static NAN_METHOD(Conj);
  static NAN_METHOD(IsReal);
};
