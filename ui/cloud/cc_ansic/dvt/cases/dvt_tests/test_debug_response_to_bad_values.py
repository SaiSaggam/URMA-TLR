import os
import time
import ic_testcase

# Expected errors encountered.
test_table = [
              "connector_config_vendor_id,connector_invalid_data_size,terminated\n",
              "connector_config_device_type,connector_invalid_data_size,terminated\n",
              "connector_config_device_id,connector_invalid_data_size,terminated\n",
              "connector_config_server_url,connector_invalid_data_size,terminated\n",
              "connector_config_rx_keepalive,connector_invalid_data_size,terminated\n",
              "connector_config_ip_addr,connector_invalid_data_size,terminated\n",  
]

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

        if len(lines) < len(test_table):
            self.fail("Not as many lines (%d) as expected (%d) in %s.  File Content: %s" \
                % (len(lines), len(test_table), filename, lines)) 

        for line in lines:
        
            is_found = False
            for test in test_table:
                if line == test:
                    self.log.info("Successfully compared %s" % test)
                    is_found = True
            
            if is_found == False:
                self.log.info("Error %s not in the test_table %s" % line)
                errors += 1

        os.unlink(filename)
    
        self.log.info("***** Ending Response to Bad Values Test *****")

        if errors != 0:
            self.fail("Failed %d" % errors)
        
if __name__ == '__main__':
    unittest.main()
