import time
import cc_testcase
import os
from base64 import encodestring

#import xml.dom.minidom
#from xml.dom.minidom import getDOMImplementation

FIRMWARE_QUERY_REQUEST = \
"""<sci_request version="1.0">
<query_firmware_targets>
    <targets>
    <device id="%s"/>
    </targets>
</query_firmware_targets>
</sci_request>"""

expected_firmware_version = '1.0.0.0'
firmware_targets = [('Binary Image' , '.*\.[bB][iI][nN]', '1.0.0.0'),
                    ('Library Image', '.*\.a'           , '0.0.1.0')]
#firmware_files   = ['dvt/cases/test_files/firmware.a',
                    #'dvt/cases/test_files/firmware.bin']
rootPath = os.path.abspath(os.path.dirname(__file__))
firmware_files   = [os.path.join(rootPath,'firmware.a'),
                    os.path.join(rootPath,'firmware.bin')]
class FirmwareTestCase(cc_testcase.TestCase):

    def getText(self, elem):
        rc = []
        for node in elem.childNodes:
            if node.nodeType == node.TEXT_NODE:
                rc.append(node.data)
        return str(''.join(rc))

    def test_check_firmware_version_match(self):

        """ Verifies that firmware version returned from device matches
        firmware version in configuration.
        """

        self.log.info("Beginning Test to Confirm Firmware Version.")
        # Retrieve Firmware Version
        self.log.info("Retrieving Firmware Version from %s." % self.device_id)
        result,firmware_level = self.cloudHandler.getDeviceFirmwareLevel(self.device_id)
        if result :
            # Ensure firmware version is correct
            self.assertEqual(firmware_level,
                            expected_firmware_version,
                            "Firmware Version (%s) does not match expected (%s)" %
                            (firmware_level,
                            expected_firmware_version))
        else:
            self.fail("Could not get Firmware Level")

    def test_confirm_firmware_targets(self):

        """ Verifies that the firmware targets returned match the firmware
        targets in the configuration.
        """

        self.log.info("Beginning Test to Validate Firmware Targets.")
        self.log.info("Sending firmware target query to %s." % self.device_id)

        # Send firmware target query request
        response = self.cloudHandler.postRequest("sci", data = FIRMWARE_QUERY_REQUEST % self.device_id)
        if (response.status_code != 200):
            self.fail("Unexpected response from the Cloud")
        self.log.info("Got Response:\n%s" % response.content)

        firmware_targets_xml = response.resource
        # Parse request response for firmware targets
        targets = firmware_targets_xml["sci_reply"]["query_firmware_targets"]["device"]["targets"]["target"]

        self.log.info("Determining if the number of targets matches targets in configuration.")
        self.assertEqual(len(targets), len(firmware_targets),
            "number of returned firmware targets (%i) does not match number of targets given (%i)"
            % (len(targets), len(firmware_targets)))

        for i in range(len(targets)):
            print "target[%i] = %s" % (i, targets[0])
            target_number = int(targets[i]["@number"])
            target = targets[i]
            parse_firmware_target = (target["name"], target["pattern"], target["version"])

            self.log.info("firmware target %i name: %s" % (target_number, parse_firmware_target[0]))
            self.log.info("firmware target %i pattern: %s" % (target_number, parse_firmware_target[1]))
            self.log.info("firmware target %i version: %s" % (target_number, parse_firmware_target[2]))
            self.log.info("Determining if firmware target %s matches given target" % target_number)
            self.assertEqual(parse_firmware_target, firmware_targets[target_number],
                            "Firmware Target %s does not match given target %s" %
                            (parse_firmware_target, firmware_targets[target_number]))


    def test_update_firmware_bytarget_multitargets(self):

        """ Updates firmware by target and verifies that the firmware
        is submitted and that the device disconnects and reconnects
        """

        target = 0
        self.log.info("Beginning Test to Update Firmware by Target.")

        self.log.info("Sending firmware target query to %s." % self.device_id)
        f = open(firmware_files[target], 'rb')
        fileData = f.read()
        f.close()
        # Send request to update firmware
        self.log.info("Sending request to update firmware.")
        result,response = self.cloudHandler.sendUpdateFirmwareRequest(self.device_id, fileData, target=target)
        if ( not result ):
            self.fail("Response to update_firmware was not sucessfully")


        # Show response in xml format
        self.log.info("Got Response:\n%s" % response.content )


        result, indexPattern, dataBuffer = self.deviceHandler.readUntilPattern ( pattern="app_firmware_download_complete: target    = %d" % target, timeout=90)
        self.log.info("Console feedback for update_firmware completed=%d" % result)
        if ( not result ):
            self.fail("Console feedback for update_firmware completed NOT arrived")

        if (self.cloudHandler.isDeviceConnected(self.device_id)):
            self.log.info("Waiting for Cloud to disconnect device.")
            self.deviceMonitor.waitForDisconnect(30)
        self.log.info("Device disconnected.")

    def test_update_firmware_nonexisting_target(self):

        """ Sends a firmware upgrade to a non-existing target and verifies
        that an error message is returned.
        """

        target = 0

        # Send firmware target query request
        response = self.cloudHandler.postRequest("sci", data = FIRMWARE_QUERY_REQUEST % self.device_id)
        if (response.status_code != 200):
            self.fail("Unexpected response from the Cloud")
        
        self.log.info("Got Response:\n%s" % response.content)
        
        firmware_targets_xml = response.resource

        # Determine number of firmware targets
        self.log.info("Determining number of firmware targets.")

        targets = firmware_targets_xml["sci_reply"]["query_firmware_targets"]["device"]["targets"]["target"]

        # Choose non-existing target for upgrade
        upgrade_target = len(targets)

        f = open(firmware_files[target], 'rb')
        fileData = f.read()
        f.close()
        
        result,response = self.cloudHandler.sendUpdateFirmwareRequest(self.device_id, fileData, target=upgrade_target)
        if ( result ):
            self.assertEqual(-1, submitted,
                "Firmware was successfully submitted to non-existing target.")
            
if __name__ == '__main__':

    unittest.main()
