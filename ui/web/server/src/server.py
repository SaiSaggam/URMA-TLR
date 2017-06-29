import sys
import os, uuid
import hashlib
import datetime
import time
import logging
import logging.handlers
log = logging.getLogger('ui.'+__name__)
rootLog = logging.getLogger('ui')
import argparse
parser = argparse.ArgumentParser(prog='server.py', description='Serve the local web UI')
parser.add_argument('-pp','--pythonPath', help='path to find zipped python modules', default=os.environ.get('PYTHON_PATH','.'))
parser.add_argument('-wp','--webPath', help='path to the web fs', default=os.environ.get('WEBFS_PATH','~/webfs:./webfs:../webfs'))
parser.add_argument('-up','--uploadPath', help='path to upload files', default=os.environ.get('UPLOAD_PATH','~'))
parser.add_argument('-fup','--fwUploadPath', help='path to upload firmware files', default=os.environ.get('FWUPLOAD_PATH','/tmp'))
parser.add_argument('-p','--webPort', help='port to serve UI at', default=os.environ.get('WEB_PORT',8000))
parser.add_argument('-ln','--loggerName', help='the logger name to configure', default=os.environ.get('LOGGER_NAME','ui'))
parser.add_argument('-ll','--loggerLevel', help='the logger level to configure', default=os.environ.get('LOGGER_LEVEL','INFO'), choices=['ERROR', 'WARNING', 'INFO','DEBUG'])
parser.add_argument('-ls','--loggerSpec', help='the logger levels to use "name:level[,name:level...]"', default=os.environ.get('LOGGER_SPEC', None))
parser.add_argument('-lh','--logHandler', help='log handler to use', default=os.environ.get('LOGGER_HANDLER', 'SYSLOG'), choices=['NONE', 'SYSLOG'])
args = parser.parse_args()

dftLog = logging.getLogger(args.loggerName)
dftLvl = args.loggerLevel.upper()
dftLog.setLevel(getattr(logging, dftLvl))
if args.loggerSpec!=None:
    #TODO: decode the list of loggers and set their levels
    pass
if args.logHandler=='SYSLOG':
    handler = logging.handlers.SysLogHandler(address='/dev/log')
    rootLog.addHandler(handler)

python_path = args.pythonPath
webfs_path = args.webPath
upload_path = args.uploadPath
fwupload_path = args.fwUploadPath
web_port = args.webPort

sys.path.append(python_path+"/appfiles.zip") # everything packed up by grayduck
sys.path.append(python_path+"/sockjs.zip")
from tornado import web, ioloop, websocket
from tornado.log import enable_pretty_logging
import httplib
import base64
from sockjs.tornado import SockJSRouter
from ddp import DDP, DDPConnection, Provider, SimpleProvider, LoginTokens
from ddpConnectionImpl import DDPConnectionImpl
from serverServices import SettingsProvider, StateProvider, UserProvider
from deviceServices import DeviceConfigManager

enable_pretty_logging()


# setup the device config manager service
deviceConfigManager = DeviceConfigManager(os.environ.get('CONFIG_MANAGER_API_TOKEN'))
# Register available collections
users = DDP.registerCollection(UserProvider())
settings = DDP.registerCollection(SettingsProvider(deviceConfigManager, users))
state = DDP.registerCollection(StateProvider(deviceConfigManager))
loginConfigs = DDP.registerCollection(SimpleProvider('meteor.loginServiceConfiguration', True))
clientVersions = DDP.registerCollection(SimpleProvider('meteor_autoupdate_clientVersions', True))
# sessions = DDP.registerCollection('sessions', SimpleProvider(False))

# Trigger device services to send initial collection data
deviceConfigManager.init()





def currentUserId(context):
    session = (context.get('ddpSession') if context!=None else None)
    if session!=None:
        return session.getUserId()
    return None

def currentUser(context):
    session = (context.get('ddpSession') if context!=None else None)
    if session!=None:
        #TODO: pull this from real user record once its available
        log.debug('get current user from session: %s ', str(session))
        return {
            '_id': session.getUserId(),
            'username': session.getUserName(),
            'permissions': ['read', 'write']
        }
    return None

def hasPermission(user, requestedPermission):
    if user!=None and user.get('permissions')!=None and requestedPermission in user.get('permissions'):
        return True
    return False

def doLogin(params, context):
    log.debug('invoked login(params=%s, context=%s)', str(params), str(context))
    error = None
    result = None
    session = (context.get('ddpSession') if context != None else None)
    if session != None:
        if 'resume' in params[0]:
            log.debug('  - attempting to resume previous session: %s', params[0]['resume'])
            time.sleep(0.05) # this delay makes our DDP trace look more like meteors.
            token = params[0]['resume']
            error, result = _resumeLogin(session, token)
        elif 'user' in params[0]:
            log.debug('  - authenticating by username')
            username = params[0]['user']['username']
            password = params[0]['password']
            error, result = _loginWithUsername(session, username, password)
        else:
            log.debug('  - invalid login request type')
            error = {
                'error': 400,
                'reason': 'invalid login request type',
                'message': 'invalid login request type',
                'errorType': 'Meteor.Error'
            }
    else:
        # Should never happen
        error = {
            'error': 400,
            'reason': 'invalid session on login',
            'message': 'invalid session on login',
            'errorType': 'Meteor.Error'
        }


    return {
        'error': error,
        'result': result
    }

def doLogout(params, context):
  log.debug('invoked logout(params=%s, context=%s)', str(params), str(context))
  session = (context.get('ddpSession') if context!=None else None)
  if session!=None:
      currentUserId = session.getUserId()
      if currentUserId!=None:
        log.info('userId %s logged out of session %s', currentUserId, session.getSessionId())
        # TODO: eventually we need to rerun the publish functions for all session subscriptions
        # but for now we will manually remove the user from the user collection to facilitate the
        # logout process in the client. Trigger the republish after setting session user to None below.
        # perhaps sessions should probably know what their subscriptions are and manage the republish?
        users.notifyRemoved(currentUserId, session)
        # Reset the session user
        session.setUser(None, None)
        # logout the other sessions associated with this login token
        LoginTokens.terminateSessionToken(session)
        # notify client that all subscriptions have been updated
        DDP.notifyAllSubscriptionsReady(session)

  return {
    'error': None,
    'result': None
  }

def _resumeLogin(session, token):
    # ToDo: Authenticate based off lastLoginToken
    log.debug('  - resume not currently supported, falling back to new login')
    error = None
    result = None
    tokenInfo = LoginTokens.resumeToken(token, session)
    if tokenInfo != None:
        userId = tokenInfo['userId']
        user = users.getItem(userId)
        if user != None:
            session.setUser(userId, user['username'])
            log.debug('auto-subscribe to users')
            # TODO: eventually we need to rerun the publish functions for all session subscriptions
            # but for now we will simply notify the client the user was added to the collection
            # login process in the client
            users.notifyAdded(user, session)
            # notify client that all subscriptions have been updated
            DDP.notifyAllSubscriptionsReady(session)

            loginToken = tokenInfo['loginToken']
            result = {
                'id': userId,
                'token': loginToken['token'],
                'tokenExpires': loginToken['tokenExpires']
            }
        else:
            error = {
                'error': 403,
                'reason': 'user not found',
                'message': 'User not found. Please log in again.',
                'errorType': 'Meteor.Error'
            }
    else:
        error = {
            'error': 403,
            'reason': "You've been logged out by the server. Please log in again.",
            'message': "You've been logged out by the server. Please log in again. [403]",
            'errorType': 'Meteor.Error'
        }

    return error, result


class ACCESS_LEVEL:
    NO_ACCESS = -1
    READ_ONLY = 0
    READ_WRITE = 1
    SUPER_USER = 2

def _loginWithUsername(session, username, password):
    log.debug('  - authenticating user (username=%s)', username)
    error = None
    result = None
    user = users.findUser(username)
    access = deviceConfigManager.authenticate(username, password)
    if user!=None and access!=ACCESS_LEVEL.NO_ACCESS:
        log.debug('  - user authenticated (username=%s)', username)
        loginToken = LoginTokens.createLoginToken(username)
        userId = user.get('_id')
        session.setUser(userId, username)
        # Register the login token for the user and session
        LoginTokens.addLoginToken(loginToken, userId, session)
        # auto subscribe to users collections
        log.debug('auto-subscribe to users')
        # TODO: eventually we need to rerun the publish functions for all session subscriptions
        # but for now we will simply notify the client the user was added to the collection
        # login process in the client
        users.notifyAdded(user, session)
        # notify client that all subscriptions have been updated
        DDP.notifyAllSubscriptionsReady(session)
        result = {
            'id': userId,
            'token': loginToken['token'],
            'tokenExpires': loginToken['tokenExpires']
        }
    else:
        log.debug('  - authentication failed')
        error = {
            'error': 401,
            'reason': 'invalid username or password',
            'message': 'invalid username or password',
            'errorType': 'Meteor.Error'
        }

    log.debug(' - returning: error=%s, result=%s', str(error), str(result))
    return error, result

# Helper to convert a datetime object to epoch timestamp (milliseconds since 1970)
def _toEpochTime(dt):
    epoch = datetime.datetime(1970,1,1)
    delta = dt - epoch
    return int(delta.total_seconds() * 1000.0)


class FW_STATUS:
    NONE = 0
    DOWNLOADING = 1
    VERIFYING = 2
    APPLYING =  3
    COMPLETE =  4
    REBOOTING = 5
    ERROR = 6

def reportFirmwareUpdateStatus(status, progress):
    newstate = {
        '_groupName': 'firmware_status',
        '_groupIndex': 0,
        '_groupPath': 'firmware_status/0',
        'status': status,
        'progress': progress,
        'result':0
    }
    newstate['_id'] = hashlib.md5('state/'+newstate['_groupPath']).hexdigest()
    log.debug('firmware upgrade status=%d progress=%d', status, progress)
    state.upsert(newstate)

# Apply specified firmware file to device. File is expected to be found in /tmp
# Currently logged in user must have read/write permissions
# params: {
#    'fileName': 'firmware.bin',
#    'autoreboot': 'False'
# }
def doUpdateFirmware(params, context):
  log.info('invoked updateFirmware(params==%s, context=%s)', str(params), str(context))

  #ensure current user has read/write permissions
  user = currentUser(context)
  log.info('current user is %s ', str(user))
  if user==None or user.get('username')==None or not hasPermission(user, 'write'):
      reportFirmwareUpdateStatus(FW_STATUS.ERROR, 0)
      return {
        'error': 'user not authorized for action',
        'result': False
      }

  # extract and validate the required parameters
  if len(params)<1:
      reportFirmwareUpdateStatus(FW_STATUS.ERROR, 0)
      return {
        'error': 'firmware filename required',
        'result': False
      }
  filename = fwupload_path+'/'+params[0]
  # extract the optional parameters (param[1] is a dictionary of optional params)
  autoReboot = False if len(params)<2 or not isinstance(params[1], dict) else params[1].get('autoReboot', False)
  remoteLoad = False if len(params)<2 or not isinstance(params[1], dict) else params[1].get('remoteLoad', False)
  if remoteLoad:
      filename = params[0]

  #perform the update!
  status = deviceConfigManager.update_firmware(user['username'], filename, remoteLoad)
  if not status:
      reportFirmwareUpdateStatus(FW_STATUS.ERROR, 0)
      return {
        'error': 'firmware update operation failed',
        'result': False
      }

  reportFirmwareUpdateStatus(FW_STATUS.COMPLETE, 100)
  #reboot if requested
  if autoReboot:
      # Wait briefly before rebooting to allow last status to be sent.
      time.sleep(1);
      status = deviceConfigManager.reboot(0)
      if not status:
          return {
            'error': 'reboot request failed',
            'result': False
          }

  return {
    'result': True
  }


# Reboot the device in specified time. Delay is specified in minutes (0=immediate)
# Currently logged in user must have read/write permissions
# params: {
#    'delay': 0
# }
def doReboot(params, context):
  log.info('invoked reboot(params==%s, context=%s)', str(params), str(context))

  #ensure current user has read/write permissions
  user = currentUser(context)
  log.info('current user is %s ', str(user))
  if user==None or not hasPermission(user, 'write'):
      reportFirmwareUpdateStatus(FW_STATUS.ERROR, 0)
      return {
        'error': 'user not authorized for action',
        'result': False
      }

  delay = 0
  if len(params)>0:
      delay = params[0]

  #perform the reboot
  status = deviceConfigManager.reboot(delay)
  if not status:
      return {
        'error': 'reboot operation failed',
        'result': False
      }

  return {
    'result': True
  }

def doProvisionDevice(username, password, deviceId, server):
    ''' Provision the specified device to device cloud using the provided credentials'''
    log.debug('invoked provisionDevice(username=%s, deviceId=%s, server=%s)', username, deviceId, server)
    error = None
    result = None
    # first check to see if device is already provisioned
    conn = httplib.HTTPSConnection(server)
    headers = {'Authorization': 'Basic '+base64.b64encode(username+':'+password)}
    conn.request('GET', '/ws/v1/devices/inventory/'+deviceId, None, headers)
    r = conn.getresponse()
    rmsg = r.read()
    if r.status==200 or r.status==201:
        # already provisioned... return success
        result = r.status
    elif r.status==404:
        # device not found... provision it now
        body = '{"id": "'+deviceId+'"}'
        conn.request('POST', '/ws/v1/devices/inventory', body, headers)
        r = conn.getresponse()
        rmsg = r.read();
        if r.status==200 or r.status==201:
            # successfully provisioned... return success
            result = r.status
        else:
            # something went wrong on the provision call, return what we know
            error = {
                'error': r.status,
                'reason': 'provision failed',
                'message': 'unable to provision the device',
                'errorType': 'Meteor.Error'
            }
    else:
        # some other error while looking for device... return error
        error = {
            'error': r.status,
            'reason': 'provision failed',
            'message': 'unable to read the device list',
            'errorType': 'Meteor.Error'
        }
    return {
        'error': error,
        'result': result
    }


# Define required and optional server methods
DDP.registerMethod('login', None, doLogin)
DDP.registerMethod('logout', None, doLogout)
DDP.registerMethod('updateFirmware', None, doUpdateFirmware)
DDP.registerMethod('reboot', None, doReboot)
DDP.registerMethod('provisionDevice', None, doProvisionDevice)



# Define any additional custom method calls - this is just an example
def updateSettingsFunc(params, context):
  log.info('invoked updateSettingsFunc(params==%s, context=%s)', str(params), str(context))
  return {
    'error': None,
    'result': 'Mission Accomplished'
  }
DDP.registerMethod('updateSettings', None, updateSettingsFunc)



#
# Finally, configure and start the server for DDP connections
#
if __name__ == "__main__":
    class UploadFirmware(web.RequestHandler):
        def post(self):
            if self.request.files.get('files[]')==None:
                log.warning('bad upload firmware request, no files[] present')
                self.set_status(400)
                self.finish()
                return
            fileinfo = self.request.files['files[]'][0]
            if fileinfo.get('filename')==None or fileinfo.get('body')==None:
                log.warning('bad upload firmware request, no files[] present')
                self.set_status(400)
                self.finish()
                return
            fname = fileinfo['filename']
            log.info('upload firmware: %s', fname)
            extn = os.path.splitext(fname)[1]
            # cname = str(uuid.uuid4()) + extn
            fullname = fwupload_path + '/' + fname
            fh = open(fullname, 'w')
            fh.write(fileinfo['body'])
            self.set_status(200);
            self.finish()
            log.info('finished uploading firmware to %s!', fullname)

    class Upload(web.RequestHandler):
        def post(self):
            log.info('upload file: %s', str(self.request.files))
            if self.request.files.get('files[]')==None:
                log.warning('bad upload file request, no files[] present')
                self.set_status(400)
                self.finish()
                return
            fileinfo = self.request.files['files[]'][0]
            if fileinfo.get('filename')==None or fileinfo.get('body')==None:
                log.warning('bad upload file request, no files[] present')
                self.set_status(400)
                self.finish()
                return
            fname = fileinfo['filename']
            extn = os.path.splitext(fname)[1]
            # cname = str(uuid.uuid4()) + extn
            fullname = upload_path + '/' + fname
            fh = open(fullname, 'w')
            fh.write(fileinfo['body'])
            self.set_status(200);
            self.finish()
            log.info('finished uploading file to %s!', fullname)

    class StaticHandler(web.StaticFileHandler):
        # Requests for paths that don't end in a file extension will be
        # handled as though / were requested and index.html will be returned.
        # The client will handle those routes. Any path ending with a file
        # extension will be handled as a normal static file.
        # This is a simple alternative to doing something like rewriting URLs,
        # which tornado doesn't support.
        def parse_url_path(self, path):
            if path:
                filename, file_extension = os.path.splitext(path)
                # If no extension on the path we just clear the path.
                # StaticFileHandler will then return the default, index.html.
                # Otherwise leave the path untouched and let parse_url_path do
                # the real work for static files.
                if not file_extension:
                    log.debug('static path conversion: %s -> <empty>', path)
                    path = ''
            return super(StaticHandler, self).parse_url_path(path)

        def get_modified_time(self):
            # URMA-535: Don't return a modified date for the root path (i.e. index.html).
            # This is to ensure the browser always gets a fresh copy of index.html.
            if not self.path or self.path == 'index.html':
                log.debug('Returning modified time of None, path=%s', self.path)
                return None
            else:
                return super(StaticHandler, self).get_modified_time()

    DDPRouter = SockJSRouter(DDPConnectionImpl, '/sockjs')
    appSettings = {
        'static_path'          : webfs_path,
        'static_handler_class' : StaticHandler,
        'debug'                : True
    }

    # print "router transport url's: ", DDPRouter.urls

    application = web.Application(
        DDPRouter.urls + [
          (r"/firmware", UploadFirmware),
          (r"/uploads", Upload),
          (r"/(.*)", StaticHandler, dict(default_filename="index.html",path=appSettings['static_path'])),
         ], debug=True)

    application.listen(web_port)
    ioloop.IOLoop.instance().start()
