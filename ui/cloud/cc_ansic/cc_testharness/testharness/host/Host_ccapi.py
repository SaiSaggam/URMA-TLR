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
# Cloud Connector test framework - Host CCAPI class
#

################ Import Common libraries
import tempfile     ### This module generates temporary files and directories
import logging      ### Functions and classes to implement a event logging system
import sys          ### Access to variables or functions maintained by the interpreter
import os           ### Some useful functions on pathnames
import subprocess   ### Allows to spawn new processes, connect to their input/output/error pipes, and obtain their return codes
import time         ### Provides various time-related functions
import shutil       ### Functions to do high-level operations on files and collections of files

################ Import Cloud Connector libraries for CCAPI
sys.path.insert(0, os.path.abspath(os.path.join('testharness','libraries','ccapi')))

libs = ["Host_linux", "helperLibraryHeaderFile_ccapi", "Makefile_ccapi"]

for elem in libs:
    exec("import %s" % elem)
    exec("reload (%s)" % elem)


class Host_ccapi(Host_linux.Host_linux):
    ################ Environment
    # root path
    rootPath = None
    # source code path for all tools
    sourcePath = None
    # folder name for source code
    sourceFolderName = None
    # source code path for Cloud Connector
    sourceCloudConnectorPath = None
    # sandbox path
    sandboxPath = None
    # Source code inside sandbox path for the project
    sourceProjectPath = None

    ################ Git Repositories
    # Etherios Cloud Connector for ANSI C
    gitRepositoryURL = "ssh://git@stash.digi.com/cc/cc_api.git"




    def __init__(self):
        ## Call to __init__ from Host(): Initialize function
        super(Host_ccapi, self).__init__()

        ################ Environment
        self.identifier = "Host_ccapi"
        self.rootPath = os.path.abspath(".") #ej. "/home/username/TestFolder"
        self.sourcePath = os.path.join(self.rootPath,"source") #ej. "/home/username/TestFolder/source"
        self.sourceFolderName = "cc_api"
        self.sourceCloudConnectorPath = os.path.join(self.sourcePath,self.sourceFolderName)

        ################ Logging
        self.logger = logging.getLogger('cc.host.ccapi')




    def createEnvironment(self):
        self.logger.info("Creating environment for Host CCAPI")

        ################ Download source code
        # Create source dir
        if not os.path.exists(self.sourcePath):
            self.logger.info("Creating source directory in Host: '%s'" % self.sourcePath)
            os.makedirs(self.sourcePath)
        else:
            self.logger.warning("Skip creation of source directory in Host, because directory '%s' already exists" % self.sourcePath)

        # Download from git source code
        downloadSourcePath = os.path.join(self.sourcePath,self.sourceFolderName) #ej. "/home/username/TestFolder/source/<folder_name>"
        if not os.path.exists(downloadSourcePath):
            status = self.downloadRepositoryGit(self.gitRepositoryURL, self.sourcePath, self.sourceFolderName)
        else:
            self.logger.warning("Skip download of source code from Git, because destination path already exists: '%s'" % downloadSourcePath)
            status = True


        if(status != True):
            return (False,"An error was occurred getting the source code from git")
        else:
            ################ Get CC Test cases from source code
            # Create cc_testcases dir
            if not os.path.exists(self.sourceTestCasesPath):
                #self.logger.info("Creating test cases directory in Host: '%s'" % self.sourceTestCasesPath)
                #os.makedirs(self.sourceTestCasesPath)

                # Copy from git source code
                ccTestCasesSourcePath = os.path.join(self.sourceCloudConnectorPath,"tests","cc_testcases") #ej. "/home/username/TestFolder/source/cc_api/tests/cc_testcases"
                if os.path.exists(ccTestCasesSourcePath):
                    try:
                        shutil.copytree(ccTestCasesSourcePath, self.sourceTestCasesPath)
                        _msg = "Cloud Connector Test cases copied in '%s'" % self.sourceTestCasesPath
                        self.logger.info(_msg)
                        status = True
                    except OSError,e:
                        self.logger.exception('Exception caught: %s' % type(e))
                        _msg = "Exception [OSError]: %s" % (str(e))
                        status = False
                else:
                    self.logger.error("Missing cc_testcases directory in '%s'" % ccTestCasesSourcePath)
                    status = False
            else:
                self.logger.warning("Skip creation of test cases directory, because directory '%s' already exists" % self.sourceTestCasesPath)


        if(status != True):
            return (False,"An error was occurred getting the test cases from source code")
        else:
            ################ Build Config Generator tool from source code
            status = self.compileConfigGeneratorTool(self.sourceCloudConnectorPath)

            if(status != True):
                return (False,"An error was occurred building the source code from Config Generator tool")
            else:
                return (True,"Environment for Host was successfully created")



    def setupDevice(self):
        ############# Setup Method to generate Device ID
        if(self.inputArguments.deviceid_basedon != None):
            # Set Source
            self.handlerDevice.setDeviceIDMethod(self.inputArguments.deviceid_basedon)
        else:
            return (False,"Missing method to generate Device ID")

        ############# Setup Device ID
        if(self.handlerDevice.getDeviceIDMethod() == "mac"):
            ## Based on MAC
            if(self.inputArguments.devicemac != None):
                # Set Device MAC
                self.handlerDevice.setDeviceMAC(self.inputArguments.devicemac)
            else:
                # Generate a random MAC
                self.handlerDevice.generateDeviceMAC()
        elif(self.handlerDevice.getDeviceIDMethod() == "imei"):
            ## Based on IMEI
            if(self.inputArguments.deviceimei != None):
                # Set Device IMEI
                self.handlerDevice.setDeviceIMEI(self.inputArguments.deviceimei)
            else:
                # Generate a random IMEI
                self.handlerDevice.generateDeviceIMEI()
        elif(self.handlerDevice.getDeviceIDMethod() == "esn"):
            ## Based on ESN
            if(self.inputArguments.deviceesn != None):
                # Set Device ESN
                self.handlerDevice.setDeviceESN(self.inputArguments.deviceesn)
            else:
                # Generate a random ESN
                self.handlerDevice.generateDeviceESN()
        elif(self.handlerDevice.getDeviceIDMethod() == "meid"):
            ## Based on MEID
            if(self.inputArguments.devicemeid != None):
                # Set Device MEID
                self.handlerDevice.setDeviceMEID(self.inputArguments.devicemeid)
            else:
                # Generate a random MEID
                self.handlerDevice.generateDeviceMEID()


        ############# Setup Device Type
        if(self.inputArguments.devicetype != None):
            # Set Device Type
            self.handlerDevice.setDeviceType(self.inputArguments.devicetype)
        else:
            # Generate a random Device Type
            self.handlerDevice.generateDeviceType()


        ############# Setup Device Firmware version
        if(self.inputArguments.devicefirmware_version != None):
            # Set Device Type
            self.handlerDevice.setDeviceFirmwareVersion(self.inputArguments.devicefirmware_version)


        ############# Setup Device TX Keepalive
        if(self.inputArguments.ansic_compile_time_data is True):
            # In compilation time, we pass different values to have different behavior
            self.handlerDevice.setDeviceTxKeepalive(40)
        else:
            # Default value
            self.handlerDevice.setDeviceTxKeepalive(5)


        ############# Setup Device RX Keepalive
        if(self.inputArguments.ansic_compile_time_data is True):
            # In compilation time, we pass different values to have different behavior
            self.handlerDevice.setDeviceRxKeepalive(45)
        else:
            # Default value
            self.handlerDevice.setDeviceRxKeepalive(75)


        return (True,"Device was configured")


    def resetDevice(self):
        ################ Create handlerDevice
        status = self.getHandlerDevice()
        if(status[0] != True):
            return (False,"An error was occurred getting the handler Device: %s" % status[1])
        else:
            self.handlerDevice = status[1]

            ################ Setup handlerDevice
            status = self.setupDevice()
            if(status[0] != True):
                return (False,"An error was occurred setting the handler Device: %s" % status[1])
            else:
                return (True,"Environment for Host was successfully created")



    def downloadRepositoryGit(self, urlGit, destinationPath, folderName):
        self.logger.info("Clone Git repository '%s' in '%s'" % (urlGit, destinationPath) )

        result = False

        ################ Download latest source code from Git
        process = subprocess.Popen(
            ['git', 'clone', '--recursive', '%s' % urlGit, '%s' % folderName],
            cwd=destinationPath,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT)

        data = process.communicate()[0]
        if(process.returncode == 0):
            self.logger.info("Git repository '%s' was successfully cloned in '%s'" % (urlGit, destinationPath) )
            if data is not None:
                self.logger.debug(data)
            result=True
        else:
            self.logger.error("An error occurred while the repository '%s' was cloned in '%s'" % (urlGit, destinationPath) )
            if data is not None:
                self.logger.debug(data)
            result=False

        ################ If a specific commit is provided, move source to this commit
        if(result and self.inputArguments.ansic_git_commit is not None):
            self.logger.info("Move Git repository to commit '%s'" % self.inputArguments.ansic_git_commit)
            destinationPathCheckout = os.path.join(destinationPath,folderName)

            process = subprocess.Popen(
                ['git', 'checkout', self.inputArguments.ansic_git_commit],
                cwd=destinationPathCheckout,
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT)

            data = process.communicate()[0]
            if(process.returncode == 0):
                self.logger.info("Git repository was successfully moved to commit '%s' in '%s'" % (self.inputArguments.ansic_git_commit,destinationPathCheckout) )
                if data is not None:
                    self.logger.debug(data)
                result=True
            else:
                self.logger.error("An error occurred while the repository was moved to commit '%s' in '%s'" % (self.inputArguments.ansic_git_commit,destinationPathCheckout) )
                if data is not None:
                    self.logger.debug(data)
                result=False

        ################ If a specific private folder is needed, get it from passed commit
        if(result and self.inputArguments.ansic_private_git_commit is not None):
            self.logger.info("Get private folder in Git repository from commit '%s'" % self.inputArguments.ansic_private_git_commit)
            temporalFolderName = "%s%s" % (folderName,"_tmp")

            # Clone the repository in a temporal folder
            process = subprocess.Popen(
                ['git', 'clone', '%s' % urlGit, '%s' % temporalFolderName],
                cwd=destinationPath,
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT)

            data = process.communicate()[0]
            if(process.returncode == 0):
                self.logger.info("Git repository '%s' was successfully cloned in '%s'" % (urlGit, destinationPath) )
                if data is not None:
                    self.logger.debug(data)
                result=True
            else:
                self.logger.error("An error occurred while the repository '%s' was cloned in '%s'" % (urlGit, destinationPath) )
                if data is not None:
                    self.logger.debug(data)
                result=False


            # Checkout the temporal repository to the passed commit
            destinationPathCheckout = os.path.join(destinationPath,temporalFolderName)

            process = subprocess.Popen(
                ['git', 'checkout', self.inputArguments.ansic_private_git_commit],
                cwd=destinationPathCheckout,
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT)

            data = process.communicate()[0]
            if(process.returncode == 0):
                self.logger.info("Git repository was successfully moved to commit '%s' in '%s'" % (self.inputArguments.ansic_private_git_commit,destinationPathCheckout) )
                if data is not None:
                    self.logger.debug(data)
                result=True
            else:
                self.logger.error("An error occurred while the repository was moved to commit '%s' in '%s'" % (self.inputArguments.ansic_private_git_commit,destinationPathCheckout) )
                if data is not None:
                    self.logger.debug(data)
                result=False


            # Replace the original private folder with the private folder in this commit
            #** Remove original private folder
            originalPrivatePath = os.path.join(destinationPath,folderName,"private")
            shutil.rmtree(originalPrivatePath)
            self.logger.debug("Removing private directory from: '%s'" % originalPrivatePath)

            #** Copy modified private folder to the original path
            modifiedPrivatePath = os.path.join(destinationPath,temporalFolderName,"private")
            shutil.copytree(modifiedPrivatePath, originalPrivatePath, symlinks=True)
            self.logger.debug("Copying private directory from: '%s' to: '%s'" % (modifiedPrivatePath,originalPrivatePath) )

            #** Remove temporal repository
            shutil.rmtree(destinationPathCheckout)
            self.logger.debug("Removing temporal directory from: '%s'" % destinationPathCheckout)


        return result


    def compileConfigGeneratorTool(self, sourceCloudConnectorPath):
        self.logger.info("Building Config Generator tool from source code in '%s' " % sourceCloudConnectorPath)
        # Build options path inside Cloud Connector code
        buildOptionsPath='source/cc_ansic/tools/config/build.xml'

        process = subprocess.Popen(
            ['ant','-f', buildOptionsPath],
            cwd=sourceCloudConnectorPath,
            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

        data = process.communicate()[0]

        if(process.returncode == 0):
            self.logger.info("Config Generator tool was successfully compiled in '%s'" % (sourceCloudConnectorPath) )
            if data is not None:
                self.logger.debug(data)
            return True
        else:
            self.logger.error("An error occurred while compilation process in '%s'" % (sourceCloudConnectorPath) )
            if data is not None:
                self.logger.debug(data)
            return False



    def generateMakefileApp(self, sourceCloudConnectorPath, testType,groupName):
        # Params:
        # sourceCloudConnectorPath : #ej. "/tmp/tmp03ihHdsd/app"
        self.logger.info("Building Makefile for source code in '%s' " % sourceCloudConnectorPath)

        # Get Library type from command line
        libType = self.inputArguments.ansic_type # 'run' or 'step'
        # Setup Makefile
        makeFile = Makefile_ccapi.Makefile_ccapi(mkTemplatePath=None)

        # Set Default compiler for each architecture
        makeFile.setCompiler(self.inputArguments.architecture)

        # Read Makefile template file
        makeFile.readTemplateFile()
        # Generate Makefile for this sample project
        makeFile.generateMakefile(sourceCloudConnectorPath)
        # Write Makefile in output path
        makeFile.writeDestinationFile()

        return True



    def createSourceProject(self, sourceGroupPath,testType,testGroup):

        ################ Test Cases with the standard structure
        # Source path
        sourceCodePath = os.path.join(sourceGroupPath,"source","ccapi")


        # Create sandbox for this group
        if(self.createSandbox() != True):
            return (False,"Error creating temporary directory to store the project")

        # Copy all common files needed into sandbox
        if(self.copySourceToSandbox() != True):
            return (False,"Error copying source code to sandbox")

        ## Copy all files for the proyect
        ################ Test Cases with the standard structure
        # Copy all files needed for the project into sandbox
        if(self.copySourceAppToSandbox(sourceCodePath) != True):
            return (False,"Error copying project source code to sandbox")

        # Verify if exist a Makefile
        if(not os.path.isfile(os.path.join(self.sourceProjectPath,"Makefile"))):
            ################ Test Cases with the standard structure
            # Generate Makefile
            if(self.generateMakefileApp(self.sourceProjectPath,testType,testGroup) != True):
                return (False,"Error generating Makefile for project source code in sandbox")
        else:
            self.logger.info("Skip Makefile generation because already exists in '%s'" % self.sourceProjectPath)


        # Set Device with the Cloud Connector path
        self.handlerDevice.sourceProjectPath = self.sourceProjectPath

        return(True, "Source project was created successfully")



    def saveSourceProject(self,testType,testGroup):
        # Get IP of this server
        serverIP= self.getIP()
        self.logger.info("Host has the following IP: %s" % serverIP)

        # Source Project path
        filename = "cc_%s_%s.tar.gz" % (testType,testGroup)
        outputFilePath = os.path.abspath(os.path.join(self.testResultPath,filename))

        self.logger.info("Generating '%s' file with source code for this test project in '%s'" % (filename,self.sandboxPath))
        process = subprocess.Popen(
            ['tar','-cvzpf', outputFilePath, "."],
            cwd=self.sandboxPath,
            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

        data = process.communicate()[0]

        if(process.returncode == 0):
            self.logger.info("Compressed file was successfully created in '%s'" % (self.testResultPath) )
            if data is not None:
                #self.logger.debug(data)
                pass
            return True
        else:
            self.logger.error("An error occurred while compression process in '%s'" % (self.testResultPath) )
            if data is not None:
                self.logger.debug(data)
            return False




    def removeSourceProject(self):
        if ( self.sandboxPath is not None ):
            if os.path.exists(self.sandboxPath):
                if(self.sandboxPath != "/"):
                    # Delete any content
                    shutil.rmtree(self.sandboxPath)
                    self.logger.info("Temporary directory was removed from '%s'" % self.sandboxPath)
        else:
            self.logger.warning("Temporary directory for source project is empty, and it was not removed")



    def createSandbox(self):
        """
        Creates a temporary directory with contents of input directory.
        This can be used to compile code and execute tests in a separate
        place and will allow tests to run concurrently.
        """
        # Get a temporary directory name.
        self.sandboxPath = tempfile.mkdtemp()
        # Verify is an existing directory
        status = os.path.isdir(self.sandboxPath)
        if(status):
            self.logger.info("Temporary directory was created in '%s'" % self.sandboxPath)
            # Delete any content
            os.removedirs(self.sandboxPath)
            return True
        else:
            self.logger.error("Temporary directory '%s' does not exists" % self.sandboxPath)
            return False


    def copySourceToSandbox(self):
        ################ Source code base (Libraries, common files, etc.)

        # Copy all source to sandbox
        shutil.copytree(self.sourceCloudConnectorPath, self.sandboxPath, symlinks=True)

        # Remove all no necessary files
        listFilesRemove = [ os.path.join(self.sandboxPath,"testrunner.cpp"),
                            os.path.join(self.sandboxPath,"Makefile"),
                            ]
        for item in listFilesRemove:
            if(os.path.isfile(item)):
                self.logger.info("Removing file from sandbox: '%s'" % item)
                os.remove(item)

        # Remove all no necessary directories
        listDirRemove = [ os.path.join(self.sandboxPath, "tests", "samples"),
                            os.path.join(self.sandboxPath, "tests", "mocks"),
                            os.path.join(self.sandboxPath, "tests", "unit_tests"),
                            ]

        for item in listDirRemove:
            if(os.path.isdir(item)):
                self.logger.info("Removing directory from sandbox: '%s'" % item)
                shutil.rmtree(item)

        return True


    def copySourceAppToSandbox(self,testGroupPath):
        ################ Copy source code for the project into the sandbox

        # Destination Folder
        self.sourceProjectPath = os.path.join(self.sandboxPath,"app") #ej. "/tmp/tmp03ihHdsd/app")

        # Origin Folder
        projectCodePath = testGroupPath #ej. "/home/username/TestFolder/source/cc_testcases/dvt/reboot/source/ansic")



        # Verify if Origin folder is an existing directory
        if(os.path.isdir(projectCodePath)):
            shutil.copytree(projectCodePath, self.sourceProjectPath, symlinks=True)
            self.logger.info("Copying project directory from '%s' to '%s'" % (projectCodePath,self.sourceProjectPath))

            return True
        else:
            self.logger.error("Source project directory '%s' does not exists" % projectCodePath)
            return False


    def buildSourceProject(self):

        # Setup "custom_connector_config.h"
        self.setupLibrary("helper_library.h")

        # Build project code
        status = self.compileProjectCode(self.sourceProjectPath)

        if(status[0]):
            return (True,"Cloud Connector Project was successfully compiled")
        else:
            return (False,"Compilation process failed: %s" % status[1])




    def compileProjectCode(self, sourcePath):
        self.logger.info("Building Cloud Connector Project from '%s' " % sourcePath)

        ### Execute make process
        # Generate compilation command line
        commandLine = ['make']

        self.logger.info("Compilation command line: %s " % " ".join(commandLine))

        # Execute compilation command line
        process = subprocess.Popen(
            commandLine,
            cwd=sourcePath,
            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

        data = process.communicate()[0]

        if(process.returncode == 0):
            self.logger.info("Cloud Connector Project was successfully compiled in '%s'" % (sourcePath) )
            if data is not None:
                self.logger.debug(data)
            return (True,"Compilation process was successful")
        else:
            self.logger.error("An error occurred while compilation process in '%s'" % (sourcePath) )
            if data is not None:
                self.logger.debug(data)
            return (False,data)



    def setupLibrary(self, filename):

        if(filename == "helper_library.h"):
            filePath = os.path.join(self.sourceProjectPath, "helper", filename)
            # Verify if this file exists
            if(os.path.isfile(filePath)):
                # Setup app/helper/helper_library.h
                configFile = helperLibraryHeaderFile_ccapi.helperLibraryHeaderFile_ccapi(filePath)
                # Read original file
                configFile.readFile()


                if(self.handlerDevice.getDeviceIDMethod() == "provision"):
                    pass
                else:
                    # Set MAC Address
                    if(self.handlerDevice.getDeviceIDMethod() == "mac"):
                        # Add Device MAC
                        configFile.setupDeviceMAC(self.handlerDevice.getDeviceMAC())
                    #elif(self.handlerDevice.getDeviceIDMethod() == "imei"):
                        ## Add Device IMEI
                        #configFile.setupDeviceIMEI(self.handlerDevice.getDeviceIMEI())
                    #elif(self.handlerDevice.getDeviceIDMethod() == "esn"):
                        ## Add Device ESN
                        #configFile.setupDeviceESN(self.handlerDevice.getDeviceESN())
                    #elif(self.handlerDevice.getDeviceIDMethod() == "meid"):
                        ## Add Device MEID
                        #configFile.setupDeviceMEID(self.handlerDevice.getDeviceMEID())


                # Set Device ID
                configFile.setupDeviceID( self.handlerDevice.getDeviceID() )

                # Set Vendor ID
                configFile.setupVendorID(self.inputArguments.vendorid)

                # Set Cloud Server
                configFile.setupCloudServer(self.inputArguments.server)

                # Set Device Type
                configFile.setupDeviceType(self.handlerDevice.getDeviceType())

                # Set Device IP
                configFile.setupDeviceIP( self.getIP() )

                # Modify file with custom configuration
                configFile.setupFile()
            else:
                self.logger.warning("Skip configuration for file '%s' because does not exists in '%s'" % (filename,filePath))



    def flashSourceProject(self):
        self.logger.info("Skip flash process to Cloud Connector for CCAPI")

    def launchCloudConnectorOnDevice(self):
        self.logger.info("Start Cloud Connector for CCAPI on the PC")
        # Reset Device flags
        self.handlerDevice.resetFlags()
        # Launch thread monitoring Device
        self.handlerDevice.start()

        # Wait or verify that device is started
        time.sleep(10)
        return True


    def stopCloudConnectorOnDevice(self):
        self.logger.info("Stop Cloud Connector for CCAPI on the PC")

        # If Coverage is enabled we need to send the SIGINT signal to collect all coverage info
        if(self.inputArguments.gcov):
            self.handlerDevice.sendSignalSIGINT()


        # Stop thread monitoring Device
        self.handlerDevice.stop()

        # Wait until device is stopped
        timeout = 60
        while( timeout>0 and self.handlerDevice.isFinished()!=True):
            timeout-=1
            time.sleep(2)

        result = None
        if(timeout>0):
            self.logger.info("Cloud Connector for CCAPI was stopped successfully")
            result = True
        else:
            self.logger.error("Cloud Connector for CCAPI was not stopped due to a timeout")
            result = False

        return result
