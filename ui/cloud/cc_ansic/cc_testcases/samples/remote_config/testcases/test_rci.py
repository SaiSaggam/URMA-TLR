import cc_testcase
import time

from requests import post
import xpath
from time import sleep
from string import Template
from nose.tools import *
import logging
from string import Template
from random import randrange, choice

RCI_BASE_TEMPLATE = Template("""<sci_request version="1.0">
  <send_message cache="${cache}">
    <targets>
      <device id="${device_id}"/>
    </targets>
    <rci_request ${rci_attributes}>
      ${request}
    </rci_request>
  </send_message>
</sci_request>""")

DEFAULT_RCI_ATTRIBUTES='version="1.1"'
DEFAULT_RCI_CACHE="false"

class TestCase(object):

    def __init__(self, test, error, name=''):
        self.test=test
        self.error=error
        self.name=name

class RCIRequest(object):

    def __init__(self,rci,error,name='',
                    rci_attributes=DEFAULT_RCI_ATTRIBUTES,
                    cache=DEFAULT_RCI_CACHE):
        self.rci=rci
        self.name=name
        self.error=error
        self.rci_attributes=rci_attributes
        self.cache=cache

    def __repr__(self):
        return "test_%s"%self.name


class ConnectionTestCase(cc_testcase.TestCase):

    def send_rci(self, request):
        self.log.info("Sending SCI Request: \n%s" % request)
        response = self.cloudHandler.postRequest("sci", data = request)
        if (response.status_code != 200):
            self.fail("Unexpected response from the Cloud")
        self.log.info("Received SCI Response: \n%s" % response.content)
        return response.resource

    def parse_error(self, doc):
        print "DOC = %s" % doc
        if (doc["sci_reply"]["send_message"]["device"].has_key("error")):
            error = doc["sci_reply"]["send_message"]["device"]["error"]
            print "ERROR = %s" % error
            desc = doc["sci_reply"]["send_message"]["device"]["error"]["desc"]
            print "DESC = %s" % desc
            if (error.has_key("desc")):
                desc = error["desc"]
                print "DESC = %s" % desc
            else:
                desc = ''
            if (error.has_key("hint")):
                hint = error["hint"]
            else:
                hint = ''

            return (error["@id"], desc, hint)
        return None



    def send_request(self, rci_request):
        request = RCI_BASE_TEMPLATE.substitute(cache=rci_request.cache,
                                                device_id=self.device_id,
                                                request=rci_request.rci,
                                                rci_attributes=rci_request.rci_attributes)

        doc = self.send_rci(request)

        error = self.parse_error(doc)
        if error:
            self.log.info('Error: %s.  Description: %s.  Hint: %s' % error)
            perr=error[1]
        else:
            perr=None
        self.assertEqual(perr,rci_request.error, 'Expected: "%s" Received: "%s"' % (rci_request.error,perr))


    #def test_bad_rci_versions(self):
        #tests = [
                #TestCase('version="2.0"',
                            #'Invalid version',
                            #'version_2.0'),
                #TestCase('version="1.0"',
                            #'Invalid version',
                            #'version_1.0'),
                #TestCase('',
                            #None,
                            #'version_blank'),
                #]

        #for test in tests:
            #request = RCIRequest('<query_state/>',
                                    #test.error,
                                    #'bad_rci_%s'%test.name,
                                    #test.test)
            #return self.send_request(request)



    def test_query_descriptor(self):
        tests = [
                TestCase('<query_descriptor/>',
                            'Invalid BinaryRCI request submited',
                            'query_descriptor_only'),
                TestCase('<query_descriptor></query_descriptor>',
                            'Invalid BinaryRCI request submited',
                            'query_descriptor_blank'),
                TestCase('<query_descriptor><query_setting/></query_descriptor>',
                            'Invalid BinaryRCI request submited',
                            'query_setting'),
                TestCase('<query_descriptor><query_state/></query_descriptor>',
                            'Invalid BinaryRCI request submited',
                            'query_state'),
                ]

        for test in tests:
            request = RCIRequest(test.test,
                                    test.error,
                                    'query_descriptor_%s'%test.name)
            return self.send_request(request)

    def get_string(self, value):
        chars = []
        for _ in xrange(value):
            rand = randrange(65,90)
            chars.append(chr(rand))
        return ''.join(chars)


    def test_query_setting_and_state(self):
        tests = [
                TestCase('<query_setting><unkownGroup/></query_setting>','Invalid BinaryRCI request submited','query_setting_unknown_group'),
                TestCase('<query_state><unkownGroup/></query_state>','Invalid BinaryRCI request submited','query_state_unknown_group'),
                TestCase('<query_setting><serial/></query_setting>',None,'query_setting_no_index_group'),
                TestCase('<query_state><device_state/></query_state>',None,'query_state_no_index_group'),
                TestCase('<query_setting><serial index="10"></serial></query_setting>',"Bad index",'query_setting_invalid_index_group'),
                TestCase('<query_state><device_state index="10"></device_state></query_state>',"Bad index",'query_state_invalid_index_group'),
                TestCase('<query_setting><serial/><system/></query_setting>',None,'query_setting_multiple_groups'),
                TestCase('<query_state><gps_stats/><device_state/></query_state>',None,'query_state_multiple_groups'),
                TestCase('<query_setting><serial/></query_setting>',None,'query_setting_no_element_1'),
                TestCase('<query_state><device_state/></query_state>',None,'query_state_no_element_1'),
                TestCase('<query_setting><serial></serial></query_setting>',None,'query_setting_no_element_2'),
                TestCase('<query_state><device_state></device_state></query_state>',None,'query_state_no_element_2'),
                TestCase('<query_setting><serial><unknownElement/></serial></query_setting>','Bad element','query_setting_unknown_element'),
                TestCase('<query_state><device_state><unknownElement/></device_state></query_state>','Bad element','query_state_unknown_element'),
                ]


        for test in tests:
            request = RCIRequest(test.test,test.error,test.name)
            return self.send_request(request)

    def test_set_setting_and_state(self):
        tests = [
                TestCase('<set_setting><unkownGroup><someElement>12345</someElement></unkownGroup></set_setting>','Invalid BinaryRCI request submited','set_setting_unknown_group'),
                TestCase('<set_state><unkownGroup><someElement>12345</someElement></unkownGroup></set_state>','Invalid BinaryRCI request submited','set_state_unknown_group'),
                TestCase('<set_setting><system><description>Testing No Index</description></system></set_setting>',None,'set_setting_no_index_group'),
                TestCase('<set_state><device_state><signed_integer>45</signed_integer></device_state></set_state>',None,'set_state_no_index_group'),
                TestCase('<set_setting><system index="10"><description>Testing Invalid Index</description></system></set_setting>',"Bad index",'set_setting_invalid_index_group'),
                TestCase('<set_state><device_state index="10"><signed_integer>45</signed_integer></device_state></set_state>',"Bad index",'set_state_invalid_index_group'),
                TestCase('<set_setting><system><description>Testing Multiple Groups</description><contact>Testing Contact</contact></system></set_setting>',None,'set_setting_multiple_groups'),
                #TestCase('<set_state><device_state/><device_state/></set_state>',None,'set_state_multiple_groups'),
                TestCase('<set_setting><system/></set_setting>','Bad element','set_setting_no_element_1'),
                TestCase('<set_state><device_state/></set_state>','Bad element','set_state_no_element_1'),
                TestCase('<set_setting><system></system></set_setting>','Bad element','set_setting_no_element_2'),
                TestCase('<set_state><device_state></device_state></set_state>','Bad element','set_state_no_element_2'),
                TestCase('<set_setting><system><unknownElement>Testing Unkown Element</unknownElement></system></set_setting>','Bad element','set_setting_unknown_element'),
                TestCase('<set_state><device_state><unknownElement/></device_state></set_state>','Bad element','set_state_unknown_element'),
                TestCase('<set_setting><system><description>asdfghjklaasdfghjklaasdfghjklaasdfghjklaasdfghjklaasdfghjklaasdfghjkla</description></system></set_setting>','Bad value','set_setting_out_of_range'),
                TestCase('<set_state><device_state><signed_integer>9999999999999999999999999999999999999999999999999999999999999999999999999999999</signed_integer></device_state></set_state>','Bad value','set_state_very_out_of_range_above'),
                TestCase('<set_state><device_state><signed_integer>-9999999999999999999999999999999999999999999999999999999999999999999999999999999</signed_integer></device_state></set_state>','Bad value','set_state_very_out_of_range_below'),
                TestCase('<set_state><device_state><signed_integer>101</signed_integer></device_state></set_state>','Bad value','set_state_out_of_range_above'),
                TestCase('<set_state><device_state><signed_integer>-101</signed_integer></device_state></set_state>','Bad value','set_state_out_of_range_below'),
                TestCase("""<set_setting><system><description>Testing No Index
                    Multiple lines
                    to be bad test.</description></system></set_setting>""",'Bad value','set_setting_bad_type'),
                TestCase('<set_state><device_state><signed_integer>Some string value</signed_integer></device_state></set_state>','Bad value','set_state_bad_type'),
                ]


        for test in tests:
            request = RCIRequest(test.test,test.error,test.name)
            return self.send_request(request)


    def rci_length_verification(self, rci_request):
        baud_list=[2400,4800,9600,19200,38400,57600,115200,230400]
        requests = [RCI_BASE_TEMPLATE.substitute(cache=rci_request.cache,
                                                device_id=self.device_id,
                                                request=rci_request.rci,
                                                rci_attributes=rci_request.rci_attributes),
                    RCI_BASE_TEMPLATE.substitute(cache=DEFAULT_RCI_CACHE,
                                                device_id=self.device_id,
                                                request='<query_setting><device_info><company/></device_info></query_setting>',
                                                rci_attributes=DEFAULT_RCI_ATTRIBUTES),
                    RCI_BASE_TEMPLATE.substitute(cache=DEFAULT_RCI_CACHE,
                                                device_id=self.device_id,
                                                request='<set_setting><serial index="2"><baud>%s</baud></serial></set_setting>'%choice(baud_list),
                                                rci_attributes=DEFAULT_RCI_ATTRIBUTES),
                    RCI_BASE_TEMPLATE.substitute(cache=DEFAULT_RCI_CACHE,
                                                device_id=self.device_id,
                                                request='<query_setting><serial index="2"><baud/></serial></query_setting>',
                                                rci_attributes=DEFAULT_RCI_ATTRIBUTES),
                    ]

        for request in requests:

            doc = self.send_rci(request)

            error = self.parse_error(doc)
            if error:
                self.log.info('Error: %s.  Description: %s.  Hint: %s' % error)
                perr=error[1]
            else:
                perr=None
            self.assertEqual(perr,None, 'Expected: "%s" Recieved: "%s"' % (rci_request.error,perr))

    def test_set_query_zero_to_large(self):
        start_length = 0
        end_length = 25
        step_length = 5
        tests = []
        for i in xrange(start_length,end_length,step_length):
            tests.append(TestCase('<set_setting><device_info><company>%s</company></device_info></set_setting>' % self.get_string(i),
                None,
                'set_setting_various_lengths_%s'%i))

        for test in tests:
            request = RCIRequest(test.test,test.error,test.name)
            return self.rci_length_verification(request)
