import time
import ic_testcase
import sys

from utils import DeviceConnectionMonitor

class DataPointTestCase(ic_testcase.TestCase):
    monitor=None

    def setUp(self):
        ic_testcase.TestCase.setUp(self)
        # Optimization, reuse the DeviceConnectionMonitor to avoid creating
        # multiple sessions over and over.
        if DataPointTestCase.monitor is None:
            DataPointTestCase.monitor = DeviceConnectionMonitor(self.push_client, self.dev_id)
            DataPointTestCase.monitor.start()
        self.monitor = DataPointTestCase.monitor

    @classmethod
    def tearDownClass(cls):
        if DataPointTestCase.monitor is not None:
            DataPointTestCase.monitor.stop()
        ic_testcase.TestCase.tearDownClass()

    def test_data_point(self):
        """ Validate data point sample """
        stream_run = "cpu_usage_run"
        stream_step = "cpu_usage_step"
        stream_run_path = "DataPoint/%s/%s" % (self.device_id, stream_run)
        stream_step_path = "DataPoint/%s/%s" % (self.device_id, stream_step)

        self.log.info("Sleeping for 20 seconds to collect some samples...")
        time.sleep(20)

        dataPoint_response = self.rest_session.get_first(stream_run_path, params={'order' : 'desc'})
        if dataPoint_response.status_code == 200:
            self.log.info("Deleting %s..." % stream_run)
            response = self.rest_session.delete(stream_run_path)
            self.log.info("Delete response: %s" % response)
        else:
            # Since we won't know whether it is step or run is creating the file, we try both
            dataPoint_response = self.rest_session.get_first(stream_step_path, params={'order' : 'desc'})
            if dataPoint_response.status_code == 200:
                self.log.info("Deleting %s..." % stream_step)
                response = self.rest_session.delete(stream_step_path)
                self.log.info("Delete response: %s" % response)

        self.log.info("Data response code [%d]\n" % dataPoint_response.status_code)
        self.assertTrue(dataPoint_response.status_code == 200)
        data_point  = dataPoint_response.resource
        self.log.info("Found %s" % data_point)

if __name__ == '__main__':
    unittest.main()