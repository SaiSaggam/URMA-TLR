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
# S.A.R.E.S Framework - Base class to connection handler

# Import to get compatibility with SARES
#import ExpandSysPaths

import logging          # Functions and classes to implement a event logging system
import threading        # Provide higher-level threading interfaces on top of the lower level thread module
import os               # Some useful functions on pathnames
import time             # Provides various time-related functions
import datetime         # Supplies classes for manipulating dates and times in both simple and complex ways
import deque            # Custom Library: redifinition of standard collections.deque to improve the API


"""
    @TODOs:
        2. Change all names to standard names
        3. Check if change the type of exception raised
        4. Check the type of exceptions catched and raised
        5. Check function that parses output to queue
"""


libs = ['ExceptionLibrary']
for elem in libs:
    exec("import %s" % elem)
    #exec("reload (%s)" % elem)



## Base Class to all connection handler types
class ConnectionHandler(threading.Thread):
    "Main class to manage a connection handler"

    #*************** Environment
    ## Object identifier
    identifier = None


    #************** Logging
    ## Logger object to save system logging
    logger = None
    ## Log path for connection logging
    logPath = None
    ## Log filename for connection logging
    logFilename = None
    ## Log connection logging flag
    logFileEnable = None
    ## Log connection logging level
    logFileLevel = None

    ## Logger object to save in system logging
    loggerExecution = None
    ## Log execution logging flag
    logExecutionEnable = None
    ## Log identifier name
    logIdentifier = None


    #************** Settings
    ## Connection handler
    connectionHandler = None
    ## Connection timeout
    connectionTimeout = None
    ## Connection prompt
    connectionPrompt = None
    ## Connection delimiter
    connectionDelimiter = None
    ## Connection Username
    connectionUsername = None
    ## Connection Password
    connectionPassword = None

    ## Internal sleep time between reads
    readSleepTime = None
    ## Buffer to contain a historic output
    #log_data_buffer = None
    ## Buffer to contain a unread data
    unread_data_buffer = None

    ## Force test stop
    stop = False
    ### Test was finished
    finished = False


    #*************** Historical Output Management for Tests
    # Will store all data readed in a buffer. The Test script
    #   will consume from it and will be the responsible of cleaning it
    #   __Only__ readBuffer will update the historicalBuffer
    ## Historical Buffer Output
    unread_data_historical_buffer = None
    ## Historical Buffer Enabled
    historicalEnable = False


    #************** Synchronization
    ## Lock variable to write in data buffer
    _sharedLock = None



    ## Class Builder
    def __init__(self):
        # Do not call the __init__ thread with super
        threading.Thread.__init__(self)

        self.setLogIdentifier("sares")

        self.setIdentifier("ConnectionHandler")

        self.logFileEnable = True
        self.logFileLevel = logging.DEBUG       # By default is DEBUG
        self.logExecutionEnable = True

        # Initialize sleep time between reads in buffer
        self.readSleepTime = 0.1
        # Initialize array for save work log
        self.unread_data_buffer = ""
        # Initialize connection timeout
        self.connectionTimeout = 8

        # Initialize threading lock
        self._sharedLock = threading.Lock()


    ## Set Identifier for this object
    #   @param self reference for this object
    #   @param name string containing the object identifier
    def setIdentifier(self, name):
        if (name is None):
            raise ExceptionLibrary.Error('ConnectionHandler: Null identifier')
        else:
            self.identifier = name
            self.logFilename = "%s.log" % name
            self.setName(name)


    ## Set identifier for logging system
    #   @param self reference for this object
    #   @param name string containing the log identifier
    def setLogIdentifier(self, name):
        if (name is None):
            raise ExceptionLibrary.Error('ConnectionHandler: Null log identifier')
        else:
            self.logIdentifier = name



    ## Set output path for results, logs, etc
    #   @param self reference for this object
    #   @param path absolute path
    def setOuputPath(self, path):
        self.logPath = path


    ## Set timeout for this connection type
    #   @param self reference for this object
    #   @param timeout seconds to wait
    def setConnectionTimeout(self, timeout):
        self.connectionTimeout = timeout


    ## Set timeout in Library Handler descriptor
    # This timeout _only_ applies for the open call
    #   @param self reference for this object
    def _setConnectionOpenTimeout(self, timeout):
        self.connectionHandler.setSessionOpenTimeout(timeout)


    ## Set prompt for this connection type
    #   @param self reference for this object
    #   @param prompt string with prompt
    def setConnectionPrompt(self, prompt):
        self.connectionPrompt = prompt


    ## Set delimiter for this connection type
    #   @param self reference for this object
    #   @param delimiter string with delimiter
    def setConnectionDelimiter(self, delimiter):
        self.connectionDelimiter = delimiter


    ## Set username for this connection type
    #   @param self reference for this object
    #   @param username string with user to login
    def setConnectionUsername(self, username):
        self.connectionUsername = username


    ## Set password for this connection type
    #   @param self reference for this object
    #   @param password string with pass to login
    def setConnectionPassword(self, password):
        self.connectionPassword = password


    ## Set all settings for this connection type
    #   @param self reference for this object
    #   @param settings dict with connection settings
    def setConnectionSettings(self, settings):
        if (isinstance(settings,dict)):
            # Check Connection settings
            if (settings.has_key("timeout")):
                self.setConnectionTimeout(settings["timeout"])
            if (settings.has_key("prompt")):
                self.setConnectionPrompt(settings["prompt"])
            if (settings.has_key("delimiter")):
                self.setConnectionDelimiter(settings["delimiter"])
            if (settings.has_key("username")):
                self.setConnectionUsername(settings["username"])
            if (settings.has_key("password")):
                self.setConnectionPassword(settings["password"])
        else:
            raise ExceptionLibrary.Error('setConnectionSettings requires a dict')


    ## Enable the connection logging system
    #   @param self reference for this object
    def enableLogFile(self):
        self.logFileEnable = True


    ## Disable the connection logging system
    #   @param self reference for this object
    def disableLogFile(self):
        self.logFileEnable = False


    ## Set logging level for this handler
    #   @param self reference for this object
    def setLogFileLevel(self, logLevel):
        self.logFileLevel = logLevel


    ## Enable the inclusion of this connection in the general system logging
    #   @param self reference for this object
    def enableLogExecution(self):
        self.logExecutionEnable = True


    ## Disable the inclusion of this connection in the general system logging
    #   @param self reference for this object
    def disableLogExecution(self):
        self.logExecutionEnable = False


    ## Stopping the execution process
    #   @param self reference for this object
    def stopExecution(self):
        self.stop = True


    ## Open descriptor and create a new handler to manage it
    #   @param self reference for this object
    #   @return true if processs was successfully, otherwise false
    def open(self):
        # Start logging
        self.startLoggingSystem()

        # Create object for handler
        self._initializeConnectionHandler()

        # Open descriptor
        details = self._openConnectionHandler()

        # Set an initial sentence to the beginning of each session
        if (self.logFileEnable):
            # Set an initial sentence to the beginning of each session
            self._printTimestampStartSession(details)

        return True


    ## (private) Set an initial sentence to the beginning of each session
    #   @param self reference for this object
    def _printTimestampStartSession(self, message = None):
        # Set an initial sentence to each session
        timestamp = datetime.datetime.fromtimestamp(time.time()).strftime('%Y-%m-%d %H:%M:%S')
        dottedLine = '-'*20
        self.logger.info('\n%s Opened a new %s descriptor at %s %s\n' % (dottedLine, type(self).__name__, timestamp, dottedLine))

        # Print optional message
        if (message is not None):
            self.logger.info(message)



    ## (private) Creates(__init__) the Command Library object
    #   @param self reference for this object
    def _initializeConnectionHandler(self):
        raise Exception('_initializeConnectionHandler not implemented')


    ## (private) Open descriptor and create a new handler to manage it
    #   @param self reference for this object
    #   @return true if processs was successfully, otherwise false
    def _openConnectionHandler(self):
        """
        In case there is some exception if the except code we will get an "UnboundLocalError: local variable 'details' referenced before assignment"
        Setting details and error out of try/except
        """
        details = 'Handler still not opened'
        error = None
        try:
            # This _only_ sets the max open timeout
            self._setConnectionOpenTimeout(self.connectionTimeout)
            # Open ssh session
            status,details = self.connectionHandler.open()
            if not status:
                error = details
        except ExceptionLibrary.Error,error:
            self.logger.warning(error)
        except EOFError,e:
            error = " Error EOF: %s " % e
        except Exception,e:
            error = " Exception: %s " % e
            self.logger.exception(e)
        finally:
            if error is None:
                return details
            else:
                raise ExceptionLibrary.Error('OpenHandler: %s' % error)


    ## Main function to start the reading of the descriptor
    #   @param self reference for this object
    def run(self):
        data_buffer = ""
        try:
            while (not self.stop):
                # Read all data from handler
                data_buffer = self.connectionHandler.readAll()

                # Check if we have new data
                if (len(data_buffer) > 0):

                    # Save new data in the unreaded buffer
                    self._sharedLock.acquire() # Wait to acquire the lock.
                    self.unread_data_buffer += data_buffer
                    self._sharedLock.release() # Liberate lock acquired

                    # Save it in the file logger
                    if (self.logFileEnable):
                        # We do it here to have a clean output
                        # the same than a user would have.
                        self._parse_output_to_fileLogging(data_buffer)

                    # Save it in the Execution logger
                    if (self.logExecutionEnable):
                        self._parse_output_to_executionLogging(data_buffer)

                    # Clean data buffer
                    data_buffer = ""

                # Wait to read next data in the buffer
                time.sleep(self.readSleepTime)

        except (ExceptionLibrary.Error),e:
            self.loggerExecution.warning(e)
        except (Exception),e:
            self.loggerExecution.exception(e)
        finally:
            # Activate Flags
            self.finished = True

            # Flush residual data in the buffer
            if (len(data_buffer) > 0):

                # Save it in the file logger
                if self.logFileEnable:
                    self._parse_output_to_fileLogging(data_buffer)

                # Save it in the Execution logger
                if (self.logExecutionEnable):
                    self._parse_output_to_executionLogging(data_buffer)



    ## Start logging system for the execution
    #   @param self reference for this object
    #   @return true if all was ok, otherwise false
    def startLoggingSystem(self):
        #*************** Execution Logging
        if (self.logExecutionEnable):
            self.loggerExecution = logging.getLogger('%s.%s' % (self.logIdentifier,self.identifier) )

        #*************** Connection Logging
        if (self.logFileEnable):
            # generate a random identifier to avoid collisions between handlers
            sessionID = os.urandom(32)
            # create logger
            self.logger = logging.getLogger(sessionID)
            # set log level
            self.logger.setLevel(self.logFileLevel)

            # check if destination path exist
            if (not os.path.exists(self.logPath)):
                # Create output path
                os.makedirs(self.logPath)

            # create file handler which logs even debug messages
            filePath = os.path.join(self.logPath, self.logFilename)
            fh = logging.FileHandler(filePath)
            fh.setLevel(1)
            # Doing this will set this specific handler to do not jump
            #  from line in the stored file for every log call
            # ALERT: this is a custom function included in a custom
            #        logging library. THIS IS NOT STANDARD
            try:
                fh.setLineSeparator('')
            except AttributeError, e:
                if (self.logExecutionEnable):
                    self.loggerExecution.error("Exception caught: %s" % e)
                    self.loggerExecution.exception(e)

            # create formatter
            formatter = logging.Formatter('%(message)s')

            # add formatter to console handler and file handler
            fh.setFormatter(formatter)

            # add handlers to logger
            self.logger.addHandler(fh)



    ## This function parses the output readed from descriptor,
    #   reformat it send it to the file logging.
    #   @param self reference for this object
    #   @param data_buffer string containing all data from the buffer
    def _parse_output_to_fileLogging(self, data_buffer):
        # Save data without changes
        self.logger.info(data_buffer)



    ## This function parses the output readed from descriptor,splits it in lines,
    #   and send it to the execution logging.
    #   see http://es.wikipedia.org/wiki/ASCII
    #   @param self reference for this object
    #   @param data_buffer string containing all data from the buffer
    def _parse_output_to_executionLogging(self, data_buffer):
        lines = self._parse_output_to_queue(data_buffer)

        for eachLine in lines:
            # Parser some characters to get a better representation

            # \x08 is the character representation for backspace
            msg = eachLine.replace("\x08","\\x08")

            # ^M is the character representation for return line
            msg = msg.replace(chr(13),"\n")

            # ^G is the character representation for BEL
            msg = msg.replace(chr(7),"\n")

            # Remove any other unknown/unexpected char. Optionally we can remove this instruction here and add it to logging class
            newLine = ''.join([c for c in msg if (31 < ord(c) < 256 or ord(c) in [9, 10 , 13])])

            # Send line to the logger
            self.loggerExecution.debug(newLine)



    ## This function parses the output readed from descriptor and splits it in lines.
    #   Also save all data in the data buffer
    #   @param self reference for this object
    #   @param data_buffer string containing all data from the buffer
    #   @return array each element is a line of the data buffer
    def _parse_output_to_queue(self, data_buffer):
        parser_output_in_lines = deque.deque()
        # split output in different lines
        parser_output = data_buffer.split("\n")

        # if buffer has new entries
        if (len(parser_output) > 0):
            for data_group in parser_output:
                data_group = data_group.split("\r")

                # if each entry has lines
                for eachLine in data_group:
                    if (len(eachLine) > 0):
                        # Save the new line
                        parser_output_in_lines.append(eachLine)

        return parser_output_in_lines


    ## This function parses the output readed from descriptor and splits it in lines,
    #   returning the first line only.
    #   @param self reference for this object
    #   @param data_buffer string containing all data from the buffer
    #   @return line string containing the first line in the buffer
    #   @return index integer with the index of the fisrt line final in the data_buffer, including \n\r
    def _parse_output_to_get_one_line(self, data_buffer):
        # Index to remove all characters before it
        index = 0

        # Find the first "/n"
        index_n = data_buffer.find("\n")

        if(index_n >= 0):
            # If we have some element "/n"
            index = index_n+1
        else:
            # If we have not any element "/n"
            index = len(data_buffer)

        # Get the read line
        line = data_buffer[:index]

        # Remove from the line unused characters
        line = line.replace("\r","")
        line = line.replace("\n","")

        # Return formated line and final line index
        return line,index



    ## Function reads a only line from data buffer
    #   @param self reference for this object
    #   @return string first line read from data buffer
    def readLine(self):
        result = None
        if (len(self.unread_data_buffer) > 0):
            # Wait to acquire the lock.
            self._sharedLock.acquire()

            # line: string with the content of the first line in the buffer
            # index: integer with the index of the first line final, including \n\r
            line,index = self._parse_output_to_get_one_line(self.unread_data_buffer)

            # If historical buffer is enabled, save the new data
            if (self.isHistoricalEnabled()):
                self.unread_data_historical_buffer += self.unread_data_buffer[:index]

            # Remove the first line from the data buffer
            self.unread_data_buffer = self.unread_data_buffer[index:]

            # Liberate lock acquired
            self._sharedLock.release()

            # Return readed line
            result = line

        return result


    ## Function reads all lines from data buffer
    #   @param self reference for this object
    #   @return array all lines read from data buffer
    def readBuffer(self, type_ = "deque"):
        dataBuffer = None

        # Create a list deque
        if (type_ == "deque"):
            newLine = self.readLine()

            while newLine is not None:
                # First time we create the queue
                if (dataBuffer is None):
                    dataBuffer = deque.deque()
                # Add the newLine
                dataBuffer.append(newLine)
                # Read a new Line
                newLine = self.readLine()
        else:
            # Return a string
            if (len(self.unread_data_buffer) > 0):
                # Wait until acquire lock resource
                self._sharedLock.acquire()

                # Read the buffer
                dataBuffer = self.unread_data_buffer

                # Clear the buffer, we can do it because resource is locked
                self.unread_data_buffer = ''

                # Liberate lock resource
                self._sharedLock.release()

                # If historical buffer is enabled, save the new data
                if (self.isHistoricalEnabled()):
                    self.unread_data_historical_buffer += dataBuffer


        return dataBuffer


    ## Function reads data from data buffer until a pattern was found or by timeout
    #   @param self reference for this object
    #   @param pattern string to find
    #   @param timeout second to wait
    #   @return array with the following parameters:
    #   * [status]: true or false
    #   * [line]: line containing pattern or message indicating error
    #   * [buffer]: all data read from buffer (type deque)
    def readUntilPattern(self, pattern=None, timeout=None):

        if (pattern is None):
            # If pattern is not provided, use the default connection prompt
            pattern = self.connectionPrompt

        # Convert pattern to list
        if not ( isinstance(pattern, list) ):
            pattern = [pattern]

        # If timeout is not provided, use the default connection timeout
        if (timeout is None):
            timeout = self.connectionTimeout

        # Get Time
        start_time = time.time()
        current_time = time.time()

        # Initialize parameters
        bufferRead = ""
        patternFound = None


        while ( not self.stop
                and (patternFound is None)
                and ((current_time-start_time) < timeout)):
            # Read all data from buffer. We need to read in strings to avoid incorrect splitted lines because in that case we will not find the pattern
            newBuffer = self.readBuffer(type_="string")

            if (newBuffer is not None):
                # Save new data in buffer
                bufferRead += newBuffer
                # Create a deque object from string so we can use its search methods
                dequeBufferRead = self._parse_output_to_queue(bufferRead)

                for eachPattern in pattern:
                    # Verify if the pattern is in the new data Buffer using deque search method
                    if (dequeBufferRead.find(eachPattern) != -1):
                        patternFound = eachPattern
                        break
            # Update current time
            current_time = time.time()

            """
            Sleep to avoid a continuosly resources locking and blocking the main run thread
            # In case we have problems we can use as sleep a very 'safety' value like (self.readSleepTime + 0.02),
            in which case the max shift will be twice the readSleepTime. Using 0.25 of sleep time allow us to get
            very precise values and we minimize the shift.
            """
            time.sleep(self.readSleepTime*0.25)

        # Transform string in lines into a deque list
        dataBuffer = self._parse_output_to_queue(bufferRead)

        # Check if pattern was found in the data read from the buffer
        if (patternFound is not None):
            firstMatchedIndex = dataBuffer.find(patternFound)
            return (True, firstMatchedIndex, dataBuffer)
        else:
            if (len(pattern) == 1):
                msg = "pattern %s was " % repr(pattern)
            else:
                msg = "patterns %s were " % repr(pattern)
            msg += "not found in the buffer, finish by timeout of"
            msg += " %.2f seconds (%.2f)" % (timeout, (current_time-start_time))
            return (False, msg, dataBuffer)



    ## Function to write characters without termination line (delimiter)
    #   @param self reference for this object
    #   @param command string characters or command to write
    #   @return array contain a tuple:
    #   - [status]: true or false
    #   - [message]: result/error message
    def write(self, command = None):
        if (command is None):
            return (False, "Specified a null command")
        else:
            try:
                self.connectionHandler.write(command)
                return (True, "Command was written successfully")
            except (ExceptionLibrary.Error), e:
                return (False, "Command was not written due to '%s'" % e)


    ## Function to execute a command
    #   @param self reference for this object
    #   @param command string command to execute
    #   @return array contain a tuple:
    #   - [status]: true or false
    #   - [message]: result/error message
    def executeCommand(self, command, writeDelimiter = True):
        if (writeDelimiter):
            # Write command in the handler with delimiter
            command += self.connectionDelimiter
        if (self.logExecutionEnable):
            self.loggerExecution.debug("Executing command: %s" % repr(command))
        return self.write(command)


    ## Function to execute a command, wait for a pattern and read buffer, then return the output
    #   @param self reference for this object
    #   @param command string command to execute
    #   @param pattern string to find
    #   @param timeout second to wait
    #   @return array with the following parameters:
    #   * [status]: true or false
    #   * [buffer]: all data read from buffer
    #   * [line]: line containing pattern
    def executeCommandReturnOutput(self, command, pattern=None, timeout=None, writeDelimiter = True):
        # Flush Connection Output
        cachedData = self.readBuffer()

        # TODO: we can implement a retry system to
        # make sure than buffer is clean, perform
        # several reads and only execute the command
        # after two empty reads

        # First execute command
        status,details = self.executeCommand(command, writeDelimiter)

        if (status):
            # Wait until pattern
            status,details,bufferRead = self.readUntilPattern(pattern, timeout)
            if (cachedData is not None):
                # Merge cachedData with bufferData

                # Note: we need to reverse the cachedData because
                # extendleft() appends results in reversing the order of elements in the iterable argument
                cachedData.reverse()
                bufferRead.extendleft(cachedData)

                # If we found the pattern, we provide the macthing index
                # then if we expand the list with the cached data,
                # we need to increase the index of the matching line.
                if ( status and details != -1):
                    details += len(cachedData)

            return status,details,bufferRead
        else:
            return status,details,cachedData


    ## Check if the connection handler is working
    #   @param self reference for this object
    #   @return true if connection is ok, otherwise false
    def isOpen(self):
        # Base on finished status, we decide if it is open
        return not self.finished


    # TODO: to keep the memory of the system, we should create
    #       a system based on read the logging file, and return
    #       the data in this file, instead of save a copy in
    #       the system memory



    #*************** Historical Buffer, will be managed by Tests

    ## Enable the historical buffer system
    #   @param self reference for this object
    def enableHistorical(self):
        self.clearHistoricalBuffer()
        self.historicalEnable = True


    ## Disable the historical buffer system
    #   @param self reference for this object
    def disableHistorical(self):
        self.clearHistoricalBuffer()
        self.historicalEnable = False


    ## Check if the historical buffer system is enabled
    #   @param self reference for this object
    #   @return true if is enabled, otherwise false
    def isHistoricalEnabled(self):
        return self.historicalEnable


    ## Clear the historical buffer
    #   @param self reference for this object
    def clearHistoricalBuffer(self):
        self.unread_data_historical_buffer = ""


    ## Function reads all lines from historical data buffer
    #   @param self reference for this object
    #   @return array all lines read from data buffer
    def readHistoricalBuffer(self):
        # Initialize output
        result = None

        if (self.unread_data_historical_buffer is not None):
            # Split historical data buffer in lines
            lines = self._parse_output_to_queue(self.unread_data_historical_buffer)
            if (len(lines) > 0):
                for eachLine in lines:
                    # First time we create the queue
                    if (result is None):
                        result = deque.deque()
                    # Add the line
                    result.append(eachLine)
        return result



    ## Clean environment before exit
    #   @param self reference for this object
    #   @return true if all was ok, otherwise false
    def free(self):
        if ( self.connectionHandler is not None ):
            self.connectionHandler.close()
            del self.connectionHandler
            self.connectionHandler = None
        if ( self.connectionTimeout is not None ):
            del self.connectionTimeout
