const fs = require('fs');
const path = require('path');

const searchPath = 'D:/Dev/MomentumEngine-Real/source/components/ia/enemies/common/';
const iaIncludePath = '#include "components/ia/enemies/common/';

let includes = [];

search(searchPath, iaIncludePath);
function search(folder, includePath) {
    fs.readdir(folder, function (err, files) {
        files.forEach(function (file, index) {
            let fromPath = path.join(folder, file);
            fs.stat(fromPath, function (error, stat) {
                if (stat.isFile()) {
                    if (path.extname(file) == '.h') {
                        includes.push(includePath + file + '"');
                    }
                } else if (stat.isDirectory()) {
                    search(fromPath, includePath + file + '/');
                }
            });
        });
    });

    includes.forEach(value => {
        console.log(value);
    });
}

