# -*- coding: utf-8 -*-
import time
import cc_testcase
import datetime
import re
import os
import base64

from random import randint

MAX_TEST_FILE_SIZE = 4321
MAX_TEST_PUT_SIZE = MAX_TEST_FILE_SIZE * 2 + 1000
FileData = ""
TEST_ERROR_BUSY = 0
TEST_ERROR_AT_START = 1
TEST_ERROR_AT_MIDDLE = 2
TEST_ERROR_AT_END = 3
TEST_ERROR_TIMEOUT = 4
TEST_ERROR_TIMEOUT_MIDDLE = 5

class FileSystemErrorTestCase(cc_testcase.TestCase):

    def test_rm_errors(self):
        """ Tests rm with file not there and rm timeout """
        fname = "dvt_fs_rm_tout.test"
        putData = "abcd"
        fsize = len(putData)
        outData = base64.encodestring(putData)
        self.verify_put_file(fname, outData, fsize, errorExpected=False)
        self.verify_remove_file(fname, errorExpected=True, errorString="Connection timed out") # dvt_fs_error_rm_timeout

    def verify_put_file(self, fpath, fdata, fsize, errorExpected, offset = 0, truncate = False, errorString=""):
        putRequest = \
        """          <sci_request version="1.0">
          <file_system>
            <targets>
              <device id="%s"/>
            </targets>
            <commands>
               <put_file path="%s" offset="%d" truncate="%r">
                  <data>%s</data>
               </put_file>
            </commands>
          </file_system>
          </sci_request>""" % (self.device_id, fpath, offset, truncate, fdata)

        self.log.info("put request file:\"%s\", offset:%d, size:%d" % (fpath, offset, fsize))

        response = self.cloudHandler.postRequest("sci", data = putRequest)
        # Check query was successfully
        if (response.status_code != 200):
            self.fail("Unexpected response from the Cloud")

        self.verify_device_response(response.resource["sci_reply"]["file_system"]["device"]["commands"]["put_file"], errorExpected, errorString)

    def verify_remove_file(self, path, errorExpected, errorString=""):
        rmRequest = \
            """<sci_request version="1.0">
              <file_system>
                <targets>
                  <device id="%s"/>
                </targets>
                <commands>
                  <rm path="%s"/>
                </commands>
              </file_system>
            </sci_request>""" % (self.device_id, path)

        self.log.info("Sending rm request for \"%s\" " % path)
        response = self.cloudHandler.postRequest("sci", data = rmRequest)
        # Check query was successfully
        if (response.status_code != 200):
            self.fail("Unexpected response from the Cloud")
            
        self.verify_device_response(response.resource["sci_reply"]["file_system"]["device"]["commands"]["rm"], errorExpected, errorString)

    def verify_device_response(self, command_response, errorExpected, errorString=""):
        # command_response, for example: response["sci_reply"]["file_system"]["device"]["commands"]["get_file"]
        self.log.info("command_response [%s]" % command_response)
        if (command_response is not None):
            if ("error" in command_response):
                errorActual = True
                error_msg = "Expected success, but got error"
                if ("#text" in command_response["error"]):
                    response_error_string = command_response["error"]["#text"]
                else:
                    response_error_string = command_response["error"]
            else:
                errorActual = False
                error_msg = "Expected error, but got success"
        else:
            errorActual = False
            error_msg = "Expected error, but got success"
            response_error_string = ""

        self.assertTrue(errorExpected == errorActual, "%s, response[%s]" %(error_msg, command_response))
        if (errorExpected):
            self.log.info("Error string [%s], expected [%s]" % (response_error_string, errorString))
            self.assertTrue(response_error_string == errorString, "%s, response[%s]" %(response_error_string, errorString))
        self.log.info("Test success!")

if __name__ == '__main__':
    unittest.main() 
