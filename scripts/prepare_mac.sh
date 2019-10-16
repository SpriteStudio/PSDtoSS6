SCRIPTDIR=`dirname $0`
SCRIPTDIR=`cd $SCRIPTDIR && pwd -P`
BASEDIR=${SCRIPTDIR}/..
BASEDIR=`cd ${BASEDIR} && pwd -P`
BUILDDIR=${BASEDIR}/
BUILDDIR=`cd ${BUILDDIR} && pwd -P`

pushd ${BUILDDIR}
cd ~

git clone https://github.com/Microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.sh

set VCPKG_DEFAULT_TRIPLET=x64-osx

cd vcpkg
./vcpkg install tinyxml2
./vcpkg install zlib
./vcpkg install libpng
./vcpkg install libjpeg-turbo
./vcpkg install icu
./vcpkg integrate install


popd > /dev/null # 


