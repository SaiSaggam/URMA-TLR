# -*- coding: utf-8 -*-
from nose.plugins import Plugin
import os
import idigi_rest_api
import idigi_monitor_api
import logging
import requests
from nose.tools import *

log = logging.getLogger('ic_plugin')
log.setLevel(logging.INFO)

if len(log.handlers) == 0:
    handler = logging.StreamHandler()
    handler.setLevel(logging.INFO)
    formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
    handler.setFormatter(formatter)
    log.addHandler(handler)

class ICPlugin(Plugin):
    name = 'ic'
    rest_session = None
    session = None
    push_client = None
    hostname = None

    def options(self, parser, env=os.environ):
        Plugin.options(self, parser, env=env)
        parser.add_option('--ic-username', action='store', type="string", dest="username", default="iikdvt", help="Username device is provisioned to.")
        parser.add_option('--ic-password', action='store', type="string", dest="password", default="iik1sfun", help="Password of username device is provisioned to.")
        parser.add_option('--ic-hostname', action='store', type="string", dest="hostname", default="test.etherios.com", help="Server device is connected to.")
        parser.add_option('--ic-deviceid', action='store', type="string", dest="device_id", default="00000000-00000000-00409DFF-FF432317", help="Device ID of device running iDigi Connector.")
        parser.add_option('--ic-vendorid', action='store', type="string", dest="vendor_id", default="16777242", help="Vendor ID of device running iDigi Connector.")
        parser.add_option('--ic-devicetype', action='store', type="string", dest="device_type", default="ECC DVT", help="Device Type of device runnning iDigi Connector.")
        parser.add_option('--ic-ipaddr', action='store', type="string", dest="ipaddr", default="0.0.0.0", help="IP address of device under test.")
        parser.add_option('--ic-configuration', action='store', type="string", dest="configuration", default="default", help="Cloud Connector configuration mode to run")
    def configure(self, options, conf):
        Plugin.configure(self, options, conf)

        if not self.enabled:
            return

        ICPlugin.hostname      = options.hostname
        ICPlugin.username      = options.username
        ICPlugin.password      = options.password
        ICPlugin.rest_session  = idigi_rest_api.rest_session(options.hostname,
            auth=(options.username, options.password), verify=False,
            timeout=120)
        ICPlugin.session       = requests.session(
                                    auth=(options.username, options.password),
                                    verify=False, timeout=120)
        ICPlugin.admin_session = requests.session(
                                    auth=('testbed', 'Sa!test11'),
                                    verify=False)
        ICPlugin.push_client   = idigi_monitor_api.push_client(options.username,
                                    options.password, hostname=options.hostname, 
                                    secure=False)
        ICPlugin.device_id     = options.device_id
        ICPlugin.device_type   = options.device_type
        ICPlugin.vendor_id     = options.vendor_id
        ICPlugin.ipaddr        = options.ipaddr
        ICPlugin.configuration = options.configuration

    def finalize(self, result):
        pass

    def prepareTestCase(self, test):
        test.test.rest_session  = ICPlugin.rest_session
        test.test.session       = ICPlugin.session
        test.test.admin_session = ICPlugin.admin_session
        test.test.push_client   = ICPlugin.push_client
        test.test.hostname      = ICPlugin.hostname
        test.test.device_id     = ICPlugin.device_id
        test.test.device_type   = ICPlugin.device_type
        test.test.vendor_id     = ICPlugin.vendor_id
        test.test.ipaddr        = ICPlugin.ipaddr
        test.test.configuration = ICPlugin.configuration
