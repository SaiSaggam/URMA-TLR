Package.describe({
  name: 'digi:urma-core',
  version: '1.0.0',
  // Brief, one-line summary of the package.
  summary: 'Digi Unified Remote Manager Application core framework',
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
    'chai': '3.5.0'
});

Package.onUse(function(api) {
    api.use('mongo', ['client', 'server']);
    api.use('es5-shim');
    api.use('ecmascript');
    api.use('fourseven:scss');
    api.use('accounts-password');
    api.use('wolves:bourbon@3.1.0');
    api.use('wolves:bitters@3.1.0');
    api.use('wolves:neat@3.1.0');
    api.use('digi:urma-styles@1.0.0');
    api.use('meteorhacks:subs-manager@1.6.3');
    api.use('chrismbeckett:toastr@2.1.2_1');
    api.use('react-meteor-data');
    api.use('matb33:collection-hooks');
    api.mainModule('urma-core.js', 'client');
    api.mainModule('urma-core-server.js', 'server');
    api.addFiles([
            'navigation/styles/_main-menu-layout.scss',
            'navigation/styles/_main-menu.scss',
            'security/styles/_security.scss',
            'file-system/styles/_file-system.scss',
            'styles.scss'
      ],
      'client');
});


Package.onTest(function(api) {
    api.use('ecmascript');
    api.use('practicalmeteor:mocha');
    api.use('erasaur:meteor-lodash');
    api.use('accounts-password');
    api.use('chrismbeckett:toastr');
    api.use('digi:urma-core');
    api.mainModule('urma-core-tests.js', 'client');
});
