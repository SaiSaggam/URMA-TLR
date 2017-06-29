import sys
sys.path.append("/WEB/python/appfiles.zip") # Look in web fs when running on device
sys.path.append("appfiles.zip")             # Look on local fs when running on pc
sys.path.append("/WEB/python/sockjs.zip")
sys.path.append("sockjs.zip")
from tornado import web, ioloop, websocket
from tornado.log import enable_pretty_logging
from sockjs.tornado import SockJSRouter
from ddp import DDP, DDPConnection, Provider, SimpleProvider
from ddpConnectionImpl import DDPConnectionImpl
from serverServices import authenticateUser, SettingsProvider, StateProvider

enable_pretty_logging()

def doLogin(params, context):
  print 'invoked login(params='+str(params)+', context='+str(context)+')'
  error, result, userInfo = authenticateUser(params)

  if error==None:
    print 'add the new user '+str(userInfo)
    # Update the current user collection
    context['ddpSession'].setUser(userInfo['_id'], userInfo['username']);
    users.insert(userInfo)
    # TODO: update other publications as needed
    DDP.notifyAllSubscriptionsReady(context['ddpSession'])

  return {
    'error': error,
    'result': result
  }

def doLogout(params, context):
  print 'invoked logout(params='+str(params)+', context='+str(context)+')'
  currentUserId = context['ddpSession'].getUserId()
  if currentUserId!=None:
    # Update the current user collection
    users.remove({'_id': currentUserId})
    # TODO: update other publications as needed
    DDP.notifyAllSubscriptionsReady(context['ddpSession'])

  return {
    'error': None,
    'result': None
  }



# Define required and optional server methods
DDP.registerMethod('login', None, doLogin)
DDP.registerMethod('logout', None, doLogout)

# Register available collections
settings = DDP.registerCollection(SettingsProvider())
state = DDP.registerCollection(StateProvider())
users = DDP.registerCollection(SimpleProvider('users', True))
loginConfigs = DDP.registerCollection(SimpleProvider('meteor.loginServiceConfiguration', True))
clientVersions = DDP.registerCollection(SimpleProvider('meteor_autoupdate_clientVersions', True))
# sessions = DDP.registerCollection('sessions', SimpleProvider(False))


# Define any additional custom method calls - this is just an example
def updateSettingsFunc(params, context):
  print 'invoked updateSettingsFunc(params='+str(params)+', context='+str(context)+')'
  return {
    'error': None,
    'result': 'Mission Accomplished'
  }
DDP.registerMethod('updateSettings', None, updateSettingsFunc)





#
# Finally, configure and start the server for DDP connections
#
if __name__ == "__main__":
    settings = { 'static_path'          : './web',
                 'debug'                : True }
    DDPRouter = SockJSRouter(DDPConnectionImpl, '/sockjs')

    application = web.Application(
    DDPRouter.urls + [
      (r"/(.*)", web.StaticFileHandler, dict(default_filename="index.html",path=settings['static_path'])),
     ])

    application.listen(8000)
    ioloop.IOLoop.instance().start()

