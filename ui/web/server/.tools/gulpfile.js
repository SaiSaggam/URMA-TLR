var gulp = require('gulp');
// MODULES
var fs = require('fs');
var path = require('path');
var config = require('./gulpconfig.json');
console.log('Build using config:', config);

// VARIABLES
var basePath = './';
// var srcDistPath = config && config.srcDistPath || './dist';
var pyDistPath = config && config.pyDistPath || '.build/dist';

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

gulp.task('clean', function(cb) {
  var command = 'rm -rf ' + pyDistPath;
  execute(command, basePath, 'clean...', cb, true);
});

gulp.task('mkDeviceDistDir', ['clean'], function(cb) {
  var command = 'mkdir -p ' + pyDistPath;
  execute(command, basePath, 'create device dist dir...', cb, true);
});

gulp.task('greyduck', ['clean', 'mkDeviceDistDir'], function(cb) {
  var command = 'grayduck-build --config=.tools/grayduck.cfg  -s mysource';
  execute(command, basePath, 'grayduck-build...', cb, true);
});

gulp.task('copyLibs', ['mkDeviceDistDir'], function(cb) {
  var command = 'cp -r lib/* ' + pyDistPath;
  execute(command, basePath, 'copy support libs...', cb, true);
});

//gulp.task('mkWebDir', ['clean'], function(cb) {
//    var command = 'mkdir '+pyDistPath+'/web';
//    execute(command, basePath, 'create device web dir...', cb, true);
//});
//
// gulp.task('copyWebFiles', ['mkDeviceDistDir'], function(cb) {
//   var command = 'cp -r ' + srcDistPath + '/* ' + pyDistPath + '/web';
//   execute(command, basePath, 'copy web files...', cb, true);
// });
//
// gulp.task('copyWebAppFiles', ['mkDeviceDistDir'], function(cb) {
//   var command = 'cp -r device/web/* ' + pyDistPath + '/web';
//   execute(command, basePath, 'copy web app files...', cb, true);
// });

gulp.task('deviceDist', ['greyduck', 'copyLibs' /*, 'copyWebFiles' ,'copyWebAppFiles'*/ ]);

gulp.task('default', ['deviceDist']);
