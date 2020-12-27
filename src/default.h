/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
// by Rajib Chy
// 4:19 PM 12/24/2020

#if !defined(FALSE)
#	define FALSE 0
#endif// !FALSE
#if !defined(TRUE)
#	define TRUE 1
#endif// !TRUE
#if !defined(_free_obj)
#define _free_obj(obj)                  \
while(obj){                             \
    obj->clear();delete obj;obj = NULL; \
}
#endif//!_free_obj
#if !defined(_free_char)
#define _free_char(obj)                 \
while(obj){                             \
    delete[] obj; obj = NULL;           \
}
#endif//!_free_char
#if !defined(js_method_args)
#define js_method_args const v8::FunctionCallbackInfo<v8::Value>& args
#endif//!js_method_args
#if !defined(V8_JS_METHOD)
#define V8_JS_METHOD(name)\
void name(js_method_args)
#endif//!V8_JS_METHOD
template<class _swap_obj>
void swap_obj(_swap_obj& obj) {
	obj.clear();
	_swap_obj().swap(obj);
}
