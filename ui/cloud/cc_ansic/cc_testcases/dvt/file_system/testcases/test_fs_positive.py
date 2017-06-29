import time
import cc_testcase
import datetime
import re
import os
import base64
import xml.dom.minidom
import hashlib

from random import randint

MAX_TEST_FILE_SIZE = 23453
FileData = ""
FileList =  [
    ("dvt_fs_0.txt", 0),
    ("dvt_fs_1.txt", 1),
    ("dvt_fs_2.txt", 500),
    ("dvt_fs_4.txt", 1600),
    ("dvt_fs_5.bin", MAX_TEST_FILE_SIZE),
    ]

class FileSystemTestCase(cc_testcase.TestCase):

    def test_fs1_basic_cases(self):
        """ Tests put and get on different sized files. This creates a series of files with random content that are used by other tests. """
        global FileData
        global FileList
        global MAX_TEST_FILE_SIZE
        
        for i in xrange(MAX_TEST_FILE_SIZE):
            FileData += chr(randint(0, 255))

        for fname, fsize in FileList:
            putData = FileData[:fsize]
            outData = base64.encodestring(putData)
            self.put_a_file(fname, outData, fsize)

            inData = self.get_a_file(fname, fsize)
            if (inData is not None):
                getData = base64.decodestring(inData)
            else:
                getData = ""
            self.assertEqual(putData, getData, "Mismatch in the sent and received file [%s, put:%s, get:%s]" %(fname, putData, getData))

    def test_fs2_offset(self):
        """ Tests put and get with different offset and size on a file """
        fname = "dvt_fs_5.bin"

        offsetList =  [
            (100, 0),
            (511, 1),
            (8192, 512),
            (12345, 1580),
            (20000, MAX_TEST_FILE_SIZE-20000),
            ]

        for offset, fsize in offsetList:
            putData = FileData[:fsize]
            outData = base64.encodestring(putData)
            self.put_a_file(fname, outData, fsize, offset)

            inData = self.get_a_file(fname, fsize, offset)
            if (inData is not None):
                getData = base64.decodestring(inData)
            else:
                getData = ""

            self.assertEqual(putData, getData, "Mismatch in the sent and received file [%s, put:%s, get:%s]" %(fname, putData, getData))

    def test_fs3_truncate(self):
        """ Tests put with different offset, size and truncate ON on list of files """
        global FileList

        truncateList =  [
            (0, 4),
            (0, 1),
            (500, 83),
            (1111, 580),
            (4321, 16037),
            ]

        for i in xrange(len(FileList)):
            offset, fsize = truncateList[i]
            fname, total = FileList[i]

            putData = FileData[:fsize]
            outData = base64.encodestring(putData)
            self.put_a_file(fname, outData, fsize, offset, True)

            FileList[i] = (fname, offset + fsize)

    def test_fs4_ls(self):
        """ Tests file system ls command. Checks for known-directories and for the files created in first place. Tests that sizes match and the hashes are correct. Finally, removes files created in test_fs1_basic_cases """
        HashAlgoList = ['none', 'any', 'crc32', 'md5']

        my_path = './'
        for hashAlgo in HashAlgoList:
            fList, dList = self.get_file_list(my_path, hashAlgo)
            for fname, fsize in FileList:
                fileFound = False
                fname = my_path + fname
                for eachFile in fList:
                    if eachFile["@path"] == fname:
                        fileFound = True
                        if hashAlgo == 'md5' or hashAlgo == 'any':
                            hash_value = self.md5Checksum(fname)
                            hash_value = hash_value.upper();
                            eachFile["@hash"] = eachFile["@hash"].upper()
                            self.assertEqual(eachFile["@hash"], hash_value , "Mismatch file hash [%s, expected:%s, actual:%s]" %(fname, hash_value, eachFile["@hash"]))
                        self.assertEqual(eachFile["@size"], str(fsize), "Mismatch file size [%s, expected:%d, actual:%s]" %(fname, fsize, eachFile["@size"]))
                self.assertEqual(fileFound, True, "File %s not in the file list" %fname)

            # Check for the directories in the test-harness' directory (execution path)
            python_dir = my_path + "python"
            testharness_dir = my_path + "testharness"
            found_python = False
            found_testharness = False

            for eachDir in dList:
                if eachDir["@path"] == python_dir:
                    found_python = True
                if eachDir["@path"] == testharness_dir:
                    found_testharness = True
            
            self.assertTrue(found_python, "Expected to find directory '%s'" % python_dir)
            self.assertTrue(found_testharness, "Expected to find directory '%s'." % testharness_dir)

        for fname, fsize in FileList:
            self.remove_a_file(fname)

    def put_a_file(self, fpath, fdata, fsize, offset = 0, truncate = False):
        putRequest = \
        """          <sci_request version="1.0">
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
          </sci_request>""" % (self.device_id, fpath, offset, truncate, fdata)

        
        self.log.info("Put Request file:\"%s\", offset:%d, size:%d" % (fpath, offset, fsize))
        self.log.info("Sending SCI Request:\n%s\n" % putRequest)
        response = self.cloudHandler.postRequest("sci", data = putRequest)
        # Check query was successfully
        if (response.status_code != 200):
            self.fail("Unexpected response from the Cloud")
        response_buffer = response.resource["sci_reply"]["file_system"]["device"]["commands"]["put_file"]
        return response_buffer

    def get_a_file(self, fpath, fsize, offset = 0):
        fdata = ""
        getRequest = \
            """              <sci_request version="1.0">
              <file_system>
                <targets>
                  <device id="%s"/>
                </targets>
                <commands>
                    <get_file path="%s" offset = "%d" length = "%d"/>
                </commands>
              </file_system>
              </sci_request>""" % (self.device_id, fpath, offset, fsize)

        self.log.info("Get Request file:\"%s\", offset:%d, size:%d" % (fpath, offset, fsize))
        self.log.info("Sending SCI Request:\n%s\n" % getRequest)
        response = self.cloudHandler.postRequest("sci", data = getRequest)
        # Check query was successfully
        if (response.status_code != 200):
            self.fail("Unexpected response from the Cloud")

        # Get element 'get_file'
        get_file = response.resource["sci_reply"]["file_system"]["device"]["commands"]["get_file"]

        recv_data = response.resource["sci_reply"]["file_system"]["device"]["commands"]["get_file"]["data"]

        self.log.info("got file \"%s\" " % fpath)
        
        return recv_data

    def remove_a_file(self, path):
        rmRequest = \
            """            <sci_request version="1.0">
              <file_system>
                <targets>
                  <device id="%s"/>
                </targets>
                <commands>
                  <rm path="%s"/>
                </commands>
              </file_system>
            </sci_request>""" % (self.device_id, path)

        self.log.info("Sending 'rm' request for \"%s\" " % path)
        self.log.info("Sending SCI Request:\n%s\n" % rmRequest)
        response = self.cloudHandler.postRequest("sci", data = rmRequest)
        # Check query was successfully
        if (response.status_code != 200):
            self.fail("Unexpected response from the Cloud")

        self.log.info("\"%s\" was removed" % path)

    def get_file_list(self, path, hashAlgo):
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
        
        file_list = response.resource["sci_reply"]["file_system"]["device"]["commands"]["ls"]["file"]
        dir_list = response.resource["sci_reply"]["file_system"]["device"]["commands"]["ls"]["dir"]
                
        self.log.info("Files: %d Dirs: %d parsed" %(len(file_list), len(dir_list)))
        
        return file_list, dir_list


    def md5Checksum(self, filePath):
        fh = open(filePath, 'rb')
        m = hashlib.md5()
        while True:
            data = fh.read(8192)
            if not data:
                break
            m.update(data)
        return m.hexdigest()

if __name__ == '__main__':
    unittest.main() 
