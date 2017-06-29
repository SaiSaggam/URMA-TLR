import os
# Run tests from the device directory
os.chdir('device')

import sys
import time
sys.path.append(".")
sys.path.append("lib")
import jmespath
from jpath import jexp, jqry
import copy
import unittest
from collections import OrderedDict

class JexpTestCase(unittest.TestCase):
    def setUp(self):
        self.p1 = {'name':'dick', 'age':34, 'sex':'m', 'status':'married'}
        self.p2 = {'name':'jane', 'age':31, 'sex':'f', 'status':'married'}
        self.p3 = {'name':'billy', 'age':7, 'sex':'m', 'status':'single'}
        self.p4 = {'name':'suzy-q', 'age':5, 'sex':'f', 'status':'single'}
        self.f = [self.p1,self.p2,self.p3,self.p4]

    def tearDown(self):
        pass

    def test_basic_expression(self):
        q = {'name':'jane'}
        self.assertEqual(jexp(q), '(name==`jane`)')
        self.assertEqual(jqry(q), '[?(name==`jane`)]')
        t = {'fields':{'name':1,'age':1,'sex':1}}
        self.assertEqual(jqry(q,t), '[?(name==`jane`)].{age:age, name:name, sex:sex}')
        self.assertEqual(jmespath.search(jqry(q),self.f), [self.p2])
        p2_filtered = copy.deepcopy(self.p2)
        del p2_filtered['status']
        self.assertEqual(jmespath.search(jqry(q,t),self.f), [p2_filtered])

    def test_sort_expression(self):
        q = {}
        self.assertEqual(jexp(q), '*')
        self.assertEqual(jqry(q), '[?*]')

        # t = {'sort':{'sex':-1}}
        t = {'sort': OrderedDict([('sex', -1)])}
        qry = jqry(q,t)
        self.assertEqual(jqry(q,t), 'sort_by(reverse([?*]), &sex)[::-1]')
        print 'sorted results: ', jmespath.search(qry,self.f)
        expectedOrder = [self.p1, self.p3, self.p2, self.p4]
        self.assertEqual(jmespath.search(qry,self.f), expectedOrder)

        # t = {'sort':{'sex':-1, 'age':1}}
        t = {'sort': OrderedDict([('sex', -1),('age',1)])}
        qry = jqry(q,t)
        self.assertEqual(jqry(q,t), 'sort_by(reverse(sort_by([?*], &age)), &sex)[::-1]')
        print 'sorted results: ', jmespath.search(qry,self.f)
        expectedOrder = [self.p3, self.p1, self.p4, self.p2]
        self.assertEqual(jmespath.search(qry,self.f), expectedOrder)

        t = {'sort': OrderedDict([('status', -1),('sex',-1)])}
        qry = jqry(q,t)
        print 'sorted results: ', jmespath.search(qry,self.f)
        expectedOrder = [self.p3, self.p4, self.p1, self.p2]
        self.assertEqual(jmespath.search(qry,self.f), expectedOrder)

        t = {'sort': [['sex', 'desc'],['age','asc']]}
        qry = jqry(q,t)
        self.assertEqual(jqry(q,t), 'sort_by(reverse(sort_by([?*], &age)), &sex)[::-1]')
        print 'sorted results: ', jmespath.search(qry,self.f)
        expectedOrder = [self.p3, self.p1, self.p4, self.p2]
        self.assertEqual(jmespath.search(qry,self.f), expectedOrder)

        t = {'sort': [['sex', 'desc'],'age']}
        qry = jqry(q,t)
        self.assertEqual(jqry(q,t), 'sort_by(reverse(sort_by([?*], &age)), &sex)[::-1]')
        print 'sorted results: ', jmespath.search(qry,self.f)
        expectedOrder = [self.p3, self.p1, self.p4, self.p2]
        self.assertEqual(jmespath.search(qry,self.f), expectedOrder)

        t = {'sort': [['status', 'desc'],['sex', 'desc']]}
        qry = jqry(q,t)
        print 'sorted results: ', jmespath.search(qry,self.f)
        expectedOrder = [self.p3, self.p4, self.p1, self.p2]
        self.assertEqual(jmespath.search(qry,self.f), expectedOrder)


    def test_sub_expression(self):
        q = {'name':{'$eq':'jane'}}
        self.assertEqual(jexp(q), '(name==`jane`)')
        q = {'name':{'$ne':'jane'}}
        self.assertEqual(jexp(q), '(name!=`jane`)')
        q = {'age':{'$gt':'20'}}
        self.assertEqual(jexp(q), '(age>`20`)')
        q = {'age':{'$gte':'20'}}
        self.assertEqual(jexp(q), '(age>=`20`)')
        q = {'age':{'$lt':'20'}}
        self.assertEqual(jexp(q), '(age<`20`)')
        q = {'age':{'$lte':'20'}}
        self.assertEqual(jexp(q), '(age<=`20`)')
        self.assertEqual(jmespath.search(jqry(q),self.f), [self.p3, self.p4])

    def test_logical_expression(self):
        q = {'$and':[{'sex':'f'},{'status':'single'}]}
        self.assertEqual(jexp(q), '(((sex==`f`) && (status==`single`)))')
        self.assertEqual(jmespath.search(jqry(q),self.f), [self.p4])
        q = {'$or':[{'sex':'f'},{'status':'single'}]}
        self.assertEqual(jexp(q), '(((sex==`f`) || (status==`single`)))')
        self.assertEqual(jmespath.search(jqry(q),self.f), [self.p2,self.p3,self.p4])

from deviceServices import DeviceConfigManager

class DeviceServicesTestCase(unittest.TestCase):
    def setUp(self):
        token = os.environ.get('CONFIG_MANAGER_API_TOKEN')
        self.dcm = DeviceConfigManager(token)
        self.h1data = None
        self.h2data = None
        self.selectiveData = {}

    def tearDown(self):
        self.dcm.halt()

    def msgHandler1(self, deviceServiceMsg):
        self.h1data = deviceServiceMsg

    def msgHandler2(self, deviceServiceMsg):
        self.h2data = deviceServiceMsg

    def test_handler_registration(self):
        self.dcm.addHandler(self.msgHandler1, self)
        self.dcm.addHandler(self.msgHandler2, self)
        xmlMsg = """
            <settings>
                <groupX>
                    <f1>101</f1>
                    <f2>102</f2>
                    <f3/>
                </groupX>
                <groupX index="2">
                    <f1>201</f1>
                    <f2>202</f2>
                    <f3/>
                </groupX>
                <groupY>
                    <fa>a</fa>
                </groupY>
            </settings>"""
        DeviceConfigManager._globalHandler(xmlMsg)
        self.assertNotEqual(self.h1data, None)
        self.assertNotEqual(self.h2data, None)
        self.assertEqual(self.h1data, self.h2data)
        self.assertEqual(self.h1data.keys()[0],'settings')
        self.assertEqual(self.h1data.keys()[0],'settings')

    def selectiveHandler(self, deviceServiceMsg):
        print 'selectiveHandler received data: ', deviceServiceMsg
        dataType = deviceServiceMsg.keys()[0]
        if self.selectiveData.get(dataType)==None:
            self.selectiveData[dataType] = {}
        for group in deviceServiceMsg[dataType].keys():
            self.selectiveData[dataType][group] = deviceServiceMsg[dataType][group]

    def test_ddp_if(self):
        self.dcm.addHandler(self.selectiveHandler, self)
        self.dcm.init()
        self.assertEqual(len(self.selectiveData.keys()), 2)
        settings = self.selectiveData['settings']
        self.assertNotEqual(settings, None)
        self.assertNotEqual(settings['system'], None)
        self.assertNotEqual(settings['eth'], None)
        self.assertNotEqual(self.selectiveData['state'], None)
        self.assertEqual(self.selectiveData['state']['system']['system_time'], '19 October 2015, 22:27:35')
        pollId = self.dcm.poll('system', 1, 1)
        time.sleep(5)
        self.assertNotEqual(self.selectiveData['state']['system']['system_time'], '19 October 2015, 22:27:35')
        self.dcm.cancel_poll(pollId)
        currtime = self.selectiveData['state']['system']['system_time']
        time.sleep(5)
        self.assertEqual(self.selectiveData['state']['system']['system_time'], currtime)




   # def test_dash-issue(self):
   #     q = "[{u'_groupName': u'eth'}, {u'sort': {u'_groupIndex': 1}, u'fields': {u'_groupIndex': 1, u'name': 1, u'ip-address': 1, u'description': 1}}]"

#
# TODO: figure out how to use mocks effectively
#
# from mymodule import rm
#
# import mock
# import unittest
#
# class DSTestCase(unittest.TestCase):
#
#     @mock.patch('DeviceConfigManager.')
#     def test_rm(self, mock_os):
#         rm("any path")
#         # test that rm called os.remove with the right parameters
#         mock_os.remove.assert_called_with("any path")
#
