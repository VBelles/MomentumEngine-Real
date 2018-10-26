// Removes objects with duplicate 'pos' inside files which
// filenames start with 'nameStartingWith', overwriting them.
// Requires array 'data' inside object 'TInstance'.

const fs = require('fs');

const dir = 'instancing/'
const nameStartingWith = 'instancing_planta1';
const filesInDir = fs.readdirSync(dir);

filesInDir.forEach(file =>{
    if ( file.startsWith(nameStartingWith) ) {
        const filePath = dir + file;
        const jsonStr = fs.readFileSync(filePath);
        const dirtyJson = JSON.parse(jsonStr);
        const positions = new Set();
        
        const goodData = [];
        dirtyJson.TInstance[0].data.forEach(element => {
            if ( !positions.has(element.pos) ) {
                positions.add(element.pos);
                goodData.push(element);
            }
        });
        dirtyJson.TInstance[0].data = goodData;
        
        fs.writeFileSync(filePath, JSON.stringify(dirtyJson, null, '\t'));
    }
});
