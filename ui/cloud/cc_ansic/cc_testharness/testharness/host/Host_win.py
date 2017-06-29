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
import socket       ### Provides access to the BSD socket interface

################ Import Classes path
libs = ["Host"]

for elem in libs:
    exec("import %s" % elem)
    exec("reload (%s)" % elem)

##  Main Class for Host
#############################################################

class Host_win(Host.Host):
    "Host Windows class to work with Cloud Connector Test Framework"


    ## Class Builder
    ########################################################
    def __init__(self):
        ## Call to __init__ from Host(): Initialize function
        super(Host_win, self).__init__()

        ################ Environment
        self.identifier = "Host_win"

        ################ Logging
        self.logger = logging.getLogger('cc.host_win')





    def getIP(self, interface=None):
        if (interface is None):
            interface = self.defaultNetworkInterface

        updatedIP = socket.gethostbyname(socket.gethostname())

        return updatedIP
