# This module exports a mock ddp_if for use when running on a pc. It
# reports settings and state data from a file rather than live from
# the device.
#

import os
import sys
import time
import math
import random
import threading
import uuid
import xmltodict
import jmespath
import logging
log = logging.getLogger('ui.'+__name__)


token = os.environ.get('CONFIG_MANAGER_API_TOKEN')

class FW_STATUS:
    NONE = 0
    DOWNLOADING = 1
    VERIFYING = 2
    APPLYING = 3
    COMPLETE = 4
    REBOOTING = 5
    ERROR = 6

users = {
    'admin': {
        'password': 'admin',
        'access': 2 # SUPER_USER
    },
    'user': {
        'password': 'user',
        'access': 1 # READ_WRITE
    }
}

def simulateSystem(rate):
  #print 'simulating system...'
  jpQuery = 'state.system[0]'
  group = jmespath.search(jpQuery, ddp_if.cache)
  if group!=None:
      localtime = time.strftime('%a %B %Y, %H:%M:%S', time.localtime(time.time()))
      #print '  setting system_time to: '+localtime
      group['system_time'] = localtime
      group['cpu_usage'] = math.ceil(random.betavariate(2,5)*100)
  #print 'halted = ', ddp_if.halted
  if not ddp_if.halted:
      ddp_if.simtimer = threading.Timer(1, simulateSystem, [rate])
      ddp_if.simtimer.deamon = True
      ddp_if.simtimer.start()

def doPoll(pollId, group_name, instance, rate, callback):
  # simulate a poll of the specified group_name and instance, then requeue the next timer
  # key = group_name+str(instance)
  p = ddp_if.polls.get(pollId)
  log.debug('polling %d %s/%d at rate %d. p=%s', pollId, group_name, instance, rate, str(p))
  if p==None:
      # poll must have been cancelled for this group... don't do anything
      pass
  else:
      # fetch the requested data and make the callback
      #ex  settings.users[?("@index"==`2`)]
      if instance==0:
          jpQuery = 'state.'+group_name
      elif instance==1:
          jpQuery = 'state.'+group_name+'[?("@index"==null)]'
      else:
          jpQuery = 'state.'+group_name+'[?("@index"=="'+str(instance)+'")]'
      # log.debug('polling %s', jpQuery)
      data = jmespath.search(jpQuery, ddp_if.cache)
      # log.debug('found: %s', json.dumps(data))
      response = {
        'state': {
            group_name: data
        }
      }
      xmldata = xmltodict.unparse(response)
      log.debug('reporting: %s', xmldata)
      callback(xmldata)
      # now reschedule the next poll
      # log.debug('rescheduling next poll')
      timer = threading.Timer(rate, doPoll, [pollId, group_name, instance, rate, callback])
      ddp_if.polls[str(pollId)] = timer
      timer.daemon = True
      timer.start()

def _normalizeGroups(xmldata):
    topLevelGroup = xmldata.keys()[0]
    groupNames = xmldata[topLevelGroup].keys()
    for groupNumber in range(len(groupNames)):
      groupName = groupNames[groupNumber]
      group = xmldata[topLevelGroup][groupName]
      if not isinstance(group, list):
          xmldata[topLevelGroup][groupName] = [group]
    return xmldata

#
# Public interfaces and API's
#
class ddp_if:
    halted = False
    # simulated collection data. keyed by collection_name
    cache = {}
    # list of active polls. keyed by pollId. value is poll timer
    polls = {}


    simrate = 1 #secons
    simtimer = threading.Timer(simrate, simulateSystem, [simrate])
    simtimer.daemon = True
    simtimer.start()

    @staticmethod
    def init(signature, callback):
      log.debug('called mock ddp_if.init(%s, %s)', signature, str(callback))
      result = (signature == token)
      if result:
          log.debug('sending up initial data')
          sf = open('transportSettings.xml', 'r')
          xmldata = sf.read()
          sf.close()
          ddp_if.cache.update(_normalizeGroups(xmltodict.parse(xmldata)))
          callback(xmldata)
          sf = open('transportState.xml', 'r')
          xmldata = sf.read()
          sf.close()
          ddp_if.cache.update(_normalizeGroups(xmltodict.parse(xmldata)))
          callback(xmldata)
      return result

    @staticmethod
    def set(signature, user, group_name, instance, element_name, value, access_level):
      result = (signature == token)
      if result:
          # TODO: update simulated collection contents
          log.debug('setting %s/%d/%s to %s by %s', group_name, instance, element_name, value, user)
      return result

    @staticmethod
    def apply(signature, user, group_name, instance, access_level):
      result = (signature == token)
      if result:
          log.debug('apply configuration %s/%d by %s', group_name, instance, user)
      return result

    @staticmethod
    def save(signature, user):
      result = (signature == token)
      if result:
          log.debug('saving configuration by %s', user)
      return result

    @staticmethod
    def poll(signature, group_name, instance, rate, callback):
      if signature==token:
          pollId = uuid.uuid4().int
          timer = threading.Timer(rate, doPoll, [pollId, group_name, instance, rate, callback])
          ddp_if.polls[pollId] = timer
          timer.daemon = True
          timer.start()
          return pollId
      return -1

    @staticmethod
    def cancel_poll(signature, pollId):
      result = (signature == token)
      if result:
          timer = ddp_if.polls.get(str(pollId))
          if timer!=None:
              timer.cancel()
              del ddp_if.polls[pollId]
          result = True
      return result

    @staticmethod
    def update_firmware(signature, user, filename, loadRemote):
      result = (signature == token)
      if result:
          log.debug('user %s updating firmware to %s', user, filename)
          jpQuery = 'state.firmware_status[0]'
          group = jmespath.search(jpQuery, ddp_if.cache)

          if loadRemote:
              log.debug('   - downloading firmware from remote repository...')
              # simulate download progress
              group['status'] = FW_STATUS.DOWNLOADING
              group['progress'] = 0
              for i in range(10):
                  group['progress'] = i*10
                  time.sleep(1)
          log.debug('   - applying firmware image...')
          # simulate update progress
          group['status'] = FW_STATUS.VERIFYING
          group['progress'] = 0
          time.sleep(1)
          group['progress'] = 100
          group['status'] = FW_STATUS.APPLYING
          group['progress'] = 0
          for i in range(5):
              group['progress'] = i*20
              time.sleep(1)
          log.debug('   - update complete!')
          group['status'] = FW_STATUS.COMPLETE
          group['progress'] = 100
          group['result'] = 1
      return result

    @staticmethod
    def reboot(signature, delay):
      result = (signature == token)
      if result:
          log.debug('reboot device in %d minutes', delay)
          jpQuery = 'state.firmware_status[0]'
          group = jmespath.search(jpQuery, ddp_if.cache)

          group['status'] = FW_STATUS.REBOOTING
          group['progress'] = 0
          time.sleep(1)
          sys.exit(99) # restart status... check for in launch shell
      return result

    @staticmethod
    def authenticate(signature, user, password):
      access = -1 # NO_ACCESS
      if signature==token:
          log.debug('authenticate user %s, password %s', user, ('none' if password==None else '*******'))
          usr = users.get(user)
          if usr!=None and usr['password']==password:
            log.debug(' - user authenticated')
            access = usr['access']
      return access

    @staticmethod
    def halt():
      ddp_if.halted = True
