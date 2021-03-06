#!/bin/bash

echo "apt-get installing: clang git unzip wget libc++-dev binutils make automake libtool curl cmake subversion"
sudo apt-get install clang git unzip wget libc++-dev binutils make automake libtool curl cmake subversion

mkdir -p deps
cd deps

git clone "https://github.com/google/protobuf.git" --depth 1 -b "v3.0.0-alpha-1"
pushd protobuf
git am "../../documentation/Setup Files/protobuf.patch"
./autogen.sh
./autogen.sh # Makefile.in ends up being missing unless we run this twice??
./configure CC=clang CXX=clang++ CXXFLAGS='-fPIC -m64 -std=c++11 -stdlib=libc++ -O3 -g' LDFLAGS='-stdlib=libc++'
make -j 8

popd
git clone https://github.com/Norgg/glog
pushd glog
./configure CC=clang CXX=clang++ CXXFLAGS='-fPIC -m64 -std=c++11 -stdlib=libc++ -O3 -g' LDFLAGS='-stdlib=libc++'
make -j 8

popd
svn checkout http://googlemock.googlecode.com/svn/trunk/ gmock
svn checkout http://googletest.googlecode.com/svn/trunk/ gtest
pushd gtest
wget "https://gist.githubusercontent.com/Norgg/241ee11d278c0a55cc96/raw/4b23a866c6631ba0077229be366e67cde18fb035/gtest_linux_thread_count.patch" -O thread_count.patch
patch -p 0 -i thread_count.patch

popd
pushd gmock
patch -p 1 -i "../../documentation/Setup Files/gmock.patch"; true
