

exports.command = function() {
    var browser = this;

    browser.windowHandles(function(result) {
        browser.switchWindow(result.value[1]);
    });

    return browser;
};
