import time
import cc_testcase

filedata = 'FileData/~/'
file_name = 'test/sm_data.txt'
file_name1 = 'test/sm_data_noresp.txt'

class SmUdpSendDataTestCase(cc_testcase.UDPTestCase):

    def test_sm_udp_send_data_timeout_fail(self):

        """ This routine tests that a data push fails due to a very low timeout, checking that the
        status callback is called only once when an error occurs. See https://jira.digi.com/browse/IC4C-251"""

        self.log.info("Begin sm_send_data_timeout_fail test.")
        status, line, buffer1 = self.deviceHandler.readUntilPattern(pattern="Data Service status: ", timeout=30)
        if status == False:
            self.fail("Console feedback for timeout did not arrive")
        elif "connector_data_service_status_timeout" in buffer1[line]:
            self.log.info("Console feedback for Data Service Timeout is correct")
        else:
            self.fail("Console feedback for timeout did not arrive: %s" % (buffer1[line]))

        self.log.info("Waiting if another status callback arrives...")
        status, line, buffer1 = self.deviceHandler.readUntilPattern(pattern="Data Service status: ", timeout=20)
        if status == True:
            self.fail("Another status callback arrived: %s" % (buffer1[line]))

if __name__ == '__main__':
    unittest.main() 
