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
# S.A.R.E.S Framework - Connection handler class based on Serial Port

# Import to get compatibility with SARES
#import ExpandSysPaths
import sys

libs = ['ConnectionHandler']
libs += ['ExceptionLibrary']  # Custom class for raising exception catched in Library
#import SerialPort_linux as SerialPortClass

if( "linux" in sys.platform ):
    import SerialPort_linux as SerialPortClass
    #libs += ['SerialPort_linux']
else:
    import SerialPort_win as SerialPortClass
    #libs += ['SerialPort_win']

for elem in libs:
    exec("import %s" % elem)
    #exec("reload (%s)" % elem)


## Connection handler class based on Serial Port
class SerialHandler(ConnectionHandler.ConnectionHandler):
    "Main class to manage a serial port"
    ## System identifier for the console port
    connectionSerialPort = None
    ## Serial Port baudrate for the console port
    connectionBaudrate = None


    ## Class Builder
    def __init__(self, serialport=None):
        super(SerialHandler, self).__init__()
        self.setIdentifier("SerialHandler")
        ## Initialize serial port identifier ej. /dev/ttyS0
        self.connectionSerialPort = serialport
        ## Initialize serial port baudrate ej. 115200,38400,9600
        self.connectionBaudrate = 115200


    ## Set Baudrate for this serial connection
    # @param self reference for this object
    # @param baudrate int with the baudrate number
    def setConnectionBaudrate(self, baudrate):
        self.connectionBaudrate = baudrate


    ## Set all settings for this connection type
    # @param self reference for this object
    # @param settings dict with connection settings
    def setConnectionSettings(self, settings):
        super(SerialHandler, self).setConnectionSettings(settings)

        # Save network arguments
        if(isinstance(settings,dict)):
            # Check Connection settings
            if(settings.has_key("baudrate")):
                self.setConnectionBaudrate(settings["baudrate"])
        else:
            raise ExceptionLibrary.Error('setConnectionSettings requires a dict')


    ## This function parses the output readed from descriptor,
    #   reformat it send it to the file logging.
    #   @param self reference for this object
    #   @param data_buffer string containing all data from the buffer
    def _parse_output_to_fileLogging(self, data_buffer):
        # Remove all \n\r and \r\n
        lines = data_buffer.replace('\r\n', '\n')
        lines = lines.replace('\n\r', '\n')
        self.logger.info(lines)


    ## (private) Creates(__init__) the Command Library object
    #   @param self reference for this object
    def _initializeConnectionHandler(self):
        # Create handler, port and baudrate are mandatory
        #self.connectionHandler = Command.Serial.Command(self.connectionSerialPort,self.connectionBaudrate)
        self.connectionHandler = SerialPortClass.SerialPortClass(self.connectionSerialPort,self.connectionBaudrate)


    ## Clean environment before exit
    #   @param self reference for this object
    #   @return true if all was ok, otherwise false
    def free(self):
        super(SerialHandler, self).free()
        if ( self.connectionSerialPort is not None ):
            del self.connectionSerialPort
        if ( self.connectionBaudrate is not None ):
            del self.connectionBaudrate
