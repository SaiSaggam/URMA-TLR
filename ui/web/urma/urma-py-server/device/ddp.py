
# from tornado import web, ioloop, websocket
# from sockjs.tornado import SockJSRouter, SockJSConnection
import json
import os
import xmltodict


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
    DDP.open(self)
    # auto subscribe to users collections
    print 'auto-subscribe to users'
    DDP.subscribe(self, 'users', 'users', [])

  def getSessionId(self):
    return self.ddpConnection.getSessionId()

  def on_message(self, data):
    """ Parse and route incoming DDP protocol messages """
    msg = json.loads(data)
    msg_type = msg.get('msg')
    print 'Received request: '+data

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

  def close(self):
    DDP.close(self)

  def handle_connect(self, msg):
    print 'handle connect'
    _clientSessionName = msg.get('session')
    _clientVersion = msg.get('version')
    _clientSupport = msg.get('support')
    DDP.connect(self, _clientSessionName, _clientVersion, _clientSupport)

  def handle_ping(self, msg):
    print 'handle ping'
    rsp = '{"msg":"pong"}'
    if msg.has_key('id'):
      rsp = '{"msg":"pong","id":"'+msg['id']+'"}'
    self.ddpConnection.send(rsp)

  def handle_method(self, msg):
    print 'handle method'
    _method = msg.get('method')
    _params = msg.get('params')
    _clientMethodId = msg.get('id')
    _randomSeed = msg.get('randomSeed')
    DDP.call(self, _method, _params, _clientMethodId, _randomSeed)

  def handle_sub(self, msg):
    print 'handle sub'
    _clientSubId = msg.get('id')
    _collectionName = msg.get('name')
    _subParams = msg.get('params')
    DDP.subscribe(self, _clientSubId, _collectionName, _subParams)

  def handle_unsub(self, msg):
    print 'handle unsub'
    _clientSubId = msg.get('id')
    DDP.unsubscribe(self, _clientSubId)

  def send_error(self, text):
    self.ddpConnection.send(json.dumps({'msg': 'error', 'text': text}))

  #
  # DDPSessionListener methods
  #

  def announce(self, serverId):
    rsp = {}
    rsp['server_id'] = serverId
    self.ddpConnection.send(json.dumps(rsp))

  def connected(self, clientSessionName):
    rsp = {}
    rsp['msg'] = 'connected'
    rsp['session'] = clientSessionName
    self.ddpConnection.send(json.dumps(rsp))

  def failed(self, clientVersion):
    rsp = {}
    rsp['msg'] = 'failed'
    rsp['version'] = clientVersion
    self.ddpConnection.send(json.dumps(rsp))

  def nosub(self, clientSubId, error):
    rsp = {}
    rsp['msg'] = 'nosub'
    rsp['id'] = clientSubId
    if error != None:
      rsp['error'] = error
    self.ddpConnection.send(json.dumps(rsp))
    
  def ready(self, clientSubIds):
    rsp = {}
    rsp['msg'] = 'ready'
    rsp['subs'] = clientSubIds
    self.ddpConnection.send(json.dumps(rsp))

  def added(self, collectionName, itemId, fields):
    rsp = {}
    rsp['msg'] = 'added'
    rsp['collection'] = collectionName
    rsp['id'] = itemId
    if fields != None:
      rsp['fields'] = fields
    self.ddpConnection.send(json.dumps(rsp))

  def changed(self, collectionName, itemId, fields, cleared=None):
    rsp = {}
    rsp['msg'] = 'changed'
    rsp['collection'] = collectionName
    rsp['id'] = itemId
    if fields != None:
      rsp['fields'] = fields
    if cleared != None:
      rsp['cleared'] = cleared
    self.ddpConnection.send(json.dumps(rsp))

  def removed(self, collectionName, itemId):
    rsp = {}
    rsp['msg'] = 'removed'
    rsp['collection'] = collectionName
    rsp['id'] = itemId
    self.ddpConnection.send(json.dumps(rsp))

  def addedBefore(self, collectionName, itemId, fields, beforeId):
    rsp = {}
    rsp['msg'] = 'addedBefore'
    rsp['collection'] = collectionName
    rsp['id'] = itemId
    if fields != None:
      rsp['fields'] = fields
    rsp['before'] = beforeId
    self.ddpConnection.send(json.dumps(rsp))

  def movedBefore(self, collectionName, itemId, beforeId):
    rsp = {}
    rsp['msg'] = 'movedBefore'
    rsp['collection'] = collectionName
    rsp['id'] = itemId
    rsp['before'] = beforeId
    self.ddpConnection.send(json.dumps(rsp))

  def result(self, clientMethodCallId, error, _result):
    rsp = {}
    rsp['msg'] = 'result'
    rsp['id'] = clientMethodCallId
    if error!=None:
      rsp['error'] = error
    if _result!=None:
      rsp['result'] = _result
    self.ddpConnection.send(json.dumps(rsp))

  def updated(self, clientMethodCallIds):
    rsp = {}
    rsp['msg'] = 'updated'
    rsp['methods'] = clientMethodCallIds
    self.ddpConnection.send(json.dumps(rsp))


class Subscription(object):
  """ A ddpSession's subscription to a server collection """
  def __init__(self, ddpSession, clientSubId, collectionName, provider, subParams):
    self.ddpSession = ddpSession
    self.clientSubId = clientSubId
    self.collectionName = collectionName
    self.provider = provider
    self.subParams = subParams


class DDP:
  methods = {}
  collections = {}
  sessions = {}
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
    del DDP.sessions[ddpSession.getSessionId()]

  @staticmethod
  def connect(ddpSession, clientSessionName, clientVersion, clientSupport):
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
      ddpSession.result(clientMethodCallId, result.get('error'), result.get('result'))
      ddpSession.updated([clientMethodCallId])

  @staticmethod
  def subscribe(ddpSession, clientSubId, collectionName, subParams):
    """ subscribes client to notification of current and future collection content changes """
    provider = DDP.collections.get(collectionName)
    if provider == None:
      print 'no collection named '+collectionName+' is available'
      return None
    # create the subscription and add it to the map of known collection subscriptions
    subscription = Subscription(ddpSession, clientSubId, collectionName, provider, subParams)
    if DDP.subscriptions.get(collectionName) == None:
      DDP.subscriptions[collectionName] = [subscription]
    else:
      DDP.subscriptions[collectionName].append(subscription)
    # notify provider of new subscribing session
    provider.subscribe(ddpSession, clientSubId, collectionName, subParams)
    return subscription

  @staticmethod
  def unsubscribe(ddpSession, clientSubId):
    """ cancels client notification of future collection changes """
    # find the subscription with matching ddpSession and clientSubId
    for sub in DDP.subscriptions:
      if sub.ddpSession==ddpSession and sub.clientSubId==clientSubId:
        # notify provider of unsubsctiption
        sub.provider.unsubscribe(ddpSession, clientSubId, sub.collectionName, sub.subParams)
        # remove the subscription
        DDP.subscriptions[sub.collectionName].remove(sub)

  @staticmethod
  def notifyAllSubscriptionsReady(ddpSession):
    """ marks all current subscriptions as ready """
    sessionSubIds = []
    for collectionName in DDP.subscriptions:
      collectionSubs = DDP.subscriptions[collectionName]
      for sub in collectionSubs:
        #print 'examining(collection='+str(collectionName)+', sub='+str(sub)+', ddpSession='+str(ddpSession)+')'
        if sub.ddpSession==ddpSession:
          sessionSubIds.append(sub.clientSubId);
    ddpSession.ready(sessionSubIds)


class Provider(object):
  """ All collection providers must implement this interface """

  def __init__(self, collectionName):
    self.collectionName = collectionName
    self.subscriptions = []

  def getCollectionName(self):
    return self.collectionName

  def getItems(self, subParams):
    """ Subclass will return all items that match requested criteria"""
    pass

  def subscribe(self, ddpSession, clientSubId, collectionName, subParams):
    if collectionName!=self.collectionName:
      #TODO throw error - subscribing to wrong provider
      print 'Error: Subscribing to provider '+self.collectionName+' but requesting collection '+collectionName
      return
    # record the subscription
    self.subscriptions.append(ddpSession)
    # notify the subscriber of items they are allowed to see
    for i in self.getItems(subParams):
      if self.allowPublish(ddpSession, i):
        ddpSession.added(self.collectionName, i.get('_id'), i)
    ddpSession.ready([clientSubId])

  def unsubscribe(self, ddpSession, clientSubId, collectionName, subParams):
    self.subscriptions.remove(ddpSession)

# a["{\"msg\":\"added\",\"collection\":\"likes\",\"id\":\"yHz6YtnxPrQcX5JsD\",\"fields\":{\"name\":\"Joey\",\"description\":\"I love it!\"}}"]
  def notifyAdded(self, doc):
    """ Notify sessions with access of changes """
    for ddpSession in self.subscriptions:
      if self.allowPublish(ddpSession, doc):
        ddpSession.added(self.collectionName, doc['_id'], doc)

# a["{\"msg\":\"changed\",\"collection\":\"likes\",\"id\":\"yHz6YtnxPrQcX5JsD\",\"fields\":{\"description\":\"I love it! even more!\"}}"]
  def notifyChanged(self, doc):
    """ Notify sessions with access of changes """
    for ddpSession in self.subscriptions:
      if self.allowPublish(ddpSession, doc):
        ddpSession.changed(self.collectionName, doc['_id'], doc)

# a["{\"msg\":\"removed\",\"collection\":\"likes\",\"id\":\"yHz6YtnxPrQcX5JsD\"}"]
  def notifyRemoved(self, doc):
    """ Notify sessions with access of changes """
    for ddpSession in self.subscriptions:
      if self.allowPublish(ddpSession, doc):
        ddpSession.removed(self.collectionName, doc['_id'])

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

  def __init__(self, collectionName, shared=False, ordered=False):
    Provider.__init__(self, collectionName)
    self.items = {}
    self.nextId = 0
    self.shared = shared
    self.ordered = ordered

  def getItems(self, subParams):
    # TODO: scope collection results further using subParams as needed
    return self.items.values()

  def getItem(self, docId):
    return self.items.get(docId)

  def allowPublish(self, ddpSession, doc):
    return self.shared or doc['owner']==ddpSession.getUserId()

  def allowInsert(self, ddpSession, doc):
    return True

  def allowUpdate(self, ddpSession, operation, doc):
    return self.shared or  doc['owner']==ddpSession.getUserId()

  def allowRemove(self, ddpSession, doc):
    return self.shared or  doc['owner']==ddpSession.getUserId()

  def insert(self, doc):
      # build item to insert into collection (assign id and associate owner)
      if '_id' in doc:
        #print 'has id: '+doc['_id']
        docId = doc['_id']
      else:
        #print 'warning: inserted document needs _id: '+str(doc)
        docId = str(self.nextId)
        self.nextId += 1
        doc['_id'] = docId
      self.items[docId] = doc
      # publish the change to any listeners 
      self.notifyAdded(doc)
      return doc

  def update(self, operation, doc):
      # apply the operation to the document
      updates = operation['$set']
      for field in updates.keys():
        doc[field] = updates[field]
      # publish the change to any listeners 
      self.notifyChanged(doc)
      return doc
    
  def remove(self, doc):
      # remove the document
      del self.items[doc['_id']]
      # publish the change to any listeners 
      self.notifyRemoved(doc)
      return doc

