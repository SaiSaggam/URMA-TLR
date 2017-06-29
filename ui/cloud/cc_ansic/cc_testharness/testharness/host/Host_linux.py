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
# Cloud Connector test framework - Host class based on Linux PC
#

################ Import Common libraries
import logging      ### Functions and classes to implement a event logging system
import netifaces    ### Portable network interface information

################ Import Classes path
libs = ["Host"]

for elem in libs:
    exec("import %s" % elem)
    exec("reload (%s)" % elem)

##  Main Class for Host
#############################################################

class Host_linux(Host.Host):
    "Host Linux class to work with Cloud Connector Test Framework"


    ## Class Builder
    ########################################################
    def __init__(self):
        ## Call to __init__ from Host(): Initialize function
        super(Host_linux, self).__init__()

        ################ Environment
        self.identifier = "Host_linux"

        ################ Logging
        self.logger = logging.getLogger('cc.host_linux')





    def getIP(self, interface=None):
        if (interface is None):
            interface = self.defaultNetworkInterface

        updatedIP = None
        try:
            for ifaceName in netifaces.interfaces():
                # Get one interface
                ifaceInfo = netifaces.ifaddresses(ifaceName)
                if ( ifaceInfo[netifaces.AF_LINK][0].has_key("addr") ):
                    ifaceMAC = ifaceInfo[netifaces.AF_LINK][0]["addr"]

                if ( ifaceInfo[netifaces.AF_INET][0].has_key("addr") ):
                    ifaceIPv4 = ifaceInfo[netifaces.AF_INET][0]["addr"]
                else:
                    ifaceIPv4 = None

                if ( ifaceInfo[netifaces.AF_INET][0].has_key("netmask") ):
                    ifaceMaskv4 = ifaceInfo[netifaces.AF_INET][0]["netmask"]

                if ( ifaceInfo[netifaces.AF_INET6][0].has_key("addr") ):
                    ifaceIPv6 = ifaceInfo[netifaces.AF_INET6][0]["addr"]

                if ( ifaceInfo[netifaces.AF_INET6][0].has_key("netmask") ):
                    ifaceMaskv6 = ifaceInfo[netifaces.AF_INET6][0]["netmask"]

                # Get only for eth0
                if(ifaceName == interface):
                    updatedIP = ifaceIPv4
        except KeyError, e:
            self.logger.exception(e)

        return updatedIP
