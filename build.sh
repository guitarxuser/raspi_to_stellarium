#!/bin/sh

#build

cd rot_enc
make clean
make
cd ../telescope_server/src
make clean
make
cd ../../tty0tty/pts
make clean
make
cd ../..

#install within /usr/local/bin with    sudo sh install.sh



