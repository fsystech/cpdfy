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
/**
 * Create PDF Config
 * @param {IPdfConfig} config
 * @returns {NodeJS.Dict<any>}
 */
function prepareConfig(config) {
    let outConfig = {};
    if (config.global_settings) {
        outConfig.global_settings = {};
        for (let prop in config.global_settings) {
            let next = config[prop];
            if (!next && typeof (next) === "object") {
                for (let nprop in next) {
                    outConfig.global_settings[`${prop}.${nprop}`] = String(next[nprop]);
                }
            }else{
                outConfig.global_settings[prop] = String(next);
            }
        }
    }
    if (config.object_settings) {
        outConfig.object_settings = {};
        for (let prop in config.object_settings) {
            let next = config[prop];
        }
    }
    return outConfig
}
class html2pdf {
    static generatePdf(config, htmlStr) {
        return nativeHtml2pdf.generate_pdf(prepareConfig(config), htmlStr);
    }
    static generatePdfAsync(config, htmlStr) {
        return new Promise((reject, reslove) => {
            reslove(nativeHtml2pdf.generate_pdf(prepareConfig(config), htmlStr));
        });
    }
};
module.exports.html2pdf = html2pdf;