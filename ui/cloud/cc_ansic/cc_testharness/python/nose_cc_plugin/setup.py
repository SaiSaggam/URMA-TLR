try:
    import ez_setup
    ez_setup.use_setuptools()
except ImportError:
    pass

from setuptools import setup

setup(
    name='nose_cc_plugin',
    version='2.0',
    author='Digi International',
    author_email = 'CloudConnector.Embedded.Developers@digi.com',
    description = 'Plugin to manage Device Cloud credentials, requests, settings.',
    license = 'For Digi Internal Use Only',
    py_modules = ['cc_plugin', 'cc_testcase' ],
    install_requires=['requests==2.1.0', 'py-dom-xpath==0.1'],
    entry_points = {
        'nose.plugins.0.10': [
            'cc = cc_plugin:CloudConnectorPlugin'
            ]
        }
    )
