#!/bin/sh

# build web app
cd "$1"/app
npm install
if [ "$?" != "0" ]; then
    echo "Failed to install app dependencies"
    exit 1
fi
mgp link
npm run build
cd ..

# build web server
cd "$1"/server
npm install
if [ "$?" != "0" ]; then
    echo "Failed to install server dependencies"
    exit 1
fi
npm run build
cd ..
