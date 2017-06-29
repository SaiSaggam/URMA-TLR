import os
import xmltodict
import time

token = os.environ.get('ddp_token')

#
# Public interfaces and API's
#
def init(signature, callback):
  result = (signature == token)
  if result:
      sf = open('transportSettings.xml', 'r')
      callback(sf.read())
      sf.close()
      sf = open('transportState.xml', 'r')
      callback(sf.read())
      sf.close()
  return result

def set(signature, user, group_name, instance, element_name, value, is_super):
  result = (signature == token)
  if result:
      print ("setting %s/%d/%s to %s by %s" %(group_name, instance, element_name, value, user))
  return result

def apply(signature, user, group_name, instance, is_super):
  result = (signature == token)
  if result:
      print ("apply configuration %s/%d by %s" %(group_name, instance, user))
  return result

def save(signature, user):
  result = (signature == token)
  if result:
      print ("saving configuration by %s" %user)
  return result

def poll(signature, group_name, instance, rate, callback):
  result = (signature == token)
  if result:
      # TBD: need to create a thread to read the status periodically
      time.sleep(rate)
      callback("<state><%s index=\"%d\"><element1>value1</element1></%s></state>" %(group_name, instance, group_name))      
  return result

