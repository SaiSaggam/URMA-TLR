import time
import cc_testcase
import datetime
import re
import os
import tempfile
import base64

class SendFileGetTestCase(cc_testcase.TestCase):

    def sendDeviceRequest(self, device_id, target_name, payload):
        my_target_device_request = \
            """            <sci_request version="1.0">
              <data_service>
                <targets>
                  <device id="%s"/>
                </targets>
                <requests>
                <device_request target_name="%s">%s</device_request>
                </requests>
              </data_service>
            </sci_request>""" % (device_id, target_name, payload)
        self.log.info("Sending SCI Request:\n%s\n" % my_target_device_request)
        response = self.cloudHandler.postRequest("sci", data = my_target_device_request)
        # Check query was successfully
        if (response.status_code != 200):
            self.fail("Unexpected response from the Cloud")
        # Show response in xml format
        self.log.info("Received SCI Response: \n%s" % response.content)

        # Get element 'device_request'
        element = response.resource["sci_reply"]["data_service"]["device"]["requests"]["device_request"]

        # Return xml response in dict format
        return element
    
    def test_file_get(self):

        """ Sends file get command.
        """
        data_length = 4096
        data = os.urandom(data_length)
        target_data = os.urandom(data_length)
        tmp_file = tempfile.NamedTemporaryFile(delete=False)
        target_file = tempfile.NamedTemporaryFile(delete=False)

        try:
            tmp_file.write(data)
            tmp_file.close()

            my_file_getput_request = \
                """<sci_request version="1.0">
                <file_system>
                    <targets>
                    <device id="%s"/>
                    </targets>
                    <commands>
                    <get_file path="%s" offset = "0" length = "4294967294"/>
                    <put_file path="%s" offset = "0" truncate = "true">
                    <data>%s</data>
                    </put_file>
                    </commands>
                </file_system>
                </sci_request>""" % (self.device_id, tmp_file.name, target_file.name, base64.encodestring(target_data))

            my_file_ls_request = \
                """<sci_request version="1.0">
                <file_system>
                    <targets>
                    <device id="%s"/>
                    </targets>
                    <commands>
                    <ls path="%s"/>
                    </commands>
                </file_system>
                </sci_request>""" % (self.device_id, '/tmp')

            self.log.info("Beginning Test for file get command.")
            self.log.info("Sending file get command for \"%s\" to server for device id  %s." % (tmp_file.name, self.device_id))
            self.log.info("Sending file put command for \"%s\" to server for device id  %s." % (target_file.name, self.device_id))

            self.log.info("Sending SCI Request:\n%s\n" % my_file_getput_request)
            response = self.cloudHandler.postRequest("sci", data = my_file_getput_request)
            # Check query was successfully
            if (response.status_code != 200):
                self.fail("Unexpected response from the Cloud")
            # Show response in xml format
            self.log.info("Received SCI Response: \n%s" % response.content)
            # Get element 'get_file'
            get_file = response.resource["sci_reply"]["file_system"]["device"]["commands"]["get_file"]

            recv_data = response.resource["sci_reply"]["file_system"]["device"]["commands"]["get_file"]["data"]
            recv_data =  base64.b64decode(recv_data)

            infile = open(tmp_file.name, "r")
            in_text = infile.read()
            infile.close()

            out_file = open(target_file.name, "r")
            out_text = out_file.read()
            out_file.close()

            self.log.info("!!! data len: \"%d\, in len: \"%d\", out len: \"%d\"," %  (len(data), len(in_text), len(out_text)))

            self.assertEqual(in_text, recv_data,
                "get file error")

            self.assertEqual(out_text, target_data,
                "put file error")

            self.log.info("Sending file ls command for \"%s\" to server for device id  %s." % ('/tmp', self.device_id))
            response = self.cloudHandler.postRequest("sci", data = my_file_ls_request)
            # Check query was successfully
            if (response.status_code != 200):
                self.fail("Unexpected response from the Cloud")
        finally:
            os.remove(tmp_file.name)
            os.remove(target_file.name)

if __name__ == '__main__':
    unittest.main()
