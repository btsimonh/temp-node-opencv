#include "OpenCV.h"
#include "Matrix.h"
#include <nan.h>

void OpenCV::Init(Local<Object> target) {
  Nan::HandleScope scope;

  // Version string.
  target->Set(Nan::New<String>("version").ToLocalChecked(), Nan::New<String>(CV_VERSION).ToLocalChecked());

  Nan::SetMethod(target, "readImage", ReadImage);
  Nan::SetMethod(target, "readImageMulti", ReadImageMulti);
}

class ReadImageAsyncWorker : public Nan::AsyncWorker {
public:
  ReadImageAsyncWorker(const std::string &path): Nan::AsyncWorker{nullptr}, path(path) {}
  ReadImageAsyncWorker(const unsigned &length, uint8_t *data): Nan::AsyncWorker{nullptr}, length(length), data(data) {}

  void SetImreadMode(cv::ImreadModes mode) {
    this->mode = mode;
  }

  void Execute() override {
    try {
      if (data == nullptr) {
        mat = cv::imread(path, mode);
      } else {
        cv::Mat mbuf(length, 1, CV_64FC1, data);
        mat = cv::imdecode(mbuf, mode);
      }
    } catch (cv::Exception& e) {
      return SetErrorMessage(e.what());
    }

    if (mat.empty()) {
      SetErrorMessage("Could not open or find the image");
    }
  }

protected:
  void HandleOKCallback() override {
    Nan::HandleScope scope;

    Local<Object> m = Matrix::NewInstance();
    UNWRAP_OBJ(Matrix, m)->mat = this->mat;

    OnSuccess(m);
  }

  void HandleErrorCallback() override {
    Nan::HandleScope scope;

    OnFailure(Nan::Error(ErrorMessage()));
  }

protected:
  virtual void OnSuccess(Local<Value> value) = 0;
  virtual void OnFailure(Local<Value> error) = 0;

private:
    const std::string path = nullptr;

    unsigned length = 0;
    uint8_t *data = nullptr;

    cv::ImreadModes mode = cv::IMREAD_COLOR;

    cv::Mat mat;
};

class CallbackReadImageAsyncWorker : public ReadImageAsyncWorker {
public:
  CallbackReadImageAsyncWorker(const std::string &path): ReadImageAsyncWorker(path) {}
  CallbackReadImageAsyncWorker(const unsigned &length, uint8_t *data): ReadImageAsyncWorker(length, data) {}

protected:
  void OnSuccess(Local<Value> value) override {
	Nan::HandleScope scope;

    Local<Value> argv[2] = {Nan::Undefined(), value};
    ExecuteCallback(2, argv);
  }

  void OnFailure(Local<Value> error) override {
	Nan::HandleScope scope;

    Local<Value> argv[1] = {error};
    ExecuteCallback(1, argv);
  }
private:
  void ExecuteCallback(const int &argc, Local<Value> argv[]) {
	Nan::HandleScope scope;
    
  	Local<Function> callback = Local<Function>::Cast(GetFromPersistent(0u));

    Nan::TryCatch try_catch;
    callback->Call(Nan::GetCurrentContext()->Global(), argc, argv);
    if (try_catch.HasCaught()) {
      Nan::FatalException(try_catch);
    }
  }
};

class PromiseReadImageAsyncWorker : public ReadImageAsyncWorker {
public:
  PromiseReadImageAsyncWorker(const std::string &path): ReadImageAsyncWorker(path) {}
  PromiseReadImageAsyncWorker(const unsigned &length, uint8_t *data): ReadImageAsyncWorker(length, data) {}

protected:
  void OnSuccess(Local<Value> value) override {
    GetPromise()->Resolve(Nan::GetCurrentContext(), value);
    Isolate::GetCurrent()->RunMicrotasks();
  }

  void OnFailure(Local<Value> error) override {
    GetPromise()->Reject(Nan::GetCurrentContext(), error);
    Isolate::GetCurrent()->RunMicrotasks();
  }
private:
  Local<Promise::Resolver> GetPromise() {
    return Local<Promise::Resolver>::Cast(GetFromPersistent(0u));
  }
};

NAN_METHOD(OpenCV::ReadImage) {
  Nan::EscapableHandleScope scope;

  if (info.Length() < 1) {
    return Nan::ThrowError("readImage requires at least 1 arguments");
  }

  bool isCallback = false;
  if (info.Length() > 1) {
    if (!info[1]->IsFunction()) {
      return Nan::ThrowTypeError("Argument 2 must be a Function");
    }

    isCallback = true;
  }

  ReadImageAsyncWorker *worker = nullptr;
  if (info[0]->IsString()) {
    std::string path = std::string(*Nan::Utf8String(info[0]->ToString()));
    if (isCallback) {
      worker = new CallbackReadImageAsyncWorker(path);
    } else {
      worker = new PromiseReadImageAsyncWorker(path);
    }
  } else if (Buffer::HasInstance(info[0])) {
    unsigned len = Buffer::Length(info[0]->ToObject());
    uint8_t *data = (uint8_t *)Buffer::Data(info[0]->ToObject());

    if (isCallback) {
      worker = new CallbackReadImageAsyncWorker(len, data);
    } else {
      worker = new PromiseReadImageAsyncWorker(len, data);
    }
  } else {
    return Nan::ThrowTypeError("Argument 1 must be a string or a Buffer");
  }

  if (isCallback) {
    worker->SaveToPersistent(0u, info[1]);
  } else {
    Local<Promise::Resolver> resolver = Promise::Resolver::New(Nan::GetCurrentContext()).ToLocalChecked();
    worker->SaveToPersistent(0u, resolver);
    info.GetReturnValue().Set(resolver->GetPromise());
  }

  Nan::AsyncQueueWorker(worker);
}

#if CV_MAJOR_VERSION >= 3
NAN_METHOD(OpenCV::ReadImageMulti) {
  Nan::EscapableHandleScope scope;

  REQ_FUN_ARG(1, cb);

  Local<Value> argv[2];
  argv[0] = Nan::Null();

  std::vector<cv::Mat> mats;
  try {
    if (info[0]->IsString()) {
      std::string filename = std::string(*Nan::Utf8String(info[0]->ToString()));
      cv::imreadmulti(filename, mats);

      if (mats.empty()) {
        argv[0] = Nan::Error("Error loading file");
      }
    }
  } catch (cv::Exception& e) {
    argv[0] = Nan::Error(e.what());
    argv[1] = Nan::Null();
  }

  Local <Array> output = Nan::New<Array>(mats.size());
  argv[1] = output;

  for (std::vector<cv::Mat>::size_type i = 0; i < mats.size(); i ++) {
    Local<Object> im_h = Nan::NewInstance(Nan::GetFunction(Nan::New(Matrix::constructor)).ToLocalChecked()).ToLocalChecked();
    Matrix *img = Nan::ObjectWrap::Unwrap<Matrix>(im_h);
    img->mat = mats[i];

    output->Set(i, im_h);
  }

  Nan::TryCatch try_catch;
  cb->Call(Nan::GetCurrentContext()->Global(), 2, argv);

  if (try_catch.HasCaught()) {
    Nan::FatalException(try_catch);
  }

  return;
}
#else
NAN_METHOD(OpenCV::ReadImageMulti) {
  info.GetReturnValue().Set(Nan::New<Boolean>(false));
  return;
}
#endif
