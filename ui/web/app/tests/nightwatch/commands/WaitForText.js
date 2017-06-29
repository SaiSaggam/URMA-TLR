
var WaitForText, events,
  extend = function(child, parent) { for (var key in parent) { if (hasProp.call(parent, key)) child[key] = parent[key]; } function ctor() { this.constructor = child; } ctor.prototype = parent.prototype; child.prototype = new ctor(); child.__super__ = parent.prototype; return child; },
  hasProp = {}.hasOwnProperty;

events = require('events');


WaitForText = (function(superClass) {
  extend(WaitForText, superClass);

  WaitForText.prototype.timeoutRetryInMilliseconds = 100;

  WaitForText.prototype.defaultTimeoutInMilliseconds = 50000;

  WaitForText.prototype.locateStrategy = "css";

  function WaitForText() {
    WaitForText.__super__.constructor.apply(this, arguments);
    this.startTimeInMilliseconds = null;
  }

  WaitForText.prototype.restoreLocateStrategy = function() {
    if (this.locateStrategy === "xpath") {
      this.api.useXpath();
    }
    if (this.locateStrategy === "css") {
      return this.api.useCss();
    }
  };

  WaitForText.prototype.command = function(elementSelector, checker, timeoutInMilliseconds) {
    this.locateStrategy = this.client.locateStrategy;
    this.startTimeInMilliseconds = new Date().getTime();
    if (typeof timeoutInMilliseconds !== 'number') {
      timeoutInMilliseconds = this.api.globals.waitForConditionTimeout;
    }
    if (typeof timeoutInMilliseconds !== 'number') {
      timeoutInMilliseconds = this.defaultTimeoutInMilliseconds;
    }
    this.check(elementSelector, checker, (function(_this) {
      return function(result, loadedTimeInMilliseconds) {
        var message;
        if (result) {
          message = "waitForText: " + elementSelector + ". Expression was true after " + (loadedTimeInMilliseconds - _this.startTimeInMilliseconds) + " ms.";
        } else {
          message = "waitForText: " + elementSelector + ". Expression wasn't true in " + timeoutInMilliseconds + " ms.";
        }
        _this.client.assertion(result, 'expression false', 'expression true', message, true);
        return _this.emit('complete');
      };
    })(this), timeoutInMilliseconds);
    return this;
  };

  WaitForText.prototype.check = function(elementSelector, checker, callback, maxTimeInMilliseconds) {
    this.restoreLocateStrategy();
    return this.api.getText(elementSelector, (function(_this) {
      return function(result) {
        var now;
        now = new Date().getTime();
        if (result.status === 0 && checker(result.value)) {
          return callback(true, now);
        } else if (now - _this.startTimeInMilliseconds < maxTimeInMilliseconds) {
          return setTimeout(function() {
            return _this.check(elementSelector, checker, callback, maxTimeInMilliseconds);
          }, _this.timeoutRetryInMilliseconds);
        } else {
          return callback(false);
        }
      };
    })(this));
  };

  return WaitForText;

})(events.EventEmitter);

module.exports = WaitForText;
