# -*- coding: utf-8 -*-
import os
import time
import ic_testcase

# Expected errors encountered.
test_table = [
            "connector_request_id_config_device_id_method,connector_invalid_data_range,connector_init failed\n",

            "connector_request_id_config_device_id,connector_invalid_data,connector_init failed\n",

            "connector_request_id_config_device_cloud_url,connector_invalid_data,connector_init failed\n",
            "connector_request_id_config_device_cloud_url,connector_invalid_data_size,connector_init failed\n",

            "connector_request_id_config_connection_type,connector_invalid_data_range,connector_init failed\n",

            "connector_request_id_config_mac_addr,connector_invalid_data,network_tcp_close connector_close_status_device_error\n",

            "connector_request_id_config_vendor_id,connector_invalid_data_range,connector_init failed\n",

            "connector_request_id_config_device_type,connector_invalid_data,connector_init failed\n",
            "connector_request_id_config_device_type,connector_invalid_data_size,connector_init failed\n",

            "connector_request_id_config_rx_keepalive,connector_invalid_data_range,connector_init failed\n",

            "connector_request_id_config_tx_keepalive,connector_invalid_data_range,connector_init failed\n",

            "connector_request_id_config_wait_count,connector_invalid_data_range,connector_init failed\n",

            "connector_request_id_config_network_tcp,connector_invalid_data_range,connector_init failed\n",

            "connector_request_id_config_identity_verification,connector_invalid_data_range,connector_init failed\n",

            "connector_request_id_config_ip_addr,connector_invalid_data_range,network_tcp_close connector_close_status_device_error\n",
            "connector_request_id_config_ip_addr,connector_invalid_data_size,network_tcp_close connector_close_status_device_error\n",

            "connector_request_id_config_firmware_facility,connector_invalid_data_range,connector_init failed\n",

            "connector_request_id_config_data_service,connector_invalid_data_range,connector_init failed\n",

            "connector_request_id_config_wan_type,connector_invalid_data_range,connector_init failed\n",

            "connector_request_id_config_imei_number,connector_invalid_data,connector_init failed\n",

            "connector_request_id_config_phone_number,connector_invalid_data,connector_init failed\n",
            "connector_request_id_config_phone_number,connector_invalid_data_size,connector_init failed\n",

            "connector_request_id_config_esn,connector_invalid_data,connector_init failed\n",

            "connector_request_id_config_meid,connector_invalid_data,connector_init failed\n",

            "connector_request_id_config_password,connector_invalid_data,connector_init failed\n"

]

connector_tcp_closed = "network_tcp_close connector_close_status_device_error\n"

class ResponseToBadValuesDvtTestCase(ic_testcase.TestCase):

    def test_debug_response_to_bad_values(self):

        self.log.info("***** Beginning Response to Bad Values Test *****")

        filename = "%s.txt" % self.device_id

        errors = 0

        # We should see the expected error reponses in filename
        try:
            f = open(filename, 'r')
            self.log.info("Successfully opened %s" % filename)
            lines = f.readlines()
            f.close()
        except IOError:
            self.log.info("Failure opening file %s" % filename)
            errors = errors + 1

        test_count = 0
        test_case_count = [0] * len(test_table)

        for line in lines:

            line_case = line.split(',', 3)
            is_found = False

            if len(line) > 0 and line != connector_tcp_closed:
                index = 0
                for test in test_table:
                    test_case = test.split(',', 3)
                    config_name = test_case[0]
                    config_error_status = test_case[1]
                    config_error_api = test_case[2]

                    if line_case[0] == config_name and line_case[1] == config_error_status and line_case[2] == config_error_api:
                        if test_case_count[index]!= 0:
                            self.log.info("Duplicated test: line %s" % line)
                        else:
                            test_case_count[index] = 1
                            test_count += 1
                        is_found =True
                        break
                    index += 1

                if is_found == False:
                    self.log.error("Not Found test on the test_table: line '%s'\n\n" % line)
                    errors += 1
                else:
                    self.log.info("Test found: line '%s'" % line)

        if test_count != len(test_table):
            self.log.info("Missing configuration not being tested (expected # of configuration cases in the test_table %d != configuration case in the file %d" \
                        % (len(test_table), test_count))

            # Show File content
            fileData = ""
            for eachLine in lines:
                fileData += eachLine
            self.log.info("Not as many lines (%d) as expected (%d) in %s.  File Content:\n%s\n" % (len(lines), len(test_table), filename, fileData))
            errors += 1

        if errors == 0:
            self.log.info("Successfully compared test_table")

        os.unlink(filename)

        self.log.info("***** Ending Response to Bad Values Test *****")

        if errors != 0:
            self.fail("Failed %d" % errors)

if __name__ == '__main__':
    unittest.main()
