/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
//2:05 PM 2/9/2020
#if !defined(_wj_pdf_h)
#	define _wj_pdf_h
//#undef V8_WARN_UNUSED_RESULT
#	include <v8.h>
void generate_pdf(const v8::FunctionCallbackInfo<v8::Value>& args);
void get_http_header(const v8::FunctionCallbackInfo<v8::Value>& args);
void destroy_app(const v8::FunctionCallbackInfo<v8::Value>& args);
#endif//!_image_win_h