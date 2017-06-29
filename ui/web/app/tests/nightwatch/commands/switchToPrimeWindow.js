

exports.command = function() {
    var browser = this;

    browser.windowHandles(function(result) {
        browser.switchWindow(result.value[0]);
    });

    return browser;
};
