#!/bin/bash -x
pushd $(dirname $0)
BASEDIR=$(pwd)
SRCDIR="${BASEDIR}/../gui/"
DSTDIR="${BASEDIR}/../out/"
echo "SRCDIR : ${SRCDIR}"
echo "DSTDIR : ${DSTDIR}"

if [ ! -z ${QT_PATH+x} ]; then
    # use environment variable
    QT_PATH=${QT_PATH}
elif [ -d /usr/local/opt/qt ]; then
    # use homebrew
    QT_PATH="/usr/local/opt/qt"
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

pushd ${SRCDIR}

mkdir build
pushd build 

cmake -DCMAKE_PROJECT_NAME="PSDtoSS6GUI" .. || exit 1
make clean
make || exit 1

mkdir -p ${DSTDIR}
cp -rf *.app ${DSTDIR}

cp -rf ../translate_ja.qm ${DSTDIR}

macdeployqt "${DSTDIR}/PSDtoSS6GUI.app" -always-overwrite || exit 1
