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


    def test_01_put_request_busy_state(self):
        """ Sends a put request to upload a file with the overwrite option, and the first three calls
        will be rejected with a busy status. Also verifies the file is generated with the expected content.
        """

        # Initialize vars
        target = "test_put_request_busy_state"
        payload = generateRandomString(500)

        # Initiate action on device
        self.sendDeviceRequestToInitiateAction(target, payload)


        # Wait until the send process is finished
        result, line, readBuffer = self.deviceHandler.readUntilPattern ( pattern="Data service for '.*' was successfully", timeout=20)
        if( not result ):
            self.fail("Console feedback not readed!!")

        # Verify that the busy messages (expected 3 retries) were in the console feedback
        matchingLines = readBuffer.findAll("test_put_request_busy_state: return a busy state...")
        if ( len(matchingLines) != 3 ):
            self.fail("We didn't found the expected busy messages in the console feedback")
        else:
            self.log.info("Found the expected busy messages in the console feedback")

        # Verify file content uploaded on Device Cloud
        filePath = "%s/test/test_overwrite.txt" % self.device_id
        self.verifyFileContent(filePath, payload)

        # Workaround to avoid Device Cloud throttle
        time.sleep(1)

        # Remove file from Device Cloud
        self.cloudHandler.removeFileFromServer(filePath)




    def test_02_put_request_cancel_at_start(self):
        """ Sends a put request to upload a file with the overwrite option, and in the first call
        will be rejected with an error status. Also verifies the file is not generated in Device Cloud.
        """

        # Initialize vars
        target = "test_put_request_cancel_at_start"
        payload = generateRandomString(500)

        # Initiate action on device
        self.sendDeviceRequestToInitiateAction(target, payload)


        # Wait until the send process is finished
        result, line, readBuffer = self.deviceHandler.readUntilPattern ( pattern="Data service error: 1", timeout=20)
        if( not result ):
            self.fail("Console feedback not readed!!")

        # Verify that the error message is in the console feedback
        if ( readBuffer.find("test_put_request_cancel_at_start: return an error on the start...") == -1 ):
            self.fail("We didn't found the expected cancel message in the console feedback")
        else:
            self.log.info("Found the expected cancel message in the console feedback")

        # Verify file content uploaded on Device Cloud
        filePath = "%s/test/test_overwrite.txt" % self.device_id

        # Remove file from Device Cloud
        result,requestResponse = self.cloudHandler.removeFileFromServer(filePath)
        if ( requestResponse.resource["result"]["message"] != "0 items deleted" ):
            # We could remove the file but the file shouldn't exist
            self.fail("We could remove the file but the file should not exist...but exist: %s" % requestResponse.content)
        else:
            self.log.info("Verified that the file not exist on Device Cloud")




    def test_03_put_request_cancel_in_middle_first_call(self):
        """ Sends a put request to upload a file with the overwrite option, and in the first call
        will be rejected with an error status, after fill the data structure. Also verifies the file is not generated in Device Cloud.
        """

        # Initialize vars
        target = "test_put_request_cancel_in_middle_first_call"
        payload = generateRandomString(500)

        # Initiate action on device
        self.sendDeviceRequestToInitiateAction(target, payload)


        # Wait until the send process is finished
        result, line, readBuffer = self.deviceHandler.readUntilPattern ( pattern="Data service error: 1", timeout=20)
        if( not result ):
            self.fail("Console feedback not readed!!")

        # Verify that the error message is in the console feedback
        if ( readBuffer.find("test_put_request_cancel_in_middle_first_packet: return an error on the first call...") == -1 ):
            self.fail("We didn't found the expected cancel message in the console feedback")
        else:
            self.log.info("Found the expected cancel message in the console feedback")

        # Verify file content uploaded on Device Cloud
        filePath = "%s/test/test_overwrite.txt" % self.device_id

        # Remove file from Device Cloud
        result,requestResponse = self.cloudHandler.removeFileFromServer(filePath)
        if ( requestResponse.resource["result"]["message"] != "0 items deleted" ):
            # We could remove the file but the file shouldn't exist
            self.fail("We could remove the file but the file should not exist...but exist: %s" % requestResponse.content)
        else:
            self.log.info("Verified that the file not exist on Device Cloud")



    def test_04_put_request_cancel_in_middle_final_call(self):
        """ Sends a put request to upload a file with the overwrite option, and in the first call
        will be rejected with an error status, after fill the data structure. Also verifies the file is not generated in Device Cloud.
        """

        # Initialize vars
        target = "test_put_request_cancel_in_middle_final_call"
        payload = generateRandomString(5000)

        # Initiate action on device
        self.sendDeviceRequestToInitiateAction(target, payload)


        # Wait until the send process is finished
        result, line, readBuffer = self.deviceHandler.readUntilPattern ( pattern="Data service error: 1", timeout=20)
        if( not result ):
            self.fail("Console feedback not readed!!")

        # Verify that the error message is in the console feedback
        if ( readBuffer.find("test_put_request_cancel_in_middle_final_call: return an error on the last call...") == -1 ):
            self.fail("We didn't found the expected cancel message in the console feedback")
        else:
            self.log.info("Found the expected cancel message in the console feedback")

        # Verify file content uploaded on Device Cloud
        filePath = "%s/test/test_overwrite.txt" % self.device_id

        # Remove file from Device Cloud
        result,requestResponse = self.cloudHandler.removeFileFromServer(filePath)
        if ( requestResponse.resource["result"]["message"] != "0 items deleted" ):
            # We could remove the file but the file shouldn't exist
            self.fail("We could remove the file but the file should not exist...but exist: %s" % requestResponse.content)
        else:
            self.log.info("Verified that the file not exist on Device Cloud")



    def test_05_put_request_timeout(self):
        """ Sends a put request to upload a file with the overwrite option, and in all calls
        will be rejected with an busy status until a timeout. Also verifies the file is not generated in Device Cloud.
        """

        # Initialize vars
        target = "test_put_request_timeout"
        payload = generateRandomString(50000)

        # Initiate action on device
        self.sendDeviceRequestToInitiateAction(target, payload)


        # Wait until the send process is finished
        result, line, readBuffer = self.deviceHandler.readUntilPattern ( pattern="Data service error:", timeout=180)
        if( not result ):
            self.fail("Console feedback readed!!")

        # Verify that the busy message is in the console feedback
        numberBusyMessages = readBuffer.findAll("test_put_request_timeout: return a busy state...")
        if ( len(numberBusyMessages) < 6 ):
            self.fail("We didn't found the expected busy message in the console feedback")
        else:
            self.log.info("Found the expected busy messages (%d) in the console feedback" % len(numberBusyMessages))

        # Verify file content uploaded on Device Cloud
        filePath = "%s/test/test_overwrite.txt" % self.device_id

        # Remove file from Device Cloud
        result,requestResponse = self.cloudHandler.removeFileFromServer(filePath)
        if ( requestResponse.resource["result"]["message"] != "0 items deleted" ):
            # We could remove the file but the file shouldn't exist
            self.fail("We could remove the file but the file should not exist...but exist: %s" % requestResponse.content)
        else:
            self.log.info("Verified that the file not exist on Device Cloud")






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
