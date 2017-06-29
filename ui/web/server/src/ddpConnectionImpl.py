import sys
import logging
log = logging.getLogger('ui.'+__name__)
sys.path.append("/WEB/python/appfiles.zip")
sys.path.append("/WEB/python/sockjs.zip")
from ddp import DDPConnection, DDPSessionImpl
from sockjs.tornado import SockJSConnection


class DDPConnectionImpl(SockJSConnection,DDPConnection):
  sessions = {}

  def __init__(self, session):
    SockJSConnection.__init__(self, session)
    self.connected = False
    self.closing = False
    self.tornadoSession = session

  def log_message(self, msg):
    log.info(msg)

  def on_open(self, info):
    log.info('DDP - open client session ['+str(self.session.session_id)+']')
    # print ' session : ', self.session
    # print ' session state: ', self.session.state
    # print ' session handler on_close: ', self.session.handler.on_close
    # print ' session handler session: ', self.session.handler.session
    self.ddpSession = DDPSessionImpl(self);

  def on_close(self, info):
    log.info('DDP - close session ['+str(self.session.session_id)+']')
    self.closing = True
    self.ddpSession.close()
    del self.ddpSession

  def on_message(self, data):
    self.ddpSession.on_message(data)

  def close(self, code, msg):
      self.tornadoSession.close(code, msg)

  def getSessionId(self):
    return self.session.session_id

  def send(self, data):
    """ send data to client if connection is open """
    if not self.closing:
        if self.session.is_closed:
            # NOTE: for some reason SockJSConnection is not getting closed event when session closes
            # so we are explicitly closing it here when we detect the session is closed
            self.on_close('session closed')
            self.close(3000, 'detected dead session')
        else:
            SockJSConnection.send(self, data)
