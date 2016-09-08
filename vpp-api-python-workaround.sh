#!/bin/sh

VPP_ROOT=`git rev-parse --show-toplevel`
OUT="/usr/lib/python2.7/site-packages/vpp_papi-1.1-py2.7.egg"

cp -vf ./build-root/install-vpp_debug-native/vpp-api/lib64/*.so* $OUT
cp -vf ./build-root/install-vpp_debug-native/vpp/vpp-api/vpe.py $OUT
cp -vf ./build-root/install-vpp_debug-native/vlib-api/vlibmemory/memclnt.py $OUT
cp -vf ./build-root/install-vpp_debug-native/plugins/vpp_papi_plugins/portmirroring.py $OUT

