from ddp import DDP, DDPConnection, Provider, SimpleProvider
import xmltodict
import hashlib
import datetime
import time

#
# Authentication Service
#

# Table of known userids - this probably comes from flash somewhere
users = {
  'admin': {
    'info': {
        '_id': 'aaaaaaaaaa',
        'username': 'admin',
        'emails': [
            {
                'address': 'joeadmin@digi.com',
                'verified': False
            }
         ],
        'createdAt': datetime.date.today().toordinal(),
        'profile': {
            'name': 'Joe Admin'
        },
        'services': None
    },
    'password': {
        'algorithm': 'sha-256',
        'hash': hashlib.sha256('admin').hexdigest()
    },
    'lastLoginToken': None
  },
  'user': {
    'info': {
        '_id': 'bbbbbbbbbb',
        'username': 'user',
        'emails': [
            {
                'address': 'joeuser@digi.com',
                'verified': False
            }
         ],
        'createdAt': datetime.date.today().toordinal(),
        'profile': {
            'name': 'Joe User'
        },
        'services': None
    },
    'password': {
        'algorithm': 'sha-256',
        'hash': hashlib.sha256('user').hexdigest()
    },
    'lastLoginToken': None
  }
}


# Authenticates the supplied user login parameters and returns validation results
#
#   params: [ {
#         'user': {
#           'username': 'buckrodgers'
#         },
#         password: {
#           'digest': 'ca978112ca1bbdcafac231b39a23dc4da786eff8147c4e72b9807785afee48bb',
#           'algorithm': 'sha-256'
#         }
#   }]
#
#   returns:
#     error: None if authentication successful, else an object describing failure
#       {
#          'error': 400,
#          'reason': 'incorrect password',
#          'message': 'incorrect password',
#          'errorType': 'Meteor.Error'
#       },
#     result: None if authentication fails, else an object containing authentication results
#       {
#          'id': _id of authenticated user,
#          'token': authentication token (sha256 digest),
#          'tokenExpires': expiration time of token (# seconds since epoch)
#       },
#     userInfo: None if authentication fails, else object contains info object of authenticated user
#       {
#          ...  see user.info record in user table above
#       }
#
def authenticateUser(params):
    print 'authenticating user'
    error = None
    result = {}
    userInfo = None
    if 'resume' in params[0]:
        print ' - trying to resume'
        # ToDo: Authenticate based off lastLoginToken
        error = {
            'error': 400,
            'reason': 'token unsupported',
            'message': 'login by token is not supported',
            'errorType': 'Meteor.Error'
        }
    elif 'user' in params[0]:
        print ' - checking by username'
        username = params[0]['user']['username']
        if username in users:
            print ' - found user'
            user = users[username]
            password = params[0]['password']
            if password['algorithm']==user['password']['algorithm'] and password['digest']==user['password']['hash']:
                print ' - accepted password'
                # TODO: make up a proper authorization token & expiration time
                loginToken = {
                    'token': hashlib.sha256(str(time.time)).hexdigest(),
                    'tokenExpires': {
                        '$date': (datetime.date.today() + datetime.timedelta(days=1)).toordinal()
                    }
                }
                user['lastLoginToken'] = loginToken
                result['id'] = user['info']['_id']
                result['token'] = loginToken['token']
                result['tokenExpires'] = loginToken['tokenExpires']
                userInfo = user['info']
            else:
                print ' - bad password'
                error = {
                    'error': 400,
                    'reason': 'incorrect password',
                    'message': 'incorrect password',
                    'errorType': 'Meteor.Error'
                }
        else:
            print ' - bad username'
            error = {
                'error': 401,
                'reason': 'unknown user',
                'message': 'unknown user',
                'errorType': 'Meteor.Error'
            }
    else:
        error = {
            'error': 402,
            'reason': 'invalid login request type',
            'message': 'invalid login request type',
            'errorType': 'Meteor.Error'
        }

    print ' - returning: error='+str(error)+', result='+str(result)+', userInfo='+str(userInfo)
    return error, result, userInfo



#
# Settings Provider
#
class SettingsProvider(SimpleProvider):
  """ A simple non-persistent, in-memory collection provider that is initialized from an xml file """

  def __init__(self, collectionName='settings', shared=True, ordered=False):
    SimpleProvider.__init__(self, collectionName, shared, ordered)
    sf = open('transportSettings.xml', 'r')
    xmlData = xmltodict.parse(sf.read())
    _loadGroupData(xmlData, self);


#
# State Provider
#
class StateProvider(SimpleProvider):
  """ A simple non-persistent, in-memory collection provider that is initialized from an xml file """

  def __init__(self, collectionName='state', shared=True, ordered=False):
    SimpleProvider.__init__(self, collectionName, shared, ordered)
    sf = open('transportState.xml', 'r')
    xmlData = xmltodict.parse(sf.read())
    _loadGroupData(xmlData, self);






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

# loads the given xml group data into the specified collection - assumed to be settings or state xml data
def _loadGroupData(xmldata, collection):
    #print 'parsing xml for collection: '
    _normalizeXmlData(xmldata)
    #print(pretty(xmldata))
    topLevelGroup = xmldata.keys()[0]
    groupNames = xmldata[topLevelGroup].keys()
    for groupNumber in range(len(groupNames)):
        groupName = groupNames[groupNumber]
        group = xmldata[topLevelGroup][groupName]
        #print 'loading group: '+groupName
        if isinstance(group, list):
            for index in range(len(group)):
                inst = group[index]
                #inst['_deviceId'] = None
                inst['_groupName'] = groupName
                inst['_groupIndex'] = index
                inst['_groupPath'] = groupName+'/'+str(index)
                collection.insert(inst)
        else:
            inst = group
            index=0
            #inst['_deviceId'] = None
            inst['_groupName'] = groupName
            inst['_groupIndex'] = index
            inst['_groupPath'] = groupName+'/'+str(index)
            collection.insert(inst)








# Pretty print formatter for dumping objects
class Formatter(object):
    def __init__(self):
        self.types = {}
        self.htchar = '\t'
        self.lfchar = '\n'
        self.indent = 0
        self.set_formater(object, self.__class__.format_object)
        self.set_formater(dict, self.__class__.format_dict)
        self.set_formater(list, self.__class__.format_list)
        self.set_formater(tuple, self.__class__.format_tuple)

    def set_formater(self, obj, callback):
        self.types[obj] = callback

    def __call__(self, value, **args):
        for key in args:
            setattr(self, key, args[key])
        formater = self.types[type(value) if type(value) in self.types else object]
        return formater(self, value, self.indent)

    def format_object(self, value, indent):
        return repr(value)

    def format_dict(self, value, indent):
        items = [
            self.lfchar + self.htchar * (indent + 1) + repr(key) + ': ' +
            (self.types[type(value[key]) if type(value[key]) in self.types else object])(self, value[key], indent + 1)
            for key in value
        ]
        return '{%s}' % (','.join(items) + self.lfchar + self.htchar * indent)

    def format_list(self, value, indent):
        items = [
            self.lfchar + self.htchar * (indent + 1) + (self.types[type(item) if type(item) in self.types else object])(self, item, indent + 1)
            for item in value
        ]
        return '[%s]' % (','.join(items) + self.lfchar + self.htchar * indent)

    def format_tuple(self, value, indent):
        items = [
            self.lfchar + self.htchar * (indent + 1) + (self.types[type(item) if type(item) in self.types else object])(self, item, indent + 1)
            for item in value
        ]
        return '(%s)' % (','.join(items) + self.lfchar + self.htchar * indent)


from collections import OrderedDict

def format_ordereddict(self, value, indent):
    items = [
        self.lfchar + self.htchar * (indent + 1) +
        "(" + repr(key) + ', ' + (self.types[
            type(value[key]) if type(value[key]) in self.types else object
        ])(self, value[key], indent + 1) + ")"
        for key in value
    ]
    return 'OrderedDict([%s])' % (','.join(items) +
           self.lfchar + self.htchar * indent)

pretty = Formatter()
pretty.set_formater(OrderedDict, format_ordereddict)