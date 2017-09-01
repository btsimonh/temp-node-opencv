#include "Scalar.h"
#include "OpenCV.h"

Nan::Persistent<FunctionTemplate> Scalar::constructor;

void Scalar::Init(Local<Object> target) {
  Nan::HandleScope scope;

  // Constructor
  Local<FunctionTemplate> ctor = Nan::New<FunctionTemplate>(Scalar::New);
  constructor.Reset(ctor);

  ctor->SetClassName(Nan::New("Scalar").ToLocalChecked());

  // Instance
  Local<ObjectTemplate> inst = ctor->InstanceTemplate();
  inst->SetInternalFieldCount(1);
  Nan::SetIndexedPropertyHandler(inst, IndexGetter, IndexSetter, IndexQuery, 0, IndexEnumerator);

  // Prototype Methods
  Nan::SetPrototypeMethod(ctor, "mul", Mul);
  Nan::SetPrototypeMethod(ctor, "conj", Conj);
  Nan::SetPrototypeMethod(ctor, "isReal", IsReal);


  Nan::Set(target, Nan::New("Scalar").ToLocalChecked(), ctor->GetFunction());
};

cv::Scalar Scalar::RawScalar(const int &argc, const Local<Value> argv[]) {
  if (argc == 0) {
    return cv::Scalar();
  } else if (argc == 1 && HasInstance(argv[0])) {
    Scalar *s = Nan::ObjectWrap::Unwrap<Scalar>(argv[0]->ToObject());
    return s->scalar;
  } else if (argc == 1 && argv[0]->IsArray()) {
    Local<Array> scalarLike = Local<Array>::Cast(argv[0]->ToObject());
    if (scalarLike->Length() != 4) {
      throw "Array must have 4 values";
    }

    Local<Value> objValue_v0 = scalarLike->Get(0);
    Local<Value> objValue_v1 = scalarLike->Get(1);
    Local<Value> objValue_v2 = scalarLike->Get(2);
    Local<Value> objValue_v3 = scalarLike->Get(3);
    if (!objValue_v0->IsNumber() ||!objValue_v1->IsNumber() ||!objValue_v2->IsNumber() ||!objValue_v3->IsNumber()) {
      throw "Array values must be numbers";
    }

    return cv::Scalar(objValue_v0->NumberValue(), objValue_v1->NumberValue(), objValue_v2->NumberValue(), objValue_v3->NumberValue());
  } else if (argc == 1 && !argv[0]->IsNumber()) {
    throw "Argument 1 must be either a Scalar or array";
  } else if (argc <= 4) {
    for (int i = 0; i < argc; i++) {
      if (!argv[i]->IsNumber()) {
        throw cv::format("Argument %d must be a number", i).c_str();
      }
    }

    return cv::Scalar(argv[0]->NumberValue(), argc > 1 ? argv[1]->NumberValue() : 0, argc > 2 ? argv[2]->NumberValue() : 0, argc > 3 ? argv[3]->NumberValue() : 0);
  }

  throw "No matching call signature found";
}

NAN_METHOD(Scalar::New) {
  Nan::HandleScope scope;

  if (!info.IsConstructCall()) {
    return Nan::ThrowTypeError("Cannot instantiate without new");
  }

  cv::Scalar scalar;
  try {
    SETUP_ARGC_AND_ARGV()

    scalar = RawScalar(argc, argv);
  } catch (const char* msg) {
    return Nan::ThrowTypeError(msg);
  }

  Scalar* s = new Scalar();
  s->scalar = scalar;
  s->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

Local<Object> Scalar::NewInstance() {
  return Nan::NewInstance(Nan::GetFunction(Nan::New(constructor)).ToLocalChecked()).ToLocalChecked();
}

Local<Object> Scalar::NewInstance(const cv::Scalar &scalar) {
  Local<Object> obj = NewInstance();
  Scalar* s = Nan::ObjectWrap::Unwrap<Scalar>(obj);
  s->scalar = scalar;

  return obj;
}

Local<Object> Scalar::NewInstance(const double &v0, const double &v1, const double &v2, const double &v3) {
  Local<Object> obj = NewInstance();
  Scalar* s = Nan::ObjectWrap::Unwrap<Scalar>(obj);
  s->scalar[0] = v0;
  s->scalar[1] = v1;
  s->scalar[2] = v2;
  s->scalar[3] = v3;

  return obj;
}

bool Scalar::HasInstance(Local<Value> object) {
  return Nan::New(constructor)->HasInstance(object);
}

NAN_INDEX_GETTER(Scalar::IndexGetter) {
  Scalar *self = Nan::ObjectWrap::Unwrap<Scalar>(info.This());
  if (index < 4) {
    info.GetReturnValue().Set(Nan::New<Number>(self->scalar[index]));
  }
}

NAN_INDEX_SETTER(Scalar::IndexSetter) {
  Scalar *self = Nan::ObjectWrap::Unwrap<Scalar>(info.This());
  if (index < 4) {
    if (!value->IsNumber()) {
      return Nan::ThrowTypeError("value must be a number");
    }

    self->scalar[index] = value->NumberValue();
    info.GetReturnValue().Set(info.This());
  }
}

NAN_INDEX_QUERY(Scalar::IndexQuery) {
  if (index < 4) {
    info.GetReturnValue().Set(Nan::New<Integer>(DontDelete));
  }
}

NAN_INDEX_ENUMERATOR(Scalar::IndexEnumerator) {
  v8::Local<v8::Array> arr = Nan::New<v8::Array>();
  for (int i = 0; i < 4; i++) {
    Nan::Set(arr, i, Nan::New(std::to_string(i)).ToLocalChecked());
  }
  info.GetReturnValue().Set(arr);
}

NAN_METHOD(Scalar::Mul) {
  SETUP_FUNCTION(Scalar)

  if (info.Length() == 0) {
    return Nan::ThrowError("Scalar.mul requires at least 1 argument");
  }

  cv::Scalar scalar;
  try {
    SETUP_ARGC_AND_ARGV()

    scalar = RawScalar(argc, argv);
  } catch (const char* msg) {
    return Nan::ThrowTypeError(msg);
  }

  double scale = 1;
  DOUBLE_FROM_ARGS(scale, 1)

  info.GetReturnValue().Set(NewInstance(self->scalar.mul(scalar, scale)));
}

NAN_METHOD(Scalar::Conj) {
  SETUP_FUNCTION(Scalar)

  info.GetReturnValue().Set(NewInstance(self->scalar.conj()));
}

NAN_METHOD(Scalar::IsReal) {
  SETUP_FUNCTION(Scalar)

  info.GetReturnValue().Set(Nan::New<Boolean>(self->scalar.isReal()));
}
