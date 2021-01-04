// @ts-check
console.log(process.pid);
const path = require('path');
const os = require("os");
const platform = os.platform();
const arch = os.arch();
//if (platform !== 'win32' || arch !== 'ia32') throw new Error(`Not supported platform =>${platform} and arch => ${arch}`);
console.log(`platform =>${platform} and arch => ${arch}`);
const { Cpdfy } = require('./index');
const _test_dir = './test_output';
const fs = require('fs');
if (!fs.existsSync(_test_dir)){
    fs.mkdirSync(_test_dir);
}
const _html = `<!DOCTYPE html>
    <html lang="es">
    <head>
        <title>Test PDF</title>
    </head>
    <BODY>
        <h1 style="color:red;">Hello World....</h1>
    </BODY>
    </html>
    `;
const _test_html_path = `${_test_dir}/test.html`;
console.log(Cpdfy.getHttpHeader());
const sleep = require('util').promisify(setTimeout);
if (!fs.existsSync(_test_html_path)){
    fs.writeFileSync(_test_html_path, _html);
}
async function test() {
    /*try {
        const pdfStream = await Cpdfy.createStreamAsync({ from_url: "https://wkhtmltopdf.org/" });
        pdfStream.pipe(fs.createWriteStream(path.resolve(`./test_output/0_test_${Math.floor((0x999 + Math.random()) * 0x10000000)}.pdf`)));
    } catch (e) { 
        console.log(e);
    }*/
    Cpdfy.createStream({ from_url: "https://wkhtmltopdf.org/" }, (err, stream) => {
        if (err) {
            console.log(err);
            return;
        }
        const fst = fs.createWriteStream(path.resolve(`${_test_dir}/1_test_${Math.floor((0x999 + Math.random()) * 0x10000000)}.pdf`));
        stream.pipe(fst);
    });
    const html = fs.readFileSync(`${_test_dir}/test.html`, { encoding: "utf-8" }).replace(/^\uFEFF/, '');
    Cpdfy.createStream(html, (err, stream) => {
        if (err) {
            console.log(err);
            return;
        }
        const fst = fs.createWriteStream(path.resolve(`${_test_dir}/2_test_${Math.floor((0x999 + Math.random()) * 0x10000000)}.pdf`));
        stream.pipe(fst);
    });
    const file = fs.createWriteStream(path.resolve(`${_test_dir}/3_test_${Math.floor((0x999 + Math.random()) * 0x10000000)}.pdf`));
    Cpdfy.createStream(file, html, (err) => {
        if (err instanceof Error) {
            console.log(err);
        } else {
            console.log("Done...");
        }
    });
    
    const file4 = fs.createWriteStream(path.resolve(`${_test_dir}/4_test_${Math.floor((0x999 + Math.random()) * 0x10000000)}.pdf`));
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
    }, _html);
    return;
    Cpdfy.generatePdf({ from_url: "https://wkhtmltopdf.org/", out_path: path.resolve('./from_url.pdf') });
    //await sleep(5);

    const xhtml = `<!DOCTYPE html>
    <html lang="es">
    <head>
        <title>Test PDF</title>
    </head>
    <BODY>
        <h1 style="color:red;">Hello World....</h1>
    </BODY>
    </html>
    `;
    //await sleep(1000);
    console.log(os.tmpdir());

    Cpdfy.createStream({}, html, (err, stream) => {
        if (err) {
            console.log(err);
            return;
        }
        const fst = fs.createWriteStream(path.resolve(`./test_output/test_${Math.floor((0x999 + Math.random()) * 0x10000000)}.pdf`));
        fst.on("error", (err) => {
            console.log(err);
            Cpdfy.destroyApp();
            stream.emit("end");
        }).on("close", () => {
            console.log("destroy app");
            Cpdfy.destroyApp();
        });
        stream.pipe(fst);
    });
    console.log("exit");
    return;
    for (let i = 0; i < 2; i++) {
        const result = Cpdfy.generatePdf({ out_path: path.resolve(`./test_output/test_${Math.floor((0x999 + Math.random()) * 0x10000000)}.pdf`) }, html);
        console.log(`Result ${result}=>${i + 1}`);
        //if (global.gc) { global.gc(); }
        /*const pdfBuff = Cpdfy.generatePdf({ }, html);
        if (Buffer.isBuffer(pdfBuff)) {
            if (pdfBuff.length > 10) {
                require('fs').writeFileSync(`./test_output/test_${Math.floor((0x999 + Math.random()) * 0x10000000)}.pdf`, pdfBuff);
                console.log(`Success=>${i + 1}`);
            } else {
                console.log(pdfBuff.toString() + '=>' + i + 1);
            }

        } else {
            console.log(pdfBuff);
        }*/

        //await sleep(500);
    }

}
test();