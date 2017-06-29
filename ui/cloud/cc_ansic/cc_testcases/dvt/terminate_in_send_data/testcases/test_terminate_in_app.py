import time
import cc_testcase


class ConnectionTestCase(cc_testcase.TestCase):

    def test_memory_leak_in_send_data(self):

        """ Check that no memory leak is detected when issuing connector_initiate_terminate from send data callback.
        """

        # Flush previous messages
        self.deviceHandler.readBuffer()

        status, line1, buffer1 = self.deviceHandler.readUntilPattern ( pattern="SUCCESS: no memory leak detected", timeout=30)
        self.log.info("Console feedback for memory leak=%d" % status)
        if status == False:
            self.fail("Console feedback for memory leak NOT arrived")

