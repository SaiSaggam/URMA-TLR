# -*- coding: iso-8859-1 -*-
import cc_testcase
import os



class FileSystemDvtTestCase(cc_testcase.TestCase):


    def test_01_rm_file_return_busy(self):
        """ Sends rm request to delete a file, several times the Connector shows a "busy" message and then continues the execution normally.
        Verifies if the returned response is the expected.
        """

        filePath = os.path.join(self.tempPath, "test_01_rm_file_return_busy.txt")
        fileData = "payload"


        # Send the file to the Device
        status, response = self.cloudHandler.uploadFileToDevice(self.device_id, filePath, fileData)

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



        #** Remove file from device
        status, response = self.cloudHandler.removeFileFromDevice(self.device_id, filePath)
        if(status):
            self.log.info("Received the expected response for the remove request")
        else:
            self.log.error("Response content from device: %s" % response.content)
            self.fail("Incorrect response code: %d" % response.status_code)



        #** Verify the console message for each busy state
        result, line, dataBuffer = self.deviceHandler.readUntilPattern ( pattern="Connector busy. Wait till response is sent...", timeout=30)
        if(result):
            # Verify if we found 4 busy messages
            if( len(dataBuffer.findAll("Connector busy. Wait till response is sent...")) == 4):
                self.log.info("Connector busy feedback received correctly")
            else:
                self.fail("Console feedback for busy status NOT received.\n dataBuffer: %s" % dataBuffer)
        else:
            self.fail("Console feedback for busy messages NOT found")


        #** Verify that the file was removed from device
        status, response = self.cloudHandler.removeFileFromDevice(self.device_id, filePath)

        if(not status):
            # Status 200. Checking the received response
            self.fail("Received response from device: \"%s\" and is not the expected (code %s)" % (response.content, response.status_code) )
        else:
            # Get response text
            responseText = response.resource["sci_reply"]["file_system"]["device"]["commands"]["rm"]["error"]["#text"]

            if ( responseText != "No such file or directory" ):
                self.fail("Incorrect response from device: %s" % response.content)
            else:
                self.log.info("Received expected response '%s' from device" % responseText)



    def test_02_rm_file_error_permission_denied(self):
        """ Sends rm request to delete a file to simulate a permission denied error.
        Verifies if the returned response is the expected.
        """

        self.sendRmRequestAndVerifyErrorResponse( "test_02_rm_file_error_permission_denied.txt", "Permission denied")



    def test_03_rm_file_error_invalid_parameter(self):
        """ Sends rm request to delete a file to simulate an invalid parameter error.
        Verifies if the returned response is the expected.
        """

        self.sendRmRequestAndVerifyErrorResponse( "test_03_rm_file_error_invalid_parameter.txt", "Invalid argument")



    def test_04_rm_file_error_out_of_memory(self):
        """ Sends rm request to delete a file to simulate an out of memory error.
        Verifies if the returned response is the expected.
        """

        self.sendRmRequestAndVerifyErrorResponse( "test_04_rm_file_error_out_of_memory.txt", "Cannot allocate memory")



    def test_05_rm_file_error_path_not_found(self):
        """ Sends rm request to delete a file to simulate a path not found error.
        Verifies if the returned response is the expected.
        """

        self.sendRmRequestAndVerifyErrorResponse( "test_05_rm_file_error_path_not_found.txt", "No such file or directory")



    def test_06_rm_file_error_insufficient_storage_space(self):
        """ Sends rm request to delete a file to simulate an insufficient storage space left error.
        Verifies if the returned response is the expected.
        """

        self.sendRmRequestAndVerifyErrorResponse( "test_06_rm_file_error_insufficient_storage_space.txt", "No space left on device")



    def test_07_rm_file_error_unspecified(self):
        """ Sends rm request to delete a file to simulate an unknown error.
        Verifies if the returned response is the expected.
        """

        self.sendRmRequestAndVerifyErrorResponse( "test_07_rm_file_error_unspecified.txt", "Unspecified error")



    def test_08_rm_file_error_timeout(self):
        """ Sends rm request to delete a file to simulate a timeout error.
        Verifies if the returned response is the expected.
        """

        self.sendRmRequestAndVerifyErrorResponse( "test_08_rm_file_error_timeout.txt", "Connection timed out")






    def sendRmRequestAndVerifyErrorResponse(self, filename, expectedResponse):

        filePath = os.path.join(self.tempPath, filename)
        fileData = "payload"


        # Remove file from device
        status, response = self.cloudHandler.removeFileFromDevice(self.device_id, filePath)

        if(not status):
            # Status 200. Checking the received response
            self.fail("Received response from device: \"%s\" and is not the expected (code %s)" % (response.content, response.status_code) )
        else:
            # Get response text
            responseText = response.resource["sci_reply"]["file_system"]["device"]["commands"]["rm"]["error"]["#text"]

            if ( responseText != expectedResponse ):
                self.fail("Incorrect response from device: %s" % response.content)
            else:
                self.log.info("Received expected response '%s' from device" % responseText)
