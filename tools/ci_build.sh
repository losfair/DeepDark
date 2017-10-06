#!/bin/bash

export CC=gcc-6
export CXX=g++-6
apt-get install -y build-essential autoconf libtool libgflags-dev libgtest-dev clang libc++-dev
git clone https://github.com/grpc/grpc
cd grpc
git submodule update --init
make -j5
make install
cd third_party/protobuf
make install
cd ../..
cd ..
mkdir target
make
