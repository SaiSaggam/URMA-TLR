# -*- coding: utf-8 -*-
import time
import cc_testcase
import datetime
import re
import os
import base64

from random import randint

# These tests do not need modifications in the file_system.c file.

class FileSystemErrorTestCase(cc_testcase.TestCase):

    def test_invalid_path(self):
        """ Tests get, put, rm and ls on a path which is not found """
        fname = "dummy/test_file.error"
        putData = "abcd"
        outData = base64.encodestring(putData)
        fsize = len(putData)

        self.verify_get_file(fname, fsize, errorExpected=True, errorString="No such file or directory")
        self.verify_put_file(fname, outData, fsize, errorExpected=True, errorString="No such file or directory")
        self.verify_remove_file(fname, errorExpected=True, errorString="No such file or directory")
        self.verify_file_list(fname, errorExpected=True, errorString="No such file or directory")

    def test_invalid_offset(self):
        """ Tests get, put with invalid offset """
        fname = "test_file.error"
        putData = "abcd"
        fsize = len(putData)

        outData = base64.encodestring(putData)
        self.verify_put_file(fname, outData, fsize, errorExpected=True, offset=2, errorString="Invalid offset") # Try to write in a non-existent file with offset != 0.
        self.verify_put_file(fname, outData, fsize, errorExpected=False) # Put file, no error
        self.verify_put_file(fname, outData, fsize, errorExpected=True, offset=fsize+1, errorString="Invalid offset") # Try to write out of range.
        self.verify_get_file(fname, fsize, errorExpected=True, offset=fsize+1, errorString="Invalid offset") # Try to read out of range.
        self.verify_remove_file(fname, errorExpected=False) # Remove the file, no error expected.

    def test_rm_error(self):
        """ Tests rm with file not there and rm timeout """
        fname = "dvt_fs_rm_tout.test"
        putData = "abcd"
        fsize = len(putData)
        outData = base64.encodestring(putData)

        self.verify_put_file(fname, outData, fsize, errorExpected=False)
        self.verify_remove_file(fname, errorExpected=False)
        self.verify_remove_file(fname, errorExpected=True, errorString="No such file or directory") # File is not existent

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

    def verify_get_file(self, fpath, fsize, errorExpected, offset = 0, errorString=""):
        getRequest = \
            """<sci_request version="1.0">
              <file_system>
                <targets>
                  <device id="%s"/>
                </targets>
                <commands>
                    <get_file path="%s" offset = "%d" length = "%d"/>
                </commands>
              </file_system>
              </sci_request>""" % (self.device_id, fpath, offset, fsize)

        self.log.info("get request file:\"%s\", offset:%d, size:%d" % (fpath, offset, fsize))
        response = self.cloudHandler.postRequest("sci", data = getRequest)
        # Check query was successfully
        if (response.status_code != 200):
            self.fail("Unexpected response from the Cloud")
        if ("commands" in response.resource["sci_reply"]["file_system"]["device"]):
            # Normal case
            self.verify_device_response(response.resource["sci_reply"]["file_system"]["device"]["commands"]["get_file"], errorExpected, errorString)
        else:
            # For some reason, with no-compression test_fs5_get_error's timeout returns the error at device-level instead of command-level
            self.verify_device_response(response.resource["sci_reply"]["file_system"]["device"], errorExpected, errorString)

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

if __name__ == '__main__':
    unittest.main() 
