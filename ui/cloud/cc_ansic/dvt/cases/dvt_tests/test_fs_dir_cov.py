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

MAX_TEST_FILE_SIZE = 30
dummyFsize = 4
expectError = True
noCleanup = False
doTruncate = True

my_test_dir = "./dvt/cases/test_files/test_dir/"
my_test_dir1 = "./dvt/cases/test_files/test_dir"
my_test_file = "./dvt/cases/test_files/test_dir/fs_test1.txt"

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

filelsRequest = \
    """<sci_request version="1.0">
      <file_system>
        <targets>
          <device id="%s"/>
        </targets>
        <commands>
          <ls path="%s" hash="%s"/>
        </commands>
      </file_system>
    </sci_request>"""

class FileSystemDirTestCase(ic_testcase.TestCase):

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

    def test_fs01_stat1_abort(self):
        """ Tests abort conditions in 1st stat callback """
        self.check_ls_abort("dvt_fs_stat1_abort", my_test_dir)
        self.check_ls_abort("dvt_fs_stat1_retinvalid", my_test_dir)
        self.log.info("Test success!")

    def test_fs02_stat1_canceled(self):
        """ Tests canceled session in 1st stat callback """
        self.check_ls_user_canceled("dvt_fs_stat1_unrecognized", my_test_dir)
        self.log.info("Test success!")

    def test_fs03_stat1_errors(self):
        """ Tests errors returned session in 1st stat callback """
        self.check_ls("dvt_fs_stat1_io_error", my_test_dir, errorExpected = True)
        self.check_ls("dvt_fs_stat1_io_error_no_errno", my_test_dir, expectError)
        self.log.info("Test success!")

    def test_fs04_stat1_busy(self):
        """ Tests connector_callback_busy return in file 1st stat callback """
        self.check_ls("dvt_fs_stat1_busy", my_test_dir)
        self.log.info("Test success!")

    def test_fs05_stat1_badhash(self):
        """ Tests flags set to 0 return in file 1st stat callback """
        self.check_ls("dvt_fs_stat1_not_regdir", my_test_file, False, "md5")
        self.check_ls("dvt_fs_stat1_not_regdir", my_test_file)
        self.check_ls("dvt_fs_stat1_badhash", my_test_dir, False, "any")
        self.log.info("Test success!")
  
    def test_fs06_opendir_abort(self):
        """ Tests connector_callback_abort return in opendir callback """
        self.check_ls_abort("dvt_fs_opendir_abort", my_test_dir)
        self.check_ls_abort("dvt_fs_opendir_retinvalid", my_test_dir)
        self.check_ls_abort("dvt_fs_opendir_null_handle", my_test_dir)
        self.log.info("Test success!")

    def test_fs07_opendir_canceled(self):
        """ Tests canceled session in opendir callback """
        self.check_ls_user_canceled("dvt_fs_opendir_unrecognized", my_test_dir)
        self.log.info("Test success!")

    def test_fs08_opendir_errors(self):
        """ Tests errors returned session in opendir callback """
        self.check_ls("dvt_fs_opendir_io_error", my_test_dir, expectError)
        self.check_ls("dvt_fs_opendir_io_error_no_errno", my_test_dir, expectError)
        self.log.info("Test success!")

    def test_fs09_opendir_busy(self):
        """ Tests connector_callback_busy return in opendir callback """
        self.check_ls("dvt_fs_opendir_busy", my_test_dir)
        self.check_ls("dvt_fs_opendir_busy", my_test_dir1)
        self.log.info("Test success!")


    def test_fs10_readdir_abort(self):
        """ Tests connector_callback_abort return in readdir callback """
        self.check_ls_abort("dvt_fs_readdir_abort", my_test_dir)
        self.check_ls_abort("dvt_fs_readdir_abort2", my_test_dir)
        self.check_ls_abort("dvt_fs_readdir_retinvalid", my_test_dir)
        self.check_ls_abort("dvt_fs_readdir_bad_datalength", my_test_dir)
        self.log.info("Test success!")

    def test_fs11_readdir_canceled(self):
        """ Tests canceled session in readdir callback """
        self.check_ls_user_canceled("dvt_fs_readdir_unrecognized", my_test_dir)
        self.log.info("Test success!")

    def test_fs12_readdir_errors(self):
        """ Tests errors returned session in readdir callback """
        self.check_ls("dvt_fs_readdir_io_error", my_test_dir, expectError)
        self.check_ls("dvt_fs_readdir_io_error_no_errno", my_test_dir, expectError)
        self.log.info("Test success!")

    def test_fs13_readdir_busy(self):
        """ Tests connector_callback_busy return in readdir callback """
        self.check_ls("dvt_fs_readdir_busy", my_test_dir)
        self.check_ls("dvt_fs_readdir_busy2", my_test_dir)
        self.log.info("Test success!")

    def test_fs13_readdir_zerodata(self):
        """ Tests empty data return in readdir callback """
        self.check_ls("dvt_fs_readdir_zero_datalength", my_test_dir)
        self.log.info("Test success!")

    def test_fs14_readdir(self):
        """ Tests positive ls cases for and single files """
        self.verify_ls(my_test_dir, False, "any")
        self.verify_ls(my_test_dir, False, "crc32")
        self.verify_ls(my_test_dir, False, "md5")
        self.check_put_file_response("./dvt_fs_any.test", False, 0, noCleanup)
        self.check_put_file_response("./dvt_fs_crc32.test", False, 0, noCleanup)
        self.check_put_file_response("./dvt_fs_md5.test", False, 0, noCleanup)
        self.check_put_file_response("./dvt_fs_none.test", False, 0, noCleanup)
        self.verify_ls("./dvt_fs_none.test")
        self.verify_ls("./dvt_fs_any.test", False, "any")
        self.verify_ls("./dvt_fs_crc32.test", False, "crc32")
        self.verify_ls("./dvt_fs_md5.test", False, "md5")
        self.verify_remove_file("./dvt_fs_none.test")
        self.verify_remove_file("./dvt_fs_crc32.test")
        self.verify_remove_file("./dvt_fs_md5.test")
        self.verify_remove_file("./dvt_fs_any.test")
        self.log.info("Test success!")

    def test_fs20_stat2_abort(self):
        """ Tests abort conditions in stat callback inside directory """
        self.check_ls_abort("dvt_fs_stat3_abort", my_test_dir)
        self.check_ls_abort("dvt_fs_stat3_retinvalid", my_test_dir)
        self.log.info("Test success!")

    def test_fs21_stat2_canceled(self):
        """ Tests canceled session in stat callback inside directory """
        self.check_ls_user_canceled("dvt_fs_stat3_unrecognized", my_test_dir)
        self.check_ls_user_canceled("dvt_fs_stat3_io_error", my_test_dir)
        self.check_ls_user_canceled("dvt_fs_stat3_io_error_no_errno", my_test_dir)
        self.log.info("Test success!")

    def test_fs22_stat2_errors(self):
        """ Tests errors returned session in 1st stat callback """
        self.check_ls("dvt_fs_stat2_io_error", my_test_dir, expectError)
        self.check_ls("dvt_fs_stat2_io_error_no_errno", my_test_dir, expectError)
        self.log.info("Test success!")


    def test_fs23_stat2_busy(self):
        """ Tests connector_callback_busy return in stat callback inside directory """
        self.check_ls("dvt_fs_stat3_busy", my_test_dir)
        self.log.info("Test success!")

    def test_fs30_hash_abort(self):
        """ Tests connector_callback_abort return in file hash callback """
        self.check_ls_abort("dvt_fs_hash_abort", my_test_dir, "md5")
        self.check_ls_abort("dvt_fs_hash_retinvalid", my_test_dir, "md5")
        self.log.info("Test success!")

    def test_fs31_hash_canceled(self):
        """ Tests canceled session in file hash callback """
        self.check_ls_user_canceled("dvt_fs_hash_unrecognized", my_test_dir, "md5")
        self.check_ls_user_canceled("dvt_fs_hash_io_error", my_test_dir, "md5")
        self.check_ls_user_canceled("dvt_fs_hash_io_error_no_errno", my_test_dir, "md5")
        self.log.info("Test success!")

    def test_fs32_hash_busy(self):
        """ Tests connector_callback_busy return in file hash callback """
        self.check_ls("dvt_fs_hash_busy", my_test_dir, False,  "md5")
        self.log.info("Test success!")


    def test_fs40_hash_abort(self):
        """ Tests connector_callback_abort return in file hash callback for ls for a single file """
        self.check_put_file_response("./dvt_fs_file1.test", False, 0, noCleanup)
        self.check_ls_abort("dvt_fs_hash_file_abort", "./dvt_fs_file1.test", "md5")
        self.check_ls_abort("dvt_fs_hash_file_retinvalid", "./dvt_fs_file1.test", "md5")
        self.verify_remove_file("./dvt_fs_file1.test")
        self.log.info("Test success!")

    def test_fs41_hash_canceled(self):
        """ Tests canceled session in file hash callback for ls for a single file """
        self.check_put_file_response("./dvt_fs_file2.test", False, 0, noCleanup)
        self.check_ls_user_canceled("dvt_fs_hash_file_unrecognized", "./dvt_fs_file2.test", "md5")
        self.verify_remove_file("./dvt_fs_file2.test")
        self.log.info("Test success!")

    def test_fs42_hash_error(self):
        """ Tests errors returned session in file hash callback for ls for a single file """
        self.check_put_file_response("./dvt_fs_file3.test", False, 0, noCleanup)
        self.check_ls("dvt_fs_hash_file_io_error", "./dvt_fs_file3.test", expectError, "md5")
        self.check_ls("dvt_fs_hash_file_io_error_no_errno", "./dvt_fs_file3.test", expectError, "md5")
        self.verify_remove_file("./dvt_fs_file3.test")
        self.log.info("Test success!")

    def test_fs43_hash_busy(self):
        """ Tests connector_callback_busy return in file hash callback for ls for a single file """
        self.check_put_file_response("./dvt_fs_file4.test", False, 0, noCleanup)
        self.check_ls("dvt_fs_hash_file_busy", "./dvt_fs_file4.test", False,  "md5")
        self.verify_remove_file("./dvt_fs_file4.test")
        self.log.info("Test success!")

    def test_fs44_strerr_get(self):
        """ Tests ignored stuff file strerr callback """
        self.check_ls("dvt_fs_strerr_zero_datalen", "./dvt_fs_no_such_file", expectError)
        self.log.info("Test success!")

    def test_fs45_strerr_abort(self):
        """ Tests connector_callback_abort return in file strerr callback """
        self.check_ls_abort("dvt_fs_strerr_abort", "./dvt_fs_no_such_file")
        self.check_ls_abort("dvt_fs_strerr_retinvalid", "./dvt_fs_no_such_file")
        self.check_ls_abort("dvt_fs_strerr_bad_datalen", "./dvt_fs_no_such_file")
        self.log.info("Test success!")


    def test_fs50_closedir_abort(self):
        """ Tests abort conditions in closedir callback """
        self.check_ls_abort("dvt_fs_closedir_abort", my_test_dir)
        self.check_ls_abort("dvt_fs_closedir_retinvalid", my_test_dir)
        self.log.info("Test success!")

    def test_fs51_closedir_canceled(self):
        """ Tests canceled sessions in closedir callback """
        self.check_ls_user_canceled("dvt_fs_closedir_unrecognized", my_test_dir)
        self.check_ls_user_canceled("dvt_fs_closedir_io_error", my_test_dir)
        self.check_ls_user_canceled("dvt_fs_closedir_io_error_no_errno", my_test_dir)
        self.log.info("Test success!")


    def test_fs51_closedir_busy(self):
        """ Tests connector_callback_busy return in closedir callback """
        self.check_ls("dvt_fs_closedir_busy", my_test_dir)
        self.log.info("Test success!")

    def test_fs60_rm_abort(self):
        """ Tests abort conditions in rm callback """
        self.check_rm_abort("dvt_fs_rm_abort", "./test_rm_file")
        self.check_rm_abort("dvt_fs_rm_retinvalid", "./test_rm_file")
        self.log.info("Test success!")

    def test_fs61_rm_canceled(self):
        """ Tests canceled sessions in rm callback """
        self.check_rm_user_canceled("dvt_fs_rm_unrecognized", "./test_rm_file")
        self.log.info("Test success!")

    def test_fs62_rm_busy(self):
        """ Tests connector_callback_busy return in rm callback """
        self.check_put_file_response("./dvt_rm_file.test", False, 0, noCleanup)
        self.check_rm("dvt_fs_rm_busy", "./dvt_rm_file.test")
        self.log.info("Test success!")

    def test_fs63_rm_error(self):
        """ Tests error return in rm callback """
        self.check_rm("dvt_fs_rm_io_error", "./test_rm_file", expectError)
        self.check_rm("dvt_fs_rm_io_error_no_errno", "./test_rm_file", expectError)
        self.log.info("Test success!")


    def test_fs70_truncate_abort(self):
        """ Tests abort conditions in ftruncate callback """
        self.check_truncate_abort("dvt_fs_truncate_abort", "./dvt_truncate_file.test")
        self.check_truncate_abort("dvt_fs_truncate_retinvalid", "./dvt_truncate_file.test")
        self.log.info("Test success!")

    def test_fs71_truncate_canceled(self):
        """ Tests canceled sessions in ftruncate callback """
        self.check_truncate_canceled("dvt_fs_truncate_unrecognized", "./dvt_truncate_file.test")
        self.log.info("Test success!")

    def test_fs72_truncate_busy(self):
        """ Tests connector_callback_busy return in ftruncate callback """
        self.check_truncate("dvt_fs_truncate_busy", "./dvt_truncate_file.test")
        self.log.info("Test success!")

    def test_fs73_truncate_error(self):
        """ Tests error return in ftruncate callback """
        self.check_truncate("dvt_fs_truncate_io_error", "./dvt_truncate_file.test", expectError)
        self.check_truncate("dvt_fs_truncate_io_error_no_errno", "./dvt_truncate_file.test", expectError)
        self.log.info("Test success!")

#--------- 


    def verify_truncate_abort(self, fpath):
        self.verify_put_file_abort(fpath, self.outData[10:], 2, doTruncate)
        self.verify_remove_file(fpath)
        
    def verify_truncate_canceled(self, fpath):
        self.verify_put_file_user_canceled(fpath, self.outData[10:], 2, doTruncate)
        self.verify_remove_file(fpath)

    def verify_truncate_response(self, fpath, errorExpected = False):
        self.verify_put_file(fpath, self.outData[10:], errorExpected, 2, doTruncate)
        self.verify_remove_file(fpath)

    def check_truncate_abort(self, test_name, fpath):
        self.check_put_file_response(fpath, False, 0, noCleanup)
        self.log.info("test %s, put request:\"%s\"" % (test_name, fpath))

        self.verify_no_file(test_name) 
        self.verify_truncate_abort(fpath)

    def check_truncate_canceled(self, test_name, fpath):
        self.check_put_file_response(fpath, False, 0, noCleanup)
        self.log.info("test %s, put request:\"%s\"" % (test_name, fpath))

        self.verify_no_file(test_name) 
        self.verify_truncate_canceled(fpath)

    def check_truncate(self, test_name, fpath, errorExpected = False):
        self.check_put_file_response(fpath, False, 0, noCleanup)
        self.log.info("test %s, put request:\"%s\"" % (test_name, fpath))

        self.verify_no_file(test_name) 
        self.verify_truncate_response(fpath, errorExpected)

    def check_put_file_response(self, fpath, errorExpected = False, offset = 0, cleanup = True):
        self.verify_put_file(fpath, self.outData, errorExpected, offset)
        if cleanup == True:
            self.verify_remove_file(fpath)

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

    def verify_file_request_canceled(self, request):

        self.verify_device_response_text(
            self.session.post('http://%s/ws/sci' % self.hostname, data=request).content,
            "Message transmission cancelled", "File system session cancel expected")  

    def verify_no_file(self, fpath):
        getRequest = fileGetRequest % (self.device_id, fpath, 0, dummyFsize)
        response = self.session.post('http://%s/ws/sci' % self.hostname, data=getRequest).content
        #self.log.info("verify_no_file: %s" % response)

        if (response.find('error id="1"') == -1):
            self.assertTrue(False, "Expected: No such file or directory, response[%s]" %(response))


    def verify_file_request_abort(self, request):
 
        response = self.session.post('http://%s/ws/sci' % self.hostname, data=request).content 

        self.log.info("verify_file_request_abort: %s" % response)

        self.verify_device_response_text(response, 'error id="2006"', "Abort expected") 

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

       

    def verify_remove_file(self, path, errorExpected = False):
        rmRequest = fileRmRequest % (self.device_id, path)

        self.log.info("Sending rm request for \"%s\" " % path)
        self.verify_device_response(
            self.session.post('http://%s/ws/sci' % self.hostname, data=rmRequest).content, 
            errorExpected)

    def verify_ls(self, fpath, errorExpected = False, hashAlgo = "none"):
        lsRequest = filelsRequest % (self.device_id, fpath, hashAlgo)

        self.log.info("test %s, ls request:\"%s\", and hash:%s" % (fpath, fpath, hashAlgo))

        response = self.session.post('http://%s/ws/sci' % self.hostname, data=lsRequest).content
        self.verify_device_response(response, errorExpected)
        return self.parse_ls(response)


    def check_ls(self, test_name, ls_path, errorExpected = False, hashAlgo = "none"):
        lsRequest = filelsRequest % (self.device_id, ls_path, hashAlgo)

        self.log.info("test %s, ls request:\"%s\", and hash:%s" % (test_name, ls_path, hashAlgo))

        self.verify_no_file(test_name)
        response = self.session.post('http://%s/ws/sci' % self.hostname, data=lsRequest).content
        
        self.verify_device_response(response, errorExpected)
        return self.parse_ls(response)

    def check_ls_abort(self, test_name, ls_path, hashAlgo = "none"):
        lsRequest = filelsRequest % (self.device_id, ls_path, hashAlgo)

        self.log.info("test %s, ls request:\"%s\", and hash:%s" % (test_name, ls_path, hashAlgo))

        self.verify_no_file(test_name) 
        self.verify_file_request_abort(lsRequest)

    def check_ls_user_canceled(self, test_name, ls_path, hashAlgo="none"):
        lsRequest = filelsRequest % (self.device_id, ls_path, hashAlgo)

        self.log.info("test %s, ls request:\"%s\", and hash:%s" % (test_name, ls_path, hashAlgo))

        self.verify_no_file(test_name)
        self.verify_file_request_canceled(lsRequest)

    def check_rm_abort(self, test_name, rm_path):
        rmRequest = fileRmRequest % (self.device_id, rm_path)

        self.log.info("test %s, rm request:\"%s\"" % (test_name, rm_path))

        self.verify_no_file(test_name) 
        self.verify_file_request_abort(rmRequest)

    def check_rm_user_canceled(self, test_name, rm_path):
        rmRequest = fileRmRequest % (self.device_id, rm_path)

        self.log.info("test %s, rm request:\"%s\"" % (test_name, rm_path))

        self.verify_no_file(test_name)
        self.verify_file_request_canceled(rmRequest)

    def check_rm(self, test_name, rm_path, errorExpected = False):
        rmRequest = fileRmRequest % (self.device_id, rm_path)

        self.log.info("test %s, rm request:\"%s\"" % (test_name, rm_path))

        self.verify_no_file(test_name)
        response = self.session.post('http://%s/ws/sci' % self.hostname, data=rmRequest).content

        self.verify_device_response(response, errorExpected)


    def verify_device_response(self, response, errorExpected):
        # validate response?

        if ((response.find('error id="2006"') != -1) or (response.find('error id="2001"') != -1)):
            self.assertTrue(False, "%s, response[%s]" %("Did not expect disconnect", response))
        elif (response.find('<error') == -1):
            errorActual = False
            errorString = "Expected error, but got success"
        else:
            errorActual = True
            errorString = "Expected success, but got error"

        self.assertTrue(errorExpected == errorActual, "%s, response[%s]" %(errorString, response))

    def verify_device_response_text(self, response, text, errorString):
        # validate response?
        found = True

        if (response.find(text) == -1):
            found = False

        if (errorString == "Abort expected"):
            self.log.info("Abort Response: %s" % response)
        else:
            self.log.info("Cancel Response: %s" % response)

        self.assertTrue(found, "%s, response[%s]" %(errorString, response))

    def get_obj_from_slice(self, segment):
        obj = []

        path_re = re.compile('path="([\.\w/-]+)"')
        last_re = re.compile('last_modified="([\.\w/-]+)"')
        size_re = re.compile('size="([\.\w/-]+)"')
        hash_re = re.compile('hash="([\.\w/-]+)"')

        pm = path_re.search(segment)
        obj.append((pm and pm.group(1)) or None)

        lm = last_re.search(segment)
        obj.append((lm and lm.group(1)) or None)

        sm = size_re.search(segment)
        obj.append((sm and sm.group(1)) or None)

        hm = hash_re.search(segment)
        obj.append((hm and hm.group(1)) or None)

        return obj

    def get_line_indexes(self, resp, starter):
        index = resp.find(starter)
        if index == -1:
            return None, None
        end_index = resp[index:].find('/>')
        self.assertTrue(end_index != -1, "Failed to parse out file from current response: %s" %resp)

        end_index += index

        return index, end_index

    def parse_ls(self, resp):
        file_list = []
        dir_list = []
        tmp_resp = resp
        while 1:
            index, end_index = self.get_line_indexes(tmp_resp, '<file ')
            if index is None:
                break

            segment = tmp_resp[index:end_index]
            obj = self.get_obj_from_slice(segment)
            file_list.append(obj)
            tmp_resp = tmp_resp[end_index:]

        tmp_resp = resp
        while 1:
            index, end_index = self.get_line_indexes(tmp_resp, '<dir')
            if index is None:
                break

            segment = tmp_resp[index:end_index]
            obj = self.get_obj_from_slice(segment)
            dir_list.append(obj)
            tmp_resp = tmp_resp[end_index:]

        self.log.info("Files: %d Dirs: %d parsed" %(len(file_list), len(dir_list)))
        return file_list, dir_list


if __name__ == '__main__':
    unittest.main() 
