/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
#if defined(_MSC_VER)
#pragma once
#endif//!_MSC_VER
#if !defined(_v8_util_h)
#	define _v8_util_h
#	include <node.h>
#if !defined(v8_str)
#define v8_str(isolate, x)\
	v8::String::NewFromUtf8(isolate, x, v8::NewStringType::kNormal).ToLocalChecked()
#endif//!v8_str

#if !defined(throw_js_error)
#define throw_js_error(isolate, err)\
	isolate->ThrowException(v8::Exception::Error(v8_str(isolate, err)))
#endif//!throw_js_error

#if !defined(throw_js_type_error)
#define throw_js_type_error(isolate, err)\
	isolate->ThrowException(v8::Exception::TypeError(v8_str(isolate, err)))
#endif//!throw_js_type_error

namespace sow_html_to_pdf {
	bool to_boolean(v8::Isolate* isolate, v8::Local<v8::Value> value);
	std::string to_cstr(v8::Isolate*isolate, v8::Local<v8::Value>val);
	const char* to_char_str(const v8::String::Utf8Value& value);
	const char* to_char_str(v8::Isolate* isolate, v8::Local<v8::Value> x);
	const char *v8_object_get_cc(
		v8::Isolate *isolate, v8::Local<v8::Context> ctx,
		v8::Local<v8::Object> v8_obj, const char *prop
	);
	int v8_object_get_str(
		v8::Isolate *isolate, v8::Local<v8::Context> ctx,
		v8::Local<v8::Object> v8_obj, const char *prop, std::string &out
	);
	int v8_object_get_number(
		v8::Isolate *isolate, v8::Local<v8::Context> ctx,
		v8::Local<v8::Object> obj, const char *prop
	);
	/*class native_string {
	private:
		char *_data;
		size_t _length;
		v8::String::Utf8Value *_utf8Value = nullptr;
		bool _invalid = false;
	public:
		explicit native_string( v8::Isolate *isolate, const v8::Local<v8::Value> &value );
		native_string( const native_string & ) = delete;
		native_string &operator=( const native_string & ) = delete;
		bool is_invalid( v8::Isolate *isolate );
		std::string& get_string( );
		const char *c_str( );
		bool is_empty( );
		size_t size( );
		//Sets the handle to be empty. is_empty() will then return true.
		void clear( );
		~native_string( );
	};*/
};
#endif //!_v8_util_h
