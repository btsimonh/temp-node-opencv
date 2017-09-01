#include "Rect.h"
#include "Point.h"
#include "Size.h"
#include "OpenCV.h"

Nan::Persistent<FunctionTemplate> Rect::constructor;

void Rect::Init(Local<Object> target) {
  Nan::HandleScope scope;

  // Constructor
  Local<FunctionTemplate> ctor = Nan::New<FunctionTemplate>(Rect::New);
  constructor.Reset(ctor);
  
  ctor->SetClassName(Nan::New("Rect").ToLocalChecked());

  // Instance
  Local<ObjectTemplate> inst = ctor->InstanceTemplate();
  inst->SetInternalFieldCount(1);
  inst->SetHandler(NamedPropertyHandlerConfiguration(PropertyGetter, PropertySetter, 0, 0, PropertyEnumerator));

  // Prototype Methods
  Nan::SetPrototypeMethod(ctor, "tl", TopLeft);
  Nan::SetPrototypeMethod(ctor, "br", BottomRight);

  Nan::SetPrototypeMethod(ctor, "size", Size);
  Nan::SetPrototypeMethod(ctor, "area", Area);

  Nan::SetPrototypeMethod(ctor, "contains", Contains);

  Nan::SetPrototypeMethod(ctor, "toString", ToString);


  target->Set(Nan::New("Rect").ToLocalChecked(), ctor->GetFunction());
};

cv::Rect Rect::RawRect(const int &argc, const Local<Value> argv[]) {
  Local<Value> objKey_x = Nan::New<String>("x").ToLocalChecked();
  Local<Value> objKey_y = Nan::New<String>("y").ToLocalChecked();
  Local<Value> objKey_w = Nan::New<String>("width").ToLocalChecked();
  Local<Value> objKey_h = Nan::New<String>("height").ToLocalChecked();

  if (argc == 0) {
    return cv::Rect();
  } else if (argc == 1 && HasInstance(argv[0])) {
    Rect* r = Nan::ObjectWrap::Unwrap<Rect>(argv[0]->ToObject());
    return r->rect;
  } else if (argc == 1 && argv[0]->IsObject()) {
    Local<Object> rectLike = argv[0]->ToObject();

    if (!rectLike->Has(objKey_x) || !rectLike->Has(objKey_y) || !rectLike->Has(objKey_w) || !rectLike->Has(objKey_h)) {
      throw "Object must have properties \"x\", \"y\", \"width\" and \"height\"";
    }

    Local<Value> objValue_x = rectLike->Get(objKey_x);
    Local<Value> objValue_y = rectLike->Get(objKey_y);
    Local<Value> objValue_w = rectLike->Get(objKey_w);
    Local<Value> objValue_h = rectLike->Get(objKey_h);

    if (!objValue_x->IsNumber() || !objValue_y->IsNumber() || !objValue_w->IsNumber() || !objValue_h->IsNumber()) {
      throw "Properties \"x\", \"y\", \"width\" and \"height\" must be numbers";
    }

    return cv::Rect(objValue_x->Int32Value(), objValue_y->Int32Value(), objValue_w->Int32Value(), objValue_h->Int32Value());
  } else if (argc == 2 && argv[0]->IsObject() && argv[1]->IsObject()) {
    Local<Object> pointLike = argv[0]->ToObject();
    Local<Object> pointOrSizeLike = argv[1]->ToObject();

    bool isPoint;
    if (pointOrSizeLike->Has(objKey_x) && pointOrSizeLike->Has(objKey_y)) {
      isPoint = true;
    } else if (pointOrSizeLike->Has(objKey_w) && pointOrSizeLike->Has(objKey_h)) {
      isPoint = false;
    } else {
      // neither point or size
      throw "Argument 2 must be an object with properties \"x\" and \"y\" or \"width\" and \"height\"";
    }

    if (!pointLike->Has(objKey_x) || !pointLike->Has(objKey_y)) {
      // not a point
      throw "Argument 2 must be an object with properties \"x\" and \"y\"";
    }

    Local<Value> objValue_x = pointLike->Get(objKey_x);
    Local<Value> objValue_y = pointLike->Get(objKey_y);
    if (!objValue_x->IsNumber() || !objValue_y->IsNumber()) {
      throw "Properties \"x\" and \"y\" must be numbers";
    }

    if (isPoint) {
      Local<Value> objValue_x2 = pointOrSizeLike->Get(objKey_x);
      Local<Value> objValue_y2 = pointOrSizeLike->Get(objKey_y);
      if (!objValue_x2->IsNumber() || !objValue_y2->IsNumber()) {
        throw "Properties \"x\" and \"y\" must be numbers";
      }

      return cv::Rect(cv::Point(objValue_x->Int32Value(), objValue_y->Int32Value()), cv::Point(objValue_x2->Int32Value(), objValue_y2->Int32Value()));
    }

    Local<Value> objValue_w = pointOrSizeLike->Get(objKey_w);
    Local<Value> objValue_h = pointOrSizeLike->Get(objKey_h);
    if (!objValue_w->IsNumber() || !objValue_h->IsNumber()) {
      throw "Properties \"width\" and \"height\" must be numbers";
    }

    return cv::Rect(cv::Point(objValue_x->Int32Value(), objValue_y->Int32Value()), cv::Size(objValue_w->Int32Value(), objValue_h->Int32Value()));
  } else if (argc == 4) {
    if (!argv[0]->IsNumber() || !argv[1]->IsNumber() || !argv[2]->IsNumber() || !argv[3]->IsNumber()) {
      throw "Arguments 1, 2, 3 and 4 must be numbers";
    }

    return cv::Rect(argv[0]->Int32Value(), argv[1]->Int32Value(), argv[2]->Int32Value(), argv[3]->Int32Value());
  }

  throw "No matching call signature found";
}

NAN_METHOD(Rect::New) {
  Nan::HandleScope scope;

  if (!info.IsConstructCall()) {
    return Nan::ThrowTypeError("Cannot Instantiate without new");
  }

  cv::Rect rect;
  try {
    SETUP_ARGC_AND_ARGV()

    rect = RawRect(argc, argv);
  } catch (const char* msg) {
    return Nan::ThrowTypeError(msg);
  }

  Rect *r = new Rect();
  r->rect = rect;
  r->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

Local<Object> Rect::NewInstance() {
  return Nan::NewInstance(Nan::GetFunction(Nan::New(constructor)).ToLocalChecked()).ToLocalChecked();
}

Local<Object> Rect::NewInstance(const cv::Rect &rect) {
  Local<Object> obj = NewInstance();
  Rect* r = Nan::ObjectWrap::Unwrap<Rect>(obj);
  r->rect = rect;

  return obj;
}

Local<Object> Rect::NewInstance(const cv::Point &point, const cv::Size &size) {
  Local<Object> obj = NewInstance();
  Rect* r = Nan::ObjectWrap::Unwrap<Rect>(obj);
  r->rect = cv::Rect(point, size);

  return obj;
}

Local<Object> Rect::NewInstance(const cv::Point &point1, const cv::Point &point2) {
  Local<Object> obj = NewInstance();
  Rect* r = Nan::ObjectWrap::Unwrap<Rect>(obj);
  r->rect = cv::Rect(point1, point2);

  return obj;
}

Local<Object> Rect::NewInstance(const int &x, const int &y, const int &width, const int &height) {
  Local<Object> obj = NewInstance();
  Rect* r = Nan::ObjectWrap::Unwrap<Rect>(obj);
  r->rect = cv::Rect(x, y, width, height);

  return obj;
}

bool Rect::HasInstance(Local<Value> object) {
  return Nan::New(constructor)->HasInstance(object);
}

void Rect::PropertyGetter(Local<Name> property, const PropertyCallbackInfo<Value> &info) {
  if (!property->IsString()) {
    return;
  }

  Rect *self = Nan::ObjectWrap::Unwrap<Rect>(info.This());

  Nan::Utf8String name(property);
  if (strcmp(*name, "x") == 0) {
    info.GetReturnValue().Set(Nan::New<Number>(self->rect.x));
  } else if (strcmp(*name, "y") == 0) {
    info.GetReturnValue().Set(Nan::New<Number>(self->rect.y));
  } else if (strcmp(*name, "width") == 0) {
    info.GetReturnValue().Set(Nan::New<Number>(self->rect.width));
  } else if (strcmp(*name, "height") == 0) {
    info.GetReturnValue().Set(Nan::New<Number>(self->rect.height));
  }
}

void Rect::PropertySetter(Local<Name> property, Local<Value> value, const PropertyCallbackInfo<Value> &info) {
  if (!property->IsString()) {
    return;
  }

  Rect *self = Nan::ObjectWrap::Unwrap<Rect>(info.This());

  std::string name = *Nan::Utf8String(property);
  if (name == "x") {
    if (!value->IsNumber()) {
      return Nan::ThrowTypeError("value must be a number");
    }

    self->rect.x = value->Int32Value();
    info.GetReturnValue().Set(info.This());
  } else if (name == "y") {
    if (!value->IsNumber()) {
      return Nan::ThrowTypeError("value must be a number");
    }

    self->rect.y = value->Int32Value();
    info.GetReturnValue().Set(info.This());
  } else if (name == "width") {
    if (!value->IsNumber()) {
      return Nan::ThrowTypeError("value must be a number");
    }

    self->rect.width = value->Int32Value();
    info.GetReturnValue().Set(info.This());
  } else if (name == "height") {
    if (!value->IsNumber()) {
      return Nan::ThrowTypeError("value must be a number");
    }

    self->rect.height = value->Int32Value();
    info.GetReturnValue().Set(info.This());
  }
}

void Rect::PropertyEnumerator(const PropertyCallbackInfo<Array> &info) {
  v8::Local<v8::Array> arr = Nan::New<v8::Array>();
  Nan::Set(arr, 0, Nan::New("x").ToLocalChecked());
  Nan::Set(arr, 1, Nan::New("y").ToLocalChecked());
  Nan::Set(arr, 2, Nan::New("width").ToLocalChecked());
  Nan::Set(arr, 3, Nan::New("height").ToLocalChecked());
  info.GetReturnValue().Set(arr);
}

NAN_METHOD(Rect::TopLeft) {
  SETUP_FUNCTION(Rect)

  info.GetReturnValue().Set(Point::NewInstance(self->rect.x, self->rect.y));
}

NAN_METHOD(Rect::BottomRight) {
  SETUP_FUNCTION(Rect)

  info.GetReturnValue().Set(Point::NewInstance(self->rect.x + self->rect.width, self->rect.y + self->rect.height));
}

NAN_METHOD(Rect::Size) {
  SETUP_FUNCTION(Rect)

  info.GetReturnValue().Set(Size::NewInstance(self->rect.width, self->rect.height));
}

NAN_METHOD(Rect::Area) {
  SETUP_FUNCTION(Rect)

  info.GetReturnValue().Set(Nan::New<Number>(self->rect.area()));
}

NAN_METHOD(Rect::Contains) {
  SETUP_FUNCTION(Rect)

  if (info.Length() != 1 || !Point::HasInstance(info[0])) {
    return Nan::ThrowTypeError("Argument 1 must be a Point");
  }
  
  Point* pt = Nan::ObjectWrap::Unwrap<Point>(info[0]->ToObject());

  // TODO use a wrapped point object
  info.GetReturnValue().Set(Nan::New<Boolean>(self->rect.contains(pt->point)));
}

NAN_METHOD(Rect::ToString) {
  SETUP_FUNCTION(Rect)

  std::ostringstream stream;
  stream << self->rect;

  info.GetReturnValue().Set(Nan::New<String>(stream.str()).ToLocalChecked());
}
