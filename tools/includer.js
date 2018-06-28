const fs = require('fs');
const path = require('path');

const searchPath = 'D:/Dev/MomentumEngine-Real/source/components/ia/enemies/common/';
const iaIncludePath = '#include "components/ia/enemies/common/';

let includes = [];

searchSync(searchPath, iaIncludePath);

includes.forEach(include =>{
    console.log(include);
});

function searchSync(folder, includePath){
    const files = fs.readdirSync(folder).sort();
    console.log(files);
    files.forEach(file =>{
        let absolutePath = path.join(folder, file);
        const stat = fs.statSync(absolutePath);
        if(stat.isFile()){
            if(path.extname(file) === '.h'){
                includes.push(includePath + file + '"');
            }
        }
        else if(stat.isDirectory()){
            searchSync(absolutePath, includePath + file + '/');
        }
    });
}