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
# Cloud Connector test framework - Cloud main file class for ANSI C
#

#*************** Import Common libraries
import logging      ### Functions and classes to implement a event logging system


#*************** Import Classes path
libs = ["BaseFile_ansic"]

for elem in libs:
    exec("import %s" % elem)
    exec("reload (%s)" % elem)


##  Main Class for Cloud main file
class MainFile_ansic(BaseFile_ansic.BaseFile_ansic):
    "platform/linux/main.c class to work with Cloud Connector Test Framework"


    ## Class Builder
    def __init__(self, filePath):
        ## Call to __init__ from Host(): Initialize function
        super(MainFile_ansic, self).__init__(filePath)

        #*************** Environment
        self.identifier = "main_c"

        #*************** Logging
        self.logger = logging.getLogger('cc.main_c')



    def setupCoverageIncludes(self):
        GCOV_INCLUDE_DATA = """
        #include <signal.h>
        #include <stdlib.h>
        """

        foundSentence = False
        fileLines = self.fileContent

        # Find if the define is in the file then modify it
        newFileContent = ""
        for line in fileLines.splitlines():
            # Enable compile time data passing so these values are used.
            if ( not foundSentence and line.startswith("#include") ):
                foundSentence = True

                # In the first include we add the coverage functions
                newFileContent += "%s\n" % GCOV_INCLUDE_DATA
                newFileContent += "\n"
                newFileContent += "%s\n" % line

            else:
                # Add the readed line
                newFileContent += "%s\n" % line

        # If this sentence there is not in the file we add it
        if( not foundSentence ):
            self.logger.error("Error adding the includes for coverage support on main.c")

        # Save new file content
        self.fileContent = newFileContent




    def setupCoverageFunctions(self):

        GCOV_FUNCTION_DATA = """
        void __gcov_flush(void);

        void gcov_handler(int signum)
        {
            APP_DEBUG("received signal %d to dump coverage data.\\n", signum);
            __gcov_flush(); /* dump coverage data on receiving SIGUSR1 */
            exit(0); /* exit cleanly after flushing */
        }

        void add_gcov_signal(void)
        {
            struct sigaction new_action, old_action;

            /* setup signal hander */
            new_action.sa_handler = gcov_handler;
            sigemptyset(&new_action.sa_mask);
            new_action.sa_flags = 0;
            sigaction(SIGUSR1, NULL, &old_action);
            if (old_action.sa_handler != SIG_IGN)
                sigaction (SIGUSR1, &new_action, NULL);
        }
        """



        GCOV_FUNCTION_DATA = """
        void __gcov_flush(void);

        void gcov_handler(int signum)
        {
            APP_DEBUG("received signal %d to dump coverage data.\\n", signum);
            __gcov_flush(); /* dump coverage data on receiving SIGINT */
            exit(0); /* exit cleanly after flushing */
        }

        void add_gcov_signal(void)
        {
            struct sigaction new_action, old_action;

            /* setup signal hander */
            new_action.sa_handler = gcov_handler;
            sigemptyset(&new_action.sa_mask);
            new_action.sa_flags = 0;
            sigaction(SIGINT, NULL, &old_action);
            if (old_action.sa_handler != SIG_IGN)
                sigaction (SIGINT, &new_action, NULL);
        }
        """


        foundIncludes = False
        foundBlock = False
        fileLines = self.fileContent

        # Find if the define is in the file then modify it
        newFileContent = ""
        for line in fileLines.splitlines():
            # Enable compile time data passing so these values are used.
            if ( not foundIncludes and line.startswith("#include") ):
                foundIncludes = True

            elif ( foundIncludes and not foundBlock and not line.startswith("#include") and line !=""):
                foundBlock = True
                # we add the coverage functions
                newFileContent += "%s\n" % GCOV_FUNCTION_DATA
                newFileContent += "\n"

            # Add the readed line
            newFileContent += "%s\n" % line

        # If this sentence there is not in the file we add it
        if( not foundIncludes or not foundBlock ):
            self.logger.error("Error adding the functions for coverage support on main.c")

        # Save new file content
        self.fileContent = newFileContent




    def setupCoverageSignal(self):
        GCOV_SIGNAL_DATA = """
    add_gcov_signal();
        """

        foundMain = False
        foundFirstLine = False
        fileLines = self.fileContent

        # Find if the define is in the file then modify it
        newFileContent = ""
        for line in fileLines.splitlines():

            # Add the readed line
            newFileContent += "%s\n" % line

            # Enable compile time data passing so these values are used.
            if ( not foundMain and " main" in line ):
                foundMain = True

            elif ( foundMain and not foundFirstLine and "{" in line):
                foundFirstLine = True
                # we add the coverage functions
                newFileContent += "%s\n" % GCOV_SIGNAL_DATA
                newFileContent += "\n"


        # If this sentence there is not in the file we add it
        if( not foundMain or not foundFirstLine ):
            self.logger.error("Error adding the includes for coverage support on main.c")

        # Save new file content
        self.fileContent = newFileContent
