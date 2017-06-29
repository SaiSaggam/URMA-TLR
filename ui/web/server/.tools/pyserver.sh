#!/bin/bash
# Sample script file for running the python web server on the device.
# Notice that you can pass additional command line arguments through.
cd /home/digi/user/device_dist
export PYTHON_ENV='production'
export CONFIG_MANAGER_API_TOKEN='!digi#ddpv1!'
export 'WEB_PORT=80'
python server.py "$@"
