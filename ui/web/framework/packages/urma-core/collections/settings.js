/**
 *  Collection of  Configuration Settings. One entry per settings group.
 *  Each entry has the follow implicit fields in addition to the custom fields
 *  of that group
 *    _id
 *    _deviceId
 *    _groupName
 *    _groupIndex
 *    _groupPath
 *
 *  Subscription examples:
 *   fetch all fields of all ethernet groups
 *     meteor.subscribe('settings', {_groupName: 'ethernet'})
 *   fetch just certain fields for all ethernet groups
 *     meteor.subscribe('settings', {_groupName: 'ethernet'}, {fields: {IPaddr:1, DNSserver:1, descr:1}})
 *   fetch just _ fields for all ethernet groups
 *     meteor.subscribe('settings', {_groupName: 'ethernet'}, {fields: {}})
 *   fetch all fields of just one ethernet group
 *     meteor.subscribe('settings', {_groupName: 'ethernet',_groupIndex:2})
 *   fetch just certain fields of just one ethernet group
 *     meteor.subscribe('settings', {_groupName: 'ethernet',_groupIndex:2}, {fields: {IPaddr:1, DNSserver:1, descr:1}})
 *   fetch various fields of various groups
 *     meteor.subscribe('settings', {_groupName: {$in: ['ethernet','serial']}}, {fields: {}})
 *
 *  Once subscribed, you can perform a mongo query against the collection and filter
 *  results by these one or more of these field values. for example:
 *     settings.find({mask: "255.255.255.0"})
 */
const Settings = new Mongo.Collection('settings');

export default Settings;

