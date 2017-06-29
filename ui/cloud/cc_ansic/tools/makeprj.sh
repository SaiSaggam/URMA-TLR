#!/bin/sh

MAKE_PROJECT=""
MAKE_TARGET=all
SAMPLES_PATH=public/run/samples
MAKE_CMD="make"
PROJ_ROOT=`pwd`

if [ $# -eq 0 ] ; then
	MAKE_PROJECT=all
elif [ $# -eq 1 ] ; then
	MAKE_PROJECT=$1
elif [ $# -eq 1 ] ; then
	MAKE_PROJECT=$1
	MAKE_TARGET=$2
fi

for DIR in ./$SAMPLES_PATH/*; do
	APP_PATH=${DIR%*/}
	APP_DIR=${APP_PATH##*/}

	if [ "$MAKE_PROJECT" = "$APP_DIR" -o "$MAKE_PROJECT" = "all" ]; then

		cd ${PROJ_ROOT}/${SAMPLES_PATH}/${APP_DIR}/
		
		if [ ! -e Makefile ]; then
			${PROJ_ROOT}/dvt/scripts/makegen.py ${PROJ_ROOT}/${SAMPLES_PATH}/${APP_DIR}/ 
		fi		
	
	   	${MAKE_CMD} ${MAKE_TARGET}
	   	cd ${PROJ_ROOT}
	fi
done

exit 0
