#!/bin/bash
pushd $(dirname $0)
BASEDIR=$(pwd)
export DSTDIR="${BASEDIR}/../out/"

./build_mac_cui.sh || exit 1
./build_mac_gui.sh || exit 1

cp -rf ../LICENSE ${DSTDIR}
