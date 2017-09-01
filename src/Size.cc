#include "Size.h"
#include "OpenCV.h"

Nan::Persistent<FunctionTemplate> Size::constructor;

void Size::Init(Local<Object> target) {
  Nan::HandleScope scope;

  // Constructor
  Local<FunctionTemplate> ctor = Nan::New<FunctionTemplate>(Size::New);
  constructor.Reset(ctor);

  ctor->SetClassName(Nan::New("Size").ToLocalChecked());

  // Instance
  Local<ObjectTemplate> inst = ctor->InstanceTemplate();
  inst->SetInternalFieldCount(1);
  inst->SetHandler(NamedPropertyHandlerConfiguration(PropertyGetter, PropertySetter, 0, 0, PropertyEnumerator));

  // Prototype Methods
  Nan::SetPrototypeMethod(ctor, "area", Area);

  Nan::SetPrototypeMethod(ctor, "toString", ToString);


  Nan::Set(target, Nan::New("Size").ToLocalChecked(), ctor->GetFunction());
};

cv::Size Size::RawSize(const int &argc, const Local<Value> argv[]) {
  Local<Value> objKey_w = Nan::New<String>("width").ToLocalChecked();
  Local<Value> objKey_h = Nan::New<String>("height").ToLocalChecked();

  if (argc == 0) {
    return cv::Size();
  } else if (argc == 1 && HasInstance(argv[0])) {
    Size *sz = Nan::ObjectWrap::Unwrap<Size>(argv[0]->ToObject());
    return sz->size;
  } else if (argc == 1 && argv[0]->IsObject()) {
    Local<Object> sizeLike = argv[0]->ToObject();

    if (!sizeLike->Has(objKey_w) || !sizeLike->Has(objKey_h)) {
      throw "Object must have properties \"width\" and \"height\"";
    }

    Local<Value> objValue_w = sizeLike->Get(objKey_w);
    Local<Value> objValue_h = sizeLike->Get(objKey_h);

    if (!objValue_w->IsNumber() || !objValue_h->IsNumber()) {
      throw "Properties \"width\" and \"height\" must be numbers";
    }

    return cv::Size(objValue_w->Int32Value(), objValue_h->Int32Value());
  } else if (argc == 2) {
    if (!argv[0]->IsNumber() || !argv[1]->IsNumber()) {
      throw "Arguments 1 and 2 must be numbers";
    }

    return cv::Size(argv[0]->Int32Value(), argv[1]->Int32Value());
  }

  throw "No matching call signature found";
}

NAN_METHOD(Size::New) {
  Nan::HandleScope scope;

  if (!info.IsConstructCall()) {
    return Nan::ThrowTypeError("Cannot instantiate without new");
  }

  cv::Size size;
  try {
    SETUP_ARGC_AND_ARGV()

    size = RawSize(argc, argv);
  } catch (const char* msg) {
    return Nan::ThrowTypeError(msg);
  }

  Size* sz = new Size();
  sz->size = size;
  sz->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

Local<Object> Size::NewInstance() {
  return Nan::NewInstance(Nan::GetFunction(Nan::New(constructor)).ToLocalChecked()).ToLocalChecked();
}

Local<Object> Size::NewInstance(const cv::Size &size) {
  Local<Object> obj = NewInstance();
  Size* sz = Nan::ObjectWrap::Unwrap<Size>(obj);
  sz->size = size;

  return obj;
}

Local<Object> Size::NewInstance(const int &width, const int &height) {
  Local<Object> obj = NewInstance();
  Size* sz = Nan::ObjectWrap::Unwrap<Size>(obj);
  sz->size.width = width;
  sz->size.height = height;

  return obj;
}

bool Size::HasInstance(Local<Value> object) {
  return Nan::New(constructor)->HasInstance(object);
}

void Size::PropertyGetter(Local<Name> property, const PropertyCallbackInfo<Value> &info) {
  if (!property->IsString()) {
    return;
  }

  Size *self = Nan::ObjectWrap::Unwrap<Size>(info.This());

  Nan::Utf8String name(property);
  if (strcmp(*name, "width") == 0) {
    info.GetReturnValue().Set(Nan::New<Number>(self->size.width));
  } else if (strcmp(*name, "height") == 0) {
    info.GetReturnValue().Set(Nan::New<Number>(self->size.height));
  }
}

void Size::PropertySetter(Local<Name> property, Local<Value> value, const PropertyCallbackInfo<Value> &info) {
  if (!property->IsString()) {
    return;
  }

  Size *self = Nan::ObjectWrap::Unwrap<Size>(info.This());

  Nan::Utf8String name(property);
  if (strcmp(*name, "width") == 0) {
    if (!value->IsNumber()) {
      return Nan::ThrowTypeError("value must be a number");
    }

    self->size.width = value->Int32Value();
    info.GetReturnValue().Set(info.This());
  } else if (strcmp(*name, "height") == 0) {
    if (!value->IsNumber()) {
      return Nan::ThrowTypeError("value must be a number");
    }

    self->size.height = value->Int32Value();
    info.GetReturnValue().Set(info.This());
  }
}

void Size::PropertyEnumerator(const PropertyCallbackInfo<Array> &info) {
  v8::Local<v8::Array> arr = Nan::New<v8::Array>();
  Nan::Set(arr, 0, Nan::New("width").ToLocalChecked());
  Nan::Set(arr, 1, Nan::New("height").ToLocalChecked());
  info.GetReturnValue().Set(arr);
}

NAN_METHOD(Size::Area) {
  SETUP_FUNCTION(Size)

  info.GetReturnValue().Set(Nan::New<Number>(self->size.area()));
}

NAN_METHOD(Size::ToString) {
  SETUP_FUNCTION(Size)

  std::ostringstream stream;
  stream << self->size;

  info.GetReturnValue().Set(Nan::New<String>(stream.str()).ToLocalChecked());
}
