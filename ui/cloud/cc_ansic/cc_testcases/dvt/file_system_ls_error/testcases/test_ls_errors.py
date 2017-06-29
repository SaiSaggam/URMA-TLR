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

    def test_ls_error(self):
        """ Tests ls errors, the applicaiton is hardcoded to return errors as response for these commands """
        self.verify_file_list("./source", errorExpected=False) # dvt_fs_error_ls_busy
        self.verify_file_list("./testharness/device", errorExpected=True, errorString="Permission denied") # dvt_fs_error_ls_start
        self.verify_file_list("./testharness/host", errorExpected=True, errorString="Cannot allocate memory") # dvt_fs_error_ls_end
        self.verify_file_list("./testharness/libraries/ansic", errorExpected=True, errorString="Invalid argument") # dvt_fs_error_ls_middle
        self.verify_file_list("./testharness/sares/ExceptionLibrary", errorExpected=True, errorString="Connection timed out") # dvt_fs_error_ls_timeout
        file_list, dir_list = self.verify_file_list("./testharness/test_files", hashAlgo="md5",errorExpected=False) # dvt_fs_error_ls_invalid_hash
        self.assertFalse("hash" in file_list[0]) # Hash is none, so it's not reported

    def verify_file_list(self, path, errorExpected, hashAlgo="none", errorString=""):
        lsRequest = \
            """<sci_request version="1.0">
              <file_system>
                <targets>
                  <device id="%s"/>
                </targets>
                <commands>
                  <ls path="%s" hash="%s"/>
                </commands>
              </file_system>
            </sci_request>""" % (self.device_id, path, hashAlgo)

        self.log.info("ls for path:\"%s\" and hash:%s" % (path, hashAlgo))
        response = self.cloudHandler.postRequest("sci", data = lsRequest)
        # Check query was successfully
        if (response.status_code != 200):
            self.fail("Unexpected response from the Cloud")
        ls_command = response.resource["sci_reply"]["file_system"]["device"]["commands"]["ls"]
        self.verify_device_response(ls_command, errorExpected, errorString)
        file_list = None
        dir_list = None
        if (not errorExpected):
            if "file" in ls_command:
                file_list = ls_command["file"]
            if "dir" in ls_command:
                dir_list = ls_command["dir"]

        return file_list, dir_list

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
