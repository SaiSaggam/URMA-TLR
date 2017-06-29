#!/bin/sh
echo ""
echo ""
echo "#########################################################"
echo "######### Cloud Connector for Java build system #########"
echo "#########################################################"
echo ""


#### Step 1: build the JazzLib library
echo "### Step 1: Compile the Jazzlib library..."
CommandOutput=$(ant -f ./cc_java_core/JazzLib/build_JazzLib.xml)
result=$(echo $?)

if [ $result -eq 0 ]; then
    echo ""
    echo "\t JazzLib library was compiled successfully."
else
    echo ""
    echo "\t JazzLib build system could not be finished successfully."
    echo "$CommandOutput"
    exit 1
fi




#### Step 2: build the Cloud Connector for Java Core
echo ""
echo "### Step 2: Compile the Cloud Connector for Java Core..."
CommandOutput=$(ant -f ./cc_java_core/CloudConnector_Core/build_JavaCore.xml)
result=$(echo $?)

if [ $result -eq 0 ]; then
    echo ""
    echo "\t Cloud Connector for Java Core was compiled successfully."
else
    echo ""
    echo "\t Cloud Connector for Java Core build system could not be finished successfully."
    echo "$CommandOutput"
    exit 1
fi




#### Step 3: build the Cloud Connector for Java Core
echo ""
echo "### Step 3: Compile the Cloud Connector for Java SE..."
CommandOutput=$(ant -f ./cc_java_core/CloudConnector_JavaSE/build_JavaSE.xml)
result=$(echo $?)

if [ $result -eq 0 ]; then
    echo ""
    echo "\t Cloud Connector for Java SE was compiled successfully."
else
    echo ""
    echo "\t Cloud Connector for Java SE build system could not be finished successfully."
    echo "$CommandOutput"
    exit 1
fi




#### Step 4: build the Cloud Connector for Java Core
echo ""
echo "### Step 4: Compile the Cloud Connector for PC implementation..."
CommandOutput=$(ant -f ./cc_java_se/general/CloudConnector_JavaSE_General/build_JavaPC.xml)
result=$(echo $?)

if [ $result -eq 0 ]; then
    echo ""
    echo "\t Cloud Connector for Java for PC implementation was compiled successfully."
else
    echo ""
    echo "\t Cloud Connector for Java for PC implementation build system could not be finished successfully."
    echo "$CommandOutput"
    exit 1
fi




#### Build system process finished
###############################################################################
echo ""
echo ""
echo "##########################################################################"
echo "### Cloud Connector for Java build system was successfully finished!!! ###"
echo "##########################################################################"
echo ""
echo "Now you can find the Cloud Connector for Java PC Implementation typing:"
echo ""
echo "ls -la ./cc_java_se/general/CloudConnector_JavaSE_General/dist"
echo ""
echo ""