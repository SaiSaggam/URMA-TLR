# -*- coding: utf-8 -*-
import os
import shutil
import subprocess
import tempfile
import sys
sys.path.append('./dvt/scripts')
import config

TEMPLATE_TEST_DIR = 'dvt/samples/build_library'

TEMPLATE_TEST_DIR = 'dvt/samples/template_test'
TEMPLATE_SCRIPT_DIR = 'dvt/cases/dvt_tests/'
TEMPLATE_PLATFORM_DIR = 'public/run/platforms/template/'

LIBRARY_TEST_DIR = 'dvt/samples/build_library'
LIBRARY_SCRIPT_DIR = 'dvt/cases/dvt_tests/'
LIBRARY_PLATFORM_DIR = 'public/run/platforms/template/'

def build(dir, cflags):
    process = subprocess.Popen(['make', 'clean'], cwd=dir, 
        stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    data = process.communicate()[0]
    if process.returncode != 0:
        print "+++FAIL: Build failed dir=[%s]" % dir
        if data is not None:
            print data

    process = subprocess.Popen(['make', cflags], cwd=dir, 
        stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    data = process.communicate()[0]
    if process.returncode != 0:
        print "+++FAIL: Build failed dir=[%s]" % dir
        if data is not None:
            print data
    
    return (process.returncode, data)

def generate_rci_code(dir, device_type, firmware_version, 
            hostname, username, password, jar, keystore):
    rci_configs = [ f for f in os.listdir(dir) if f.endswith('.rci') ]

    abs_dir = os.path.abspath(dir)
    
    if not len(rci_configs) > 0:
        return (0, '')

    rci_config = os.path.join(abs_dir, rci_configs[0])

    print '>>> Generating RCI Config Code for %s.' % (rci_config)
    args = ['java', 
                '-jar', jar, 
                '-path=%s' % abs_dir,
                '-url=%s' % hostname,
                '%s:%s' % (username, password),
                device_type, firmware_version, rci_config]

    if keystore is not None:
        args.insert(1, '-Djavax.net.ssl.trustStore=%s' % keystore)

    process = subprocess.Popen(args, cwd=abs_dir, 
            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

    data = process.communicate()[0]

    if process.returncode != 0:
        print "+++FAIL: ConfigGenerator failed dir=[%s]" % dir
        if data is not None:
            print data        

    return (process.returncode, data)

def generate_rci_global_header(dir, jar, keystore):
    
    abs_dir = os.path.abspath(dir)

    print '>>> Generating RCI global connector_api_remote.h in %s'% dir
    args = ['java', 
                '-jar', jar, 
                '-path=%s' % abs_dir,
                '-type=global_header']

    if keystore is not None:
        args.insert(1, '-Djavax.net.ssl.trustStore=%s' % keystore)

    process = subprocess.Popen(args, cwd=abs_dir, 
            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

    data = process.communicate()[0]

    if process.returncode != 0:
        print "+++FAIL: ConfigGenerator failed dir=[%s]" % dir
        if data is not None:
            print data        

    return (process.returncode, data)

def setup_platform(platform_dir, **device_config):
    config.remove_errors(os.path.join(platform_dir, 'config.c'))
    config.update_config_source(os.path.join(platform_dir, 'config.c'), **device_config)

def sandbox(directory):
    """
    Creates a temporary directory with contents of input directory.
    This can be used to compile code and execute tests in a separate
    place and will allow tests to run concurrently.
    """
    # Get a temporary directory name, then delete it.
    sandbox_directory = tempfile.mkdtemp(suffix='_ecc')
    # Copy tree requires destination to not exist.
    os.removedirs(sandbox_directory)
    shutil.copytree(directory, sandbox_directory, symlinks=True)
    return sandbox_directory

def get_template_dirs():
    sandbox_dir = sandbox('.')
    template_script_dir = os.path.join(sandbox_dir, TEMPLATE_SCRIPT_DIR)
    template_platform_dir = os.path.join(sandbox_dir, TEMPLATE_PLATFORM_DIR)
    template_test_dir = os.path.join(sandbox_dir, TEMPLATE_TEST_DIR)
    connector_config_path = os.path.join(template_test_dir, './connector_config.h')
    if not os.path.exists(connector_config_path):
        connector_config_path = os.path.join(sandbox_dir, 
            'public/include/connector_config.h')

    return (sandbox_dir, template_script_dir, template_platform_dir, template_test_dir, connector_config_path)

def get_library_dirs():
    sandbox_dir = sandbox('.')
    library_script_dir = os.path.join(sandbox_dir, LIBRARY_SCRIPT_DIR)
    library_test_dir = os.path.join(sandbox_dir, LIBRARY_TEST_DIR)

    return (sandbox_dir, library_script_dir, library_test_dir)
