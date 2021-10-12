#!/bin/bash -ex

SCRIPTDIR=`dirname $0`
SCRIPTDIR=`cd $SCRIPTDIR && pwd -P`
BASEDIR=${SCRIPTDIR}/..
BASEDIR=`cd ${BASEDIR} && pwd -P`

QT_PATH="/Users/yoshida/Qt/5.15.2/clang_64"

SRCDIR="${BASEDIR}/gui"
DSTDIR="${BASEDIR}/out"

if [ -z ${VCPKG_PREFIX+x} ]; then
   if [ -d ${BASEDIR}/vcpkg ]; then
      VCPKG_PREFIX=${BASEDIR}/vcpkg
   fi
fi

if [ ! -z ${QT_PATH+x} ]; then
    # use environment variable
    QT_PATH=${QT_PATH}
elif [ -d /usr/local/opt/qt@5 ]; then
    # use homebrew
    QT_PATH="/usr/local/opt/qt@5"
else 
    # search latest Qt
    if [ ! -d ${HOME}/Qt ]; then
        echo "not install Qt"
        exit 1
    fi

    LATEST_VERSION=`/bin/ls ${HOME}/Qt | egrep '[0-9].*\.[0-9].*' | sort -r | head -n 1`
    if [ -n ${LATEST_VERSION} ]; then
        QT_PATH=${HOME}/Qt/${LATEST_VERSION}/clang_64
        echo ${QT_PATH}
    else
        # not install QT
        echo "not found Qt version from ${HOME}/Qt"
        exit 1
    fi
fi
PATH="${QT_PATH}/bin:${PATH}"

pushd ${BASEDIR} > /dev/null
echo "SRCDIR : ${SRCDIR}"
echo "DSTDIR : ${DSTDIR}"

pushd ${SRCDIR} > /dev/null

/bin/rm -rf build
/bin/mkdir build
BUILDDIR=${SRCDIR}/build
BUILDDIR=`cd ${BUILDDIR} && pwd -P`

pushd ${BUILDDIR} > /dev/null

cmake -DCMAKE_PROJECT_NAME="PSDtoSS6GUI" .. || exit 1
cmake --build . || exit 1
popd > /dev/null # build
popd > /dev/null # SRCDIR

/bin/mkdir -p ${DSTDIR}
/bin/cp -rf ${BUILDDIR}/*.app ${DSTDIR}
/bin/cp -rf ${SRCDIR}/translate_ja.qm ${DSTDIR}/PSDtoSS6GUI.app/Contents/MacOS
/bin/cp -rf ${SRCDIR}/translate_en.qm ${DSTDIR}/PSDtoSS6GUI.app/Contents/MacOS

macdeployqt "${DSTDIR}/PSDtoSS6GUI.app" -always-overwrite || exit 1

popd > /dev/null # BASEDIR
