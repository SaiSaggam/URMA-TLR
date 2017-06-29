# -*- coding: utf-8 -*-
import time
import ic_testcase
import datetime
import re
import os
import base64
import xml.dom.minidom
from time import sleep

from random import randint
from xml.dom.minidom import getDOMImplementation
impl = getDOMImplementation()

from ..utils import getText

MAX_TEST_FILE_SIZE = 3000
dummyFsize = 4
expectError = True
noCleanup = False

noFileRequest = \
            """<sci_request version="1.0">
              <file_system>
                <targets>
                  <device id="%s"/>
                </targets>
                <commands>
                    <get_file path="ThisFileDoesNotExist" offset = "0" length = "10"/>
                </commands>
              </file_system>
              </sci_request>"""

fileGetRequest = \
            """<sci_request version="1.0">
              <file_system>
                <targets>
                  <device id="%s"/>
                </targets>
                <commands>
                    <get_file path="%s" offset = "%d" length = "%d"/>
                </commands>
              </file_system>
              </sci_request>"""

filePutRequest = \
        """<sci_request version="1.0">
          <file_system>
            <targets>
              <device id="%s"/>
            </targets>
            <commands>
               <put_file path="%s" offset = "%d" truncate = "%r">
                  <data>%s</data>
               </put_file>
            </commands>
          </file_system>
          </sci_request>"""

fileRmRequest = \
            """<sci_request version="1.0">
              <file_system>
                <targets>
                  <device id="%s"/>
                </targets>
                <commands>
                  <rm path="%s"/>
                </commands>
              </file_system>
            </sci_request>"""

class FileSystemCoverageTestCase(ic_testcase.TestCase):

    out_size = 0
    outData = ""

    def setUp(self):
        ic_testcase.TestCase.setUp(self)

        FileData = ""
        for i in xrange(MAX_TEST_FILE_SIZE):
            FileData += chr(randint(0, 255))
        self.out_size = len(FileData)
        self.outData = base64.encodestring(FileData)
    
    @classmethod
    def tearDownClass(cls):
        ic_testcase.TestCase.tearDownClass()

    def test_fs01_open_abort(self):
        """ Tests abort conditions in file open callback """
        self.verify_get_file_abort("dvt_fs_open_abort.test", dummyFsize)
        self.verify_get_file_abort("dvt_fs_open_retinvalid.test", dummyFsize)
        self.verify_get_file_abort("dvt_fs_open_set_null_handle.test", dummyFsize)
        self.log.info("Test success!")

    def test_fs02_open_canceled(self):
        """ Tests canceled sessions in file open callback """
        self.verify_get_file_user_canceled("dvt_fs_open_unrecognized.test", dummyFsize)
        self.log.info("Test success!")

    def test_fs03_open_errors(self):
        """ Tests errors returned sessions in file open callback """
        self.verify_get_file("dvt_fs_open_io_error.test", dummyFsize, expectError)
        self.verify_get_file("dvt_fs_open_io_error_no_errno.test", dummyFsize, expectError)
        self.log.info("Test success!")

    def test_fs04_open_busy(self):
        """ Tests connector_callback_busy return in file open callback """
        self.check_get_file_response("dvt_fs_open_busy.test")
        self.log.info("Test success!")

    def test_fs05_strerr_get_abort(self):
        """ Tests abort conditions in file get strerr callback """
        self.verify_get_file_abort("dvt_fs_strerr_abort.test", dummyFsize)
        self.verify_get_file_abort("dvt_fs_strerr_retinvalid.test", dummyFsize)
        self.verify_get_file_abort("dvt_fs_strerr_bad_datalen.test", dummyFsize)
        self.log.info("Test success!")

    def test_fs06_strerr_get(self):
        """ Tests ignored stuff file strerr get callback """
        self.verify_get_file("dvt_fs_strerr_zero_datalen.test", dummyFsize, expectError)
        self.log.info("Test success!")

    def test_fs07_read_abort(self):
        """ Tests abort conditions in file read callback """
        self.check_get_file_abort("dvt_fs_read_retinvalid.test")
        self.check_get_file_abort("dvt_fs_read_abort.test")
        self.check_get_file_abort("dvt_fs_read_invalid_datalen.test")

    def test_fs08_read_busy(self):
        """ Tests connector_callback_busy, 0 data bytes return in file read callback """
        self.check_get_file_response("dvt_fs_read_busy.test")
        self.check_get_file_response("dvt_fs_read_busy2.test")
        self.check_get_file_response("dvt_fs_read_zero_datalen.test")

    def test_fs09_read_partial(self):
        """ Tests partial read in file read callback """
        self.check_get_file_response("dvt_fs_read_part.test")
        self.check_get_file_response("dvt_fs_read_part2.test")

    def test_fs10_read_errors(self):
        """ Tests canceled sessions in file read callback """
        self.check_get_file_canceled("dvt_fs_read_unrecognized.test")
        self.check_get_file_canceled("dvt_fs_read_io_error2.test")

    def test_fs11_read_errors(self):
        """ Tests IO error return in file read callback """
        self.check_get_file_response("dvt_fs_read_io_error.test", expectError)
        self.check_get_file_response("dvt_fs_read_io_error_no_errno.test", expectError)

    def test_fs12_write_abort(self):
        """ Tests abort conditions in file write callback """
        self.check_put_file_abort("dvt_fs_write_abort.test")
        self.check_put_file_abort("dvt_fs_write_retinvalid.test")
        self.check_put_file_abort("dvt_fs_write_invalid_datalen.test")

    def test_fs13_write_busy(self):
        """ Tests connector_callback_busy, 0 data bytes return in file write callback """
        self.check_put_file_response("dvt_fs_write_busy.test")
        self.check_put_file_response("dvt_fs_write_busy2.test")
        self.check_put_file_response("dvt_fs_write_zero_datalen.test")
        self.check_put_file_response("dvt_fs_write_zero_datalen2.test")

    def test_fs14_write_partial(self):
        """ Tests partial write in file write callback """
        self.check_put_file_response("dvt_fs_write_part.test")

    def test_fs15_write_canceled(self):
        """ Tests canceled sessions in file write callback """
        self.check_put_file_canceled("dvt_fs_write_unrecognized.test")

    def test_fs16_write_errors(self):
        """ Tests IO error return in file write callback """
        self.check_put_file_response("dvt_fs_write_io_error.test", expectError)
        self.check_put_file_response("dvt_fs_write_io_error_no_errno.test", expectError)
        self.check_put_file_response("dvt_fs_write_io_error2.test", expectError)

    def test_fs17_lseek_put_abort(self):
        """ Tests abort conditions in file put lseek callback """
        self.check_put_file_abort("dvt_fs_lseek_write_abort.test", 1)
        self.check_put_file_response("dvt_fs_lseek_write_abort2.test", False, 0, noCleanup)
        self.check_put_file_abort("dvt_fs_lseek_write_abort2.test", 1)
        self.check_put_file_abort("dvt_fs_lseek_write_retinvalid.test", 1)

    def test_fs18_lseek_put_busy(self):
        """ Tests connector_callback_busy return in file lseek callback """
        self.check_put_file_response("dvt_fs_lseek_write_busy.test", False, 0, noCleanup)
        self.check_put_file_response("dvt_fs_lseek_write_busy.test", False, 1)
        self.check_put_file_response("dvt_fs_lseek_write_busy2.test", False, 0, noCleanup)
        self.check_put_file_response("dvt_fs_lseek_write_busy2.test", False, 1)

    def test_fs19_lseek_put_canceled(self):
        """ Tests canceled sessions in put lseek callback """
        self.check_put_file_canceled("dvt_fs_lseek_write_unrecognized.test", 1)

    def test_fs20_lseek_put_errors(self):
        """ Tests errors in put lseek callback """
        self.check_put_file_response("dvt_fs_lseek_write_io_error.test", expectError, 1)
        self.check_put_file_response("dvt_fs_lseek_write_io_error2.test", False, 0, noCleanup)
        self.check_put_file_response("dvt_fs_lseek_write_io_error2.test", expectError, 1)
        self.check_put_file_response("dvt_fs_lseek_write_io_error_no_errno.test", expectError, 1)
        self.check_put_file_response("dvt_fs_lseek_write_neg_offset.test", False, 0, noCleanup)
        self.check_put_file_response("dvt_fs_lseek_write_neg_offset.test", expectError, 1)
        self.check_put_file_response("dvt_fs_lseek_write_neg_offset2.test", False, 0, noCleanup)
        self.check_put_file_response("dvt_fs_lseek_write_neg_offset2.test", expectError, 1)
        self.check_put_file_response("dvt_fs_lseek_write_bad_offset.test", False, 0, noCleanup)
        self.check_put_file_response("dvt_fs_lseek_write_bad_offset.test", expectError, 50000)

    def test_fs21_lseek_put_ok (self):
        """ Tests file lseek in write callback """
        self.check_put_file_response("dvt_fs_lseek_write_ok.test", False, 0, noCleanup)
        self.check_put_file_response("dvt_fs_lseek_write_ok.test", False, 1)

#-------
    def test_fs22_lseek_get_abort(self):
        """ Tests abort conditions in file get lseek callback """
        self.check_get_file_abort("dvt_fs_lseek_read_abort.test", 1)
        self.check_get_file_abort("dvt_fs_lseek_read_abort2.test", 1)
        self.check_get_file_abort("dvt_fs_lseek_read_retinvalid.test", 1)
  
    def test_fs23_lseek_get_busy(self):
        """ Tests connector_callback_busy return in file get lseek callback """
        self.check_get_file_response("dvt_fs_lseek_read_busy.test", False, 1)
        self.check_get_file_response("dvt_fs_lseek_read_busy2.test", False, 1)

    def test_fs24_lseek_get_canceled(self):
        """ Tests canceled sessions in get lseek callback """
        self.check_get_file_canceled("dvt_fs_lseek_read_unrecognized.test", 1)

    def test_fs25_lseek_get_errors(self):
        """ Tests errors in get lseek callback """
        self.check_get_file_response("dvt_fs_lseek_read_io_error.test", expectError, 1)
        self.check_get_file_response("dvt_fs_lseek_read_io_error_no_errno.test", expectError, 1)
        self.check_get_file_response("dvt_fs_lseek_read_neg_offset.test", expectError, 1)
        self.check_get_file_response("dvt_fs_lseek_read_bad_offset.test", expectError, 50000)

    def test_fs26_lseek_get_ok (self):
        """ Tests file get lseek callback """
        self.check_get_file_response("dvt_fs_lseek_read_ok.test", False, 1)

    def test_fs27_close_put_abort(self):
        """ Tests abort conditions in file put close callback """
        self.check_put_file_abort("dvt_fs_close_write_abort.test")
        self.check_put_file_abort("dvt_fs_close_write_retinvalid.test")

    def test_fs28_close_put_busy(self):
        """ Tests connector_callback_busy return in file put close callback """
        self.check_put_file_response("dvt_fs_close_write_busy.test")

    def test_fs29_close_put_canceled(self):
        """ Tests canceled cession in file put close callback """
        self.check_put_file_canceled("dvt_fs_close_write_unrecognized.test")

    def test_fs29_close_put_errors(self):
        """ Tests errors in file put close callback """
        self.check_put_file_response("dvt_fs_close_write_io_error.test", expectError)
        self.check_put_file_response("dvt_fs_close_write_io_error_no_errno.test", expectError)
        
 
#---
    def test_fs30_close_get_abort(self):
        """ Tests abort conditions in file get close callback """
        self.check_get_file_abort("dvt_fs_close_read_abort.test")
        self.check_get_file_abort("dvt_fs_close_read_retinvalid.test")

    def test_fs31_close_get_busy(self):
        """ Tests connector_callback_busy return in file get close callback """
        self.check_get_file_response("dvt_fs_close_read_busy.test")
        self.check_get_small_file_response("dvt_fs_close_read_busy_twice.test")
    
    def test_fs32_close_get_canceled(self):
        """ Tests canceled sessions in file get close callback """
        self.check_get_file_canceled("dvt_fs_close_read_unrecognized.test")
        self.check_get_file_canceled("dvt_fs_close_read_io_error.test")
        self.check_get_file_canceled("dvt_fs_close_read_io_error_no_errno.test")

    def test_fs33_close_get_error(self):
        """ Tests errors returned in file get close callback """
        self.check_get_small_file_response("dvt_fs_close_read_err_small.test", expectError)

    def test_fs34_strerr_put_abort(self):
        """ Tests abort conditions in file put strerr callback """
        self.verify_put_file_abort("dvt_fs_strerr_put_abort/file", self.outData)
        self.verify_put_file_abort("dvt_fs_strerr_put_retinvalid/file", self.outData)
        self.verify_put_file_abort("dvt_fs_strerr_put_bad_datalen/file", self.outData)
        self.log.info("Test success!")

    def test_fs35_strerr_get(self):
        """ Tests ignored stuff file strerr put callback """
        self.verify_put_file("dvt_fs_strerr_put_zero_datalen/file", self.outData, expectError)
        self.log.info("Test success!")

#--------- 

    def check_get_file_abort(self, fpath, offset = 0):
        self.verify_put_file(fpath, self.outData)
        self.verify_get_file_abort(fpath, self.out_size, offset)
        self.verify_remove_file(fpath)
        self.log.info("Test success!")

    def check_get_file_canceled(self, fpath, offset = 0):
        self.verify_put_file(fpath, self.outData)
        self.verify_get_file_user_canceled(fpath, self.out_size, offset)
        self.verify_remove_file(fpath)
        self.log.info("Test success!")
   
    def check_get_file_response(self, fpath, errorExpected = False, offset = 0):
        self.verify_put_file(fpath, self.outData)
        self.verify_get_file(fpath, self.out_size, errorExpected, offset)
        self.verify_remove_file(fpath)
        self.log.info("Test success!")

    def check_get_small_file_response(self, fpath, errorExpected = False, offset = 0):
        data = "Small file data"
        out_size = len(data)
        out_data = base64.encodestring(data)
        self.verify_put_file(fpath, out_data)
        self.verify_get_file(fpath, out_size, errorExpected, offset)
        self.verify_remove_file(fpath)
        self.log.info("Test success!")

    def check_put_file_abort(self, fpath, offset = 0):
        self.verify_put_file_abort(fpath, self.outData, offset)
        self.verify_remove_file(fpath)
        self.log.info("Test success!")

    def check_put_file_canceled(self, fpath, offset = 0):
        self.verify_put_file_user_canceled(fpath, self.outData, offset)
        self.verify_remove_file(fpath)
        self.log.info("Test success!")

    def check_put_file_response(self, fpath, errorExpected = False, offset = 0, cleanup = True):
        self.verify_put_file(fpath, self.outData, errorExpected, offset)
        if cleanup == True:
            self.verify_remove_file(fpath)
        self.log.info("Test success!")

    def verify_put_file(self, fpath, fdata, errorExpected = False, offset = 0, truncate = False):
        putRequest = filePutRequest % (self.device_id, fpath, offset, truncate, fdata)

        self.log.info("put request file:\"%s\", offset:%d" % (fpath, offset))
        self.verify_device_response(
            self.session.post('http://%s/ws/sci' % self.hostname, data=putRequest).content, 
            errorExpected)

    def verify_put_file_abort(self, fpath, fdata, offset = 0, truncate = False):
        putRequest = filePutRequest % (self.device_id, fpath, offset, truncate, fdata)

        self.log.info("put request file:\"%s\", offset:%d" % (fpath, offset))
        self.verify_file_request_abort(putRequest)

 
    def verify_put_file_user_canceled(self, fpath, fdata, offset = 0,  truncate = False):
        putRequest = filePutRequest % (self.device_id, fpath, offset, truncate, fdata)

        self.log.info("put request file:\"%s\", offset:%d" % (fpath, offset))
        self.verify_file_request_canceled(putRequest)

    def verify_get_file(self, fpath, fsize, errorExpected = False, offset = 0):
        getRequest = fileGetRequest % (self.device_id, fpath, offset, fsize)

        self.log.info("get request file:\"%s\", offset:%d, size:%d" % (fpath, offset, fsize))
        self.verify_device_response(
            self.session.post('http://%s/ws/sci' % self.hostname, data=getRequest).content, 
            errorExpected)

    def verify_get_file_abort(self, fpath, fsize, offset = 0):

        getRequest = fileGetRequest % (self.device_id, fpath, offset, fsize)
        self.log.info("get request file:\"%s\", offset:%d, size:%d" % (fpath, offset, fsize))
        self.verify_file_request_abort(getRequest)

    def verify_get_file_user_canceled(self, fpath, fsize, offset = 0):
        getRequest = fileGetRequest % (self.device_id, fpath, offset, fsize)

        self.log.info("get request file:\"%s\", offset:%d, size:%d" % (fpath, offset, fsize))
        self.verify_file_request_canceled(getRequest)


    def verify_file_request_canceled(self, request):
        cancelExpected = True
        self.verify_device_response_text(
            self.session.post('http://%s/ws/sci' % self.hostname, data=request).content,
            "Message transmission cancelled", cancelExpected, "File system session canceled, not expected", "File system session cancel expected")  

    def verify_file_request_abort(self, request):
        abortExpected = True

        self.verify_device_response_text(
            self.session.post('http://%s/ws/sci' % self.hostname, data=request).content, 
            'error id="2006"', abortExpected, "Device aborted, not expected", "Abort expected") 

        response = ""
        sync_failed = True
        no_file_request = noFileRequest % (self.device_id)
        for x in xrange(1, 20):
            response = self.session.post('http://%s/ws/sci' % self.hostname, data=no_file_request).content
            if (response.find('error id="2001"') == -1):
                sync_failed = False
                break
            time.sleep(1)

        self.log.info("Sync after abort %d times" % x)
        if (response.find('error id="1"') == -1):
            self.log.info("Response to sync after abort %s" % response)

        self.assertTrue(sync_failed == False, "**********  DEVICE STILL NOT CONNECTED after 20 sec  **********, response[%s]" %(response))


    def verify_device_response(self, response, errorExpected):
        # validate response?

        if (response.find('<error') == -1):
            errorActual = False
            errorString = "Expected error, but got success"
        else:
            errorActual = True
            errorString = "Expected success, but got error"

        self.assertTrue(errorExpected == errorActual, "%s, response[%s]" %(errorString, response))

    def verify_device_response_text(self, response, text, actionExpected, text_yes, text_no):
        # validate response?
        
        if (response.find(text) == -1):
            actionActual = False
            errorString = text_no
        else:
            actionActual = True
            errorString = text_yes
        
        self.assertTrue(actionExpected == actionActual, "%s, response[%s]" %(errorString, response))
        

    def verify_remove_file(self, path, errorExpected = False):
        rmRequest = fileRmRequest % (self.device_id, path)

        self.log.info("Sending rm request for \"%s\" " % path)
        self.verify_device_response(
            self.session.post('http://%s/ws/sci' % self.hostname, data=rmRequest).content, 
            errorExpected)

if __name__ == '__main__':
    unittest.main() 
