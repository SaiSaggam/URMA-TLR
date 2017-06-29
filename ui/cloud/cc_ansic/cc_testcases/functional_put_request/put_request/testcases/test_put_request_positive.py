# -*- coding: iso-8859-1 -*-
import cc_testcase
import requests.exceptions

import time


import string           # Provides a number of useful constants and classes for strings
import random           # Implements pseudo-random number generators for various distributions


def generateRandomString(length):
    charList = [random.choice(string.ascii_letters + string.digits + " ") for n in xrange(length)]
    randomString = "".join(charList)
    return randomString


class PutRequestDvtTestCase(cc_testcase.TestCase):


    def test_01_put_request_send_option_overwrite_zero_bytes(self):
        """ Sends a put request to upload zero bytes data with the overwrite option.
        Verifies the file is generated with an empty content.
        """

        # Initialize vars
        target = "test_put_request_send_option_overwrite"
        payload = ""

        # Initiate action on device
        self.sendDeviceRequestToInitiateAction(target, payload)


        # Wait until the send process is finished
        result, line, readBuffer = self.deviceHandler.readUntilPattern ( pattern="Data service for '.*' was successfully", timeout=20)
        if( not result ):
            self.fail("Console feedback not readed!!")


        # Verify file content uploaded on Device Cloud
        filePath = "%s/test/test_overwrite.txt" % self.device_id
        self.verifyFileContent(filePath, payload)

        # Workaround to avoid Device Cloud throttle
        time.sleep(1)

        # Remove file from Device Cloud
        self.cloudHandler.removeFileFromServer(filePath)



    def test_02_put_request_send_option_overwrite_decreasing_bytes(self):
        """ Sends a put request to upload data over the same file with the overwrite option.
        Starts sending a big data and ends with small data, and verifies the file is override and content is the expected.
        """

        # We do it several times to verify that the override process is ok
        maxLoops = 20
        dataChunkSize = 500 # This is the chunk size
        filePath = "%s/test/test_overwrite.txt" % self.device_id

        for i in range(0,maxLoops):
            # Initialize vars
            target = "test_put_request_send_option_overwrite"
            payloadLength = (maxLoops*dataChunkSize) - (dataChunkSize * i)

            self.log.info("")
            self.log.info("*** Send a file content length of '%d' with option override" % payloadLength)
            # Generate a random file content to send like the payload
            payload = generateRandomString(payloadLength)

            # Initiate action on device
            self.sendDeviceRequestToInitiateAction(target, payload)


            # Wait until the send process is finished
            result, line, readBuffer = self.deviceHandler.readUntilPattern ( pattern="Data service for '.*' was successfully", timeout=20)
            if( not result ):
                self.fail("Console feedback not readed!!")


            # Verify file content uploaded on Device Cloud
            self.verifyFileContent(filePath, payload)

            # Workaround to avoid Device Cloud throttle
            time.sleep(1)


    def test_03_put_request_send_option_overwrite_increasing_bytes(self):
        """ Sends a put request to upload data over the same file with the overwrite option.
        Starts sending a small data and ends with big data, and verifies the file is override and content is the expected.
        """

        # We do it several times to verify that the override process is ok
        dataChunkSize = 500 # This is the chunk size
        filePath = "%s/test/test_overwrite.txt" % self.device_id

        for i in range(1,30):
            # Initialize vars
            target = "test_put_request_send_option_overwrite"
            payloadLength = dataChunkSize * i

            self.log.info("")
            self.log.info("*** Send a file content length of '%d' with option override" % payloadLength)
            # Generate a random file content to send like the payload
            payload = generateRandomString(payloadLength)

            # Initiate action on device
            self.sendDeviceRequestToInitiateAction(target, payload)


            # Wait until the send process is finished
            result, line, readBuffer = self.deviceHandler.readUntilPattern ( pattern="Data service for '.*' was successfully", timeout=20)
            if( not result ):
                self.fail("Console feedback not readed!!")


            # Verify file content uploaded on Device Cloud
            self.verifyFileContent(filePath, payload)

            # Workaround to avoid Device Cloud throttle
            time.sleep(1)



    def test_04_put_request_send_option_append_zero_bytes(self):
        """ Sends a put request to upload zero bytes data with the append option.
        Verifies the file is generated with an empty content.
        """

        # Initialize vars
        target = "test_put_request_send_option_append"
        payload = ""

        # Initiate action on device
        self.sendDeviceRequestToInitiateAction(target, payload)


        # Wait until the send process is finished
        result, line, readBuffer = self.deviceHandler.readUntilPattern ( pattern="Data service for '.*' was successfully", timeout=20)
        if( not result ):
            self.fail("Console feedback not readed!!")


        # Verify file content uploaded on Device Cloud
        filePath = "%s/test/test_append.txt" % self.device_id
        self.verifyFileContent(filePath, payload)

        # Workaround to avoid Device Cloud throttle
        time.sleep(1)

        # Remove file from Device Cloud
        self.cloudHandler.removeFileFromServer(filePath)



    def test_05_put_request_send_option_append_decreasing_bytes(self):
        """ Sends a put request to upload data over the same file with the append option.
        Starts sending a big data and ends with small data, and verifies the file is append and content is the expected.
        """

        # We do it several times to verify that the override process is ok
        totalFileContent = ""
        dataChunkSize = 500 # This is the chunk size
        maxLoops = 20
        filePath = "%s/test/test_append.txt" % self.device_id

        for i in range(0,maxLoops):
            # Initialize vars
            target = "test_put_request_send_option_append"
            payloadLength = (maxLoops*dataChunkSize) - (dataChunkSize * i)

            self.log.info("")
            self.log.info("*** Send a file content length of '%d' with option append" % payloadLength)
            # Generate a random file content to send like the payload
            payload = generateRandomString(payloadLength)

            # Add the new payload to the global file content
            totalFileContent = "%s%s" % (totalFileContent,payload)

            # Initiate action on device
            self.sendDeviceRequestToInitiateAction(target, payload)


            # Wait until the send process is finished
            result, line, readBuffer = self.deviceHandler.readUntilPattern ( pattern="Data service for '.*' was successfully", timeout=20)
            if( not result ):
                self.fail("Console feedback not readed!!")


            # Verify file content uploaded on Device Cloud
            self.verifyFileContent(filePath, totalFileContent)

            # Workaround to avoid Device Cloud throttle
            time.sleep(1)

        # Remove file from Device Cloud
        self.cloudHandler.removeFileFromServer(filePath)




    def test_06_put_request_send_option_overwrite_increasing_bytes(self):
        """ Sends a put request to upload data over the same file with the append option.
        Starts sending a small data and ends with big data, and verifies the file is append and content is the expected.
        """

        # We do it several times to verify that the override process is ok
        totalFileContent = ""
        dataChunkSize = 500 # This is the chunk size
        filePath = "%s/test/test_append.txt" % self.device_id

        for i in range(1,20):
            # Initialize vars
            target = "test_put_request_send_option_append"
            payloadLength = dataChunkSize * i

            self.log.info("")
            self.log.info("*** Send a file content length of '%d' with option override" % payloadLength)
            # Generate a random file content to send like the payload
            payload = generateRandomString(payloadLength)

            # Add the new payload to the global file content
            totalFileContent = "%s%s" % (totalFileContent,payload)

            # Initiate action on device
            self.sendDeviceRequestToInitiateAction(target, payload)


            # Wait until the send process is finished
            result, line, readBuffer = self.deviceHandler.readUntilPattern ( pattern="Data service for '.*' was successfully", timeout=20)
            if( not result ):
                self.fail("Console feedback not readed!!")


            # Verify file content uploaded on Device Cloud
            self.verifyFileContent(filePath, totalFileContent)

            # Workaround to avoid Device Cloud throttle
            time.sleep(1)


        # Remove file from Device Cloud
        self.cloudHandler.removeFileFromServer(filePath)



    def test_07_put_request_send_option_archive_zero_bytes(self):
        """ Sends a put request to upload zero bytes data with the archive option.
        Verifies the file is generated with an empty content and is creatred a new entry in the historic database.
        """

        # Initialize vars
        target = "test_put_request_send_option_archive"
        payload = ""

        # Initiate action on device
        self.sendDeviceRequestToInitiateAction(target, payload)


        # Wait until the send process is finished
        result, line, readBuffer = self.deviceHandler.readUntilPattern ( pattern="Data service for '.*' was successfully", timeout=20)
        if( not result ):
            self.fail("Console feedback not readed!!")


        # Verify file content uploaded on Device Cloud
        filePath = "%s/test/test_archive.txt" % self.device_id

        #** Verify file content in the historic database
        self.log.info("Verify file content in the historic database...")
        self.verifyFileContent(filePath, payload, historic=True)

        #** Verify file content in the file data service
        self.log.info("Verify file content in the file data service...")
        self.verifyFileContent(filePath, payload)


        # Workaround to avoid Device Cloud throttle
        time.sleep(1)

        # Remove file from Historic database in Device Cloud
        self.cloudHandler.removeFileHistoryFromServer(filePath)
        # Remove file from file data service in Device Cloud
        self.cloudHandler.removeFileHistoryFromServer(filePath)





    def test_08_put_request_send_option_archive_increasing_bytes(self):
        """ Sends a put request to upload data over the same file with the archive option.
        Starts sending a small data and ends with big data, and verifies the file is archive and content is the expected.
        """

        # We do it several times to verify that the archive process is ok
        dataChunkSize = 500 # This is the chunk size
        filePath = "%s/test/test_archive.txt" % self.device_id

        for i in range(1,20):
            # Initialize vars
            target = "test_put_request_send_option_archive"
            payloadLength = dataChunkSize * i

            self.log.info("")
            self.log.info("*** Send a file content length of '%d' with option archive" % payloadLength)
            # Generate a random file content to send like the payload
            payload = generateRandomString(payloadLength)

            # Initiate action on device
            self.sendDeviceRequestToInitiateAction(target, payload)


            # Wait until the send process is finished
            result, line, readBuffer = self.deviceHandler.readUntilPattern ( pattern="Data service for '.*' was successfully", timeout=20)
            if( not result ):
                self.fail("Console feedback not readed!!")


            # Verify file content uploaded on Device Cloud
            #** Verify file content in the historic database
            self.log.info("Verify file content in the historic database...")
            self.verifyFileContent(filePath, payload, historic=True)

            #** Verify file content in the file data service
            self.log.info("Verify file content in the file data service...")
            self.verifyFileContent(filePath, payload)


            # Workaround to avoid Device Cloud throttle
            time.sleep(1)


        # Remove file from Historic database in Device Cloud
        self.cloudHandler.removeFileHistoryFromServer(filePath)
        # Remove file from file data service in Device Cloud
        self.cloudHandler.removeFileHistoryFromServer(filePath)






    def sendDeviceRequestToInitiateAction(self, target, payload, expectedResponse="Launch successful"):

        # Send Device Request to the target to initiate action
        status, response = self.cloudHandler.sendDeviceRequest(self.device_id, target, payload)

        if(status):
            # Status 200. Checking the received response
            responseText = response.resource["sci_reply"]["data_service"]["device"]["requests"]["device_request"]["#text"]
            if(responseText == expectedResponse):
                self.log.info("Received the expected response\n")
            else:
                self.fail("Received response from device: \"%s\" is not the expected\n" % responseText)
        else:
            self.log.error("Response content from device: %s" % response.content)
            self.fail("Incorrect response code: %d" % response.status_code)



    def verifyFileContent(self, filePath, expectedContent, historic=False):

        maxRetries = 3
        counter = 0
        verificationResult = False
        message = ""

        while ( counter < maxRetries ):
            counter += 1

            # Get File from Device Cloud
            if ( historic ):
                result,fileContent,requestResponse = self.cloudHandler.downloadFileHistoryFromServer(filePath)
            else:
                result,fileContent,requestResponse = self.cloudHandler.downloadFileFromServer(filePath)

            if(not result):
                self.log.error("Response Error: %s" % requestResponse.content)

            if ( fileContent != expectedContent ):
                message = "File content is not the expected: Received '%s'\n , Expected '%s'" % (fileContent, expectedContent)
                self.log.warning( message )
                time.sleep(1)
                continue
            else:
                self.log.info("File content matches with the expected (total length %d)!!!" % len(fileContent))
                verificationResult = True
                break


        if ( not verificationResult ):
            self.fail( message )
