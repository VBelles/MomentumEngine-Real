const fs = require('fs');

const dir = 'instancing/'
const nameStartingWith = 'instancing_grass_02';
const files = fs.readdirSync(dir);

const outFile = dir + nameStartingWith + '.json';
const goodData = [];
files.forEach(file => {
    if (file.startsWith(nameStartingWith)) {
        const filePath = dir + file;
        const jsonStr = fs.readFileSync(filePath);
        const dirtyJson = JSON.parse(jsonStr);
        
        dirtyJson.TInstance[0].data.forEach(element => {
            goodData.push(element); //Probably there is a better way to do this.
        });
    }
});
// TODO: create TInstance and data array.
fs.writeFileSync(outFile, JSON.stringify(goodData, null, 3));
