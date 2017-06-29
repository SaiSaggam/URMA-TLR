#!/bin/bash

mkdir cc_testharness/source
ln -s `pwd` cc_testharness/source/cc_ansic
ln -s `pwd`/cc_testcases cc_testharness/source/cc_testcases
cd cc_testharness
./install_unattended.sh
