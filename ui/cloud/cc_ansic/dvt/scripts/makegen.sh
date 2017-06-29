#!/bin/bash
echo ""
echo ""
echo "######################################################"
echo "######### Cloud Connector Makefile Generator #########"
echo "######################################################"
echo ""

COLORRED="\033[0;31m"
COLORGREEN="\033[0;32m"
COLORESCAPE="\033[0m"



echo "Create a new folder called 'makeGenerator' to donwload all nedeed files"
mkdir makeGenerator

cd makeGenerator

echo ">>> Download the python class to manage the Makefile"
git archive --remote=ssh://git@stash.digi.com/cc/cc_testharness.git HEAD testharness/libraries/ansic/Makefile_ansic.py | tar -x --strip=3
echo ">>> Download the Makefile template"
git archive --remote=ssh://git@stash.digi.com/cc/cc_testharness.git HEAD testharness/libraries/ansic/Makefile_ansic.template | tar -x --strip=3
echo ">>> Download the Makefile Generator script"
git archive --remote=ssh://git@stash.digi.com/cc/cc_testharness.git HEAD testharness/libraries/ansic/MakefileSamplesGenerator.py | tar -x --strip=3


# Check we have all required files
if [ -f "Makefile_ansic.py" ]
then
    if [ -f "Makefile_ansic.template" ]
    then
        if [ -f "MakefileSamplesGenerator.py" ]
        then
            echo "    Successfully downloaded the required files!!"

        else
            echo -e "$COLORRED    Cannot find 'MakefileSamplesGenerator.py' file$COLORESCAPE"
            echo ""
            exit 1
        fi

    else
        echo -e "$COLORRED    Cannot find 'Makefile_ansic.template' file$COLORESCAPE"
        echo ""
        exit 1
    fi

else
    echo -e "$COLORRED    Cannot find 'Makefile_ansic.py' file$COLORESCAPE"
    echo ""
    exit 1
fi

# Get relative path where the bash script is stored
SOURCE="${BASH_SOURCE[0]}"
DIR="$( dirname "$SOURCE" )"
# Compound the relative path to the root path from the python script path
RDIR=$DIR"/../../../"


echo ""
echo "Execute Makefile Generator python script....."
# Execute the MakefileSamplesGenerator
python MakefileSamplesGenerator.py --path $RDIR

result=$(echo $?)
if [ "0" == "$result" ]; then
    echo ""
    echo -e "$COLORGREEN Makefile Generator was successfully executed.$COLORESCAPE"
    echo ""
    exit 1
else
    echo ""
    echo -e "$COLORRED Makefile Generator execution process could not be finished successfully.$COLORESCAPE"
    echo ""
    exit 0
fi