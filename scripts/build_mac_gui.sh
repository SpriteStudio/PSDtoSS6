#!/bin/bash
pushd $(dirname $0)
BASEDIR=$(pwd)
${SRCDIR:="${BASEDIR}/../gui/"}
${DSTDIR:="${BASEDIR}/../out/"}
echo "SRCDIR : ${SRCDIR}"
echo "DSTDIR : ${DSTDIR}"

QT_PATH="/Users/username/Qt5.9.7/5.9.7/clang_64/bin/"
PATH="${QT_PATH}:${PATH}"

pushd ${SRCDIR}

mkdir build
pushd build 

cmake -DCMAKE_PROJECT_NAME="PSDtoSS6GUI" .. || exit 1
make clean
make || exit 1

mkdir -p ${DSTDIR}
cp -rf *.app ${DSTDIR}

macdeployqt "${DSTDIR}/PSDtoSS6GUI.app" -always-overwrite || exit 1
