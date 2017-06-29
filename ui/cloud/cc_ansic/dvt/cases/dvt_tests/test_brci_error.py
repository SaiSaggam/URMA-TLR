from requests import post
import xpath
from random import randrange
from time import sleep
from string import Template
from nose.tools import *
from xml.dom.minidom import parseString,getDOMImplementation
import ic_testcase
import logging
dom = getDOMImplementation()

from ic_plugin import ICPlugin

log = logging.getLogger('ic_testcase')
log.setLevel(logging.INFO)
if len(log.handlers) == 0:
    handler = logging.StreamHandler()
    handler.setLevel(logging.INFO)
    formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
    handler.setFormatter(formatter)
    log.addHandler(handler)

logging.getLogger('requests').setLevel(logging.WARNING)

##################################################################
#Templates for XML to send.
##################################################################
def parse_error(doc):
    errors = xpath.find('//error',doc)
    if len(errors) > 0:
        error = errors[0]

        desc = xpath.find('desc/text()', error)
        if len(desc) > 0:
            desc = desc[0].data

        hint = xpath.find('hint/text()', error)
        if len(hint) > 0:
            hint = hint[0].data

        return (error.getAttribute('id'), desc, hint)
    return None

##################################################################


def send_rci(request):
    try:
        req_data = parseString(request).toxml()
    except Exception, e:
        error = "Invalid XML passed in: %s"%request
        assert 0==1, error
    log.info("Sending SCI Request: \n%s" % request)
    response = ICPlugin.session.post('https://%s/ws/sci' % ICPlugin.hostname,
                    data=request,
                    verify=False)

    assert_equal(200, response.status_code, "Non 200 Status Code: %d.  " \
        "Response: %s" % (response.status_code, response.content))
    try:
        res_data = parseString(response.content)
        log.info("Received SCI Response: \n%s" \
            % res_data.toprettyxml(indent=' '))
        return res_data
    except Exception, e:
        error = "Response was not valid XML: %s" % response.content
        assert 0==1, error

class RciErrorDvtTestCase(ic_testcase.TestCase):

    def set_setting(self, name, error_id, error_desc, error_hint):

        rci = \
            """<sci_request version="1.0">
              <send_message cache="false">
                <targets>
                  <device id="%s"/>
                </targets>
                <rci_request version="1.1">
                  <set_setting>
                    <test_error><%s/></test_error>
                  </set_setting>
                </rci_request>
              </send_message>
            </sci_request>""" % (self.device_id, name)

        doc = send_rci(rci)

        error = parse_error(doc)
        if error_id != 0:
            assert_true(error_id == int(error[0]), "Error not match: (expect: %d %d) %s " % (error_id, int(error[0]), error,))
            assert_true(error_desc == error[1], "Error description not match: (expect: %s) %s " % (error_desc, error,))
            if len(error_hint) > 0:
                assert_true(error_hint == error[2], "Error hint not match: (expect: %s) %s " % (error_hint, error,))
        else:
            assert_true(error is None, "Error found: %s " % (error,))

    def test_start_session(self):
        self.set_setting("start_session", 0, "", "")
        self.set_setting("start_session", 5, "Insufficient memory", "")

    def test_start_action(self):
        self.set_setting("start_action", 0, "", "")
        self.set_setting("start_action", 5, "Insufficient memory", "")

    def test_start_group(self):
        self.set_setting("start_group", 0, "", "")
        self.set_setting("start_group", 5, "Insufficient memory", "")

    def test_process_group(self):
        self.set_setting("process_group", 0, "", "")
        self.set_setting("process_group", 3, "Load fail", "Hint")

    def test_end_group(self):
        self.set_setting("end_group", 5, "Insufficient memory", "")

    def test_end_action(self):
        self.set_setting("end_action", 5, "Insufficient memory", "")

    def test_end_session(self):
        self.set_setting("end_session", 5, "Insufficient memory", "")
