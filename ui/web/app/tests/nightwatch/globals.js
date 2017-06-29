var device1 = {
  url: 'http://10.9.120.105',
  username: 'admin',
  password: 'admin',
  path: 'firmware_135.bin'
};

var device2 = {
  url: 'http://10.9.120.101',
  username: 'admin',
  password: 'admin',
  path: 'firmware_135.bin'
};

var localhost = {
  url: 'http://localhost:3000',
  username: 'admin',
  password: 'admin',
  path: 'firmware_135.bin'
};

var args = require('minimist')(process.argv.slice(1));
var globalSite;

function getSite() {
    return args["device"] || "localhost";
}

module.exports = {
    device1: device1,
    device2: device2,
    localhost: localhost,
    get site() {
        if (!globalSite) {
            globalSite = getSite();
        }
        return globalSite;
    }
};
