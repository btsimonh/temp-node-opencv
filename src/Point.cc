#include "Point.h"
#include "OpenCV.h"

Nan::Persistent<FunctionTemplate> Point::constructor;

void Point::Init(Local<Object> target) {
  Nan::HandleScope scope;

  // Constructor
  Local<FunctionTemplate> ctor = Nan::New<FunctionTemplate>(Point::New);
  constructor.Reset(ctor);

  ctor->SetClassName(Nan::New("Point").ToLocalChecked());

  // Instance
  Local<ObjectTemplate> inst = ctor->InstanceTemplate();
  inst->SetInternalFieldCount(1);
  Nan::SetNamedPropertyHandler(inst, PropertyGetter, PropertySetter, 0, 0, PropertyEnumerator);

  // Prototype Methods
  Nan::SetPrototypeMethod(ctor, "dot", Dot);


  target->Set(Nan::New("Point").ToLocalChecked(), ctor->GetFunction());
};

cv::Point Point::RawPoint(const int &argc, const Local<Value> argv[]) {
  Local<Value> objKey_x = Nan::New<String>("x").ToLocalChecked();
  Local<Value> objKey_y = Nan::New<String>("y").ToLocalChecked();

  if (argc == 0) {
    return cv::Point();
  } else if (argc == 1 && HasInstance(argv[0])) {
    Point *pt = Nan::ObjectWrap::Unwrap<Point>(argv[0]->ToObject());
    return pt->point;
  } else if (argc == 1 && argv[0]->IsObject()) {
    Local<Object> pointLike = argv[0]->ToObject();

    if (!pointLike->Has(objKey_x) || !pointLike->Has(objKey_y)) {
      throw "Object must have properties \"x\" and \"y\"";
    }

    Local<Value> objValue_x = pointLike->Get(objKey_x);
    Local<Value> objValue_y = pointLike->Get(objKey_y);

    if (!objValue_x->IsNumber() || !objValue_y->IsNumber()) {
      throw "Properties \"x\" and \"y\" must be numbers";
    }

    return cv::Point(objValue_x->Int32Value(), objValue_y->Int32Value());
  } else if (argc == 2) {
    if (!argv[0]->IsNumber() || !argv[1]->IsNumber()) {
      throw "Arguments 1 and 2 must be numbers";
    }

    return cv::Point(argv[0]->Int32Value(), argv[1]->Int32Value());
  }

  throw "No matching call signature found";
}

NAN_METHOD(Point::New) {
  Nan::HandleScope scope;

  if (!info.IsConstructCall()) {
    return Nan::ThrowTypeError("Cannot instantiate without new");
  }

  cv::Point point;
  try {
    SETUP_ARGC_AND_ARGV()

    point = RawPoint(argc, argv);
  } catch (const char* msg) {
    return Nan::ThrowTypeError(msg);
  }

  Point* pt = new Point();
  pt->point = point;
  pt->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

Local<Object> Point::NewInstance() {
  return Nan::NewInstance(Nan::GetFunction(Nan::New(constructor)).ToLocalChecked()).ToLocalChecked();
}

Local<Object> Point::NewInstance(const cv::Point &point) {
  Local<Object> obj = NewInstance();
  Point* pt = Nan::ObjectWrap::Unwrap<Point>(obj);
  pt->point = point;

  return obj;
}

Local<Object> Point::NewInstance(const int &x, const int &y) {
  Local<Object> obj = NewInstance();
  Point* pt = Nan::ObjectWrap::Unwrap<Point>(obj);
  pt->point.x = x;
  pt->point.y = y;

  return obj;
}

bool Point::HasInstance(Local<Value> object) {
  return Nan::New(constructor)->HasInstance(object);
}

NAN_PROPERTY_GETTER(Point::PropertyGetter) {
  Point *self = Nan::ObjectWrap::Unwrap<Point>(info.This());

  Nan::Utf8String name(property);
  if (strcmp(*name, "x") == 0) {
    info.GetReturnValue().Set(Nan::New<Number>(self->point.x));
  } else if (strcmp(*name, "y") == 0) {
    info.GetReturnValue().Set(Nan::New<Number>(self->point.y));
  }
}

NAN_PROPERTY_SETTER(Point::PropertySetter) {
  Point *self = Nan::ObjectWrap::Unwrap<Point>(info.This());

  Nan::Utf8String name(property);
  if (strcmp(*name, "x") == 0) {
    if (!value->IsNumber()) {
      return Nan::ThrowTypeError("value must be a number");
    }

    self->point.x = value->Int32Value();
    info.GetReturnValue().Set(info.This());
  } else if (strcmp(*name, "y") == 0) {
    if (!value->IsNumber()) {
      return Nan::ThrowTypeError("value must be a number");
    }

    self->point.y = value->Int32Value();
    info.GetReturnValue().Set(info.This());
  }
}

NAN_PROPERTY_ENUMERATOR(Point::PropertyEnumerator) {
  v8::Local<v8::Array> arr = Nan::New<v8::Array>();
  Nan::Set(arr, 0, Nan::New("x").ToLocalChecked());
  Nan::Set(arr, 1, Nan::New("y").ToLocalChecked());
  info.GetReturnValue().Set(arr);
}

NAN_METHOD(Point::Dot) {
  Nan::HandleScope scope;
  Point *p1 = Nan::ObjectWrap::Unwrap<Point>(info.This());
  Point *p2 = Nan::ObjectWrap::Unwrap<Point>(info[0]->ToObject());

  info.GetReturnValue().Set(Nan::New<Number>(p1->point.dot(p2->point)));
}
