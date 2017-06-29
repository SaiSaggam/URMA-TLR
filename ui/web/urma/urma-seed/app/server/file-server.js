import Files from 'meteor/digi:urma-seed';
//import Busboy from 'busboy';
import fs from 'fs';
import os from 'os';
import path from 'path';
import mkdirp from 'mkdirp';
import Fiber from 'fibers';
var log = new Logger('server/file-server');
Logger.setLevel('info');

Meteor.startup(function () {
    log.info('Starting File Server ');

    // Server upload endpoint using WebApp middleware handlers (busboy for multi-part file uploads)
    var uploadRoute = '/uploads';
    var localFSRoot = path.join(os.tmpDir(),'fsroot');
    WebApp.connectHandlers
        .use(uploadRoute, function(req, res, next) {
            if (req.method === "POST") {
                console.log('POST to '+uploadRoute+': ', req);
                req.body = req.body || {};
                var filenames = [];
                var formData = {};
                var busboy = new Busboy({ headers: req.headers });

                busboy.on("file", function (fieldname, file, filename, encoding, mimetype) {
                    console.log('saving file: ', filename, encoding, mimetype);
                    var subPath = req.originalUrl.substr(uploadRoute.length);
                    if (subPath.length<=0 && formData.filePath) {
                        subPath = formData.filePath;
                    }
                    if (subPath[subPath.length-1]!=='/') {
                        subPath += '/';
                    }
                    var fileSpec = path.join(subPath, filename);
                    var saveToPath = path.join(localFSRoot, subPath);
                    var saveTo = path.join(saveToPath, filename);
                    console.log('saving file to '+saveTo);
                    mkdirp(saveToPath, function(err) {
                        if (err) {
                            console.log('error creating upload directory '+saveToPath+' \n',err);
                            return;
                        }
                        file.pipe(fs.createWriteStream(saveTo));
                        filenames.push(saveTo);
                        var gatewayFile = {
                            path: subPath,
                            name: filename,
                            type: 'file',
                            encoding: encoding,
                            mimetype: mimetype,
                            fileSpec: fileSpec,
                            location: saveTo,
                            size: 0,
                            status: 'loading',
                            progress: 0
                        };
                        Fiber(function() {
                            Files.insert(gatewayFile);
                        }).run();
                    });
                });

                busboy.on("field", function(fieldname, value) {
                    console.log('busboy.on.field: ',fieldname, value);
                    formData[fieldname] = value;
                });

                busboy.on("finish", function () {
                    console.log('busboy.on.finish', filenames, formData);
                    // Pass filenames to request
                    req.filenames = filenames;
                    next();
                });

                // Pass request to busboy
                req.pipe(busboy);
            } else {
                next();
            }

            // TODO:
            //   - on client, pass additional formdata that includes file sizes of each file being uploaded
            //   - calculate progress of each file as it is being copied and update files entries
        });

});