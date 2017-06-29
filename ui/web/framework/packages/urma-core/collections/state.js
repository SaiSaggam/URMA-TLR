/**
 *  Collection of  State. One entry per state group.
 *  Each entry has the follow implicit fields in addition to the custom fields
 *  of that group
 *    _id
 *    _deviceId
 *    _groupName
 *    _groupIndex
 *    _groupPath
 */
const State = new Mongo.Collection('state');

export default State;
