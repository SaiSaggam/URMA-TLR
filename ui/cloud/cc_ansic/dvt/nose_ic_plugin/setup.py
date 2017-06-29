try:
    import ez_setup
    ez_setup.use_setuptools()
except ImportError:
    pass

from setuptools import setup

setup(
    name='nose_ic_plugin',
    version='1.2',
    author='Digi International',
    author_email = 'CFT.product.ICC.developers@digi.com',
    description = 'Plugin to pass in iDigi Credentials and Device ID to iDigi Connector Tests.',
    license = 'For Digi Internal Use Only',
    py_modules = ['ic_plugin', 'ic_testcase', 'build_plugin' ], 
    install_requires=['requests == 0.14.2', 'py-dom-xpath'],
    entry_points = {
        'nose.plugins.0.10': [
            'ic = ic_plugin:ICPlugin',
            'build = build_plugin:BuildPlugin',
            ]
        }
    )
