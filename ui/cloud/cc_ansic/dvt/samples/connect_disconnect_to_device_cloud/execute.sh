#!/bin/bash
../../bin/nosetests test_disconnect.py -v --ic-username=abuzarra --ic-password=abuzarra --with-ic --ic-hostname=test.idigi.com --ic-deviceid=00000000-00000000-00049DFF-FFAABBCC --with-xunit --xunit-file=test_restul.xml
