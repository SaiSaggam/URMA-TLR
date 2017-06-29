# -*- coding: iso-8859-1 -*-
import cc_testcase
import random
import time
import os


MAX_TEST_FILE_SIZE = 2097152
fileContent = ""

for i in xrange(MAX_TEST_FILE_SIZE):
    fileContent += chr(random.randint(0, 255))


class FileSystemDvtTestCase(cc_testcase.TestCase):


    def test_01_send_file_invalid_path(self):
        """ Sends put_file request to upload a file in a non-existent path.
        Verifies if the returned response is the expected and the file was not generated.
        """

        filePath = os.path.join(self.tempPath, "invalidPath", "test_01_send_file_invalid_path.txt")
        fileData = ""

        # Send the file to the Device
        status, response = self.cloudHandler.uploadFileToDevice(self.device_id, filePath, fileData)

        if(status):
            # Status 200. Checking the received response
            self.fail("Received response from device: \"%s\" is not the expected (code %s)" % (response.content, response.status_code) )
        else:
            # Get response text
            responseText = response.resource["sci_reply"]["file_system"]["device"]["commands"]["put_file"]["error"]["#text"]

            if ( responseText != "No such file or directory" ):
                self.fail("Incorrect response from device: %s" % response.content)
            else:
                self.log.info("Received expected response '%s' from device" % responseText)



    def test_02_get_file_invalid_path(self):
        """ Sends get_file request to download a file from a non-existent path.
        Verifies if the returned response is the expected.
        """

        filePath = os.path.join(self.tempPath, "invalidPath", "test_02_get_file_invalid_path.txt")


        # Get the file from device
        status, obtainedFileData, response = self.cloudHandler.downloadFileFromDevice(self.device_id, filePath)

        if(status):
            # Status 200. Checking the received response
            self.fail("Received response from device: \"%s\" is not the expected (code %s)" % (response.content, response.status_code) )
        else:
            # Get response text
            responseText = response.resource["sci_reply"]["file_system"]["device"]["commands"]["get_file"]["error"]["#text"]

            if ( responseText != "No such file or directory" ):
                self.fail("Incorrect response from device: %s" % response.content)
            else:
                self.log.info("Received expected response '%s' from device" % responseText)




    def test_03_remove_file_invalid_path(self):
        """ Sends remove file request from a non-existent path.
        Verifies if the returned response is the expected.
        """

        filePath = os.path.join(self.tempPath, "invalidPath", "test_03_remove_file_invalid_path.txt")

        # Remove file from device
        status, response = self.cloudHandler.removeFileFromDevice(self.device_id, filePath)
        if(not status):
            # Status 200. Checking the received response
            self.fail("Received response from device: \"%s\" is not the expected (code %s)" % (response.content, response.status_code) )
        else:
            # Get response text
            responseText = response.resource["sci_reply"]["file_system"]["device"]["commands"]["rm"]["error"]["#text"]

            if ( responseText != "No such file or directory" ):
                self.fail("Incorrect response from device: %s" % response.content)
            else:
                self.log.info("Received expected response '%s' from device" % responseText)



    def test_04_remove_invalid_file(self):
        """ Sends remove file request for a non-existent file.
        Verifies if the returned response is the expected.
        """

        filePath = os.path.join(self.tempPath, "test_04_remove_invalid_file.txt")

        # Remove file from device
        status, response = self.cloudHandler.removeFileFromDevice(self.device_id, filePath)
        if(not status):
            # Status 200. Checking the received response
            self.fail("Received response from device: \"%s\" is not the expected (code %s)" % (response.content, response.status_code) )
        else:
            # Get response text
            responseText = response.resource["sci_reply"]["file_system"]["device"]["commands"]["rm"]["error"]["#text"]

            if ( responseText != "No such file or directory" ):
                self.fail("Incorrect response from device: %s" % response.content)
            else:
                self.log.info("Received expected response '%s' from device" % responseText)





    def test_05_list_file_invalid_path(self):
        """ Sends list folder request over file system for a non-existent path.
        Verifies if the returned response is the expected.
        """

        folderPath = os.path.join(self.tempPath, "invalidPath")

        # Remove file from device
        status, response = self.cloudHandler.listFilesytemFromDevice(self.device_id, folderPath)
        if(not status):
            # Status 200. Checking the received response
            self.fail("Received response from device: \"%s\" is not the expected (code %s)" % (response.content, response.status_code) )
        else:
            # Get response text
            responseText = response.resource["sci_reply"]["file_system"]["device"]["commands"]["ls"]["error"]["#text"]

            if ( responseText != "No such file or directory" ):
                self.fail("Incorrect response from device: %s" % response.content)
            else:
                self.log.info("Received expected response '%s' from device" % responseText)



    def test_06_send_invalid_file_with_offset(self):
        """ Sends put_file request to upload data for a non-existent file with a valid offset.
        Verifies if the returned response is the expected and the file was generated empty.
        """

        filePath = os.path.join(self.tempPath, "test_06_send_invalid_file_with_offset.txt")
        fileData = fileContent[:2048]

        # Send the file to the Device
        status, response = self.cloudHandler.uploadFileToDevice(self.device_id, filePath, fileData, offset=100)

        if(status):
            # Status 200. Checking the received response
            self.fail("Received response from device: \"%s\" is not the expected (code %s)" % (response.content, response.status_code) )
        else:
            # Get response text
            responseText = response.resource["sci_reply"]["file_system"]["device"]["commands"]["put_file"]["error"]["#text"]

            if ( responseText != "Invalid offset" ):
                self.fail("Incorrect response from device: %s" % response.content)
            else:
                self.log.info("Received expected response '%s' from device" % responseText)


        # Remove file from device, because the file without any data was created
        status, response = self.cloudHandler.removeFileFromDevice(self.device_id, filePath)
        if(status):
            self.log.info("Received the expected response for the remove request")
        else:
            self.log.error("Response content from device: %s" % response.content)
            self.fail("Incorrect response code: %d" % response.status_code)




    def test_07_send_file_with_invalid_offset(self):
        """ Sends put_file request to upload data for an existent file with an invalid offset.
        Verifies if the returned response is the expected and the file was not modified.
        """

        filePath = os.path.join(self.tempPath, "test_07_send_file_with_invalid_offset.txt")
        fileDataOriginal = fileContent[:2048]

        #** Send the original file
        status, response = self.cloudHandler.uploadFileToDevice(self.device_id, filePath, fileDataOriginal )

        if(status):
            # Status 200. Checking the received response
            responseText = response.resource["sci_reply"]["file_system"]["device"]["commands"]["put_file"]
            if(responseText is None):
                self.log.info("Received the expected response for the put_file request")
            else:
                self.fail("Received response from device: \"%s\" is not the expected" % responseText)
        else:
            self.log.error("Response content from device: %s" % response.content)
            self.fail("Incorrect response code: %d" % response.status_code)



        #** Send data chunk with an invalid offset
        fileData = fileContent[:500]
        fileData = fileData[::-1] # reverse the string
        status, response = self.cloudHandler.uploadFileToDevice(self.device_id, filePath, fileData, offset=5000)

        if(status):
            # Status 200. Checking the received response
            self.fail("Received response from device: \"%s\" is not the expected (code %s)" % (response.content, response.status_code) )
        else:
            # Get response text
            responseText = response.resource["sci_reply"]["file_system"]["device"]["commands"]["put_file"]["error"]["#text"]

            if ( responseText != "Invalid offset" ):
                self.fail("Incorrect response from device: %s" % response.content)
            else:
                self.log.info("Received expected response '%s' from device" % responseText)



        #** Get the file from device
        status, obtainedFileData, response = self.cloudHandler.downloadFileFromDevice(self.device_id, filePath)

        if(status):
            self.log.info("Received the expected response for the get_file request")
        else:
            self.log.error("Response content from device: %s" % response.content)
            self.fail("Incorrect response code: %d" % response.status_code)



        #** Verify file content
        if ( fileDataOriginal == obtainedFileData ):
            self.log.info("File content received was successfully!")
        else:
            self.fail("Received file content from device: \"%s\"\n is not the expected: \"%s\"" % (obtainedFileData, fileDataOriginal) )



        # Remove file from device, because the file without any data was created
        status, response = self.cloudHandler.removeFileFromDevice(self.device_id, filePath)
        if(status):
            self.log.info("Received the expected response for the remove request")
        else:
            self.log.error("Response content from device: %s" % response.content)
            self.fail("Incorrect response code: %d" % response.status_code)



    def test_08_get_file_with_invalid_offset(self):
        """ Sends get_file request to download data for an existent file with an invalid offset.
        Verifies if the returned response is the expected.
        """

        filePath = os.path.join(self.tempPath, "test_08_get_file_with_invalid_offset.txt")
        fileDataOriginal = fileContent[:2048]

        #** Send the original file
        status, response = self.cloudHandler.uploadFileToDevice(self.device_id, filePath, fileDataOriginal )

        if(status):
            # Status 200. Checking the received response
            responseText = response.resource["sci_reply"]["file_system"]["device"]["commands"]["put_file"]
            if(responseText is None):
                self.log.info("Received the expected response for the put_file request")
            else:
                self.fail("Received response from device: \"%s\" is not the expected" % responseText)
        else:
            self.log.error("Response content from device: %s" % response.content)
            self.fail("Incorrect response code: %d" % response.status_code)



        #** Get the file from device
        status, obtainedFileData, response = self.cloudHandler.downloadFileFromDevice(self.device_id, filePath, offset = 2049)

        if(status):
            # Status 200. Checking the received response
            self.fail("Received response from device: \"%s\" is not the expected (code %s)" % (response.content, response.status_code) )
        else:
            # Get response text
            responseText = response.resource["sci_reply"]["file_system"]["device"]["commands"]["get_file"]["error"]["#text"]

            if ( responseText != "Invalid offset" ):
                self.fail("Incorrect response from device: %s" % response.content)
            else:
                self.log.info("Received expected response '%s' from device" % responseText)



        # Remove file from device, because the file without any data was created
        status, response = self.cloudHandler.removeFileFromDevice(self.device_id, filePath)
        if(status):
            self.log.info("Received the expected response for the remove request")
        else:
            self.log.error("Response content from device: %s" % response.content)
            self.fail("Incorrect response code: %d" % response.status_code)
