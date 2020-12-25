{
    "targets": [
        {
            "target_name": "html_pdf_c",
            "cflags!": ["-fno-exceptions"],
            "cflags_cc!": ["-fno-exceptions"],
            "sources": [
                "./src/v8_util.cpp", "./src/addon.cc", "./src/pdf_generator.cpp", "./src/wj_pdf.cpp"
            ],
            'defines': ['NAPI_DISABLE_CPP_EXCEPTIONS'],
            "msvs_settings": {
                "VCCLCompilerTool": {
                    "ExceptionHandling": 1
                }
            },
            "direct_dependent_settings": {},
            "conditions": [
                ["OS=='linux'", {
                    'defines': [
                        'LINUX_OS',
                    ],
                    'include_dirs': [
                        "<!(node -e \"console.log('./dependency/linux/%s/wkhtmltopdf/include',require('process').arch);\")",
                        "<!(node -e \"require('nan')\")"
                    ],
                    "link_settings": {
                        "libraries": [
                            "<!(node -e \"console.log('../dependency/linux/%s/wkhtmltopdf/lib/libwkhtmltox.so',require('process').arch);\")"
                        ]
                    },
                }],
                ['OS=="win"', {
                    "include_dirs": [
                        "<!(node -e \"console.log('./dependency/win/%s/wkhtmltopdf/include',require('process').arch);\")",
                        "<!(node -e \"require('nan')\")"
                    ],
                    "link_settings": {
                        "libraries": [
                            "<!(node -e \"console.log('../dependency/win/%s/wkhtmltopdf/lib/wkhtmltox.lib',require('process').arch);\")"
                        ]
                    },
                    "defines": [
                        'WINDOWS_OS'
                    ]
                }]
            ]
        },
        {
            "target_name": "action_after_build",
            "type": "none",
            "dependencies": ["<(module_name)"],
            "conditions": [
                ["OS=='linux'", {
                    "copies": [{
                        "files": [
                            "<(PRODUCT_DIR)/<(module_name).node",
                            "<!(node -e \"console.log('./dependency/linux/%s/wkhtmltopdf/lib/libwkhtmltox.so',require('process').arch);\")"
                        ],
                        "destination": "<(module_path)"
                    }]
                }],
                ['OS=="win"', {
                    "copies": [{
                        "files": [
                            "<(PRODUCT_DIR)/<(module_name).node",
                            "<!(node -e \"console.log('./dependency/win/%s/wkhtmltopdf/bin/wkhtmltox.dll',require('process').arch);\")"
                        ],
                        "destination": "<(module_path)"
                    }]
                }]
            ]
        }
    ]
}
