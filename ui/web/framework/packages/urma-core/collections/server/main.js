import Settings from '../settings.js';
import State from '../state.js';
import Files from '../files.js';
//import './file-server.js';
//import './device-server.js';

//var log = new Logger('server/main');
//Logger.setLevel('info');

/*
 * The initial Meteor server startup function.
 */
let startup = function() {
    //log.info('Starting Server ');

    /**
     * Establish the publish/allow/deny rules for all the collections
     */

    // Some helpers for collection allow/deny rules
    var allowAll = {
        insert: function (userId, doc) {
            return true;
        },
        update: function (userId, doc, fieldNames, modifier) {
            return true;
        },
        remove: function (userId, doc) {
            return true;

        }
    };
    var denyNone = {
        insert: function (userId, doc) {
            return false;
        },
        update: function (userId, doc, fieldNames, modifier) {
            return false;
        },
        remove: function (userId, doc) {
            return false;
        }
    };
    var denyAll = {
        insert: function (userId, doc) {
            return true;
        },
        update: function (userId, doc, fieldNames, modifier) {
            return true;
        },
        remove: function (userId, doc) {
            return true;
        }
    };

    /**
     * Offer Settings collection
     */
    Meteor.publish('settings', function (searchCriteria, options) {
        console.log('publishing setting groups');
        searchCriteria = searchCriteria || {};
        options = options || {};
        if (!!options.fields) {
            options.fields['_id'] = 1;
            //options.fields['_deviceId'] = 1;
            options.fields['_groupName'] = 1;
            options.fields['_groupIndex'] = 1;
            options.fields['_groupPath'] = 1;
        }

        if (!this.userId) {
            // if the users not logged in they don't get any settings groups
            return [];
        }
        // once their logged in show them everything
        return Settings.find(searchCriteria, options);

    });
    Settings.allow(allowAll);
    Settings.deny(denyNone);


    /**
     * Offer State collection
     */

    Meteor.publish('state', function (searchCriteria, options) {
        console.log('publishing state groups');
        searchCriteria = searchCriteria || {};
        options = options || {};
        if (!!options.fields) {
            options.fields['_id'] = 1;
            options.fields['_groupName'] = 1;
            options.fields['_groupIndex'] = 1;
            options.fields['_groupPath'] = 1;
        }

        if (!this.userId) {
            // if the users not logged in they are restricted to public state groups
            searchCriteria = {
                $and: [
                    {_groupName: {$in: ['system']}},
                    searchCriteria
                ]
            };
            return State.find(searchCriteria, options);
        }
        // once their logged in show them everything
        return State.find(searchCriteria, options);

    });
    State.allow(allowAll);
    State.deny(denyAll);


    /**
     * Offer Files collection
     */

    Meteor.publish('files', function (searchCriteria, options) {
        console.log('publishing files');
        searchCriteria = searchCriteria || {};
        options = options || {};
        if (!options.sort) {
            // Default sorting options
            options.sort = {path: 1, name: 1};
        }

        if (!this.userId) {
            // you don't get anything till your logged in
            return [];
        }
        // once their logged in show them everything
        return Files.find(searchCriteria, options);

    });
    Files.allow(allowAll);
    Files.deny({
        insert: function (userId, doc) {
            if (!userId ||            // user must be logged in
                doc.type === 'file') {  // can't upload file directly (must be done as http upload)
                // TODO: additional validation as required...
                return true;
            }
            return false;
        },
        update: function (userId, doc, fieldNames, modifier) {
            return true; // don't allow remote updates
        },
        remove: function (userId, doc) {
            // TODO: validate the file being removed is allowed and add a hook to delete the underlying file upon removal
            return !userId;
        }
    });

    /**
     * Offer Users collection
     */
        // Configure accounts system
    Accounts.config({
        sendVerificationEmail: false,
        forbidClientAccountCreation: true
    });

    // Autoprime some users if not already done
    if (Meteor.users.find().count()==0) {
        console.log('creating some users');
        var uid = Accounts.createUser({username: 'admin', email: 'joeadmin@digi.com', password:'admin',profile: {name:'joe admin'}});
        console.log('joe admin created: ',uid);
        uid = Accounts.createUser({username: 'user', email: 'joeuser@digi.com', password:'user',profile: {name:'joe user'}});
        console.log('joe user created: ',uid);
    }
};


/**
 * server functions that can be invoked from the client using Meteor.call
 */
Meteor.methods({
    setLogLevel: function (level) {
        console.log('setting log level');
        //Logger.setLevel(level);
        return level;
    },
    setComponentLogLevel: function (component, level) {
        //Logger.setLevel(component, level);
    }
});

Meteor.onConnection(function(connection) {
    //TODO: log as debug?
    //console.log("** New connection");
    //connection.onClose(function() {
    //    console.log("!!!! connection closed");
    //})
});


/**
 * Autoprime some data into the collections
 * TODO: eventually this should come from the device itself. Currently we are loading from
 * mock data files
 */
//function _autoprimeCollections() {
//    let deviceProps = {deviceId: 'FF0-111-AEF'};
//    // wipe out and reload the gateway settings
//    Settings.remove({});
//    if (Settings.find().count()==0) {
//        loadRciGroupData(transportSettings, Settings, deviceProps);
//    }
//
//    // wipe out and reload the gateway state
//    State.remove({});
//    if (State.find().count()==0) {
//        loadRciGroupData(transportState, State, deviceProps);
//    }
//
//    // Autoprime some directories if not already done
//    Files.remove({});
//    if (Files.find().count()==0) {
//        loadFiles(Files, deviceProps);
//    }
//
//    // Autoprime some users if not already done
//    if (Meteor.users.find().count()==0) {
//        console.log('creating some users');
//        var uid = Accounts.createUser({username: 'admin', email: 'joeadmin@digi.com', password:'admin',profile: {name:'joe admin'}});
//        console.log('joe admin created: ',uid);
//        uid = Accounts.createUser({username: 'user', email: 'joeuser@digi.com', password:'user',profile: {name:'joe user'}});
//        console.log('joe user created: ',uid);
//    }
//}

/** Convert a json value from string to a native type based on its value **/
function nativeTypes(key,value) {
    if (typeof(value)==="string" && value!=="") {
        if (_.isFinite(value)) {
            return Number(value);
        } else {
            let lvalue = value.toLowerCase();
            if (lvalue ==='false') {
                return false
            } else if (lvalue === 'true') {
                return true
            }
        }
    }
    return value;
}

/** Traverses a json object and applies the supplied function against each element **/
function _traverse(o,func) {
    for (var i in o) {
        o[i] = func.apply(this,[i,o[i]]);
        if (o[i] !== null && typeof(o[i])=="object") {
            //going on step down in the object tree!!
            _traverse(o[i],func);
        }
    }
}

/** cleans up a DC json settings object to be more suitable for processing */
function _normalizeJsonData(doc) {
    _traverse(doc,nativeTypes);
}

/**
 * Parse RCI XML data (including RCI or SCI) and return a javascript object.
 * This simply calls xml2js.parseString, but provides consistent options for parsing any RCI/SCI data. It
 * also handles wrapping the call via Meteor.wrapAsync.
 *
 * @param xmldata The XML string.
 * @return javascript object of parsed data.
 */
function parseRciXml(xmldata) {
    let parseStringSync = Meteor.wrapAsync(xml2js.parseString);
    let rciObj = parseStringSync(xmldata, {explicitArray:false, attrkey:'@'});
    return rciObj;
}

/**
 * Load RCI settings or state data into a collection. Each RCI group is loaded as a separate object in the colleciton.
 * @param collection Collection to load data into
 * @param rciObj Javascript object representing  RCI data to loa (typically parsed XML via parseRciXml)d
 * @param additionalProps Additional properties to add each group object before loading (e.g. {deviceId: '123'})
 */
function loadRciGroupData(collection, rciObj, additionalProps) {
    additionalProps = additionalProps || {};

    _normalizeJsonData(rciObj);

    var groupNames = _.keys(rciObj);
    for (var groupNumber=0; groupNumber<groupNames.length; groupNumber++) {
        var groupName = groupNames[groupNumber];
        var group = rciObj[groupName];
        var inst;
        //console.log('saving group: '+groupName, group);
        if (_.isArray(group)) {
            for (index=0; index < group.length; index++) {
                inst = group[index];
                //inst['_deviceId'] = null;
                inst['_groupName'] = groupName;
                inst['_groupIndex'] = index;
                inst['_groupPath'] = groupName+'/'+index;
                _.extend(inst, additionalProps);
                collection.insert(inst);
            }
        } else {
            inst = group;
            var index=0;
            //inst['_deviceId'] = null;
            inst['_groupName'] = groupName;
            inst['_groupIndex'] = index;
            inst['_groupPath'] = groupName+'/'+index;
            _.extend(inst, additionalProps);
            collection.insert(inst);
        }
    }
}

function loadFiles(filesCollection, additionalProps) {
    var files= [
        {
            path: '/',
            name: 'web',
            type: 'directory'
        },
        {
            path: '/web/',
            name: 'img',
            type: 'directory'
        },
        {
            path: '/web/',
            name: 'data',
            type: 'directory'
        },
        {
            path: '/',
            name: 'firmware',
            type: 'directory'
        }
    ];

    for (var i=0; i<files.length; i++) {
        files[i].fileSpec = files[i].path+files[i].name;
        _.extend(files[i], additionalProps);
        filesCollection.insert(files[i]);
    }
}

export const UrmaServer = {
    startup: startup,
    parseRciXml: parseRciXml,
    loadRci: loadRciGroupData,
    loadFiles: loadFiles
};
