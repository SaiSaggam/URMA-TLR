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
# Cloud Connector test framework - Config class for ANSI C
#

#*************** Import Common libraries
import logging      ### Functions and classes to implement a event logging system


#*************** Import Classes path
libs = ["BaseFile_ansic"]

for elem in libs:
    exec("import %s" % elem)
    exec("reload (%s)" % elem)


##  Main Class for Config File
class ConfigFile_ansic(BaseFile_ansic.BaseFile_ansic):
    "public/run_step/platform/linux/config.c class to work with Cloud Connector Test Framework"

    ## Class Builder
    def __init__(self, filePath):
        ## Call to __init__ from Host(): Initialize function
        super(ConfigFile_ansic, self).__init__(filePath)

        #*************** Environment
        self.identifier = "config_c"


        #*************** Logging
        self.logger = logging.getLogger('cc.config_c')


    def removeErrorsFile(self):
        self.fileContent = self.fileContent.replace('#error', '//#error')


    def setupDeviceMAC(self,deviceMAC):
        old_mac = 'device_mac_addr[MAC_ADDR_LENGTH] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};'

        deviceMAC = deviceMAC.replace("-", "")
        deviceMAC = deviceMAC.replace(":", "")
        
        new_mac = 'device_mac_addr[MAC_ADDR_LENGTH] = {'
        new_mac += '0x' + deviceMAC[0] + deviceMAC[1] + ','
        new_mac += '0x' + deviceMAC[2] + deviceMAC[3] + ','
        new_mac += '0x' + deviceMAC[4] + deviceMAC[5] + ','
        new_mac += '0x' + deviceMAC[6] + deviceMAC[7] + ','
        new_mac += '0x' + deviceMAC[8] + deviceMAC[9] + ','
        new_mac += '0x' + deviceMAC[10] + deviceMAC[11]
        new_mac += '};'
        
        self.fileContent = self.fileContent.replace(old_mac, new_mac)


    def setupDeviceIDMethod(self,deviceIDMethod):
        old_device_id_method = 'connector_device_id_method_auto'
        if (deviceIDMethod == "auto"):
            new_device_id_method = 'connector_device_id_method_auto'
        else:
            new_device_id_method = 'connector_device_id_method_manual'
        self.fileContent = self.fileContent.replace(old_device_id_method, new_device_id_method)

    def setupDeviceIMEI(self,deviceIMEI):
        old_imei = 'device_imei[IMEI_LENGTH] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};'
        
        deviceIMEI = deviceIMEI.replace("-", "")
        deviceIMEI = deviceIMEI.replace(":", "")
        new_imei = 'device_imei[IMEI_LENGTH] = {'
        new_imei += '0x' + '0' + deviceIMEI[0] + ','
        new_imei += '0x' + deviceIMEI[1] + deviceIMEI[2] + ','
        new_imei += '0x' + deviceIMEI[3] + deviceIMEI[4] + ','
        new_imei += '0x' + deviceIMEI[5] + deviceIMEI[6] + ','
        new_imei += '0x' + deviceIMEI[7] + deviceIMEI[8] + ','
        new_imei += '0x' + deviceIMEI[9] + deviceIMEI[10] + ','
        new_imei += '0x' + deviceIMEI[11] + deviceIMEI[12] + ','
        new_imei += '0x' + deviceIMEI[13] + deviceIMEI[14]
        new_imei += '};'
        self.fileContent = self.fileContent.replace(old_imei, new_imei)


    def setupDeviceESN(self,deviceESN):
        old_esn = 'device_esn[ESN_LENGTH] = {0x00, 0x00, 0x00, 0x00};'
        
        deviceESN = deviceESN.replace("-", "")
        deviceESN = deviceESN.replace(":", "")
        new_esn_id = 'device_esn[ESN_LENGTH] = {'
        new_esn_id += '0x' + deviceESN[0] + deviceESN[1] + ','
        new_esn_id += '0x' + deviceESN[2] + deviceESN[3] + ','
        new_esn_id += '0x' + deviceESN[4] + deviceESN[5] + ','
        new_esn_id += '0x' + deviceESN[6] + deviceESN[7]
        new_esn_id += '};'
        self.fileContent = self.fileContent.replace(old_esn, new_esn_id)


    def setupDeviceMEID(self,deviceMEID):
        old_meid = 'device_meid[MEID_LENGTH] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};'
        
        deviceMEID = deviceMEID.replace("-", "")
        deviceMEID = deviceMEID.replace(":", "")
        new_meid = 'device_meid[MEID_LENGTH] = {'
        new_meid += '0x' + '00' + ','
        new_meid += '0x' + deviceMEID[0] + deviceMEID[1] + ','
        new_meid += '0x' + deviceMEID[2] + deviceMEID[3] + ','
        new_meid += '0x' + deviceMEID[4] + deviceMEID[5] + ','
        new_meid += '0x' + deviceMEID[6] + deviceMEID[7] + ','
        new_meid += '0x' + deviceMEID[8] + deviceMEID[9] + ','
        new_meid += '0x' + deviceMEID[10] + deviceMEID[11] + ','
        new_meid += '0x' + deviceMEID[12] + deviceMEID[13]
        new_meid += '};'
        self.fileContent = self.fileContent.replace(old_meid, new_meid)

    def setupConnectionType(self,connection_type):
        old_connection_type = 'connector_connection_type_lan'
        if (connection_type == "lan"):
            new_connection_type = 'connector_connection_type_lan'
        else:
            new_connection_type = 'connector_connection_type_wan'
        self.fileContent = self.fileContent.replace(old_connection_type, new_connection_type)
        
    def setupWanType(self,wan_type):
        old_wan_type = 'connector_wan_type_imei'
        if (wan_type == "imei"):
            new_wan_type = 'connector_wan_type_imei'
        elif (wan_type == "meid"):
            new_wan_type = 'connector_wan_type_meid'
        elif (wan_type == "esn"):
            new_wan_type = 'connector_wan_type_esn'
        else:
            new_wan_type = 'connector_wan_type_imei'
        self.fileContent = self.fileContent.replace(old_wan_type, new_wan_type)

    def setupVendorID(self,vendorid):
        old_vendor_id = '0x00000000'

        #hex_string = '%08X' % int(vendorid)
        #new_vendor_id  = '{%s}' % ', '.join([ '0x%s' % hex_string[x:x+2] for x in xrange(0,len(hex_string), 2)])

        self.fileContent = self.fileContent.replace(old_vendor_id, vendorid)



    def setupCloudServer(self,serverURL):
        old_server = 'login.etherios.com'

        self.fileContent = self.fileContent.replace(old_server, serverURL)


    def setupDeviceType(self,deviceType):
        old_device_type = '"Linux Application"'
        new_device_type = '"' + deviceType + '"'

        self.fileContent = self.fileContent.replace(old_device_type, new_device_type)


    def setupCloudPhoneNumber(self,cloudPhoneNumber):
        old_phone_number = '"447786201216";'
        new_phone_number = '"' + cloudPhoneNumber + '";'

        self.fileContent = self.fileContent.replace(old_phone_number, new_phone_number)


    def setupCloudServiceID(self,cloudServiceID):
        old_service_id = '"";  /* empty: No shared-code used */'
        if(cloudServiceID is not ""):
            new_service_id = '"' + cloudServiceID + '";'

            self.fileContent = self.fileContent.replace(old_service_id, new_service_id)
