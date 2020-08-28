#!/bin/bash -ex

SCRIPTDIR=`dirname $0`
SCRIPTDIR=`cd $SCRIPTDIR && pwd -P`
BASEDIR=${SCRIPTDIR}/..
BASEDIR=`cd ${BASEDIR} && pwd -P`
BUILDDIR=${BASEDIR}/
BUILDDIR=`cd ${BUILDDIR} && pwd -P`

pushd ${BASEDIR}

#/bin/rm -rf vcpkg
#git clone https://github.com/Microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.sh

if [ -z ${VCPKG_DEFAULT_TRIPLET+x} ]; then
   export VCPKG_DEFAULT_TRIPLET=x64-osx
fi

pushd vcpkg > /dev/null
./vcpkg install tinyxml2
./vcpkg install zlib
./vcpkg install libpng
./vcpkg install libjpeg-turbo
./vcpkg install icu
./vcpkg integrate install
popd > /dev/null

popd > /dev/null # 


