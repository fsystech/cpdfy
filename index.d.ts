/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
//By Rajib Chy
// On 12:25 PM 12/25/2020
declare interface IPdfConfig {
    global_settings?: {
        documentTitle?: string;
        size?: { paperSize?: string };
        orientation?: string;
        colorMode?: string;
        dpi?: number;
        imageDPI?: number;
        imageQuality?: number;
        margin: {
            top?: string;
            bottom?: string;
            left?: string;
            right?: string;
        }
    };
    object_settings?: {
        web?: {
            defaultEncoding?: string;
            background?: boolean;
            loadImages?: boolean;
            enablePlugins?: boolean;
            enableJavascript?: boolean;
            enableIntelligentShrinking?: boolean;
            minimumFontSize?: number;
            printMediaType?: boolean;
        };
        header?: {
            fontSize?: number;
            /** e.g. `Times New Roman` */
            fontName?: string;
            /** e.g. `[date]` */
            left?: string;
            line?: boolean;
            spacing?: number;
        };
        footer?: {
            fontSize?: number;
            /** e.g. `Page [page] of [topage]` */
            right?: string;
            line?: boolean;
            spacing?: number;
        };
    };
}
declare interface html2pdf {
    generatePdf(config: IPdfConfig, htmlStr: string): Buffer;
    generatePdf(htmlStr: string): Buffer;
    generatePdfAsync(config: IPdfConfig, htmlStr: string): Promise<Buffer>;
    generatePdfAsync(htmlStr: string): Promise<Buffer>;
}