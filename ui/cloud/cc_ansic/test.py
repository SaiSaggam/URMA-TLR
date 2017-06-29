# -*- coding: iso-8859-1 -*-
# ***************************************************************************
# Copyright (c) 2013 Digi International Inc.,
# All rights not expressly granted are reserved.
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
#
# ***************************************************************************
#
# Build the Connector test harness
#
import logging
import commands
import sys
import getopt
import signal
import subprocess
import imp
import tempfile
import os, time
import StringIO
import uuid
import tempfile
import shutil
from idigi_rest_api import rest_session, rest_mapper
import nose
sys.path.insert(0, './dvt/scripts')
sys.path.insert(0, './dvt/cases')
import config
import argparse
import gcov as gcov_config
from build_utils import get_template_dirs, setup_platform, build, sandbox
from stat import * # ST_SIZE etc
from threading import Thread
import random
import errno            # This module makes available standard errno system symbols

import makegen

# Force unbuffered output
unbuffered = os.fdopen(sys.stdout.fileno(), 'w', 0)
sys.stdout = unbuffered

FORMATTER = logging.Formatter('%(asctime)-15s %(name)-20s %(levelname)-8s %(message)s')
log = logging.getLogger('dvt_harness')
log.setLevel(logging.INFO)
console = logging.StreamHandler()
console.setLevel(logging.INFO)
console.setFormatter(FORMATTER)
log.addHandler(console)

BASE_SCRIPT_DIR= 'dvt/cases/'
SAMPLE_SCRIPT_DIR='dvt/cases/sample_tests/'
SAMPLE_PLATFORM_RUN_DIR = 'public/run/platforms/linux/'
SAMPLE_PLATFORM_STEP_DIR = 'public/step/platforms/linux/'

BUILD_TEST='dvt/cases/build_tests/test_build.py'

MEMORY_USAGE_FILE = './dvt/memory_usage.txt'
STACK_SIZE_FILE = './dvt/stacksize.html'

DEVICE_ID_BASED_ON_MAC_PROTOTYPE = '00000000-00000000-%sFF-FF%s'
DEVICE_ID_BASED_ON_IMEI_PROTOTYPE = '00010000-00000000-0%s-%s'
DEVICE_ID_BASED_ON_ESN_PROTOTYPE = '00020000-00000000-00000000-%s'
DEVICE_ID_BASED_ON_MEID_PROTOTYPE = '00040000-00000000-00%s-%s'
MAC_ADDR_PROTOTYPE = '%s:%s'
IMEI_NUMBER_PROTOTYPE = "%s-%s-%s-%s"
ESN_NUMBER_PROTOTYPE = "%s"
MEID_NUMBER_PROTOTYPE = "%s%s%s"

GCOV_FLAGS = " -g -pg -fprofile-arcs -ftest-coverage"
BASE_FLAGS = " -DRUNNIG_FROM_BASE "

class TestType(object):

    def __init__(self, name, src_dir, script_dir, tests):
        self.name = name
        self.src_dir = src_dir
        self.script_dir = script_dir
        self.tests = tests

sample_tests = {
    'compile_and_link'  : [],
    'connect_to_device_cloud'  : ('test_discovery.py',),
    'connect_on_ssl'    : ('test_ssl.py',),
    'firmware_download' : ('test_firmware.py',),
    'send_data'         : ('test_send_data.py',),
    'device_request'    : ('test_device_request.py',),
    'file_system'       : ('test_file_system.py',),
    'remote_config'     : ('test_binary_rci.py',),
    'data_point'        : ('test_data_point.py',),
}
run_sample  = TestType('run_sample', 'public/run/samples/',
                       'dvt/cases/sample_tests', sample_tests)
step_sample = TestType('step_sample', 'public/step/samples/',
                       'dvt/cases/sample_tests', sample_tests)

dvt_tests = {
    'full_test'                   : ('test_device_request.py',
                                     'test_put_request.py',
                                     'test_firmware_errors.py',
                                     'test_firmware_validation.py',
                                     'test_reboot.py',
                                     'test_disconnect.py',),
    'file_system_cov'             : ('test_fs_cov.py',),
    'file_system_dir_cov'         : ('test_fs_dir_cov.py',),
#    'file_system'                 : ('test_fs_positive.py',
#                                     'test_fs_negative.py'),
#    'reboot_test'                 : ('test_delay_reboot.py',
#                                     'test_disconnect.py'),
#    'terminate_test'              : ('test_ds_terminate.py',),
#    'response_to_bad_values_test' : ('test_debug_bad_values.py',),
    'compile_remote_config'       : (),
    'remote_config'               : ('test_brci_descriptors.py',
                                     'test_brci_string.py',),
    'connect_disconnect_to_device_cloud' : ('test_disconnect.py',),
#    'sm_udp'                      : ('test_sm_udp.py',),
#    'sm_udp_negative'             : ('test_sm_udp_negative.py',),
#    'sm_udp_multipart'            : ('test_sm_udp_multipart.py',),
#    'sm_send_data'                : ('test_sm_send_data_udp.py',),
#    'sm_device_request'           : ('test_sm_device_request_udp.py',),
#    'sm_connect'                  : ('test_sm_connect_udp.py',),
#    'sm_connect_imei_wan'         : ('test_sm_connect_udp.py',),
#    'stop_start_action'           : ('test_stop_start.py',
#                                     'test_abort_ds.py',
#                                     'test_abort_stop_cb.py',
#                                     'test_terminate_and_close_status.py',),
    'remote_config_error'         : ('test_brci_error.py',),
#    'imei_wan_type'               : ('test_imei_wan_type.py',),
#    'esn_wan_type'                : ('test_esn_wan_type.py',),
#    'meid_wan_type'               : ('test_meid_wan_type.py',),
#    'data_point_positive'         : ('test_data_point_positive.py',),
#    'data_point_negative'         : ('test_data_point_negative.py',),
}

dvt_test    = TestType('dvt_test', 'dvt/samples/', 'dvt/cases/dvt_tests',
                       dvt_tests)

keepalive_tests = {
    'keep_alive_test'             : ('test_keep_alive.py',),
}
keepalive_test = TestType('keepalive_test', 'dvt/samples/',
                          'dvt/cases/keep_alive', keepalive_tests)

admin_tests = {
    'full_test'                   : ('test_redirect.py',
                                     'test_nodebug_redirect.py',)
}
admin_test  = TestType('admin_test',  'dvt/samples',
                       'dvt/cases/admin_tests', admin_tests)

malloc_tests = {'malloc_test'     : ('test_malloc.py',)
}
malloc_test  = TestType('malloc_test',  'dvt/samples',
                       'dvt/cases/malloc_tests', malloc_tests)

timing_tests = {'ic_timing'     : ('test_ic_timing.py',)
}
timing_test  = TestType('ic_timing',  'dvt/samples',
                       'dvt/cases/ic_timing', timing_tests)

stacksize_tests = {'stack_size'     : ('test_stack_size.py',
                                       'test_stack_size_rci.py',
                                       'test_stack_size_ds.py',
                                       'test_stack_size_fw.py',
                                       'test_stack_size_fs.py',)
}
stacksize_test  = TestType('stack_size',  'dvt/samples',
                       'dvt/cases/stack_size', stacksize_tests)

# Dictionary mapping Test Type name to it's instance.
TESTS = dict((test.name,test) for test in [run_sample, step_sample,
                                           malloc_test,
                                           dvt_test, admin_test,
                                           keepalive_test, timing_test,
                                           stacksize_test])
SAMPLE_TESTS = dict((test.name,test) for test in [run_sample, step_sample])

DVT_TESTS = dict((test.name,test) for test in [malloc_test,
                                           dvt_test, keepalive_test,
                                           timing_test, stacksize_test])

ADMIN_TESTS = dict((test.name,test) for test in [admin_test])

def generate_id(rest_session, method="mac", mac_address=None, imei=None, meid=None, esn=None):
    """
    Generate a Pseudo Random Device Id (low probability of duplication) and
    provision device to account.
    """

    generated_id = {}

    for _ in xrange(0,5):
        # Make up to 5 attempts to provision a unique device.
        base_id = str.upper(str(uuid.uuid4())) ## ej: '9F3AD1F5-F0A5-4F8F-8F1A-BE8D14A8F107'

        if(method == "mac"):
            # DEVICE ID based on MAC ADDRESS
            #--- Example MAC: 112233:445566
            #--- Device ID mapping: 00000000-00000000-112233FF-FF445566
            if(mac_address is None):
                device_id = DEVICE_ID_BASED_ON_MAC_PROTOTYPE % (base_id[:6], base_id[-6:])
                mac_addr = MAC_ADDR_PROTOTYPE % (base_id[:6], base_id[-6:])
                generated_id["mac"] = mac_addr
            else:
                device_id = DEVICE_ID_BASED_ON_MAC_PROTOTYPE % (mac_address[:6], mac_address[-6:])
                generated_id["mac"] = MAC_ADDR_PROTOTYPE % (mac_address[:6], mac_address[-6:])
        elif(method == "imei"):
            # DEVICE ID based on IMEI
            #--- Example IMEI: AA-BBBBBB-CCCCCC-D
            #--- Device ID mapping: 00010000-00000000-0AABBBBB-BCCCCCCD
            #*** Config.c:
            #--- Example IMEI: AAAAAA-BB-CCCCCC-D"
            #--- Device ID mapping: 00010000-00000000-0AAAAAAB-BCCCCCCD
            # '00010000-00000000-0%s-%s'
            if (imei is None):
                block_A = random.randint(100000,999999)
                block_B = random.randint(10,99)
                block_C = random.randint(100000,999999)
                block_D = random.randint(0,9)
                imei_id = IMEI_NUMBER_PROTOTYPE % (block_A,
                                                block_B,
                                                block_C,
                                                block_D)
                device_id = DEVICE_ID_BASED_ON_IMEI_PROTOTYPE % (str(block_A)+str(block_B)[:1],
                                                                str(block_B)[1:]+str(block_C)+str(block_D))
                generated_id["imei"] = imei_id

            else:
                device_id = DEVICE_ID_BASED_ON_IMEI_PROTOTYPE % (str(imei)[:6] + str(imei)[7], str(imei)[8] + str(imei)[10:16] + str(imei)[-1:])
                generated_id["imei"] = IMEI_NUMBER_PROTOTYPE % (str(imei)[:6], str(imei)[7:9], str(imei)[10:16], str(imei)[-1:])

        elif(method == "esn"):
            # DEVICE ID based on ESN
            #--- Example ESN-Hex: MMSSSSSS
            #--- Device ID mapping: 00020000-00000000-00000000-MMSSSSSS
            if (esn is None):
                esn_id = base_id[:8]
                device_id = DEVICE_ID_BASED_ON_ESN_PROTOTYPE % (esn_id)
                generated_id["esn"] = esn_id
            else:
                device_id = DEVICE_ID_BASED_ON_ESN_PROTOTYPE % (esn)
                generated_id["esn"] = esn
        elif(method == "meid"):
            # DEVICE ID based on MEID
            #--- Example MEID-Hex: RRXXXXXXZZZZZZ
            #--- Device ID mapping: 00040000-00000000-00RRXXXX-XXZZZZZZ
            if (meid is None):
                block_R = base_id[:2]
                block_X = base_id[2:8]
                block_Z = base_id[-6:]
                meid_id = MEID_NUMBER_PROTOTYPE % (block_R,
                                                block_X,
                                                block_Z)
                device_id = DEVICE_ID_BASED_ON_MEID_PROTOTYPE % (block_R+block_X[:4],
                                                                block_X[4:]+block_Z)
                generated_id["meid"] = meid_id
            else:
                device_id = DEVICE_ID_BASED_ON_MEID_PROTOTYPE % (str(meid)[:6],str(meid)[-8:])
                generated_id["meid"] = MEID_NUMBER_PROTOTYPE % (str(meid)[:2], str(meid)[2:8], str(meid)[-6:])

        TestRunner.log.info("Generated Device ID and adding to Device Cloud Account.",
            extra={'description' : '', 'test': '', 'device_id' : device_id})
        device_core = rest_mapper.create_resource('DeviceCore',
            devConnectwareId=device_id, dpUdpSmEnabled='true')
        response = rest_session.post(device_core)
        if response.status_code == 201:
            # break on successful post.
            return (device_id, generated_id, response.headers['location'])
        else:
            TestRunner.log.info("Error Creating Device. %s" % response.content,
                extra={'description' : '', 'test' : '', 'device_id' : device_id})
            TestRunner.log.info("Trying to remove Device %s before continuing" % device_id, extra={'description' : '', 'test' : '', 'device_id' : device_id})
            rest_session.delete("DeviceCore?condition=devConnectwareId='%s'" % (device_id))
    # If here, we couldn't provision a device, raise Exception.
    raise Exception("Failed to Provision a Device.")

def start_iik(executable, tty=False):
    """
    Starts a Connector session in given path with given executable name.
    """
    if tty:
        os.system('/usr/bin/script -q -f -c "%s"' % (executable))
    else:
        os.system('%s 2>&1' % (executable))

def unique_device_type(testName = None):
    # Device type format:
    #   "ECC DVT test_case cb9e96ba3d5bbb"
    #   "ECC DVT test_case_bigger 4f9ef4d"
    #   "ECC DVT test_case_maxbigger 4f9e"

    # Common header for all test cases
    deviceType = "ECC DVT " # 8 characters

    # Additional header based on test name
    if ( testName != None ):
        maxTestNameLenght = 32 - len(deviceType) - 5 # at least we reserve 5 characters for random

        if( len(testName)<maxTestNameLenght ):
            deviceType = "%s%s " % (deviceType,testName)
        else:
            deviceType = "%s%s " % (deviceType,testName[:maxTestNameLenght])

    # Additional random header
    if ( len(deviceType) < 32 ):
        randomLenght = 32 - len(deviceType)
        deviceType = "%s%s" % (deviceType, str(uuid.uuid4().hex)[:randomLenght] )

    # Max lenght for Device type is 32 characters
    if( len(deviceType) > 32 ):
        deviceType = "%s" % (deviceType[:32])

    # return "Device type"
    return deviceType



class TestRunner(object):
    log = None

    def __init__(self, hostname, username, password, mac_address, imei, meid, esn, description, base_dir, configuration="default",
        debug_on=False, cflags='', replace_list=[], update_config_header=False,
        tty=False, gcov=False, test_type=None, test_name=None,
        config_tool_jar='ConfigGenerator.jar', keystore=None, build_only=False):

        self.hostname     = hostname
        self.username     = username
        self.password     = password
        self.rest_session = rest_session(hostname,
                                auth=(username, password), verify=False)

        if (test_name == 'connect_on_ssl') or (test_name is None):
            cflags += BASE_FLAGS

        self.mac_address          = mac_address
        self.imei                 = imei
        self.meid                 = meid
        self.esn                  = esn
        self.description          = description
        self.base_dir             = base_dir
        self.debug_on             = debug_on
        self.cflags               = cflags
        self.replace_list         = replace_list
        self.update_config_header = update_config_header
        self.configuration        = configuration

        self.tty  = tty
        self.gcov = gcov

        self.test_type = test_type
        self.test_name = test_name

        self.config_tool_jar = config_tool_jar
        self.keystore        = keystore
        self.build_only      = build_only

        response = self.rest_session.get_first('DeviceVendor')

        if response.status_code != 200:
            raise Exception(response.content)

        vendor_id = response.resource.dvVendorId

        # Defines key values device should be compiled with
        self.device_config = {
            'device_type'     : None,
            'device_mac'      : None,
            'device_imei'     : None,
            'device_esn'      : None,
            'device_meid'     : None,
            'connector_host'  : self.hostname,
            'tx_keepalive'    : 45,
            'rx_keepalive'    : 45,
            'wait_count'      : 3,
            'vendor_id'       : int(vendor_id),
            'max_transaction' : 1
        }

        if TestRunner.log is None:
            TestRunner.log = logging.getLogger('TestRunner')
            TestRunner.log.setLevel(logging.INFO)
            console = logging.StreamHandler()
            console.setLevel(logging.INFO)
            formatter = logging.Formatter('%(asctime)-15s %(description)-20s %(levelname)-8s %(device_id)s %(test)-10s %(message)s')
            console.setFormatter(formatter)
            TestRunner.log.addHandler(console)

        self.log = TestRunner.log
        self.log_extra = { 'description'    : self.description,
                           'test'           : '',
                           'device_id'      : '' }

    def run_tests(self):

        test_modes = ([('CONNECTOR_FILE_SYSTEM', 'CONNECTOR_NO_FILE_SYSTEM'),
                       ('CONNECTOR_FIRMWARE_SERVICE', 'CONNECTOR_NO_FIRMWARE_SERVICE')],
                      [('CONNECTOR_FILE_SYSTEM', 'CONNECTOR_NO_FILE_SYSTEM'),
                       ('CONNECTOR_FIRMWARE_SERVICE', 'CONNECTOR_NO_FIRMWARE_SERVICE'),
                       ('DS_MAX_USER 1', 'DS_MAX_USER 0')],
                      [('CONNECTOR_DATA_SERVICE', 'CONNECTOR_NO_DATA_SERVICE'),
                       ('CONNECTOR_FILE_SYSTEM', 'CONNECTOR_NO_FILE_SYSTEM')],
                      [('CONNECTOR_DATA_SERVICE', 'CONNECTOR_NO_DATA_SERVICE'),
                       ('CONNECTOR_FIRMWARE_SERVICE', 'CONNECTOR_NO_FIRMWARE_SERVICE')])

        tests = TESTS
        # If test_type is defined, filter tests executed to the test_type.
        if self.test_type is not None:
            # If 'sample' is provided as the test_type, include all samples.
            if self.test_type == 'sample':
                self.log.info("Executing sample tests.", extra=self.log_extra)
                tests = SAMPLE_TESTS
            elif self.test_type == 'dvt':
                self.log.info("Executing dvt tests.", extra=self.log_extra)
                tests = DVT_TESTS
            elif self.test_type == 'admin':
                self.log.info("Executing admin tests.", extra=self.log_extra)
                tests = ADMIN_TESTS
            elif self.test_type not in TESTS:
                self.log.error("test_type (%s) is not valid (%s)."\
                    % (self.test_type, TESTS.keys()), extra=self.log_extra)
                return
            else:
                self.log.info("Executing %s tests." % self.test_type, extra=self.log_extra)
                tests = { self.test_type : TESTS[self.test_type] }

        for test in tests:
                test_type = tests[test]
                test_list = test_type.tests

                # If test_name is specified, filter test list to the test_name.
                if self.test_name is not None:
                    if test_list.has_key(self.test_name):
                        test_list = { self.test_name : test_list[self.test_name] }
                    else:
                        self.log.warn("no tests found for %s with this test type (%s)."\
                            % (self.test_name, test_type.name), extra=self.log_extra)
                        continue

                for test_set in test_list:
                    sample = test_type.name.find('sample') != -1
                    more_modes = test_type.name.find('malloc') != -1

                    self.run_test(test_set, test_list[test_set], test,
                                    os.path.join(test_type.src_dir, test_set),
                                    test_type.script_dir, self.replace_list,
                                    sample=sample)
                    if more_modes:
                        for test_mode in test_modes:
                            new_replace_list = self.replace_list + test_mode
                            self.run_test(test_set, test_list[test_set], test,
                                            os.path.join(test_type.src_dir, test_set),
                                            test_type.script_dir, new_replace_list,
                                            sample=sample)

    def run_test(self, test, test_list, execution_type, base_src_dir,
        base_script_dir, replace_list=[], sample=False):
        device_config = self.device_config.copy()

        # Generate a device type based on test case
        device_config['device_type'] = unique_device_type(test)

        if test == 'keep_alive_test':
            # Temporary kludge, inject different keepalive/waitcount values
            # for this test only.  This will eventually be refactored.
            device_config['tx_keepalive'] = 10
            device_config['rx_keepalive'] = 10
            device_config['wait_count']   = 4

        log_extra = {}
        log_extra.update(self.log_extra)
        log_extra['execution_type'] = execution_type,
        log_extra['test']           = test

        device_location = None
        test_script = None
        try:
            pid = None
            sandbox_dir = sandbox(self.base_dir)
            src_dir        = os.path.join(sandbox_dir, base_src_dir)
            test_dir       = os.path.join(sandbox_dir, base_script_dir)
            connector_config   = os.path.join(src_dir, 'connector_config.h')

            for (s, r) in replace_list:
                config.replace_string(connector_config, s, r)

            # Setup method for generate Device ID
            if test in ['imei_wan_type','sm_connect_imei_wan']:
                method = "imei"
            elif test == 'esn_wan_type':
                method = "esn"
            elif test == 'meid_wan_type':
                method = "meid"
            else:
                method = "mac"

            (device_id, generated_id, device_location) = generate_id(self.rest_session, method, self.mac_address, self.imei, self.meid, self.esn)
            log_extra['device_id'] = device_id

            if(method=="mac"):
                device_config['device_mac'] = generated_id["mac"]
            elif(method=="imei"):
                device_config['device_imei'] = generated_id["imei"]
            elif(method=="esn"):
                device_config['device_esn'] = generated_id["esn"]
            elif(method=="meid"):
                device_config['device_meid'] = generated_id["meid"]

            # Use config.c in the local directory if it exists
            try:
                filename = os.path.join(src_dir, "config.c")
                f = open(filename, 'r')
                f.close()
                setup_platform(src_dir, **device_config)
            except IOError:
                setup_platform(os.path.join(sandbox_dir,
                    SAMPLE_PLATFORM_RUN_DIR), **device_config)
                setup_platform(os.path.join(sandbox_dir,
                    SAMPLE_PLATFORM_STEP_DIR), **device_config)

            config.update_platform_h(os.path.join(SAMPLE_PLATFORM_STEP_DIR, "platform.h"))
            config.update_platform_h(os.path.join(SAMPLE_PLATFORM_RUN_DIR, "platform.h"))

            if self.update_config_header:
                config.update_config_header(connector_config, **device_config)

            if self.gcov is True and test != 'compile_and_link':
                self.cflags += GCOV_FLAGS
                # Resolve the main.c file.  If it exists inthe src_dir assume
                # that is what is used, otherwise autoresolve to
                # ../../platforms/linux/main.c
                main = None
                local_main = os.path.join(src_dir, "main.c")
                if os.path.isfile(local_main):
                    main = local_main
                else:
                    platform_main = os.path.join(src_dir,
                        "../../platforms/linux/main.c")
                    if os.path.isfile(platform_main):
                        main = platform_main
                    else:
                        platform_main = os.path.join(src_dir,
                                "../../../public/run/platforms/linux/main.c")
                        if os.path.isfile(platform_main):
                            main = platform_main
                        else:
                            self.log.error("Could not resolve main.c from %s." % src_dir, extra=log_extra)
                            return
                # Instrument the main file with a __gcov_flush USR1 signal hook.
                gcov_config.instrument(main)
                # Add -lgcov to libraries.
                gcov_config.add_lib(os.path.join(src_dir, "Makefile"))

            self.log.info('Starting Test', extra=log_extra)

            build_args = ['dvt/bin/nosetests',
                          '--with-xunit',
                          '-s', # Don't Capture STDOUT
                          '--xunit-file=%s_%s_%s_build.xml' % (self.description, execution_type, test),
                          '--with-build',
                          '--build_cflags=%s' % self.cflags,
                          '--build_src=%s' % src_dir,
                          '--build_username=%s' % self.username,
                          '--build_password=%s' % self.password,
                          '--build_hostname=%s' % self.hostname,
                          '--build_config_tool_jar=%s' % self.config_tool_jar,
                          '--build_device_type=%s' % device_config['device_type']]

            if self.keystore is not None:
                build_args.append('--build_keystore=%s' % self.keystore)

            rc = nose.run(defaultTest=[BUILD_TEST], argv=build_args)

            # If False is returned, Fail this build
            if rc == False:
                raise Exception("Failed to Build from %s." % src_dir)

            if test == 'compile_and_link' or self.build_only == True:
                self.log.info('Finished Test.', extra=log_extra)
                return

            self.log.info('Starting Cloud Connector.', extra=log_extra)

            # Move executable to a unique file name to allow us to isolate
            # the pid.
            old_connector_path = os.path.join(src_dir, 'connector')
            connector_executable = str(uuid.uuid4())
            connector_path = os.path.join(src_dir, connector_executable)
            shutil.move(old_connector_path, connector_path)

            # Spawn in separate thread rather than shelling off in background.
            # Will allow capture of STDOUT/STDERR easier, save to separate file.
            iik_thread = Thread(group=None, target=start_iik, args=(connector_path,self.tty))
            iik_thread.start()

            # Sleep 5 seconds to allow device to do it's initialization (push data for example)
            time.sleep(5)
            #test_script = None
            try:
                pid = commands.getoutput('pidof -s %s' % connector_path)
                if pid == '':
                    raise Exception(">>> [%s] connector [%s] not running dir=[%s]" % (self.description, execution_type, src_dir))

                self.log.info('Started Cloud Connector.', extra=log_extra)
                for test_script in test_list:
                    # skip the test if script filename starts with 'test_nodebug'
                    if self.debug_on and test_script.find('test_nodebug') == 0:
                        self.log.info('Skipping since debug is on.', extra=log_extra)
                    else:
                        # skip the test if script filename starts with 'test_debug'
                        if (not self.debug_on and test_script.find('test_debug') == 0) or (self.update_config_header and test_script.find('test_debug') == 0):
                            self.log.info('Skipping since debug is off or update_config_header.', extra=log_extra)
                            filename1 = "%s.txt" % device_id
                            self.log.info('Deleting file %s.' % filename1, extra=log_extra)
                            os.unlink(filename1)
                        else:
                            self.log.info('Executing Test Script %s.' % test_script, extra=log_extra)

                            # Argument list to call nose with.  Generate a nosetest xml file in
                            # current directory, pass in device cloud / iik connection settings.
                            arguments = ['dvt/bin/nosetests',
                                         '--with-xunit',
                                         '-s', # Don't capture STDOUT (allow everything else to print)
                                         '--xunit-file=%s_%s_%s_%s.xml' % (self.description, execution_type, test, test_script),
                                         '--with-ic',
                                         '--ic-username=%s'  % self.username,
                                         '--ic-password=%s'  % self.password,
                                         '--ic-hostname=%s'  % self.hostname,
                                         '--ic-deviceid=%s' % device_id,
                                         '--ic-devicetype=%s' % device_config['device_type'],
                                         '--ic-vendorid=%s' % device_config['vendor_id'],
                                         '--ic-configuration=%s'  % self.configuration]

                            test_to_run = os.path.join(test_dir, test_script)
                            nose.run(defaultTest=[test_to_run], argv=arguments)
                            self.log.info('Finished Test Script %s.' % test_script, extra=log_extra)
            finally:
                # For now, don't Delete the Device after we're done with it, we do this before
                # killing the process because IDIGI-614 could cause a DB
                # deadlock.
                # self.log.info("Deleting Device %s." % device_location, extra=log_extra)
                # try:
                #     self.rest_session.delete(device_location)
                # except:
                #     pass # If delete fails, proceed anyways
                #device_location = None
                # Killing the process should also cause the thread to complete.
                if self.gcov and test != 'compile_and_link' and test_script is not None:
                    self.log.info('Flushing gcov coverage data for pid %s and exiting.' % pid, extra=log_extra)
                    os.kill(int(pid), signal.SIGUSR1)
                    cwd = os.getcwd()
                    coverageInfoPath = "lcov_coverage.info"
                    cmd = 'lcov --capture --directory %s --base-directory %s --output-file %s' % (sandbox_dir, src_dir, coverageInfoPath)
                    self.log.info("Executing lcov: %s" % cmd, extra=log_extra)
                    print "Executing %s" % (cmd)
                    os.system(cmd)
                    cmd = 'lcov --remove %s /usr\* --output-file "%s"' % (coverageInfoPath, coverageInfoPath)
                    print "Executing %s" % (cmd)
                    os.system(cmd)
                    cmd = 'genhtml %s --output-directory "%s/coverage"' % (coverageInfoPath, cwd)
                    print "Executing %s" % (cmd)
                    os.system(cmd)
                else:
                    if(pid is not None and len(pid)>0):
                        self.log.info('Killing Process with pid %s.' % pid, extra=log_extra)
                        os.kill(int(pid), signal.SIGKILL)
        except Exception, e:

            # Initialize vars
            message = "Execution script process has failed"
            if ( test_script is None ):
                test_script = "execution"
            resultFilename = "%s_%s_%s_%s.xml" % (self.description, execution_type, test, test_script)
            resultFilePath = os.path.abspath(os.path.join(".",resultFilename))
            result = False
            logRegisterProcess = "%s" % e

            # Generate the XML file
            return self.generateXMLTestResultContent(resultFilePath,execution_type,test,test_script,result,message,logRegisterProcess)

            log.exception(e)
        finally:
            # Remove Sandbox directory
            self.log.info("Removing Sandbox directory from '%s'." % (sandbox_dir), extra=log_extra)
            status = self.remove_sandbox(sandbox_dir)
            if(status[0]):
                self.log.info(status[1], extra=log_extra)
            else:
                self.log.error(status[1], extra=log_extra)

            # Delete device if it was not previously deleted.
            if device_location is not None and self.build_only == False:
                if(pid is not None and len(pid)>0):
                    # Check that process is finished
                    retries = 10
                    status = self.isRunningProcess(pid)
                    while(retries>0 and status[0] is True):
                        retries -= 1
                        self.log.info(status[1], extra=log_extra)
                        self.log.info("Waiting a second to finish the process...", extra=log_extra)
                        time.sleep(1)

                # Remove Device from server
                try:
                    self.log.info("Deleting Device %s." % device_location, extra=log_extra)
                    self.rest_session.delete(device_location)
                except Exception, e:
                    log.exception(e)

            if self.build_only == False:
                device_type = device_config['device_type']
                self.log.info("Deleting RCI Descriptors for %s." % device_type, extra=log_extra)
                try:
                    self.rest_session.delete('DeviceMetaData',
                        params={'condition': "dmDeviceType='%s'" % device_type})
                except Exception, e:
                    log.exception(e)
                self.log.info("Done.", extra=log_extra)


    def remove_sandbox(self, directory):
        # Remove temporal directory
        if(directory != None):
            if(directory != "/"):
                if os.path.exists(directory):
                    try:
                        #shutil.rmtree(directory)
                        _msg = "Sandbox was removed from '%s'" % (directory)
                        return (True, _msg)
                    except OSError,e:
                        _msg = "Exception [OSError]: %s" % (str(e))
                        return (False, _msg)
                else:
                    _msg = "Sandbox directory was not removed from '%s', because does not exists" % (directory)
                    return (True, _msg)
            else:
                _msg = "Can not delete the sandbox path from '%s'" % (directory)
                return (False, _msg)
        else:
            _msg = "Sandbox is None, aborts the removal process"
            return (False, _msg)


    def isRunningProcess(self, pid):
        try:
            os.kill(int(pid), 0)
            _msg = "Process with pid %s is still running" % pid
            return (True,_msg)
        except OSError, err:
            if err.errno == errno.ESRCH:
                _msg ="Process with pid %s is not running" % pid
                return (False,_msg)
            elif err.errno == errno.EPERM:
                _msg ="No permission to signal this process with pid %s !!" % pid
                return (True,_msg)
            else:
                _msg ="Unknown error sending the signal to process with pid %s !!" % pid
                return (True,_msg)


    def generateXMLTestResultContent(self,resultFilePath,testType,testGroup,testName,result,message,logProcess):
        # filePath: destination path to generate a xml file, ej. "/home/username/TestFolder/results/result_xxxxx/dvt_reboot/<filename>.xml"
        # testType: Type name, ej. 'dvt'
        # testGroup: Group name, ej. 'reboot'
        # testName: Test name, ej. 'build'
        # result: 'True' or 'False'
        # Message: Summary for this result, ej. 'Build Process Failed'
        # logProcess: all stack trace for the build process

        # Initialize vars
        numTests = 1
        if(result):
            numFailures = 0
        else:
            numFailures = 1


        # Escape characters that will have conflicts with XML
        # "   &quot;
        # '   &apos;
        # <   &lt;
        # >   &gt;
        # &   &amp;
        if(logProcess != None):
            #logProcess = logProcess.replace("<","&lt;")
            #logProcess = logProcess.replace(">","&gt;")
            #logProcess = "<![CDATA[%s]]>" % logProcess
            logProcess = logProcess.replace("<","")
            logProcess = logProcess.replace(">","")


        # File Content
        fileContent = """<?xml version="1.0" encoding="UTF-8"?>"""
        fileContent += """<testsuite name="nosetests" tests="%s" errors="0" failures="%s" skip="0">""" % (numTests,numFailures)

        if(result):
            # Test Case was successfully
            fileContent += """<testcase classname="%s_%s.%sTestCase" name="test_%s_%s_%s" time="1" />""" % (testType,testGroup,testName,testType,testGroup,testName)
        else:
            fileContent += """<testcase classname="%s_%s.%sTestCase" name="test_%s_%s_%s" time="1" >""" % (testType,testGroup,testName,testType,testGroup,testName)
            # Add a Failure line
            fileContent += """<failure type="exceptions.AssertionError" message="%s">%s</failure>""" % (message,logProcess)
            # End Test case
            fileContent += """</testcase>"""

        fileContent += """</testsuite>"""

        # Generate result file
        try:
            handlerFile = open(resultFilePath, "w")
            handlerFile.write(fileContent)
            handlerFile.close()
        except IOError,e:
            log.info("An error was occurred generating XML result file")
            _msg = "Exception [IOError]: %s" % (str(e))
            log.info(_msg)
            log.exception('Exception caught: %s' % type(e))

        return True


def clean_output(description, directory):
    for root, folders, files in os.walk(directory):
        for test_result in filter(lambda f: f.startswith(description) and f.endswith('.xml') or f.endswith('coverage.xml'), files):
            file_path = os.path.join(root, test_result)
            log.info("Removing %s." % file_path)
            os.remove(file_path)

def build_template(description, cflags):

    test_script = "test_build.py"
    test_dir = BASE_SCRIPT_DIR+'template_tests'

    log.info('[%s] Executing [%s].' % (description, test_script))

    # Argument list to call nose with.  Generate a nosetest xml file in
    # current directory, pass in connection settings.
    arguments = ['dvt/bin/nosetests',
                 '-s',
                 '--with-xunit',
                 '--xunit-file=%s_%s.xml' % (description, test_script),
                 '--with-build',
                 '--build_cflags=%s' % (cflags)]

    test_to_run = os.path.join(test_dir, test_script)
    nose.run(defaultTest=[test_to_run], argv=arguments)
    log.info('[%s] Finished [%s].' % (description, test_script))

def build_library(description, cflags, config_tool_jar):

    test_script = "test_build_lib.py"
    test_dir = BASE_SCRIPT_DIR+'template_tests'

    log.info('[%s] Executing [%s].' % (description, test_script))

    # Argument list to call nose with.  Generate a nosetest xml file in
    # current directory, pass in connection settings.
    arguments = ['dvt/bin/nosetests',
                 '-s',
                 '--with-xunit',
                 '--xunit-file=%s_%s.xml' % (description, test_script),
                 '--with-build',
                 '--build_config_tool_jar=%s' % config_tool_jar,
                 '--build_cflags=%s' % (cflags)]

    test_to_run = os.path.join(test_dir, test_script)
    nose.run(defaultTest=[test_to_run], argv=arguments)
    log.info('[%s] Finished [%s].' % (description, test_script))

def generate_config_tool_jar(build_path='tools/config'):
    log.info('Generating Config Tool')
    process = subprocess.Popen(['ant',
            '-f', '%s/build.xml' % build_path],
            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

    data = process.communicate()[0]

    if(process.returncode == 0):
        log.info("Config Tool Generation Successful.")

    return (process.returncode, data)

def main():
    parser = argparse.ArgumentParser(description="Cloud Connector Test Harness",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter)

    parser.add_argument('--username', action='store', type=str,
        default='iikdvt', help="Device Cloud Username to run the test with.")
    parser.add_argument('--password', action='store', type=str,
        default='iik1sfun', help="Device Cloud Password to run the test with.")
    parser.add_argument('--hostname', action='store', type=str,
        default='test.etherios.com',
        help='Device Cloud URL to connect devices to.')
    parser.add_argument('--mac_address', action='store', type=str,
        default=None,
        help='Device MAC to use in Device Cloud URL. ej: 112233:445566')
    parser.add_argument('--imei', action='store', type=str,
        default=None,
        help='Device IMEI to use for generating Device ID. ej: AAAAAA-BB-CCCCCC-D')
    parser.add_argument('--meid', action='store', type=str,
        default=None,
        help='Device MEID to use for generating Device ID. ej: RRXXXXXXZZZZZZ')
    parser.add_argument('--esn', action='store', type=str,
        default=None,
        help='Device ESN to use for generating Device ID. ej: MMSSSSSS')
    parser.add_argument('--descriptor', action='store', type=str,
        default='linux-x64', help='A unique descriptor to describe the test.')
    parser.add_argument('--architecture', action='store', type=str,
        default='x64', help='Architecture to run test with (x64 or i386).')
    parser.add_argument('--test_name', action='store', type=str, default=None,
        help='Optional test name to execute. Default executes all.')
    parser.add_argument('--test_type', action='store', type=str, default=None)
    parser.add_argument('--configuration', action='store', type=str,
        default='default', choices=['default', 'nodebug', 'compression',
                                    'debug', 'config_header', 'template',
                                    'all', 'library', 'no_malloc' ],
        help='Configuration to run against.  Default executes all.')
    parser.add_argument('--tty', action='store_true',dest='tty', default=False,
        help='Whether or not to run in a separate TTY session (only useful for Jenkins execution).')
    parser.add_argument('--gcov', action='store_true', dest='gcov', default=False,
        help='Whether or not to run gcov Code coverage.')
    parser.add_argument('--build_only', action='store_true', dest='build_only', default=False,
        help='Only build projects, do not run any application.')

    args = parser.parse_args()

    # Generate all makefiles ahead of running tests.
    makegen.make('all')

    config_tool_jar = os.path.abspath('tools/config/dist/ConfigGenerator.jar')
    log.info("Generating %s." % config_tool_jar)
    (rc, output) = generate_config_tool_jar()

    if rc != 0:
        log.error(output)
        log.error("ConfigGenerator.jar not created.  " \
            "This may cause tests to fail.")

    keystore = None
    # Use provided e2e.keystore if target host is idigi-e2e.sa.digi.com
    if args.hostname == "idigi-e2e.sa.digi.com":
        keystore = os.path.abspath('dvt/conf/e2e.keystore')

    # If 64-bit not passed in, assume 32-bit
    if args.architecture == 'x64':
        cflags='DFLAGS=-m64'
    else:
        cflags='DFLAGS=-m32'

    log.info("============ Cleaning Test Previous Output Files. ============")
    clean_output(args.descriptor, '.')

    # create empty memory usage file
    mem_usage_file = open(MEMORY_USAGE_FILE, 'w')
    mem_usage_file.close()

    # create empty stack usage file
    mem_usage_file = open(STACK_SIZE_FILE, 'w')
    mem_usage_file.close()

    configurations = []
    if args.configuration == 'all':
        configurations = ['default', 'nodebug', 'compression', 'debug',
                            'config_header', 'library', 'no_malloc']
    else:
        configurations = [args.configuration]

    for configuration in configurations:
        description = '%s_%s' % (args.descriptor, configuration)
        base_dir = '.'
        debug_on = True if configuration in ['default', 'debug', 'config_header'] else False
        update_config_header = False
        replace_list = []
        if configuration in ['nodebug', 'compression']:
            replace_list.append(('CONNECTOR_DEBUG', 'CONNECTOR_NO_DEBUG'))
        if configuration == 'compression':
            replace_list.append(('/* #define CONNECTOR_COMPRESSION */', '#define CONNECTOR_COMPRESSION'))
        if configuration == 'debug':
            replace_list.append(('/* #define CONNECTOR_DEBUG */', '#define CONNECTOR_DEBUG'))
        if configuration == 'config_header':
            update_config_header = True
        if configuration == 'no_malloc':
            replace_list.append(('/* #define CONNECTOR_NO_MALLOC */', '#define CONNECTOR_NO_MALLOC'))

        if configuration == 'template':
            log.info("============ Template platform =============")
            build_template(args.descriptor, cflags)
        elif configuration == 'library':
            log.info("============ Library platform =============")
            build_library(args.descriptor, cflags, config_tool_jar)
        else:
            log.info("============ %s =============" % configuration)
            runner = TestRunner(args.hostname, args.username, args.password,
                args.mac_address, args.imei, args.meid, args.esn, description, base_dir, configuration=configuration, debug_on=debug_on, cflags=cflags,
                replace_list=replace_list, tty=args.tty,
                test_name=args.test_name, test_type=args.test_type,
                config_tool_jar = config_tool_jar, keystore= keystore,
                gcov=args.gcov, update_config_header=update_config_header, build_only=args.build_only)
            runner.run_tests()

    log.info("All Tests Completed.")

if __name__ == '__main__':
    main()

