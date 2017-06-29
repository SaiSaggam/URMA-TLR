import time
import cc_testcase
import sys

class DataPointPositiveTestCase(cc_testcase.TestCase):
    def test_dp0_wait_data_sent(self):
        """ Wait last requested reply is received.... Data sent without reply may still need some time to arrive """
        status, unused1, unused2 = self.deviceHandler.readUntilPattern ( pattern="Data point test complete", timeout=10)

    def test_dp1_tcp_int(self):
        """ Validate dp_tcp_int stream """
        self.verify_dp_file("dp_tcp_int")

    def test_dp1_udp_int(self):
        """ Validate dp_udp_int stream """
        self.verify_dp_file("dp_udp_int")

    def test_dp2_tcp_long(self):
        """ Validate dp_tcp_long stream """
        self.verify_dp_file("dp_tcp_long")

    def test_dp2_udp_long(self):
        """ Validate dp_udp_long stream """
        self.verify_dp_file("dp_udp_long")

    def test_dp3_tcp_float(self):
        """ Validate dp_tcp_float stream """
        self.verify_dp_file("dp_tcp_float")

    def test_dp3_udp_float(self):
        """ Validate dp_udp_float stream """
        self.verify_dp_file("dp_udp_float")

    def test_dp4_tcp_double(self):
        """ Validate dp_tcp_double stream """
        self.verify_dp_file("dp_tcp_double")

    def test_dp4_udp_double(self):
        """ Validate dp_udp_double stream """
        self.verify_dp_file("dp_udp_double")

    def test_dp5_tcp_string(self):
        """ Validate dp_tcp_string stream """
        self.verify_dp_file("dp_tcp_string")

    def test_dp5_udp_string(self):
        """ Validate dp_udp_string stream """
        self.verify_dp_file("dp_udp_string")

    def test_dp6_tcp_binary(self):
        """ Validate dp_tcp_binary stream """
        self.verify_dp_file("dp_tcp_binary")

    def test_dp6_udp_binary(self):
        """ Validate dp_udp_binary stream """
        self.verify_dp_file("dp_udp_binary")

    def verify_dp_file (self, stream_name):
        retry_count = 10
        stream_path = "DataPoint/%s/%s" % (self.device_id, stream_name)

        self.log.info("Verifying %s..." % stream_path)

        while (retry_count > 0):
            retry_count -= 1
            try:
                dataPoint_response = self.cloudHandler.getRequest(stream_path, params={'order' : 'desc','size':'1'}, timeout=2000)
                if dataPoint_response.status_code == 200:
                    break
                else:
                    self.log.info("Data response code [%d] for %s, retrying\n" % (dataPoint_response.status_code, stream_name))
            except Exception, e:
                self.log.info("Received Exception for %s, retrying\n" % stream_name)

            time.sleep(1)

        self.assertTrue(dataPoint_response.status_code == 200)
        data_point  = dataPoint_response.resource
        if stream_name.find('binary') == -1:
            self.log.info("Found %s, %s" % (stream_name, data_point))
        else:
            self.log.info("Found %s, DataPoint: binary data" % stream_name)

        resultSize = data_point["result"]["resultSize"]
        self.log.info("resultSize: %s" % resultSize)
        self.assertTrue(resultSize == "1")

        stream_path = "DataStream/%s/%s" % (self.device_id, stream_name)
        self.log.info("Deleting %s..." % stream_path)
        response = self.cloudHandler.deleteRequest(stream_path)
        self.log.info("Delete response: %s" % response)

if __name__ == '__main__':
    unittest.main()