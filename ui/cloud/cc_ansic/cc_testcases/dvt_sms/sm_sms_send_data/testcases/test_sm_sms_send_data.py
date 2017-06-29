import time
import cc_testcase

filedata = 'FileData/~/'
file_name1 = 'test/sm_data.txt'
file_name2 = 'test/sm_data_noresp.txt'

class SmSmsSendDataTestCase(cc_testcase.SMSTestCase):

    def test_sm_sms_send_data(self):
    
        """ This routine gets the sent file from the cloud and   
        verifies that the correct data is pushed """
        
        self.log.info("Begin sm_send_data test.")

        expected_content = 'Etherios sm_send_data sample data\n'

        # Make sure through console feedback that the file has been sent & receibed by the server
        status1, line1, buffer1 = self.deviceHandler.readUntilPattern ( pattern="Received success response from server", timeout=300)
        self.log.info("Console feedback for file1=%d" % status1)
        if status1 == False:
            self.failAndStopAllTests("Console feedback for file1 NOT arrived")

        # Create path to file.
        file_location = filedata + self.device_id + '/' + file_name1
        self.log.info("Attempting to Retrieve data from File Location: %s" % (file_location))

        # Check for existing file and its contents.
        try:
            file_content = self.cloudHandler.getRequest(file_location).content
        except Exception, e:
            self.failAndStopAllTests('Failed to Get Device Contents.  Reason: %s' % e)
        
        # Verify file's contents
        if file_content != expected_content:
           self.failAndStopAllTests("Received content: %s does not match expected: %s" % (file_content, expected_content))

        self.log.info("File %s: %s" % (file_name1, file_content))

        file2_message = "Data service error: 0"
        buffer_idx = buffer1.find(file2_message) 
        if  buffer_idx != -1: 
            self.log.info("Console feedback for file2 got from buffer1 at index %d" % buffer_idx)
        else:
            status2, line2, buffer2 = self.deviceHandler.readUntilPattern ( pattern=file2_message, timeout=180)
            self.log.info("Console feedback for file2=%d" % status2)
            if status2 == False:
                self.failAndStopAllTests("Console feedback for file2 NOT arrived")    

        # As we sen't File2 without reply, its console feedback will arrive soon but we have to leave time 
        # to the message to arrive to the server through SMS
        #TODO: Improve getRequest function below so we can query and don't have to wait a fix time
        time.sleep(180)

        # Create path to file.
        file_location1 = filedata + self.device_id + '/' + file_name2
        self.log.info("Attempting to Retrieve data from File Location: %s" % (file_location1))

        # Check for existing file and its contents.
        try:
            file_content = self.cloudHandler.getRequest(file_location1).content
        except Exception, e:
            self.failAndStopAllTests('Failed to Get Device Contents.  Reason: %s' % e)

        self.log.info("File %s: %s" % (file_name2, file_content))
        
        # Verify file's contents
        if file_content != expected_content:
           self.failAndStopAllTests("Received content: %s does not match expected: %s" % (file_content, expected_content))

    def tearDown(self):
        self.log.info("Performing cleanup.")
        file_location = filedata + self.device_id + '/' + file_name1
        file_location1 = filedata + self.device_id + '/' + file_name2
        self.cloudHandler.deleteRequest(file_location)
        self.cloudHandler.deleteRequest(file_location1)

if __name__ == '__main__':
    unittest.main() 
