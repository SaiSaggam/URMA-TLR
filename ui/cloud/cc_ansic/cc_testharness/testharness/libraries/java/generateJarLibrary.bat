@echo .
@echo .
@echo #########################################################
@echo ######### Cloud Connector for Java build system #########
@echo #########################################################
@echo

REM #### Step 1: build the JazzLib library
@echo ### Step 1: Compile the Jazzlib library...
cmd /c "ant -f %~dp0/cc_java_core/JazzLib/build_JazzLib.xml"
IF %ERRORLEVEL% EQU 1 GOTO Error


REM #### Step 2: build the Cloud Connector for Java Core
@echo .
@echo ### Step 2: Compile the Cloud Connector for Java Core...
cmd /c "ant -f %~dp0/cc_java_core/CloudConnector_Core/build_JavaCore.xml"
IF %ERRORLEVEL% EQU 1 GOTO Error

REM #### Step 3: build the Cloud Connector for Java Core
@echo .
@echo ### Step 3: Compile the Cloud Connector for Java SE...
cmd /c "ant -f %~dp0/cc_java_core/CloudConnector_JavaSE/build_JavaSE.xml"
IF %ERRORLEVEL% EQU 1 GOTO Error

REM #### Step 4: build the Cloud Connector for Java Core
@echo .
@echo ### Step 4: Compile the Cloud Connector for PC implementation...
cmd /c "ant -f %~dp0/cc_java_se/general/CloudConnector_JavaSE_General/build_JavaPC.xml"
IF %ERRORLEVEL% EQU 1 GOTO Error



REM #### Build system process finished
REM ###############################################################################
@echo .
@echo .
@echo ##########################################################################
@echo ### Cloud Connector for Java build system was successfully finished!!! ###
@echo ##########################################################################
@echo .
@echo Now you can find the Cloud Connector for Java PC Implementation typing:
@echo .
@echo "dir %~dp0\cc_java_se\general\CloudConnector_JavaSE_General\dist"
@echo .
@echo .
exit 0

:Error
exit 1
