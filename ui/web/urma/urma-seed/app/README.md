# Meteor Development Environment
This directory contains the meteor development environment for running the device UI in a development environment
without the need for a device to host the UI. 

## URMA framework package
The URMA framework packages are pulled into this project using Meteor Git Packages (MGP). By default they are pulled
in directly from the remote git repository in Stash (see git-packages.json). To update the framework simply run the
'mgp' command from this directory. If you want to do local URMA framework development and pull the packages from your
local file system then edit the local-packages.json file to reference the package and then use the 'mgp link' command.

