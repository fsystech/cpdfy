// @ts-check
/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
//By Rajib Chy
// On 11:00 AM 12/25/2020
const binary = require('node-pre-gyp');
const path = require('path');
const binding_path = binary.find(path.resolve(path.join(__dirname, './package.json')));
const nativeHtml2pdf = require(binding_path).html_pdf_c;
const defaultConfig = {
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
};
function parseConfig(obj, outObj){
    for (let prop in obj) {
        let next = obj[prop];
        if (!next && typeof (next) === "object") {
            for (let nprop in next) {
                outObj[`${prop}.${nprop}`] = String(next[nprop]);
            }
        }else{
            outObj[prop] = String(next);
        }
    }
}
/**
 * Create PDF Config
 * @param {import('./index').IPdfConfig} config
 * @returns {NodeJS.Dict<any>}
 */
function prepareConfig(config) {
    let outConfig = {};
    if (config.global_settings) {
        outConfig.global_settings = {};
        parseConfig(config.global_settings, outConfig.global_settings);
    }
    if (config.object_settings) {
        outConfig.object_settings = {};
        parseConfig(config.object_settings, outConfig.object_settings);
    }
    return outConfig
}
class html2pdf {
    static generatePdf(config, htmlStr) {
        return nativeHtml2pdf.generate_pdf(prepareConfig(config), htmlStr);
    }
    static getHttpHeader() {
        return nativeHtml2pdf.get_http_header();
    }
    static generatePdfAsync(config, htmlStr) {
        return new Promise((reject, reslove) => {
            reslove(nativeHtml2pdf.generate_pdf(prepareConfig(config), htmlStr));
        });
    }
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.html2pdf = html2pdf;