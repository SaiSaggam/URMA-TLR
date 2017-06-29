# -*- coding: iso-8859-1 -*-
import cc_testcase
import os



class FileSystemDvtTestCase(cc_testcase.TestCase):


    def test_01_ls_dir_return_busy(self):
        """ Sends list dir request, several times the Connector shows a "busy" message and then continues the execution normally.
        Verifies if the returned response is the expected.
        """

        folderPath = os.path.join(self.tempPath, "test_01_ls_dir_return_busy")
        subfolderPath1 = os.path.join(folderPath, "test_folder_1")
        subfolderPath2 = os.path.join(folderPath, "test_folder_2")

        #** Create new folder to identify the test case
        self.createDirectory(folderPath)
        self.createDirectory(subfolderPath1)
        self.createDirectory(subfolderPath2)

        directoryList = []
        directoryList.append(subfolderPath1)
        directoryList.append(subfolderPath2)


        #** Get the list from device
        status, response = self.cloudHandler.listFilesytemFromDevice(self.device_id, folderPath)

        if(status):
            # Status 200. Checking the received response
            obtainedDirectoryList = response.resource["sci_reply"]["file_system"]["device"]["commands"]["ls"]["dir"]

            ## Verify all directories are in the list
            # Number of original directories to find
            counterFoundDirectories = len(directoryList)

            for eachDirectory in obtainedDirectoryList:

                eachObtainedDirectoryPath = eachDirectory["@path"]

                # Find the original file
                for eachDirectoryInfo in directoryList:
                    if ( eachObtainedDirectoryPath == eachDirectoryInfo ):
                        self.log.info("Found the directory '%s'" % eachObtainedDirectoryPath)
                        counterFoundDirectories-=1

            # Check if all original directories were found
            if ( counterFoundDirectories != 0 ):
                self.fail("Not found all directories!!! missing directories '%s'" % counterFoundDirectories)
            else:
                self.log.info("All directories (%s) were found....OK" % len(directoryList))


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
                self.fail("Console feedback for busy status NOT received")
        else:
            self.fail("Console feedback for busy messages NOT found")




    def test_02_ls_dir_error_permission_denied(self):
        """ Sends list dir request to simulate a permission denied error.
        Verifies if the returned response is the expected.
        """

        folderPath = os.path.join(self.tempPath, "test_02_ls_dir_error_permission_denied")

        #** Create new folder to identify the test case
        self.createDirectory(folderPath)

        #** Send a filesystem ls query
        self.sendLsRequestAndVerifyErrorResponse( folderPath, "Permission denied")



    def test_03_ls_dir_error_invalid_parameter(self):
        """ Sends list dir request to simulate an invalid parameter error.
        Verifies if the returned response is the expected.
        """

        folderPath = os.path.join(self.tempPath, "test_03_ls_dir_error_invalid_parameter")

        #** Create new folder to identify the test case
        self.createDirectory(folderPath)

        #** Send a filesystem ls query
        self.sendLsRequestAndVerifyErrorResponse( folderPath, "Invalid argument")



    def test_04_ls_dir_error_out_of_memory(self):
        """ Sends list dir request to simulate an out of memory error.
        Verifies if the returned response is the expected.
        """

        folderPath = os.path.join(self.tempPath, "test_04_ls_dir_error_out_of_memory")

        #** Create new folder to identify the test case
        self.createDirectory(folderPath)

        #** Send a filesystem ls query
        self.sendLsRequestAndVerifyErrorResponse( folderPath, "Cannot allocate memory")



    def test_05_ls_dir_error_path_not_found(self):
        """ Sends list dir request to simulate a path not found error.
        Verifies if the returned response is the expected.
        """

        folderPath = os.path.join(self.tempPath, "test_05_ls_dir_error_path_not_found")

        #** Send a filesystem ls query
        self.sendLsRequestAndVerifyErrorResponse( folderPath, "No such file or directory")



    def test_06_ls_dir_error_insufficient_storage_space(self):
        """ Sends list dir request to simulate an insufficient storage space left error.
        Verifies if the returned response is the expected.
        """

        folderPath = os.path.join(self.tempPath, "test_06_ls_dir_error_insufficient_storage_space")

        #** Create new folder to identify the test case
        self.createDirectory(folderPath)

        #** Send a filesystem ls query
        self.sendLsRequestAndVerifyErrorResponse( folderPath, "No space left on device")



    def test_07_ls_dir_error_unspecified(self):
        """ Sends list dir request to simulate an unknown error.
        Verifies if the returned response is the expected.
        """

        folderPath = os.path.join(self.tempPath, "test_07_ls_dir_error_unspecified")

        #** Create new folder to identify the test case
        self.createDirectory(folderPath)

        #** Send a filesystem ls query
        self.sendLsRequestAndVerifyErrorResponse( folderPath, "Unspecified error")



    def test_08_ls_dir_error_timeout(self):
        """ Sends list dir request to simulate a timeout error.
        Verifies if the returned response is the expected.
        """

        folderPath = os.path.join(self.tempPath, "test_08_ls_dir_error_timeout")

        #** Create new folder to identify the test case
        self.createDirectory(folderPath)

        #** Send a filesystem ls query
        self.sendLsRequestAndVerifyErrorResponse( folderPath, "Connection timed out")









    def createDirectory(self, directoryPath):

        if not os.path.exists(directoryPath):
            os.makedirs(directoryPath)




    def sendLsRequestAndVerifyErrorResponse(self, folderPath, expectedResponse):


        #** Get the list from device
        status, response = self.cloudHandler.listFilesytemFromDevice(self.device_id, folderPath)

        if(not status):
            # Status 200. Checking the received response
            self.fail("Received response from device: \"%s\" and is not the expected (code %s)" % (response.content, response.status_code) )
        else:
            # Get response text
            responseText = response.resource["sci_reply"]["file_system"]["device"]["commands"]["ls"]["error"]["#text"]

            if ( responseText != expectedResponse ):
                self.fail("Incorrect response from device: %s" % response.content)
            else:
                self.log.info("Received expected response '%s' from device" % responseText)

