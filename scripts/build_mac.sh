#!/bin/bash -ex

pushd $(dirname $0)
BASEDIR=$(pwd)
DSTDIR="${BASEDIR}/../out/"
/bin/rm -rf ${DSTDIR}
/bin/mkdir ${DSTDIR}

./build_mac_cui.sh || exit 1
./build_mac_gui.sh || exit 1

cp -rf ../LICENSE ${DSTDIR}
