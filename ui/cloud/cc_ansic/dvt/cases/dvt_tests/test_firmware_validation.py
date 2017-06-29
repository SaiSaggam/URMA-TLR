# -*- coding: iso-8859-1 -*-
import os
import time
import ic_testcase
import xml.dom.minidom

from ..utils import update_firmware, DeviceConnectionMonitor

firmware_file = 'dvt/cases/test_files/pattern.txt'
target_file = 'pattern.txt'

class FirmwareValidTestCase(ic_testcase.TestCase):
    """ Performs a series of tests against several different firmware targets
    and verifies that the correct error response is returned.
    """

    monitor = None

    def setUp(self):
        ic_testcase.TestCase.setUp(self)
        # Optimization, reuse the DeviceConnectionMonitor to avoid creating
        # multiple sessions over and over.
        if FirmwareValidTestCase.monitor is None:
            FirmwareValidTestCase.monitor = DeviceConnectionMonitor(self.push_client, self.dev_id)
            FirmwareValidTestCase.monitor.start()

    @classmethod
    def tearDownClass(cls):
        if FirmwareValidTestCase.monitor is not None:
            FirmwareValidTestCase.monitor.stop()
        ic_testcase.TestCase.tearDownClass()

    def test_download_validation(self):
        self.log.info("Beginning Test - test_download_validation")
        # must be 12 to start writing a file
        target = 12

        # send request to update firmware
        self.log.info("Sending request to update firmware.")
        response = update_firmware(self.session, self.hostname, self.device_id,
            firmware_file, "%d" % target)

        # print response
        self.log.info("response: \n%s" % response)

        expected_file = open(firmware_file, "r")
        expected_content = expected_file.read()
        expected_file.close()

        read_file = open(target_file, "r")
        read_content = read_file.read()
        read_file.close()

        os.remove(target_file)

        self.log.info("Verifying the file data for '%s'." % (target_file) )
        self.assertEqual(expected_content, read_content)


    def test_download_validation_filename(self):
        self.log.info("Beginning Test - test_download_validation_filename")
        self.monitor.wait_for_connect(60)
        # must be 13 to start writing a file
        target = 13

        target_filename = "pattern_downloaded.txt"

        # send request to update firmware
        self.log.info("Sending request to update firmware.")
        response = update_firmware(self.session, self.hostname, self.device_id,
            firmware_file, "%d" % target, filename = target_filename)

        # print response
        self.log.info("response: \n%s" % response)

        expected_file = open(firmware_file, "r")
        expected_content = expected_file.read()
        expected_file.close()

        read_file = open(target_filename, "r")
        read_content = read_file.read()
        read_file.close()

        os.remove(target_filename)

        self.log.info("Verifying the file data for '%s'." % (target_filename) )
        self.assertEqual(expected_content, read_content)

if __name__ == '__main__':

    unittest.main()
