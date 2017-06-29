import {Collections} from 'meteor/digi:urma-core';
import fwUpdateSimulator from './firmware-update-simulator.js';
import Busboy from 'busboy';
import fs from 'fs';
import os from 'os';
import path from 'path';
import mkdirp from 'mkdirp';
import Fiber from 'fibers';
let log = new Logger('server/file-server');

let Files = Collections.Files;
let State = Collections.State;

Meteor.startup(function () {
    log.info('Starting File Server ');

    // Server upload endpoint using WebApp middleware handlers (busboy for multi-part file uploads)
    let uploadRoute = '/uploads';
    let firmwareRoute = '/firmware';

    let localFSRoot = path.join(os.tmpDir(),'fsroot');
    WebApp.connectHandlers
        // Upload route
        .use(uploadRoute, function(req, res, next) {
            if (req.method === "POST") {
                log.debug('POST to '+uploadRoute+', url:', req.originalUrl);
                log.trace(' -> localFSRoot:', localFSRoot);
                log.trace(' -> HTTP request:', req);
                req.body = req.body || {};
                let filenames = [];
                let formData = {};
                let busboy = new Busboy({ headers: req.headers });

                busboy.on("file", function (fieldname, file, filename, encoding, mimetype) {
                    log.debug('saving file: ', filename, encoding, mimetype);
                    let subPath = req.originalUrl.substr(uploadRoute.length);
                    if (subPath.length<=0 && formData.filePath) {
                        subPath = formData.filePath;
                    }
                    if (subPath[subPath.length-1]!=='/') {
                        subPath += '/';
                    }
                    let fileSpec = path.join(subPath, filename);
                    let saveToPath = path.join(localFSRoot, subPath);
                    let saveTo = path.join(saveToPath, filename);
                    log.debug('saving file to '+saveTo);
                    mkdirp(saveToPath, function(err) {
                        if (err) {
                            log.debug('error creating upload directory '+saveToPath+' \n',err);
                            return;
                        }
                        file.pipe(fs.createWriteStream(saveTo));
                        filenames.push(saveTo);
                        let gatewayFile = {
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
                    log.debug('busboy.on.field: ',fieldname, value);
                    formData[fieldname] = value;
                });

                busboy.on("finish", function () {
                    log.debug('busboy.on.finish', filenames, formData);
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
        })
        // Firmware route
        .use(firmwareRoute, Meteor.bindEnvironment(function(req, res, next) {
            if (req.method === "POST") {
                log.debug('POST to '+firmwareRoute+', url:', req.originalUrl);
                log.trace(' -> localFSRoot:', localFSRoot);
                log.trace(' -> HTTP request:', req);

                fwUpdateSimulator.reset();

                req.body = req.body || {};
                let filenames = [];
                let formData = {};
                let busboy = new Busboy({ headers: req.headers });

                busboy.on("file", function (fieldname, file, filename, encoding, mimetype) {
                    log.debug('saving firmware file: ', filename, encoding, mimetype);

                    let subPath = 'firmware/'; // hard coded firmware location
                    let fileSpec = path.join(subPath, filename);
                    let saveToPath = path.join(localFSRoot, subPath);
                    let saveTo = path.join(saveToPath, filename);
                    log.debug('saving firmware file to '+saveTo);
                    mkdirp(saveToPath, function(err) {
                        if (err) {
                            log.debug('error creating firmware directory '+saveToPath+' \n',err);
                            return;
                        }
                        file.pipe(fs.createWriteStream(saveTo));
                        filenames.push(saveTo);
                    });
                });

                busboy.on("field", function(fieldname, value) {
                    log.debug('busboy.on.field: ',fieldname, value);
                    formData[fieldname] = value;
                });

                busboy.on("finish", Meteor.bindEnvironment(function () {
                    log.debug('busboy.on.finish', filenames, formData);
                    // Pass filenames to request
                    req.filenames = filenames;
                    next();
                }));

                // Pass request to busboy
                return req.pipe(busboy);
            } else {
                next();
            }
        }));

});