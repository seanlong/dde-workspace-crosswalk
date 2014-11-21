#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#ifdef __DUI_DEBUG
#define PRIVATE 
#else
#define PRIVATE static
#endif

#define JS_EXPORT_API
#define DBUS_EXPORT_API

#include <stdint.h>

typedef struct _ArrayContainer {
  void* data;
  size_t num;
} ArrayContainer;

//FIXME(xiang): remove this to another header, and add associated callback id support.
typedef int32_t XW_Instance;
typedef struct {
   XW_Instance xw_instance;
} JSCallback;

#endif
