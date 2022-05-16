#!/bin/bash -ex

SCRIPTDIR=`dirname $0`
SCRIPTDIR=`cd $SCRIPTDIR && pwd -P`
BASEDIR=${SCRIPTDIR}/..
BASEDIR=`cd ${BASEDIR} && pwd -P`

SRCDIR="${BASEDIR}/cui"
DSTDIR="${BASEDIR}/out"

if [ -z ${VCPKG_PREFIX+x} ]; then
   if [ -d ${BASEDIR}/vcpkg ]; then
      VCPKG_PREFIX=${BASEDIR}/vcpkg
   fi
fi

pushd ${BASEDIR} > /dev/null

echo "SRCDIR : ${SRCDIR}"
echo "DSTDIR : ${DSTDIR}"

PROJECT_NAME="PSDtoSS6"

pushd "${SRCDIR}"

/bin/rm -rf build
/bin/mkdir build
BUILDDIR=${SRCDIR}/build
BUILDDIR=`cd ${BUILDDIR} && pwd -P`

pushd ${BUILDDIR}

conan install .. 
cmake -DCMAKE_PROJECT_NAME=${PROJECT_NAME} .. || exit 1

cmake --build . || exit 1
popd > /dev/null # build

popd > /dev/null # SRCDIR

/bin/mkdir -p ${DSTDIR}
/bin/cp -rf ${BUILDDIR}/${PROJECT_NAME} ${DSTDIR}

popd > /dev/null # BASEDIR