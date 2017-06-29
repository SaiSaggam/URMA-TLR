#Overview

This is a prototype device backend that is paired with the angular/meteor frontend UI

This app uses the tornado framework to serve the webapp and to support websockets. It also uses 
sockjs server support to handle the connection from the meteor client. 

The app has a rudimentary DDP connection handler that responds to the standard DDP protocol.
It currently just handles sub on a single posts collection and does a very simple publish
Likewise it just supports a single remote method call

# Prerequisite Tooling
Make sure you have installed these tools on your development machine. They are required to build the project.

## Update installable packages [apt-get](https://www.debian.org/doc/manuals/debian-reference/ch02.en.html#_basic_package_management_operations_with_the_commandline)
```
$ sudo apt-get update
```
## Nodejs - [Install nodejs](https://nodejs.org/en/download/package-manager/)

```
$ sudo apt-get install nodejs
```
## gulp - [Install gulp](https://github.com/gulpjs/gulp)

```
$ [sudo] npm install -g gulp
```
## greyduck - [Install greyduck](https://stash.digi.com/projects/PYTHON/repos/grayduck/browse)
```
$ pip install -i http://pypi.digi.com/simple grayduck
```

# Building
The device application is using gulp. To build do the following:

1. clone and build the seed project per that projects readme
2. edit this pojects gulpconfig.json and point srcDistPath to the dist directory of the seed project
3. build this projects python distribution by running gulp from the root directory of the project


    $ gulp

The artifacts are placed in the /device_dist directory. Currently, the device application can be ran on a pc running
running python 2.X (2.6 or newer) or on a linux based digi gateway such as the X2e. To run this 
application on your pc simply do this: 

    $ cd device_dist
    
    $ python server.py

To deploy to a device, copy the files found in device_dist and below into a linux based digi
gateway in the python file system. Then run server.py and the application should be running at:

    http://xxx.xxx.xxx.xxx:8000/index.html

Modify server.py to run at the desired port and to share the whatever collections and rpc functions
are desired.





#Design notes

This device application is partitioned into a few main modules:

    ddpConnectionImpl.py  
      The websocket connection handler. This handler listens for the new web socket activity and routes
      it to the ddp protocol handler service. This handler is specific to the tornado web server and which
      is configured and initialized at the end of server startup (see server.py)
    ddp.py 
      The DDP service. This service handles many aspects of working with DDP. It understands the DDP
      protocol and helps the application server define the collections and rpc functions to share with
      DDP clients. This service is fairly independent of the specific collections and methods being shared
      with the clients and can therefore be used in many different DDP server applications
    server.py
      The server application that wishes to share resources over DDP. The server decides what collections
      and methods are to be shared and registers those with the DDP service. When clients connect they
      indicate the collections to subscribe to and the DDP service makes a callback to the server to aquire
      the resources. Similarly if the client calls a registered DDP function the DDP service will recieve the 
      request and invoke the callack handler registered by the server. If the server modifies collection 
      resources if should notify the DDP service of the change and the service will in turn notify any 
      interested subscribers. 
  
      The server is also responsible for initializing the web server. In this case we are using the low
      overhead tornado web server which is capable of serving simple web files as well as web socket 
      connections. The server configures the web server and sets up the web socket connection handler and
      then tornado handles the rest.

other files of note:

    lib/sockjs.zip
      This is the sockjs connection library. The front end client uses sockjs to establish the websocket
      connection to the server. Sockjs is a multiprotocol connection library that will try various was to
      establish communication from client to server. Websocket is the first method it will try but it will
      optionally fall back to alternate methods if the websocket connection fails. We are currently only 
      using the websocket method of connection
    lib/xmltodict.py
      This helper library allows easy convertion between xml and python dictionaries. We're using it to 
      communicate with existing xml based interfaces.
    transportSettings.xml
      Simply an example of settings on a transport device - used for early prototyping


#Build Output
  The results of the build will be put into /device_dist. This directory should contain everything you need
  to run the python application either on your pc or on a digi linux device. The content of this directory are as follows:
  
    appfiles.zip
      A zip file of common python resources used by the application. We use the grey-duck to find all the 
      dependent python packages and build them into this zip file.
    ddp.py, ddpConnectionImpl.py, server.py, transportSettings.xml, xmltodict.sockjs.zip
      These are all resources described above copied into the build directory for execution
    /web
      Files in this directory are to be served up by the tornado web server when they are asked for by the
      client browser

      index.html
        The main page loaded by the users browser when they visit the device. It loads the rest of the app.
      nnnnn.js
        This is a simple wrapper app that in turn loads the device-bundle app
      mmmmm.css
        Styling for the simple wrapper app
      /devbundles/device-bundle.js
        This is bundled package of all the device specific pages in the /app folder
      /packages
        This directory contains other 3rd party web packages used by the application (eg twitter bootstrap)




#Todo:
* expand to support digi device collections for settings and state
* add remote rci command support
* figure out a way to make grayduck pull in the sockjs files


