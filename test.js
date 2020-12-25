console.log(require('process').arch);
const os = require("os");
const platform = os.platform();
const arch = os.arch();
//if (platform !== 'win32' || arch !== 'ia32') throw new Error(`Not supported platform =>${platform} and arch => ${arch}`);
console.log(`platform =>${platform} and arch => ${arch}`);
const { html2pdf } = require('./index');
console.log(html2pdf);

const pdfBuff = html2pdf.generatePdf({}, `<!DOCTYPE html>
<html lang="es">
<head>
    <title>Test PDF</title>
</head>
<BODY>
    <h1 style="color:red;">Hello World....</h1>
</BODY>
</html>
`);
if (Buffer.isBuffer(pdfBuff)) {
    if (pdfBuff.length > 10) {
        require('fs').writeFileSync("./test.pdf", pdfBuff);
    }
    console.log("Success");
} else {
    console.log("No Response found..");
}

// call "D:\\Program Files\\nodejs\\node.exe" test.js