from nose.tools import *
import xml.dom.minidom          # http://docs.python.org/2/library/xml.dom.html
import xml.sax.saxutils         # http://wiki.python.org/moin/EscapingXml
import time
import ic_testcase
import logging
import random
from ic_plugin import ICPlugin

log = logging.getLogger('ic_testcase')
log.setLevel(logging.INFO)
if len(log.handlers) == 0:
    handler = logging.StreamHandler()
    handler.setLevel(logging.INFO)
    formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
    handler.setFormatter(formatter)
    log.addHandler(handler)

logging.getLogger('requests').setLevel(logging.WARNING)


##################################################################
#Templates for XML to send.
##################################################################
RCI_BASE_TEMPLATE = """<sci_request version="1.0">
  <send_message cache="%s">
    <targets>
      <device id="%s"/>
    </targets>
    <rci_request version="1.1">
      %s
    </rci_request>
  </send_message>
</sci_request>"""

QUERY_DESCRIPTOR_SETTING = """<query_descriptor><query_setting/></query_descriptor>"""

QUERY_DESCRIPTOR_STATE = """<query_descriptor><query_state/></query_descriptor>"""

SET_SETTING = """<set_setting><%s index="%s"><%s>%s</%s></%s></set_setting>"""

QUERY_SETTING = """<query_setting %s><%s/></query_setting>"""

SET_STATE = """<set_state><%s index="%s"><%s>%s</%s></%s></set_state>"""

QUERY_STATE = """<query_state %s><%s/></query_state>"""



def send_rci(request):
    try:
        req_data = xml.dom.minidom.parseString(request).toxml()
    except Exception, e:
        error = "Invalid XML passed in: %s"%request
        assert 0==1, error
    log.info("Sending SCI Request: \n%s" % request)
    response = ICPlugin.session.post('https://%s/ws/sci' % ICPlugin.hostname,
                    data=request,
                    verify=False)

    assert_equal(200, response.status_code, "Non 200 Status Code: %d.  " \
        "Response: %s" % (response.status_code, response.content))
    try:
        res_data = xml.dom.minidom.parseString(response.content)
        log.info("Received SCI Response: \n%s" \
            % res_data.toprettyxml(indent=' '))
        return res_data
    except Exception, e:
        error = "Response was not valid XML: %s" % response.content
        assert 0==1, error






class Test_brci_string(object):

    def ensure_connected(self):
        log.info("Ensuring Device %s is connected." \
            % ICPlugin.device_id)

        response = ICPlugin.rest_session.get_first('DeviceCore',
                        params={'condition' : "devConnectwareId='%s'" \
                        % ICPlugin.device_id})

        assert_equal(200, response.status_code)
        self.device_core = response.resource

        # If not connected, fail the TestCase.
        if not self.device_core.dpConnectionStatus == '1':
            assert_equal('1', self.device_core.dpConnectionStatus,
                "Device %s not connected." % ICPlugin.device_id)

    def getSettingValueWithAttributes(self, command_attribute, groupName, settingName, settingType, cache = "false"):
        # Use a specific query for setting or state
        if ( settingType == "setting" ):
            getQuery = QUERY_SETTING
        else:
            getQuery = QUERY_STATE

        # Create RCI query, ej:
        # <sci_request version="1.0">
        #       <send_message cache="no">
        #           <targets>
        #               <device id="00000000-00000000-BD34D8FF-FFD8E610"/>
        #           </targets>
        #           <rci_request version="1.1">
        #               <query_setting source="defaults">
        #                   <device_info/>
        #               </query_setting>
        #           </rci_request>
        #       </send_message>
        # </sci_request>
        rciQuery = RCI_BASE_TEMPLATE % ( cache, # cache
                                         ICPlugin.device_id, # Device ID
                                         getQuery % (command_attribute, groupName) ) # RCI query


        # Send request to server and get the response
        # Response returned by server, ej:
        # <?xml version="1.0" ?>
        #   <sci_reply version="1.0">
        #     <send_message>
        #       <device id="00000000-00000000-BD34D8FF-FFD8E610">
        #         <rci_reply version="1.1">
        #           <query_setting>
        #             <device_info>
        #               <product>default</product>
        #               <model>default</model>
        #               <company>default</company>
        #               <desc>default</desc>
        #               <syspwd>default</syspwd>
        #             </device_info>
        #           </query_setting>
        #         </rci_reply>
        #       </device>
        #     </send_message>
        #   </sci_reply>
        xmlResponse = send_rci(rciQuery)



        # Get list of nodes for the setting
        nodeList = xmlResponse.getElementsByTagName(settingName) # return a NodeList object

        # Check if we only have a setting node
        if(nodeList.length != 1):
            log.error("Duplicated setting '%s', Response: %s" % (settingName, xmlResponse.toprettyxml(indent="  ")) )
            return (None,xmlResponse)



        # Get Node for the setting, ej:
        # node.toxml():
        #  <product>default</product>
        nodeElement = nodeList[0] # return a Node object

        #Check if we have a valid element node
        if(nodeElement.nodeType != xml.dom.minidom.Node.ELEMENT_NODE):
            log.error("Wrong Element Node for setting '%s'" % (settingName) )
            return (None,xmlResponse)



        # Check if the element is empty, ej "<product/>"
        # Then return empty value
        if( nodeElement.toxml() == "<%s/>" % settingName):
            return ("",xmlResponse)



        # Get Text Node for the setting
        nodeText = nodeElement.firstChild # return a Text Node object

        ## Check if we have a valid Text node
        if(nodeText.nodeType != xml.dom.minidom.Node.TEXT_NODE):
            log.error("Wrong Text Node for setting '%s'" % (settingName) )
            return (None,xmlResponse)


        # Return value for the setting
        return (nodeText.data,xmlResponse)

    def getSettingValue(self, groupName, settingName, settingType, cache = "false"):
        return (self.getSettingValueWithAttributes('', groupName, settingName, settingType, cache))

    def setSettingValue(self, groupName, settingName, settingType, newValue, cache = "false"):
        # Escape new setting value to allow all kind of characters
        newValue = xml.sax.saxutils.escape(newValue)


        # Use a specific query for setting or state
        if ( settingType == "setting" ):
            setQuery = SET_SETTING
        else:
            setQuery = SET_STATE


        # Create RCI query, ej:
        # <sci_request version="1.0">
        #       <send_message cache="no">
        #           <targets>
        #               <device id="00000000-00000000-BD34D8FF-FFD8E610"/>
        #           </targets>
        #           <rci_request version="1.1">
        #               <query_setting>
        #                   <device_info/>
        #               </query_setting>
        #           </rci_request>
        #       </send_message>
        # </sci_request>
        rciQuery = RCI_BASE_TEMPLATE % ( cache, # cache
                                         ICPlugin.device_id, # Device ID
                                         setQuery % (groupName, # Group
                                                        1, # Index
                                                        settingName, # setting
                                                        newValue, # new value to set
                                                        settingName, # setting
                                                        groupName, # Group
                                                        ) # RCI query
                                       )


        # Send request to server and get the response
        # Response returned by server, ej:
        # <?xml version="1.0" ?>
        #   <sci_reply version="1.0">
        #     <send_message>
        #       <device id="00000000-00000000-BD34D8FF-FFD8E610">
        #         <rci_reply version="1.1">
        #           <set_setting>
        #             <device_info>
        #               <product/>
        #             </device_info>
        #           </set_setting>
        #         </rci_reply>
        #       </device>
        #     </send_message>
        #   </sci_reply>
        xmlResponse = send_rci(rciQuery)



        # Get list of nodes for the setting
        nodeList = xmlResponse.getElementsByTagName(settingName) # return a NodeList object

        # Check if we only have a setting node
        if(nodeList.length != 1):
            if(nodeList.length >1):
                log.error("Duplicated setting '%s', Response: %s" % (settingName, xmlResponse.toprettyxml(indent="  ")) )
            elif(nodeList.length == 0):
                log.error("There is not a setting '%s', Response: %s" % (settingName, xmlResponse.toprettyxml(indent="  ")) )
            return (False,xmlResponse)



        # Get Node for the setting, ej:
        # node.toxml():
        #  <product>default</product>
        nodeElement = nodeList[0] # return a Node object

        #Check if we have a valid element node
        if(nodeElement.nodeType != xml.dom.minidom.Node.ELEMENT_NODE):
            log.error("Wrong Element Node for setting '%s'" % (settingName) )
            return (False,xmlResponse)


        # Get Text Node for the setting
        nodeText = nodeElement.firstChild # return a Text Node object

        ## Check if we have a valid Text node
        if(nodeText is not None):
            log.error("Wrong Text Node for setting '%s'" % (settingName) )
            return (False,xmlResponse)

        return (True,xmlResponse)



    def verifySettingValue(self, groupName, settingName, settingType, newValue, cache = "false"):
        # Initialize result
        result = False

        # Get original value
        originalValue,xmlResponse = self.getSettingValue(groupName, settingName, settingType, cache = cache)
        log.info("Original value is '%s'" % (originalValue))

        # Check if we expect changes
        if(originalValue == newValue):
            log.warning("Original value '%s' match with the new value '%s' to be setup " % (originalValue,newValue) )

        # Set new value
        setResult,xmlResponse = self.setSettingValue(groupName, settingName, settingType, newValue, cache = cache)
        if( setResult ):
            # Get new value
            modifiedValue,xmlResponse = self.getSettingValue(groupName, settingName, settingType, cache = cache)

            # Verify original and modified values
            if(newValue == modifiedValue):
                result = True
                log.info("New value '%s' match with obtained value '%s'" % (newValue,modifiedValue) )
            else:
                log.error("New value '%s' does not match with obtained value '%s'" % (newValue,modifiedValue) )

        return result



    def verifySettingError(self, groupName, settingName, settingType, newValue, cache = "false", errorID = None, errorDesc = None, errorHint = None):
        # Initialize result
        result = False

        # Get original value
        originalValue,xmlResponse = self.getSettingValue(groupName, settingName, settingType, cache = cache)
        log.info("Original value is '%s'" % (originalValue))

        # Set new value
        setResult,xmlSetResponse = self.setSettingValue(groupName, settingName, settingType, newValue, cache = cache)
        if( not setResult ):
            # Get saved value
            modifiedValue,xmlResponse = self.getSettingValue(groupName, settingName, settingType, cache = cache)

            # Verify original and modified are the same
            if(originalValue == modifiedValue):
                result = True
                log.info("Original value '%s' match with obtained value '%s'" % (originalValue,modifiedValue) )
            else:
                log.error("Original value '%s' does not match with obtained value '%s'" % (originalValue,modifiedValue) )
                return False

            # Verify error codes and descriptions
            # <sci_reply version="1.0">
            #  <send_message>
            #   <device id="00000000-00000000-2B5254FF-FFA18761">
            #    <error id="2021">
            #      <desc>Invalid BinaryRCI request submited</desc>
            #      <hint>Invalid set_setting/set_state unable to set field model which is has access of read_only</hint>
            #    </error>
            #   </device>
            #  </send_message>
            # </sci_reply>
            if ( self.checkError(xmlSetResponse, errorID = errorID, errorDesc = errorDesc, errorHint = errorHint) ):
                result = True
            else:
                return False
        else:
            log.error("Set process did not report any errors with the value '%s'" % (newValue) )

        return result



    def checkError(self, xmlResponse, errorID = None, errorDesc = None, errorHint = None):
        # Get list of nodes for the setting
        nodeListError = xmlResponse.getElementsByTagName("error") # return a NodeList object

        # Check if we only have a setting node
        if(nodeListError.length != 1):
            if(nodeListError.length >1):
                log.error("Duplicated setting '%s', Response: %s" % ("error", xmlResponse.toprettyxml(indent="  ")) )
            elif(nodeListError.length == 0):
                log.error("There is not a setting '%s', Response: %s" % ("error", xmlResponse.toprettyxml(indent="  ")) )
            return False

        # Get Node for the setting, ej:
        # node.toxml():
        #  <error id="2021">
        #    <desc>Invalid BinaryRCI request submited</desc>
        #    <hint>Invalid set_setting/set_state unable to set field model which is has access of read_only</hint>
        #  </error>
        nodeElementError = nodeListError[0] # return a Node object

        #Check if we have a valid element node
        if(nodeElementError.nodeType != xml.dom.minidom.Node.ELEMENT_NODE):
            log.error("Wrong Element Node for setting '%s'" % ("error") )
            return False


        # Verify if error identifier matches with the errorID as argument
        if ( errorID != None ):
            # Check error node has an id attribute
            if ( nodeElementError.hasAttribute("id") ):
                # Get error id
                returnedID = nodeElementError.getAttribute("id")
                # Check error ids are the same
                if(returnedID != errorID):
                    log.error("Expected error ID '%s' does not match with returned id '%s'" % (errorID,returnedID) )
                    return False
                else:
                    log.info("Expected error ID '%s' match with returned id '%s'" % (errorID,returnedID) )



        # Verify if error description matches with the description as argument
        if ( errorDesc != None ):
            # Get list of nodes for the setting "desc"
            settingName = "desc"
            nodeListDesc = nodeElementError.getElementsByTagName(settingName) # return a NodeList object

            # Check if we only have a setting node
            if(nodeListDesc.length != 1):
                if(nodeListDesc.length >1):
                    log.error("Duplicated setting '%s', Response: %s" % (settingName, nodeElementError.toprettyxml(indent="  ")) )
                elif(nodeListDesc.length == 0):
                    log.error("There is not a setting '%s', Response: %s" % (settingName, nodeElementError.toprettyxml(indent="  ")) )
                return False

            # Get Node for the setting, ej:
            # node.toxml():
            #    <desc>Invalid BinaryRCI request submited</desc>
            nodeElementDesc = nodeListDesc[0]

            #Check if we have a valid element node
            if(nodeElementDesc.nodeType != xml.dom.minidom.Node.ELEMENT_NODE):
                log.error("Wrong Element Node for setting '%s'" % (settingName) )
                return False

            # Get Text Node for the setting
            nodeTextDesc = nodeElementDesc.firstChild # return a Text Node object

            ## Check if we have a valid Text node
            if(nodeTextDesc.nodeType != xml.dom.minidom.Node.TEXT_NODE):
                log.error("Wrong Text Node for setting '%s'" % (settingName) )
                return False

            # Return value for the setting
            returnedDescription = nodeTextDesc.data

            # Check error descriptions are the same
            if(returnedDescription != errorDesc):
                log.error("Expected error description '%s' does not match with returned description '%s'" % (errorDesc,returnedDescription) )
                return False
            else:
                log.info("Expected error description '%s'  match with returned description '%s'" % (errorDesc,returnedDescription) )



        # Verify if error hint matches with the hint as argument
        if ( errorHint != None ):
            # Get list of nodes for the setting "desc"
            settingName = "hint"
            nodeListHint = nodeElementError.getElementsByTagName(settingName) # return a NodeList object

            # Check if we only have a setting node
            if(nodeListHint.length != 1):
                if(nodeListHint.length >1):
                    log.error("Duplicated setting '%s', Response: %s" % (settingName, nodeElementError.toprettyxml(indent="  ")) )
                elif(nodeListHint.length == 0):
                    log.error("There is not a setting '%s', Response: %s" % (settingName, nodeElementError.toprettyxml(indent="  ")) )
                return False

            # Get Node for the setting, ej:
            # node.toxml():
            #    <desc>Invalid BinaryRCI request submited</desc>
            nodeElementHint = nodeListHint[0]

            #Check if we have a valid element node
            if(nodeElementHint.nodeType != xml.dom.minidom.Node.ELEMENT_NODE):
                log.error("Wrong Element Node for setting '%s'" % (settingName) )
                return False

            # Get Text Node for the setting
            nodeTextHint = nodeElementHint.firstChild # return a Text Node object

            ## Check if we have a valid Text node
            if(nodeTextHint.nodeType != xml.dom.minidom.Node.TEXT_NODE):
                log.error("Wrong Text Node for setting '%s'" % (settingName) )
                return False

            # Return value for the setting
            returnedHint = nodeTextHint.data


            # Check error hints are the same
            if(returnedHint != errorHint):
                log.error("Expected error hint '%s' does not match with returned hint '%s'" % (errorHint,returnedHint) )
                return False
            else:
                log.info("Expected error hint '%s' match with returned hint '%s'" % (errorHint,returnedHint) )


        return True



    def getChunks(self, string, number):
        # split a string each "number" of characters
        list = [string[i:i+number] for i in range(0,len(string),number)]

        return list



    def getListValidCharactersString(self):
        listValidCharacters = []
        for index in range(32,126):
            # We use this range of valid characters
            # see: http://www.ascii.cl/htmlcodes.htm
            listValidCharacters.append(chr(index))

        # Return a string
        return "".join(listValidCharacters)



    #**********************#
    #***** TEST_CASES *****#
    #**********************#
    def test_valid_characters_device_info_product(self):
        ''' Verify if a string allows to use all valid characters '''

        # Verify that Device is connected
        self.ensure_connected()

        # Generate all valid characters in lise
        lines = self.getChunks( self.getListValidCharactersString(), # return a string, ej: !"#$%&\'()*+,-.
                                64) # max length for each line

        # Initialize arguments
        groupName = "device_info"
        settingName = "product"
        settingType = "setting"

        # Set and verify setting with each line
        for eachline in lines:
            assert_equal(   True, # Expected value
                            self.verifySettingValue( groupName,       # Group name
                                                     settingName,     # Setting name
                                                     settingType,     # Setting type
                                                     eachline,        # New value
                                                     cache = "false"  # Disable/Enable cache
                                                     ), # Returned value
                            "Verification for characters '%s' was unsuccessful." % eachline)

        # Verify that Device is connected
        self.ensure_connected()




    def test_increasing_string_size_device_info_description(self):
        ''' Increase a string until the limit of the reserved buffer '''

        # Verify that Device is connected
        self.ensure_connected()

        # Generate all valid characters in a line
        listCharacters = self.getListValidCharactersString()

        # Initialize arguments
        groupName = "device_info"
        settingName = "desc"
        settingType = "setting"

        # String for the description
        newDescription = "%s" % listCharacters * 14 # 1316 characters

        # Check string to maximum size 1871
        for index in range(0,555):

            log.info("\n\n")
            log.info("Set and Verify description with a string lenght of %s characters:\n'%s'\n\n" % (len(newDescription), newDescription) )

            # Set and verify setting with each line
            assert_equal(   True, # Expected value
                            self.verifySettingValue( groupName,       # Group name
                                                    settingName,      # Setting name
                                                    settingType,      # Setting type
                                                    newDescription,   # New value
                                                    cache = "false"   # Disable/Enable cache
                                                    ), # Returned value
                            "Verification of field description for characters '%s' was unsuccessful." % newDescription)

            # Generate a new character
            charIndex = random.randint(0,len(listCharacters)-1)

            # Add a new character
            newDescription = "%s%s" % (newDescription,listCharacters[charIndex])


        # Check string bigger than maximum size fails (1401) and the value is not modified
        # reverse string to verify with a different string
        newDescription = newDescription[::-1]

        #log.info("\n\n")
        #log.info("Set and Verify description with a string lenght of %s characters bigger than reserved buffer of 1400:\n'%s'\n\n" % (len(newDescription), newDescription) )

        #assert_equal(   True, # Expected value
                #self.verifySettingError(groupName,       # Group name
                                        #settingName,     # Setting name
                                        #settingType,     # Setting type
                                        #newDescription,      # New value
                                        #cache = "false",  # Disable/Enable cache
                                        #errorID = "2", # Expected Error ID
                                        #errorDesc = "Bad configuration", # Expected Error Description
                                        #errorHint = "Maximum content size exceeded" # Expected Error Hint
                                        #), # Returned value
                #"Verification of field description for characters '%s' was unsuccessful." % newDescription)

        # Verify that Device is connected
        self.ensure_connected()



    def test_read_only_debug_info_version(self):
        ''' Verify if a read_only field is immutable '''

        # Verify that Device is connected
        self.ensure_connected()

        # Initialize arguments
        newVersion = "0123456789"
        groupName = "debug_info"
        settingName = "version"
        settingType = "state"

        assert_equal(   True, # Expected value
                        self.verifySettingError(groupName,       # Group name
                                                settingName,     # Setting name
                                                settingType,     # Setting type
                                                newVersion,      # New value
                                                cache = "false",  # Disable/Enable cache
                                                errorID = "2021", # Expected Error ID
                                                errorDesc = "Invalid BinaryRCI request submited", # Expected Error Description
                                                errorHint = "Invalid set_setting/set_state unable to set field %s which is has access of read_only" % settingName  # Expected Error Hint
                                                ), # Returned value
                        "Verification for read_only type field '%s' was unsuccessful." % newVersion)

        # Verify that Device is connected
        self.ensure_connected()



    def test_xbigger_string_than_buffer_device_info_description(self):
        ''' Verify if setting a string bigger than reserved buffer, not throws memory errors '''

        # Verify that Device is connected
        self.ensure_connected()

        # Generate all valid characters in a line
        listCharacters = self.getListValidCharactersString()

        # Initialize arguments
        groupName = "device_info"
        settingName = "desc"
        settingType = "setting"

        # String for the description
        newDescription = "%s" % listCharacters * 30 # 2820 characters


        log.info("Set and Verify description with a string lenght of %s characters bigger than reserved buffer:\n'%s'\n\n" % (len(newDescription), newDescription) )

        #assert_equal(   True, # Expected value
                #self.verifySettingError(groupName,       # Group name
                                        #settingName,     # Setting name
                                        #settingType,     # Setting type
                                        #newDescription,      # New value
                                        #cache = "false",  # Disable/Enable cache
                                        #errorID = "2", # Expected Error ID
                                        #errorDesc = "Bad configuration", # Expected Error Description
                                        #errorHint = "Maximum content size exceeded" # Expected Error Hint
                                        #), # Returned value
                #"Verification of field description for characters '%s' was unsuccessful." % newDescription)



        # Set and verify setting with each line
        assert_equal(   True, # Expected value
                        self.verifySettingValue( groupName,       # Group name
                                                settingName,      # Setting name
                                                settingType,      # Setting type
                                                newDescription,   # New value
                                                cache = "false"   # Disable/Enable cache
                                                ), # Returned value
                        "Verification of field description for characters '%s' was unsuccessful." % newDescription)


        # Verify that Device is connected
        self.ensure_connected()

    # This test must execute the last. Keep the _x_ in the test name.
    def test_x_command_attributes(self):
        ''' Verify command attributes '''

        # Verify that Device is connected
        self.ensure_connected()

        # Initialize arguments
        groupName = "attibute_feedback"
        settingType = "setting"
        cache = "false"

        # Initialize result
        result = False

        # Get values
        attributeName = "source"
        settingName = attributeName
        Value,xmlResponse = self.getSettingValueWithAttributes('%s="stored"' % (attributeName), groupName, settingName, settingType, cache = cache)
        log.info("source value is '%s'" % (Value))
        # Compare returned value with 1 (rci_query_setting_attribute_source_stored)
        assert_equal("1", Value, "Verification attribute '%s' was unsuccessful." % attributeName)

        attributeName = "compare_to"
        settingName = attributeName
        Value,xmlResponse = self.getSettingValueWithAttributes('%s="defaults"' % (attributeName), groupName, settingName, settingType, cache = cache)
        log.info("source value is '%s'" % (Value))
        # Compare returned value with 3 (rci_query_setting_attribute_compare_to_defaults)
        assert_equal("3", Value, "Verification attribute '%s' was unsuccessful." % attributeName)

        # Verify that Device is connected
        self.ensure_connected()
