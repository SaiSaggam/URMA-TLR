Package.describe({
    name: 'digi:urma-styles',
    version: '1.0.0',
    // Brief, one-line summary of the package.
    summary: 'Styles for Digi Unified Remote Manager Application framework',
    // URL to the Git repository containing the source code for this package.
    git: '',
    // By default, Meteor will default to using README.md for documentation.
    // To avoid submitting documentation, set this field to null.
    documentation: 'README.md'
});

Package.onUse(function(api) {
    api.versionsFrom('1.2.1');
    api.use('fourseven:scss');
    api.use('wolves:bourbon@3.1.0');
    api.use('wolves:bitters@3.1.0');
    api.use('wolves:neat@3.1.0');
    api.use('fortawesome:fontawesome@4.5.0');
    api.imply('fourseven:scss');
    api.addAssets([
        'fonts/Source-Sans-Pro-600/Source-Sans-Pro-600.woff2',
        'fonts/Source-Sans-Pro-600/Source-Sans-Pro-600.woff',
        'fonts/Source-Sans-Pro-italic/Source-Sans-Pro-italic.woff2',
        'fonts/Source-Sans-Pro-italic/Source-Sans-Pro-italic.woff',
        'fonts/Source-Sans-Pro-regular/Source-Sans-Pro-regular.woff2',
        'fonts/Source-Sans-Pro-regular/Source-Sans-Pro-regular.woff'
    ], 'client');
    api.addFiles([
        'fonts.css',
        'styles/abstractions/_mixins.scss',
        'styles/abstractions/_variables.scss',
        'styles/base/_animations.scss',
        'styles/base/_base.scss',
        'styles/base/_buttons.scss',
        'styles/base/_toggle-groups.scss',
        'styles/base/_typography.scss',
        'styles/layout/_base-layout.scss',
        'styles/layout/_grid-settings.scss',
        'styles/layout/_panel-layout.scss',
        'styles/components/_deviceConfig.scss',
        'styles/components/_header.scss',
        'styles/components/_progress-bar.scss',
        'styles/components/_countdown-timer.scss',
        'styles/components/_loader.scss',
        'styles/bourbon/refills/flash.scss',
        'styles.scss'
    ], 'client');
});