var gulp = require('gulp');
// MODULES
var fs = require('fs');
var path = require('path');
var config = require('./gulpconfig.json');


// VARIABLES
var basePath = './';
var meteorPath = basePath + 'app';
var distPath = config && config.distPath || '../dist';

// execute shell scripts
var execute = function(command, path, name, complete, spin) {
    var exec = require('child_process').exec;

    var completeFunc = (typeof complete === 'function') ? complete : console.log;

    var commandProcess = exec(command, {
        cwd: path
    },function(err, res) {
        //process error
        if(err){
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
    var command = 'rm -rf '+distPath;
    execute(command, basePath, 'clean...', cb, true);
});

gulp.task('clientDist', ['clean'], function(cb) {
    var command = 'meteor-build-client '+distPath+' -u default';
    //var command = 'npm run meteor-build-client '+distPath+' -u default';
    console.log(command);
    execute(command, meteorPath, 'meteor-build-client...', cb, true);
});

gulp.task('meteorDist', ['clean'], function(cb) {
    var command = 'meteor build --debug --directory ../mdist';
    execute(command, meteorPath, 'meteor build with debug...', cb, true);
});

gulp.task('meteorDebugDist', ['clean'], function(cb) {
    var command = 'meteor build --directory ../mdist';
    execute(command, meteorPath, 'meteor build...', cb, true);
});

gulp.task('default', ['clientDist']);