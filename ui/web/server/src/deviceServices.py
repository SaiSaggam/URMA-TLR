# This module exports a DeviceConfigManager object that can be used
# to communicate with native services on the device. When run on a
# pc this module will select a mock that simulates the device behavior
import os
import xmltodict
import threading
import logging
log = logging.getLogger('ui.'+__name__)

# Import the production or mock device services based off run mode
if os.environ.get('PYTHON_ENV')=='production':
    import ddp_if
else:
    from mockDeviceServices import ddp_if

#
# Exported constants
#
# firmware_status.status
class FW_STATUS:
    NONE = 0
    DOWNLOADING = 1
    VERIFYING = 2
    APPLYING = 3
    COMPLETE = 4
    REBOOTING = 5
    ERROR = 6



# This class wraps the underlying device ddp_if service and
# exports it as the DeviceConfigManager object. Callers send
# the signature on construction and it is subsequently passed
# on all device calls. Callers can also register multiple handlers
# against this object and they will all be invoked when the
# ddp_if service reports data. The ddp_if service reports data
# in xml form but this will be parsed into dictionary form before
# it is passed to the registered handlers.
class DeviceConfigManager:
  """ Exposes select device ddp_if service api's """
  handlerList = []

  def __init__(self, signature):
      """ Create the DeviceConfigManager instance with the signature token"""
      log.debug('create deviceConfigManager(%s)', signature)
      self.signature = signature

  def init(self):
      log.debug('calling ddp_if.init(%s, %s)', self.signature, str(DeviceConfigManager._globalHandler))
      return ddp_if.init(self.signature, DeviceConfigManager._globalHandler)

  def set(self, user, group_name, instance, element_name, value, access_level):
      log.debug('calling ddp_if.set(%s, %s, %s, %d, %s, %s, %s)', self.signature, user, group_name, instance, element_name, str(value), str(access_level))
      return ddp_if.set(self.signature, user, group_name, instance, element_name, value, access_level)

  def apply(self, user, group_name, instance, access_level):
      log.debug('calling ddp_if.apply(%s, %s, %s, %d, %s)', self.signature, user, group_name, instance, str(access_level))
      return ddp_if.apply(self.signature, user, group_name, instance, access_level)

  def save(self, user):
      log.debug('calling ddp_if.save(%s, %s)', self.signature, user)
      return ddp_if.save(self.signature, user)

  def poll(self, group_name, instance, rate):
      log.debug('calling ddp_if.poll(%s, %s, %d, %d, %s)', self.signature, group_name, instance, rate, DeviceConfigManager._globalHandler)
      return ddp_if.poll(self.signature, group_name, instance, rate, DeviceConfigManager._globalHandler)

  def cancel_poll(self, pollId):
      log.debug('calling ddp_if.cancel_poll(%s, %d)', self.signature, pollId)
      return ddp_if.cancel_poll(self.signature, pollId)

  def update_firmware(self, user, filename, remoteLoad):
      log.debug('calling ddp_if.update_firmware(%s, %s, %s, %s)', self.signature, user, filename, str(remoteLoad))
      return ddp_if.update_firmware(self.signature, user, filename, remoteLoad)

  def reboot(self, delay):
      log.debug('calling ddp_if.reboot(%s, %d)', self.signature, delay)
      return ddp_if.reboot(self.signature, delay)

  def authenticate(self, user, password):
      log.debug('calling ddp_if.authenticate(%s, %s, %s)', self.signature, user, ('none' if password==None else '*******'))
      return ddp_if.authenticate(self.signature, user, password)

  def addHandler(self, handlerFn, handlerCtx):
      DeviceConfigManager.handlerList.append({'fn': handlerFn, 'ctx': handlerCtx})


  def halt(self):
      return ddp_if.halt()

  handlerLock = threading.RLock()
  @staticmethod
  def _globalHandler(configManagerXmlMsg):
      with DeviceConfigManager.handlerLock: # ensure serialize callback processing
          log.debug('ddp_if handler received: %s', configManagerXmlMsg)
          # parse the message
          parsedMsg = xmltodict.parse(configManagerXmlMsg)

          # notify all registered handlers
          for handler in DeviceConfigManager.handlerList:
              handler['fn'](parsedMsg)
