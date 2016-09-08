#!/bin/sh

VPP_ROOT=`git rev-parse --show-toplevel`
OUT="/usr/local/lib/python2.7/dist-packages/vpp_papi-1.1-py2.7.egg"

cp -vf ./build-root/install-vpp_debug-native/vpp-api/lib64/*.so* $OUT
cp -vf ./build-root/install-vpp-native/vpp/vpp-api/vpe.py $OUT
cp -vf ./build-root/install-vpp-native/vlib-api/vlibmemory/memclnt.py $OUT

