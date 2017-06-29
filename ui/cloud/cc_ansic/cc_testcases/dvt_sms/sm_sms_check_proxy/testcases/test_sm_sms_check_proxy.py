import time
import cc_testcase

# Enviroment settings

class SmsCheckProxyTestCase(cc_testcase.SMSTestCase):

    def test_check_proxy(self):

        self.log.info("***** Beginning SM SMS Check Proxy Test *****")

        result,indexPattern,dataBuffer = self.deviceHandler.readUntilPattern ( pattern="app_network_sms_open: connected to", timeout=90)
        self.log.info("Console feedback for Check Proxy=%d" % result)
        if ( not result ):
            self.fail("Console feedback for Check Proxy didn't arrive.")

if __name__ == '__main__':
    unittest.main()
