import time
import cc_testcase
import sys

class DataPointNegativeTestCase(cc_testcase.TestCase):
    def test_dp1_tcp_int(self):
        """ Validate dp_tcp_int stream """
        self.verify_dp_file("dpn_tcp_int")

    def test_dp7_udp_int(self):
        """ Validate dp_udp_int stream """
        self.verify_dp_file("dpn_udp_int")

    def test_dp2_tcp_long(self):
        """ Validate dp_tcp_long stream """
        self.verify_dp_file("dpn_tcp_long")

    def test_dp8_udp_long(self):
        """ Validate dp_udp_long stream """
        self.verify_dp_file("dpn_udp_long")

    def test_dp3_tcp_float(self):
        """ Validate dp_tcp_float stream """
        self.verify_dp_file("dpn_tcp_float")

    def test_dp9_udp_float(self):
        """ Validate dp_udp_float stream """
        self.verify_dp_file("dpn_udp_float")

    def test_dp4_tcp_double(self):
        """ Validate dp_tcp_double stream """
        self.verify_dp_file("dpn_tcp_double")

    def test_dpa_udp_double(self):
        """ Validate dp_udp_double stream """
        self.verify_dp_file("dpn_udp_double")

    def test_dp5_tcp_string(self):
        """ Validate dp_tcp_string stream """
        self.verify_dp_file("dpn_tcp_string")

    def test_dpb_udp_string(self):
        """ Validate dp_udp_string stream """
        self.verify_dp_file("dpn_udp_string")

    def test_dp6_tcp_binary(self):
        """ Validate dp_tcp_binary stream """
        self.verify_dp_file("dpn_tcp_binary")

    def test_dpc_udp_binary(self):
        """ Validate dp_udp_binary stream """
        self.verify_dp_file("dpn_udp_binary")

    def verify_dp_file (self, stream_name):
        if stream_name == 'dpn_udp_int' or stream_name == 'dpn_udp_float' or stream_name == 'dpn_udp_string' or stream_name == 'dpn_udp_binary':
            #This files should not be there... so no waste so much time
            retry_count = 5
        else:
            retry_count = 20
        point_path = "DataPoint/%s/%s" % (self.device_id, stream_name)

        self.log.info("Verifying %s..." % point_path)

        while (retry_count > 0):
            retry_count -= 1
            try:
                dataPoint_response = self.cloudHandler.getRequest(point_path, params={'order' : 'desc','size':'1'}, timeout=2000)
                if dataPoint_response.status_code == 200 or dataPoint_response.status_code == 500:
                    break
                else:
                    self.log.info("Data response code [%d] for %s, retrying\n" % (dataPoint_response.status_code, stream_name))
            except Exception, e:
                self.log.info("Received Exception for %s, retrying\n" % stream_name)
            time.sleep(1)

        if dataPoint_response.status_code == 200:
            data_point  = dataPoint_response.resource
            if stream_name.find('binary') == -1:
                self.log.info("Found %s, %s" % (stream_name, data_point))
                resultSize = data_point["result"]["resultSize"]
                self.log.info("resultSize: %s" % resultSize)
                self.assertTrue(resultSize == "1")
            else:
                self.log.info("Found %s, DataPoint: binary data" % stream_name)
                resultSize = data_point["result"]["resultSize"]
                self.log.info("resultSize: %s" % resultSize)
                self.assertTrue(resultSize == "1")

            stream_path = "DataStream/%s/%s" % (self.device_id, stream_name)
            self.log.info("Deleting %s..." % stream_path)
            response = self.cloudHandler.deleteRequest(stream_path)
            self.log.info("Delete response: %s" % response)

            if stream_name == 'dpn_udp_int' or stream_name == 'dpn_udp_float' or stream_name == 'dpn_udp_string' or stream_name == 'dpn_udp_binay':
                self.fail("%s shouldn't be there!!!!" % stream_name)
            else:
                self.log.info("Nice that %s is there" % stream_name)                


        elif dataPoint_response.status_code == 500:
            # For DataStreams with 0 Datapoints (the string example) I see quite often
            # the server returning 500 forever. In this case, query for the DataStream
            # instead of for the DataPoint.
            self.log.info("Data response code [%d] for %s, Trying with DataStream!!!!!!\n" % (dataPoint_response.status_code, stream_name))
            self.assertTrue(stream_name.find('string') > 0)
            stream_path = "DataStream/%s/%s" % (self.device_id, stream_name)

            self.log.info("Verifying %s..." % stream_path)
            dataStream_response = self.cloudHandler.getRequest(stream_path, params={'order' : 'desc','size':'1'}, timeout=2000)
            self.assertTrue(dataStream_response.status_code == 200)
            self.log.info("Deleting %s..." % stream_path)
            response = self.cloudHandler.deleteRequest(stream_path)
            self.log.info("Delete response: %s" % response)
        else:
            #Following streams are cancel in the tests, so it's ok they don't appear:
            if stream_name == 'dpn_udp_int' or stream_name == 'dpn_udp_float' or stream_name == 'dpn_udp_string' or stream_name == 'dpn_udp_binary':
                self.log.info("It's OK that %s is not there" % stream_name)
            else:
                self.fail("%s is not found!!!!" % stream_name)

if __name__ == '__main__':
    unittest.main()