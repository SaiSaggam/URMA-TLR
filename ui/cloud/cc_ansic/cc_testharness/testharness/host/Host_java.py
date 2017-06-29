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
# Cloud Connector test framework - Host JAVA class
#

# Import Common libraries
import tempfile     ### This module generates temporary files and directories
import logging      ### Functions and classes to implement a event logging system
import sys          ### Access to variables or functions maintained by the interpreter
import os           ### Some useful functions on pathnames
import subprocess   ### Allows to spawn new processes, connect to their input/output/error pipes, and obtain their return codes
import time         ### Provides various time-related functions
import shutil       ### Functions to do high-level operations on files and collections of files

################ Import Cloud Connector libraries for ANSI C
sys.path.insert(0, os.path.abspath(os.path.join('testharness','libraries','java')))

if ( "linux" in sys.platform ):
    from Host_linux import Host_linux as Host_parent
else:
    from Host_win import Host_win as Host_parent

libs = ["ConfigFile_java"]

for elem in libs:
    exec("import %s" % elem)
    exec("reload (%s)" % elem)


class Host_java(Host_parent):

    #*************** Environment
    # root path
    rootPath = None
    # source code path for all tools
    sourcePath = None
    # folder name for Java core source code
    sourceCoreFolderName = None
    # folder name for Java SE source code
    sourceSEFolderName = None
    # source code path for Cloud Connector
    sourceCloudConnectorPath = None
    # sandbox path
    sandboxPath = None
    # Source code inside sandbox path for the project
    sourceProjectPath = None

    #*************** Git Repositories
    # Etherios Cloud Connector for JAVA CORE
    gitJavaCoreRepositoryURL = "ssh://git@stash.digi.com/cc/cc_java_core.git"
    # Etherios Cloud Connector for JAVA SE
    gitJavaSERepositoryURL = "ssh://git@stash.digi.com/cc/cc_java_se.git"


    def __init__(self):
        ## Call to __init__ from Host(): Initialize function
        super(Host_java, self).__init__()

        #*************** Environment
        self.identifier = "Host_java"
        self.rootPath = os.path.abspath(".") #ej. "/home/username/TestFolder"
        self.sourcePath = os.path.join(self.rootPath,"source") #ej. "/home/username/TestFolder/source"
        self.sourceCoreFolderName = "cc_java_core"
        self.sourceSEFolderName = "cc_java_se"
        self.sourceCloudConnectorPath = os.path.join(self.sourcePath,self.sourceCoreFolderName)

        #*************** Logging
        self.logger = logging.getLogger('cc.host.java')




    def createEnvironment(self):
        self.logger.info("Creating environment for Host Java")

        #** Download source code
        # Create source dir
        if not os.path.exists(self.sourcePath):
            self.logger.info("Creating source directory in Host: '%s'" % self.sourcePath)
            os.makedirs(self.sourcePath)
        else:
            self.logger.warning("Skip creation of source directory in Host, because directory '%s' already exists" % self.sourcePath)

        # Download from git source code for java core
        downloadSourcePath = os.path.join(self.sourcePath,self.sourceCoreFolderName) #ej. "/home/username/TestFolder/source/<folder_name>"
        if not os.path.exists(downloadSourcePath):
            status = self.downloadRepositoryGit(self.gitJavaCoreRepositoryURL, self.sourcePath, self.sourceCoreFolderName)
        else:
            self.logger.warning("Skip download of Java Core source code from Git, because destination path already exists: '%s'" % downloadSourcePath)
            status = True

        if(status != True):
            return (False,"An error was occurred getting the Java Core source code from git")


        # Download from git source code for java SE implementation for PC
        downloadSourcePath = os.path.join(self.sourcePath,self.sourceSEFolderName) #ej. "/home/username/TestFolder/source/<folder_name>"
        if not os.path.exists(downloadSourcePath):
            status = self.downloadRepositoryGit(self.gitJavaSERepositoryURL, self.sourcePath, self.sourceSEFolderName)
        else:
            self.logger.warning("Skip download of Java SE source code from Git, because destination path already exists: '%s'" % downloadSourcePath)
            status = True

        if(status != True):
            return (False,"An error was occurred getting the Java SE source code from git")



        #** Get CC Test cases from source code
        # Create cc_testcases dir
        if not os.path.exists(self.sourceTestCasesPath):
            # Copy from git source code
            ccTestCasesSourcePath = os.path.join(self.sourcePath,self.sourceSEFolderName,"general","cc_testcases") #ej. "/home/username/TestFolder/source/cc_java_se/general//cc_testcases"
            if os.path.exists(ccTestCasesSourcePath):
                try:
                    shutil.copytree(ccTestCasesSourcePath, self.sourceTestCasesPath)
                    self.logger.info( "Cloud Connector Test cases copied in '%s'" % self.sourceTestCasesPath )
                    status = True
                except OSError,e:
                    self.logger.exception('Exception caught: %s' % type(e))
                    status = False
            else:
                self.logger.error("Missing cc_testcases directory in '%s'" % ccTestCasesSourcePath)
                status = False
        else:
            self.logger.warning("Skip creation of test cases directory, because directory '%s' already exists" % self.sourceTestCasesPath)

        if(status != True):
            return (False,"An error was occurred getting the TestCases from git")



        #** Build Cloud Connector Library from source code
        status = self.compileJavaLibrary(self.sourceCloudConnectorPath)

        if(status != True):
            return (False,"An error was occurred building the source code from Java Library")



        return (True,"Environment for Host Java was successfully created")



    def downloadRepositoryGit(self, urlGit, destinationPath, folderName):
        self.logger.info("Clone Git repository '%s' in '%s'" % (urlGit, destinationPath) )

        result = False

        # Download latest source code from Git
        process = subprocess.Popen(
            ['git', 'clone', '%s' % urlGit, '%s' % folderName],
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

        return result



    def compileJavaLibrary(self, sourceCloudConnectorPath):
        self.logger.info("Building Java Library from source code in '%s' " % sourceCloudConnectorPath)

        #** Copy ant build scripts in source code
        javaLibrariesPath = os.path.abspath(os.path.join('testharness','libraries','java'))

        # Copy build script for JazzLib
        fileName = "build_JazzLib.xml"
        sourceFilePath = os.path.join( javaLibrariesPath, fileName )
        destinationFilePath = os.path.join( self.sourcePath, self.sourceCoreFolderName, "JazzLib", fileName )
        shutil.copy2( sourceFilePath, destinationFilePath )

        # Copy build script for Cloud Connector Core
        fileName = "build_JavaCore.xml"
        sourceFilePath = os.path.join( javaLibrariesPath, fileName )
        destinationFilePath = os.path.join( self.sourcePath, self.sourceCoreFolderName, "CloudConnector_Core", fileName )
        shutil.copy2( sourceFilePath, destinationFilePath )

        # Copy build script for Cloud Connector SE
        fileName = "build_JavaSE.xml"
        sourceFilePath = os.path.join( javaLibrariesPath, fileName )
        destinationFilePath = os.path.join( self.sourcePath, self.sourceCoreFolderName, "CloudConnector_JavaSE", fileName )
        shutil.copy2( sourceFilePath, destinationFilePath )

        # Copy build script for Cloud Connector for PC
        fileName = "build_JavaPC.xml"
        sourceFilePath = os.path.join( javaLibrariesPath, fileName )
        destinationFilePath = os.path.join( self.sourcePath, self.sourceSEFolderName, "general", "CloudConnector_JavaSE_General", fileName )
        shutil.copy2( sourceFilePath, destinationFilePath )

        # Copy shell script to automate the build process
        if ( "linux" in sys.platform ):
            fileName = "generateJarLibrary.sh"
        else:
            fileName = "generateJarLibrary.bat"
        sourceFilePath = os.path.join( javaLibrariesPath, fileName )
        destinationFilePath = os.path.join( self.sourcePath, fileName )
        shutil.copy2( sourceFilePath, destinationFilePath )


        #** Build script to generate the Cloud Connector library
        process = subprocess.Popen(
            [ os.path.join(self.sourcePath,fileName) ],
            cwd=self.sourcePath,
            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

        data = process.communicate()[0]

        if(process.returncode == 0):
            self.logger.info("Cloud Connector for Java Library was successfully compiled in '%s'" % (self.sourcePath) )
            if data is not None:
                self.logger.debug(data)
            return True
        else:
            self.logger.error("An error occurred while compilation process in '%s'" % (self.sourcePath) )
            if data is not None:
                self.logger.debug(data)
            return False




    def setupDevice(self):

        #** Setup Method to generate Device ID
        if(self.inputArguments.deviceid_basedon != None):
            # Set Source
            self.handlerDevice.setDeviceIDMethod(self.inputArguments.deviceid_basedon)
        else:
            return (False,"Missing method to generate Device ID")

        #** Setup Device ID
        if(self.handlerDevice.getDeviceIDMethod() == "mac"):
            #* Based on MAC
            if(self.inputArguments.devicemac != None):
                # Set Device MAC
                self.handlerDevice.setDeviceMAC(self.inputArguments.devicemac)
            else:
                # Generate a random MAC
                self.handlerDevice.generateDeviceMAC()
        elif(self.handlerDevice.getDeviceIDMethod() == "imei"):
            #* Based on IMEI
            if(self.inputArguments.deviceimei != None):
                # Set Device IMEI
                self.handlerDevice.setDeviceIMEI(self.inputArguments.deviceimei)
            else:
                # Generate a random IMEI
                self.handlerDevice.generateDeviceIMEI()
        elif(self.handlerDevice.getDeviceIDMethod() == "esn"):
            #* Based on ESN
            if(self.inputArguments.deviceesn != None):
                # Set Device ESN
                self.handlerDevice.setDeviceESN(self.inputArguments.deviceesn)
            else:
                # Generate a random ESN
                self.handlerDevice.generateDeviceESN()
        elif(self.handlerDevice.getDeviceIDMethod() == "meid"):
            #* Based on MEID
            if(self.inputArguments.devicemeid != None):
                # Set Device MEID
                self.handlerDevice.setDeviceMEID(self.inputArguments.devicemeid)
            else:
                # Generate a random MEID
                self.handlerDevice.generateDeviceMEID()
        elif(self.handlerDevice.getDeviceIDMethod() == "static"):
            #* Based on STATIC
            if(self.inputArguments.devicestatic != None):
                # Set Device STATIC
                self.handlerDevice.setDeviceID(self.inputArguments.devicestatic)
            else:
                # Generate a random based by default on MAC
                self.handlerDevice.generateDeviceMAC()


        #** Setup Device Type
        if(self.inputArguments.devicetype != None):
            # Set Device Type
            self.handlerDevice.setDeviceType(self.inputArguments.devicetype)
        else:
            # Generate a random Device Type
            self.handlerDevice.generateDeviceType()


        return (True,"Device was configured")


    def resetDevice(self):
        #* Create handlerDevice
        status = self.getHandlerDevice()
        if(status[0] != True):
            return (False,"An error was occurred getting the handler Device: %s" % status[1])
        else:
            self.handlerDevice = status[1]

            # Setup handlerDevice
            status = self.setupDevice()
            if(status[0] != True):
                return (False,"An error was occurred setting the handler Device: %s" % status[1])
            else:
                return (True,"Environment for Host was successfully created")



    def createSourceProject(self, sourceGroupPath,testType,testGroup):

        #** Test Cases with the standard structure
        # Source path
        sourceCodePath = os.path.join(sourceGroupPath,"source","java")


        # Create sandbox for this group
        if(self.createSandbox() != True):
            return (False,"Error creating temporary directory to store the project")


        #** Test Cases with the standard structure
        # Copy all files needed for the project into sandbox
        if(self.copySourceAppToSandbox(sourceCodePath) != True):
            return (False,"Error copying project source code to sandbox")


        #** Include the latest Cloud Connector Library
        # Copy jar library for the project into sandbox
        if(self.copyCCLibraryToSandbox() != True):
            return (False,"Error copying Cloud Connector library to project path on sandbox")



        ## Destination Folder
        #self.sourceProjectPath = os.path.join(self.sandboxPath,"app") #ej. "/tmp/tmp03ihHdsd/app")


        #projectCodePath = "/home/satest/TestHarness/java"
        #shutil.copytree(projectCodePath, self.sourceProjectPath, symlinks=True)
        #self.logger.info("Copying java project from '%s' to '%s'" % (projectCodePath,self.sourceProjectPath))


        # Set Device with the Cloud Connector path
        self.handlerDevice.sourceProjectPath = self.sourceProjectPath

        return(True, "Source project was created successfully")



    def copySourceAppToSandbox(self,testGroupPath):
        # Copy source code for the project into the sandbox

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



    def copyCCLibraryToSandbox(self):
        self.logger.info("Copy Cloud Connector for Java library to source project directory")

        # Verify if Library folder is an existing directory
        libraryPath = os.path.join(self.sandboxPath,"app","lib")
        if (not os.path.exists(libraryPath)):
            os.makedirs(libraryPath)

        # Origin Folder path
        sourceFilePath = os.path.join( self.sourcePath, self.sourceSEFolderName, "general", "CloudConnector_JavaSE_General", "dist", "CloudConnectorPC.jar" )

        # Destination Folder
        destinationFilePath = os.path.join(self.sandboxPath,"app","lib","CloudConnector.jar") #ej. "/tmp/tmp03ihHdsd/app/libs/xxxx.jar")

        shutil.copy2( sourceFilePath, destinationFilePath )


        return True



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




    def buildSourceProject(self):

        #* Setup the conector_config.txt
        self.setupLibrary("connector_config.txt")

        #* Build project code
        result,details = self.compileProjectCode(self.sourceProjectPath)

        if(not result):
            return (False,"Compilation process failed: %s" % details)

        #* Clean the sandbox TODO

        return (True,"Cloud Connector Project was successfully compiled")


    def setupLibrary(self, filename):

        if(filename == "connector_config.txt"):
            filePath = os.path.join(self.sourceProjectPath,filename)
            # Verify if this file exists
            if(os.path.isfile(filePath)):
                # Setup public/{run,step}/platforms/linux_template/config.c
                configFile = ConfigFile_java.ConfigFile_java(filePath)
                # Read original file
                configFile.readFile()

                # Set Device ID type
                configFile.setupDeviceIDType( self.handlerDevice.getDeviceIDMethod() )

                # Set Device ID
                if(self.handlerDevice.getDeviceIDMethod() == "static"):
                    # Add Device ID
                    configFile.setupDeviceID(self.handlerDevice.getDeviceID())

                # Add Vendor ID
                configFile.setupVendorID(self.inputArguments.vendorid)
                # Add Cloud Server
                configFile.setupCloudServer(self.inputArguments.server)
                # Add Device Type
                configFile.setupDeviceType(self.handlerDevice.getDeviceType())

                # Modify file with custom configuration
                configFile.setupFile()
            else:
                self.logger.warning("Skip configuration for file '%s' because does not exists in '%s'" % (filename,filePath))







    def compileProjectCode(self, sourcePath):
        self.logger.info("Building Cloud Connector Project from '%s' " % sourcePath)

        #** Execute ant process
        # Build options path inside Cloud Connector code
        buildOptionsPath='build.xml'
        # Generate compilation command line
        commandLine = ['ant','-f',buildOptionsPath]

        self.logger.info("Compilation command line: %s " % " ".join(commandLine))

        # Execute compilation command line
        process = subprocess.Popen(
            commandLine,
            cwd=sourcePath,
            shell=True,
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



    def flashSourceProject(self):
        self.logger.info("Skip flash process to Cloud Connector for JAVA")



    def launchCloudConnectorOnDevice(self):
        self.logger.info("Start Cloud Connector for JAVA on the PC")
        # Reset Device flags
        self.handlerDevice.resetFlags()
        # Launch thread monitoring Device
        self.handlerDevice.start()

        # Wait or verify that device is started
        time.sleep(10)
        return True




    def stopCloudConnectorOnDevice(self):
        self.logger.info("Stop Cloud Connector for JAVA on the PC")

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
            self.logger.info("Cloud Connector for JAVA was stopped successfully")
            result = True
        else:
            self.logger.error("Cloud Connector for JAVA was not stopped due to a timeout")
            result = False


        return result
