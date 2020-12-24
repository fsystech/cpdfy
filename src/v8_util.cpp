/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
//6:24 PM 1/22/2019
#	include "v8_util.h"
#	include <libplatform/v8-tracing.h>
namespace sow_html_to_pdf {
	bool sow_html_to_pdf::to_boolean(v8::Isolate* isolate, v8::Local<v8::Value> value) {
#if V8_MAJOR_VERSION < 7 || (V8_MAJOR_VERSION == 7 && V8_MINOR_VERSION == 0)
		/* Old */
		return value->BooleanValue(isolate->GetCurrentContext()).ToChecked();
#else
		return value->BooleanValue(isolate);
#endif
	}
	const char* sow_html_to_pdf::to_char_str(const v8::String::Utf8Value& value) {
		if (value.length() <= 0)return "";
		return *value ? *value : "<string conversion failed>";
	}
	const char* sow_html_to_pdf::to_char_str(v8::Isolate* isolate, v8::Local<v8::Value> x) {
		v8::String::Utf8Value str(isolate, x);
		return to_char_str(str);
	}
	const char* v8_object_get_cc(
		v8::Isolate *isolate, v8::Local<v8::Context> ctx,
		v8::Local<v8::Object> v8_obj, const char *prop
	) {
		v8::Local<v8::Value> v8_str = v8_obj->Get( ctx, v8_str( isolate, prop ) ).ToLocalChecked( );
		if ( v8_str->IsNullOrUndefined( ) ) return NULL;
		if ( !v8_str->IsString( ) )return NULL;
		return to_char_str( isolate, v8_str );
	}
	int v8_object_get_str(
		v8::Isolate *isolate, v8::Local<v8::Context> ctx,
		v8::Local<v8::Object> v8_obj, const char *prop, std::string &out
	) {
		out.clear( );
		const char *str = v8_object_get_cc( isolate, ctx, v8_obj, prop );
		if ( str == NULL )return NULL;
		out = std::string( str );
		return 1;
	}
	int v8_object_get_number(
		v8::Isolate *isolate, v8::Local<v8::Context> ctx, 
		v8::Local<v8::Object> obj, const char *prop 
	) {
		v8::MaybeLocal<v8::Value> mval;
		mval = obj->Get( ctx, v8_str( isolate, prop ) );
		if ( mval.IsEmpty( ) ) {
			throw_js_error( isolate, "Value should be number..." );
			return -500;
		}
		v8::Local<v8::Value> val = mval.ToLocalChecked( );
		if ( !val->IsNumber( ) ) {
			throw_js_error( isolate, "Value should be number..." );
			return -500;
		}
		return val->Int32Value( ctx ).FromMaybe( 0 );
	}
}