/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
#	include "default.h"
#	include "wj_pdf.h"
#	include "pdf_generator.h"
#	include <map>
#	include "v8_util.h"
#	include <nan.h>
using namespace sow_html_to_pdf;

void v8_object_loop(v8::Isolate* isolate, const v8::Local<v8::Object>v8_obj, std::map<std::string, std::string>& out_put) {
	v8::Local<v8::Context>ctx = isolate->GetCurrentContext();
	v8::Local<v8::Array> property_names = v8_obj->GetOwnPropertyNames(ctx).ToLocalChecked();
	uint32_t length = property_names->Length();
	for (uint32_t i = 0; i < length; ++i) {
		v8::Local<v8::Value> key = property_names->Get(ctx, i).ToLocalChecked();
		v8::Local<v8::Value> value = v8_obj->Get(ctx, key).ToLocalChecked();
		if (value->IsNullOrUndefined())continue;
		if (key->IsString() && value->IsString()) {
			const char* native_key = to_char_str(isolate, key);
			const char* native_value= to_char_str(isolate, value);
			out_put[std::string(native_key)] = std::string(native_value);
		}
	}
}
void generate_pdf(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = args.GetIsolate();
	if (args[0]->IsNullOrUndefined() || !args[0]->IsObject()) {
		throw_js_error(args.GetIsolate(),"Options should be Object");
		return;
	}
	if (args[1]->IsNullOrUndefined() || !args[1]->IsString()) {
		throw_js_error(args.GetIsolate(),"HTML body should be String");
		return;
	}
	pdf_ext::pdf_generator* pdf_gen = new pdf_ext::pdf_generator();
	v8::Local<v8::Context>ctx = isolate->GetCurrentContext();
	v8::Local<v8::Object> config = Nan::To<v8::Object>(args[0]).ToLocalChecked();
	v8::Local<v8::Value> v8_global_settings_str = config->Get(ctx, v8_str(isolate, "global_settings")).ToLocalChecked();
	auto wgs_settings = new std::map<std::string, std::string>();
	if (!v8_global_settings_str->IsNullOrUndefined() && v8_global_settings_str->IsObject()) {
		v8::Local<v8::Object>v8_global_settings_object = v8::Local<v8::Object>::Cast(v8_global_settings_str);
		v8_object_loop(isolate, v8_global_settings_object, *wgs_settings);
		v8_global_settings_str.Clear();
		v8_global_settings_object.Clear();
	}
	v8::Local<v8::Value> v8_object_settings_str = config->Get(ctx, v8_str(isolate, "object_settings")).ToLocalChecked();
	auto wos_settings = new std::map<std::string, std::string>();
	if (!v8_object_settings_str->IsNullOrUndefined() && v8_object_settings_str->IsObject()) {
		v8::Local<v8::Object>v8_object_settings_object = v8::Local<v8::Object>::Cast(v8_object_settings_str);
		v8_object_loop(isolate, v8_object_settings_object, *wos_settings);
		v8_object_settings_str.Clear();
		v8_object_settings_object.Clear();
	}
	config.Clear();
	int rec = pdf_gen->init(true, *wgs_settings, *wos_settings);
	_free_obj(wgs_settings); _free_obj(wos_settings);
	std::stringstream body_stream(to_char_str(isolate, args[1]));
	if (rec < 0) {
		body_stream << pdf_gen->get_status_msg();
		pdf_gen->dispose();
		delete pdf_gen;
	}else{
		rec = pdf_gen->generate(body_stream);
		if (rec < 0) {
			body_stream << pdf_gen->get_status_msg();
		}
		pdf_gen->dispose();
		delete pdf_gen;
	}
	std::string* byte_char = new std::string(body_stream.str());
	body_stream.clear(); std::stringstream().swap(body_stream);
	args.GetReturnValue().Set(Nan::CopyBuffer(byte_char->c_str(), byte_char->length()).ToLocalChecked());
	//args.GetReturnValue().Set(v8_str(isolate, byte_char->c_str()));
	//std::cout << byte_char->c_str() << std::endl;
	_free_obj(byte_char);
	return;
}
