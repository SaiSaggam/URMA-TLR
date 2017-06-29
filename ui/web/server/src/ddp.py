import os
import json
import time
import datetime
import jmespath
import hashlib
from copy import deepcopy
import logging
log = logging.getLogger('ui.'+__name__)


#
# Public interfaces and API's
#
class DDPConnection (object):
  """ All DDP connection implementations must implement this minimum set of methods """
  def getSessionId(self):
    pass
  def send(self, msg):
    pass

class DDPSession (object):
  """ DDPconnection implementations can expect a DDPSession to support these methods """
  def __init__(self):
    self.userId = None
    self.userName = None
  def getSessionId(self):
    pass
  def on_message(self, data):
    pass
  def close(self):
    pass
  def setUser(self, userId, userName):
    log.info('setting user in session %s to %s with userId %s ', self.getSessionId(), userName, userId)
    self.userId = userId
    self.userName = userName
  def getUserId(self):
    return self.userId
  def getUserName(self):
    return self.userName


#
# Implementation
#
class DDPSessionImpl(DDPSession):
  def __init__(self, ddpConnection):
    DDPSession.__init__(self)
    self.ddpConnection = ddpConnection
    self.sessionId = ddpConnection.getSessionId()
    DDP.open(self)
    # auto subscribe to users collections
    log.debug('auto-subscribe to users')
    DDP.subscribe(self, 'users', 'users', [])

  def getSessionId(self):
    return self.sessionId

  def on_message(self, data):
    """ Parse and route incoming DDP protocol messages """
    msg = json.loads(data)
    msg_type = msg.get('msg')
    log.debug('--> ['+self.getSessionId()+'] '+data)

    if msg_type == 'connect':
      self.handle_connect(msg)
    elif msg_type == 'ping':
      self.handle_ping(msg)
    elif msg_type == 'sub':
      self.handle_sub(msg)
    elif msg_type == 'unsub':
      self.handle_unsub(msg)
    elif msg_type == 'method':
      self.handle_method(msg)

  def close(self, code=1000, msg='Normal closure'):
    log.debug('closing ddpSession %s', self.sessionId)
    DDP.unsubscribe(self, 'users')
    DDP.close(self)
    self.ddpConnection.close(code, msg)

  def sendMsg(self, msg):
    log.debug('<-- ['+self.getSessionId()+'] '+msg)
    self.ddpConnection.send(msg)

  def handle_connect(self, msg):
    _clientSessionName = msg.get('session')
    _clientVersion = msg.get('version')
    _clientSupport = msg.get('support')
    DDP.connect(self, _clientSessionName, _clientVersion, _clientSupport)

  def handle_ping(self, msg):
    rsp = '{"msg":"pong"}'
    if msg.has_key('id'):
      rsp = '{"msg":"pong","id":"'+msg['id']+'"}'
    self.sendMsg(rsp)

  def handle_method(self, msg):
    _method = msg.get('method')
    _params = msg.get('params')
    _clientMethodId = msg.get('id')
    _randomSeed = msg.get('randomSeed')
    DDP.call(self, _method, _params, _clientMethodId, _randomSeed)

  def handle_sub(self, msg):
    _clientSubId = msg.get('id')
    _collectionName = msg.get('name')
    _subParams = msg.get('params')
    DDP.subscribe(self, _clientSubId, _collectionName, _subParams)

  def handle_unsub(self, msg):
    _clientSubId = msg.get('id')
    DDP.unsubscribe(self, _clientSubId)

  def send_error(self, text):
    self.sendMsg(json.dumps({'msg': 'error', 'text': text}))

  #
  # DDPSessionListener methods
  #

  def announce(self, serverId):
    rsp = {}
    rsp['server_id'] = serverId
    self.sendMsg(json.dumps(rsp))

  def connected(self, clientSessionName):
    rsp = {}
    rsp['msg'] = 'connected'
    rsp['session'] = clientSessionName
    self.sendMsg(json.dumps(rsp))

  def failed(self, clientVersion):
    rsp = {}
    rsp['msg'] = 'failed'
    rsp['version'] = clientVersion
    self.sendMsg(json.dumps(rsp))

  def nosub(self, clientSubId, error):
    rsp = {}
    rsp['msg'] = 'nosub'
    rsp['id'] = clientSubId
    if error != None:
      rsp['error'] = error
    self.sendMsg(json.dumps(rsp))

  def ready(self, clientSubIds):
    rsp = {}
    rsp['msg'] = 'ready'
    rsp['subs'] = clientSubIds
    self.sendMsg(json.dumps(rsp))

  # TODO: This is a temporary hack to remove any password fields from results. Eventually we will
  # use the descriptor and the users access level to determine if fields should be published out
  # to the client. But till we get the descriptor we will just manually remove any password fields
  # before sending results to the client
  def _cleanFields(self, collectionName, obj):
    bannedFields = ['password']
    result = obj
    if any(field in obj for field in bannedFields):
        result = deepcopy(obj)
        for field in bannedFields:
            if field in result:
                del result[field]
        log.info('stripped firmware_version before sending to client. Before: %s  After: %s', json.dumps(obj), json.dumps(result))
    return result

  def added(self, collectionName, itemId, fields):
    rsp = {}
    rsp['msg'] = 'added'
    rsp['collection'] = collectionName
    rsp['id'] = itemId
    if fields != None:
      rsp['fields'] = self._cleanFields(collectionName, fields)
    self.sendMsg(json.dumps(rsp))

  def changed(self, collectionName, itemId, fields, cleared=None):
    rsp = {}
    rsp['msg'] = 'changed'
    rsp['collection'] = collectionName
    rsp['id'] = itemId
    if fields != None:
      rsp['fields'] = self._cleanFields(collectionName, fields)
    if cleared != None:
      rsp['cleared'] = cleared
    self.sendMsg(json.dumps(rsp))

  def removed(self, collectionName, itemId):
    rsp = {}
    rsp['msg'] = 'removed'
    rsp['collection'] = collectionName
    rsp['id'] = itemId
    self.sendMsg(json.dumps(rsp))

  def addedBefore(self, collectionName, itemId, fields, beforeId):
    rsp = {}
    rsp['msg'] = 'addedBefore'
    rsp['collection'] = collectionName
    rsp['id'] = itemId
    if fields != None:
      rsp['fields'] = self._cleanFields(collectionName, fields)
    rsp['before'] = beforeId
    self.sendMsg(json.dumps(rsp))

  def movedBefore(self, collectionName, itemId, beforeId):
    rsp = {}
    rsp['msg'] = 'movedBefore'
    rsp['collection'] = collectionName
    rsp['id'] = itemId
    rsp['before'] = beforeId
    self.sendMsg(json.dumps(rsp))

  def result(self, clientMethodCallId, error, _result):
    rsp = {}
    rsp['msg'] = 'result'
    rsp['id'] = clientMethodCallId
    if error!=None:
      rsp['error'] = error
    if _result!=None:
      rsp['result'] = _result
    self.sendMsg(json.dumps(rsp))

  def updated(self, clientMethodCallIds):
    rsp = {}
    rsp['msg'] = 'updated'
    rsp['methods'] = clientMethodCallIds
    self.sendMsg(json.dumps(rsp))


class Subscription(object):
  """ A ddpSession's subscription to a server collection """
  def __init__(self, ddpSession, clientSubId, collectionName, provider, subParams):
    self.ddpSession = ddpSession
    self.clientSubId = clientSubId
    self.collectionName = collectionName
    self.provider = provider
    self.subParams = subParams

  def getId(self):
    return self.ddpSession.getSessionId()+'_'+self.clientSubId


class DDP:
  # Map of callable DDP methods. Key: methodName, Value: registered callback func
  methods = {}
  # Map of collections. Key: collectionName, Value: collection provider instance
  collections = {}
  # Map of DDP client sessions. Key: sessionId, Value: DDPSession
  sessions = {}
  # Map of subscriptions. Key: collectionName, Value: [] of Subscriptions
  subscriptions = {}
  serverId = 1 #TODO pick something maybe based on server ip or something

  # Server API's
  @staticmethod
  def registerMethod(methodName, provider, handlerFunc):
    """ Register a method that can later be called by clients """
    DDP.methods[methodName] = {
      'provider': provider,
      'handlerFunc': handlerFunc
    }

  @staticmethod
  def registerCollection(provider):
    """ Register a collection and its associated provider that can later be subscribed to by clients """
    collectionName = provider.getCollectionName()
    DDP.collections[collectionName] = provider
    if hasattr(provider,'clientInsert'):
      DDP.registerMethod('/'+collectionName+'/insert', provider, getattr(provider,'clientInsert'))
    if hasattr(provider,'clientUpdate'):
      DDP.registerMethod('/'+collectionName+'/update', provider, getattr(provider,'clientUpdate'))
    if hasattr(provider,'clientRemove'):
      DDP.registerMethod('/'+collectionName+'/remove', provider, getattr(provider,'clientRemove'))
    return provider

  # Session API's
  @staticmethod
  def open(ddpSession):
    """ registers a new DDPSession and announces the service to that session """
    DDP.sessions[ddpSession.getSessionId()] = ddpSession
    ddpSession.announce(DDP.serverId)

  @staticmethod
  def close(ddpSession):
    """ disconnect a terminated session """
    # cleanup all outstanding subscriptions
    DDP.unsubscribe(ddpSession, None)
    # TODO: consider expiring sessions on a timer to give them a chance to reconnect
    #       due to temporary network issues.
    del DDP.sessions[ddpSession.getSessionId()]

  @staticmethod
  def connect(ddpSession, clientSessionName, clientVersion, clientSupport):
    """ client connect/reconnect request """
    supportedVersion = "1"
    if clientSessionName != None:
      # TODO: restore old session??
      pass
    else:
      # start new session - default to sessionId for name
      clientSessionName = ddpSession.getSessionId()
    if clientVersion == supportedVersion:
      ddpSession.connected(clientSessionName)
    else:
      ddpSession.failed(supportedVersion)

  @staticmethod
  def call(ddpSession, methodName, params, clientMethodCallId, randomSeed):
    """ client request to invoke a registered server method """
    method = DDP.methods.get(methodName)
    if method==None:
      error = 'no such method available'
    else:
      context = {"ddpSession": ddpSession, "methodName": methodName, "clientMethodCallId": clientMethodCallId, "randomSeed":randomSeed}
      provider = method.get('provider')
      handlerFunc = method.get('handlerFunc')
      if provider==None:
        result = handlerFunc(params, context)
      else:
        result = handlerFunc(params, context)
      ddpSession.updated([clientMethodCallId])
      ddpSession.result(clientMethodCallId, result.get('error'), result.get('result'))

  @staticmethod
  def subscribe(ddpSession, clientSubId, collectionName, subParams):
    """ subscribes client to notification of current and future collection content changes """
    provider = DDP.collections.get(collectionName)
    if provider == None:
      log.error('no collection named '+collectionName+' is available')
      return None
    # create the subscription and add it to the map of known collection subscriptions
    subscription = Subscription(ddpSession, clientSubId, collectionName, provider, subParams)
    log.debug('DDP.subscribe session=%s, clientSubId=%s, subId=%s', ddpSession.getSessionId(), clientSubId, subscription.getId())
    if DDP.subscriptions.get(collectionName) == None:
      DDP.subscriptions[collectionName] = [subscription]
    else:
      DDP.subscriptions[collectionName].append(subscription)
    # notify provider of new subscribing session
    provider.subscribe(subscription)
    return subscription

  @staticmethod
  def unsubscribe(ddpSession, clientSubId):
    """ cancels client notification of future collection changes """
    log.debug('DDP.unsubscribe session=%s, clientSubId=%s', ddpSession.getSessionId(), clientSubId)
    # find the subscription with matching ddpSession and clientSubId(if passed)
    for collectionName in DDP.subscriptions:
      collectionSubs = DDP.subscriptions[collectionName]
      removedSubs = []
      for sub in collectionSubs:
        if sub.ddpSession==ddpSession and (clientSubId==None or sub.clientSubId==clientSubId):
          # notify provider and mark subscription for removal
          sub.provider.unsubscribe(sub)
          removedSubs.append(sub)
      # once done interating, safely remove from list
      for sub in removedSubs:
          DDP.subscriptions[sub.collectionName].remove(sub)

  @staticmethod
  def notifyAllSubscriptionsReady(ddpSession):
    """ marks all current (non internal) subscriptions as ready """
    sessionSubIds = []
    for collectionName in DDP.subscriptions:
      collectionSubs = DDP.subscriptions[collectionName]
      for sub in collectionSubs:
        # log.debug('notifyAllSubscriptionsReady examining: collection=%s, sub=%s, ddpSession=%s', str(collectionName), str(sub), str(ddpSession))
        if sub.ddpSession==ddpSession and not sub.provider.internalSubscription:
          sessionSubIds.append(sub.clientSubId);
    ddpSession.ready(sessionSubIds)


class Provider(object):
  """ All collection providers must implement this interface """

  def __init__(self, collectionName, internalSubscription=False):
    self.collectionName = collectionName
    self.subscriptions = []
    self.internalSubscription = internalSubscription

  def getCollectionName(self):
    return self.collectionName

  def getItems(self, subscription):
    """ Subclass will return all items that match requested criteria"""
    pass

  def subscribe(self, subscription):
    if subscription.collectionName!=self.collectionName:
      #TODO throw error - subscribing to wrong provider
      log.error('Error: Subscribing to provider '+self.collectionName+' but requesting collection '+collectionName)
      return
    # record the subscription
    self.subscriptions.append(subscription)
    # notify the subscriber of items they are allowed to see
    for i in self.getItems(subscription):
      if self.allowPublish(subscription.ddpSession, i):
        subscription.ddpSession.added(self.collectionName, i.get('_id'), i)
    if not self.internalSubscription:
        subscription.ddpSession.ready([subscription.clientSubId])

  def unsubscribe(self, subscription):
    self.subscriptions.remove(subscription)


# a["{\"msg\":\"added\",\"collection\":\"likes\",\"id\":\"yHz6YtnxPrQcX5JsD\",\"fields\":{\"name\":\"Joey\",\"description\":\"I love it!\"}}"]
  def notifyAdded(self, doc, session=None):
    """ Notify session(s) with access of changes """
    if session:
        if self.allowPublish(session, doc):
            session.added(self.collectionName, doc['_id'], doc)
    else:
        notified = []
        for sub in self.subscriptions:
          if sub.ddpSession not in notified and self.allowPublish(sub.ddpSession, doc):
            sub.ddpSession.added(self.collectionName, doc['_id'], doc)
            notified.append(sub.ddpSession)

# a["{\"msg\":\"changed\",\"collection\":\"likes\",\"id\":\"yHz6YtnxPrQcX5JsD\",\"fields\":{\"description\":\"I love it! even more!\"}}"]
  def notifyChanged(self, docId, changedFields, clearedFields, session=None):
    """ Notify session(s) with access of changes """
    if session:
        if self.allowPublish(session, doc):
            session.changed(self.collectionName, docId, changedFields, clearedFields)
    else:
        notified = []
        for sub in self.subscriptions:
          if sub.ddpSession not in notified and self.allowPublish(sub.ddpSession, self.getItem(docId)):
            sub.ddpSession.changed(self.collectionName, docId, changedFields, clearedFields)
            notified.append(sub.ddpSession)

# a["{\"msg\":\"removed\",\"collection\":\"likes\",\"id\":\"yHz6YtnxPrQcX5JsD\"}"]
  def notifyRemoved(self, docId, session=None):
    """ Notify session(s) with access of changes """
    if session:
        if self.allowPublish(session, self.getItem(docId)):
            session.removed(self.collectionName, docId)
    else:
        notified = []
        for sub in self.subscriptions:
          if sub.ddpSession not in notified and self.allowPublish(sub.ddpSession, self.getItem(docId)):
            sub.ddpSession.removed(self.collectionName, docId)
            notified.append(sub.ddpSession)

# example collection calls
# ["{\"msg\":\"method\",\"method\":\"/likes/insert\",\"params\":[{\"name\":\"Joey\",\"description\":\"I love it!\"}],\"id\":\"1\",\"randomSeed\":\"5c7d6a496069324c61b0\"}"]
# ["{\"msg\":\"method\",\"method\":\"/likes/update\",\"params\":[{\"_id\":\"sJrgqvwFZ237sqJbN\"},{\"$set\":{\"description\":\"I love it! even more!\"}},{}],\"id\":\"2\"}"]
# ["{\"msg\":\"method\",\"method\":\"/likes/remove\",\"params\":[{\"_id\":\"eohPvzAoczQdvBhDa\"}],\"id\":\"1\"}"]

  def clientInsert(self, params, context):
    # Verify caller has insert permission
    # collectionName = context['collectionName']
    ddpSession = context['ddpSession']
    doc = params[0]
    doc['owner'] = ddpSession.getUserId()
    if self.allowInsert(ddpSession, doc):
      return self.insert(doc)

  def clientUpdate(self, params, context):
    # Verify caller has insert permission
    # collectionName = context['collectionName']
    ddpSession = context['ddpSession']
    docId = params[0]
    operation = params[1]
    doc = self.getItem(docId['_id'])
    # doc = self.items[docId]
    if self.allowUpdate(ddpSession, operation, doc):
      return self.update(operation, doc)

  def clientRemove(self, params, context):
    # Verify caller has insert permission
    # collectionName = context['collectionName']
    ddpSession = context['ddpSession']
    docId = params[0]
    doc = self.getItem(docId['_id'])
    # doc = self.items[docId]
    if self.allowRemove(ddpSession, doc):
      return self.remove(doc)





class SimpleProvider(Provider):
  """ A simple non-persistent, in-memory collection provider """
  nextId = 0

  def __init__(self, collectionName, shared=False, ordered=False, internalSubscription=False):
    Provider.__init__(self, collectionName, internalSubscription)
    self.items = {}
    self.shared = shared
    self.ordered = ordered

  def getItems(self, subscription):
    # TODO: scope collection results further using subParams as needed
    return self.items.values()

  def getItem(self, docId):
    return self.items.get(docId)

  def allowPublish(self, ddpSession, doc):
    return self.shared or doc['owner']==ddpSession.getUserId()

  def allowInsert(self, ddpSession, doc):
    # TODO: Check user write permissions
    return True

  def allowUpdate(self, ddpSession, operation, doc):
    # TODO: Check user write permissions
    return self.shared or  doc['owner']==ddpSession.getUserId()

  def allowRemove(self, ddpSession, doc):
    # TODO: Check user write permissions
    return self.shared or  doc['owner']==ddpSession.getUserId()

  def upsert(self, doc):
      # if item is new, insert it into collection. if it already exists,
      # update the changed fields appropriately
      docId = doc.get('_id')
      oldDoc = self.items[docId] if docId in self.items else None
      if oldDoc!=None:
          changed = {}
          cleared = []
          for field in doc.keys():
              if oldDoc.get(field)!=doc[field]:
                changed[field] = doc[field]
          for field in oldDoc.keys():
              if doc.get(field)==None:
                cleared.append(field)
          return self.update(docId, changed, cleared)
      return self.insert(doc)

  def insert(self, doc):
      # build item to insert into collection (assign id and associate owner)
      docId = doc.get('_id')
      if docId==None:
        log.warning('inserted %s doc lacks required "_id", autoassigning one: %s _groupPath=%s', self.collectionName, str(SimpleProvider.nextId), doc['_groupPath'])
        docId = str(SimpleProvider.nextId)
        SimpleProvider.nextId += 1
        doc['_id'] = docId
      self.items[docId] = doc
      # publish the change to any listeners
      self.notifyAdded(doc)
      return doc

  def update(self, docId, changedFields, clearedFieldNames):
      # updated an existing document with specified changes
      if changedFields==None:
          changedFields = {}
      if clearedFieldNames==None:
          clearedFieldNames = []
      doc = self.items.get(docId)
      if doc==None:
          log.warning('updating %d docId:%s but it was not found', self.collectionName, docId)
          return None
      updated = False
      for field in changedFields.keys():
          if doc.get(field) != changedFields[field]:
              doc[field] = changedFields[field]
              updated = True
      for field in clearedFieldNames:
          if field in doc:
              del doc[field]
              updated = True
      # publish the change to any listeners
      if updated:
          self.notifyChanged(docId, changedFields, clearedFieldNames)
      return doc

  def remove(self, doc):
      docId = doc['_id'] if doc!=None and '_id' in doc else None
      if docId in self.items:
          # remove the document
          del self.items[docId]
          # publish the change to any listeners
          self.notifyRemoved(docId)
      return doc


# Class for managing login tokens used for resuming sessions.
# flows:
#   - login:
#       - initial: create token. assign user and session
#       - resume: lookup existing token, assign session
#   - logout: remove token, close other sessions using the token
#   - session close: remove session from existing token
#   - sheduled cleanup: walk list of tokens, if expired remove and close sessions
class LoginTokens:
    # List  of login tokens with associated users, and sessions
    # key: login token hash value
    # Array entry: {
    #   'loginToken': {
    #       'token': authentication token (sha256 digest),
    #       'tokenExpires': expiration time of token (# milliseconds since epoch)
    #   }
    #   'userId': _id of authenticated user,
    #   'sessions': [
    #       {
    #           'sessionId': current connection session_id,
    #           'session': DDP session object
    #       }
    #   ]
    # }
    tokenRegistry = []

    @staticmethod
    def _toEpochTime(dt):
        epoch = datetime.datetime.utcfromtimestamp(0)
        delta = (dt - epoch)
        return int(delta.total_seconds() * 1000.0)

    @staticmethod
    def createLoginToken(username):
        """ creates a new loginToken for the specified username """
        now = datetime.datetime.utcnow()
        tokenExpires = now + datetime.timedelta(days=1)
        loginToken = {
            'token': hashlib.sha256(str(time.time())).hexdigest(),
            'tokenExpires': {
                '$date': LoginTokens._toEpochTime(tokenExpires)
            }
        }
        return loginToken

    @staticmethod
    def addLoginToken(loginToken, userId, session):
        """ Create a new registry entry containing the provided loginToken, userId, and initial session and add it to the registry. """
        LoginTokens.tokenRegistry.append({
            'loginToken': loginToken,
            'userId': userId,
            'sessions': [{'sessionId': session.getSessionId(), 'session': session}]
        })

    @staticmethod
    def resumeToken(token, session):
        """ look up the registry entry with the specified token and add the specified session to it.  """
        results = jmespath.search('[?loginToken.token==`%s`]'%token, LoginTokens.tokenRegistry)
        entry = results[0] if len(results)>0 else None
        if entry!=None:
            entry['sessions'].append({'sessionId': session.getSessionId(), 'session': session})
        #TODO: Dave, should we throw an exception if the token isn't found or if it is expired?
        return entry

    @staticmethod
    def terminateSessionToken(session):
        """ looks up the registry entry for the specified session. Removes it from the registry and then closes all the other sessions associated with the token """
        log.debug('terminating other sessions sharing login token with session %s'%session.getSessionId())
        results = jmespath.search('[?sessions[?sessionId==`%s`]]'%session.getSessionId(), LoginTokens.tokenRegistry)
        entry = results[0] if len(results)>0 else None
        if entry!=None:
            # remove the entry from the registry
            LoginTokens.tokenRegistry.remove(entry)
            # NOTE: METEOR seems to terminate other DDP sessions using the token but the automatic
            #       relogin/logout path is problematic for us and we don't understand why. We have
            #       found that simply not terminating those sessions is fine because their local token
            #       store is polled and they get logged out automatically anyway. So for now we have
            #       commented out the session closing code below:
            # # close the other sessions besides the one given
            # for s in entry['sessions']:
            #     if s['session']!=session:
            #         log.debug(' - terminating session %s'% s['session'].getSessionId())
            #         s['session'].close()

    @staticmethod
    def cleanupExpiredTokens():
        """ finds all expired entries in the token registry. Removes them from the registry and closes all their associated sessions """
        # locate old registry entries
        now = datetime.datetime.utcnow()
        cutoffTime = LoginTokens._toEpochTime(now)
        results = jmespath.search('[?tokenInfo.expires<`%d`]'%cutoffTime, LoginTokens.tokenRegistry)
        for entry in results:
            # remove the entry from the registry
            LoginTokens.tokenRegistry.remove(entry)
            #  and then close the other sessions besides the one given
            for s in entry['sessions']:
                s['session'].close()
