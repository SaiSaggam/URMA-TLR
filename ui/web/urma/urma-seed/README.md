# Remote Device Manager
This is a seed project for developing a device UI in using the Unified Remote Management Architecture (URMA).
It provides a starting point for developing the device UI. This UI can be hosted directly on the device as
well as on in Remote Manager on Device Cloud, providing a unified user experience.

Development can be done locally using meteor. When ready to deploy to a device you can build this project
into a minimized build that can be placed in the device's file system.

The entire device UI must be placed in the 'app' subdirectory of this project. The contents of the app directory
can be organized as you see fit for your device UI. However, this seed project has been organized to reflect the
recommended structure. All other directories outside of the 'app' directory are for the development and build
environment.

# Before You Begin
For development purposes this project includes a meteor environment. This allows quick and easy local development
without having to load the UI onto a device. This is for *development only* and will not be included in the minimized
builds that get loaded onto a device's file system.

# Prerequisites
Make sure you have installed these prerequisites on your development machine.

## Nodejs

```
$ curl -sL https://deb.nodesource.com/setup_0.10 | sudo -E bash -

$ sudo apt-get install -y nodejs
```

## Meteor - [Install Meteor](https://www.meteor.com/install)

```
$ curl https://install.meteor.com/ | sh
```

# Download and install the project

```
$ git clone .... 

$ cd project

$ npm install
```

# Run the project in PC development mode

```
$ cd app

$ meteor
```

# Build a the project for distribution to an embedded device

```
$ gulp
```

## NPM - [Install NPM](https://www.npmjs.com/package/npm)

```
$ sudo apt-get install npm
```

## gulp - [Install gulp](https://github.com/gulpjs/gulp)

```
$ [sudo] npm install -g gulp
```
## meteor-build-client - [Install meteor-build-client](https://github.com/frozeman/meteor-build-client)

```
$ [sudo] npm install -g meteor-build-client
```

## meteor git packages (mgp) - [Install mgp](https://github.com/DispatchMe/mgp)

```
$ [sudo] npm install -g mgp
```

# Updating URMA package dependencies from stash using mgp

```
$ cd app
```

```
$ mgp
```


