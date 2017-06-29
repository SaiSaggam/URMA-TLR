#!/bin/bash
echo ""
echo ""
echo "############################################################"
echo "######### Cloud Connector Test Framework installer #########"
echo "############################################################"
echo ""

STEPCOUNTER="0"
COLORRED="\033[0;31m"
COLORGREEN="\033[0;32m"
COLORESCAPE="\033[0m"

listPackages="gcc lcov libssl-dev zlib1g-dev ant python python-dev python-setuptools git"

function increaseStepNumber {
    STEPCOUNTER=$[$STEPCOUNTER+1]
    return $STEPCOUNTER
}



#### CHECK / INSTALL REQUIRED PACKAGES
###############################################################################
increaseStepNumber
echo "### Step $STEPCOUNTER: Check installed packages"

listPackagesToInstall=""
result="False"

for eachPackage in $listPackages; do

    PKG_OK=$(dpkg-query -W --showformat='${Status}\n' $eachPackage 2>&1|grep "install ok installed" )

    if [ "" == "$PKG_OK" ]; then
        echo -e "\t$COLORRED Package '$eachPackage' is currently NOT INSTALLED.$COLORESCAPE"
        listPackagesToInstall="$listPackagesToInstall $eachPackage"
    #   sudo apt-get --force-yes --yes install the.package.name
    else
        echo -e "\t Package '$eachPackage' is currently installed"
    fi


    if [ "" == "$listPackagesToInstall" ]; then
        result="True"
    else
        result="False"
    fi

done

if [ "False" == $result ]; then
    echo ""
    echo "Instaler can not continue until are installed these packages."
    echo "You can install it by typing:"
    echo "sudo apt-get install$listPackagesToInstall"
    echo ""
    exit 1
else
    echo ""
    echo -e "\t$COLORGREEN All required packages are successfully installed in your system.$COLORESCAPE"
fi



#### UPDATE SETUPTOOLS PACKAGE
###############################################################################
increaseStepNumber
echo ""
echo ""
echo "### Step $STEPCOUNTER: Update setuptools package to use the bootstrap script."
sudo easy_install -U setuptools
result=$(echo $?)

if [ "0" == "$result" ]; then
    echo ""
    echo -e "\t$COLORGREEN Setuptools was successfully updated.$COLORESCAPE"
else
    echo ""
    echo -e "\t$COLORRED Setuptools update process could not be finished successfully.$COLORESCAPE"
    echo ""
    echo -e "\t ### Try to update setuptools using an alternative package manager..."
    echo ""
    echo -e "\t ### Installing package pip:"
    sudo easy_install pip
    echo ""
    echo -e "\t ### Updating distribute version:"
    sudo pip install -U distribute
    echo ""
    echo -e "\t ### Trying to update the setuptools package again:"
    sudo easy_install -U setuptools

    result=$(echo $?)
    if [ "0" == "$result" ]; then
        echo ""
        echo -e "\t$COLORGREEN Setuptools was successfully updated.$COLORESCAPE"
    else
        echo ""
        echo -e "\t$COLORRED Setuptools update process could not be finished successfully.$COLORESCAPE"
        echo -e "\t$COLORRED Try to update it manually.$COLORESCAPE"
        exit 1
    fi
fi





#### SETUP PYTHON BUILD SYSTEM
###############################################################################
increaseStepNumber
echo ""
echo ""
echo "### Step $STEPCOUNTER: Setup Python build system using the bootstrap script."
cd python
python bootstrap.py
result=$(echo $?)

if [ "0" == "$result" ]; then
    echo ""
    echo -e "\t$COLORGREEN Python build system successfully configured.$COLORESCAPE"
else
    echo ""
    echo -e "\t$COLORRED Python build system could not be finished successfully.$COLORESCAPE"
    exit 1
fi



#### RUN INSTALLATION PROCESS FOR BUILDOUT SYSTEM
###############################################################################
increaseStepNumber
echo ""
echo ""
echo "### Step $STEPCOUNTER: Run the buildout install system. This will retrieve all dependency libraries and install it."
bin/buildout install
result=$(echo $?)

if [ "0" == "$result" ]; then
    echo ""
    echo -e "\t$COLORGREEN All libraries and packages were successfully installed.$COLORESCAPE"
else
    echo ""
    echo -e "\t$COLORRED Buildout installation system could not be finished successfully.$COLORESCAPE"
    exit 1
fi


#### Instaler process finished
###############################################################################
echo ""
echo ""
echo "################################################################################"
echo "### Cloud Connector Test Framework installation was successfully finished!!! ###"
echo "################################################################################"
echo ""
echo "Now you can start the Cloud Connector Test Framework typing:"
echo ""
echo "./python/bin/python cc_test.py --help"
echo ""
echo ""
