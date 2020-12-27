/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
// by Rajib chy
// 6:55 PM 12/12/2020
#include "v8_util.h"
#include "wj_pdf.h"

#define SET_NATIVE_METHOD(app, isolate, method_name, method, context) \
app->Set( context,                                                    \
v8_str( isolate, method_name ),                                       \
v8::FunctionTemplate::New( isolate, method )                          \
->GetFunction( context ).ToLocalChecked( ) ).FromJust( );

void Initialize( v8::Local<v8::Object> exports ) {
    v8::Isolate *isolate = v8::Isolate::GetCurrent( );
    v8::Local<v8::Context> context = isolate->GetCurrentContext( );
    
    v8::Local<v8::Object> app = v8::Object::New( isolate );
    // Expose the method `Method` to JavaScript, and make sure it receives the
    // per-addon-instance data we created above by passing `external` as the
    // third parameter to the `FunctionTemplate` constructor.
    SET_NATIVE_METHOD( app, isolate, "generate_pdf", generate_pdf, context );
    bool status = exports->Set( context, v8_str( isolate, "html_pdf_c" ), app ).ToChecked();
    if(status == false){
        throw_js_error(isolate, "Unable to create addon.");
    }
}

NODE_MODULE( html_pdf_c, Initialize )