# Remote Device Manager
This is a seed project for developing a device UI in using the Unified Remote Management Architecture (URMA).
It provides a starting point for developing the device UI. This UI can be hosted directly on the device as
well as on in Remote Manager on Device Cloud, providing a unified user experience.

Development can be done locally using meteor. When ready to deploy to a device you can build this project
into a minimized build that can be placed in the device's file system.

Most of the UI is created as a Meteor package (see packages/urma-seed). The main meteor application is
quite simple and basically just pulls in the UI package. The primary reason for this structure is to allow
us to create and run automated mocha unit tests for the package.

# Getting Started
## Prerequisites
Make sure you have installed these prerequisites on your development machine.

### Nodejs
<pre>
$ curl -sL https://deb.nodesource.com/setup_0.10 | sudo -E bash -
$ sudo apt-get install -y nodejs
$ sudo chown -R $(whoami) $(npm config get prefix)/{lib/node_modules,bin/node,bin/npm,share}
</pre>

### Meteor - [Install Meteor](https://www.meteor.com/install)
<pre>
$ curl https://install.meteor.com/ | sh
</pre>

## Get the project source
To begin with simply clone this repo to your local unix/osx development system
<pre>
$ git clone ssh://git@stash.digi.com/idigi/urma-seed.git
</pre>

## Install the global tools
Next, there are some global tools that must be installed on your system in order to run and build
the project. This only needs to be done once per system - not on each checkout. Move to the
cloned project directory and run the global install as follows:
<pre>
$ cd urma-seed
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

## Running the seed application
You can run the application on your local machine by simply invoking Meteor as follows:
<pre>
$ meteor
</pre>

Once the application is launched, you will see a message like this:
> => App running at: http://localhost:3000/

Open a browser window and go to this url and you should see the application

## Making updates
You can generally modify the application while it is running and meteor will detect
this and update the application on the fly without you needing to restart it. You may
want to use an incognito window to avoid caching issues during development.

## Unit tests
Unit tests should be written for all new features. These are Mocha tests created at the package level and
and are generally found along side the code that they test. Generally same part name with a -test added to them.
See the urma-core package for some good examples.

Unit tests can be run with an html view or to the command console. The html view is handy to use during
development as the tests are executed as you write and save them. Invoke the html view by starting meteor
like this:
<pre>
$ meteor test-packages --driver-package=practicalmeteor:mocha
</pre>

Executing unit tests to the console is what we typically do in the automated bamboo builds. You can
do it from the command line using the spacejam tool like this:
<pre>
$ spacejam test-packages --driver-package=practicalmeteor:mocha-console-runner
</pre>

## Validation tests
End-to-End UI validation testing is done using nightwatch. These tests are found in the <code>tests/nightwatch/walkthroughs</code> folder.

To run the validation tests, start the application normally as shown above, then run the tests like this:
<pre>
$ starrynight run-tests --framework nightwatch --env phantomjs
</pre>

This will run the UI tests using phantomjs (a headless browser that runs in the background).
If you want to watch your tests run interactively in the local browser then invoke the command
shown above without the --env phantomjs option.

To pass custom input to the tests. Edit globals.json file in URMA-Seed/tests/nightwatch

## Building device artifacts
Once the UI is developed and the tests are all passing, you will want to deploy the UI to a
device. To do this you need to build a distribution that can subsequently be incorporated into
the web file system of the device. To build the distribution, invoke the following command from
the root folder of the project:
<pre>
$ npm run gulp
</pre>

The output of the build will be found in the .build/dist folder.

# URMA framework developers
If you are doing development work on the urma framework it can be handy to have your local seed repo point to your
local framework repo so that you can test your changes locally before committing them to git.

This can be done using the mgp link command. First edit the file <code>urma-seed/local-packages.json</code> such that it points points to your locally installed urma-framework directory. Then run the command:
<pre>
$ mgp link
</pre>

This tells the mgp tool to just use the local git repo rather than pulling in the official repo from stash. To revert to the official repo simply type:
<pre>
$ mgp
</pre>

*Note:* Be aware that running npm install will automatically revert the link to the official repo.
