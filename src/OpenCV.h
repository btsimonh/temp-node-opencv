#ifndef __NODE_OPENCV_H__
#define __NODE_OPENCV_H__

#ifdef WIN
    /*
        This is needed on Windows for Visual Studio to not throw an error in the
        build/include/opencv2/flann/any.h file in OpenCV.
    */
    namespace std{ typedef type_info type_info; }
#endif



#include <v8.h>
#include <node.h>
#include <node_object_wrap.h>
#include <node_version.h>
#include <node_buffer.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#if CV_MAJOR_VERSION >= 3
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/opencv_modules.hpp>
#endif
#if ((CV_MAJOR_VERSION == 2) && (CV_MINOR_VERSION >=4) && (CV_SUBMINOR_VERSION>=4))
#define HAVE_OPENCV_FACE
#endif

#include <string.h>
#include <nan.h>

using namespace v8;
using namespace node;

#define UNWRAP_OBJ(TYP, OBJ) Nan::ObjectWrap::Unwrap<TYP>(OBJ)

#define UNWRAP_ARG(TYP, IND) Nan::ObjectWrap::Unwrap<TYP>(info[IND]->ToObject())

#define REQ_FUN_ARG(I, VAR)                                             \
  if (info.Length() <= (I) || !info[I]->IsFunction())                   \
    return Nan::ThrowTypeError("Argument " #I " must be a function");  \
  Local<Function> VAR = Local<Function>::Cast(info[I]);

#define SETUP_FUNCTION(TYP)	\
	Nan::HandleScope scope;		\
	TYP *self = UNWRAP_OBJ(TYP, info.This());

#define JSFUNC(NAME) \
  static NAN_METHOD(NAME);

#define JSTHROW_TYPE(ERR) \
  Nan::ThrowTypeError( ERR );

#define JSTHROW(ERR) \
  Nan::ThrowError( ERR );

#define INT_FROM_ARGS(NAME, IND) \
  if (info.Length() > IND && info[IND]->IsNumber()) { \
    NAME = info[IND]->Int32Value(); \
  }

#define DOUBLE_FROM_ARGS(NAME, IND) \
  if (info.Length() > IND && info[IND]->IsNumber()){ \
    NAME = info[IND]->NumberValue(); \
  }

#define SETUP_ARGC_AND_ARGV() \
  int argc = info.Length(); \
  Local<Value> *argv = new Local<Value>[argc](); \
  for (int n = 0; n < argc; ++n) argv[n] = info[n];

class OpenCV: public Nan::ObjectWrap {
public:
  static void Init(Local<Object> target);

  static NAN_METHOD(ReadImage);
  static NAN_METHOD(ReadImageMulti);
};

#endif
