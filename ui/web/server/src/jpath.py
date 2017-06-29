import logging
log = logging.getLogger('ui.'+__name__)
from collections import OrderedDict
import json

# Helper functions to map mongo style queries to jmespath
# See: https://docs.mongodb.org/manual/reference/method/db.collection.find/
# See: https://github.com/jmespath/jmespath.py

def jexp(mexp, joinOp='&&'):
    """ convert a mongo expresion to a jmespath expression """
    if mexp==None or mexp=={}:
        return '*'
    result = '('
    if isinstance(mexp, list):
        for item in mexp:
            result += jexp(item)
            result += ' '+joinOp+' '
        result = result[:-(2+len(joinOp))]
    elif isinstance(mexp, dict):
        joinOp = '&&' # object fields are always AND's
        for key in mexp.keys():
            value = mexp[key]
            if key=='$and':
                result += jexp(value, '&&')
            elif key=='$or':
                result += jexp(value, '||')
            elif isinstance(value, dict):
                if '$eq' in value:
                    result += key+'==`'+str(value['$eq'])+'`'
                elif '$ne' in value:
                    result += key+'!=`'+str(value['$ne'])+'`'
                elif '$gt' in value:
                    result += key+'>`'+str(value['$gt'])+'`'
                elif '$gte' in value:
                    result += key+'>=`'+str(value['$gte'])+'`'
                elif '$lt' in value:
                    result += key+'<`'+str(value['$lt'])+'`'
                elif '$lte' in value:
                    result += key+'<=`'+str(value['$lte'])+'`'
            else:
                result += key+'==`'+str(value)+'`'
            result += ' '+joinOp+' '
        result = result[:-(2+len(joinOp))]
    result += ')'
    return result

def jqry(mexp, moptions={}):
    """ convert a mongo query with options to a jmespath expression """
    log.debug('create jmespath query for mongo exp:%s options:%s', json.dumps(mexp), json.dumps(moptions))
    result = '[?'+jexp(mexp)+']'
    log.debug('jmespath expression is %s', result)
    if 'fields' in moptions:
        fields = moptions['fields']
        result += '.{'
        for key in fields.keys():
            result += key+':'+key+', '
        result = result[:-2]
        result += '}'
    if 'sort' in moptions:
        sortKeys = moptions['sort']
        orderedKeys = OrderedDict()
        if isinstance(sortKeys, OrderedDict):
            orderedKeys = sortKeys
        elif isinstance(sortKeys, dict):
            orderedKeys = sortKeys
            if len(sortKeys.keys())>1:
                log.warning('mongo sort options dictionary is unordered and priority unknown, use list format instead! mexp:%s moptions:%s', json.dumps(mexp), json.dumps(moptions))
        elif isinstance(sortKeys, list):
            for item in sortKeys:
                if isinstance(item, list):
                    orderedKeys[item[0]] = 1 if str(item[1]).lower()=='asc' else '-1'
                else:
                    orderedKeys[item] = 1
        for key in orderedKeys.keys()[::-1]:
            if orderedKeys[key]==1:
                result = 'sort_by('+result+', &'+key+')'
            else:
                result = 'sort_by(reverse('+result+'), &'+key+')[::-1]'
    log.debug('jmespath query is %s', result)
    return result
