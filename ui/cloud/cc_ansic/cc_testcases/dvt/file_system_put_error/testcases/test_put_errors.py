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

DVT_BUSY_OFFSET = 1
DVT_START_OFFSET = 2
DVT_MIDDLE_OFFSET = 3
DVT_END_OFFSET = 4
DVT_TIMEOUT_OFFSET = 5

class FileSystemErrorTestCase(cc_testcase.TestCase):

    def test_put_error(self):
        """ Tests file put with error returned by the connector at start, middle and end of write. The application determines when to send the errors because of the offset. """
        fname = "dvt_fs_put_error.test"
        global FileData

        FileData = ""
        for i in xrange(MAX_TEST_FILE_SIZE):
            FileData += chr(randint(0, 255))
    
        fsize = len(FileData)    
        outData = base64.encodestring(FileData)
        self.verify_put_file(fname, outData, fsize, errorExpected=False, offset=0) # Upload the file
        self.verify_put_file(fname, outData, fsize, errorExpected=True, offset=DVT_START_OFFSET, errorString="Permission denied")
        self.verify_put_file(fname, outData, fsize, errorExpected=True, offset=DVT_MIDDLE_OFFSET, errorString="Invalid argument")
        self.verify_put_file(fname, outData, fsize, errorExpected=True, offset=DVT_END_OFFSET, errorString="Cannot allocate memory")
        self.verify_put_file(fname, outData, fsize, errorExpected=False, offset=DVT_BUSY_OFFSET)

        for i in xrange(MAX_TEST_PUT_SIZE - fsize):
            FileData += chr(randint(0, 255))
        outData = base64.encodestring(FileData)
        fsize = len(FileData)

        self.verify_put_file(fname, outData, fsize, errorExpected=True, offset=DVT_TIMEOUT_OFFSET, errorString="Connection timed out")

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
