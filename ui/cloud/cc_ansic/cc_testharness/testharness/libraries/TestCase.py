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
# Cloud Connector test framework - Test Case class
#

################ Import Common libraries
import logging      ### Functions and classes to implement a event logging system



##  Main Class for Test Case
#############################################################

class TestCase(object):
    "Test Case class to work with Cloud Connector Test Framework"

    ################ Environment
    # Identifier for the script
    identifier = None
    # Source code base
    sourcePath = None
    # Source code application
    sourceAppPath = None
    # Script with test cases
    scriptPath = None

    ################ Logging
    ## Logger class to save system logging
    logger = None





    ## Class Builder
    ########################################################
    def __init__(self):

        ################ Environment
        self.identifier = "TestCase"

        ################ Logging
        self.logger = logging.getLogger('cc.testcase')
