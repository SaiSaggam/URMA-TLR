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


    def test_01_send_file_valid_path_without_content(self):
        """ Sends a put_file request to upload a file to the device without payload
        Verifies if the returned response is the expected and the generated file has the expected content.
        """

        filePath = os.path.join(self.tempPath, "test_01_send_file_valid_path_without_content.txt")
        fileData = ""

        # Send and verify the file
        self.uploadFileAndVerifyFileContent(filePath, fileData)

        # Remove file from device
        status, response = self.cloudHandler.removeFileFromDevice(self.device_id, filePath)
        if(status):
            self.log.info("Received the expected response for the remove request")
        else:
            self.log.error("Response content from device: %s" % response.content)
            self.fail("Incorrect response code: %d" % response.status_code)




    def test_02_send_file_valid_path_with_increasing_content(self):
        """ Sends a put_file request to upload a file to the device several times with a different payload in each iteration.
        Verifies if the returned response is the expected and the generated file has the expected content.
        """

        maxFileSize = 23453

        fileSizeList = range(0,maxFileSize, 1024)
        fileSizeList.append(maxFileSize)

        for eachFileSize in fileSizeList:

            self.log.info("*** Sending a file size of %s bytes..." % eachFileSize)

            filePath = os.path.join(self.tempPath, "test_02_send_file_valid_path_with_increasing_content_size_%s.txt" % eachFileSize)
            fileData = fileContent[:eachFileSize]

            # Send and verify the file
            self.uploadFileAndVerifyFileContent(filePath, fileData)

            # Remove file from device
            status, response = self.cloudHandler.removeFileFromDevice(self.device_id, filePath)
            if(status):
                self.log.info("Received the expected response for the remove request")
            else:
                self.log.error("Response content from device: %s" % response.content)
                self.fail("Incorrect response code: %d" % response.status_code)



    def test_03_send_file_valid_path_with_big_content(self):
        """ Sends a put_file request to upload a file to the device with the maximum payload allowed.
        Verifies if the returned response is the expected and the generated file has the expected content.
        """

        fileSizeList = [1048576, 2000000] # 1MB,2MB,...

        for eachFileSize in fileSizeList:

            self.log.info("*** Sending a file size of %s bytes..." % eachFileSize)

            filePath = os.path.join(self.tempPath, "test_03_send_file_valid_path_with_big_content_size_%s.txt" % eachFileSize)
            fileData = fileContent[:eachFileSize]

            # Send and verify the file
            self.uploadFileAndVerifyFileContent(filePath, fileData)

            # Remove file from device
            status, response = self.cloudHandler.removeFileFromDevice(self.device_id, filePath)
            if(status):
                self.log.info("Received the expected response for the remove request")
            else:
                self.log.error("Response content from device: %s" % response.content)
                self.fail("Incorrect response code: %d" % response.status_code)



    def test_04_send_data_chunks_with_valid_offset(self):
        """ Sends several put_file requests to upload different data chunks over the same file with a different offset.
        Verifies if the returned response is the expected and the generated file has the expected content.
        """

        filePath = os.path.join(self.tempPath, "test_04_send_file_with_offset.txt")
        originalFileData = fileContent

        # Send the original file to the Device
        status, response = self.cloudHandler.uploadFileToDevice(self.device_id, filePath, originalFileData, timeout = 120)

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


        # Generate a new file content
        newFileContent = ""
        for i in xrange(MAX_TEST_FILE_SIZE):
            newFileContent += chr(random.randint(0, 255))


        # Send a request to override the original file
        offsetList =  [
            (100, 0),
            (511, 1),
            (8192, 512),
            (12345, 1580),
            (35201, 4097),
            (60000, MAX_TEST_FILE_SIZE-20000),
            ]

        for eachOffset, eachFileSize in offsetList:

            self.log.info("*** Sending a data size of '%s' bytes with an offset of '%s'..." % (eachFileSize, eachOffset) )
            dataBlock = newFileContent[:eachFileSize]

            self.uploadFileAndVerifyFileContent(filePath, dataBlock, offset=eachOffset, fileSize=eachFileSize )


        # Remove file from device
        status, response = self.cloudHandler.removeFileFromDevice(self.device_id, filePath)
        if(status):
            self.log.info("Received the expected response for the remove request")
        else:
            self.log.error("Response content from device: %s" % response.content)
            self.fail("Incorrect response code: %d" % response.status_code)





    def test_05_send_data_chunks_with_truncate_option(self):
        """ Sends several put_file requests to upload different data chunks over the same file with the truncate option.
        Verifies if the returned response is the expected and the generated file has the expected content.
        """

        filePath = os.path.join(self.tempPath, "test_05_send_data_chunks_with_truncate_option.txt")
        originalFileData = fileContent

        # Send the original file to the Device
        status, response = self.cloudHandler.uploadFileToDevice(self.device_id, filePath, originalFileData, timeout = 120)

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


        # Generate a new file content
        newFileContent = ""
        for i in xrange(MAX_TEST_FILE_SIZE):
            newFileContent += chr(random.randint(0, 255))


        # Send a request to override the original file
        offsetList =  [
            (60000, MAX_TEST_FILE_SIZE-80000),
            (4321, 16037),
            (1111, 580),
            (500, 83),
            (100, 0),
            (0, 4),
            (0, 1),
            (0, 0),
            ]

        for eachOffset, eachFileSize in offsetList:

            self.log.info("*** Sending a data size of '%s' bytes with an offset of '%s'..." % (eachFileSize, eachOffset) )
            dataBlock = newFileContent[:eachFileSize]

            self.uploadFileAndVerifyFileContent(filePath, dataBlock, offset=eachOffset, fileSize=eachFileSize, truncate=True )

            ## Verify that the truncate option was effect and the file size is the expected
            # Get file content from device
            status, obtainedFileData, response = self.cloudHandler.downloadFileFromDevice(self.device_id, filePath, timeout = 120)

            if(status):
                self.log.info("Received the expected response for the get_file request")
            else:
                self.log.error("Response content from device: %s" % response.content)
                self.fail("Incorrect response code: %d" % response.status_code)

            # Verify if the expected content match with the obtained content
            expectedTruncateFileContent = originalFileData[:eachOffset] + dataBlock

            if ( expectedTruncateFileContent == obtainedFileData ):
                self.log.info("Truncated file content size of '%s' match with the expected!!!" % len(expectedTruncateFileContent))
            else:
                self.log.error("Truncated file content does not match with the expected:\n Obtained: '%s'\n Expected: '%s'" % (obtainedFileData, expectedTruncateFileContent) )
                self.fail("Truncated file content does not match with the expected!!!")



        # Remove file from device
        status, response = self.cloudHandler.removeFileFromDevice(self.device_id, filePath)
        if(status):
            self.log.info("Received the expected response for the remove request")
        else:
            self.log.error("Response content from device: %s" % response.content)
            self.fail("Incorrect response code: %d" % response.status_code)





    def test_06_verify_filesystem_ls_command(self):
        """ Sends list folder request over file system and verify that the response match with the expected files.
        """
 
        # Generate a suite of files and directories to test
        fileList,directoryList = self.generateFilesAndDirectories("test_06_verify_filesystem_ls_command")



        # Send a filesystem ls query
        folderPath = self.tempPath

        status, response = self.cloudHandler.listFilesytemFromDevice(self.device_id, folderPath)

        if(status):
            # Status 200. Checking the received response
            obtainedFileList = response.resource["sci_reply"]["file_system"]["device"]["commands"]["ls"]["file"]
            obtainedDirectoryList = response.resource["sci_reply"]["file_system"]["device"]["commands"]["ls"]["dir"]

            ## Verify all uploaded files are in the list
            # Number of original files to find
            counterFoundFiles = len(fileList)

            for eachFile in obtainedFileList:

                eachObtainedFilePath = eachFile["@path"]
                eachObtainedFileSize = eachFile["@size"]

                # Find the original file
                for eachFileInfo in fileList:
                    if ( eachObtainedFilePath == eachFileInfo["path"] ):
                        self.log.info("Found the file '%s'" % eachObtainedFilePath)
                        counterFoundFiles-=1

            # Check if all original files were found
            if ( counterFoundFiles != 0 ):
                self.fail("Not found all files!!! missing files '%s'" % counterFoundFiles)
            else:
                self.log.info("All files (%s) were found....OK" % len(fileList))



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




        # Clean temporal folder
        for eachFileInfo in fileList:
            # Remove file from device
            status, response = self.cloudHandler.removeFileFromDevice(self.device_id, eachFileInfo["path"])
            if(status):
                self.log.info("Received the expected response for the remove request")
            else:
                self.log.error("Response content from device: %s" % response.content)
                self.fail("Incorrect response code: %d" % response.status_code)

        for eachDirectoryInfo in directoryList:
            os.rmdir( eachDirectoryInfo )



    def test_07_verify_filesystem_ls_command_with_hash(self):
        """ Sends list folder request over file system with the hash flag enabled and verify that the response match with the expected files.
        """

        # Generate a suite of files and directories to test
        fileList,directoryList = self.generateFilesAndDirectories("test_07_verify_filesystem_ls_command_with_hash")

        # Send a filesystem ls query
        folderPath = self.tempPath

        hashAlgorithmList = [None,"none","crc32","md5","any"]

        for hashAlgorithm in hashAlgorithmList:

            self.log.info("")
            self.log.info("***** Starting test to verify file system with hash '%s'" % hashAlgorithm)

            status, response = self.cloudHandler.listFilesytemFromDevice(self.device_id, folderPath, hashAlgorithm = hashAlgorithm)

            if(status):
                # Status 200. Checking the received response
                obtainedFileList = response.resource["sci_reply"]["file_system"]["device"]["commands"]["ls"]["file"]

                ## Verify all uploaded files are in the list
                # Number of original files to find
                counterFoundFiles = len(fileList)

                for eachFile in obtainedFileList:

                    eachObtainedFilePath = eachFile["@path"]
                    eachObtainedFileSize = eachFile["@size"]

                    # Find the original file
                    for eachFileInfo in fileList:
                        if ( eachObtainedFilePath == eachFileInfo["path"] ):
                            self.log.info("Found the file '%s'" % eachObtainedFilePath)

                            if ( hashAlgorithm in ["md5","any"] ):
                                # Verify that the element hash match with the expected for the supported hash algorithms
                                eachObtainedFileHash = eachFile["@hash"]

                                if ( eachObtainedFileHash.upper() == eachFileInfo[hashAlgorithm].upper() ):
                                    self.log.info("Hash '%s' match with the expected!!!" % eachObtainedFileHash)
                                    counterFoundFiles-=1
                                else:
                                    self.log.error("Hash not match '%s' with the expected '%s'!!!" % (eachObtainedFileHash,eachFileInfo[hashAlgorithm]) )
                            else:
                                # Verify that the element hash is not exist
                                if ( "@hash" in eachFile ):
                                    self.fail("hash element exist for an unsupported type")
                                else:
                                    counterFoundFiles-=1

                # Check if all original files were found
                if ( counterFoundFiles != 0 ):
                    self.fail("Not found all files!!! missing files '%s'" % counterFoundFiles)
                else:
                    self.log.info("All files (%s) were found....OK" % len(fileList))

            else:
                self.log.error("Response content from device: %s" % response.content)
                self.fail("Incorrect response code: %d" % response.status_code)



        # Clean temporal folder
        for eachFileInfo in fileList:
            # Remove file from device
            status, response = self.cloudHandler.removeFileFromDevice(self.device_id, eachFileInfo["path"])
            if(status):
                self.log.info("Received the expected response for the remove request")
            else:
                self.log.error("Response content from device: %s" % response.content)
                self.fail("Incorrect response code: %d" % response.status_code)

        for eachDirectoryInfo in directoryList:
            os.rmdir( eachDirectoryInfo )





    def generateFilesAndDirectories(self, testIdentifier):

        ## Compound a list of files
        fileList = []

        for i in range(0,10):
            fileInfo = {}
            fileInfo["name"] = "%s_%d.txt" % (testIdentifier,i)
            fileInfo["path"] = os.path.join(self.tempPath, fileInfo["name"])
            fileInfo["size"] = random.randint(0, 30000)
            fileInfo["data"] = fileContent[:fileInfo["size"]]
            fileInfo["md5"] = self.md5Hash( fileInfo["data"] )
            fileInfo["any"] = fileInfo["md5"]
            fileInfo["crc32"] = self.crc32Hash( fileInfo["data"] )
            fileList.append(fileInfo)

        ## Generates files on device
        for eachFileInfo in fileList:
            filePath = eachFileInfo["path"]
            fileData = eachFileInfo["data"]

            # Send the file to the Device
            status, response = self.cloudHandler.uploadFileToDevice(self.device_id, filePath, fileData, timeout = 120)

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


        ## Compound a list of directories
        directoryList = []
        for i in range(1,4):
            folderPath = os.path.join( self.tempPath, "%s_folder_%d" % (testIdentifier,i) )
            directoryList.append(folderPath)


        for eachDirectory in directoryList:

            directoryPath = os.path.join( self.tempPath, eachDirectory)

            if not os.path.exists(directoryPath):
                os.makedirs(directoryPath)



        return fileList,directoryList




    def uploadFileAndVerifyFileContent(self, filePath, fileData, offset = None, truncate = None, fileSize = None):

        # Send the file to the Device
        status, response = self.cloudHandler.uploadFileToDevice(self.device_id, filePath, fileData, offset = offset, truncate = truncate, timeout = 120)

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


        # Get the file from device
        status, obtainedFileData, response = self.cloudHandler.downloadFileFromDevice(self.device_id, filePath, fileSize = fileSize, offset = offset, timeout = 120)

        if(status):
            self.log.info("Received the expected response for the get_file request")
        else:
            self.log.error("Response content from device: %s" % response.content)
            self.fail("Incorrect response code: %d" % response.status_code)


        # Verify file content
        if ( fileData == obtainedFileData ):
            #self.log.warning("Received file content from device: \"%s\" is the expected: \"%s\"" % (obtainedFileData, fileData) )
            self.log.info("File content received was successfully!")
        else:
            self.fail("Received file content from device: \"%s\"\n is not the expected: \"%s\"" % (obtainedFileData, fileData) )





    def md5Hash(self, fileData):
        import hashlib
        m = hashlib.md5()
        m.update(fileData)
        return m.hexdigest()


    def crc32Hash(self, fileData):
        import binascii
        # Obtain the CRC32 from file data
        crc32 = (binascii.crc32(fileData) & 0xFFFFFFFF)
        crc32_decimal = "%s" % crc32
        crc32_hexadecimal = "%08X" % crc32
        return crc32_decimal
