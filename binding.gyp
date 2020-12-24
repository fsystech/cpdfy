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
                        "./dependency/linux/wkhtmltopdf/include",
                        "<!(node -e \"require('nan')\")"
                    ],
                    "link_settings": {
                        "libraries": [
                        "../dependency/linux/wkhtmltopdf/lib/wkhtmltox.so",
                        ]
                    },
                }],
                ['OS=="win"', {
                    "include_dirs": [
                        "./dependency/win/x64/wkhtmltopdf/include",
                        "<!(node -e \"require('nan')\")"
                    ],
                    "link_settings": {
                        "libraries": [
                        "../dependency/win/x64/wkhtmltopdf/lib/wkhtmltox.lib",
                        ]
                    },
                    "defines": [
                        'WINDOWS_OS'
                    ]
                }]
            ]
        }
    ]
}
