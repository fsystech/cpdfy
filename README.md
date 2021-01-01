# cPDFY
HTML string to PDF converter with <a href="https://github.com/wkhtmltopdf/wkhtmltopdf" target="_blank">wkhtmltopdf</a> native library both of Win/Linux operating system

## Linux OS prerequisite
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
04# This example create `pdf` from `url` and write to `ServerResponse`
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
05# This example create `pdf` from `url` and write to `ServerResponse` with callback
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
#### Note: This version (v0.0.1) supported win32/win64/linux64