# Overview
This is a prototype device backend that is paired with the angular/meteor frontend UI

This app uses the tornado framework to serve the webapp and to support websockets. It also uses
sockjs server support to handle the connection from the meteor client.

The app has a rudimentary DDP connection handler that responds to the standard DDP protocol.
It currently just handles sub on a single posts collection and does a very simple publish
Likewise it just supports a single remote method call


# Getting Started
## Prerequisites
Make sure you have installed these prerequisites on your development machine.

### Nodejs
<pre>
$ curl -sL https://deb.nodesource.com/setup_0.10 | sudo -E bash -
$ sudo apt-get install -y nodejs
$ sudo chown -R $(whoami) $(npm config get prefix)/{lib/node_modules,bin,share}
</pre>

## Get the project source
To begin with simply clone this repo to your local unix/osx development system
<pre>
$ git clone ssh://git@stash.digi.com/idigi/urma-py-server.git
</pre>

## Install the global tools
Next, there are some global tools that must be installed on your system in order to run and build
the project. This only needs to be done once per system - not on each checkout. Move to the
cloned project directory and run the global install as follows:
<pre>
$ cd urma-py-server
$ npm install package-script
$ npm run global_install
</pre>

*Note*: the global_install requires sudo access. You will be prompted for your password.

## Install the project dependencies
All the project dependencies are installed via the global npm tool we installed previously. After each checkout
or pull you can refresh those dependencies. From the root of the project run the following command:
<pre>
$ npm install
</pre>


## Building
The device application is using gulp. To build do the following:

1. clone and build the seed project per that projects readme
2. edit this pojects gulpconfig.json and point srcDistPath to the dist directory of the seed project
3. build this projects python distribution by running gulp from the root directory of the project


<pre>
$ npm run build
</pre>

The artifacts are placed in the /device_dist directory. Currently, the device application can be ran on a pc running
running python 2.X (2.6 or newer) or on a linux based digi gateway such as the X2e. To run this
application on your pc simply do this:

<pre>
$ cd device_dist
$ python server.py
</pre>

To deploy to a device, copy the files found in device_dist and below into a linux based digi
gateway in the python file system. Then run server.py and the application should be running at:

    http://xxx.xxx.xxx.xxx:8000/index.html

Modify server.py to run at the desired port and to share the whatever collections and rpc functions
are desired.


## Testing
Unit tests of the python code will be written in pythons built in unittest style. The tests will be ran using the pytest
runner with output rendered in junitxml format. This allows us to run automated unit tests in bamboo.

For scaffolding we will use the python mock library.

These tools are all installed via <b>npm install</b>. The units can be ran from the root directory as follows:
<pre>
$ npm run test
</pre>

Reference:
https://docs.python.org/2/library/unittest.html
http://pytest.org/latest/getting-started.html
https://docs.python.org/dev/library/unittest.mock.html




# Design notes

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


# Build Output
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




# Todo:
* unittest for all code
* expand to support digi device collections for settings and state
* add remote rci command support
* figure out a way to make grayduck pull in the sockjs files




# Server DDP processing engine

## Key Constructs
### Collections
Collections are published from the device to subscribing clients

## Server startup
### Register remote methods & their handlers
### Register collections & collection providers
### Initialize Firmware Bridge
When the python server is started, it will initialize the firmware bridge and register
a callback handler for asynchronous events that need to be propagated up from the
firmware.


### Prime Collection contents
When server starts, it will initialize DDP service and register all collections and
collection providers. The collection providers can initialize the contents of those
collections either at startup or upon client subscriptions

#### Settings Collection
Subscribe to firmware settings data. This triggers a callback from the firmware sending
up all known settings groups & instances to the registered python callback handler. Upon
return from the subscribe call, all settings data will have been delivered.

## On new subscription
- Run query against specified collection finding matches:
  - filter output fields to those requested by subscription
  - compare output record against what is currently published to this client
    - New: publish to client as added. insert publishedTo record
    - Subset: no publish necessary
    - Superset: publish to client as changed with new fields. update publishedTo record

## On unsubscribe
- Run through specified collection looking for entries published to this client
  - check record to see if it applies to expiring subscription, if yes:
    - get record as it applies to other subscriptions to same client
      - NoOtherMatches: publish to client as removed. remove publishedTo record
      - Subset match: publish to client as changed with cleared fields. update publishedTo record

## On Firmware collection update
- accept update operation from firmware callback hook
- Find affected collection record & identify what values actually changed
- for every client subscribed to this collection
  - get record as it applies to subscriptions for this client
  - compare this against current publishedTo record for that client
    - New: publish to client as added. insert publishedTo record
    - Subset: if subset fields changed publish as changed and update publishedTo record. else no publish necessary
    - Superset: publish to client as changed with new fields. update publishedTo record
    - NoOtherMatches: publish to client as removed. remove publishedTo record

## On remote client update
- accept update operation from remote client
- compare to see what values actually changed
- send changed fields to firmware
- onChange callback handled as above

## On session republish of a collection (ie login/out)
- build composite query for collection
- run publish functions again for session and compare to cached copy
- send added/removed/changed events as needed


# Design Constructs
## interfaces
### DDPConnection
### DDPSession
### DDP
### Provider

## DDP Service
###collections
Key: collectionName
Value: Provider
