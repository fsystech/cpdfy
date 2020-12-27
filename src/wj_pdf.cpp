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

std::string to_cstr(v8::Isolate*isolate, v8::Local<v8::Value>val){
	v8::String::Utf8Value str(isolate, val);
	return std::string(*str);
}
void v8_object_loop(v8::Isolate* isolate, const v8::Local<v8::Object>v8_obj, std::map<std::string, std::string>& out_put) {
	v8::Local<v8::Context>ctx = isolate->GetCurrentContext();
	v8::Local<v8::Array> property_names = v8_obj->GetOwnPropertyNames(ctx).ToLocalChecked();
	uint32_t length = property_names->Length();
	for (uint32_t i = 0; i < length; ++i) {
		v8::Local<v8::Value> key = property_names->Get(ctx, i).ToLocalChecked();
		v8::Local<v8::Value> value = v8_obj->Get(ctx, key).ToLocalChecked();
		if (value->IsNullOrUndefined())continue;
		if (key->IsString() && value->IsString()) {
			out_put[to_cstr(isolate, key)] = to_cstr(isolate, value);
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
	std::string output;
	if (rec < 0) {
		output = std::string(pdf_gen->get_status_msg());
		pdf_gen->dispose();
		delete pdf_gen;
	}else{
		std::string cc_string = to_cstr(isolate, args[1]);
		rec = pdf_gen->generate(cc_string.c_str(), output);
		swap_obj(cc_string);
		if (rec < 0 && output.length() == 0) {
			output = std::string(pdf_gen->get_status_msg());
		}
		pdf_gen->dispose();
		delete pdf_gen;
	}
	if( rec <0 ){
		args.GetReturnValue().Set(v8_str(isolate, output.c_str()));
	} else{
		args.GetReturnValue().Set(Nan::CopyBuffer(output.c_str(), output.length()).ToLocalChecked());
	}
	swap_obj(output);
	return;
}
