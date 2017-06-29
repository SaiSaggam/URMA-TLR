import time
import cc_testcase
import os

class ConnectionTestCase(cc_testcase.TestCase):

    def test_memory_leak_in_firmware_cb(self):

        """ Check that no memory leak is detected when issuing connector_initiate_terminate from firmware download callback.
        """

        rootPath = os.path.abspath(os.path.dirname(__file__))
        filename = os.path.join(rootPath,'firmware.bin')

        self.log.info("Sending firmware target query to %s." % self.device_id)
        target = 0
        f = open(filename, 'rb')
        fileData = f.read()
        f.close()

        # Flush previous messages
        self.deviceHandler.readBuffer()
        
        self.log.info("Sending request to update firmware.")
        result,response = self.cloudHandler.sendUpdateFirmwareRequest(self.device_id, fileData, target=target)

        status, line1, buffer1 = self.deviceHandler.readUntilPattern ( pattern="SUCCESS: no memory leak detected", timeout=30)
        self.log.info("Console feedback for memory leak=%d" % status)
        if status == False:
            self.fail("Console feedback for memory leak NOT arrived")

