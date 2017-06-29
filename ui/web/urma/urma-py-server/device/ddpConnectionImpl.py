import sys
sys.path.append("/WEB/python/appfiles.zip")
sys.path.append("/WEB/python/sockjs.zip")
from ddp import DDPConnection, DDPSessionImpl
from sockjs.tornado import SockJSConnection

class DDPConnectionImpl(SockJSConnection,DDPConnection):
  sessions = {}

  def __init__(self, session):
    SockJSConnection.__init__(self, session)
    self.connected = False

  def log_message(self, msg):
    print (msg)

  def on_open(self, info):
    print 'on_open of session_id '+str(self.session.session_id)
    self.ddpSession = DDPSessionImpl(self);

  def on_close(self, info):
    print 'on_close'
    self.ddpSession.close();
    del self.ddpSession

  def on_message(self, data):
    self.ddpSession.on_message(data)

  def getSessionId(self):
    return self.session.session_id
