/**
 * Collection of files found on the gateway. One entry per file. Each entry has the
 * following fields.
 *   name
 *   path
 *   fileType
 *   size
 *   dataType
 *   permissions
 *   createdDate
 *   lastModifiedDate
 *
 * Notes:
 * - These entries are metadata referencing actual files on the device. Client uses this
 *   collection to find and work with files on the device. Client can also:
 *    - insert a new File (uploads a web File from the client to the server)
 *    - update an existing File (uploads a web File from the client to the server replaces existing file with new content)
 *    - fetch an existing File (downloads the web File from the server and saves locally)
 *    - remove an existing File
 *  The file transfer will be done by either chunking the file contents over DDP or by leveraging
 *  the servers http handlers.
 *
 *  - Clients can read the contents of the remote file system by subscribing to the
 *    collection as follows (note this is similar to settings/state subscription):
 *      Meteor.subscribe('files', findCriteria, options);
 *    For example:
 *      To get all files in the web directory:
 *        Meteor.subscribe('files', {path: 'web'}, {});
 *      To get all directories:
 *        Meteor.subscribe('files', {fileType: 'directory'}, {});
 *      To recursively get all files at or below a given path:
 *        Meteor.subscribe('files', {path: {$regex: '/^web//python/i'}}, {});
 *
 *  Reference see:
 *   https://atmospherejs.com/ostrio/files
 *   https://developer.mozilla.org/en-US/docs/Using_files_from_web_applications
 *   https://developer.mozilla.org/en-US/docs/Web/API/File
 *   https://developer.mozilla.org/en-US/docs/Web/API/FileList
 *   https://developer.mozilla.org/en-US/docs/Web/API/XMLHttpRequest/Using_XMLHttpRequest
 *
 *
 * Questions:
 *  1. Is html5 upload a standard? Can we build a server that handles that?
 *     Answer: The html5 upload improvements are all client side. They all still upload files as
 *             normal multi-part messages. Therefore we should be able to build or use a standard upload
 *             handler.
 *  2. Can we use EJSON binary data type to efficiently send data over DDP. how about
 *     compressing it as well?
 *     Answer: We can use the binary data type but unfortunately since DDP is text based this means encoding
 *             the binary data as text using something like base64 encoding. This expands the size of the
 *             payload significantly. Compression might help but you'd need to re-encode the binary output of
 *             the compressor. It's questionable how well it would really work. CFS tried this and switched to
 *             http upload for better performance.
 *
 *
 *  Info Links:   so many potentials ways out there...
 *
 *  meteor packages (can they work with device servers?)
 *    https://github.com/vsivsi/meteor-file-collection
 *    https://github.com/tomitrescak/meteor-uploads
 *    cfs is dead??
 *   +xrh post from client to server w/ proper handlers on server? (see option 2 below)
 *      http://stackoverflow.com/questions/27934141/meteor-uploading-file-from-client-to-mongo-collection-vs-file-system-vs-gridfs
 *    https://github.com/VeliovGroup/Meteor-Files/   (coffee)
 *  core ddp mechanism:
 *    https://gist.github.com/dariocravero/3922137
 *    https://forums.meteor.com/t/can-you-use-ddp-to-upload-files/7957/3
 *    https://www.eventedmind.com/feed/meteor-build-a-file-upload-package
 *  http client uploader:
 *    http://www.webcodegeeks.com/html5/html5-file-upload-example/
 *  http server handler:
 *    https://github.com/apendua/meteor-uploader/blob/master/server.js
 *  Third party upload service
 *    https://meteor.hackpad.com/Meteor-Cookbook-Filepicker.io-Uploads-and-Image-Conversion-hIpSwJQV3HJ
 *    https://github.com/CulturalMe/meteor-slingshot/
 *
 *  other overviews
 *    https://github.com/awatson1978/meteor-cookbook/blob/master/cookbook/file-uploads.md
 *
 * @type {Mongo.Collection}
 */
const Files = new Mongo.Collection('files');

export default Files;