const os = require("os");
const platform = os.platform();
const arch = os.arch();
//if (platform !== 'win32' || arch !== 'ia32') throw new Error(`Not supported platform =>${platform} and arch => ${arch}`);
console.log(`platform =>${platform} and arch => ${arch}`);
const html2pdf = require('./build/Release/html_pdf_c').html_pdf_c;
console.log(html2pdf);
const pdfBuff = html2pdf.generate_pdf({}, `<!DOCTYPE html>
<html lang="es">
<head>
    <title>Test PDF</title>
</head>
<BODY>
    Hello World
</BODY>
</html>
`);
if (Buffer.isBuffer(pdfBuff)) {
    if(pdfBuff.length>10){
        require('fs').writeFileSync("./test.pdf", pdfBuff);
    }
    console.log(pdfBuff.toString())
    console.log("this is buffer...");
} else {
    console.log("Success");
    console.log(pdfBuff)
}

// call "D:\\Program Files\\nodejs\\node.exe" test.js