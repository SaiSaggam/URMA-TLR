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
# Cloud Connector test framework - Host ANSI-C class
#

################ Import Common libraries
import tempfile     ### This module generates temporary files and directories
import logging      ### Functions and classes to implement a event logging system
import sys          ### Access to variables or functions maintained by the interpreter
import os           ### Some useful functions on pathnames
import subprocess   ### Allows to spawn new processes, connect to their input/output/error pipes, and obtain their return codes
import time         ### Provides various time-related functions
import shutil       ### Functions to do high-level operations on files and collections of files

################ Import Cloud Connector libraries for ANSI C
sys.path.insert(0, os.path.abspath(os.path.join('testharness','libraries','ansic')))

libs = ["Host_linux","ConfigFile_ansic","CloudConfigHeaderFile_ansic","platformHeaderFile_ansic","Makefile_ansic","MainFile_ansic","NetworkSmsProxyFile_ansic"]

for elem in libs:
    exec("import %s" % elem)
    exec("reload (%s)" % elem)


class Host_ansic(Host_linux.Host_linux):
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
    gitRepositoryURL = "ssh://git@stash.digi.com/cc/cc_ansic.git"




    def __init__(self):
        ## Call to __init__ from Host(): Initialize function
        super(Host_ansic, self).__init__()

        ################ Environment
        self.identifier = "Host_ansic"
        self.rootPath = os.path.abspath(".") #ej. "/home/username/TestFolder"
        self.sourcePath = os.path.join(self.rootPath,"source") #ej. "/home/username/TestFolder/source"
        self.sourceFolderName = "cc_ansic"
        self.sourceCloudConnectorPath = os.path.join(self.sourcePath,self.sourceFolderName)

        ################ Logging
        self.logger = logging.getLogger('cc.host.ansic')




    def createEnvironment(self):
        self.logger.info("Creating environment for Host ANSI C")

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
                icTestCasesSourcePath = os.path.join(self.sourceCloudConnectorPath,"cc_testcases") #ej. "/home/username/TestFolder/source/cc_ansic/cc_testcases"
                if os.path.exists(icTestCasesSourcePath):
                    try:
                        shutil.copytree(icTestCasesSourcePath, self.sourceTestCasesPath)
                        _msg = "Cloud Connector Test cases copied in '%s'" % self.sourceTestCasesPath
                        self.logger.info(_msg)
                        status = True
                    except OSError,e:
                        self.logger.exception('Exception caught: %s' % type(e))
                        _msg = "Exception [OSError]: %s" % (str(e))
                        status = False
                else:
                    self.logger.error("Missing cc_testcases directory in '%s'" % icTestCasesSourcePath)
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
        # Generate a new Device ID until it is available
        isDeviceIDAvailable = False
        timeout = time.time() + 60 # We provide a timeout

        while ( not isDeviceIDAvailable and time.time() < timeout ):
            # Define like available for all cases
            isDeviceIDAvailable = True

            if(self.handlerDevice.getDeviceIDMethod() == "mac"):
                ## Based on MAC
                if(self.inputArguments.devicemac != None):
                    # Set Device MAC
                    self.handlerDevice.setDeviceMAC(self.inputArguments.devicemac)
                else:
                    # Generate a random MAC
                    result,details = self.handlerDevice.generateDeviceMAC()
                    if ( not result ):
                        return(False,details)

                    # Verify if the Device ID based on the generated MAC is available
                    if ( self.handlerDeviceCloud.isDeviceRegistered( self.handlerDevice.getDeviceID() ) ):
                        self.logger.warning("Generated Device ID '%s' is already being used" % self.handlerDevice.getDeviceID() )
                        # Modify flag to generate another Device ID
                        isDeviceIDAvailable = False


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
        result,details = self.getHandlerDevice()
        if( not result ):
            return (False,"An error was occurred getting the handler Device: %s" % details)
        else:
            self.handlerDevice = details

            ################ Setup handlerDevice
            result,details = self.setupDevice()
            if( not result ):
                return (False,"An error was occurred setting the handler Device: %s" % details)
            else:
                return (True,"Environment for Host was successfully created")



    def downloadRepositoryGit(self, urlGit, destinationPath, folderName):
        self.logger.info("Clone Git repository '%s' in '%s'" % (urlGit, destinationPath) )

        result = False

        ################ Download latest source code from Git
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
        buildOptionsPath='tools/config/build.xml'

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
        #makeFile = Makefile_ansic.Makefile_ansic(mkTemplatePath=self.sourceCloudConnectorPath, libType=libType, sType=testType, sGroup=groupName)
        makeFile = Makefile_ansic.Makefile_ansic(mkTemplatePath=None, libType=libType, sType=testType, sGroup=groupName)
        # Enable Coverage support
        if(self.inputArguments.gcov):
            makeFile.setCoverageSupport(True)
        # Set TCP/SSL support for this sample project
        makeFile.setSSLSupport(self.inputArguments.ansic_enable_ssl)
        # Set Default compiler for each architecture
        makeFile.setCompiler(self.inputArguments.architecture)
        # Set Platform to compile
        makeFile.setPlatform(self.inputArguments.ansic_platform)
        # Read Makefile template file
        makeFile.readTemplateFile()
        # Generate Makefile for this sample project
        makeFile.generateMakefile(sourceCloudConnectorPath)
        # Write Makefile in output path
        makeFile.writeDestinationFile()

        return True


    def generateMakefileSample(self, sourceSamplePath):
        # Params:
        # sourceSamplePath : #ej. "/tmp/tmpBx5Ynf/public/run/samples/connect_to_device_cloud"
        self.logger.info("Building Makefile for Sample source code in '%s' " % sourceSamplePath)

        # External tool path
        makegenPath = os.path.join(self.sourceCloudConnectorPath,"dvt","scripts")

        process = subprocess.Popen(
            ['python','makegen.py', sourceSamplePath],
            cwd=makegenPath,
            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

        data = process.communicate()[0]

        if(process.returncode == 0):
            self.logger.info("Make Generator tool was successfully executed in '%s'" % (makegenPath) )
            if data is not None:
                self.logger.debug(data)
            #return True
        else:
            self.logger.error("An error occurred executing Make generator in '%s'" % (makegenPath) )
            if data is not None:
                self.logger.debug(data)
            #return False


        ###################################################
        ############ WORKAROUND FOR MAKEGEN.py ############
        if(process.returncode == 0):
            # Modify Compiler for Makefile if architecture is ARM
            if(self.inputArguments.architecture == "ARM"):
                # Open Makefile
                self.logger.warning("Workaround for all Makefiles from Cloud Connector Samples to allow ARM compiler")
                makefilePath = os.path.abspath(os.path.join(sourceSamplePath,"Makefile"))
                self.logger.warning("Read Makefile file from '%s'" % makefilePath)
                handlerFile = open(makefilePath, "r")
                makefileContent = handlerFile.read()
                handlerFile.close()
                # Modify Compiler
                gccLine = "CC = gcc"
                armLine = "CC = arm-linux-gcc"
                self.logger.warning("Replace on Makefile '%s' for '%s'" % (gccLine,armLine) )
                makefileContent = makefileContent.replace(gccLine, armLine)
                # Save new Content
                self.logger.warning("Save new content on Makefile")
                handlerFile = open(makefilePath, "w")
                handlerFile.write(makefileContent)
                handlerFile.close()
            return True
        else:
            return False



        ############ WORKAROUND FOR MAKEGEN.py ############
        ###################################################



    def createSourceProject(self, sourceGroupPath,testType,testGroup):

        if(testType == "samples"):
            ################ Sample tests
            # Source path
            sourceCodePath = os.path.join(self.sourceCloudConnectorPath,"public",
                                          self.inputArguments.ansic_type,"samples",
                                          testGroup)
        else:
            ################ Test Cases with the standard structure
            # Source path
            sourceCodePath = os.path.join(sourceGroupPath,"source","ansic")


        # Create sandbox for this group
        if(self.createSandbox() != True):
            return (False,"Error creating temporary directory to store the project")

        # Copy all common files needed into sandbox
        if(self.copySourceToSandbox() != True):
            return (False,"Error copying source code to sandbox")

        # Copy all files for the proyect
        if(testType == "samples"):
            ################ Sample tests
            # Copy all files needed for the sample project into sandbox
            if(self.copySourceSampleToSandbox(sourceCodePath,testGroup) != True):
                return (False,"Error copying sample project source code to sandbox")
        else:
            ################ Test Cases with the standard structure
            # Copy all files needed for the project into sandbox
            if(self.copySourceAppToSandbox(sourceCodePath) != True):
                return (False,"Error copying project source code to sandbox")

        # Verify if exist a Makefile
        if(not os.path.isfile(os.path.join(self.sourceProjectPath,"Makefile"))):
            if(testType == "samples"):
                ################ Sample tests
                # Generate Makefile with standard tool "makegen.py"
                #if(self.generateMakefileSample(self.sourceProjectPath) != True):

                # Generate Makefile with new classes
                if(self.generateMakefileApp(self.sourceProjectPath,testType,testGroup) != True):
                    return (False,"Error generating Makefile for sample project source code in sandbox")
            else:
                ################ Test Cases with the standard structure
                # Generate Makefile
                if(self.generateMakefileApp(self.sourceProjectPath,testType,testGroup) != True):
                    return (False,"Error generating Makefile for project source code in sandbox")
        else:
            self.logger.info("Skip Makefile generation because already exists in '%s'" % self.sourceProjectPath)


        # To support coverage in the source code we need to inject some code in main.c
        if(self.inputArguments.gcov):
            if(not self.addCoverageSupport(self.sourceProjectPath,testType,testGroup) ):
                return (False,"Error adding support to coverage in source code in sandbox")


        # Set Device with the Cloud Connector path
        self.handlerDevice.sourceProjectPath = self.sourceProjectPath

        return(True, "Source project was created successfully")


    def addCoverageSupport(self,sourcePath,testType,testGroup):

        # Get path for main.c
        mainFilePath = None
        if( os.path.isfile(os.path.join(sourcePath,"main.c")) ):
             mainFilePath = os.path.join(sourcePath,"main.c")
        else:
            if(testType == "samples"):
                mainFilePath = os.path.join(sourcePath,"..","..","platforms",self.inputArguments.ansic_platform,"main.c")
                ################ Sample tests
                # Generate Makefile with standard tool
                #if(self.generateMakefileSample(self.sourceProjectPath) != True):
                    #return (False,"Error generating Makefile for sample project source code in sandbox")
            else:
                mainFilePath = os.path.join(sourcePath,"..","public",self.inputArguments.ansic_type,"platforms",self.inputArguments.ansic_platform,"main.c")
                ################ Test Cases with the standard structure
                # Generate Makefile
                #if(self.generateMakefileApp(self.sourceProjectPath,testType,testGroup) != True):
                    #return (False,"Error generating Makefile for project source code in sandbox")

        if( os.path.isfile(mainFilePath) ):
            self.logger.info("main.c files exist in '%s'" % mainFilePath)

            # Setup app/main.c or platforms/linux_template/main.c
            mainFile = MainFile_ansic.MainFile_ansic(mainFilePath)
            # Read original file
            mainFile.readFile()
            # Set Includes to support coverage
            mainFile.setupCoverageIncludes()
            # Add functions to support coverage
            mainFile.setupCoverageFunctions()
            # Add signal function in main function
            mainFile.setupCoverageSignal()

            # Modify file with custom configuration
            mainFile.setupFile()
        else:
            self.logger.error("main.c files does not exist in '%s'" % mainFilePath)
            return False

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


    def copySourceToSandbox(self):
        ################ Source code base (Libraries, common files, etc.)

        # Copy all source to sandbox
        shutil.copytree(self.sourceCloudConnectorPath, self.sandboxPath, symlinks=True)

        # Remove all no necessary files
        listFilesRemove = [ os.path.join(self.sandboxPath,"hudson.sh"),
                            os.path.join(self.sandboxPath,"build_release.sh"),
                            os.path.join(self.sandboxPath,"test.py"),
                            os.path.join(self.sandboxPath,"startup.sh"),
                            os.path.join(self.sandboxPath,"reboot.py"),
                            os.path.join(self.sandboxPath,"Getting Started.docx"),
                            os.path.join(self.sandboxPath,"Makefile"),
                            os.path.join(self.sandboxPath,"Makefile.template"),
                            os.path.join(self.sandboxPath,"validate.dict"),
                            os.path.join(self.sandboxPath,"validate.py"),
                            os.path.join(self.sandboxPath,"replace_all.sh"),
                            os.path.join(self.sandboxPath,".gitignore"),
                            ]
        for item in listFilesRemove:
            if(os.path.isfile(item)):
                self.logger.info("Removing file from sandbox: '%s'" % item)
                os.remove(item)

        # Remove all no necessary directories
        listDirRemove = [ os.path.join(self.sandboxPath,"coverity"),
                            os.path.join(self.sandboxPath,"doxygen"),
                            os.path.join(self.sandboxPath,"dvt"),
                            os.path.join(self.sandboxPath,"kits"),
                            os.path.join(self.sandboxPath,"tools"),
                            os.path.join(self.sandboxPath,"cc_testcases"),
                            os.path.join(self.sandboxPath,".git"),
                            os.path.join(self.sandboxPath,"public",self.inputArguments.ansic_type,"samples"),
                            os.path.join(self.sandboxPath,"public",self.inputArguments.ansic_type,"platforms","freescale"),
                            os.path.join(self.sandboxPath,"public",self.inputArguments.ansic_type,"platforms","windows"),
                            os.path.join(self.sandboxPath,"public",self.inputArguments.ansic_type,"platforms","mqx"),
                            os.path.join(self.sandboxPath,"public",self.inputArguments.ansic_type,"platforms","ucos"),
                            ]

        # Remove non-selected platform folder
        if ( self.inputArguments.ansic_platform == "linux" ):
            listDirRemove.append(os.path.join(self.sandboxPath,"public",self.inputArguments.ansic_type,"platforms","template"))
        elif ( self.inputArguments.ansic_platform == "template" ):
            listDirRemove.append(os.path.join(self.sandboxPath,"public",self.inputArguments.ansic_type,"platforms","linux"))

        # Check if the test uses the run or step mechanism:
        if(self.inputArguments.ansic_type == "run"):
            listDirRemove.append(os.path.join(self.sandboxPath,"public","step"))
        elif(self.inputArguments.ansic_type == "step"):
            listDirRemove.append(os.path.join(self.sandboxPath,"public","run"))


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



    def copySourceSampleToSandbox(self,testGroupPath,testGroup):
        ################ Copy source code for the sample into the sandbox

        # Destination Folder
        self.sourceProjectPath = os.path.join(self.sandboxPath,"public",
                                              self.inputArguments.ansic_type,"samples",
                                              testGroup) #ej. "/tmp/tmp03ihHdsd/public/run/samples/connect_to_device_cloud")

        # Origin Folder
        projectCodePath = testGroupPath #ej. "/home/username/TestFolder/source/ic_ansic/public/run/samples/connect_to_device_cloud")

        # Verify if Origin folder is an existing directory
        if(os.path.isdir(projectCodePath)):
            shutil.copytree(projectCodePath, self.sourceProjectPath, symlinks=True)
            self.logger.info("Copying sample project directory from '%s' to '%s'" % (projectCodePath,self.sourceProjectPath))

            return True
        else:
            self.logger.error("Source sample directory '%s' does not exists" % projectCodePath)
            return False


    def buildSourceProject(self):

        # Setup "config.c"
        self.setupLibrary("config.c")

        # Setup "connector_config.h"
        self.setupLibrary("connector_config.h")

        # Setup "platform.h"
        self.setupLibrary("platform.h")

        # Setup "network_sms_proxy.c_aux"
        if ( self.inputArguments.ansic_sms_proxy_ip is not None ):
            self.setupLibrary("network_sms_proxy.c_aux")

        # Build RCI code
        status = self.compileRCICode(self.sourceCloudConnectorPath, self.sourceProjectPath)

        if( not status[0]):
            return (False,"RCI Compilation process failed: %s" % status[1])

        # Build project code
        status = self.compileProjectCode(self.sourceProjectPath)

        if(status[0]):
            return (True,"Cloud Connector Project was successfully compiled")
        else:
            return (False,"Compilation process failed: %s" % status[1])




    def compileRCICode(self, cloudConnectorPath, sourcePath):
        self.logger.info("Building RCI for Cloud Connector Project from '%s' " % sourcePath)

        # Search in the source folder all *.rci files
        listRCIConfigs = [ f for f in os.listdir(sourcePath) if f.endswith('.rci') ]


        # If this sample don't have RCI we skip the process
        if not len(listRCIConfigs) > 0:
            return (True, 'Any RCI file was found in the path')

        # If we have more RCI files, we used only the first
        rciConfigfile = os.path.join(sourcePath, listRCIConfigs[0])

        # Build ConfigGenerator path
        configGeneratorPath = os.path.join(cloudConnectorPath,"tools","config","build","ConfigGenerator.jar")


        self.logger.info("Generating RCI Config Code for '%s' " % rciConfigfile)

        ### Execute ConfigGenerator process
        # Generate command line
        commandLine = [ "java",
                        "-jar",
                        configGeneratorPath,
                        "-path=%s" % sourcePath,
                        "-url=%s" % self.inputArguments.server,
                        "%s:%s" % (self.inputArguments.username,self.inputArguments.password),
                        self.handlerDevice.getDeviceType(),
                        "%s" % self.handlerDevice.getDeviceFirmwareVersion(),
                        rciConfigfile
                        ]

        self.logger.info("Compilation command line: %s " % " ".join(commandLine))

        # Execute compilation command line
        process = subprocess.Popen(
            commandLine,
            cwd=sourcePath,
            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

        data = process.communicate()[0]


        if(process.returncode == 0):
            self.logger.info("RCI was successfully compiled in '%s'" % (sourcePath) )
            if data is not None:
                self.logger.debug(data)
            return (True,"RCI Compilation process was successful")
        else:
            self.logger.error("An error occurred while RCI compilation process in '%s'" % (sourcePath) )
            if data is not None:
                self.logger.debug(data)
            return (False,data)



    def compileProjectCode(self, sourcePath):
        self.logger.info("Building Cloud Connector Project from '%s' " % sourcePath)

        ### Create a external flags rules
        # Set architecture
        cflags = None
        if self.inputArguments.architecture == 'x64':
            cflags='DFLAGS=-m64 '
        elif self.inputArguments.architecture == 'i386':
            cflags='DFLAGS=-m32 '
        elif self.inputArguments.architecture == 'ARM':
            cflags = None
        else:
            cflags='DFLAGS=-m%s ' % self.inputArguments.architecture

        # Set Coverage flags
        if(self.inputArguments.gcov):
            cflags += '-g -pg -fprofile-arcs -ftest-coverage '

        # Absolute certificate path
        cert_file_path = os.path.join(self.sandboxPath, "public", "certificates", "Digi_Int-ca-cert-public.crt")
        cflags += '-DAPP_SSL_CA_CERT_PATH=\\"%s\\" ' % (cert_file_path)

        ### Execute make process
        # Generate compilation command line
        commandLine = ['make','clean','all']
        if(cflags != None):
            commandLine.append(cflags)

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

        if(filename == "config.c"):
            filePath = os.path.join(self.sandboxPath,"public",self.inputArguments.ansic_type,"platforms",self.inputArguments.ansic_platform,filename)
            # Verify if this file exists
            if(os.path.isfile(filePath)):
                # Setup public/{run,step}/platforms/linux_template/config.c
                configFile = ConfigFile_ansic.ConfigFile_ansic(filePath)
                # Read original file
                configFile.readFile()
                # Remove error setences
                configFile.removeErrorsFile()

                if(self.handlerDevice.getDeviceIDMethod() == "provision"):
                    configFile.setupDeviceIDMethod("manual")
                else:
                    configFile.setupDeviceIDMethod("auto")
                    # Setup only one item to create Device ID
                    if(self.handlerDevice.getDeviceIDMethod() == "mac"):
                        configFile.setupConnectionType("lan")
                    else:
                        configFile.setupConnectionType("wan")
                        configFile.setupWanType(self.handlerDevice.getDeviceIDMethod())

                    if(self.handlerDevice.getDeviceIDMethod() == "mac"):
                        # Add Device MAC
                        configFile.setupDeviceMAC(self.handlerDevice.getDeviceMAC())
                    elif(self.handlerDevice.getDeviceIDMethod() == "imei"):
                        # Add Device IMEI
                        configFile.setupDeviceIMEI(self.handlerDevice.getDeviceIMEI())
                    elif(self.handlerDevice.getDeviceIDMethod() == "esn"):
                        # Add Device ESN
                        configFile.setupDeviceESN(self.handlerDevice.getDeviceESN())
                    elif(self.handlerDevice.getDeviceIDMethod() == "meid"):
                        # Add Device MEID
                        configFile.setupDeviceMEID(self.handlerDevice.getDeviceMEID())

                # In compilation time, we pass data for this settings in header file instead here
                if(self.inputArguments.ansic_compile_time_data is False):
                    # Add Vendor ID
                    configFile.setupVendorID(self.inputArguments.vendorid)
                    # Add Cloud Server
                    configFile.setupCloudServer(self.inputArguments.server)
                    # Add Device Type
                    configFile.setupDeviceType(self.handlerDevice.getDeviceType())

                    if(self.inputArguments.cloudphone is not None):
                        # Add Cloud Phone Number
                        configFile.setupCloudPhoneNumber(self.inputArguments.cloudphone)

                    if(self.inputArguments.cloudservice_id is not None):
                        # Add Cloud Service-ID
                        configFile.setupCloudServiceID(self.inputArguments.cloudservice_id)

                # Modify file with custom configuration
                configFile.setupFile()
            else:
                self.logger.warning("Skip configuration for file '%s' because does not exists in '%s'" % (filename,filePath))
        elif(filename == "connector_config.h"):
            filePath = os.path.join(self.sourceProjectPath,filename)
            # Verify if this file exists
            if(os.path.isfile(filePath)):
                # Setup app/connector_config.h
                configFile = CloudConfigHeaderFile_ansic.CloudConfigHeaderFile_ansic(filePath)
                # Read original file
                configFile.readFile()
                # Set Defines for the configuration mode
                configFile.setupHeaderDefines(self.inputArguments.ansic_configuration)
                # Set Compression mode
                configFile.setupHeaderCompression(self.inputArguments.ansic_enable_compression)
                # In compilation time, we pass data for this settings in header file instead here
                if(self.inputArguments.ansic_compile_time_data is True):
                    # Set Define to use data passing in compilation time
                    configFile.setupHeaderDataCompileTime()
                    # Set Vendor ID
                    configFile.setupHeaderVendorID(self.inputArguments.vendorid)
                    # Set Cloud Server
                    configFile.setupHeaderCloudServer(self.inputArguments.server)
                    # Set Device Type
                    configFile.setupHeaderDeviceType(self.handlerDevice.getDeviceType())
                    # Set TX Keepalive
                    configFile.setupHeaderTxKeepAlive(self.handlerDevice.getDeviceTxKeepalive())
                    # Set RX Keepalive
                    configFile.setupHeaderRxKeepAlive(self.handlerDevice.getDeviceRxKeepalive())
                # Modify file with custom configuration
                configFile.setupFile()
            else:
                self.logger.warning("Skip configuration for file '%s' because does not exists in '%s'" % (filename,filePath))
        elif(filename == "platform.h"):
            filePath = os.path.join(self.sandboxPath,"public",self.inputArguments.ansic_type,"platforms",self.inputArguments.ansic_platform,filename)
            # Verify if this file exists
            if(os.path.isfile(filePath)):
                # Setup public/{run,step}/platforms/linux_template/platform.h
                configFile = platformHeaderFile_ansic.platformHeaderFile_ansic(filePath)
                # Read original file
                configFile.readFile()
                # Set certificate path
                configFile.setupHeaderCertificate()
                # Modify file with custom configuration
                configFile.setupFile()
            else:
                self.logger.warning("Skip configuration for file '%s' because does not exists in '%s'" % (filename,filePath))
        elif(filename == "network_sms_proxy.c_aux"):
            filePath = os.path.join(self.sandboxPath,"public",self.inputArguments.ansic_type,"platforms",self.inputArguments.ansic_platform,filename)
            # Verify if this file exists
            if(os.path.isfile(filePath)):
                ## Setup public/{run,step}/platforms/linux_template/platform.h
                configFile = NetworkSmsProxyFile_ansic.NetworkSmsProxyFile_ansic(filePath)
                ## Read original file
                configFile.readFile()
                ## Set certificate path
                configFile.setupGatewayIP(self.inputArguments.ansic_sms_proxy_ip)
                ## Modify file with custom configuration
                configFile.setupFile()
            else:
                self.logger.warning("Skip configuration for file '%s' because does not exists in '%s'" % (filename,filePath))


    def flashSourceProject(self):
        self.logger.info("Skip flash process to Cloud Connector for ANSI C")

    def launchCloudConnectorOnDevice(self):
        self.logger.info("Start Cloud Connector for ANSI C on the PC")
        # Reset Device flags
        self.handlerDevice.resetFlags()
        # Launch thread monitoring Device
        self.handlerDevice.start()

        # Wait or verify that device is started
        time.sleep(10)
        return True


    def stopCloudConnectorOnDevice(self):
        self.logger.info("Stop Cloud Connector for ANSI C on the PC")

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
            self.logger.info("Cloud Connector for ANSI C was stopped successfully")
            result = True
        else:
            self.logger.error("Cloud Connector for ANSI C was not stopped due to a timeout")
            result = False


        # If Coverage is enabled we need to collect the coverage info
        if(self.inputArguments.gcov):
            self.generateCoverageResults()

        return result


    def generateCoverageResults(self):

        #*****************#
        #***** GCOVR *****#
        #*****************#
        #appPath = os.path.join(self.sandboxPath,"app")
        #resultFilePath = os.path.join(self.testResultPath,"gcovr_coverage.html")
        #gcovrScriptPath = os.path.join(self.rootPath,"testharness","libraries","gcovr") #ej. "/home/username/TestFolder/source"



        #### Execute the collect results process
        ## Generate command line
        #commandLine = ['python',gcovrScriptPath,'--html','--output',resultFilePath]

        #self.logger.info("Coverage based on GCOVR results command line: %s " % " ".join(commandLine))

        ## Execute command line
        #process = subprocess.Popen(
            #commandLine,
            #cwd=self.sandboxPath,
            #stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

        #data = process.communicate()[0]

        #if(process.returncode == 0):
            #self.logger.info("Coverage based on GCOVR for Cloud Connector Project was successfully collected in '%s'" % (gcovrScriptPath) )
            #if data is not None:
                #self.logger.debug(data)
            ##return (True,"Covergae process was successful")
        #else:
            #self.logger.error("An error occurred while collecting the coverage based on GCOVR in '%s'" % (gcovrScriptPath) )
            #if data is not None:
                #self.logger.debug(data)
            ##return (False,data)



        #*****************#
        #***** LCOV *****#
        #*****************#
        coverageInfoFilePath = os.path.join( self.testResultPath , "lcov_coverage.info" )
        resultFolderPath = os.path.join(self.testResultPath,"coverage")



        ### Execute the collect results process
        # Generate command line
        commandLine = ['lcov','--capture','--directory',self.sandboxPath,'--base-directory',self.sourceProjectPath,'--output-file',coverageInfoFilePath]

        self.logger.info("Coverage based on LCOV command line to collect the results: %s " % " ".join(commandLine))

        # Execute command line
        process = subprocess.Popen(
            commandLine,
            cwd=self.testResultPath,
            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

        data = process.communicate()[0]

        if(process.returncode == 0):
            self.logger.info("Coverage based on LCOV for Cloud Connector Project was successfully collected in '%s'" % (coverageInfoFilePath) )
            if data is not None:
                self.logger.debug(data)
            #return (True,"Covergae process was successful")
        else:
            self.logger.error("An error occurred while collecting the coverage based on LCOV in '%s'" % (coverageInfoFilePath) )
            if data is not None:
                self.logger.debug(data)
            #return (False,data)


        ### Execute the exclude process from the results
        # Generate command line
        commandLine = ['lcov','--remove',coverageInfoFilePath, '/usr*', '--output-file', coverageInfoFilePath]

        self.logger.info("Coverage based on LCOV command line to exclude user libraries from the results: %s " % " ".join(commandLine))

        # Execute command line
        process = subprocess.Popen(
            commandLine,
            cwd=self.testResultPath,
            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

        data = process.communicate()[0]

        if(process.returncode == 0):
            self.logger.info("User libraries were successfully excluded from '%s'" % (coverageInfoFilePath) )
            if data is not None:
                self.logger.debug(data)
            #return (True,"Covergae process was successful")
        else:
            self.logger.error("An error occurred while excluding the user libraries from '%s'" % (coverageInfoFilePath) )
            if data is not None:
                self.logger.debug(data)
            #return (False,data)


        ## Execute the generation of the results process
        # Generate command line
        commandLine = ['genhtml',coverageInfoFilePath,'--output-directory',resultFolderPath]

        self.logger.info("Coverage based on LCOV command line to generate the HTML results: %s " % " ".join(commandLine))

        # Execute command line
        process = subprocess.Popen(
            commandLine,
            cwd=self.testResultPath,
            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

        data = process.communicate()[0]

        if(process.returncode == 0):
            self.logger.info("HTML Coverage based on LCOV for Cloud Connector Project was successfully generated" )
            if data is not None:
                self.logger.debug(data)
            return (True,"Covergae process was successful")
        else:
            self.logger.error("An error occurred while generating the HTML coverage based on LCOV" )
            if data is not None:
                self.logger.debug(data)
            return (False,data)
