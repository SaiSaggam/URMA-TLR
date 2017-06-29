#!/bin/bash

# This release generates all the part numbers required for releasing and
# optionally moves it to "pending" folder in the \\eng server.
# This needs the following environment variables defined:
#     TAR_REVISION
#     ZIP_REVISION
#     TAG
#     PENDING
# For example, for creating the 2.1.0.0 release, the following must be executed:
#
#     export TAR_REVISION=C
#     export ZIP_REVISION=A
#     export TAG=2.1.0.0
#     export PENDING=false
#     ./build_release.sh
#
# The project's taggint scheme is W.X.Y.Z; for Major, Minor, Revision and Build numbers
# respectively. For example, 2.1.0.0.
#
# The release is automated in this Bamboo Plan http://bamboo.digi.com/browse/CCTH-RC
# And in this Jenkins CI Job: http://jenkins.ci.digi.com/job/Etherios%20Cloud%20Connector/job/Release%20Candidate/

WORKSPACE="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
rc=0
rca=0
PRODUCT_NAME=connector
BASE_DIR=$WORKSPACE/$PRODUCT_NAME
OUTPUT_DIR=$WORKSPACE/output
GETTING_STARTED_GUIDE=90001345
GETTING_STARTED_GUIDE_RELEASED=90001345_A
RELEASE_NOTES=93000761
HTML_ZIP=40002854
TARBALL_NUMBER=40002853
TARBALL_NAME=${TARBALL_NUMBER}_${TAR_REVISION}
ZIPFILE_NUMBER=40002861
ZIPFILE_NAME=${ZIPFILE_NUMBER}_${ZIP_REVISION}
NOTES_NAME=${RELEASE_NOTES}_${TAR_REVISION}
HTML_NAME=${HTML_ZIP}_${TAR_REVISION}
TOOLS_DIR=${BASE_DIR}/tools

SAMPLES="compile_and_link
         connect_on_ssl
         connect_to_device_cloud
         data_point
         device_request
         file_system
         firmware_download
         remote_config
         send_data
         simple_remote_config"

function cleanup ()
{
    ARCHIVE=${WORKSPACE}/archive
    if [ -d "${ARCHIVE}" ]; then
        echo ">> Archive Directory already exists, cleaning it."
        rm -rfv "${ARCHIVE}"/*
    else
        echo ">> Creating Archive Directory."
        mkdir -p "${ARCHIVE}"
    fi
    echo ">> Archiving critical files."
    cp -v "${OUTPUT_DIR}/${TARBALL_NAME}.tgz" "${ARCHIVE}/"
    cp -v "${OUTPUT_DIR}/${NOTES_NAME}.zip" "${ARCHIVE}/"
    cp -v "${OUTPUT_DIR}/${HTML_NAME}.zip" "${ARCHIVE}/"
    cp -v "${OUTPUT_DIR}/${ZIPFILE_NAME}.zip" "${ARCHIVE}/"


    echo ">> Cleaning Up ${OUTPUT_DIR} and ${BASE_DIR}"
    rm -r "${OUTPUT_DIR}"
    rm -r "${BASE_DIR}"
}

function build_config_tool ()
{
    mkdir -p "${TOOLS_DIR}"
    ant -f "${WORKSPACE}/tools/config/build.xml" -Ddist.dir="${TOOLS_DIR}"
}

function copy_python_scripts ()
{
    mkdir -p "${TOOLS_DIR}/python"
    cp "${WORKSPACE}/tools/python/"*.py "${TOOLS_DIR}/python"
}

./dvt/scripts/makegen.sh

# Create the output directory.
mkdir -p "${OUTPUT_DIR}"

build_config_tool
copy_python_scripts

# Create the doxygen documentation
cd doxygen
doxygen
cd ..

# Move the HTML files into the docs directory
mkdir -p docs/html
echo ">> Removing Health Metrics documentation"
rm doxygen/html/health_metrics_sample.html
rm doxygen/html/cloud_health_metrics_config.png
rm doxygen/html/cloud_health_metrics.png
echo ">> Copying documentation"
cp -rf doxygen/html/* docs/html
cp doxygen/user_guide.html docs/
cp doxygen/images/cloud-connector-logo-H.png docs/html

# Create a "${PRODUCT_NAME}" subdirectory which will be the root of the tarball.
echo ">> Creating ${BASE_DIR} and copying public and private directories to it."
mkdir -p "${BASE_DIR}"
cp -rf private "${BASE_DIR}"
cp -rf public "${BASE_DIR}"
cp -rf docs "${BASE_DIR}"
echo ">> Removing some samples"
rm -rf "${BASE_DIR}"/public/run/samples/bRCI_commands
rm -rf "${BASE_DIR}"/public/run/samples/rci_reworked
rm -rf "${BASE_DIR}"/public/run/samples/health_metrics
rm -rf "${BASE_DIR}"/public/step/samples/health_metrics
echo ">> Removing unused directories"
rm     "${BASE_DIR}/public/run/platforms/linux/network_sms_proxy.c_aux"
rm -rf "${BASE_DIR}/public/run/platforms/mqx"
rm -rf "${BASE_DIR}/public/run/platforms/ucos"
rm -rf "${BASE_DIR}/public/run/platforms/template"
rm -rf "${BASE_DIR}/public/step/platforms/template"
rm -rf "${BASE_DIR}/public/step/platforms/windows"

# Get the name of the getting starting guides and see which one is newer
released_file=$(find /eng/store/released/90000000/"${GETTING_STARTED_GUIDE_RELEASED}" -name index.html)
pending_file=$(find /eng/store/pending/90000000/"${GETTING_STARTED_GUIDE}" -name index.html)

if [ -n "${pending_file}" ]
  then
    echo ">> Pulling Getting Started Guide from ${pending_file}"
    cp -f "${pending_file}" "${BASE_DIR}/getting_started.html"
elif [ -n "${released_file}" ]
  then
    echo ">> Pulling Getting Started Guide from ${released_file}"
    cp -f "${released_file}" "${BASE_DIR}/getting_started.html"
else
    echo ">> Cannot find ${GETTING_STARTED_GUIDE}*/index.html"
    exit 1
fi

# Allows a Linux system to view the html page.  See IC4C-105
chmod -x "${BASE_DIR}/getting_started.html"

# Replace the version number in Readme.txt to match the Tag used to build
if [ $TAG != "" ]
  then
    echo ">> Setting Release notes header to ${NOTES_NAME} v${TAG} in ${BASE_DIR}/private/Readme.txt"
    sed -i 's/ _RELEASE_NOTES_PARTNO_/ '"$NOTES_NAME"'/g' "${BASE_DIR}/private/Readme.txt"
    sed -i 's/ v_CONNECTOR_SW_VERSION_/ v'"$TAG"'/g' "${BASE_DIR}/private/Readme.txt"
    echo ">> Setting Version to ${TAG} in ${BASE_DIR}/private/connector_info.h"
    sed -i 's/#define CONNECTOR_SW_VERSION \S*/#define CONNECTOR_SW_VERSION "'"$TAG"'"/g' "${BASE_DIR}/private/connector_info.h"
fi

# Don't replace the date in Readme.txt to match today's date!!
# This was eliminated due to ECO004263 regarding Release Note templaces (see 96000472)
#today=`date +"%B %d, %Y"`
#echo ">> Setting Release Date to Today (${today}) in ${BASE_DIR}/private/Readme.txt"
#sed -i 's/_RELEASE_DATE_/'"${today}"'/g' "${BASE_DIR}/private/Readme.txt"

# Replace the version number in connector_api.h
python ./dvt/scripts/update_hex_version.py "${BASE_DIR}"/public/include/connector_api.h "${TAG}"

# Generate a Makefile for each sample.

# Create the tarball
echo ">> Creating the Tarball ${OUTPUT_DIR}/${TARBALL_NAME}.tgz."
tar --exclude="${PRODUCT_NAME}"/public/test --exclude="${PRODUCT_NAME}"/public/dvt -czvf "${OUTPUT_DIR}/${TARBALL_NAME}.tgz" "${PRODUCT_NAME}"/

# Create the Release Notes
echo ">> Creating the Release notes ${OUTPUT_DIR}/${NOTES_NAME}.zip"
zip -jvl "${OUTPUT_DIR}/${NOTES_NAME}.zip" "${BASE_DIR}/private/Readme.txt"

# Create the HTML ZIP
echo ">> Creating the Documenation tree ${OUTPUT_DIR}/${HTML_NAME}.zip"
cd "${BASE_DIR}/docs"
zip -v "${OUTPUT_DIR}/${HTML_NAME}.zip" user_guide.html
zip -vr "${OUTPUT_DIR}/${HTML_NAME}.zip" html/
cd "${WORKSPACE}"

# Delete the original directory
echo ">> Removing base dir ${BASE_DIR}."
rm -rf "${BASE_DIR}"

# Uncompress the tarball we just created.
echo ">> Uncompressing ${OUTPUT_DIR}/${TARBALL_NAME}.tgz."
tar -xf "${OUTPUT_DIR}/${TARBALL_NAME}.tgz"

# Change all LF to CR+LF
echo ">> Changing all LF to CR+LF and creating ${OUTPUT_DIR}/${ZIPFILE_NAME}.zip."
find "${BASE_DIR}" -type f -exec grep -Il . {} \; -exec sed -i ':a;N;$!ba;s/\n/\r\n/g' '{}' \;
zip "${OUTPUT_DIR}/${ZIPFILE_NAME}.zip" -r "${PRODUCT_NAME}"/

cd "${BASE_DIR}"
python ../dvt/scripts/replace_str.py public/run/platforms/linux/config.c '#error' '//#error'

# Build all the IIK samples and platforms

echo ">> Building all samples."
cd public/run/samples

for sample in $SAMPLES
do
  echo ">> Building $sample"
  cd $sample
  if [ "$sample" == "remote_config" ] || [ "$sample" == "simple_remote_config" ]; then
    java -jar "${TOOLS_DIR}/ConfigGenerator.jar" username:password "Linux Application" 1.0.0.0 -noUpload -vendor=0x123456 config.rci
  fi
  make clean; make all
  rc=$?
  if [[ ${rc} != 0 ]]; then
    echo "++ Failed to build $sample, exiting."
    cleanup
    exit ${rc}
  fi
  cd ../
done

cd ../../../../

if [[ "${PENDING}" == "true" ]]; then
    # If successfull push the tarball to pending, if PENDING environment variable is set to 1.
    echo ">> Copying the Tarball to Pending."
    cp -v "${OUTPUT_DIR}/${TARBALL_NAME}.tgz" /eng/store/pending/40000000
    cp -v "${OUTPUT_DIR}/${ZIPFILE_NAME}.zip" /eng/store/pending/40000000
    cp -v "${OUTPUT_DIR}/${NOTES_NAME}.zip" /eng/store/pending/93000000
    cp -v "${OUTPUT_DIR}/${HTML_NAME}.zip" /eng/store/pending/40000000
fi

cleanup
exit $rc
