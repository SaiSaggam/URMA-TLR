import json
import xmltodict
import hashlib
import time
import datetime
import pprint
import jmespath
import urllib
import logging
import collections
log = logging.getLogger('ui.'+__name__)
from jpath import jexp, jqry
from ddp import DDP, DDPConnection, Provider, SimpleProvider, LoginTokens


class GroupProvider(SimpleProvider):
  """ An in-memory group oriented collection provider thatis a basis for settings/state providers """

  def __init__(self, collectionName, deviceConfigManager, shared=True, ordered=False):
    SimpleProvider.__init__(self, collectionName, shared, ordered)
    self.deviceConfigManager = deviceConfigManager
    log.info('Registering handler for %s collection', collectionName)
    deviceConfigManager.addHandler(self._groupHandler, self)

  def getItems(self, subscription):
    subParams = subscription.subParams
    log.debug('getItems from %s subParams=%s', self.collectionName, str(subParams))
    find = {}
    options = {}
    if len(subParams)>0:
        # only grab results that match
        find = subParams[0]
    if len(subParams)>1:
        # only grab results that match
        options = subParams[1]
        if 'fields' in options:
            options['fields']['_id'] = 1
            options['fields']['_groupName'] = 1
            options['fields']['_groupIndex'] = 1
            options['fields']['_groupPath'] = 1

    query = jqry(find, options)
    # log.debug(' - jmespath search query:%s\n  over items:%s', query, str(self.items.values()))
    results = jmespath.search(query, self.items.values())
    # log.debug(' - returning items: %s', json.dumps(results, indent=4))
    return results

  def _groupHandler(self, configManagerData):
      dataType = configManagerData.keys()[0]
      if dataType==self.collectionName:
          log.debug('Recieved %s data from device.', dataType)
          _loadGroupData(configManagerData, self)


#
# Settings Provider
#
class SettingsProvider(GroupProvider):
  """ A provider for device settings """
  def __init__(self, deviceConfigManager, userCollection):
    GroupProvider.__init__(self, 'settings', deviceConfigManager)
    self.userCollection = userCollection

  def insert(self, doc):
    # do the normal thing
    result = GroupProvider.insert(self, doc)
    # if its a user group then insert it to the user collection as well
    if doc.get('_groupName')=='user':
        # id of a user item is the group path (username can be changed)
        id = hashlib.sha256(doc.get('_groupPath')).hexdigest()
        username = doc.get('name')
        user = {
            '_id': id,
            'username': username,
            'createdAt': LoginTokens._toEpochTime(datetime.datetime.utcnow()),
            'profile': {
                'name': username,
                'access': doc.get('access')
                },
            'services': None
        }
        self.userCollection.insert(user)
    return result

  def update(self, docId, changedFields, clearedFieldNames):
    # do the normal thing
    result = GroupProvider.update(self, docId, changedFields, clearedFieldNames)
    # if its a user group then update the user collection as well
    if doc.get('_groupName')=='user':
        # id of a user item is the group path (username can be changed)
        id = hashlib.sha256(doc.get('_groupPath')).hexdigest()
        # rather than try to compute the changed fields in the user record
        # we'll just recreate the entire record and send it as an upsert
        doc = self.getItem(docId)
        username = doc.get('name')
        user = {
            '_id': id,
            'username': username,
            'createdAt': LoginTokens._toEpochTime(datetime.datetime.utcnow()),
            'profile': {
                'name': username,
                'access': doc.get('access')
                },
            'services': None
        }
        self.userCollection.upsert(user)
    return result


#
# State Provider
#
class StateProvider(GroupProvider):
  """ A provider for device state """
  def __init__(self, deviceConfigManager):
    deviceConfigManager.addHandler(self.availableImagesHandler, self)
    GroupProvider.__init__(self, 'state', deviceConfigManager)
    self.polls = {}

  def subscribe(self, subscription):
      super(StateProvider, self).subscribe(subscription)
      # setup polling as requested
      groupName = subscription.subParams[0]['_groupName']
      rate = subscription.subParams[1]['refreshInterval'] if len(subscription.subParams)>1 and 'refreshInterval' in subscription.subParams[1] else 60
      # if no instance is specified pass in zero to indicate all instances
      instance = subscription.subParams[0]['_groupIndex'] if '_groupIndex' in subscription.subParams[0] else 0
      pollId = self.deviceConfigManager.poll(groupName, instance, rate)
      log.debug('subscriptionId %s to %s/%s[%d] established pollId %d at rate of %d', subscription.getId(), self.collectionName, groupName, instance, pollId, rate)
      #save off the pollid with this subscription so that we can cancel it on the unsubscribe call
      self.polls[subscription.getId()] = pollId

  def unsubscribe(self, subscription):
      super(StateProvider, self).unsubscribe(subscription)
      # cancel any outstanding polling
      pollId = self.polls.get(subscription.getId())
      if pollId!=None:
          log.debug('unsubscription of %s subId %s cancelled pollId %d', self.collectionName,  subscription.getId(), pollId)
          print 'cancelling pollId ', pollId
          self.deviceConfigManager.cancel_poll(pollId)
          del self.polls[subscription.getId()]

  # custom decoder for firmware_files.available_images field which contains url encoded xml content
  # TODO: when TLR adds nested fields, change this field to one and then get rid
  #       of this custom handler
  def availableImagesHandler(self, configManagerData):
      # if firmware_files group was passed and we got a string for available_images, then decode
      # and parse the string and replace it with the parsed dict
      query = 'state.firmware_files'
      firmware_files = jmespath.search(query, configManagerData)
      if firmware_files!=None:
          log.debug('processing firmware_files group')
          files = firmware_files.get('available_images','')
          if isinstance(files, basestring):
              log.debug('decoding available_images: %s', files)
              filesxml = '<available_images>{}</available_images>'.format(urllib.unquote(files).decode('utf8'))
              #print ' xml: ', filesxml
              files = xmltodict.parse(filesxml)
              #print ' json: ', json.dumps(files['available_images'], indent=4)
              available_images = files['available_images']
              # available_images.file is parsed into an OrderedDict. If there is only a single item it
              # ends up getting translated into a dictionary instead of an array. Convert to array.
              if not isinstance(available_images['file'], collections.Sequence):
                  available_images['file'] = [available_images['file']]
              firmware_files['available_images'] = available_images
              log.debug('firmware_files: %s', json.dumps(firmware_files, indent=4))
              #print ' found available_images', firmware_files['available_images']
          log.debug('returning available_images: %s', json.dumps(firmware_files['available_images'], indent=4))


# user collection provider - based off user groups in settings data.
#    as settings collections are loaded, the settings parser will
#    detect and insert user records into this collection
#
# User item format:
# {
#     '_id': hashlib.sha256(username).hexdigest(),
#     'username': 'admin',
#     'createdAt': ms_since_epoch,
#     'profile': {
#         'name': admin,
#         'access': access_level
#         },
#     'services': None
# }
class UserProvider(SimpleProvider):
  def __init__(self):
    SimpleProvider.__init__(self, 'users', True, False, True)

  def findUser(self, username):
    log.debug('find user %s', username)
    results = []
    if username:
        query = '[?username==`%s`]'%username
        log.debug(' - jmespath search query:%s\n  over items:%s', query, str(self.items.values()))
        results = jmespath.search(query, self.items.values())
        log.debug(' - returning users: %s', json.dumps(results, indent=4))
    return results[0] if len(results)>0 else None

  def allowPublish(self, ddpSession, doc):
    return ddpSession.getUserName() and doc['username']==ddpSession.getUserName()


#
# Internal helper functions
#


# Convert a value from string to a native type based on its value
def nativeTypes(key,value):
    #print 'converting field '+str(key)+'('+str(type(value))+'): '+str(value)
    try:
        value = int(value)
        return value
    except:
        pass
    try:
        value = float(value)
        return value
    except:
        pass
    try:
        lvalue = value.lower()
        if lvalue=='false':
            return False
        elif lvalue=='true':
            return True
    except:
        pass
    return value



# Traverses an object and applies the supplied function against each element
def traverse(o,func):
    if isinstance(o,dict):
        for key in o:
            skey = str(key)
            o[skey] = func(skey, o[skey])
            traverse(o[skey], func)
    elif isinstance(o,list):
        for i in range(len(o)):
            o[i] = func(None, o[i])
            traverse(o[i], func)


# cleans up a DC xml settings object to be more suitable for processing
def _normalizeXmlData(doc):
    traverse(doc,nativeTypes)

# Default rci group index
DEFAULT_INDEX = 1

# loads the given xml group data into the specified collection - assumed to be settings or state xml data
def _loadGroupData(xmldata, collection):
    log.debug('loading group data')
    _normalizeXmlData(xmldata)
    # log.debug('normalized data: %s', json.dumps(xmldata, indent=4))
    topLevelGroup = xmldata.keys()[0]
    groupNames = xmldata[topLevelGroup].keys()
    for groupNumber in range(len(groupNames)):
        groupName = groupNames[groupNumber]
        group = xmldata[topLevelGroup][groupName]
        # log.debug('loading group %s ', groupName)
        if isinstance(group, list):
            for i in range(len(group)):
                inst = group[i]
                index = inst.get('@index', 1) - 1
                #inst['_deviceId'] = None
                inst['_groupName'] = groupName
                inst['_groupIndex'] = index
                inst['_groupPath'] = groupName+'/'+str(index)
                inst['_id'] = hashlib.md5(topLevelGroup+'/'+inst['_groupPath']).hexdigest()
                log.debug('  upsert: %s', json.dumps(inst))
                collection.upsert(inst)
        else:
            inst = group
            index = inst.get('@index', 1) - 1
            #inst['_deviceId'] = None
            inst['_groupName'] = groupName
            inst['_groupIndex'] = index
            inst['_groupPath'] = groupName+'/'+str(index)
            inst['_id'] = hashlib.md5(topLevelGroup+'/'+inst['_groupPath']).hexdigest()
            log.debug('  upsert: %s', json.dumps(inst))
            collection.upsert(inst)
    # log.debug('  resulting collection:%s', json.dumps(collection.items, indent=4))
