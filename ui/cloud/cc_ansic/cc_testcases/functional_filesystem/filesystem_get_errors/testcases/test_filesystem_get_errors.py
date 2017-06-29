# -*- coding: iso-8859-1 -*-
import cc_testcase
import os



class FileSystemDvtTestCase(cc_testcase.TestCase):


    def test_01_get_file_return_busy(self):
        """ Sends get_file request to download a file, several times the Connector shows a "busy" message and then continues the execution normally.
        Verifies if the returned response is the expected.
        """

        filePath = os.path.join(self.tempPath, "test_01_get_file_return_busy.txt")
        fileData = "payload"

        #** Send the file to the Device
        status, response = self.cloudHandler.uploadFileToDevice(self.device_id, filePath, fileData)



        #** Get the file from device
        status, obtainedFileData, response = self.cloudHandler.downloadFileFromDevice(self.device_id, filePath)

        if(status):
            # Status 200. Checking the received response
            self.log.info("Received expected response from device: \"%s\" and the expected (code %s)" % (response.content, response.status_code) )
        else:
            self.fail("Received response from device: \"%s\" is not expected (code %s)" % (response.content, response.status_code) )


        #** Verify file content
        if ( fileData == obtainedFileData ):
            #self.log.warning("Received file content from device: \"%s\" is the expected: \"%s\"" % (obtainedFileData, fileData) )
            self.log.info("File content received was successfully!")
        else:
            self.fail("Received file content from device: \"%s\"\n is not the expected: \"%s\"" % (obtainedFileData, fileData) )



        #** Verify the console message for each busy state
        result, line, dataBuffer = self.deviceHandler.readUntilPattern ( pattern="Connector busy. Wait till response is sent...", timeout=30)
        if(result):
            # Verify if we found 4 busy messages
            if( len(dataBuffer.findAll("Connector busy. Wait till response is sent...")) == 4):
                self.log.info("Connector busy feedback received correctly")
            else:
                self.fail("Console feedback for busy status NOT received")
        else:
            self.fail("Console feedback for busy messages NOT found")


        #** Remove file from device
        status, response = self.cloudHandler.removeFileFromDevice(self.device_id, filePath)
        if(status):
            self.log.info("Received the expected response for the remove request")
        else:
            self.log.error("Response content from device: %s" % response.content)
            self.fail("Incorrect response code: %d" % response.status_code)



    def test_02_get_file_error_permission_denied(self):
        """ Sends get_file request to simulate a permission denied error.
        Verifies if the returned response is the expected.
        """

        self.sendGetRequestAndVerifyErrorResponse( "test_02_get_file_error_permission_denied.txt", "Permission denied")



    def test_03_get_file_error_invalid_parameter(self):
        """ Sends get_file request to simulate an invalid parameter error.
        Verifies if the returned response is the expected.
        """

        self.sendGetRequestAndVerifyErrorResponse( "test_03_get_file_error_invalid_parameter.txt", "Invalid argument")



    def test_04_get_file_error_out_of_memory(self):
        """ Sends get_file request to simulate an out of memory error.
        Verifies if the returned response is the expected.
        """

        self.sendGetRequestAndVerifyErrorResponse( "test_04_get_file_error_out_of_memory.txt", "Cannot allocate memory")



    def test_05_get_file_error_path_not_found(self):
        """ Sends get_file request to simulate a path not found error.
        Verifies if the returned response is the expected.
        """

        self.sendGetRequestAndVerifyErrorResponse( "test_05_get_file_error_path_not_found.txt", "No such file or directory")



    def test_06_get_file_error_insufficient_storage_space(self):
        """ Sends get_file request to simulate an insufficient storage space left error.
        Verifies if the returned response is the expected.
        """

        self.sendGetRequestAndVerifyErrorResponse( "test_06_get_file_error_insufficient_storage_space.txt", "No space left on device")



    def test_07_get_file_error_unspecified(self):
        """ Sends get_file request to simulate an unknown error.
        Verifies if the returned response is the expected.
        """

        self.sendGetRequestAndVerifyErrorResponse( "test_07_get_file_error_unspecified.txt", "Unspecified error")



    def test_08_get_file_error_timeout(self):
        """ Sends get_file request to simulate a timeout error.
        Verifies if the returned response is the expected.
        """

        self.sendGetRequestAndVerifyErrorResponse( "test_08_get_file_error_timeout.txt", "Connection timed out")






    def sendGetRequestAndVerifyErrorResponse(self, filename, expectedResponse):


        filePath = os.path.join(self.tempPath, filename)
        fileData = "payload"


        #** Send the file to the Device
        status, response = self.cloudHandler.uploadFileToDevice(self.device_id, filePath, fileData)



        #** Get the file from device
        status, obtainedFileData, response = self.cloudHandler.downloadFileFromDevice(self.device_id, filePath)

        if(status):
            # Status 200. Checking the received response
            self.fail("Received response from device: \"%s\" and is not the expected (code %s)" % (response.content, response.status_code) )
        else:
            # Get response text
            responseText = response.resource["sci_reply"]["file_system"]["device"]["commands"]["get_file"]["error"]["#text"]

            if ( responseText != expectedResponse ):
                self.fail("Incorrect response from device: %s" % response.content)
            else:
                self.log.info("Received expected response '%s' from device" % responseText)


        #** Remove file from device
        status, response = self.cloudHandler.removeFileFromDevice(self.device_id, filePath)
        if(status):
            self.log.info("Received the expected response for the remove request")
        else:
            self.log.error("Response content from device: %s" % response.content)
            self.fail("Incorrect response code: %d" % response.status_code)
