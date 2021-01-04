/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
// by Rajib chy
// 6:55 PM 12/12/2020
#   include "v8_util.h"
#   include "wj_pdf.h"

#define SET_NATIVE_METHOD(app, isolate, method_name, method, external, context) \
    app->Set( context,                                                          \
    v8_str( isolate, method_name ),                                             \
    v8::FunctionTemplate::New( isolate, method, external )                      \
    ->GetFunction( context ).ToLocalChecked( ) ).FromJust( );

void Initialize( v8::Local<v8::Object> exports ) {
    v8::Isolate *isolate = v8::Isolate::GetCurrent( );
    v8::Local<v8::Context> context = isolate->GetCurrentContext( );
    // Create a new instance of `AddonData` for this instance of the addon and
    // tie its life cycle to that of the Node.js environment.
    internal_data *data = new internal_data( isolate );
    // Wrap the data in a `v8::External` so we can pass it to the method we
    // expose.
    v8::Local<v8::External> external = v8::External::New( isolate, data );
    v8::Local<v8::Object> app = v8::Object::New( isolate );
    // Expose the method `Method` to JavaScript, and make sure it receives the
    // per-addon-instance data we created above by passing `external` as the
    // third parameter to the `FunctionTemplate` constructor.
    SET_NATIVE_METHOD( app, isolate, "generate_pdf", generate_pdf, external, context );
    SET_NATIVE_METHOD( app, isolate, "get_http_header", get_http_header, external, context );
    SET_NATIVE_METHOD( app, isolate, "destroy_app", destroy_app, external, context );
    exports->Set( context, v8_str( isolate, "cpdfy" ), app ).ToChecked( );
}

NODE_MODULE( cpdfy, Initialize )