//2:32 AM 11/27/2018
/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
#	include "pdf_generator.h"
#	include "default.h"
#	include <cstring>

int _is_wkhtmltopdf_init = FALSE;
void destroy_wkhtmltopdf(){
	if (_is_wkhtmltopdf_init == TRUE)
	wkhtmltopdf_deinit();
	_is_wkhtmltopdf_init = FALSE;
}
pdf_ext::pdf_generator::pdf_generator() {
	_status = -1; _disposed = FALSE; _msg = NULL;
	_wgs = NULL; _wos = NULL; _converter = NULL;
	finished = NULL; progress_changed = NULL;
	phase_changed = NULL; error = NULL;
	warning = NULL;
	version = wkhtmltopdf_version();
	_prepared_wgs = -1;
	_prepared_wos = -1;
	prepare_default_settings();
}
void pdf_ext::pdf_generator::set_status(int ret_val, const char * ret_msg) {
	_free_obj(_msg);
	_msg = new std::string(ret_msg, strlen(ret_msg));
	_status = ret_val;
}
void pdf_ext::pdf_generator::init_func() {
	if (finished != NULL) {
		wkhtmltopdf_set_finished_callback(_converter, finished);
	}
	if (progress_changed != NULL) {
		wkhtmltopdf_set_progress_changed_callback(_converter, progress_changed);
	}
	if (phase_changed != NULL) {
		wkhtmltopdf_set_phase_changed_callback(_converter, phase_changed);
	}
	if (error != NULL) {
		wkhtmltopdf_set_error_callback(_converter, error);
	}
	if (warning != NULL) {
		wkhtmltopdf_set_warning_callback(_converter, warning);
	}
}
int pdf_ext::pdf_generator::init_wgs() {
	if (_status < 0) {
		set_status(-1, "Not initilized. Please call `init(true);`!!!");
		return _status;
	}
	if(_prepared_wgs>0)return _status;
	for (std::map<std::string, std::string>::iterator itr = _wgs_settings->begin(); itr != _wgs_settings->end(); ++itr) {
		wkhtmltopdf_set_global_setting(_wgs, itr->first.c_str(), itr->second.c_str());
	}
	_prepared_wgs = TRUE;
	return _status;
}
int pdf_ext::pdf_generator::init_wos() {
	if (_status < 0) {
		set_status(-1, "Not initilized. Please call `init(true);`!!!");
		return _status;
	}
	if (_prepared_wos > 0)return _status;
	for (std::map<std::string, std::string>::iterator itr = _wos_settings->begin(); itr != _wos_settings->end(); ++itr) {
		wkhtmltopdf_set_object_setting(_wos, itr->first.c_str(), itr->second.c_str());
	}
	_prepared_wos = TRUE;
	return _status;
}
const char * pdf_ext::pdf_generator::get_status_msg() {
	if(_msg==NULL) return "NO_MSG_FOUND";
	return _msg->c_str();
}
pdf_ext::pdf_generator::~pdf_generator() {
	if (!_disposed)dispose();
}
void pdf_ext::pdf_generator::prepare_default_settings() {
	_wgs_settings = new std::map<std::string, std::string>{
		{ "useCompression", "false" }, { "documentTitle", "Hello World" }, { "size.paperSize", "A4" },
		{ "orientation", "Portrait" }, { "colorMode", "Color" }, { "dpi", "80" }, { "imageDPI", "300" },
		{ "imageQuality", "92" }, { "margin.top", "1.27cm" }, { "margin.bottom", "1.27cm" }, 
		{ "margin.left", "1.27cm" }, { "margin.right", "1.27cm" }
	};
	_wos_settings = new std::map<std::string, std::string>{
		{ "web.defaultEncoding", "utf-8" }, { "web.background", "true" }, { "web.loadImages", "true" },
		{ "web.enablePlugins", "false" }, { "web.enableJavascript", "false" },
		{ "web.enableIntelligentShrinking", "true" }, { "web.minimumFontSize", "12" },
		{ "web.printMediaType", "true" }, { "header.fontSize", "8" }, { "header.fontName", "Times New Roman" },
		{ "header.left", "[date]" }, { "header.line", "false" }, { "header.spacing", "0" }, { "footer.fontSize", "8" },
		{ "footer.left", "Powered by @ https://safeonline.world" }, { "footer.right", "Page [page] of [topage]" },
		{ "footer.line", "false" }, { "footer.spacing", "0" }
	};
}
int pdf_ext::pdf_generator::init(int use_graphics) {
	_status = -1;
	if( _is_wkhtmltopdf_init == FALSE ) {
		if (wkhtmltopdf_init(use_graphics) != 1) {
			_disposed = TRUE;
			set_status(_status, "PDF Engine init failed!!!");
			return -1;
		}
		_is_wkhtmltopdf_init = TRUE;
	}
	_wgs = wkhtmltopdf_create_global_settings();
	_wos = wkhtmltopdf_create_object_settings();
	_status = 1;
	return _status;
}
int pdf_ext::pdf_generator::update_map_key(
	std::map<std::string, std::string>&header, 
	std::string key, std::string values
) {
	auto it = header.find(key);
	if (it != header.end()) {
		it->second = std::string(values);
		return 1;
	}
	return -1;
}
int pdf_ext::pdf_generator::init(
	int use_graphics, 
	std::map<std::string, std::string>& wgs_settings,
	std::map<std::string, std::string>& wos_settings
) {
	_status = -1;
	if( _is_wkhtmltopdf_init == FALSE ) {
		if (wkhtmltopdf_init(use_graphics) != 1) {
			set_status(_status, "PDF Engine init failed!!!");
			return -1;
		}
		_is_wkhtmltopdf_init = FALSE;
	}
	_wgs = wkhtmltopdf_create_global_settings();
	_wos = wkhtmltopdf_create_object_settings();
	if (!wgs_settings.empty()&& !_wgs_settings->empty()) {
		for (auto itr = wgs_settings.begin(); itr != wgs_settings.end(); itr++) {
			int rec = this->update_map_key(*this->_wgs_settings, itr->first, itr->second);
			if (rec < 0) {
				std::string* msg = new std::string("Invlaid key defined in global_settings==>");
				msg->append(itr->first);
				set_status(_status, msg->c_str());
				_free_obj(msg);
				return -1;
			};
		}
	}
	if (!wos_settings.empty()) {
		for (auto itr = wos_settings.begin(); itr != wos_settings.end(); itr++) {
			if (this->update_map_key(*this->_wos_settings, itr->first, itr->second) < 0) {
				std::string* msg = new std::string("Invlaid key defined in object_settings==>");
				msg->append(itr->first);
				set_status(_status, msg->c_str());
				_free_obj(msg);
				return -1;
			};
		}
	}
	_status = 1;
	return _status;
}
int pdf_ext::pdf_generator::generate(const char * html, std::string& str_output) {
	if (_status < 0) {
		set_status (-1, "Not initialized!");
		return _status;
	}
	init_wgs(); init_wos();
	_converter = wkhtmltopdf_create_converter(_wgs);
	wkhtmltopdf_add_object(_converter, _wos, html);
	init_func();
	_disposed = FALSE;
	// Perform the conversion
	if (!wkhtmltopdf_convert(_converter)) {
		/* Output possible http error code encountered */
		set_status (-1, "PDF Conversion failed!");
		return _status;
	}
	const unsigned char *data = NULL;
	long len = wkhtmltopdf_get_output(_converter, &data);
	str_output = std::string(reinterpret_cast<const char*>(data), len);
	//delete[]data;
	data = NULL;
	set_status (1, "Success");
	return static_cast<int>(len);
}
int pdf_ext::pdf_generator::generate_to_path(const char * html, const char * output_path) {
	if (_status < 0) {
		set_status (-1, "Not initialized!");
		return _status;
	}
	init_wgs(); init_wos();
	wkhtmltopdf_set_global_setting(_wgs, "out", output_path);
	_converter = wkhtmltopdf_create_converter(_wgs);
	wkhtmltopdf_add_object(_converter, _wos, html);
	init_func();
	_disposed = FALSE;
	// Perform the conversion
	if (!wkhtmltopdf_convert(_converter)) {
		/* Output possible http error code encountered */
		set_status (-1, "PDF Conversion failed!");
		return _status;
	}
	set_status (1, "Success");
	return wkhtmltopdf_http_error_code(_converter);
}
int pdf_ext::pdf_generator::generate_from_url(const char * url, std::string& str_output) {
	if (_status < 0) {
		set_status (-1, "Not initialized!");
		return _status;
	}
	init_wgs(); init_wos();
	wkhtmltopdf_set_object_setting(_wos, "page", url);
	_converter = wkhtmltopdf_create_converter(_wgs);
	wkhtmltopdf_add_object(_converter, _wos, NULL);
	// Setup callbacks
	init_func();
	_disposed = FALSE;
	// Perform the conversion
	if (!wkhtmltopdf_convert(_converter)) {
		/* Output possible http error code encountered */
		set_status (-1, "PDF Conversion failed!");
		return _status;
	}
	const unsigned char *data = NULL;
	unsigned long len = wkhtmltopdf_get_output(_converter, &data);
	str_output = std::string(reinterpret_cast<const char*>(data), len);
	delete[] data;
	set_status (1, "Success");
	return static_cast<int>(len);
}
int pdf_ext::pdf_generator::generate_from_url(const char * url, const char* output_path) {
	if (_status < 0) {
		set_status (-1, "Not initialized!");
		return _status;
	}
	init_wgs(); init_wos();
	wkhtmltopdf_set_global_setting(_wgs, "out", output_path);
	wkhtmltopdf_set_object_setting(_wos, "page", url);
	_converter = wkhtmltopdf_create_converter(_wgs);
	wkhtmltopdf_add_object(_converter, _wos, NULL);
	// Setup callbacks
	init_func();
	_disposed = FALSE;
	// Perform the conversion
	if (!wkhtmltopdf_convert(_converter)) {
		/* Output possible http error code encountered */
		set_status (-1, "PDF Conversion failed!");
		return _status;
	}
	set_status (1, "Success");
	return wkhtmltopdf_http_error_code(_converter);
}

void pdf_ext::pdf_generator::dispose() {
	if ( _disposed == FALSE ) {
		_free_obj(_msg);
		_disposed = TRUE;
		if ( _wgs != NULL ) {
			/* Destroy the global settings since we are done with it */
			wkhtmltopdf_destroy_global_settings(_wgs); _wgs = NULL;
		}
		if (_wos != NULL) {
			/* Destroy the object settings since we are done with it */
			wkhtmltopdf_destroy_object_settings(_wos); _wos = NULL;
		}
		_free_obj(_wgs_settings); _free_obj(_wos_settings);
		if (_converter != NULL) {
			/* Destroy the converter object since we are done with it */
			wkhtmltopdf_destroy_converter(_converter); _converter = NULL;
		}
		//wkhtmltopdf_deinit();
	}
};
/*void finished(wkhtmltopdf_converter* converter, int p) {
	//printf("Finished: %d\n", p);
};

void progress_changed(wkhtmltopdf_converter* converter, int p) {
	//printf("%3d\n", p);
};

void phase_changed(wkhtmltopdf_converter* converter) {
	//int phase = wkhtmltopdf_current_phase(converter);
	//printf("Phase: %s\n", wkhtmltopdf_phase_description(converter, phase));
};

void error(wkhtmltopdf_converter* converter, const char * msg) {
	//printf("Error: %s\n", msg);
};

void warning(wkhtmltopdf_converter* converter, const char * msg) {
	//printf("Warning: %s\n", msg);
};*/
/**void to_char(const char* const_str, char*out_str) {
	out_str = new char[strlen(const_str)];
	strcpy(out_str, const_str);
};*/