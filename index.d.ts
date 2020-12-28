/**
* Copyright (c) 2018, SOW (https://www.safeonline.world). (https://github.com/RKTUXYN) All rights reserved.
* @author {SOW}
* Copyrights licensed under the New BSD License.
* See the accompanying LICENSE file for terms.
*/
//By Rajib Chy
// On 12:25 PM 12/25/2020
export interface IPdfConfig {
    /** Full url e.g. https://www.safeonline.world */
    from_url?: string;
    /** should be absolute path */
    out_path?: string;
    /** Absolute local file path */
    from_path?: string;
    global_settings?: {
        documentTitle?: string;
        size?: { paperSize?: string };
        orientation?: string;
        colorMode?: string;
        dpi?: number;
        imageDPI?: number;
        imageQuality?: number;
        margin?: {
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
export interface html2pdf_native {
    generate_pdf(...args: any[]): any;
    get_http_header(): NodeJS.Dict<string>;
    destroy_app(): void;
}
function getHttpHeader(): NodeJS.Dict<string>;
function createStream(config: IPdfConfig, htmlStr: string, next: (err: Error, stream: import('fs').ReadStream) => void);
function createStream(config: IPdfConfig, next: (err: Error, stream: import('fs').ReadStream) => void);
function createStream(htmlStr: string, next: (err: Error, stream: import('fs').ReadStream) => void);
function createStreamAsync(config: IPdfConfig, htmlStr: string): Promise<import('fs').ReadStream>;
function createStreamAsync(htmlStr: string): Promise<import('fs').ReadStream>;
function createStreamAsync(config: IPdfConfig): Promise<import('fs').ReadStream>;
function generatePdf(config: IPdfConfig, htmlStr: string): Buffer | void;
function generatePdf(config: IPdfConfig): void;
function generatePdf(htmlStr: string): Buffer;
function generatePdfAsync(config: IPdfConfig, htmlStr: string): Promise<Buffer>;
function generatePdfAsync(htmlStr: string): Promise<Buffer>;
function destroyApp(): void;