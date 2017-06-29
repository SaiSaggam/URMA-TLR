import unittest
import shutil
from ..build_utils import get_library_dirs, setup_platform, build, generate_rci_global_header
import sys
sys.path.append('./dvt/scripts')
import config

class BuildLibraryTestCase(unittest.TestCase):

    def setUp(self):
         (self.sandbox_dir, self.library_script_dir, self.library_test_dir) = get_library_dirs()


    def test_compile(self):
        (rc,output) = generate_rci_global_header(self.library_test_dir, self.config_tool_jar, self.keystore)

        if rc != 0:
    	    self.fail("RCI Config Global Header Generation Failed: \n%s" % output)

        print "Building [%s] with CFLAGS [%s]." % (self.src, self.cflags)

        (rc, output) = build(self.library_test_dir, self.cflags)
        if rc != 0:
            self.fail("%s:\n%s" % ("Did not cleanly compile", output))

    def tearDown(self):
        shutil.rmtree(self.sandbox_dir)
