import unittest
import shutil
from ..build_utils import get_template_dirs, setup_platform, build
import sys
sys.path.append('./dvt/scripts')
import config

class BuildTemplateTestCase(unittest.TestCase):

    def setUp(self):
        (self.sandbox_dir, self.template_script_dir, self.template_platform_dir, 
            self.template_test_dir, self.connector_config_path) = get_template_dirs()
        setup_platform(self.template_platform_dir)

    def test_build_default(self):
        (rc, output) = build(self.template_test_dir, self.cflags)
        if rc != 0:
            self.fail("%s:\n%s" % ("Did not cleanly compile", output))

    def test_build_default_and_file_system(self):
        config.replace_string(self.connector_config_path, 'CONNECTOR_NO_FILE_SYSTEM', 'CONNECTOR_FILE_SYSTEM')
        (rc, output) = build(self.template_test_dir, self.cflags)
        if rc != 0:
            self.fail("%s:\n%s" % ("Did not cleanly compile", output))

    def test_build_data_service_only(self):
        config.replace_string(self.connector_config_path, 'CONNECTOR_COMPRESSION', 'CONNECTOR_NO_COMPRESSION')
        config.replace_string(self.connector_config_path, 'CONNECTOR_FIRMWARE_SERVICE', 'CONNECTOR_NO_FIRMWARE_SERVICE')
        (rc, output) = build(self.template_test_dir, self.cflags)
        if rc != 0:
            self.fail("%s:\n%s" % ("Did not cleanly compile", output))

    def test_build_no_features(self):
        config.replace_string(self.connector_config_path, 'CONNECTOR_COMPRESSION', 'CONNECTOR_NO_COMPRESSION')
        config.replace_string(self.connector_config_path, 'CONNECTOR_FIRMWARE_SERVICE', 'CONNECTOR_NO_FIRMWARE_SERVICE')
        config.replace_string(self.connector_config_path, 'CONNECTOR_DATA_SERVICE', 'CONNECTOR_NO_DATA_SERVICE')
        (rc, output) = build(self.template_test_dir, self.cflags)
        if rc != 0:
            self.fail("%s:\n%s" % ("Did not cleanly compile", output))     

    def test_build_firmware_only(self):
        config.replace_string(self.connector_config_path, 'CONNECTOR_COMPRESSION', 'CONNECTOR_NO_COMPRESSION')
        config.replace_string(self.connector_config_path, 'CONNECTOR_DATA_SERVICE', 'CONNECTOR_NO_DATA_SERVICE')
        (rc, output) = build(self.template_test_dir, self.cflags)
        if rc != 0:
            self.fail("%s:\n%s" % ("Did not cleanly compile", output))

    def test_build_file_system_only(self):
        config.replace_string(self.connector_config_path, 'CONNECTOR_COMPRESSION', 'CONNECTOR_NO_COMPRESSION')
        config.replace_string(self.connector_config_path, 'CONNECTOR_FIRMWARE_SERVICE', 'CONNECTOR_NO_FIRMWARE_SERVICE')
        config.replace_string(self.connector_config_path, 'CONNECTOR_DATA_SERVICE', 'CONNECTOR_NO_DATA_SERVICE')
        config.replace_string(self.connector_config_path, 'CONNECTOR_NO_FILE_SYSTEM', 'CONNECTOR_FILE_SYSTEM')
        (rc, output) = build(self.template_test_dir, self.cflags)
        if rc != 0:
            self.fail("%s:\n%s" % ("Did not cleanly compile", output))

    def tearDown(self):
        shutil.rmtree(self.sandbox_dir)