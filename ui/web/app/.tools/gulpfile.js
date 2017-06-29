var gulp = require('gulp');
// MODULES
var fs = require('fs');
var path = require('path');
var config = require('./gulpconfig.json');


// VARIABLES
var basePath = './';
var meteorPath = basePath;
var distPath = config && config.distPath || '.build/dist';
var mdistPath = config && config.mdistPath || '.build/mdist';

// execute shell scripts
var execute = function(command, path, name, complete, spin) {
  var exec = require('child_process').exec;

  var completeFunc = (typeof complete === 'function') ? complete : console.log;

  var commandProcess = exec(command, {
    cwd: path
  }, function(err, res) {
    //process error
    if (err) {
      console.log(err.message);
      completeFunc(err);

    } else {
      completeFunc();
    }
  });
  commandProcess.stdout.pipe(process.stdout);
  commandProcess.stderr.pipe(process.stderr);
};

gulp.task('clean_dist', function(cb) {
  var command = 'rm -rf ' + distPath;
  execute(command, basePath, 'clean...', cb, true);
});

gulp.task('clean_mdist', function(cb) {
  var command = 'rm -rf ' + mdistPath;
  execute(command, basePath, 'clean...', cb, true);
});

gulp.task('clientDist', ['clean_dist'], function(cb) {
  var command = 'meteor-build-client ' + distPath +
    ' -u default -s .tools/mbc_settings.json -t .tools/index_template.html';
  // console.log(command);
  var copyExtraFiles = function(err) {
    var command = 'cp -r .tools/web ' + distPath + '/web'
    execute(command, meteorPath, 'meteor-build-client...', cb, true);
  }
  execute(command, meteorPath, 'meteor-build-client...', copyExtraFiles,
    true);
});

gulp.task('meteorDist', ['clean_mdist'], function(cb) {
  var command = 'meteor build --directory ' + mdistPath;
  var copyExtraFiles = function(err) {
    var command = 'cp -r .tools/web ' + mdistPath + '/web'
    execute(command, meteorPath, 'meteor-build-client...', cb, true);
  }
  execute(command, meteorPath, 'meteor build with debug...', copyExtraFiles,
    true);
});

gulp.task('meteorDebugDist', ['clean_mdist'], function(cb) {
  var command = 'meteor build --debug --directory ' + mdistPath;
  var copyExtraFiles = function(err) {
    var command = 'cp -r .tools/web ' + mdistPath + '/web'
    execute(command, meteorPath, 'meteor-build-client...', cb, true);
  }
  execute(command, meteorPath, 'meteor build...', copyExtraFiles, true);
});

gulp.task('default', ['clientDist']);
