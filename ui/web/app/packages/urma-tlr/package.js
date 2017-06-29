Package.describe({
    name: 'digi:urma-tlr',
    version: '1.0.0',
    // Brief, one-line summary of the package.
    summary: '',
    // URL to the Git repository containing the source code for this package.
    git: '',
    // By default, Meteor will default to using README.md for documentation.
    // To avoid submitting documentation, set this field to null.
    documentation: 'README.md'
});

Npm.depends({
    'react': '0.14.7',
    'react-addons-create-fragment': '0.14.7',
    'react-addons-linked-state-mixin': '0.14.7',
    'react-addons-pure-render-mixin': '0.14.7',
    'react-addons-test-utils': '0.14.7',
    'react-addons-transition-group': '0.14.7',
    'react-addons-update': '0.14.7',
    'react-dom': '0.14.7',
    'react-helmet': '2.3.0',
    'react-mixin': '3.0.3',
    'react-router': '2.4.0',
    'formsy-react': '0.18.0',
    'react-fileupload-progress': '0.3.0',
    'react-modal-dialog': '2.0.0',
    'events': '1.1.0',
    'chai': '3.5.0',
    'jstz': '1.0.7'
});

Package.onUse(function(api) {
    api.use('mongo', ['client', 'server']);
    api.use('jag:pince');
    api.use('modules');
    api.use('http');
    api.use('es5-shim');
    api.use('ecmascript');
    api.use('react-meteor-data');
    api.use('digi:urma-core@1.0.0');
    api.use('digi:urma-styles@1.0.0');
    api.mainModule('urma-tlr.js', 'client');
    api.mainModule('urma-tlr-server.js', 'server');
    api.addFiles('styles.scss','client');
    api.use('3stack:country-codes-tz','client');
});

Package.onTest(function(api) {
    api.use('jag:pince');
    api.use('ecmascript');
    api.use('practicalmeteor:mocha');
    api.use('erasaur:meteor-lodash');
    api.use('react-meteor-data');
    api.use('accounts-password');
    api.use('digi:urma-tlr');
    api.mainModule('urma-tlr-tests.js', 'client');
    api.use('3stack:country-codes-tz','client');
});
