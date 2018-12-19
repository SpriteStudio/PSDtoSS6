#!/bin/bash
pushd $(dirname $0)
BASEDIR=$(pwd)
${SRCDIR:="${BASEDIR}/../cui/"}
${DSTDIR:="${BASEDIR}/../out/"}
echo "SRCDIR : ${SRCDIR}"
echo "DSTDIR : ${DSTDIR}"

PROJECT_NAME="PSDtoSS6"

pushd "${SRCDIR}"

mkdir build
pushd build

cmake -DCMAKE_PROJECT_NAME=${PROJECT_NAME} .. || exit 1
make clean
make || exit 1

mkdir -p ${DSTDIR}
cp -rf ${PROJECT_NAME} ${DSTDIR}