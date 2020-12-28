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
	if( val->IsNullOrUndefined() ) return std::string();
	v8::String::Utf8Value str(isolate, val);
	return std::string(*str);
}
void to_cstr(v8::Isolate*isolate, v8::Local<v8::Value>val, std::string&output){
	if( val->IsNullOrUndefined() ) return;
	v8::String::Utf8Value str(isolate, val);
	output.insert(output.length(), *str);
}
void destroy_app(const v8::FunctionCallbackInfo<v8::Value>& args){
	destroy_wkhtmltopdf();
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
	pdf_ext::pdf_generator* pdf_gen = new pdf_ext::pdf_generator();
	v8::Local<v8::Context>ctx = isolate->GetCurrentContext();
	v8::Local<v8::Object> config = Nan::To<v8::Object>(args[0]).ToLocalChecked();
	std::string* output_path_str = new std::string();
	to_cstr(isolate, config->Get(ctx, v8_str(isolate, "out_path")).ToLocalChecked(), *output_path_str);
	std::string *from_url_str = new std::string();
	to_cstr(isolate,config->Get(ctx, v8_str(isolate, "from_url")).ToLocalChecked(), *from_url_str);
	int is_from_url = FALSE;
	if( from_url_str->length() > 0 ){
		if( output_path_str->length() == 0 ){
			throw_js_error(isolate,"out_path required. If you like to generate pdf from url.");
			_free_obj(from_url_str);_free_obj(output_path_str);
			return;
		}
		is_from_url = TRUE;
	}
	if( is_from_url == FALSE ) {
		if (args[1]->IsNullOrUndefined() || !args[1]->IsString()) {
			throw_js_error(args.GetIsolate(),"HTML body should be String");
			return;
		}
	}
	int rec = -1;
	v8::Local<v8::Value> v8_global_settings_str = config->Get(ctx, v8_str(isolate, "global_settings")).ToLocalChecked();
	std::map<std::string, std::string>* wgs_settings = new std::map<std::string, std::string>();
	if (!v8_global_settings_str->IsNullOrUndefined() && v8_global_settings_str->IsObject()) {
		v8::Local<v8::Object>v8_global_settings_object = v8::Local<v8::Object>::Cast(v8_global_settings_str);
		v8_object_loop(isolate, v8_global_settings_object, *wgs_settings);
		v8_global_settings_str.Clear();
		v8_global_settings_object.Clear();
	}
	v8::Local<v8::Value> v8_object_settings_str = config->Get(ctx, v8_str(isolate, "object_settings")).ToLocalChecked();
	std::map<std::string, std::string>* wos_settings = new std::map<std::string, std::string>();
	if (!v8_object_settings_str->IsNullOrUndefined() && v8_object_settings_str->IsObject()) {
		v8::Local<v8::Object>v8_object_settings_object = v8::Local<v8::Object>::Cast(v8_object_settings_str);
		v8_object_loop(isolate, v8_object_settings_object, *wos_settings);
		v8_object_settings_str.Clear();
		v8_object_settings_object.Clear();
	}
	rec = pdf_gen->init(FALSE, *wgs_settings, *wos_settings);
	if (rec < 0) {
		throw_js_error(isolate, pdf_gen->get_status_msg());
		pdf_gen->dispose();
		delete pdf_gen;
	} else {
		if( is_from_url == FALSE ) {
			int has_output_path = output_path_str->length() > 0 ? TRUE : FALSE;
			std::string* output = new std::string();
			std::string* cc_string = new std::string();
			to_cstr(isolate, args[1], *cc_string);
			if( has_output_path == FALSE ){
				rec = pdf_gen->generate(cc_string->c_str(), *output);
			} else {
				rec = pdf_gen->generate_to_path(cc_string->c_str(), output_path_str->c_str());
			}
			_free_obj(cc_string); args[1].Clear();
			if (rec < 0) {
				output->insert(output->length(), pdf_gen->get_status_msg());
			}else if( has_output_path == FALSE && output->length() == 0){
				rec = -1;
				output->insert(output->length(), "NO_OUTPUT_DATA_FOUND");
			}
			pdf_gen->dispose();
			delete pdf_gen;
			if( rec < 0 ){
				throw_js_error(isolate, output->c_str());
			} else {
				if( has_output_path == TRUE ) {
					args.GetReturnValue().Set(v8::Number::New(isolate, rec));
				} else {
					args.GetReturnValue().Set(Nan::CopyBuffer(output->c_str(), output->length()).ToLocalChecked());
				}
			}
			_free_obj(output);
		} else {
			rec = pdf_gen->generate_from_url(from_url_str->c_str(), output_path_str->c_str());
			if (rec < 0) {
				throw_js_error(isolate, pdf_gen->get_status_msg());
			}
			pdf_gen->dispose();
			delete pdf_gen;
			if( rec >= 0 ){
				args.GetReturnValue().Set(v8::Number::New(isolate, rec));
			}
		}
	}
	_free_obj(wgs_settings); _free_obj(wos_settings);
	_free_obj(from_url_str); _free_obj(output_path_str);
	config.Clear();
	return;
}
void get_http_header(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = args.GetIsolate();
	v8::Local<v8::Context> context = isolate->GetCurrentContext( );
	v8::Local<v8::Object> header = v8::Object::New( isolate );
	header->Set( context, v8_str( isolate, "x-wkhtmltopdf-version" ), v8_str(isolate, wkhtmltopdf_version()) ).ToChecked();
	header->Set( context, v8_str( isolate, "accept-ranges" ), v8_str(isolate, "bytes") ).ToChecked();
	header->Set( context, v8_str( isolate, "content-type" ), v8_str(isolate, "application/pdf") ).ToChecked();
	args.GetReturnValue().Set(header);
}