// @ts-check
/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
//By Rajib Chy
// On 11:00 AM 12/25/2020
/** 
 * @typedef {import('./index').ICPdfConfig} ICPdfConfig
 */
const path = require('path');
const os = require("os");
const fs = require('fs');
const { Writable } = require('stream');
const { ServerResponse } = require('http');

/**
 * Import PDF Native Module
 * @returns {import('./index').html2pdf_native}
 */
function import_module() {
    let binding_path;
    if (process.env.LUNCH_MODE === "DEBUG") {
        binding_path = "./build/Release/cpdfy.node";
    } else {
        binding_path = require('node-pre-gyp').find(path.resolve(path.join(__dirname, './package.json')));
    }
    return require(binding_path).cpdfy;
}
const cpdfy = import_module();
/*const defaultConfig = {
    global_settings: {
        "documentTitle": "Hello World",
        "size.paperSize": "A4",
        "orientation": "Portrait",
        "colorMode": "Color",
        "dpi": "80",
        "imageDPI": "300",
        "imageQuality": "92",
        "margin.top": "1.27cm",
        "margin.bottom": "1.27cm",
        "margin.left": "1.27cm",
        "margin.right": "1.27cm"
    },
    object_settings: {
        "web.defaultEncoding": "utf-8",
        "web.background": "true",
        "web.loadImages": "true",
        "web.enablePlugins": "false",
        "web.enableJavascript": "false",
        "web.enableIntelligentShrinking": "true",
        "web.minimumFontSize": "12",
        "web.printMediaType": "true",
        "header.fontSize": "8",
        "header.fontName": "Times New Roman",
        "header.left": "[date]",
        "header.line": "false",
        "header.spacing": "0",
        "footer.fontSize": "8",
        "footer.right": "Page [page] of [topage]",
        "footer.line": "false",
        "footer.spacing": "0"
    }
};*/
/**
 * To `String`
 * @param {any} val
 * @returns {string} 
 */
function toString(val) {
    if (!val) return undefined;
    if (typeof (val) === "string") return val;
    return String(val);
}
function parseConfig(obj, outObj) {
    for (let prop in obj) {
        let next = obj[prop];
        if (next && typeof (next) === "object") {
            for (let nprop in next) {
                outObj[`${prop}.${nprop}`] = toString(next[nprop]);
            }
        } else {
            outObj[prop] = toString(next);
        }
    }
}
/**
 * Create PDF Config
 * @param {ICPdfConfig} config
 */
function prepareConfig(config) {
    /** @type {ICPdfConfig} */
    let outConfig = {};
    if (config.global_settings) {
        outConfig.global_settings = {};
        parseConfig(config.global_settings, outConfig.global_settings);
    }
    if (config.object_settings) {
        outConfig.object_settings = {};
        parseConfig(config.object_settings, outConfig.object_settings);
    }
    outConfig.from_path = config.from_path || undefined;
    outConfig.from_url = config.from_url || undefined;
    outConfig.out_path = config.out_path || undefined;
    for (let prop in outConfig) {
        config[prop] = outConfig[prop];
    }
}
/**
 * Set pdf response header
 * @param {ServerResponse} res 
 */
function _setHeader(res) {
    const header = cpdfy.get_http_header();
    for (let key in header) {
        res.setHeader(key, header[key]);
    }
}
/**
 * Write to Http Response
 * @param {Writable} res
 * @param {ICPdfConfig} config 
 * @param {string} htmlStr
 * @param {(err?:Error)=>void} next
 * @param {()=>boolean} onOpen
 * @returns {void} 
 */
function _pipeToWritableStream(res, config, htmlStr, next, onOpen) {
    prepareConfig(config);
    config.out_path = path.resolve(`${os.tmpdir()}/${Math.floor((0x999 + Math.random()) * 0x10000000)}.pdf`);
    try {
        cpdfy.generate_pdf(config, htmlStr);
    } catch (e) {
        return next(e);
    }
    const stream = fs.createReadStream(config.out_path);
    let isEnded = false;
    stream.on("open", (fd) => {
        if (!onOpen()) {
            return stream.emit("end");
        }
        stream.pipe(res);
    }).on("error", (err) => {
        return next(err);
    }).on("end", () => {
        if (isEnded) return;
        isEnded = true;
        fs.stat(config.out_path, (err, state) => {
            if (state) {
                fs.rm(config.out_path, (err) => {
                    // No need to verify this result
                });
            }
        });
        return next();
    });
    return void 0;
}
/**
 * Write to Http Response
 * @param {Writable} res
 * @param {ICPdfConfig} config 
 * @param {string} htmlStr
 * @param {(err?:Error)=>void} next
 * @returns {void} 
 */
function _pipeStream(res, config, htmlStr, next) {
    if (typeof (config) === "string") {
        if (typeof (htmlStr) === "function") {
            next = htmlStr; htmlStr = undefined;
        }
        htmlStr = config; config = {};
    }
    if (typeof (htmlStr) === "function") {
        next = htmlStr; htmlStr = undefined;
    } else if (typeof (htmlStr) !== "string") {
        htmlStr = undefined;
    }
    /** @type {()=>boolean} */
    let onOpen;
    if ("headersSent" in res || res instanceof ServerResponse) {
        onOpen = () => {
            if (res.headersSent) {
                return next(new Error("Remote connection closed...")), false;
            }
            return _setHeader(res), true;
        };
    } else {
        onOpen = () => true;
    }
    if (typeof (next) !== "function") {
        next = (...args) => void 0;
    }
    return _pipeToWritableStream(res, config, htmlStr, next, onOpen);
}
class Cpdfy {
    /**
     * Generate PDF
     * @param {ICPdfConfig} config 
     * @param {string|void} htmlStr 
     */
    static generatePdf(config, htmlStr) {
        if (typeof (config) === "string") {
            htmlStr = config; config = {};
        }
        prepareConfig(config);
        return cpdfy.generate_pdf(config, htmlStr);
    }
    /**
     * 
     * @param {ServerResponse} res 
     */
    static setHeader(res) {
        return _setHeader(res);
    }
    static getHttpHeader() {
        return cpdfy.get_http_header();
    }
    static destroyApp() {
        return cpdfy.destroy_app();
    }
    /**
     * Create or pip to ouput stream
     * @param {ICPdfConfig|ServerResponse|fs.WriteStream} config 
     * @param {string} htmlStr
     * @param {(err:Error, stream:fs.ReadStream)=>void} next
     * @returns {void} 
     */
    static createStream(config, htmlStr, next) {
        if (config instanceof ServerResponse || config instanceof fs.WriteStream) {
            return _pipeStream.apply(this, Array.prototype.slice.call(arguments));
        }
        if (typeof (config) === "string") {
            if (typeof (htmlStr) === "function") {
                next = htmlStr; htmlStr = undefined;
            }
            htmlStr = config; config = {};
        }
        if (typeof (htmlStr) === "function") {
            next = htmlStr; htmlStr = undefined;
        } else if (typeof (htmlStr) !== "string") {
            htmlStr = undefined;
        }
        if (typeof (next) !== "function") {
            throw new Error("Callback should be instance of `Function`.");
        }
        prepareConfig(config);
        config.out_path = path.resolve(`${os.tmpdir()}/${Math.floor((0x999 + Math.random()) * 0x10000000)}.pdf`);
        try {
            cpdfy.generate_pdf(config, htmlStr);
        } catch (e) {
            return next(e, null);
        }
        const stream = fs.createReadStream(config.out_path);
        stream.on("open", (fd) => {
            next(null, stream);
        }).on("error", (err) => {
            next(err, null);
        }).on("end", () => {
            // @ts-ignore
            fs.stat(config.out_path, (err, state) => {
                if (state) {
                    // @ts-ignore
                    fs.rm(config.out_path, (err) => {
                        // No need to verify this result
                    });
                }
            });
        });
        return void 0;
    }
    static createStreamAsync(config, htmlStr) {
        if (typeof (config) === "string") {
            htmlStr = config; config = {};
        }
        return new Promise((reject, reslove) => {
            try {
                Cpdfy.createStream(config, htmlStr, (err, stream) => {
                    if (err) return reject(err);
                    return reslove(stream);
                });
            } catch (e) {
                return reject(e);
            }
        });
    }
    static generatePdfAsync(config, htmlStr) {
        return new Promise((reject, reslove) => {
            try {
                return reslove(Cpdfy.generatePdf(config, htmlStr));
            } catch (e) {
                return reject(e);
            }

        });
    }
};
module.exports.Cpdfy = Cpdfy;