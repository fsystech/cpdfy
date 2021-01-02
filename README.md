[![cPDFY Logo][cpdfy-logo]][cpdfy-url] <br/>

HTML to PDF converter with <a href="https://github.com/wkhtmltopdf/wkhtmltopdf">wkhtmltopdf</a> native library. Run on both ```Win/Linux``` operating system.<br/><br/>
```cPDFY``` is not a precompiled library. It will be compiled from source code in your machine.<br/>
Before install `cPDFY` see `node-gyp` prerequisite from <a href="https://github.com/nodejs/node-gyp#node-gyp---nodejs-native-addon-build-tool">here</a><br/><br/>

## Windows prerequisite
`npm install --global windows-build-tools`<br/><br/>
## Linux OS prerequisite
`Python v2.7, v3.5, v3.6, v3.7, or v3.8`<br/>
`sudo apt install make`<br/>
`sudo apt install g++`<br/>
`sudo apt install libfontconfig1 libxrender1`<br/>
## How to install ?
```npm install cpdfy --save```
### How to use it ?
```javascript
/*import cPdfy instance*/
const { Cpdfy } = require('cpdfy');
```
#### Working with stream
01# This example create `pdf` from `url`
```javascript
Cpdfy.createStream({ from_url: "https://wkhtmltopdf.org/" }, (err, stream) => {
    if (err) {
        console.log(err);
        return;
    }
    const fst = fs.createWriteStream(path.resolve(`./test_output/1_test_${Math.floor((0x999 + Math.random()) * 0x10000000)}.pdf`));
    stream.pipe(fst);
});
```
02# This example create `pdf` from `html string`
```javascript
const html = fs.readFileSync('./test_output/test.html', { encoding: "utf-8" }).replace(/^\uFEFF/, '');
Cpdfy.createStream(html, (err, stream) => {
    if (err) {
        console.log(err);
        return;
    }
    const fst = fs.createWriteStream(path.resolve(`./test_output/2_test_${Math.floor((0x999 + Math.random()) * 0x10000000)}.pdf`));
    stream.pipe(fst);
});
```
03# This example create `pdf` from `html string` and write to `fs.WriteStream`
```javascript
const file = fs.createWriteStream(path.resolve(`./test_output/3_test_${Math.floor((0x999 + Math.random()) * 0x10000000)}.pdf`));
Cpdfy.createStream(file, html, (err) => {
    if (err instanceof Error) {
        console.log(err);
    } else {
        console.log("Done...");
    }
});
```
04# This example create `pdf` from `string` with `ICPdfConfig` and write to `WriteStream`
```javascript
const html = `
<!DOCTYPE html>
    <html lang="es">
    <head>
        <title>Test PDF</title>
    </head>
    <body>
        <h1 style="color:red;">Hello World....</h1>
    </body>
</html>
`;
const file4 = fs.createWriteStream(path.resolve(`./test_output/4_test_${Math.floor((0x999 + Math.random()) * 0x10000000)}.pdf`));
Cpdfy.createStream(file4, {
    global_settings: {
        documentTitle: "This is printed copy",
        orientation: "Landscape",
        size: {
            paperSize: "Legal"
        },
        margin: {
            top: "1.27cm",
            bottom: "1.27cm",
            left: "1.27cm",
            right: "1.27cm",
        }
    }
}, html);
```
05# This example create `pdf` from `url` and write to `ServerResponse`
```javascript
controller.get('/pdf-test', (ctx, match) => {
    const url = help.mayBeString(ctx.req.query["url"]);
    if (!url) {
        return ctx.res.status(200).type("text").send("Invalid request...");
    }
    ctx.res.status(200).noCache();
    Cpdfy.createStream(ctx.res, { from_url: decodeURIComponent(url) }, (err) => err && ctx.handleError(err, () => { }));
});
```
06# This example create `pdf` from `url` and write to `ServerResponse` with callback
```javascript
controller.get('/pdf', (ctx, match) => {
    const url = help.mayBeString(ctx.req.query["url"]);
    if (!url) {
        return ctx.res.status(200).type("text").send("Invalid request...");
    }
    Cpdfy.createStream({ from_url: decodeURIComponent(url) }, (err, stream) => {
        ctx.handleError(err, () => {
            Cpdfy.setHeader(ctx.res);
            ctx.res.status(200).noCache().type("pdf");
            stream.pipe(ctx.res);
        });
    });
});
```
07# This example create `pdf` from `url` and write to ouput path
```javascript
Cpdfy.generatePdf({ from_url: "https://wkhtmltopdf.org/", out_path: path.resolve('./from_url.pdf') });
```
08# This example create `pdf` from `url` and return `Buffer`
```javascript
Cpdfy.generatePdf({ from_url: "https://wkhtmltopdf.org/" });
```
#### Note: This version supports win32/win64/linux64
[cpdfy-logo]: https://i.imgur.com/YtsVVhk.png
[cpdfy-url]: http://cpdfy.safeonline.world/
