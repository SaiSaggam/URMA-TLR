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
# Cloud Connector test framework - Config class for Java
#

#*************** Import Common libraries
import logging      ### Functions and classes to implement a event logging system


#*************** Import Classes path
libs = ["BaseFile_java"]

for elem in libs:
    exec("import %s" % elem)
    exec("reload (%s)" % elem)


##  Main Class for Config File
class ConfigFile_java(BaseFile_java.BaseFile_java):
    "connector_config.txt class to work with Cloud Connector Test Framework"

    ## Class Builder
    def __init__(self, filePath):
        ## Call to __init__ from Host(): Initialize function
        super(ConfigFile_java, self).__init__(filePath)

        #*************** Environment
        self.identifier = "config_txt"


        #*************** Logging
        self.logger = logging.getLogger('cc.config_txt')


    def setupDeviceIDType(self,deviceIDType):
        if(deviceIDType == "provision"):
            deviceIDType = "provisioning"

        old_device_id_method = 'device_id_type='
        new_device_id_method = 'device_id_type=%s' % deviceIDType

        self.replaceSentence(old_device_id_method, new_device_id_method)


    def setupDeviceID(self,deviceID):
        old_device_id = 'device_id='
        new_device_id = 'device_id=%s' % deviceID
        self.replaceSentence(old_device_id, new_device_id)



    def setupVendorID(self,vendorid):
        hex_string = '%08X' % int(vendorid)
        old_vendor_id = 'vendor_id='
        new_vendor_id = 'vendor_id=0x%s' % hex_string

        self.replaceSentence(old_vendor_id, new_vendor_id)



    def setupCloudServer(self,serverURL):
        old_server = 'url='
        new_server = 'url=%s' % serverURL

        self.replaceSentence(old_server, new_server)


    def setupDeviceType(self,deviceType):
        old_device_type = 'device_type='
        new_device_type = 'device_type=' + deviceType

        self.replaceSentence(old_device_type, new_device_type)


    def setupCloudPhoneNumber(self,cloudPhoneNumber):
        old_phone_number = '"447786201216";'
        new_phone_number = '"' + cloudPhoneNumber + '";'

        self.fileContent = self.fileContent.replace(old_phone_number, new_phone_number)


    def setupCloudServiceID(self,cloudServiceID):
        old_service_id = '"";  /* empty: No shared-code used */'
        if(cloudServiceID is not ""):
            new_service_id = '"' + cloudServiceID + '";'

            self.fileContent = self.fileContent.replace(old_service_id, new_service_id)
