#ifndef SIGNAL_H 
#define SIGNAL_H 

#include "json-c/json.h"

void js_post_message(const char* name, json_object* json);

void js_post_signal(const char* signal);

#endif /* end of include guard: SIGNAL_H */

